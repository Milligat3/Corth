// StackBasedVM.c

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "opcodes.h"
#define STACK_SIZE 1024

typedef struct
{
	size_t sp;
	int32_t stack[STACK_SIZE];
	uint32_t PC, size;
	uint8_t* program;
}stack_t;


// stack_t VM = {0, 0};

void push(stack_t* VM, int32_t n)
{
	if(VM->sp >= STACK_SIZE)
	{
		printf("Stack Overflow. Aborting.\n");
		return;
	}
	VM->stack[VM->sp++] = n;
}

int32_t pop(stack_t* VM)
{
	if(VM->sp == 0)
	{
		printf("Stack Underflow. Aborting.\n");
		return 0;
	}
	int32_t to_ret = VM->stack[--VM->sp];
	return to_ret;
}

void add(stack_t* VM)
{
	if(VM->sp < 2)
	{
		printf("Cannot execute addition. Not enough operands.\n");
		return;
	}
	int32_t a = pop(VM);
	int32_t b = pop(VM);
	push(VM, a + b);
}

void sub(stack_t* VM)
{
	if(VM->sp < 2)
	{
		printf("Cannot execute substraction. Not enough operands.\n");
		return;
	}
	int32_t a = pop(VM);
	int32_t b = pop(VM);
	push(VM, b - a);
}

void mul(stack_t* VM)
{
	if(VM->sp < 2)
	{
		printf("Cannot execute multiplication. Not enough operands.\n");
		return;
	}
	int32_t a = pop(VM);
	int32_t b = pop(VM);
	push(VM, a * b);
}

void divis(stack_t* VM)
{
	if(VM->sp < 2)
	{
		printf("Cannot execute division. Not enough operands.\n");
		return;
	}
	int32_t a = pop(VM);
	int32_t b = pop(VM);
	push(VM, b / a);
}

void right_shift(stack_t* VM)
{
	if(VM->sp < 1)
	{
		printf("Cannot execute shift. Not enough operands.\n");
		return;
	}
	int32_t num = pop(VM);
	push(VM, num >> 1);
}

void left_shift(stack_t* VM)
{
	if(VM->sp < 1)
	{
		printf("Cannot execute shift. Not enough operands.\n");
		return;
	}
	int32_t num = pop(VM);
	push(VM, num << 1);
}

void inc(stack_t *VM)
{
	if(VM->sp < 1)
	{
		printf("Cannot execute increment. Not enough operands.\n");
		return;
	}
	int32_t a = pop(VM);
	a++;
	push(VM, a);
}

void dec(stack_t *VM)
{
	if(VM->sp < 1)
	{
		printf("Cannot execute decrement. Not enough operands.\n");
		return;
	}
	int32_t a = pop(VM);
	a--;
	push(VM, a);
}

void dup(stack_t *VM)
{
	if(VM->sp < 1)
	{
		printf("Cannot execute duplication. Not enough operands.\n");
		return;
	}
	int32_t a = pop(VM);
	push(VM, a);
	push(VM, a);
}

void swp(stack_t *VM)
{
	if(VM->sp < 2)
	{
		printf("Cannot execute swap. Not enough operands.\n");
		return;
	}
	int32_t a = pop(VM);
	int32_t b = pop(VM);
	push(VM, a);
	push(VM, b);
}

void peek(stack_t* VM)
{
	if(VM->sp < 1)
	{
		printf("Cannot peek. Not enough operands.\n");
		return;
	}
	int32_t a = pop(VM);
	push(VM, a);
	printf("TOP: %d\n", a);
}

void jmp(stack_t *VM, uint32_t addr)
{
	VM->PC = addr;
}

int only_digits(char* str)
{
	if(str == NULL || *str == '\0')
		return 0;
	int sep_count = 0;
	while(*str)
	{
		if(isdigit(*str))
		{
			str++;
			continue;
		}
		else if(*str == '.')
		{
			sep_count++;
			str++;
			if(sep_count > 1)
				return 0;
		}
		else
		{
			return 0;
		}
	}
	return 1;
}

void exec_opcode_REPL(opcode_t opcode, stack_t* VM, char* tkn)
{
	switch(opcode)
	{
		case OP_PSH:
		{
			tkn = strtok(NULL, " \n");
			printf("current tkn: %s\n", tkn);
			if(atoi(tkn))
			{
				push(VM, atoi(tkn));
				return;
			}
			else
			{
				printf("Not a number to push. Use it like \"PUSH 42\"\n");
				return;
			}
		}
		case OP_POP:
		{
			printf("Popped %d\n", pop(VM));
			return;
		}
		case OP_ADD:
		{
			add(VM);
			return;
		}
		case OP_SUB:
		{
			sub(VM);
			return;
		}
		case OP_MUL:
		{
			mul(VM);
			return;
		}
		case OP_DIV:
		{
			divis(VM);
			return;
		}

		default:
			printf("UNKNOWN OPCODE: %s\n", tkn);
			break;
	}
}

void parse_command(stack_t* VM, char* str)
{
	char* tkn = strtok(str, " \n");
	opcode_t opcode = OP_UNKNOWN;

	if(!strcmp(tkn, "PSH"))
	{
		opcode = OP_PSH;
	}
	else if(!strcmp(tkn, "POP"))
	{
		opcode = OP_POP;
	}
	else if(!strcmp(tkn, "ADD"))
	{
		opcode = OP_ADD;
	}
	else if(!strcmp(tkn, "SUB"))
	{
		opcode = OP_SUB;
	}
	else if(!strcmp(tkn, "MUL"))
	{
		opcode = OP_MUL;
	}
	else if(!strcmp(tkn, "DIV"))
	{
		opcode = OP_DIV;
	}
	printf("current tkn: %d\n", opcode);
	exec_opcode_REPL(opcode, VM, tkn);
}

void print_stack(stack_t* VM)
{
	for(int i = 0; i < VM->sp; i++)
	{
		printf("%s%d%s", i == 0 ? "[\"" : " \"", VM->stack[i], i == VM->sp-1 ? "\"]\n" : "\",");
	}
}

void exec_opcodes(stack_t *VM)
{
	while(VM->PC < VM->size)
	{
		
		printf("prog[0x%X]: 0x%X\n", VM->PC, VM->program[VM->PC]);
		switch(VM->program[VM->PC])
		{
			case OP_PSH:
			{
				VM->PC++;
				int32_t int_to_push = (VM->program[VM->PC]) | (VM->program[VM->PC + 1] << 8) | (VM->program[VM->PC + 2] << 16) | (VM->program[VM->PC + 3]  << 24);
				VM->PC += 4;
				push(VM, int_to_push);
				break;
			}
			case OP_POP:
			{
				VM->PC++;
				printf("Popped %d\n", pop(VM));
				break;
			}
			case OP_ADD:
			{
				VM->PC++;
				add(VM);
				break;
			}
			case OP_SUB:
			{
				VM->PC++;
				sub(VM);
				break;
			}
			case OP_MUL:
			{
				VM->PC++;
				mul(VM);
				break;
			}
			case OP_DIV:
			{
				VM->PC++;
				divis(VM);
				break;
			}
			case OP_RSH:
			{
				VM->PC++;
				right_shift(VM);
				break;
			}
			case OP_LSH:
			{
				VM->PC++;
				left_shift(VM);
				break;
			}
			case OP_JMP:
			{
				VM->PC++;
				int32_t addr = (VM->program[VM->PC]) | (VM->program[VM->PC + 1] << 8) | (VM->program[VM->PC + 2] << 16) | (VM->program[VM->PC + 3]  << 24);
				VM->PC += 4;
				jmp(VM, addr);
				break;
			}
			case OP_JNZ:
			{
				VM->PC++;
				int isnt_z = pop(VM);
				if(isnt_z){
					int32_t addr = (VM->program[VM->PC]) | (VM->program[VM->PC + 1] << 8) | (VM->program[VM->PC + 2] << 16) | (VM->program[VM->PC + 3]  << 24);
					VM->PC += 4;
					jmp(VM, addr);
				}
				else
				{
					VM->PC += 4;
				}
				break;
			}
			case OP_JZ:
			{
				VM->PC++;
				int isnt_z = pop(VM);
				if(!isnt_z){
					int32_t addr = (VM->program[VM->PC]) | (VM->program[VM->PC + 1] << 8) | (VM->program[VM->PC + 2] << 16) | (VM->program[VM->PC + 3]  << 24);
					VM->PC += 4;
					jmp(VM, addr);
				}
				else
				{
					VM->PC += 4;
				}
				break;
			}
			case OP_INC:
			{
				VM->PC++;
				inc(VM);
				break;
			}
			case OP_DEC:
			{
				VM->PC++;
				dec(VM);
				break;
			}
			case OP_DUP:
			{
				VM->PC++;
				dup(VM);
				break;
			}
			case OP_SWP:
			{
				VM->PC++;
				swp(VM);
				break;
			}
			case OP_PEK:
			{
				VM->PC++;
				peek(VM);
				break;
			}
			default:

				printf("UNKNOWN OPCODE: 0x%X\n", VM->program[VM->PC]);
				VM->PC++;
				break;
		}
		print_stack(VM);
	}
}

int main(int argc, char** argv)
{
	stack_t VM = {0};
	if(argc < 2){
		// push(&VM, 15.5);
		// printf("Popped %.2lf\n", pop(&VM));
		char string[256];
		while(1)
		{
			printf("Corth> ");
			fgets(string, 256, stdin);
			if(!strcmp(string, "\n"))
				break;
			parse_command(&VM, string);
			
		}
	}
	else
	{
		
		FILE* input = fopen(argv[1], "rb");
		
		fseek(input, 0, SEEK_END);
		VM.size = ftell(input);
		fseek(input, 0, SEEK_SET);
		
		VM.program = malloc(VM.size);
		fread(VM.program, 1, VM.size, input);
		fclose(input);


		exec_opcodes(&VM);
		// while(1)
	}
	return 0;
}