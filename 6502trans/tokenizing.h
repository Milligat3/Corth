#pragma once

#include <stdint.h>
#include <stddef.h>

typedef enum
{
	TKN_UNKNOWN,
	TKN_PSH,
	TKN_PSH_WORD,
	TKN_POP,
	TKN_ADD,
	TKN_ADD_IMM,
	TKN_SUB,
	TKN_SUB_IMM,
	TKN_RSH,
	TKN_LSH,
	TKN_JMP,
	TKN_JNZ,
	TKN_JZ,
	TKN_JSR,
	TKN_RTS,
	TKN_DUP,
	TKN_SWP,
	TKN_INC,
	TKN_DEC,
	TKN_WRD,
	TKN_WRD_IMM,
	TKN_WRD_WORD,
	TKN_WRD_WORD_IMM,
	TKN_PSH3_WRD_OPT,
	TKN_RDD,
	TKN_RDD_IMM,
	TKN_RDD2_WRD,
	TKN_PSH_RDD2_WRD,
	TKN_HLT,
	TKN_CONST,
	TKN_LABEL,
	TKN_ASM,
	TKN_AND,
	TKN_OR,
	TKN_XOR,
	TKN_AND_IMM,
	TKN_OR_IMM,
	TKN_XOR_IMM,
	TKN_RDD_INC_DUP_WRD_OPT,
	TKN_JCC,
	TKN_JCS,
	TKN_DEF,
	TKN_GT,
	TKN_GE,
	TKN_LT,
	TKN_LE,
	TKN_EQ,
	TKN_GT_IMM,
	TKN_GE_IMM,
	TKN_LT_IMM,
	TKN_LE_IMM,
	TKN_EQ_IMM,
	TKN_ROT,
	TKN_OVER
}token_type;



typedef struct
{
	char tkn_str[32];
	token_type type;
	union value
	{
		uint32_t num[8];
		char label[32];
	} val;
}token_t;



typedef struct
{
	token_t expect, exchange;
}const_macro_t;

typedef struct
{
	char name[32];
	char args[8][32];
	size_t amount;
	token_t *body;
	size_t arg_count, tkn_count, tkn_cap;
}func_macro_t;

typedef struct
{
	const_macro_t* const_table;
	size_t const_count, const_capacity, macro_count, macro_capacity;
	func_macro_t* macro_table;
}preprocessor_t;


typedef struct
{
	token_t *tokens;
	size_t capacity, size;
	char* init_str;
	preprocessor_t prep;
}tokenizer_t;

void free_tkn(tokenizer_t* tkn);

void push_token(tokenizer_t* tknzr, token_t tkn);
tokenizer_t init_tkn(char* init_str);
void tokenize(tokenizer_t* tknzr);
void push_str(tokenizer_t* tknzr, char* tkn);
