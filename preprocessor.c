

#include <ctype.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "preprocessor.h"
#include "tokenizing.h"
#include "utils.h"

// void push_token_into_macro(func_macro_t* macro, token_t tkn)
// {
// 	if(macro->tt.size == macro->tkn_cap-1)
// 	{
// 		macro->tkn_cap *= 2;
// 		macro->body = realloc(macro->body, macro->tkn_cap*sizeof(token_t));
// 	}
// 	macro->body[macro->tkn_count++] = tkn;

// }

preprocessor_t init_preproc(void)
{
	preprocessor_t to_ret = {.const_count = 0, .const_capacity = 256, .macro_count = 0, .macro_capacity = 16};
	to_ret.const_table = malloc(to_ret.const_capacity * sizeof(const_macro_t));
	to_ret.macro_table = malloc(to_ret.macro_capacity * sizeof(func_macro_t));
	return to_ret;
}

void push_macro(preprocessor_t *prep, func_macro_t fm)
{
	if(prep->macro_count == prep->macro_capacity - 1)
	{
		prep->macro_capacity *= 2;
		prep->macro_table = realloc(prep->macro_table, prep->macro_capacity * sizeof(func_macro_t));
	}
	for(size_t i = 0; i < prep->macro_count; i++)
	{
		if(!strcmp(prep->macro_table[i].name, fm.name))
		{
			printf("Redefinition of macro %s.\nIgnoring.\n", fm.name);
			return;
		}
	}
	prep->macro_table[prep->macro_count++] = fm;
}

void debug_macro(func_macro_t fm)
{
	printf("Name: %s\n", fm.name);
	printf("Args: %zu\n", fm.arg_count);
	for(size_t i = 0; i < fm.arg_count; i++)
	{
		printf("%s ", fm.args[i]);
	}
	putchar('\n');
	printf("Body:\n");
	for(size_t i = 0; i < fm.body.size; i++)
	{
		printf("%s%s%s", i == 0 ? "[\"" : " \"", fm.body.tokens[i].tkn_str, i == fm.body.size-1 ? "\"]\n" : "\",");
	}
}

void debug_tkns(tokenizer_t tknzr)
{
	for(size_t i = 0; i < tknzr.tt.size; i++)
	{
		printf("%s%s%s", i == 0 ? "[\"" : " \"", tknzr.tt.tokens[i].tkn_str, i == tknzr.tt.size-1 ? "\"]\n" : "\",");
	}
}

void debug_tkn_tbl(token_table_t tt)
{
	for(size_t i = 0; i < tt.size; i++)
	{
		printf("%s%s%s", i == 0 ? "[\"" : " \"", tt.tokens[i].tkn_str, i == tt.size-1 ? "\"]\n" : "\",");
	}
}

// char* get_slice(char* string, char* end)
// {

// 	return NULL;
// }

void tokenize_macro(tokenizer_t* tknzr, char* string, char* macro_end)
{
	// printf("I'm fuckin' macros!\n");
	preprocessor_t *prep = &tknzr->prep; 
	char* start = string;
	char* end = start;
	char* name = NULL;
	func_macro_t fm = {0};
	fm.body = init_tt();

	while(*start && isspace(*start)) start++;
	end = start;
	while(*end && !isspace(*end) && *end != '(') end++;
	
	name = malloc(end - start + 1);
	strncpy(name, start, end - start);
	name[end - start] = '\0';
	strcpy(fm.name, name);
	printf("%s\n", fm.name);
	fm.amount = 0;
	start = end;
	while(*end && *(end-1) != ')')
	{
		while(*start && isspace(*start) && *start != '(') start++;
		if(*start == '(') start++;
		end = start;
		while(*end && *end != ',' && *end != ')') end++;
		if(*end == ')' || *end == ',') end++;
		if(end - start < 2){
			start++;
			continue;
		}
		char* arg_name = malloc(32);
		if(end - start > 32)
		{
			printf("Too long argument name, bigger than 32 symbols are not allowed. You entered: %zu\n", end - start);
			free(tknzr);
			exit(1);
		}
		strncpy(arg_name, start, end-start);
		arg_name[end-start - 1] = '\0';
		strcpy(fm.args[fm.arg_count++], arg_name);
		
		start = end;
	}
	start = end;
	// printf("Args:\n");
	// for(size_t i = 0; i < fm.arg_count; i++)
	// {
	// 	printf("%s, ", fm.args[i]);
	// }
	// printf("\n");
	char* end_str = macro_end;
	size_t size_of_body = end_str - start;
	char* new_str = malloc(size_of_body+1);
	strncpy(new_str, start, size_of_body);
	new_str[size_of_body] = '\0';
	tokenizer_t tknzr2 = init_tkn(new_str);
	parse_macros(&tknzr2);
	tokenize(&tknzr2);
	
	preprocess(&tknzr2);
	// printf("We fell here.\n");
	fm.body = tknzr2.tt;
	tknzr2.tt.tokens = NULL;
	debug_macro(fm);
	push_macro(prep, fm);

	free(name);
	
	string = end_str + sizeof("@MACRO_END");
	printf("%s\n", string);

}



void push_const(preprocessor_t *prep, const_macro_t cm)
{
	if(prep->const_count == prep->const_capacity - 1)
	{
		prep->const_capacity *= 2;
		prep->const_table = realloc(prep->const_table, prep->const_capacity * sizeof(const_macro_t));
	}
	for(size_t i = 0; i < prep->const_count; i++)
	{
		if(!strcmp(prep->const_table[i].expect.tkn_str, cm.expect.tkn_str))
		{
			printf("Redefinition of constant %s.\nIgnoring.\n", cm.expect.tkn_str);
			return;
		}
	}
	printf("Expect: %s\n", cm.expect.tkn_str);
	debug_tkn_tbl(cm.exchange);
	prep->const_table[prep->const_count++] = cm;
}

void tokenize_define(tokenizer_t* tknzr, char** string)
{
	printf("Def string: %s\n", *string);
	preprocessor_t *prep = &tknzr->prep; 
	char* start = *string;
	char* arg, *end, *name;
	size_t name_size, arg_size;
	const_macro_t cm = {0};
	start += sizeof("@DEF");
	if(start == NULL)
		return;
	while(*start && isspace(*start)) start++;
	end = start;
	while(*end && !isspace(*end)) end++;
	
	name_size = end - start;
	name = malloc(name_size + 1);
	strncpy(name, start, name_size);
	name[name_size] = '\0';

	start = end;
	while(*start && isspace(*start)) start++;
	end = start;
	while(*end && *end != '\n') end++;
	
	arg_size = end - start;
	arg = malloc(arg_size + 1);
	strncpy(arg, start, arg_size);
	arg[arg_size] = '\0';
	tokenizer_t tt2 = init_tkn(arg);
	tokenize(&tt2);
	strcpy(cm.expect.tkn_str, name);
	cm.exchange = detach_tt(&tt2);
	printf("Name: %s\nArgs: ", name);
	debug_tkn_tbl(cm.exchange);
	push_const(prep, cm);
}

typedef struct
{
    char* macro_start;
    char* macro_end;
}paren_t;


void balance_macro(char** string, tokenizer_t *tknzr)
{

    paren_t paren;
    int depth = 0;
    char *stringptr = *string;
    while(*stringptr)
    {
        if(stringptr == strstr(stringptr, "@MACRO "))
        {
            if(depth == 0)
            {
                stringptr += sizeof("@MACRO");
                paren.macro_start = stringptr;
            }
            depth++;
        }
        if(stringptr == strstr(stringptr, "@MACRO_END"))
        {
            if(depth == 0)
            {
                printf("extra @MACRO_END\n");
                free_tkn(tknzr);
                exit(1);
            }
            else
            {
                depth--;
                if(depth == 0)
                {
                    paren.macro_end = stringptr;
                    
                    printf("End of block\n");
                    char* slice = str_slice(paren.macro_start, paren.macro_end);
                    printf("Slice: %s\n", slice);
                    tokenize_macro(tknzr, paren.macro_start, paren.macro_end);
                    free(slice);
                    stringptr += sizeof("@MACRO_END") - 1;
    				*string = stringptr;
    				return;                
                }
            }
        }
        // printf("Depth: %d\n", depth);
        stringptr++;
        
    }
    if(depth != 0)
    {
        printf("@MACRO AND @MACRO_END is unbalanced.\n");
        free_tkn(tknzr);
        exit(1);
    }
    *string = stringptr; 
}

void parse_macros(tokenizer_t* tknzr)
{
	tknzr->prep = init_preproc();
	// tokenizer_t tknzr_tmp = init_tkn(NULL);
	char* string = tknzr->init_str;
	char* s_tmp = malloc(strlen(string)+1);
	char* string_tmp = s_tmp; 
	size_t count = 0;
	while(*string)
	{
		// if(strstr(string, "@MACRO ") == string)
		// {
		// 	// tokenize_macro(tknzr, &string);
		// }
		if(strstr(string, "@MACRO ") == string)
		{
			balance_macro(&string, tknzr);
		}
		if(strstr(string, "@DEF ") == string)
		{
			char* new_start = string;
			char* new_end = new_start;
			while(*new_end && *new_end != '\n') new_end++;
			size_t size = new_end - new_start;
			char* string_for_def = malloc(size + 1);
			strncpy(string_for_def, new_start, size);
			string_for_def[size] = '\0';
			tokenize_define(tknzr, &string_for_def);
			string += size;
		}
		else
		{
			*string_tmp = *string;
			string_tmp++; string++; count++;
		}
	}
	s_tmp[count] = '\0';
	printf("We've got here!\n");
	// free_tkn(tknzr);
	free(tknzr->init_str);
	tknzr->init_str = s_tmp;
	
}


int is_macro(preprocessor_t prep, token_t tkn)
{
	for(size_t i = 0; i < prep.macro_count; i++)
	{
		if(!strcmp(tkn.tkn_str, prep.macro_table[i].name))
			return i;
	}
	return -1;
}

typedef struct
{
	token_table_t tkns;
	size_t block_size;
}Result_t;

Result_t get_args_slice(token_table_t *tknzr, size_t pos)
{
	Result_t res = {.tkns = init_tt(), .block_size = 0};
	int depth = 0;
	size_t i;
	// size_t start = 0, end = 0;
	for(i = pos + 1; i < tknzr->size; i++)
	{
		token_t this_token = tknzr->tokens[i];
		if(!strcmp(this_token.tkn_str, "("))
		{
			// if(depth == 0)
			// {
			// 	start = i;
			// }
			depth++;
		}
		if (!strcmp(this_token.tkn_str, ")"))
		{
			depth--;
			if(depth < 0)
			{
				printf("Unmatched closing bracket.\n");
				exit(1);
			}
			if(depth == 0)
			{
				printf("Block closed.\n");
				// end = i;
				push_token(&res.tkns, this_token);
				break;
			}
		}
		push_token(&res.tkns, this_token);
	}
	debug_tkn_tbl(res.tkns);
	res.block_size = i - pos;
	return res;
}

void preprocess(tokenizer_t *tknzr)
{
	token_table_t tknzr_tmp = init_tt();

	printf("We fell here!\n");
    
    if (tknzr_tmp.tokens == NULL) {
        printf("FATAL: Cannot allocate tokens in preprocess!\n");
        exit(1);
    }
	preprocessor_t prep = tknzr->prep;

	if(prep.const_count == 0 && prep.macro_count == 0)
		return;
	// tknzr->prep = init_preproc();

	// TODO: IMPLEMENT FUCKING MACRO INLINING ALREADY YOU FAT FUCK
	int i_see_no_changes = 0;
	int iterations = 0;
	
	while(!i_see_no_changes && iterations < MAX_ITERATIONS_FOR_MACRO){
		i_see_no_changes = 1;

		for(size_t j = 0; j < tknzr->tt.size; j++)
		{
			int idx_macro = 0;
			if((idx_macro = is_macro(prep, tknzr->tt.tokens[j])) != -1)
			{
				func_macro_t fm = prep.macro_table[idx_macro];
				printf("Found %s\n", fm.name);
				i_see_no_changes = 0;
				size_t arg_count = 0, arg_cap = 16, arg_block;
				token_table_t* args_num = malloc(sizeof(tokenizer_t)*arg_cap);
				for(size_t k = 0; k < arg_cap; k++)
				{
					args_num[k] = init_tt();

				}
				token_table_t cpy_bdy = init_tt();
				for(size_t k = 0; k < fm.body.size; k++)
				{
					push_token(&cpy_bdy, fm.body.tokens[k]);
				}
				Result_t args_tkns = get_args_slice(&tknzr->tt, j);
				arg_block = args_tkns.block_size;
				for(size_t arg_block_1 = 0;; arg_block_1++)
				{
					token_t this_token = args_tkns.tkns.tokens[arg_block_1]; 
					if(arg_block_1 == 0)
					{
						if(strcmp(this_token.tkn_str, "(")){
							printf("Wrong use of macro. Name(args).\n");
							free_tkn(tknzr);
							exit(1);
						}
						if(!strcmp(args_tkns.tkns.tokens[arg_block_1 + 1].tkn_str, ")"))
						{
							arg_block_1++;
							break;
						}
						continue;
					}
					
					if(arg_count == arg_cap)
					{
						arg_cap *= 2;
						args_num = realloc(args_num, arg_cap*sizeof(tokenizer_t));
					}
					while(true)
					{
						printf("%zu\n", arg_block_1);
						if(arg_block_1 >= args_tkns.tkns.size)
						{
							printf("Sum Ting Wong\n");
							exit(1);
						}
						this_token = args_tkns.tkns.tokens[arg_block_1];
						if(is_macro(prep, this_token) != -1)
						{
							Result_t res_2 = get_args_slice(&args_tkns.tkns, arg_block_1);
							push_token(&args_num[arg_count], this_token);
							for(size_t o = 0; o < res_2.tkns.size; o++)
							{
								push_token(&args_num[arg_count], res_2.tkns.tokens[o]);
							}
							arg_block_1 += res_2.block_size+1;
							continue;
						}
						if(!strcmp(this_token.tkn_str, ",") || !strcmp(this_token.tkn_str, ")"))
						{
							arg_count++;
							break;
						}
						printf("Pushing %s\n", this_token.tkn_str);
						push_token(&args_num[arg_count], this_token);
						arg_block_1++;
					}
					if(!strcmp(this_token.tkn_str, ")"))
					{
						break;
					}
				}
				if(fm.arg_count != arg_count)
				{
					printf("Wrong amount of arguments for macro %s.\nExpected: %zu\nGot: %zu\n", fm.name, fm.arg_count, arg_count);
					free_tkn(tknzr);
					exit(1);
				}
				for(size_t j = 0; j < arg_count; j++)
				{
					debug_tkn_tbl(args_num[j]);
				}
				for(size_t k = 0; k < fm.arg_count; k++)
				{
					token_table_t bdy_tmp = init_tt();
					int is_used = 0;
					for(size_t l = 0; l < cpy_bdy.size; l++)
					{

						if(!strcmp(cpy_bdy.tokens[l].tkn_str, "USE_LABEL"))
						{

							token_t next_token = cpy_bdy.tokens[l+1];
							
							char* label = malloc(strlen(next_token.tkn_str)+10);
							snprintf(label, strlen(next_token.tkn_str)+10, "%s_%zu", next_token.tkn_str, fm.amount);
							token_t tkn_push = {.type = TKN_LABEL};
							strcpy(tkn_push.tkn_str, label);
							push_token(&bdy_tmp, tkn_push);
							
							l += 1;
							continue;
						}
						if(!strcmp(cpy_bdy.tokens[l].tkn_str, "LABEL"))
						{

							token_t next_token = cpy_bdy.tokens[l+1];
							
							char* label = malloc(strlen(next_token.tkn_str)+10);
							snprintf(label, strlen(next_token.tkn_str)+10, "%s_%zu:", next_token.tkn_str, fm.amount);
							token_t tkn_push = {.type = TKN_LABEL};
							strcpy(tkn_push.tkn_str, label);
							push_token(&bdy_tmp, tkn_push);
							
							l += 1;
							continue;
						}
						if(!strcmp(cpy_bdy.tokens[l].tkn_str, "ARG_LABEL"))
						{

							token_t next_token = cpy_bdy.tokens[l+1];
							token_t label_token = {0};
							for(size_t m = 0; m < fm.arg_count; m++)
							{
								if(!strcmp(next_token.tkn_str, fm.args[m]))
								{
									label_token = args_num[m].tokens[0];
									break;
								}
							}
							char* label = malloc(strlen(label_token.tkn_str)+2);
							snprintf(label, strlen(label_token.tkn_str)+2, "%s:", label_token.tkn_str);
							token_t tkn_push = {.type = TKN_LABEL};
							strcpy(tkn_push.tkn_str, label);
							push_token(&bdy_tmp, tkn_push);
							
							l += 1;
							continue;
						}
						if(!strcmp(cpy_bdy.tokens[l].tkn_str, fm.args[k]))
						{
							for(size_t m = 0; m < args_num[k].size; m++)
							{
								push_token(&bdy_tmp, args_num[k].tokens[m]);
								is_used = 1;
							}
						}
						else
						{
							push_token(&bdy_tmp, cpy_bdy.tokens[l]); 
						}
					}
					free(cpy_bdy.tokens);
					cpy_bdy = bdy_tmp;
					if(is_used == 0)
					{
						printf("WARNING: Unused macro argument %s\n", fm.args[fm.arg_count]);
					}
				}
				
				for(size_t k = 0; k < cpy_bdy.size; k++)
				{
					push_token(&tknzr_tmp, cpy_bdy.tokens[k]);
				}

				j += arg_block;
				prep.macro_table[idx_macro].amount++;
			}
			else
			{
				push_token(&tknzr_tmp, tknzr->tt.tokens[j]);
			}
		}
		free(tknzr->tt.tokens);
		tknzr->tt = tknzr_tmp;
		tknzr_tmp = init_tt();
		iterations++;
	}

	for(size_t i = 0; i < prep.const_count; i++)
	{
		const_macro_t cm = prep.const_table[i];
		
		for(size_t j = 0; j < tknzr->tt.size; j++)
		{
			if(!strcmp(tknzr->tt.tokens[j].tkn_str, cm.expect.tkn_str))
			{
				// printf("Found one!\n");
				// tknzr->tokens[j] = exchange;
				printf("BEFORE: token[%zu] = %s\n", 
           			   j, tknzr->tt.tokens[j].tkn_str);
				for(size_t k = 0; k < cm.exchange.size; k++)
				{
					push_token(&tknzr_tmp, cm.exchange.tokens[k]);
				}
        		printf("AFTER: ");
        		debug_tkn_tbl(cm.exchange);
			}else
			{
				push_token(&tknzr_tmp, tknzr->tt.tokens[j]);
			}
		}
		free(tknzr->tt.tokens);
		tknzr->tt = tknzr_tmp;
		tknzr_tmp = init_tt();
	}

}
