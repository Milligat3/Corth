#include "tokenizing.h"
#include <stddef.h>

int only_digits(char* str);
int is_hex(char* str);
void push_str_tkn(token_table_t* tknzr, token_t tkn);
void lex_anal(token_table_t* tknzr);
