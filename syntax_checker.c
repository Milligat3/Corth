#include "tokenizing.h"
#include "labels.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syntax_checker.h"

#include "ast_builder.h"


int parse_arg_of_mnem(token_table_t *tts, size_t *i)
{
	token_table_t tknzr_3 = init_tt();
	size_t j;
	for(j = *i+1; j < tts->size; j++)
	{
		token_t *this_tok = &tts->tokens[j];
		if(this_tok->type == TKN_CONST || this_tok->type == TKN_BINOP)
		{
			push_token(&tknzr_3, *this_tok);
			continue;
		}
		
		break;
	}
	AstBuilder_t tt = {tknzr_3, 0};
	AstNode_t *to_eval = parse_expr(&tt);
	*i = j-1;
	return eval(to_eval);	
}

int check_syntax(token_table_t *tknzr)
{
	token_table_t tknzr_tmp = init_tt();
	token_table_t *tknzr2	= &tknzr_tmp;
	token_table_t tkn_string = *tknzr;
	size_t size_of_str = tknzr->size;
	int found_err = 0;
	for(size_t i = 0; i < size_of_str; i++)
	{
		token_t *tkn = &tkn_string.tokens[i];
		switch(tkn->type)
		{
			case TKN_PSH:
			{
				token_t next_tok = tkn_string.tokens[i + 1];
				if(next_tok.type != TKN_CONST)
				{
					printf("Not a valid digit operand after PSH: %s\n", next_tok.tkn_str);
					found_err = 1;
					continue;
				}
				int num = parse_arg_of_mnem(tknzr, &i);
				token_t new_token = *tkn;
				
				if(num < 0x100)
				{
					new_token.val.num[0] = num;
				}
				else if(num < 0x10000)
				{
					new_token.type = TKN_PSH_WORD;
					new_token.val.num[0] = num & 0xFF;
					new_token.val.num[1] = num >> 8;
				}
				else
				{
					printf("Not a valid digit operand after PSH: %s\n", next_tok.tkn_str);
					found_err = 1;
					continue;
				}
				
				push_token(tknzr2, new_token);
				continue;
			}
			case TKN_PSH_WORD:
			{
				token_t next_tok = tkn_string.tokens[i + 1];
				if(next_tok.type != TKN_CONST)
				{
					printf("Not a valid digit operand after PSH_W: %s\n", next_tok.tkn_str);
					found_err = 1;
					continue;
				}
				int num = parse_arg_of_mnem(tknzr, &i);
				token_t new_token = *tkn;
				
				if(num < 0x10000)
				{
					new_token.val.num[0] = num & 0xFF;
					new_token.val.num[1] = num >> 8;
				}
				else
				{
					printf("Not a valid digit operand after PSH_W: %s\n", next_tok.tkn_str);
					found_err = 1;
					continue;
				}
				
				push_token(tknzr2, new_token);
				continue;
			}
			case TKN_JMP:
			{
				token_t next_tok = tkn_string.tokens[++i];
				int idx = jmp_in_labels(next_tok.tkn_str); 
				if(idx == -1)
				{
					printf("Not a valid label after JMP: %s\n", next_tok.tkn_str);
					found_err = 1;
				}
				token_t new_token = *tkn;
				new_token.val.num[0] = idx;
				push_token(tknzr2, new_token);
				continue;
			}
			case TKN_JNZ:
			{
				token_t next_tok = tkn_string.tokens[++i];
				int idx = jmp_in_labels(next_tok.tkn_str); 
				if(idx == -1)
				{
					printf("Not a valid label after JNZ: %s\n", next_tok.tkn_str);
					found_err = 1;
				}
				token_t new_token = *tkn;
				new_token.val.num[0] = idx;
				push_token(tknzr2, new_token);
				continue;
			}
			case TKN_JZ:
			{
				token_t next_tok = tkn_string.tokens[++i];
				int idx = jmp_in_labels(next_tok.tkn_str); 
				if(idx == -1)
				{
					printf("Not a valid label after JZ: %s\n", next_tok.tkn_str);
					found_err = 1;
				}
				token_t new_token = *tkn;
				new_token.val.num[0] = idx;
				push_token(tknzr2, new_token);
				continue;
			}
			case TKN_JSR:
			{
				token_t next_tok = tkn_string.tokens[++i];
				int idx = jmp_in_labels(next_tok.tkn_str); 
				if(idx == -1)
				{
					printf("Not a valid label after JSR: %s\n", next_tok.tkn_str);
					found_err = 1;
				}
				token_t new_token = *tkn;
				new_token.val.num[0] = idx;
				push_token(tknzr2, new_token);
				continue;
			}
			case TKN_JCC:
			{
				token_t next_tok = tkn_string.tokens[++i];
				int idx = jmp_in_labels(next_tok.tkn_str); 
				if(idx == -1)
				{
					printf("Not a valid label after JCC: %s\n", next_tok.tkn_str);
					found_err = 1;
				}
				token_t new_token = *tkn;
				new_token.val.num[0] = idx;
				push_token(tknzr2, new_token);
				continue;
			}
			case TKN_JCS:
			{
				token_t next_tok = tkn_string.tokens[++i];
				int idx = jmp_in_labels(next_tok.tkn_str); 
				if(idx == -1)
				{
					printf("Not a valid label after JCS: %s\n", next_tok.tkn_str);
					found_err = 1;
				}
				token_t new_token = *tkn;
				new_token.val.num[0] = idx;
				push_token(tknzr2, new_token);
				continue;
			}
			case TKN_POP:
			{
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_POP_WORD:
			{
				push_token(tknzr2, *tkn);
				continue;
			}
		
			case TKN_ADD:
			{
				token_t next_tkn = tkn_string.tokens[i+1];
				if(next_tkn.type == TKN_CONST)
				{
					// if(strtoul(next_tkn.tkn_str, NULL, 10) >= 0x100)
					// {
					// 	printf("The number is bigger than 0xFF for immediate ADD. Abort.\n");
					// 	found_err = 1;
					// }
					int num = parse_arg_of_mnem(tknzr, &i);
					if(num > 0xFF)
					{
						printf("The number is bigger than 0xFF for immediate ADD. Abort.\n");
						found_err = 1;
					}
					token_t new_token = *tkn;
					new_token.type = TKN_ADD_IMM;
					new_token.val.num[0] = num;
					push_token(tknzr2, new_token);
					
					continue;
				}
				push_token(tknzr2, *tkn);
	
				continue;
			}
			case TKN_SUB:
			{
				token_t next_tkn = tkn_string.tokens[i+1];
				if(next_tkn.type == TKN_CONST)
				{
					int num = parse_arg_of_mnem(tknzr, &i);
					if(num > 0xFF)
					{
						printf("The number is bigger than 0xFF for immediate SUB. Abort.\n");
						found_err = 1;
					}
					token_t new_token = *tkn;
					new_token.type = TKN_SUB_IMM;
					new_token.val.num[0] = num;
					push_token(tknzr2, new_token);
					continue;
				}
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_ADD_WORD:
			{
				token_t next_tkn = tkn_string.tokens[i+1];
				if(next_tkn.type == TKN_CONST)
				{
					// if(strtoul(next_tkn.tkn_str, NULL, 10) >= 0x100)
					// {
					// 	printf("The number is bigger than 0xFF for immediate ADD. Abort.\n");
					// 	found_err = 1;
					// }
					int num = parse_arg_of_mnem(tknzr, &i);
					if(num > 0xFFFF)
					{
						printf("The number is bigger than 0xFFFF for immediate ADD. Abort.\n");
						found_err = 1;
					}
					token_t new_token = *tkn;
					new_token.type = TKN_ADD_IMM_WORD;
					new_token.val.num[0] = num;
					push_token(tknzr2, new_token);
					
					continue;
				}
				push_token(tknzr2, *tkn);
	
				continue;
			}
			case TKN_SUB_WORD:
			{
				token_t next_tkn = tkn_string.tokens[i+1];
				if(next_tkn.type == TKN_CONST)
				{
					int num = parse_arg_of_mnem(tknzr, &i);
					if(num > 0xFFFF)
					{
						printf("The number is bigger than 0xFFFF for immediate SUB. Abort.\n");
						found_err = 1;
					}
					token_t new_token = *tkn;
					new_token.type = TKN_SUB_IMM_WORD;
					new_token.val.num[0] = num;
					push_token(tknzr2, new_token);
					continue;
				}
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_RSH:
			{
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_LSH:
			{
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_RTS:
			{
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_HLT:
			{
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_DUP:
			{
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_SWP:
			{
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_INC:
			{
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_DEC:
			{
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_WRD:
			{
				token_t next_tkn = tkn_string.tokens[i+1];
				if(next_tkn.type == TKN_CONST)
				{
					int num = parse_arg_of_mnem(tknzr, &i);
					if(num > 0xFFFF)
					{
						printf("The number is bigger than 0xFFFF for immediate WRD. Abort.\n");
						found_err = 1;
					}
					token_t new_token = *tkn;
					new_token.type = TKN_WRD_IMM;
					new_token.val.num[0] = num;
					push_token(tknzr2, new_token);
					continue;
				}
				push_token(tknzr2, *tkn);
				continue;
			}		
			case TKN_WRD_WORD:
			{
				token_t next_tkn = tkn_string.tokens[i+1];
				if(next_tkn.type == TKN_CONST)
				{
					int num = parse_arg_of_mnem(tknzr, &i);
					if(num > 0xFFFF)
					{
						printf("The number is bigger than 0xFFFF for immediate WRD_WORD. Abort.\n");
						found_err = 1;
					}
					token_t new_token = *tkn;
					new_token.type = TKN_WRD_WORD_IMM;
					new_token.val.num[0] = num;
					push_token(tknzr2, new_token);
					continue;
				}
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_PEEK:
			{
				token_t next_tkn = tkn_string.tokens[i+1];
				if(next_tkn.type == TKN_CONST)
				{
					int num = parse_arg_of_mnem(tknzr, &i);
					if(num > 0xFF)
					{
						printf("The number is bigger than 0xFF for PEEK. Abort.\n");
						found_err = 1;
					}
					token_t new_token = *tkn;
					new_token.type = TKN_PEEK_IMM;
					new_token.val.num[0] = num;
					push_token(tknzr2, new_token);
					continue;
				}
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_POKE:
			{
				token_t next_tkn = tkn_string.tokens[i+1];
				if(next_tkn.type == TKN_CONST)
				{
					int num = parse_arg_of_mnem(tknzr, &i);
					if(num > 0xFF)
					{
						printf("The number is bigger than 0xFF for POKE. Abort.\n");
						found_err = 1;
					}
					token_t new_token = *tkn;
					new_token.type = TKN_POKE_IMM;
					new_token.val.num[0] = num;
					push_token(tknzr2, new_token);
					continue;
				}
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_PEEK_WORD:
			{
				token_t next_tkn = tkn_string.tokens[i+1];
				if(next_tkn.type == TKN_CONST)
				{
					int num = parse_arg_of_mnem(tknzr, &i);
					if(num > 0xFF)
					{
						printf("The number is bigger than 0xFF for PEEK_W. Abort.\n");
						found_err = 1;
					}
					token_t new_token = *tkn;
					new_token.type = TKN_PEEK_IMM_WORD;
					new_token.val.num[0] = num;
					push_token(tknzr2, new_token);
					continue;
				}
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_POKE_WORD:
			{
				token_t next_tkn = tkn_string.tokens[i+1];
				if(next_tkn.type == TKN_CONST)
				{
					int num = parse_arg_of_mnem(tknzr, &i);
					if(num > 0xFF)
					{
						printf("The number is bigger than 0xFF for POKE_W. Abort.\n");
						found_err = 1;
					}
					token_t new_token = *tkn;
					new_token.type = TKN_POKE_IMM_WORD;
					new_token.val.num[0] = num;
					push_token(tknzr2, new_token);
					continue;
				}
				push_token(tknzr2, *tkn);
				continue;
			}
			
			case TKN_RDD:
			{
				token_t next_tkn = tkn_string.tokens[i+1];
				if(next_tkn.type == TKN_CONST)
				{
					int num = parse_arg_of_mnem(tknzr, &i);
					if(num > 0xFFFF)
					{
						printf("The number is bigger than 0xFFFF for immediate RDD. Abort.\n");
						found_err = 1;
					}
					token_t new_token = *tkn;
					new_token.type = TKN_RDD_IMM;
					new_token.val.num[0] = num;
					push_token(tknzr2, new_token);
					continue;
				}
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_RDD_WORD:
			{
				token_t next_tkn = tkn_string.tokens[i+1];
				if(next_tkn.type == TKN_CONST)
				{
					int num = parse_arg_of_mnem(tknzr, &i);
					if(num > 0xFFFF)
					{
						printf("The number is bigger than 0xFFFF for immediate RDD_W. Abort.\n");
						found_err = 1;
					}
					token_t new_token = *tkn;
					new_token.type = TKN_RDD_IMM_WORD;
					new_token.val.num[0] = num;
					push_token(tknzr2, new_token);
					continue;
				}
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_AND:
			{
				token_t next_tkn = tkn_string.tokens[i+1];
				if(next_tkn.type == TKN_CONST)
				{
					int num = parse_arg_of_mnem(tknzr, &i);
					if(num > 0xFF)
					{
						printf("The number is bigger than 0xFF for immediate AND. Abort.\n");
						found_err = 1;
					}
					token_t new_token = *tkn;
					new_token.type = TKN_AND_IMM;
					new_token.val.num[0] = num;
					push_token(tknzr2, new_token);
					continue;
				}
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_OR:
			{
				token_t next_tkn = tkn_string.tokens[i+1];
				if(next_tkn.type == TKN_CONST)
				{
					int num = parse_arg_of_mnem(tknzr, &i);
					if(num > 0xFF)
					{
						printf("The number is bigger than 0xFF for immediate OR. Abort.\n");
						found_err = 1;
					}
					token_t new_token = *tkn;
					new_token.type = TKN_OR_IMM;
					new_token.val.num[0] = num;
					push_token(tknzr2, new_token);
					continue;
				}
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_XOR:
			{
				token_t next_tkn = tkn_string.tokens[i+1];
				if(next_tkn.type == TKN_CONST)
				{
					int num = parse_arg_of_mnem(tknzr, &i);
					if(num > 0xFF)
					{
						printf("The number is bigger than 0xFF for immediate XOR. Abort.\n");
						found_err = 1;
					}
					token_t new_token = *tkn;
					new_token.type = TKN_XOR_IMM;
					new_token.val.num[0] = num;
					push_token(tknzr2, new_token);
					
					continue;
				}
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_GT:
			{
				token_t next_tkn = tkn_string.tokens[i+1];
				if(next_tkn.type == TKN_CONST)
				{
					int num = parse_arg_of_mnem(tknzr, &i);
					if(num > 0xFF)
					{
						printf("The number is bigger than 0xFF for immediate GT. Abort.\n");
						found_err = 1;
					}
					token_t new_token = *tkn;
					new_token.type = TKN_GT_IMM;
					new_token.val.num[0] = num;
					push_token(tknzr2, new_token);
					continue;
				}
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_LT:
			{
				token_t next_tkn = tkn_string.tokens[i+1];
				if(next_tkn.type == TKN_CONST)
				{
					int num = parse_arg_of_mnem(tknzr, &i);					
					if(num > 0xFF)
					{
						printf("The number is bigger than 0xFF for immediate LT. Abort.\n");
						found_err = 1;
					}
					token_t new_token = *tkn;
					new_token.type = TKN_LT_IMM;
					new_token.val.num[0] = num;
					push_token(tknzr2, new_token);
					continue;
				}
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_EQ:
			{
				token_t next_tkn = tkn_string.tokens[i+1];
				if(next_tkn.type == TKN_CONST)
				{
					int num = parse_arg_of_mnem(tknzr, &i);
					if(num > 0xFF)
					{
						printf("The number is bigger than 0xFF for immediate EQ. Abort.\n");
						found_err = 1;
					}
					token_t new_token = *tkn;
					new_token.type = TKN_EQ_IMM;
					new_token.val.num[0] = num;
					push_token(tknzr2, new_token);
					continue;
				}
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_GE:
			{
				token_t next_tkn = tkn_string.tokens[i+1];
				if(next_tkn.type == TKN_CONST)
				{
					int num = parse_arg_of_mnem(tknzr, &i);
					if(num > 0xFF)
					{
						printf("The number is bigger than 0xFF for immediate GE. Abort.\n");
						found_err = 1;
					}
					token_t new_token = *tkn;
					new_token.type = TKN_GE_IMM;
					new_token.val.num[0] = num;
					push_token(tknzr2, new_token);
					continue;
				}
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_LE:
			{
				token_t next_tkn = tkn_string.tokens[i+1];
				if(next_tkn.type == TKN_CONST)
				{
					int num = parse_arg_of_mnem(tknzr, &i);
					if(num > 0xFF)
					{
						printf("The number is bigger than 0xFF for immediate LE. Abort.\n");
						found_err = 1;
					}
					token_t new_token = *tkn;
					new_token.type = TKN_LE_IMM;
					new_token.val.num[0] = num;
					push_token(tknzr2, new_token);
					continue;
				}
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_ROT:
			{
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_OVER:
			{
				push_token(tknzr2, *tkn);
				continue;
			} 
			case TKN_ASM:
			{
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_LABEL:
			{
				int idx = label_in_labels(tkn->tkn_str);
				if(idx == -1)
				{
					printf("Unreachable tho\n");
					found_err = 1;
				}
				token_t new_token = (token_t){.type = TKN_LABEL, .val = {.num[0] = idx}};
				push_token(tknzr2, new_token);
				continue;	
			} 
			default:
				break;
		}
		// if(label_in_labels(tkn->tkn_str) != -1)
		// {
		// 	push_token(tknzr2, *tkn);
		// 	continue;
		// }

		printf("Invalid Operand/Opcode/Mnemonic %s\n", tkn_string.tokens[i].tkn_str);
		found_err = 1;
	}
	// free_tkn(tknzr);
	*tknzr = tknzr_tmp;
	return found_err;
}
