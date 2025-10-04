
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "tokenizing.h"
#include "asmgen.h"
#include "utils.h"



void free_tkn(tokenizer_t* tkn)
{
      if(tkn->init_str)
      {
	free(tkn->init_str);
	tkn->init_str = NULL;
      }
      if(tkn->tokens)
      {
	free(tkn->tokens);
        tkn->tokens = NULL;
      }
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
	tokenizer_t tknzr = {.tokens = NULL, .capacity = 16, .size = 0, .init_str = init_str};
	// printf("I fell. Asked for %zu bytes and fell somehow.\n", tknzr.capacity*sizeof(token_t));
	tknzr.tokens = calloc(tknzr.capacity, sizeof(token_t));
	if (tknzr.tokens) {
        memset(tknzr.tokens, 0, tknzr.capacity * sizeof(token_t));
    }
	return tknzr;
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
		if(cnt >= 32)
		{
			printf("Tokens this big are not allowed!\n");
			free_tkn(tknzr);
			exit(1);
		}
		char token[32] = {0};
		
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
	free(tknzr->init_str);
}
