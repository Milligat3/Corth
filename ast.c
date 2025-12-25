#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "ast.h"


token_t current_token(AstBuilder_t *tt)
{
	token_t eof_tkn;
	eof_tkn.type = TKN_EOF;
	if(tt->pos < tt->tt.size)
	{
		return tt->tt.tokens[tt->pos];
	}
	return eof_tkn;
}

void eat_token(AstBuilder_t *tt)
{
	tt->pos++;
}

AstNode_t* parse_factor(AstBuilder_t *tt)
{
	AstNode_t *node; 
	token_t cur_tkn;

	node = malloc(sizeof(AstNode_t));
	node->left = NULL;
	node->right = NULL;
	cur_tkn = current_token(tt);
	switch (cur_tkn.type) {
	case TKN_CONST:
		node->type = NODE_CONST;
		node->as.constant.value = strtol(cur_tkn.tkn_str, NULL, 10);
		eat_token(tt);
		cur_tkn = current_token(tt);
		return node;
	default:
		printf("Met Not an TKN_CONST. Met: Error. %s (type %d)\n", cur_tkn.tkn_str, cur_tkn.type);
		exit(1);
		break;
	}
	return node;
}

AstNode_t* create_binop_node(token_t tkn, AstNode_t* left, AstNode_t* right)
{
	AstNode_t* binop;
	binop = malloc(sizeof(AstNode_t));
	binop->type = NODE_BINOP;
	binop->as.binop.oper = malloc(strlen(tkn.tkn_str) + 1);
	strcpy(binop->as.binop.oper, tkn.tkn_str);
	binop->left = left;
	binop->right = right;
	return binop;
}

AstNode_t* parse_term(AstBuilder_t *tkns)
{
	AstNode_t* left, *right;
	token_t op, cur_tkn;
	left = parse_factor(tkns);
	cur_tkn = current_token(tkns);
	while(cur_tkn.type != TKN_EOF && (!strcmp(cur_tkn.tkn_str, "*") || !strcmp(cur_tkn.tkn_str, "/")))
	{
		op = cur_tkn;
		
		eat_token(tkns);
		cur_tkn = current_token(tkns);
		right = parse_factor(tkns);
		left = create_binop_node(op, left, right);
	}
	return left;
}

AstNode_t* parse_expr(AstBuilder_t *tkns)
{
	AstNode_t* left, *right;
	token_t op, cur_tkn;
	left = parse_term(tkns);
	cur_tkn = current_token(tkns);
	while(cur_tkn.type != TKN_EOF && (!strcmp(cur_tkn.tkn_str, "+") || !strcmp(cur_tkn.tkn_str, "-")))
	{
		op = cur_tkn;
		
		eat_token(tkns);
		cur_tkn = current_token(tkns);
		right = parse_term(tkns);
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
	if(!strcmp(node->as.binop.oper, "+"))
	{
		return BIN_ADD;
	} 
	else if(!strcmp(node->as.binop.oper, "-"))
	{
		return BIN_SUB;
	} 
	else if(!strcmp(node->as.binop.oper, "*"))
	{
		return BIN_MULT;
	} 
	else if(!strcmp(node->as.binop.oper, "/"))
	{
		return BIN_DIV;
	}
	return BIN_NULL; 
}


int eval(AstNode_t* node)
{
	binop_type type;
	switch(node->type)
	{
        case NODE_BINOP:
        {
        	type = get_binop_type(node);
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
			return node->as.constant.value;
	}
	return 0;
}