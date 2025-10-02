	tmp = $F2
	tmp2 = $F3
	rwAddr1 = $F4
	rwAddr2 = $F5
	.org $8000
	lda #$00
	sta $F0
	lda #$07
	sta $F1
	lda $402
	sta rwAddr1
	lda $200
	sta rwAddr2
	lda #$FE
	ldy #$00
	sta (rwAddr1), Y
	
OP_PUSH:
	sta $700, X
	inx
	rts
	
OP_POP:
	dex
	lda $700, X
	rts
	
OP_ADD:
	dex
	lda $700, X
	dex
	sta tmp
	lda $700, X
	clc
	adc tmp
	sta $700, X
	inx
	rts
	
OP_SUB:
	dex
	lda $700, X
	dex
	sta tmp
	lda $700, X
	sec
	sbc tmp
	sta $700, X
	inx
	rts
	
OP_RSH:
	dex
	asl $700, X
	inx
	rts
OP_LSH:
	dex
	asl $700, X
	inx
	rts
	
OP_DUP:
	dex
	lda $700, X
	inx
	sta $700, X
	inx
	rts
	
OP_SWP:
	dex
	lda $700, X
	dex
	ldy $700, X
	sta $700, X
	inx
	tya
	sta $700, X
	inx
	rts
	
OP_INC:
	dex
	inc $700, X
	inx
	rts
	
OP_DEC:
	dex
	dec $700, X
	inx
	rts
	
OP_WRD:
	dex
	lda $700, X
	sta rwAddr2
	dex
	lda $700, X
	sta rwAddr1
	dex
	lda $700, X
	ldy #$00
	sta (rwAddr1), Y
	rts
	
OP_RDD:
	dex
	lda $700, X
	sta rwAddr2
	dex
	lda $700, X
	sta rwAddr1
	ldy #$00
	lda (rwAddr1), Y
	sta $700, X
	inx
	rts
OP_AND:
	dex
	lda $700, X
	dex
	and $700, X
	sta $700, X
	inx
	rts
OP_OR:
	dex
	lda $700, X
	dex
	ora $700, X
	sta $700, X
	inx
	rts
OP_XOR:
	dex
	lda $700, X
	dex
	eor $700, X
	sta $700, X
	inx
	rts