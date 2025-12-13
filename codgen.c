#include "codgen.h"
#include "tokenizing.h"
#include "asmgen.h"
#include "labels.h"
#include <stdio.h>

int output_str(FILE* output, token_table_t *tknzr)
{
	token_t* tkn_strng = tknzr->tokens;
	size_t size_of_str = tknzr->size;
	
	fprintf(output, "	tmp = $F2\n	tmp2 = $F3\n	rwAddr1 = $F4\n	rwAddr2 = $F5\n	.org $8000\n");
	for(size_t i = 0; i < size_of_str; i++)
	{
		token_t tkn = tkn_strng[i];
		switch(tkn.type){
			case TKN_PSH:
			{
				fprintf(output, "	lda #$%X\n	jsr OP_PUSH\n", tkn.val.num[0]);
				continue;
			}
			case TKN_PSH_WORD:
			{
				fprintf(output, "	lda #$%X\n	jsr OP_PUSH\n	lda #$%X\n	jsr OP_PUSH\n", tkn.val.num[0], tkn.val.num[1]);
				continue;
			}
			case TKN_POP:
			{
				fprintf(output, "	jsr OP_POP\n");
	
				continue;
			}
			case TKN_ADD:
			{
				fprintf(output, "	jsr OP_ADD\n");
				continue;
			}
			case TKN_ADD_IMM:
			{
				fprintf(output, "	dex\n	lda $700, X\n	clc\n	adc #$%X\n	sta $700, X\n	inx\n", tkn.val.num[0]);
				continue;
			}
			case TKN_SUB:
			{
				fprintf(output, "	jsr OP_SUB\n");
				i++;
				continue;
			}
			case TKN_SUB_IMM:
			{
				fprintf(output, "	dex\n	lda $700, X\n	sec\n	sbc #$%X\n	sta $700, X\n	inx\n", tkn.val.num[0]);
				continue;
			}
			case TKN_RSH:
			{
				fprintf(output, "	jsr OP_RSH\n");
	
				continue;
			}
			case TKN_LSH:
			{
				fprintf(output, "	jsr OP_LSH\n");
				continue;
			}
			case TKN_JMP:
			{
				fprintf(output, "	jmp %s\n", labl_tabl.labels[tkn.val.num[0]].label_name);
				continue;
			}
			case TKN_JNZ:
			{
				fprintf(output, "	dex\n	lda $700, X\n	beq *+5\n	jmp %s\n", labl_tabl.labels[tkn.val.num[0]].label_name);
				continue;
			}
			case TKN_JZ:
			{
				fprintf(output, "	dex\n	lda $700, X\n	bne *+5\n	jmp %s\n", labl_tabl.labels[tkn.val.num[0]].label_name);
				continue;
			}
			case TKN_JCC:
			{
				fprintf(output, "	bcs *+5\n	jmp %s\n", labl_tabl.labels[tkn.val.num[0]].label_name);
				continue;
			}
			case TKN_JCS:
			{
				fprintf(output, "	bcc *+5\n	jmp %s\n", labl_tabl.labels[tkn.val.num[0]].label_name);
				continue;
			}
			case TKN_JSR:
			{
				fprintf(output, "	jsr %s\n", labl_tabl.labels[tkn.val.num[0]].label_name);
				continue;
			}
			case TKN_RTS:
			{
				fprintf(output, "	rts\n");
				continue;
			}
			case TKN_HLT:
			{
				fprintf(output, "	brk\n");
				continue;
			}
			
			case TKN_DUP:
			{
				fprintf(output, "	jsr OP_DUP\n");
				continue;
			}
			case TKN_SWP:
			{
				fprintf(output, "	jsr OP_SWP\n");
				continue;
			}
			case TKN_INC:
			{
				fprintf(output, "	jsr OP_INC\n");
				continue;
			}
			case TKN_DEC:
			{
				fprintf(output, "	jsr OP_DEC\n");
				continue;
			}
			case TKN_WRD:
			{
				fprintf(output, "	jsr OP_WRD\n");
				continue;
			}
			case TKN_WRD_IMM:
			{
				fprintf(output, "	dex\n	lda $700, X\n	sta $%X\n", tkn.val.num[0]);
				continue;
			}
			case TKN_WRD_WORD:
			{
				fprintf(output, "	jsr OP_WRD_WORD\n");
				continue;
			}
			case TKN_WRD_WORD_IMM:
			{
				fprintf(output, "	dex\n	lda $700, X\n	sta $%X\n", tkn.val.num[0]+1);
				fprintf(output, "	dex\n	lda $700, X\n	sta $%X\n", tkn.val.num[0]);
				continue;
			}
			case TKN_PSH3_WRD_OPT:
			{
				fprintf(output, "	lda #$%02X\n	sta $%X\n", tkn.val.num[0], tkn.val.num[1]);
				continue;
			}
			case TKN_RDD:
			{ 
				fprintf(output, "	jsr OP_RDD\n");
				continue;
			}
			case TKN_RDD_IMM:
			{
				fprintf(output, "	lda $%X\n	jsr OP_PUSH\n", tkn.val.num[0]);
				continue;
			}
			case TKN_RDD2_WRD:
			{
				fprintf(output, "	lda $%X\n	sta rwAddr1\n	lda $%X\n	sta rwAddr2\n	dex\n	lda $700, X\n	ldy #$00\n	sta (rwAddr1), Y\n", tkn.val.num[0], tkn.val.num[1]);
				continue;
			}
			case TKN_PSH_RDD2_WRD:
			{
				fprintf(output, "	lda $%X\n	sta rwAddr1\n	lda $%X\n	sta rwAddr2\n	lda #$%X\n	ldy #$00\n	sta (rwAddr1), Y\n", tkn.val.num[0], tkn.val.num[1], tkn.val.num[2]);
				continue;
			}
			case TKN_AND:
			{ 
				fprintf(output, "	jsr OP_AND\n");
				continue;
			}
			case TKN_AND_IMM:
			{
				fprintf(output, "	dex\n	lda $700, X\n	and #$%X\n	sta $700, X\n	inx\n", tkn.val.num[0]);
				continue;
			}
			case TKN_OR:
			{ 
				fprintf(output, "	jsr OP_OR\n");
				continue;
			}
			case TKN_OR_IMM:
			{
				fprintf(output, "	dex\n	lda $700, X\n	ora #$%X\n	sta $700, X\n	inx\n", tkn.val.num[0]);
				continue;
			}
			case TKN_XOR:
			{ 
				fprintf(output, "	jsr OP_XOR\n");
				continue;
			}
			case TKN_XOR_IMM:
			{
				fprintf(output, "	dex\n	lda $700, X\n	eor #$%X\n	sta $700, X\n	inx\n", tkn.val.num[0]);
				continue;
			}
			case TKN_RDD_INC_DUP_WRD_OPT:
			{
				fprintf(output, "	lda $%X\n	clc\n	adc #$1\n	sta $%X\n	sta $700, X\n	inx\n", tkn.val.num[0], tkn.val.num[1]);
				continue;
			}
			case TKN_GT:
			{
				fprintf(output, "	jsr OP_GT\n");
				continue;
			}
			case TKN_LT:
			{
				fprintf(output, "	jsr OP_LT\n");
				continue;
			}
			case TKN_EQ:
			{
				fprintf(output, "	jsr OP_EQ\n");
				continue;
			}
			case TKN_GE:
			{
				fprintf(output, "	jsr OP_GE\n");
				continue;
			}
			case TKN_LE:
			{
				fprintf(output, "	jsr OP_LE\n");
				continue;
			}
			case TKN_GT_IMM:
			{
				fprintf(output, "	lda #$%X\n sta $700, X\n\
					inx\n	jsr OP_GT\n", tkn.val.num[0]);
				continue;
			}
			case TKN_LT_IMM:
			{
				fprintf(output, "	lda #$%X\n sta $700, X\n\
					inx\n	jsr OP_LT\n", tkn.val.num[0]);
				continue;
			}
			case TKN_EQ_IMM:
			{
				fprintf(output, "	lda #$%X\n sta $700, X\n\
					inx\n	jsr OP_EQ\n", tkn.val.num[0]);
				continue;
			}
			case TKN_GE_IMM:
			{
				fprintf(output, "	lda #$%X\n sta $700, X\n\
					inx\n	jsr OP_GE\n", tkn.val.num[0]);
				continue;
			}
			case TKN_LE_IMM:
			{
				fprintf(output, "	lda #$%X\n sta $700, X\n\
					inx\n	jsr OP_LE\n", tkn.val.num[0]);
				continue;
			}
			case TKN_ROT:
			{
				fprintf(output, "	jsr OP_ROT\n");
				continue;
			}
			case TKN_OVER:
			{
				fprintf(output, "	jsr OP_OVER\n");
				continue;
			}
			case TKN_ASM:
			{
				fprintf(output, "%s\n", asm_table.table[tkn.val.num[0]]);
				continue;
			}
			case TKN_LABEL:
			{
				fprintf(output, "%s:\n", labl_tabl.labels[tkn.val.num[0]].label_name);
				continue;
			}
			default:
				break;
		}
		// if(label_in_labels(tkn.tkn_str) != -1)
		// {
		// 	fprintf(output, "%s\n", tkn.tkn_str);
		// 	continue;
		// }
		
		printf("Invalid Operand/Opcode/Mnemonic %s", tkn_strng[i].tkn_str);
		fclose(output);
		return 0;
	}
	fprintf(output, 
	"	\nOP_PUSH:\n\
	sta $700, X\n\
	inx\n\
	rts\n\
	\n\
OP_POP:\n\
	dex\n\
	lda $700, X\n\
	rts\n\
	\n\
OP_ADD:\n\
	dex\n\
	lda $700, X\n\
	dex\n\
	sta tmp\n\
	lda $700, X\n\
	clc\n\
	adc tmp\n\
	sta $700, X\n\
	inx\n\
	rts\n\
	\n\
OP_SUB:\n\
	dex\n\
	lda $700, X\n\
	dex\n\
	sta tmp\n\
	lda $700, X\n\
	sec\n\
	sbc tmp\n\
	sta $700, X\n\
	inx\n\
	rts\n\
	\n\
OP_RSH:\n\
	dex\n\
	lsr $700, X\n\
	inx\n\
	rts\n\
OP_LSH:\n\
	dex\n\
	asl $700, X\n\
	inx\n\
	rts\n\
	\n\
OP_DUP:\n\
	dex\n\
	lda $700, X\n\
	inx\n\
	sta $700, X\n\
	inx\n\
	rts\n\
	\n\
OP_SWP:\n\
	dex\n\
	lda $700, X\n\
	dex\n\
	ldy $700, X\n\
	sta $700, X\n\
	inx\n\
	tya\n\
	sta $700, X\n\
	inx\n\
	rts\n\
	\n\
OP_INC:\n\
	dex\n\
	inc $700, X\n\
	inx\n\
	rts\n\
	\n\
OP_DEC:\n\
	dex\n\
	dec $700, X\n\
	inx\n\
	rts\n\
	\n\
OP_WRD:\n\
	dex\n\
	lda $700, X\n\
	sta rwAddr2\n\
	dex\n\
	lda $700, X\n\
	sta rwAddr1\n\
	dex\n\
	lda $700, X\n\
	ldy #$00\n\
	sta (rwAddr1), Y\n\
	rts\n\
	\n\
OP_WRD_WORD:\n\
	dex\n\
	lda $700, X\n\
	sta rwAddr2\n\
	dex\n\
	lda $700, X\n\
	sta rwAddr1\n\
	ldy #$01\n\
	dex\n\
	lda $700, X\n\
	sta (rwAddr1), Y\n\
	dey\n\
	dex\n\
	lda $700, X\n\
	sta (rwAddr1), Y\n\
	rts\n\
OP_RDD:\n\
	dex\n\
	lda $700, X\n\
	sta rwAddr2\n\
	dex\n\
	lda $700, X\n\
	sta rwAddr1\n\
	ldy #$00\n\
	lda (rwAddr1), Y\n\
	sta $700, X\n\
	inx\n\
	rts\n\
	\n\
OP_AND:\n\
	dex\n\
	lda $700, X\n\
	dex\n\
	and $700, X\n\
	sta $700, X\n\
	inx\n\
	rts\n\
	\n\
OP_OR:\n\
	dex\n\
	lda $700, X\n\
	dex\n\
	ora $700, X\n\
	sta $700, X\n\
	inx\n\
	rts\n\
	\n\
OP_XOR:\n\
	dex\n\
	lda $700, X\n\
	dex\n\
	eor $700, X\n\
	sta $700, X\n\
	inx\n\
	rts\n\
OP_GT:\n\
	dex\n\
	lda $700, X\n\
	dex\n\
	cmp $700, X\n\
	bcc LOG_PSH_1\n\
	beq LOG_PSH_1\n\
	jmp LOG_PSH_0\n\
OP_LT:\n\
	dex\n\
	lda $700, X\n\
	dex\n\
	cmp $700, X\n\
	bcs LOG_PSH_1\n\
	jmp LOG_PSH_0\n\
OP_EQ:\n\
	dex\n\
	lda $700, X\n\
	dex\n\
	cmp $700, X\n\
	beq LOG_PSH_1\n\
	jmp LOG_PSH_0\n\
OP_GE:\n\
	dex\n\
	lda $700, X\n\
	dex\n\
	cmp $700, X\n\
	bcc LOG_PSH_1\n\
	jmp LOG_PSH_0\n\
OP_LE:\n\
	dex\n\
	lda $700, X\n\
	dex\n\
	cmp $700, X\n\
	beq LOG_PSH_0\n\
	bcs LOG_PSH_1\n\
LOG_PSH_1:\n\
	lda #$1\n\
	sta $700, X\n\
	inx\n\
	rts\n\
LOG_PSH_0:\n\
	lda #$0\n\
	sta $700, X\n\
	inx\n\
	rts\n\
OP_OVER:\n\
	dex\n\
	dex\n\
	lda $700, X\n\
	inx\n\
	inx\n\
	sta $700, X\n\
	inx\n\
	rts\n\
OP_ROT:\n\
	dex\n\
	lda $700, X\n\
	pha\n\
	dex\n\
	lda $700, X\n\
	pha\n\
	dex\n\
	lda $700, X\n\
	tay\n\
	pla\n\
	sta $700, X\n\
	inx\n\
	pla\n\
	sta $700, X\n\
	inx\n\
	tya\n\
	sta $700, X\n\
	inx\n\
	rts");
	return 1;
}