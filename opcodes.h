// opcodes.h
typedef enum
{
	OP_UNKNOWN,
	OP_PSH,
	OP_POP,
	OP_ADD,
	OP_SUB,
	OP_DIV,
	OP_MUL,
	OP_RSH,
	OP_LSH,
	OP_JMP,
	OP_JNZ,
	OP_JZ,
	OP_JSR,
	OP_RTS,
	OP_DUP,
	OP_SWP,
	OP_INC,
	OP_DEC,
	OP_OUT,
	OP_STR,
	OP_WRD,
	OP_RDD,
	OP_HLT
}opcode_t;