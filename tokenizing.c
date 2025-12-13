
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
      if(tkn->tt.tokens)
      {
		free(tkn->tt.tokens);
      	tkn->tt.tokens = NULL;
      }
}

token_table_t detach_tt(tokenizer_t *tknzr)
{
	token_table_t tt = tknzr->tt;
	tknzr->tt.tokens = NULL;
	return tt;
}


void push_str(tokenizer_t* tknzr, char* tkn)
{
	if(tknzr->tt.size == tknzr->tt.cap-1)
	{
		tknzr->tt.cap *= 2;
		tknzr->tt.tokens = realloc(tknzr->tt.tokens, tknzr->tt.cap*sizeof(token_t));
	}
	size_t size = strlen(tkn);

	if(size > 32)
	{
		free_tkn(tknzr);
		printf("Size of token %s is bigger than 32. Exit.\n", tkn);
		exit(1);
	}

	strncpy(tknzr->tt.tokens[tknzr->tt.size].tkn_str, tkn, size);
	tknzr->tt.tokens[tknzr->tt.size++].tkn_str[size] = '\0';
}


void push_token_tknzr(tokenizer_t* tknzr, token_t tkn)
{
	push_token(&tknzr->tt, tkn);
}

void push_token(token_table_t* tt, token_t tkn)
{
	if(tt->size == tt->cap-1)
	{
		tt->cap *= 2;
		tt->tokens = realloc(tt->tokens, tt->cap*sizeof(token_t));
	}
	tt->tokens[tt->size++] = tkn;

}


token_table_t init_tt(void)
{
	token_table_t to_ret = {.cap = 16, .size = 0};
	to_ret.tokens = malloc(to_ret.cap*sizeof(token_t));

	return to_ret; 
}

tokenizer_t init_tkn(char* init_str)
{
	tokenizer_t tknzr = {.init_str = init_str};
	tknzr.tt = init_tt();
	// printf("I fell. Asked for %zu bytes and fell somehow.\n", tknzr.cap*sizeof(token_t));
	
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
		// tknzr->tt.init_str = start;
		push_str(tknzr, token);
	}
	free(tknzr->init_str);
}
