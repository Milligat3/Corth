	tmp = $F2
	tmp2 = $F3
	rwAddr1 = $F4
	rwAddr2 = $F5
	.org $8000
	lda #$00
	sta $F0
	lda #$07
	sta $F1
	lda #0
	jsr OP_PUSH
	jsr OP_POP
	ldy #$00
	lda ($F0), Y
	sta 16
	lda #2
	jsr OP_PUSH
	jsr OP_POP
	ldy #$00
	lda ($F0), Y
	sta 17
	lda #3
	jsr OP_PUSH
LOOP:
	jsr OP_DUP
	jsr WRITE
	jmp LOOP
	brk
WRITE:
	lda 16
	jsr OP_PUSH
	lda 17
	jsr OP_PUSH
	jsr OP_WRD
	lda 16
	jsr OP_PUSH
	jsr OP_INC
	jsr OP_DUP
	jsr OP_POP
	ldy #$00
	lda ($F0), Y
	sta 16
	ldy #$00
	jsr OP_POP
	lda ($F0), Y
	beq INC_HI
	rts
INC_HI:
	lda 17
	jsr OP_PUSH
	jsr OP_INC
	jsr OP_DUP
	jsr OP_POP
	ldy #$00
	lda ($F0), Y
	sec
	sbc #6
	jsr OP_PUSH
	ldy #$00
	jsr OP_POP
	lda ($F0), Y
	beq RESET_HI
	jsr OP_POP
	ldy #$00
	lda ($F0), Y
	sta 17
	rts
RESET_HI:
	lda #2
	jsr OP_PUSH
	jsr OP_POP
	ldy #$00
	lda ($F0), Y
	sta 17
	jsr OP_POP
	rts
	
OP_PUSH:
	ldy #$00
	sta ($F0), Y
	clc
	lda $F0
	adc #$01
	sta $F0
	lda $F1
	adc #$00
	sta $F1
	rts
	
OP_POP:
	ldy #$00
	sec
	lda $F0
	sbc #$01
	sta $F0
	lda $F1
	sbc #$00
	sta $F1
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
