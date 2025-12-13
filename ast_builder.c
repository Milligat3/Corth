#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizing.h"
#include "ast_builder.h"


token_t current_token(AstBuilder_t *tt)
{
	if(tt->pos < tt->tt.size)
	{
		return tt->tt.tokens[tt->pos];
	}
	return (token_t){.type = TKN_EOF};
}

void eat_token(AstBuilder_t *tt)
{
	tt->pos++;
}

AstNode_t* parse_factor(AstBuilder_t *tt)
{
	// printf("[DEBUG parse_factor] Enter. Current token: '%s' (Type: %d)\n", current_token(tt).tkn_str, current_token(tt).type);
	AstNode_t *node = malloc(sizeof(AstNode_t));
	node->left = NULL;
	node->right = NULL;
	switch (current_token(tt).type) {
	case TKN_CONST:
		node->type = NODE_CONST;
		node->constant.value = strtol(current_token(tt).tkn_str, NULL, 10);
		eat_token(tt);
		return node;	
	default:
		printf("Met Not an TKN_CONST. Met: Error. %s (type %d)\n", current_token(tt).tkn_str, current_token(tt).type); // Be more verbose
		exit(1);
		break;
	}
	return node;
}

AstNode_t* create_binop_node(token_t tkn, AstNode_t* left, AstNode_t* right)
{
	AstNode_t* binop = malloc(sizeof(AstNode_t));
	binop->type = NODE_BINOP;
	binop->binop.oper = malloc(strlen(tkn.tkn_str) + 1);
	strcpy(binop->binop.oper, tkn.tkn_str);
	binop->left = left;
	binop->right = right;
	return binop;
}

AstNode_t* parse_term(AstBuilder_t *tkns)
{
	// printf("[DEBUG parse_term] Enter. Current token: '%s' (Type: %d)\n", current_token(tkns).tkn_str, current_token(tkns).type);
	AstNode_t* left = parse_factor(tkns);
	while(current_token(tkns).type != TKN_EOF && (!strcmp(current_token(tkns).tkn_str, "*") || !strcmp(current_token(tkns).tkn_str, "/")))
	{
		token_t op = current_token(tkns);
		
		eat_token(tkns);
		AstNode_t* right = parse_factor(tkns);
		left = create_binop_node(op, left, right);
	}
	return left;
}

AstNode_t* parse_expr(AstBuilder_t *tkns)
{
	// printf("[DEBUG parse_expr] Enter. Current token: '%s' (Type: %d)\n", current_token(tkns).tkn_str, current_token(tkns).type);
	AstNode_t* left = parse_term(tkns);
	while(current_token(tkns).type != TKN_EOF && (!strcmp(current_token(tkns).tkn_str, "+") || !strcmp(current_token(tkns).tkn_str, "-")))
	{
		token_t op = current_token(tkns);
		
		eat_token(tkns);
		AstNode_t* right = parse_term(tkns);
		left = create_binop_node(op, left, right);
	}
	return left;
}

typedef enum
{
	BIN_ADD,
	BIN_SUB,
	BIN_MULT,
	BIN_DIV,
	BIN_NULL
}binop_type;

binop_type get_binop_type(AstNode_t* node)
{
	if(!strcmp(node->binop.oper, "+"))
	{
		return BIN_ADD;
	} 
	else if(!strcmp(node->binop.oper, "-"))
	{
		return BIN_SUB;
	} 
	else if(!strcmp(node->binop.oper, "*"))
	{
		return BIN_MULT;
	} 
	else if(!strcmp(node->binop.oper, "/"))
	{
		return BIN_DIV;
	}
	return BIN_NULL; 
}


int eval(AstNode_t* node)
{
	switch(node->type)
	{
        case NODE_BINOP:
        {
        	binop_type type = get_binop_type(node);
        	switch(type)
        	{
				case BIN_ADD:
				{
					return eval(node->left) + eval(node->right);
				}
				case BIN_SUB:
				{
					return eval(node->left) - eval(node->right);
				}
				case BIN_MULT:
				{
					return eval(node->left) * eval(node->right);
				}
				case BIN_DIV:
				{
					return eval(node->left) / eval(node->right);
				}
				case BIN_NULL:
					printf("Not a valid BINOP\n");
					exit(1);
			}
			break;
		}        
        case NODE_CONST:
			return node->constant.value;
	}
	return 0;
}