#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexical_analysys.h"
#include "tokenizing.h"

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

int is_hex(char* str)
{
	if(str[0] != '$')
		return 0;
	if(strlen(str) < 2)
		return 0;
	str++;
	while(*str)
	{
		if((*str >= '0' && *str <= '9') || (*str >= 'A' && *str <= 'F'))
		{
			str++;
			continue;
		}
		return 0;
	}
	return 1;	
}


void push_str_tkn(tokenizer_t* tknzr, token_t tkn_t)
{
	char* tkn = tkn_t.tkn_str;
	if(tknzr->size == tknzr->capacity-1)
	{
		tknzr->capacity *= 2;
		tknzr->tokens = realloc(tknzr->tokens, tknzr->capacity*sizeof(token_t));
	}
	size_t size = strlen(tkn);
	if(size > 32)
	{
		free_tkn(tknzr);
		printf("Size of token %s is bigger than 32. Exit.\n", tkn);
		exit(1);
	}

	strncpy(tknzr->tokens[tknzr->size].tkn_str, tkn, size);
	tknzr->tokens[tknzr->size].tkn_str[size] = '\0';
	if(!strcmp(tkn, "PSH"))
	{
		tknzr->tokens[tknzr->size].type = TKN_PSH;
		goto inc;
	}
	if(!strcmp(tkn, "POP"))
	{
		tknzr->tokens[tknzr->size].type = TKN_POP;
		goto inc;
	}
	if(!strcmp(tkn, "ADD"))
	{
		tknzr->tokens[tknzr->size].type = TKN_ADD;
		goto inc;
	}
	if(!strcmp(tkn, "SUB"))
	{
		tknzr->tokens[tknzr->size].type = TKN_SUB;
		goto inc;
	}
	if(!strcmp(tkn, "RSH"))
	{
		tknzr->tokens[tknzr->size].type = TKN_RSH;
		goto inc;
	}
	if(!strcmp(tkn, "LSH"))
	{
		tknzr->tokens[tknzr->size].type = TKN_LSH;
		goto inc;
	}
	if(!strcmp(tkn, "JMP"))
	{
		tknzr->tokens[tknzr->size].type = TKN_JMP;
		goto inc;
	}
	if(!strcmp(tkn, "JNZ"))
	{
		tknzr->tokens[tknzr->size].type = TKN_JNZ;
		goto inc;
	}
	if(!strcmp(tkn, "JCC"))
	{
		tknzr->tokens[tknzr->size].type = TKN_JCC;
		goto inc;
	}
	if(!strcmp(tkn, "JCS"))
	{
		tknzr->tokens[tknzr->size].type = TKN_JCS;
		goto inc;
	}
	if(!strcmp(tkn, "JZ"))
	{
		tknzr->tokens[tknzr->size].type = TKN_JZ;
		goto inc;
	}
	if(!strcmp(tkn, "JSR"))
	{
		tknzr->tokens[tknzr->size].type = TKN_JSR;
		goto inc;
	}
	if(!strcmp(tkn, "RTS"))
	{
		tknzr->tokens[tknzr->size].type = TKN_RTS;
		goto inc;
	}
	if(!strcmp(tkn, "HLT"))
	{
		tknzr->tokens[tknzr->size].type = TKN_HLT;
		goto inc;
	}
	
	if(!strcmp(tkn, "DUP"))
	{
		tknzr->tokens[tknzr->size].type = TKN_DUP;
		goto inc;
	}
	if(!strcmp(tkn, "SWP"))
	{
		tknzr->tokens[tknzr->size].type = TKN_SWP;
		goto inc;
	}
	if(!strcmp(tkn, "INC"))
	{
		tknzr->tokens[tknzr->size].type = TKN_INC;
		goto inc;
	}
	if(!strcmp(tkn, "DEC"))
	{
		tknzr->tokens[tknzr->size].type = TKN_DEC;
		goto inc;
	}
	if(!strcmp(tkn, "WRD"))
	{
		tknzr->tokens[tknzr->size].type = TKN_WRD;
		goto inc;
	}
	if(!strcmp(tkn, "WRD_W"))
	{
		tknzr->tokens[tknzr->size].type = TKN_WRD_WORD;
		goto inc;
	}
	if(!strcmp(tkn, "RDD"))
	{
		tknzr->tokens[tknzr->size].type = TKN_RDD;
		goto inc;
	}
	if(!strcmp(tkn, "AND"))
	{
		tknzr->tokens[tknzr->size].type = TKN_AND;
		goto inc;
	}
	if(!strcmp(tkn, "OR"))
	{
		tknzr->tokens[tknzr->size].type = TKN_OR;
		goto inc;
	}
	if(!strcmp(tkn, "XOR"))
	{
		tknzr->tokens[tknzr->size].type = TKN_XOR;
		goto inc;
	}
	if(!strcmp(tkn, "GT"))
	{
		tknzr->tokens[tknzr->size].type = TKN_GT;
		goto inc;
	}
	if(!strcmp(tkn, "LT"))
	{
		tknzr->tokens[tknzr->size].type = TKN_LT;
		goto inc;
	}
	if(!strcmp(tkn, "EQ"))
	{
		tknzr->tokens[tknzr->size].type = TKN_EQ;
		goto inc;
	}
	if(!strcmp(tkn, "GE"))
	{
		tknzr->tokens[tknzr->size].type = TKN_GE;
		goto inc;
	}
	if(!strcmp(tkn, "LE"))
	{
		tknzr->tokens[tknzr->size].type = TKN_LE;
		goto inc;
	}
	if(!strcmp(tkn, "ROT"))
	{
		tknzr->tokens[tknzr->size].type = TKN_ROT;
		goto inc;
	}
	if(!strcmp(tkn, "OVR"))
	{
		tknzr->tokens[tknzr->size].type = TKN_OVER;
		goto inc;
	}
	if(!strcmp(tkn, "@DEF"))
	{
		tknzr->tokens[tknzr->size].type = TKN_DEF;
		goto inc;
	}
	if(!strcmp(tkn, "ASM"))
	{
		push_token(tknzr, tkn_t);
		return;
	}
	if(only_digits(tkn))
	{
		tknzr->tokens[tknzr->size].type = TKN_CONST;
		goto inc;
	}
	if(is_hex(tkn))
	{
		tkn++;
		unsigned long long int to_p = strtoul(tkn, NULL, 16);
		tkn = itoa(to_p, tkn, 10);
		size = strlen(tkn);
		strncpy(tknzr->tokens[tknzr->size].tkn_str, tkn, size);
		tknzr->tokens[tknzr->size].tkn_str[size] = '\0';
		tknzr->tokens[tknzr->size].type = TKN_CONST;
		goto inc;
	}
	if(tkn[strlen(tkn) - 1] == ':')
	{
		tknzr->tokens[tknzr->size].type = TKN_LABEL;
		goto inc;
	}
	tknzr->tokens[tknzr->size].type = TKN_UNKNOWN;
	inc:
	tknzr->size++;
	return;
}

void lex_anal(tokenizer_t *tknzr)
{
	tokenizer_t tknzr_tmp = init_tkn(NULL);
	for(size_t i = 0; i < tknzr->size; i++)
	{
		token_t tkn = tknzr->tokens[i];
		push_str_tkn(&tknzr_tmp, tkn);
	}
	free_tkn(tknzr);
	*tknzr = tknzr_tmp;
}