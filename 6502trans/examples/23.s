	tmp = $F2
	tmp2 = $F3
	rwAddr1 = $F4
	rwAddr2 = $F5
	.org $8000
	lda #$00
	sta $F0
	lda #$07
	sta $F1
	lda #$03
	sta $0200
	lda #$04
	sta $2001
	brk
	
OP_PUSH:
	ldy #$00
	sta ($F0), Y
	inc $F0
	bne push_skip_inc
	inc $F1
push_skip_inc:
	rts
	
OP_POP:
	ldy #$00
	dec $F0
	lda $F0
	cmp #$FF
	bne pop_skip_dec
	dec $F1
pop_skip_dec:
	lda ($F0), Y
	rts
	
OP_ADD:
	ldy #$00
	jsr OP_POP
	sta tmp
	jsr OP_POP
	clc
	adc tmp
	jsr OP_PUSH
	rts
	
OP_SUB:
	ldy #$00
	jsr OP_POP
	sta tmp
	jsr OP_POP
	sec
	sbc tmp
	jsr OP_PUSH
	rts
	
OP_RSH:
	ldy #$00
	jsr OP_POP
	lsr A
	jsr OP_PUSH
	rts
	
OP_LSH:
	ldy #$00
	jsr OP_POP
	asl A
	jsr OP_PUSH
	rts
	
OP_DUP:
	ldy #$00
	jsr OP_POP
	sta tmp
	jsr OP_PUSH
	lda tmp
	jsr OP_PUSH
	rts
	
OP_SWP:
	ldy #$00
	jsr OP_POP
	sta tmp
	jsr OP_POP
	sta tmp2
	lda tmp
	jsr OP_PUSH
	lda tmp2
	jsr OP_PUSH
	rts
	
OP_INC:
	ldy #$00
	jsr OP_POP
	clc
	adc #$01
	jsr OP_PUSH
	rts
	
OP_DEC:
	ldy #$00
	jsr OP_POP
	sec
	sbc #$01
	jsr OP_PUSH
	rts
	
OP_WRD:
	ldy #$00
	jsr OP_POP
	sta rwAddr2
	jsr OP_POP
	sta rwAddr1
	jsr OP_POP
	sta (rwAddr1), Y
	rts
	
OP_RDD:
	ldy #$00
	jsr OP_POP
	sta rwAddr2
	jsr OP_POP
	sta rwAddr1
	lda (rwAddr1), Y
	jsr OP_PUSH
	rts
OP_AND:
	ldy #$00
	jsr OP_POP
	sta tmp
	jsr OP_POP
	and tmp
	jsr OP_PUSH
	rts
OP_OR:
	ldy #$00
	jsr OP_POP
	sta tmp
	jsr OP_POP
	ora tmp
	jsr OP_PUSH
	rts
OP_XOR:
	ldy #$00
	jsr OP_POP
	sta tmp
	jsr OP_POP
	eor tmp
	jsr OP_PUSH
	rts