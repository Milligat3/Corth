#ifndef AST_BUILDER_H
#define AST_BUILDER_H
#include "token.h"

typedef enum
{
	NODE_BINOP,
	NODE_CONST
}NodeType_t;

typedef struct AstNode_t
{
	NodeType_t type;
	union
	{
		struct
		{
			struct AstNode_t *name;
			struct AstNode_t *value;
		}decl;
		struct
		{
			struct AstNode_t *name;
			struct AstNode_t *value;
		}assign;
		struct
		{
			char* name;
		}var;
		
		struct 
		{
			int value;
		}constant;
		struct
		{
			char* oper;
			
		}binop;
	}as;
	struct AstNode_t *left, *right;
}AstNode_t;

typedef struct AstBuilder_t
{
	token_table_t tt;
	size_t pos;
}AstBuilder_t;

AstNode_t *parse_expr(AstBuilder_t *tt);
int eval(AstNode_t* node);

#endif