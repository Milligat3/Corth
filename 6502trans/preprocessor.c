

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "preprocessor.h"
#include "tokenizing.h"


void push_token_into_macro(func_macro_t* macro, token_t tkn)
{
	if(macro->tkn_count == macro->tkn_cap-1)
	{
		macro->tkn_cap *= 2;
		macro->body = realloc(macro->body, macro->tkn_cap*sizeof(token_t));
	}
	macro->body[macro->tkn_count++] = tkn;

}

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
	for(size_t i = 0; i < fm.tkn_count; i++)
	{
		printf("%s%s%s", i == 0 ? "[\"" : " \"", fm.body[i].tkn_str, i == fm.tkn_count-1 ? "\"]\n" : "\",");
	}
}


void tokenize_macro(tokenizer_t* tknzr, char** string)
{
	preprocessor_t *prep = &tknzr->prep; 
	char* start = *string;
	start += sizeof("@MACRO");
	if(start == NULL)
		return;
	char* end = start;
	char* name = NULL;
	char* new_macro = strstr(start, "@MACRO ");
	char* macro_end = strstr(start, "@MACRO_END");
	// while((start = strstr(tknzr->init_str, "@MACRO")) != NULL){
	// int arg_count = 0;
	func_macro_t fm = {.tkn_count = 0, .tkn_cap = 256};
	
	// fm.body = malloc(fm.tkn_cap*sizeof(token_t));
	
	if(strstr(start, "@MACRO_END") == NULL)
	{
		printf("FUCKIN' HELL! NO END FOR MACRO? ABORTION!\n");
		free_tkn(tknzr);
		exit(1);
	}
	if(new_macro < macro_end && new_macro > start && new_macro != NULL)
	{
		printf("Rule 1. No definitions of macros inside macros.\n");
		free_tkn(tknzr);
		exit(1);
	}
	while(*start && isspace(*start)) start++;
	end = start;
	while(*end && !isspace(*end) && *end != '(') end++;
	
	name = malloc(end - start + 1);
	strncpy(name, start, end - start);
	name[end - start] = '\0';
	strcpy(fm.name, name);
	start = end;
	while(*end && *(end-1) != ')')
	{
		while(*start && isspace(*start) && *start != '(') start++;
		if(*start == '(') start++;
		end = start;
		while(*end && !isspace(*end) && *end != ',' && *end != ')') end++;
		if(*end == ')' || *end == ',') end++;
		if(end - start < 2){
			start++;
			break;
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
	printf("Args:\n");
	for(size_t i = 0; i < fm.arg_count; i++)
	{
		printf("%s, ", fm.args[i]);
	}
	printf("\n");
	char* end_str = strstr(start, "@MACRO_END") + sizeof("@MACRO_END");
	size_t size_of_body = end_str - start;
	char* new_str = malloc(size_of_body+1);
	strncpy(new_str, start, size_of_body - sizeof("@MACRO_END"));
	new_str[size_of_body - sizeof("@MACRO_END")] = '\0';
	tokenizer_t tknzr2 = init_tkn(new_str);
	
	tokenize(&tknzr2);
	fm.body = tknzr2.tokens;
	fm.tkn_count = tknzr2.size;
	fm.tkn_cap = tknzr2.capacity;
	
	debug_macro(fm);
	push_macro(prep, fm);
	free(name);

	*string = end_str;
	printf("%s\n", *string);

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
		if(!strcmp(prep->const_table[i].exchange.tkn_str, cm.exchange.tkn_str))
		{
			printf("Redefinition of constant %s.\nIgnoring.\n", cm.exchange.tkn_str);
			return;
		}
	}
	printf("Expect: %s, Exchange: %s\n", cm.expect.tkn_str, cm.exchange.tkn_str);
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
	while(*end && !isspace(*end)) end++;
	
	arg_size = end - start;
	arg = malloc(arg_size + 1);
	strncpy(arg, start, arg_size);
	arg[arg_size] = '\0';
	strcpy(cm.expect.tkn_str, name);
	strcpy(cm.exchange.tkn_str, arg);
	printf("Name: %s, Arg: %s\n", name, arg);
	push_const(prep, cm);
}

void parse_macros(tokenizer_t* tknzr)
{
	tknzr->prep = init_preproc();
	tokenizer_t tknzr_tmp = init_tkn(NULL);
	char* string = tknzr->init_str;
	tknzr_tmp.init_str = malloc(strlen(string));
	char* string_tmp = tknzr_tmp.init_str;
	while(*string)
	{
		if(strstr(string, "@MACRO ") == string)
		{
			tokenize_macro(tknzr, &string);
			// string = macro;
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
			string_tmp++; string++;
		}
	}
	tknzr_tmp.prep = tknzr->prep;
	free_tkn(tknzr);
	*tknzr = tknzr_tmp;
	// free(string);
}
void preprocess(tokenizer_t *tknzr)
{
	tokenizer_t tknzr_tmp = init_tkn(NULL);
	// tknzr->prep = init_preproc();
	preprocessor_t prep = tknzr->prep;

	int i_see_no_changes = 0;
	int iterations = 0;
	while(!i_see_no_changes && iterations < MAX_ITERATIONS_FOR_MACRO){
		i_see_no_changes = 1;

		for(size_t i = 0; i < prep.macro_count; i++)
		{
			func_macro_t fm = prep.macro_table[i];
			printf("Macro name: %s\n", fm.name);
			for(size_t j = 0; j < tknzr->size; j++)
			{
				if(!strcmp(tknzr->tokens[j].tkn_str, fm.name))
				{
					printf("Found %s\n", fm.name);
					i_see_no_changes = 0;
					token_t args_num[8];
					size_t arg_count = 0, arg_block;
					token_t* cpy_bdy = malloc(fm.tkn_count * sizeof(token_t));
					memcpy(cpy_bdy, fm.body, fm.tkn_count * sizeof(token_t));
					for(arg_block = 0;; arg_block++)
					{
						token_t this_token = tknzr->tokens[j + arg_block + 1]; 
						if(arg_block == 0)
						{
							if(strcmp(this_token.tkn_str, "(")){
								printf("Wrong use of macro. Name(args).\n");
								free_tkn(tknzr);
								exit(1);
							}
							continue;
						}
						if(!strcmp(this_token.tkn_str, ","))
						{
							continue;
						}
						if(!strcmp(this_token.tkn_str, ")"))
						{
							arg_block++;
							break;
						}
						args_num[arg_count] = tknzr->tokens[j + arg_block + 1];
						arg_count++;
					}
					if(fm.arg_count != arg_count)
					{
						printf("Wrong amount of arguments for macro %s.\nExpected: %zu\nGot: %zu\n", fm.name, fm.arg_count, arg_count);
						free_tkn(tknzr);
						exit(1);
					}
					for(size_t k = 0; k < fm.arg_count; k++)
					{
						for(size_t l = 0; l < fm.tkn_count; l++)
						{
							if(!strcmp(cpy_bdy[l].tkn_str, fm.args[k]))
							{
								cpy_bdy[l] = args_num[k];
							}
						}
					}
					for(size_t k = 0; k < fm.tkn_count; k++)
					{
						push_token(&tknzr_tmp, cpy_bdy[k]);
					}
					j += arg_block;
				}
				else
				{
					push_token(&tknzr_tmp, tknzr->tokens[j]);
				}
			}
			free(tknzr->tokens);
			*tknzr = tknzr_tmp;
			tknzr_tmp = init_tkn(NULL);
		}
		iterations++;
	}


	for(size_t i = 0; i < prep.const_count; i++)
	{
		const_macro_t cm = prep.const_table[i];
		
		for(size_t j = 0; j < tknzr->size; j++)
		{
			if(!strcmp(tknzr->tokens[j].tkn_str, cm.expect.tkn_str))
			{
				// printf("Found one!\n");
				// tknzr->tokens[j] = exchange;
				printf("BEFORE: token[%zu] = %s (type %d)\n", 
           			   j, tknzr->tokens[j].tkn_str, tknzr->tokens[j].type);
       
        		tknzr->tokens[j] = cm.exchange;
       
        		printf("AFTER: token[%zu] = %s (type %d)\n", 
           			   j, tknzr->tokens[j].tkn_str, tknzr->tokens[j].type);
			}
		}
	}
	// TODO: IMPLEMENT FUCKING MACRO INLINING ALREADY YOU FAT FUCK
	
}