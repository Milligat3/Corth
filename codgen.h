#ifndef CODGEN_H
#define CODGEN_H

#include <stdio.h>
#include "token.h"
#include <stddef.h>
int output_str(FILE* output, token_table_t *tknzr);

#endif