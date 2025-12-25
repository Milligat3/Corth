
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "token.h"
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
	token_table_t tt;
	tt = tknzr->tt;
	tknzr->tt.tokens = NULL;
	return tt;
}


void push_str(tokenizer_t* tknzr, char* tkn)
{
	size_t size;
	if(tknzr->tt.size == tknzr->tt.cap-1)
	{
		tknzr->tt.cap *= 2;
		tknzr->tt.tokens = realloc(tknzr->tt.tokens, tknzr->tt.cap*sizeof(token_t));
	}
	size = strlen(tkn);

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
	token_table_t to_ret;

	to_ret.cap = 16;
	to_ret.size = 0;
	to_ret.tokens = malloc(to_ret.cap*sizeof(token_t));

	return to_ret; 
}

tokenizer_t init_tkn(char* init_str)
{
	tokenizer_t tknzr;
	tknzr.init_str = init_str;
	tknzr.tt = init_tt();
	return tknzr;
}


void tokenize(tokenizer_t* tknzr)
{
	char* start;
	char* end;
	int cnt;
	char token[32];
	char* start_asm;
	char* end_asm;
	start = tknzr->init_str;
	while(*start)
	{
		
		while(*start && isspace(*start)){start++;}
		if(!*start) break;

		end = start;
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
		cnt = end - start;
		if(cnt >= 32)
		{
			printf("Tokens this big are not allowed!\n");
			free_tkn(tknzr);
			exit(1);
		}
		
		
		strncpy(token, start, cnt);
		token[cnt] = '\0';
		
		if(!strcmp(token, "ASM"))
		{
			start_asm = end;
			end_asm = parse_asm(tknzr, start_asm);
			
			start = end_asm+1;
			continue;
		}

		start = end;
		push_str(tknzr, token);
	}
	free(tknzr->init_str);
}
