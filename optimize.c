
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizing.h"
#include "optimize.h"

int tkns_eq(token_t tkn1, token_t tkn2)
{

	return tkn1.type == tkn2.type && !memcmp(tkn1.val.num, tkn2.val.num, 8*sizeof(uint32_t));
}

int tknzr_cont_equal(token_table_t tknzr1, token_table_t tknzr2)
{
	for(size_t i = 0; i < tknzr1.size; i++)
	{
		if(!tkns_eq(tknzr1.tokens[i], tknzr2.tokens[i]))
			return 0;
	}
	return 1;
}

int tknzr_equal(token_table_t tknzr1, token_table_t tknzr2)
{
	return tknzr1.size == tknzr2.size && tknzr_cont_equal(tknzr1, tknzr2);
}

bool match_psh_psh_wrd(token_table_t *tknzr, size_t i)
{
	return tknzr->tokens[i].type == TKN_PSH &&
		   tknzr->tokens[i+1].type == TKN_PSH &&
		   tknzr->tokens[i+2].type == TKN_WRD;
}

void apply_psh_psh_wrd_opt(token_table_t *tknzr, token_table_t *tknzr2, size_t *i)
{

	uint32_t addr = tknzr->tokens[*i].val.num[0] | (tknzr->tokens[*i+1].val.num[0] << 8);
	token_t new_token = {.type = TKN_WRD_IMM};
	new_token.val.num[0] = addr;
	push_token(tknzr2, new_token);
	*i+=3;
}

bool match_psh_psh_rdd(token_table_t *tknzr, size_t i)
{
	return tknzr->tokens[i].type == TKN_PSH &&
		   tknzr->tokens[i+1].type == TKN_PSH &&
		   tknzr->tokens[i+2].type == TKN_RDD;
}

void apply_psh_psh_rdd(token_table_t *tknzr, token_table_t *tknzr2, size_t *i)
{
	uint32_t addr = tknzr->tokens[*i].val.num[0] | (tknzr->tokens[*i+1].val.num[0] << 8);
	token_t new_token = (token_t){.type = TKN_RDD_IMM, .val = {.num = {addr}}};
	push_token(tknzr2, new_token);
	*i+=2;
}

bool match_psh_wrd_imm(token_table_t *tknzr, size_t i)
{
	return tknzr->tokens[i].type == TKN_PSH &&
		   tknzr->tokens[i+1].type == TKN_WRD_IMM;
}

void apply_psh_wrd_imm(token_table_t *tknzr, token_table_t *tknzr2, size_t *i)
{
	token_t new_token = (token_t){.type = TKN_PSH3_WRD_OPT, .val = {.num = {tknzr->tokens[*i].val.num[0], tknzr->tokens[*i+1].val.num[0]}}};
	push_token(tknzr2, new_token);
	*i+=1;
}


bool match_rrd_rrd_wrd(token_table_t *tknzr, size_t i)
{
	return tknzr->tokens[i].type == TKN_RDD_IMM &&
		   tknzr->tokens[i+1].type == TKN_RDD_IMM &&
		   tknzr->tokens[i+2].type == TKN_WRD;
}

void apply_rrd_imm_rrd_imm_wrd(token_table_t *tknzr, token_table_t *tknzr2, size_t *i)
{
	uint32_t addr1 = tknzr->tokens[*i].val.num[0];
	uint32_t addr2 = tknzr->tokens[*i+1].val.num[0];
	token_t new_token = (token_t){.type = TKN_RDD2_WRD, .val = {.num = {addr1, addr2}}};
	push_token(tknzr2, new_token);
	*i+=2;
}

bool match_psh_rdd2_wrd(token_table_t *tknzr, size_t i)
{
	return tknzr->tokens[i].type == TKN_PSH &&
		   tknzr->tokens[i+1].type == TKN_RDD2_WRD;
}

void apply_psh_rdd2_wrd(token_table_t *tknzr, token_table_t *tknzr2, size_t *i)
{
	uint32_t val = tknzr->tokens[*i].val.num[0];
	uint32_t addr1 = tknzr->tokens[*i+1].val.num[0];
	uint32_t addr2 = tknzr->tokens[*i+1].val.num[1];
	token_t new_token = (token_t){.type = TKN_PSH_RDD2_WRD, .val = {.num = {addr1, addr2, val}}};
	push_token(tknzr2, new_token);
	*i+=1;
}

bool match_rdd_inc_dup_wrd(token_table_t *tknzr, size_t i)
{
	return tknzr->tokens[i].type == TKN_RDD_IMM &&
		   tknzr->tokens[i+1].type == TKN_INC &&
		   tknzr->tokens[i+2].type == TKN_DUP &&
		   tknzr->tokens[i+3].type == TKN_WRD_IMM;
}

void apply_rrd_inc_dup_wrd(token_table_t *tknzr, token_table_t *tknzr2, size_t *i)
{
	uint32_t addr1 = tknzr->tokens[*i].val.num[0];
	uint32_t addr2 = tknzr->tokens[*i+3].val.num[0];
	token_t new_token = (token_t){.type = TKN_RDD_INC_DUP_WRD_OPT, .val = {.num = {addr1, addr2}}};
	push_token(tknzr2, new_token);
	*i+=3;
}

int match_psh_psh_add(token_table_t *tknzr, size_t i)
{
	return tknzr->tokens[i].type == TKN_PSH &&
		   tknzr->tokens[i+1].type == TKN_PSH &&
		   tknzr->tokens[i+2].type == TKN_ADD;
}

void apply_psh_psh_add(token_table_t *tknzr, token_table_t *tknzr2, size_t *i)
{
	uint32_t op_1 = tknzr->tokens[*i].val.num[0];
	uint32_t op_2 = tknzr->tokens[*i+1].val.num[0];
	token_t new_token = (token_t){.type = TKN_PSH, .val = {.num = {(uint8_t)(op_2 + op_1)}}};
	push_token(tknzr2, new_token);
	*i+=2;
}

int match_psh_add_imm(token_table_t *tknzr, size_t i)
{
	return tknzr->tokens[i].type == TKN_PSH &&
		   tknzr->tokens[i+1].type == TKN_ADD_IMM;
}

void apply_psh_add_imm(token_table_t *tknzr, token_table_t *tknzr2, size_t *i)
{
	uint32_t op_1 = tknzr->tokens[*i].val.num[0];
	uint32_t op_2 = tknzr->tokens[*i+1].val.num[0];
	token_t new_token = (token_t){.type = TKN_PSH, .val = {.num = {(uint8_t)(op_2 + op_1)}}};
	push_token(tknzr2, new_token);
	*i+=1;
}


int match_psh_psh_sub(token_table_t *tknzr, size_t i)
{
	return tknzr->tokens[i].type == TKN_PSH &&
		   tknzr->tokens[i+1].type == TKN_PSH &&
		   tknzr->tokens[i+2].type == TKN_SUB;
}


void apply_psh_psh_sub(token_table_t *tknzr, token_table_t *tknzr2, size_t *i)
{
	uint32_t op_1 = tknzr->tokens[*i].val.num[0];
	uint32_t op_2 = tknzr->tokens[*i+1].val.num[0];
	token_t new_token = (token_t){.type = TKN_PSH, .val = {.num = {(uint8_t)(op_1 - op_2)}}};
	push_token(tknzr2, new_token);
	*i+=2;
}


int match_psh_sub_imm(token_table_t *tknzr, size_t i)
{
	return tknzr->tokens[i].type == TKN_PSH &&
		   tknzr->tokens[i+1].type == TKN_SUB_IMM;
}

void apply_psh_sub_imm(token_table_t *tknzr, token_table_t *tknzr2, size_t *i)
{
	uint32_t op_1 = tknzr->tokens[*i].val.num[0];
	uint32_t op_2 = tknzr->tokens[*i+1].val.num[0];
	token_t new_token = (token_t){.type = TKN_PSH, .val = {.num = {(uint8_t)(op_1 - op_2)}}};
	push_token(tknzr2, new_token);
	*i+=1;
}

int match_psh2_2wrd_imm(token_table_t *tknzr, size_t i)
{
	return tknzr->tokens[i].type == TKN_PSH_WORD &&
		   tknzr->tokens[i+1].type == TKN_WRD_IMM &&
		   tknzr->tokens[i+2].type == TKN_WRD_IMM;
}



void apply_psh2_2wrd_imm(token_table_t *tknzr, token_table_t *tknzr2, size_t *i)
{
	uint32_t op_1 = tknzr->tokens[*i].val.num[0];
	uint32_t op_2 = tknzr->tokens[*i].val.num[1];
	uint32_t addr_1 = tknzr->tokens[*i+1].val.num[0];
	uint32_t addr_2 = tknzr->tokens[*i+2].val.num[0];

	token_t new_token = (token_t){.type = TKN_PSH3_WRD_OPT, .val = {.num = {op_2, addr_1}}};
	push_token(tknzr2, new_token);
	new_token = (token_t){.type = TKN_PSH3_WRD_OPT, .val = {.num = {op_1, addr_2}}};
	push_token(tknzr2, new_token);
	*i+=2;
	
}

int match_psh2_wrd_word(token_table_t *tknzr, size_t i)
{
	return tknzr->tokens[i].type == TKN_PSH_WORD &&
		   tknzr->tokens[i+1].type == TKN_WRD_WORD_IMM;
}

void apply_psh2_wrd_word(token_table_t *tknzr, token_table_t *tknzr2, size_t *i)
{
	uint32_t op_1 = tknzr->tokens[*i].val.num[0];
	uint32_t op_2 = tknzr->tokens[*i].val.num[1];
	uint32_t addr_1 = tknzr->tokens[*i+1].val.num[0];
	uint32_t addr_2 = addr_1+1;

	token_t new_token = (token_t){.type = TKN_PSH3_WRD_OPT, .val = {.num = {op_1, addr_1}}};
	push_token(tknzr2, new_token);
	new_token = (token_t){.type = TKN_PSH3_WRD_OPT, .val = {.num = {op_2, addr_2}}};
	push_token(tknzr2, new_token);
	*i+=1;
	
}


void optimize(token_table_t *tknzr)
{
	token_table_t tknzr_tmp = init_tt();
 	token_table_t *tknzr2	= &tknzr_tmp;
 	int nobody_came = 0;
	int iterations = 0;
	// tknzr = tknzr;
	// tknzr2 = tknzr;
	// tknzr2 = tknzr2;

	while(!nobody_came && iterations < MAX_ITER_OPT){
		for(size_t i = 0; i < tknzr->size; i++)
		{
			if(match_psh_psh_wrd(tknzr, i))
			{
				apply_psh_psh_wrd_opt(tknzr, tknzr2, &i);
				continue;	
			}
			if (match_psh_wrd_imm(tknzr, i))
			{
				apply_psh_wrd_imm(tknzr, tknzr2, &i);
				continue;
			}
			if(match_rrd_rrd_wrd(tknzr, i))
			{
				apply_rrd_imm_rrd_imm_wrd(tknzr, tknzr2, &i);
				continue;
			}
			if(match_rdd_inc_dup_wrd(tknzr, i))
			{
				apply_rrd_inc_dup_wrd(tknzr,tknzr2, &i);
				continue;
			}
			if(match_psh_psh_add(tknzr, i))
			{
				apply_psh_psh_add(tknzr, tknzr2, &i);
				continue;
			}
			if(match_psh_add_imm(tknzr, i))
			{
				apply_psh_add_imm(tknzr, tknzr2, &i);
				continue;
			}
			if(match_psh_psh_sub(tknzr, i))
			{
				apply_psh_psh_sub(tknzr, tknzr2, &i);
				continue;
			}
			if(match_psh_sub_imm(tknzr, i))
			{
				apply_psh_sub_imm(tknzr, tknzr2, &i);
				continue;
			}
			if(match_psh_psh_rdd(tknzr, i))
			{
				apply_psh_psh_rdd(tknzr, tknzr2, &i);
				continue;
			}
			if(match_psh_rdd2_wrd(tknzr, i))
			{
				apply_psh_rdd2_wrd(tknzr,tknzr2, &i);
				continue;
			}
			if(match_psh2_2wrd_imm(tknzr, i))
			{
				apply_psh2_2wrd_imm(tknzr, tknzr2, &i);
				continue;
			}
			if(match_psh2_wrd_word(tknzr, i))
			{
				apply_psh2_wrd_word(tknzr, tknzr2, &i);
				continue;
			}
			push_token(tknzr2, tknzr->tokens[i]);
		}
		nobody_came = tknzr_equal(*tknzr, tknzr_tmp);
		free(tknzr->tokens);
		*tknzr = tknzr_tmp;
		tknzr_tmp = init_tt();
		iterations++;
	}
}

