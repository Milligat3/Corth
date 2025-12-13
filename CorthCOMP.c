#include <stdlib.h>
#include <string.h>

#include "tokenizing.h"
#include "codgen.h"
#include "labels.h"
#include "optimize.h"
#include "syntax_checker.h"
#include "asmgen.h"
#include "preprocessor.h"
#include "lexical_analysys.h"

#define NOB_IMPLEMENTATION
#include "nob.h"

typedef struct{
	char* input;
	char* output;
	char* asm_output;
	char** flags_str;
}cml_t;

typedef enum{
	COM,
	OUTPUT,
	INPUT
}cl_exp_t;

char* strip_postfix_return_dot_s(char* file)
{
	char* s = malloc(strlen(file));

	strcpy(s, file);

	char* ptr = s + strlen(s);
	while(*ptr != '.' && ptr > s){ptr--;}
	ptr++;
	*ptr++ = 's';
	*ptr = '\0';
	return s;
}

void parse_commands(char** argv, int argc, cml_t *tkn)
{
	tkn->input = NULL;
	tkn->output = NULL;
	// cl_exp_t expect = COM;
	int counter = 1;
	while(counter < argc)
	{
		if(!strcmp(argv[counter], "-o"))
		{
			if(counter + 1 < argc && tkn->output == NULL)
			{
				counter++;
				tkn->output = argv[counter];
				counter++;
				continue;
			}
			else
			{
				nob_log(NOB_ERROR, "Double declaration of output or no output file specified\n");
				exit(1);
			}
		}
		if(tkn->input == NULL)
		{
			tkn->input = argv[counter++];
			continue;
		}
		else
		{
			nob_log(NOB_ERROR, "Double declaration of input or unrecognized option specified\n");
			counter++;
			continue;
		}
	}
	tkn->asm_output = strip_postfix_return_dot_s(tkn->output);
}



int main(int argc, char** argv)
{
	if(argc < 3)
	{
		nob_log(NOB_ERROR, "%s: fatal. No files provided. Usage:\n%s input output\n", argv[0], argv[0]);
		return 1;
	}
	cml_t cmd;
	parse_commands(argv, argc, &cmd);
	int filesize = 0;
	FILE* input = fopen(cmd.input, "r");
	while(fgetc(input) != EOF) filesize++;

	fseek(input, 0, SEEK_SET);
	char* init_str = malloc(filesize+1);
	size_t act_read = fread(init_str, 1, filesize, input);
	
	fclose(input);
	
	nob_log(NOB_INFO, "FILESIZE IS %zu\n", act_read);
	init_str[act_read] = '\0';
	
	char* ptr = init_str;
	
	while(*ptr)
	{
		*ptr = toupper(*ptr);
		ptr++;
	}
	init_asm_table();
	tokenizer_t tknzr_st = init_tkn(init_str);
	
	parse_macros(&tknzr_st);
	tokenize(&tknzr_st);

	nob_log(NOB_INFO, "Tokenizing complete.\n");
	for(size_t i = 0; i < tknzr_st.tt.size; i++)
	{
		printf("%s%s%s", i == 0 ? "[\"" : " \"", tknzr_st.tt.tokens[i].tkn_str, i == tknzr_st.tt.size-1 ? "\"]\n" : "\",");
	}
	preprocess(&tknzr_st);
	find_labels(&tknzr_st);
	token_table_t tknzr = detach_tt(&tknzr_st);

	nob_log(NOB_INFO, "Labels found\n");
	for(size_t i = 0; i < labl_tabl.size; i++)
	{
		printf("%s%s%s", i == 0 ? "[\"" : " \"", labl_tabl.labels[i].label_name, i == labl_tabl.size-1 ? "\"]\n" : "\",");
	}
	nob_log(NOB_INFO, "Tokens after preprocessing:\n");
	for(size_t i = 0; i < tknzr.size; i++)
	{
		printf("%s%s%s", i == 0 ? "[\"" : " \"", tknzr.tokens[i].tkn_str, i == tknzr.size-1 ? "\"]\n" : "\",");
	}
	lex_anal(&tknzr);
	if(check_syntax(&tknzr))
	{
		free(tknzr.tokens);
		return 1;
	}
	nob_log(NOB_INFO, "Syntax checked.\n");
	
	optimize(&tknzr);
	for(size_t i = 0; i < tknzr.size; i++)
	{
		printf("%s%d%s", i == 0 ? "[\"" : " \"", tknzr.tokens[i].type, i == tknzr.size-1 ? "\"]\n" : "\",");
	}
	// free_tkn(&tknzr2);

	FILE* output = fopen(cmd.asm_output, "w");
	size_t sizeofprog = output_str(output, &tknzr); 
	if(!sizeofprog)
	{
		free(tknzr.tokens);
		return 1;
	}
	nob_log(NOB_INFO, "Assembly generated.\n");
	free(tknzr.tokens);
	fclose(output);

	Nob_Cmd nob_cmd = {0};
	nob_cmd_append(&nob_cmd, "cl65", "-t", "none", "-o", cmd.output, cmd.asm_output);
	if(!nob_cmd_run_sync_and_reset(&nob_cmd))
	{
		nob_log(NOB_ERROR, "Something went terribly wrong\n");
		exit(1);
	}
	nob_log(NOB_INFO, "All good! Assmembly done.");
	//fwrite(bytecode_res, 1, sizeofprog, output);
	return 0;
}

