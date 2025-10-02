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
	lda #0
	jsr OP_PUSH
	lda #1
	jsr OP_PUSH
	jsr OP_WRD
	lda #2
	jsr OP_PUSH
	lda #1
	jsr OP_PUSH
	lda #1
	jsr OP_PUSH
	jsr OP_WRD
	lda #3
	jsr OP_PUSH
LOOP:
	jsr OP_DUP
	jsr WRITE
	jmp LOOP
	brk
WRITE:
	lda #0
	jsr OP_PUSH
	lda #1
	jsr OP_PUSH
	jsr OP_RDD
	jsr OP_INC
	jsr OP_DUP
	lda #0
	jsr OP_PUSH
	lda #1
	jsr OP_PUSH
	jsr OP_WRD
	jsr OP_DUP
	ldy #$00
	jsr OP_POP
	lda ($F0), Y
	beq INC_HI
	lda #1
	jsr OP_PUSH
	lda #1
	jsr OP_PUSH
	jsr OP_RDD
ACTUAL_WRITE:
	jsr OP_WRD
	rts
INC_HI:
	lda #1
	jsr OP_PUSH
	lda #1
	jsr OP_PUSH
	jsr OP_RDD
	jsr OP_DUP
	lda #6
	jsr OP_PUSH
	jsr OP_SUB
	ldy #$00
	jsr OP_POP
	lda ($F0), Y
	beq RESET_HI
	jsr OP_INC
	jsr OP_DUP
	lda #1
	jsr OP_PUSH
	lda #1
	jsr OP_PUSH
	jsr OP_WRD
	jmp ACTUAL_WRITE
RESET_HI:
	jsr OP_POP
	lda #2
	jsr OP_PUSH
	jsr OP_DUP
	lda #1
	jsr OP_PUSH
	lda #1
	jsr OP_PUSH
	jsr OP_WRD
	jmp ACTUAL_WRITE
	
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
	sec
	lda $F0
	sbc #$01
	sta $F0
	lda $F1
	sbc #$00
	sta $F1
	rts
	
OP_ADD:
	ldy #$00
	jsr OP_POP
	lda ($F0), Y
	sta tmp
	jsr OP_POP
	lda ($F0), Y
	clc
	adc tmp
	jsr OP_PUSH
	rts
	
OP_SUB:
	ldy #$00
	jsr OP_POP
	lda ($F0), Y
	sta tmp
	jsr OP_POP
	lda ($F0), Y
	sec
	sbc tmp
	jsr OP_PUSH
	rts
	
OP_RSH:
	ldy #$00
	jsr OP_POP
	lda ($F0), Y
	lsr A
	jsr OP_PUSH
	rts
	
OP_LSH:
	ldy #$00
	jsr OP_POP
	lda ($F0), Y
	asl A
	jsr OP_PUSH
	rts
	
OP_DUP:
	ldy #$00
	jsr OP_POP
	lda ($F0), Y
	sta tmp
	jsr OP_PUSH
	lda tmp
	jsr OP_PUSH
	rts
	
OP_SWP:
	ldy #$00
	jsr OP_POP
	lda ($F0), Y
	sta tmp
	jsr OP_POP
	lda ($F0), Y
	sta tmp2
	lda tmp
	sta ($F0), Y
	lda tmp2
	jsr OP_PUSH
	rts
	
OP_INC:
	ldy #$00
	jsr OP_POP
	lda ($F0), Y
	clc
	adc #$01
	jsr OP_PUSH
	rts
	
OP_DEC:
	ldy #$00
	jsr OP_POP
	lda ($F0), Y
	sec
	sbc #$01
	jsr OP_PUSH
	rts
	
OP_WRD:
	ldy #$00
	jsr OP_POP
	lda ($F0), Y
	sta rwAddr2
	jsr OP_POP
	lda ($F0), Y
	sta rwAddr1
	jsr OP_POP
	lda ($F0), Y
	sta (rwAddr1), Y
	rts
	
OP_RDD:
	ldy #$00
	jsr OP_POP
	lda ($F0), Y
	sta rwAddr2
	jsr OP_POP
	lda ($F0), Y
	sta rwAddr1
	lda (rwAddr1), Y
	jsr OP_PUSH
	rts
