#pragma once
#include "tokenizing.h"
#include <stdint.h>

typedef struct
{
	char label_name[32];
	uint16_t addr;
}label_t;

typedef struct
{
	label_t labels[32];
	size_t size;
}label_table_t;

extern label_table_t labl_tabl;

int jmp_in_labels(char* tkn);
int label_in_labels(char* tkn);
void find_labels(tokenizer_t *tknzr);