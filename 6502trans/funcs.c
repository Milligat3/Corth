// // funcs.c

// #include "tokenizing.h"
// #include <stddef.h>
// #include <string.h>
// #include <stdlib.h>
// #include <stdio.h>

// func_table_t init_funcs(void)
// {
// 	func_table_t ret = {.count = 0, .cap = 16};
// 	ret.funcs = malloc(sizeof(func_macro_t) * ret.cap);
// 	return ret;
// }


// void parse_function(tokenizer_t* tknzr, tokenizer_t ref_tok, size_t *i)
// {
// 	size_t depth = 0;
// 	token_t *start = &ref_tok.tokens[*i];
// 	token_t *end = NULL;
// 	for(size_t local_i = *i; local_i < ref_tok.size; local_i++)
// 	{
// 		token_t tkn = ref_tok.tokens[local_i];
// 		if(!strcmp(tkn.tkn_str, "@func_end"))
// 		{
// 			if(depth == 0)
// 			{
// 				printf("Extra @func_end detected.\n");
// 				free(tknzr->tokens);
// 				free(ref_tok.tokens);
// 				exit(1);
// 			}
// 			depth--;
// 			if(depth == 0)
// 			{
// 				printf("End of block.\n");
// 				end = &ref_tok.tokens[local_i];
// 				size_t block_size = local_i - *i;
// 				*i = local_i;
				
// 				return;
// 			}	
// 		}
// 		else if(!strcmp(tkn.tkn_str, "@func"))
// 		{
// 			depth++;
// 		}
// 	}
// }

// void parse_funcs(tokenizer_t *tknzr)
// {
// 	tokenizer_t ref_tok = *tknzr;
// 	*tknzr = init_tkn(NULL);
// 	func_table_t funcs = init_funcs();
// 	for(size_t i = 0; i < ref_tok.size; i++)
// 	{
// 		token_t tkn = ref_tok.tokens[i];
// 		if(!strcmp(tkn.tkn_str, "@func"))
// 		{
// 			parse_function(tknzr, ref_tok, &i);
// 		}
// 		else
// 		{
// 			push_token(tknzr, tkn);
// 		}
// 	}
// }