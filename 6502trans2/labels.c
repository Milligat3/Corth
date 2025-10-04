#include "tokenizing.h"
#include "labels.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
label_table_t labl_tabl = {0};

int jmp_in_labels(char* tkn)
{
	char* tkn_lbl = malloc(32);
	tkn_lbl = strdup(tkn);
	
	for(size_t i = 0; i < labl_tabl.size; i++)
	{

		if(!strcmp(labl_tabl.labels[i].label_name, tkn_lbl))
		{
			return i;
		}
	}
	return -1;
}

int label_in_labels(char* tkn)
{
	char* tkn_lbl = malloc(32);
	tkn_lbl = strdup(tkn);
	if(tkn_lbl[strlen(tkn_lbl)-1] == ':')
		tkn_lbl[strlen(tkn_lbl)-1] = '\0';
	for(size_t i = 0; i < labl_tabl.size; i++)
	{

		if(!strcmp(labl_tabl.labels[i].label_name, tkn_lbl))
		{
			return i;
		}
	}
	return -1;
}



void find_labels(tokenizer_t *tknzr)
{
	uint32_t addr = 0;
	for(size_t idx = 0; idx < tknzr->size; idx++)
	{
		token_t tkn = tknzr->tokens[idx];
		if(tkn.tkn_str[strlen(tkn.tkn_str)-1] == ':')
		{
			if(label_in_labels(tkn.tkn_str) != -1)
			{
				printf("Double declaration of label %s\n", tkn.tkn_str);
				free_tkn(tknzr);
				exit(1);
			}
			strcpy(labl_tabl.labels[labl_tabl.size].label_name, tkn.tkn_str);
			labl_tabl.labels[labl_tabl.size].label_name[strlen(labl_tabl.labels[labl_tabl.size].label_name)-1] = '\0'; 
			labl_tabl.labels[labl_tabl.size].addr = addr;
			labl_tabl.size++;
		}
		else if(tkn.type == TKN_PSH)
		{
			addr += 5;
			idx++;
		}else if(tkn.type == TKN_JMP || tkn.type == TKN_JNZ || tkn.type == TKN_JZ || tkn.type == TKN_JSR)
		{
			addr += 5;
			idx++;
		}
		else
		{
			addr++;
		}

	}
}
