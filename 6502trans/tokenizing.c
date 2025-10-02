
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "tokenizing.h"
#include "asmgen.h"




void free_tkn(tokenizer_t* tkn)
{
	free(tkn->init_str);
	free(tkn->tokens);
}

void push_str(tokenizer_t* tknzr, char* tkn)
{
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
	tknzr->tokens[tknzr->size++].tkn_str[size] = '\0';
}


void push_token(tokenizer_t* tknzr, token_t tkn)
{
	if(tknzr->size == tknzr->capacity-1)
	{
		tknzr->capacity *= 2;
		tknzr->tokens = realloc(tknzr->tokens, tknzr->capacity*sizeof(token_t));
	}
	tknzr->tokens[tknzr->size++] = tkn;

}

tokenizer_t init_tkn(char* init_str)
{
	tokenizer_t tknzr = {.tokens = NULL, .capacity = 256, .size = 0, .init_str = init_str};
	tknzr.tokens = malloc(tknzr.capacity*sizeof(token_t));
	return tknzr;
}

void strip_token(char** token)
{
	if(**token == '(')
	{
		(*token)++;
	}
	if((*token)[strlen(*token) - 1] == ',' || (*token)[strlen(*token) - 1] == ')')
	{
		(*token)[strlen(*token) - 1] = '\0';
	}
}

int is_delim(char n)
{
	return n == '{' || n == '}' || n == '(' || n == ')' || n == ',';
}

void tokenize(tokenizer_t* tknzr)
{
	char* start = tknzr->init_str;
	// char *ptr = start;
	while(*start)
	{
		
		while(*start && isspace(*start)){start++;}
		if(!*start) break;

		char* end = start;
		while(*end && !isspace(*end))
		{
			if(is_delim(*start))
			{
				end++;
				break;
			}
			end++;
			if(is_delim(*end))
			{
				break;
			}
		}
		int cnt = end - start;
		
		char* token = malloc(cnt+1);
		strncpy(token, start, cnt);
		token[cnt] = '\0';
		// strip_token(&token);
		// if(cnt == 0 || strlen(token) == 0 || *start == '\0')
		// {
		// 	start = end; 
		// 	continue;
		// }
		if(!strcmp(token, "ASM"))
		{
			char* start_asm = end;
			char* end_asm = parse_asm(tknzr, start_asm);
			
			start = end_asm+1;
			continue;
		}

		start = end;
		// tknzr->init_str = start;
		push_str(tknzr, token);
	}
}
