#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	char** tokens;
	size_t capacity;
	size_t size;
	char* init_str;	
}tokenizer_t;

typedef struct
{
	char* lable_name;
	uint32_t addr;
}lable;

// typedef struct{
// 	char* input;
// 	char* output;
// 	char** oth_flags;
// }cml_t;

// typedef enum{
// 	COM,
// 	OUTPUT,
// 	INPUT
// }cl_exp_t;

typedef enum
{
	OP_UNKNOWN,
	OP_PSH,
	OP_POP,
	OP_ADD,
	OP_SUB,
	OP_DIV,
	OP_MUL,
	OP_RSH,
	OP_LSH
}token_t;


// void parse_commands(char** argv, int argc, cml_t *tkn)
// {
// 	cl_exp_t expect = COM;
// 	int counter = 1;
// 	while(counter < argc)
// 	{
// 		if(!strcmp(argv[counter], "-o"))
// 		{

// 		}
// 	}
// }


void push_tkn(tokenizer_t* tknzr, char* tkn)
{
	if(tknzr->size == tknzr->capacity-1)
	{
		tknzr->capacity *= 2;
		tknzr->tokens = realloc(tknzr->tokens, tknzr->capacity*sizeof(char*));
	}
	size_t size = strlen(tkn);
	tknzr->tokens[tknzr->size] = malloc(size+1);

	strncpy(tknzr->tokens[tknzr->size], tkn, size);
	tknzr->tokens[tknzr->size][size] = '\0';
	tknzr->size++;
	return;

}

tokenizer_t init_tkn(char* init_str)
{
	tokenizer_t tknzr = {.tokens = NULL, .capacity = 256, .size = 0, .init_str = init_str};
	tknzr.tokens = malloc(tknzr.capacity*sizeof(char*));
	return tknzr;
}

void free_tkn(tokenizer_t* tkn)
{
	free(tkn->init_str);
	for(int i = 0; i < tkn->size; i++)
	{
		free(tkn->tokens[i]);
	}
	free(tkn->tokens);
}

void tokenize(tokenizer_t* tknzr)
{
	char* start = tknzr->init_str;
	while(*start)
	{
		
		while(*start && isspace(*start)){start++;}
		if(!*start) break;

		char* end = start;
		while(*end && !isspace(*end)) {end++;}
		int cnt = end - start;
		
		if(cnt == 0) continue;
		char token[cnt+1];
		strncpy(token, start, cnt);
		token[cnt] = '\0';
		
		push_tkn(tknzr, token);
		start = end;
	}
}

int only_digits(char* str)
{
	while(*str)
	{ 
		if(!isdigit(*str))
		{
			return 0;
		} 
		str++;
	}
	return 1;	
}

int output_str(uint8_t** output, tokenizer_t *tknzr)
{
	char** tkn_str = tknzr->tokens;
	int size_of_str = tknzr->size;
	size_t i = 0;
	size_t size_for_out = 256;
	*output = calloc(size_for_out, sizeof(uint8_t));
	uint8_t* ptr = *output;

	while(i < size_of_str)
	{
		char* tkn = tkn_str[i];
		if(!strcmp(tkn, "PSH"))
		{
			if((ptr - *output) >= size_for_out - 1)
			{
				size_for_out *= 2;
				output = realloc(*output, size_for_out);
			}
			*ptr++ = OP_PSH;
			i++;
			continue;
		}
		if(!strcmp(tkn, "POP"))
		{
			if((ptr - *output) >= size_for_out - 1)
			{
				size_for_out *= 2;
				output = realloc(*output, size_for_out);
			}
			*ptr++ = OP_POP;
			i++;
			continue;
		}
		if(!strcmp(tkn, "ADD"))
		{
			if((ptr - *output) >= size_for_out - 1)
			{
				size_for_out *= 2;
				output = realloc(*output, size_for_out);
			}
			*ptr++ = OP_ADD;
			i++;
			continue;
		}
		if(!strcmp(tkn, "SUB"))
		{
			if((ptr - *output) >= size_for_out - 1)
			{
				size_for_out *= 2;
				output = realloc(*output, size_for_out);
			}
			*ptr++ = OP_SUB;
			i++;
			continue;
		}
		if(!strcmp(tkn, "MUL"))
		{
			if((ptr - *output) >= size_for_out - 1)
			{
				size_for_out *= 2;
				output = realloc(*output, size_for_out);
			}
			*ptr++ = OP_MUL;
			i++;
			continue;
		}
		if(!strcmp(tkn, "DIV"))
		{
			if((ptr - *output) >= size_for_out - 1)
			{
				size_for_out *= 2;
				output = realloc(*output, size_for_out);
			}
			*ptr++ = OP_DIV;
			i++;
			continue;
		}
		if(!strcmp(tkn, "RSH"))
		{
			if((ptr - *output) >= size_for_out - 1)
			{
				size_for_out *= 2;
				output = realloc(*output, size_for_out);
			}
			*ptr++ = OP_RSH;
			i++;
			continue;
		}
		if(!strcmp(tkn, "LSH"))
		{
			if((ptr - *output) >= size_for_out - 1)
			{
				size_for_out *= 2;
				output = realloc(*output, size_for_out);
			}
			*ptr++ = OP_LSH;
			i++;
			continue;
		}
		
		if(only_digits(tkn))
		{
			if((ptr - *output) >= size_for_out - 5)
			{
				size_for_out *= 2;
				output = realloc(*output, size_for_out);
			}
			int int_tkn = atoi(tkn);
			printf("TKN_NUM = %d\n", int_tkn);
			for(int k = 0; k < 4; k++)
			{
				*ptr = (int_tkn >> k*8) & 0xFF; 
			

				ptr++;
			}
			i++;
			continue;
		}

		printf("Invalid Operand/Opcode/Mnemonic %s", tkn_str[i]);
		free(*output);
		return 0;
	}
	for(int i = 0; i < ptr-*output; i++)
	{
		printf("0x%X\n", (*output)[i]);
	}
	return ptr - *output;
}

int main(int argc, char** argv)
{
	if(argc < 3)
	{
		printf("%s: fatal. No files provided. Usage:\n%s input output\n", argv[0], argv[0]);
		return 1;
	}
	int filesize = 0;
	FILE* input = fopen(argv[1], "r");
	while(fgetc(input) != EOF) filesize++;

	fseek(input, 0, SEEK_SET);
	char* init_str = malloc(filesize+1);
	size_t act_read = fread(init_str, 1, filesize, input);
	fclose(input);
	printf("FILESIZE IS %zu\n", act_read);
	init_str[act_read] = '\0';
	
	uint8_t *bytecode_res;
	tokenizer_t tknzr = init_tkn(init_str);
	tokenize(&tknzr);

	for(int i = 0; i < tknzr.size; i++)
	{
		printf("%s%s%s", i == 0 ? "[\"" : " \"", tknzr.tokens[i], i == tknzr.size-1 ? "\"]\n" : "\",");
	}
	size_t sizeofprog = output_str(&bytecode_res, &tknzr); 
	if(!sizeofprog)
	{
		free_tkn(&tknzr);
		return 1;
	}
	for(int i = 0; i < sizeofprog; i++)
	{
		printf("0x%X\n", bytecode_res[i]);
	}
	FILE* output = fopen(argv[2], "wb");
	fwrite(bytecode_res, 1, sizeofprog, output);
	fclose(output);
	return 0;
}
