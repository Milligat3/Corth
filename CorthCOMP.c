#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "opcodes.h"

typedef struct{
	char** tokens;
	size_t capacity;
	size_t size;
	char* init_str;
}tokenizer_t;

typedef struct
{
	char lable_name[32];
	uint32_t addr;
}lable_t;

typedef struct
{
	lable_t lables[100];
	int size;
}lable_table_t;

lable_table_t labl_tabl = {0};
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

int jmp_in_lables(char* tkn)
{
	char* tkn_lbl = malloc(32);
	tkn_lbl = strdup(tkn);
	
	for(int i = 0; i < labl_tabl.size; i++)
	{

		if(!strcmp(labl_tabl.lables[i].lable_name, tkn_lbl))
		{
			return i;
		}
	}
	return -1;
}

int lable_in_lables(char* tkn)
{
	char* tkn_lbl = malloc(32);
	tkn_lbl = strdup(tkn);
	if(tkn_lbl[strlen(tkn_lbl)-1] == ':')
		tkn_lbl[strlen(tkn_lbl)-1] = '\0';
	for(int i = 0; i < labl_tabl.size; i++)
	{

		if(!strcmp(labl_tabl.lables[i].lable_name, tkn_lbl))
		{
			return i;
		}
	}
	return -1;
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
			char* next_token = tkn_str[i+1];
			if(!only_digits(next_token))
			{
				printf("%s is not a number, aborting\n", next_token);
				free(*output);
				return 0;
			}
			if((ptr - *output) >= size_for_out - 5)
			{
				size_for_out *= 2;
				output = realloc(*output, size_for_out);
			}
			int int_tkn = atoi(next_token);
			printf("TKN_NUM = %d\n", int_tkn);
			for(int k = 0; k < 4; k++)
			{
				*ptr =  int_tkn & 0xFF;
				int_tkn >>= 8;			
				ptr++;
			}
			i += 2;
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
		if(!strcmp(tkn, "JMP"))
		{
			if((ptr - *output) >= size_for_out - 5)
			{
				size_for_out *= 2;
				output = realloc(*output, size_for_out);
			}
			*ptr++ = OP_JMP;
			char* next_token = tknzr->tokens[i+1]; 
			int idx = jmp_in_lables(next_token);
			if(idx == -1)
			{
				printf("%s is not a valid lable, aborting\n", next_token);
				free(*output);
				return 0;
			}
			uint32_t addr = labl_tabl.lables[idx].addr;
			
			printf("ADDR = %d\n", addr);
			for(int k = 0; k < 4; k++)
			{
				*ptr = addr & 0xFF;
				addr >>= 8;
				ptr++;
			}
			
			i += 2;
			continue;
		}
		if(!strcmp(tkn, "JNZ"))
		{
			if((ptr - *output) >= size_for_out - 5)
			{
				size_for_out *= 2;
				output = realloc(*output, size_for_out);
			}
			*ptr++ = OP_JNZ;
			char* next_token = tknzr->tokens[i+1]; 
			int idx = jmp_in_lables(next_token);
			if(idx == -1)
			{
				printf("%s is not a valid lable, aborting\n", next_token);
				free(*output);
				return 0;
			}
			uint32_t addr = labl_tabl.lables[idx].addr;
			
			printf("ADDR = %d\n", addr);
			for(int k = 0; k < 4; k++)
			{
				*ptr = addr & 0xFF;
				addr >>= 8;
				ptr++;
			}
			
			i += 2;
			continue;
		}
		if(!strcmp(tkn, "JZ"))
		{
			if((ptr - *output) >= size_for_out - 5)
			{
				size_for_out *= 2;
				output = realloc(*output, size_for_out);
			}
			*ptr++ = OP_JZ;
			char* next_token = tknzr->tokens[i+1]; 
			int idx = jmp_in_lables(next_token);
			if(idx == -1)
			{
				printf("%s is not a valid lable, aborting\n", next_token);
				free(*output);
				return 0;
			}
			uint32_t addr = labl_tabl.lables[idx].addr;
			
			printf("ADDR = %d\n", addr);
			for(int k = 0; k < 4; k++)
			{
				*ptr = addr & 0xFF;
				addr >>= 8;
				ptr++;
			}
			
			i += 2;
			continue;
		}
		if(!strcmp(tkn, "DUP"))
		{
			if((ptr - *output) >= size_for_out - 1)
			{
				size_for_out *= 2;
				output = realloc(*output, size_for_out);
			}
			*ptr++ = OP_DUP;
			i++;
			continue;
		}
		if(!strcmp(tkn, "SWP"))
		{
			if((ptr - *output) >= size_for_out - 1)
			{
				size_for_out *= 2;
				output = realloc(*output, size_for_out);
			}
			*ptr++ = OP_SWP;
			i++;
			continue;
		}
		if(!strcmp(tkn, "INC"))
		{
			if((ptr - *output) >= size_for_out - 1)
			{
				size_for_out *= 2;
				output = realloc(*output, size_for_out);
			}
			*ptr++ = OP_INC;
			i++;
			continue;
		}
		if(!strcmp(tkn, "DEC"))
		{
			if((ptr - *output) >= size_for_out - 1)
			{
				size_for_out *= 2;
				output = realloc(*output, size_for_out);
			}
			*ptr++ = OP_DEC;
			i++;
			continue;
		}
		if(!strcmp(tkn, "PEK"))
		{
			if((ptr - *output) >= size_for_out - 1)
			{
				size_for_out *= 2;
				output = realloc(*output, size_for_out);
			}
			*ptr++ = OP_PEK;
			i++;
			continue;
		}
		if(lable_in_lables(tkn) != -1)
		{
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

void find_lables(tokenizer_t *tknzr)
{
	uint32_t addr = 0;
	for(int idx = 0; idx < tknzr->size; idx++)
	{
		char* tkn = tknzr->tokens[idx];
		if(tkn[strlen(tkn)-1] == ':')
		{
			strcpy(labl_tabl.lables[labl_tabl.size].lable_name, tkn);
			labl_tabl.lables[labl_tabl.size].lable_name[strlen(labl_tabl.lables[labl_tabl.size].lable_name)-1] = '\0'; 
			labl_tabl.lables[labl_tabl.size].addr = addr;
			labl_tabl.size++;
		}
		else if(!strcmp(tkn, "PSH"))
		{
			addr += 5;
			idx++;
		}else if(!strcmp(tkn, "JMP") || !strcmp(tkn, "JNZ") || !strcmp(tkn, "JZ"))
		{
			addr += 5;
			idx++;
		}
		else
		{
			addr++;
		}

	}
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
	char* ptr = init_str;
	
	while(*ptr)
	{
		*ptr = toupper(*ptr);
		ptr++;
	}

	tokenizer_t tknzr = init_tkn(init_str);
	tokenize(&tknzr);

	for(int i = 0; i < tknzr.size; i++)
	{
		printf("%s%s%s", i == 0 ? "[\"" : " \"", tknzr.tokens[i], i == tknzr.size-1 ? "\"]\n" : "\",");
	}
	find_lables(&tknzr);
	for(int i = 0; i < labl_tabl.size; i++)
	{
		printf("%s%s%s", i == 0 ? "[\"" : " \"", labl_tabl.lables[i].lable_name, i == labl_tabl.size-1 ? "\"]\n" : "\",");
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
