#ifndef ASMGEN_H
#define ASMGEN_H
#include "token.h"
#include <stddef.h>

typedef struct
{
	char** table;
	size_t size;
	size_t capacity;
}asm_table_t;

extern asm_table_t asm_table;

void push_asm(char* tkn);
void init_asm_table(void);
char * parse_asm(tokenizer_t* tknzr, char* start_asm);
#endif