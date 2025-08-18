// StackBasedVM.c
// MAIN TODO: БЛЯТЬ! ДРУЖИЩЕ! СМОТРИ, ЧТОБЫ НАХУЙ СДЕЛАТЬ ФРЕЙМЫ, ЗАСЕКАЙ БЛЯТЬ, МЫ ПУШИМ FP НА CALL_STACK, 
// А ПРИ RTS ЕГО ВОССТАНАВЛИВАЕМ! ВСЕ ПРОСТО ЕБИТ ТВОЮ МАТЬ ХАХАХХ! ДАВАЙ, ЗАВТРА ЗА РАБОТУ ПОСЛЕ ГАШНИ
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "opcodes.h"
#define STACK_SIZE 1024

typedef struct
{
	size_t sp, fp, csp, dp, fdp;
	int32_t stack[STACK_SIZE], data_stack[STACK_SIZE];
	uint32_t PC, size, call_stack[STACK_SIZE];
	uint8_t* program;
}stack_t;


// stack_t VM = {0, 0};

void push(stack_t* VM, int32_t n)
{
	if(VM->sp >= STACK_SIZE)
	{
		printf("Stack Overflow. Aborting.\n");
		free(VM->program);
		exit(1);
	}
	VM->stack[VM->sp++] = n;
}

int32_t pop(stack_t* VM)
{
	if(VM->sp <= VM->fp)
	{
		printf("Stack Underflow. Aborting.\n");
		free(VM->program);
		exit(1);
	}
	int32_t to_ret = VM->stack[--VM->sp];
	return to_ret;
}

void add(stack_t* VM)
{
	if(VM->sp - VM->fp < 2)
	{
		printf("Cannot execute addition. Not enough operands.\n");
		free(VM->program);
		exit(1);
	}
	int32_t a = pop(VM);
	int32_t b = pop(VM);
	push(VM, a + b);
}

void sub(stack_t* VM)
{
	if(VM->sp - VM->fp < 2)
	{
		printf("Cannot execute substraction. Not enough operands.\n");
		free(VM->program);
		exit(1);
	}
	int32_t a = pop(VM);
	int32_t b = pop(VM);
	push(VM, b - a);
}

void mul(stack_t* VM)
{
	if(VM->sp - VM->fp < 2)
	{
		printf("Cannot execute multiplication. Not enough operands.\n");
		free(VM->program);
		exit(1);
	}
	int32_t a = pop(VM);
	int32_t b = pop(VM);
	push(VM, a * b);
}

void divis(stack_t* VM)
{
	if(VM->sp - VM->fp < 2)
	{
		printf("Cannot execute division. Not enough operands.\n");
		free(VM->program);
		exit(1);
	}
	int32_t a = pop(VM);
	int32_t b = pop(VM);
	push(VM, b / a);
}

void right_shift(stack_t* VM)
{
	if(VM->sp - VM->fp < 1)
	{
		printf("Cannot execute shift. Not enough operands.\n");
		free(VM->program);
		exit(1);
	}
	int32_t num = pop(VM);
	push(VM, num >> 1);
}

void left_shift(stack_t* VM)
{
	if(VM->sp - VM->fp < 1)
	{
		printf("Cannot execute shift. Not enough operands.\n");
		free(VM->program);
		exit(1);
	}
	int32_t num = pop(VM);
	push(VM, num << 1);
}

void inc(stack_t *VM)
{
	if(VM->sp - VM->fp < 1)
	{
		printf("Cannot execute increment. Not enough operands.\n");
		free(VM->program);
		exit(1);
	}
	int32_t a = pop(VM);
	a++;
	push(VM, a);
}

void dec(stack_t *VM)
{
	if(VM->sp - VM->fp < 1)
	{
		printf("Cannot execute decrement. Not enough operands.\n");
		free(VM->program);
		exit(1);
	}
	int32_t a = pop(VM);
	a--;
	push(VM, a);
}

void dup(stack_t *VM)
{
	if(VM->sp - VM->fp < 1)
	{
		printf("Cannot execute duplication. Not enough operands.\n");
		free(VM->program);
		exit(1);
	}
	int32_t a = pop(VM);
	push(VM, a);
	push(VM, a);
}

void swp(stack_t *VM)
{
	if(VM->sp - VM->fp < 2)
	{
		printf("Cannot execute swap. Not enough operands.\n");
		free(VM->program);
		exit(1);
	}
	int32_t a = pop(VM);
	int32_t b = pop(VM);
	push(VM, a);
	push(VM, b);
}

void out(stack_t* VM)
{
	if(VM->sp - VM->fp < 1)
	{
		printf("Cannot print. Not enough operands.\n");
		free(VM->program);
		exit(1);
	}
	int32_t a = pop(VM);
	push(VM, a);
	printf("TOP: %d\n", a);
}

void jmp(stack_t *VM, uint32_t addr)
{
	VM->PC = addr;
}

void call_st_push(stack_t* VM, uint32_t addr)
{
	if(VM->csp >= STACK_SIZE)
	{
		printf("Call Stack Overflow. Aborting.\n");
		free(VM->program);
		exit(1);
	}
	VM->call_stack[VM->csp++] = addr;
}

uint32_t call_st_pop(stack_t* VM)
{
	if(VM->csp == 0)
	{
		printf("Call Stack Underflow. Aborting.\n");
		free(VM->program);
		exit(1);
	}
	int32_t addr = VM->call_stack[--VM->csp];
	return addr;
}

void jsr(stack_t *VM, uint32_t addr, uint8_t num_of_args)
{
	if(num_of_args > VM->sp)
	{
		printf("Not enough arguments on the stack.\n");
		free(VM->program);
		exit(1);
	}
	int32_t num = pop(VM);
	push(VM, num);
	call_st_push(VM, VM->PC);
	call_st_push(VM, VM->fp);
	call_st_push(VM, VM->fdp);
	VM->fp = VM->sp;
	VM->fp -= num_of_args;
	VM->fdp = VM->dp;
	VM->PC = addr;
}

void rts(stack_t *VM)
{
	int32_t ret_num = pop(VM);

	VM->sp = VM->fp;
	VM->dp = VM->fdp;
	push(VM, ret_num);

	VM->fdp = call_st_pop(VM);
	VM->fp = call_st_pop(VM);
	VM->PC = call_st_pop(VM);

}

void store(stack_t *VM)
{
	if(VM->sp - VM->fp < 1)
	{
		printf("Cannot store. Not enough operands on the stack.\n");
		free(VM->program);
		exit(1);
	}
	int32_t to_push = pop(VM);
	VM->data_stack[VM->dp++] = to_push;
}

void peek_data(stack_t *VM)
{
	if(VM->sp - VM->fp < 1)
	{
		printf("Cannot peek at variable. Not enough operands on the stack.\n");
		free(VM->program);
		exit(1);
	}
	int32_t index = VM->fdp + pop(VM);
	if(index >= VM->dp)
	{
		printf("No such variable by that index(%d) in variable stack.\n", index);
		free(VM->program);
		exit(1);
	}
	int32_t to_push = VM->data_stack[index];
	push(VM, to_push);
}

void poke_data(stack_t *VM)
{
	if(VM->sp - VM->fp < 2)
	{
		printf("Cannot poke at variable. Not enough operands on the stack.\n");
		free(VM->program);
		exit(1);
	}
	int32_t index = VM->fdp + pop(VM);
	if(index >= VM->dp)
	{
		printf("No such variable by that index(%d) in variable stack.\n", index);
		free(VM->program);
		exit(1);
	}
	VM->data_stack[index] = pop(VM);
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

void print_call_stack(stack_t* VM)
{
	for(int i = 0; i < VM->csp; i++)
	{
		printf("%s%d%s", i == 0 ? "[\"" : " \"", VM->call_stack[i], i == VM->csp-1 ? "\"]\n\n" : "\",");
	}
}
void print_var_stack(stack_t* VM)
{
	for(int i = 0; i < VM->dp; i++)
	{
		printf("%s%d%s", i == 0 ? "[\"" : " \"", VM->data_stack[i], i == VM->dp-1 ? "\"]\n\n" : "\",");
	}
}
void debug_output(stack_t *VM)
{
	printf("\nDATA STACK:\n");
	print_stack(VM);
	printf("CALL STACK:\n");
	print_call_stack(VM);
	printf("VAR STACK:\n");
	print_var_stack(VM);
	printf("Current state: SP: %zu, FP: %zu, CSP: %zu, DP: %zu, FDP: %zu, PC: %d\n\n", VM->sp, VM->fp, VM->csp, VM->dp, VM->fdp, VM->PC);

}
char opcodes[][32] = {
	"OP_UNKNOWN",
	"OP_PSH",
	"OP_POP",
	"OP_ADD",
	"OP_SUB",
	"OP_DIV",
	"OP_MUL",
	"OP_RSH",
	"OP_LSH",
	"OP_JMP",
	"OP_JNZ",
	"OP_JZ",
	"OP_JSR",
	"OP_RTS",
	"OP_DUP",
	"OP_SWP",
	"OP_INC",
	"OP_DEC",
	"OP_OUT",
	"OP_STR",
	"OP_WRD",
	"OP_RDD",
	"OP_HLT"
};

void exec_opcodes(stack_t *VM)
{
	while(VM->PC < VM->size)
	{
		
		//printf("prog[0x%X]: %s\n", VM->PC, opcodes[VM->program[VM->PC]]);
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
			case OP_JSR:
			{
				VM->PC++;
				int32_t addr = (VM->program[VM->PC]) | (VM->program[VM->PC + 1] << 8) | (VM->program[VM->PC + 2] << 16) | (VM->program[VM->PC + 3]  << 24);
				VM->PC += 4;
				uint8_t num_of_args = (VM->program[VM->PC++]);
				jsr(VM, addr, num_of_args);
				break;
			}
			case OP_RTS:
			{
				rts(VM);
				break;
			}
			case OP_HLT:
			{
				debug_output(VM);
				return;
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
			case OP_OUT:
			{
				VM->PC++;
				out(VM);
				break;
			}
			case OP_STR:
			{
				VM->PC++;
				store(VM);
				break;
			}
			
			case OP_WRD:
			{
				VM->PC++;
				poke_data(VM);
				break;
			}
			
			case OP_RDD:
			{
				VM->PC++;
				peek_data(VM);
				break;
			}
			
			default:

				printf("UNKNOWN OPCODE: 0x%X\n", VM->program[VM->PC]);
				VM->PC++;
				break;
		}
		// debug_output(VM);
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