# Corth
Stack-based assembly-like language for 6502 implemented in C. (CorthCOMP.c)

Has nothing much for now:

PSH <number>(8-bit or 16-bit)

POP (Pops into accumulator, but you can't use it through lang itself, only ASM blocks)

ADD and ADD <number> (only 8-bit)

SUB (same as ADD)

LSH 

RSH (bitshifts top of the stack left/right)

WRD (without arguments uses three items from stack [addr_hi, addr_lo, value] if you consider addr_hi is the top of the stack)

WRD <addr> (Writes a number from the top of the stack to the memory address)

WRD_W <addr> (Writes a word from the top of the stack to the memory address) (wasn't tested tho)

RDD (without arguments uses two items from stack [addr_hi, addr_lo] (same order as in WRD)

RDD <addr> (Reads a number from the memory and pushes onto the stack)

JMP <label> 

JSR <label> (Jump to subroutine)

RTS (Return from subroutine)

JZ/JNZ <label> (Jump if Zero/Not Zero) (pops an item from the top of the stack)

JCC/JCS <label> (Jump if Carry Clear/Set) (Doesn't pop an item)

GT/GT <number> (Greater than. Compares item under the top of the stack and the number on the top of the stack)

LT/LT <number> (Same as GT)

EQ/EQ <number>

GE/GE <number> 

LE/LE <number>

ROT (Rotates top of the stack)

SWP (Swaps the top and item under it)

DUP (Duplicates number on the top)

OVR (Duplicates number from under the top of the stack onto the top)

ASM { <asm_code> } ("Inline" assembly, used as is, as a full block)
```
@macro name (arg1, arg2, arg3) 
	body 
@macro_end 
```
(Macro system. You can use up to 8 arguments, argument can be a sequence of tokens, that allows you to make things like showed below)

```
@macro FOR(i, start, end, body)
        PSH start
        WRD i
        LABEL loop
        body
        RDD i
        INC
        DUP
        WRD i
        LE end
        JNZ USE_LABEL loop
@macro_end


@macro DRAW_LINE(col, hi, lo)
        FOR($1, 0, 32,
                PSH col
                RDD $1
                PSH lo
                ADD
                PSH hi
                WRD
        )
@macro_end


DRAW_LINE(1, 2, $10*0)
DRAW_LINE(2, 2, $10*2)
DRAW_LINE(3, 3, $10*4)
PSH 1
WRD $2001
TRAP: JMP TRAP
```

To compile simply use make and gcc

