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

PEK // PEEK

JMP <lable>

JZ/JNZ <lable>

DUP

SWP

INC/DEC

to compile simply use 

gcc CorthCOMP.c -o (Prefered name)

