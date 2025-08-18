# Corth
Stack-based assembly-like language implemented in C. (CorthCOMP.c)

Stack-based VM implemented in C too! (CorthREPL.c)

Has nothing much for now:

PSH <number>

POP (Works like "." in Forth, but actually pops a number)

ADD

SUB

MUL

DIV

LSH

RSH // Possibly will change to SHL/SHR

JSR/RTS // As in 6502 assembly

JNZ/JZ/JMP // Conditional and unconditional branching

OUT // Outputs a number

INC/DEC // +1/-1 

DUP // Duplicates the top of the stack

SWP // Swaps top and the element under it

STR // Stores the value on top of the stack to the variables stack

RDD // Reads the value by the index off the variable stack. Usage: PSH 1 RDD - Will put whatever is on the variable stack by the index 1

WRD // Writes the value to the variable stack by the index. It doesn't store new value, it changes already stored one. Usage: PSH 42 (What you need to push) PSH 0 (Index) WRD  

to compile simply use 

gcc CorthCOMP.c -o (Prefered name)

