#include "tokenizing.h"
#include "labels.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syntax_checker.h"

int check_syntax(tokenizer_t *tknzr)
{
	tokenizer_t tknzr_tmp = init_tkn(NULL);
	tokenizer_t *tknzr2	= &tknzr_tmp;
	token_t* tkn_strng = tknzr->tokens;
	size_t size_of_str = tknzr->size;
	int found_err = 0;
	for(size_t i = 0; i < size_of_str; i++)
	{
		token_t *tkn = &tkn_strng[i];
		switch(tkn->type)
		{
			case TKN_PSH:
			{
				token_t next_tok = tknzr->tokens[++i];
				if(next_tok.type != TKN_CONST)
				{
					printf("Not a valid digit operand after PSH: %s\n", next_tok.tkn_str);
					found_err = 1;
					continue;
				}
				uint32_t num = strtoul(next_tok.tkn_str, NULL, 10);
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
			
			case TKN_JMP:
			{
				token_t next_tok = tknzr->tokens[++i];
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
				token_t next_tok = tknzr->tokens[++i];
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
				token_t next_tok = tknzr->tokens[++i];
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
				token_t next_tok = tknzr->tokens[++i];
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
				token_t next_tok = tknzr->tokens[++i];
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
				token_t next_tok = tknzr->tokens[++i];
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
			case TKN_ADD:
			{
				token_t next_tkn = tknzr->tokens[i+1];
				if(next_tkn.type == TKN_CONST)
				{
					if(strtoul(next_tkn.tkn_str, NULL, 10) >= 0x100)
					{
						printf("The number is bigger than 0xFF for immediate ADD. Abort.\n");
						found_err = 1;
					}
					token_t new_token = *tkn;
					new_token.type = TKN_ADD_IMM;
					new_token.val.num[0] = strtoul(next_tkn.tkn_str, NULL, 10);
					push_token(tknzr2, new_token);
					i++;
					continue;
				}
				push_token(tknzr2, *tkn);
	
				continue;
			}
			case TKN_SUB:
			{
				token_t next_tkn = tknzr->tokens[i+1];
				if(next_tkn.type == TKN_CONST)
				{
					if(strtoul(next_tkn.tkn_str, NULL, 10) >= 0x100)
					{
						printf("The number is bigger than 0xFF for immediate SUB. Abort.\n");
						found_err = 1;
					}
					token_t new_token = *tkn;
					new_token.type = TKN_SUB_IMM;
					new_token.val.num[0] = strtoul(next_tkn.tkn_str, NULL, 10);
					push_token(tknzr2, new_token);
					i++;
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
				token_t next_tkn = tknzr->tokens[i+1];
				if(next_tkn.type == TKN_CONST)
				{
					if(strtoul(next_tkn.tkn_str, NULL, 10) >= 0x10000)
					{
						printf("The adress is bigger than 0xFFFF and is not suitable for immediate WRD. Abort.\n");
						found_err = 1;
					}
					token_t new_token = *tkn;
					new_token.type = TKN_WRD_IMM;
					new_token.val.num[0] = strtoul(next_tkn.tkn_str, NULL, 10);
					push_token(tknzr2, new_token);
					i++;
					continue;
				}
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_WRD_WORD:
			{
				token_t next_tkn = tknzr->tokens[i+1];
				if(next_tkn.type == TKN_CONST)
				{
					if(strtoul(next_tkn.tkn_str, NULL, 10) >= 0x10000)
					{
						printf("The adress is bigger than 0xFFFF and is not suitable for immediate WRD_W. Abort.\n");
						found_err = 1;
					}
					token_t new_token = *tkn;
					new_token.type = TKN_WRD_WORD_IMM;
					new_token.val.num[0] = strtoul(next_tkn.tkn_str, NULL, 10);
					push_token(tknzr2, new_token);
					i++;
					continue;
				}
				push_token(tknzr2, *tkn);
				continue;
			}
			
			case TKN_RDD:
			{
				token_t next_tkn = tknzr->tokens[i+1];
				if(next_tkn.type == TKN_CONST)
				{
					if(strtoul(next_tkn.tkn_str, NULL, 10) >= 0x10000)
					{
						printf("The adress is bigger than 0xFFFF and is not suitable for immediate RDD. Abort.\n");
						found_err = 1;
					}
					token_t new_token = *tkn;
					new_token.type = TKN_RDD_IMM;
					new_token.val.num[0] = strtoul(next_tkn.tkn_str, NULL, 10);
					push_token(tknzr2, new_token);
					i++;
					continue;
				}
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_AND:
			{
				token_t next_tkn = tknzr->tokens[i+1];
				if(next_tkn.type == TKN_CONST)
				{
					if(strtoul(next_tkn.tkn_str, NULL, 10) >= 0x100)
					{
						printf("The number is bigger than 0xFF for immediate AND. Abort.\n");
						found_err = 1;
					}
					token_t new_token = *tkn;
					new_token.type = TKN_AND_IMM;
					new_token.val.num[0] = strtoul(next_tkn.tkn_str, NULL, 10);
					push_token(tknzr2, new_token);
					i++;
					continue;
				}
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_OR:
			{
				token_t next_tkn = tknzr->tokens[i+1];
				if(next_tkn.type == TKN_CONST)
				{
					if(strtoul(next_tkn.tkn_str, NULL, 10) >= 0x100)
					{
						printf("The number is bigger than 0xFF for immediate OR. Abort.\n");
						found_err = 1;
					}
					token_t new_token = *tkn;
					new_token.type = TKN_OR_IMM;
					new_token.val.num[0] = strtoul(next_tkn.tkn_str, NULL, 10);
					push_token(tknzr2, new_token);
					i++;
					continue;
				}
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_XOR:
			{
				token_t next_tkn = tknzr->tokens[i+1];
				if(next_tkn.type == TKN_CONST)
				{
					if(strtoul(next_tkn.tkn_str, NULL, 10) >= 0x100)
					{
						printf("The number is bigger than 0xFF for immediate XOR. Abort.\n");
						found_err = 1;
					}
					token_t new_token = *tkn;
					new_token.type = TKN_XOR_IMM;
					new_token.val.num[0] = strtoul(next_tkn.tkn_str, NULL, 10);
					push_token(tknzr2, new_token);
					i++;
					continue;
				}
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_GT:
			{
				token_t next_tkn = tknzr->tokens[i+1];
				if(next_tkn.type == TKN_CONST)
				{
					if(strtoul(next_tkn.tkn_str, NULL, 10) > 0xFF)
					{
						printf("The number is bigger than 0xFF for immediate GT. Abort.\n");
						found_err = 1;
					}
					token_t new_token = *tkn;
					new_token.type = TKN_GT_IMM;
					new_token.val.num[0] = strtoul(next_tkn.tkn_str, NULL, 10);
					push_token(tknzr2, new_token);
					i++;
					continue;
				}
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_LT:
			{
				token_t next_tkn = tknzr->tokens[i+1];
				if(next_tkn.type == TKN_CONST)
				{
					if(strtoul(next_tkn.tkn_str, NULL, 10) > 0xFF)
					{
						printf("The number is bigger than 0xFF for immediate LT. Abort.\n");
						found_err = 1;
					}
					token_t new_token = *tkn;
					new_token.type = TKN_LT_IMM;
					new_token.val.num[0] = strtoul(next_tkn.tkn_str, NULL, 10);
					push_token(tknzr2, new_token);
					i++;
					continue;
				}
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_EQ:
			{
				token_t next_tkn = tknzr->tokens[i+1];
				if(next_tkn.type == TKN_CONST)
				{
					if(strtoul(next_tkn.tkn_str, NULL, 10) > 0xFF)
					{
						printf("The number is bigger than 0xFF for immediate EQ. Abort.\n");
						found_err = 1;
					}
					token_t new_token = *tkn;
					new_token.type = TKN_EQ_IMM;
					new_token.val.num[0] = strtoul(next_tkn.tkn_str, NULL, 10);
					push_token(tknzr2, new_token);
					i++;
					continue;
				}
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_GE:
			{
				token_t next_tkn = tknzr->tokens[i+1];
				if(next_tkn.type == TKN_CONST)
				{
					if(strtoul(next_tkn.tkn_str, NULL, 10) > 0xFF)
					{
						printf("The number is bigger than 0xFF for immediate GE. Abort.\n");
						found_err = 1;
					}
					token_t new_token = *tkn;
					new_token.type = TKN_GE_IMM;
					new_token.val.num[0] = strtoul(next_tkn.tkn_str, NULL, 10);
					push_token(tknzr2, new_token);
					i++;
					continue;
				}
				push_token(tknzr2, *tkn);
				continue;
			}
			case TKN_LE:
			{
				token_t next_tkn = tknzr->tokens[i+1];
				if(next_tkn.type == TKN_CONST)
				{
					if(strtoul(next_tkn.tkn_str, NULL, 10) > 0xFF)
					{
						printf("The number is bigger than 0xFF for immediate LE. Abort.\n");
						found_err = 1;
					}
					token_t new_token = *tkn;
					new_token.type = TKN_LE_IMM;
					new_token.val.num[0] = strtoul(next_tkn.tkn_str, NULL, 10);
					push_token(tknzr2, new_token);
					i++;
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

		printf("Invalid Operand/Opcode/Mnemonic %s\n", tkn_strng[i].tkn_str);
		found_err = 1;
	}
	free_tkn(tknzr);
	*tknzr = tknzr_tmp;
	return found_err;
}
