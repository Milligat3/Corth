#include "tokenizing.h"
#include <stddef.h>

#define MAX_ITERATIONS_FOR_MACRO 100

void parse_macros(tokenizer_t* tknzr);
void push_macro(preprocessor_t *prep, func_macro_t fm);
void debug_macro(func_macro_t fm);
void push_token_into_macro(func_macro_t *fm, token_t tkn);
void preprocess(tokenizer_t *tkznr);
