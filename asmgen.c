
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "asmgen.h"
#include "token.h"

asm_table_t asm_table = {0, 0, 256}; 

void push_asm(char* tkn)
{
	if(asm_table.size == asm_table.capacity - 1)
	{
		asm_table.capacity *= 2;
		asm_table.table = realloc(asm_table.table, asm_table.capacity*sizeof(char*));
	}
	asm_table.table[asm_table.size++] = tkn;
}

void init_asm_table(void)
{
	asm_table.table = malloc(asm_table.capacity * sizeof(char*));
}

char * parse_asm(tokenizer_t* tknzr, char* start_asm)
{
	char *end_asm;
	size_t size;
	char* asm_block;
	token_t asm_tkn;
	while(*start_asm && isspace(*start_asm)) { start_asm++; }
	if(*start_asm != '{')
	{
		printf("Wrong use of ASM block.\n Use it like this:\n ASM {LDA #$00}");
		free_tkn(tknzr);
		exit(1);
	}
	end_asm = ++start_asm;
	while(*end_asm != '}') { end_asm++; }
	size = end_asm - start_asm;
	asm_block = malloc((size+1)*sizeof(char));
	strncpy(asm_block, start_asm, size);
	asm_block[size] = '\0';
	strcpy(asm_tkn.tkn_str, "ASM");
	asm_tkn.type = TKN_ASM;
	asm_tkn.val.num[0] = asm_table.size;

	push_asm(asm_block);
	push_token_tknzr(tknzr, asm_tkn);
	printf("ASM BLOCK:\n %s\n", asm_block);
	return end_asm;
}