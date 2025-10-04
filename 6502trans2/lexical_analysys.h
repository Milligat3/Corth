#include "tokenizing.h"
#include <stddef.h>
int only_digits(char* str);
int is_hex(char* str);
void push_str_tkn(tokenizer_t* tknzr, token_t tkn);
void lex_anal(tokenizer_t *tknzr);
