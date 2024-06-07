#ifndef SHUNTINGYARD_H
#define SHUNTINGYARD_H

#include <stdlib.h>

#include "node.h"
#include "vector.h"

typedef struct {
	NodeTypes type;
	char* name;
} Token;

static int precedence[] = {0, 0, 0, 0, 1, 1, 2, 2};

Token* ShuntingYard(NodeTypes* o, StringVector* tokens) {
	int num_elements = tokens->size;

	Token* r = (Token*)malloc((num_elements + 1) * sizeof(Token));
	int current_output = 0;

	Token* op_stack = (Token*)malloc((num_elements + 1) * sizeof(Token));
	Token* sp = op_stack;

	for (int i = 0; i < num_elements; i++) {
		switch (o[i]) {
		case literal:
			r[current_output].type = literal;
			r[current_output].name = tokens->start[i];
			current_output++;
			break;
		case variable:
			r[current_output].type = variable;
			r[current_output].name = tokens->start[i];
			current_output++;
			break;
		case function:
			FunctionTypes f = GetFunctionNum(tokens->start[i]);

			while ((sp > op_stack) && (precedence[f] < precedence[GetFunctionNum((*(sp-1)).name)])) {
				r[current_output].type = function;
				r[current_output].name = (*(sp-1)).name;
				current_output++;
				sp--;
			}

			*sp = (Token){function, tokens->start[i]};
			sp++;
			break;
		}
	}

	while (sp > op_stack) {
		r[current_output].type = function;
		r[current_output].name = (*(sp-1)).name;
		current_output++;
		sp--;
	}

	return r;
}

void AddFuncToAST(NodeAST* r, NodeAST* newfunctionAST) {
	int functionnumargs = numargs[newfunctionAST->data.func.type];
	newfunctionAST->data.func.children = (NodeAST**)malloc(functionnumargs * sizeof(NodeAST*));
	for (int arg = 0; arg < functionnumargs; arg++) {
		newfunctionAST->data.func.children[functionnumargs - arg - 1] = r->data.prog.children[r->data.prog.num_lines - 1];
		r->data.prog.num_lines -= 1;
	}
	r->data.prog.children = (NodeAST**)realloc(r->data.prog.children, (r->data.prog.num_lines + 1) * sizeof(NodeAST*));
	r->data.prog.num_lines++;
	r->data.prog.children[r->data.prog.num_lines - 1] = newfunctionAST;
}

NodeAST* ShuntingYardAST(NodeTypes* o, StringVector* tokens) {
	int num_elements = tokens->size;

	NodeAST* r = (NodeAST*)malloc(sizeof(NodeAST));
	r->type = program;
	r->data.prog.children = NULL;
	r->data.prog.num_lines = 0;

	Token* op_stack = (Token*)malloc((num_elements + 1) * sizeof(Token));
	Token* sp = op_stack;

	for (int i = 0; i < num_elements; i++) {
		switch (o[i]) {
		case literal:
			{
			NodeAST* newliteralAST = (NodeAST*)malloc(sizeof(NodeAST));
			newliteralAST->type = literal;
			newliteralAST->data.lit = (LiteralNode){atoi(tokens->start[i])};
			r->data.prog.children = (NodeAST**)realloc(r->data.prog.children, (r->data.prog.num_lines + 1) * sizeof(NodeAST*));
			r->data.prog.num_lines++;
			r->data.prog.children[r->data.prog.num_lines - 1] = newliteralAST;
			break;
			}
		case variable:
			{
			NodeAST* newvariableAST = (NodeAST*)malloc(sizeof(NodeAST));
			newvariableAST->type = variable;
			newvariableAST->data.var = (VariableNode){tokens->start[i]};
			r->data.prog.children = (NodeAST**)realloc(r->data.prog.children, (r->data.prog.num_lines + 1) * sizeof(NodeAST*));
			r->data.prog.num_lines++;
			r->data.prog.children[r->data.prog.num_lines - 1] = newvariableAST;
			break;
			}
		case function:
			if (tokens->start[i][0] == '(') {
				*sp = (Token){function, "("};
				sp++;
			} else if (tokens->start[i][0] == ')') {
				while ((*(sp-1)).name[0] != '(') {
					Token functionToken = *(sp-1);
					NodeAST* newfunctionAST = (NodeAST*)malloc(sizeof(NodeAST));
					newfunctionAST->type = function;
					newfunctionAST->data.func.type = GetFunctionNum((*(sp-1)).name);
					AddFuncToAST(r, newfunctionAST);
					sp--;
				}
				sp--;
			} else {
				int f = GetFunctionNum(tokens->start[i]);
				while ((sp > op_stack) && ((*(sp-1)).name[0] != '(') && (precedence[f] <= precedence[GetFunctionNum((*(sp-1)).name)])) {
					Token functionToken = *(sp-1);
					NodeAST* newfunctionAST = (NodeAST*)malloc(sizeof(NodeAST));
					newfunctionAST->type = function;
					newfunctionAST->data.func.type = GetFunctionNum((*(sp-1)).name);
					AddFuncToAST(r, newfunctionAST);
					sp--;
				}

				*sp = (Token){function, tokens->start[i]};
				sp++;
				break;
			}
		}
	}

	while (sp > op_stack) {
		Token functionToken = *(sp-1);
		NodeAST* newfunctionAST = (NodeAST*)malloc(sizeof(NodeAST));
		newfunctionAST->type = function;
		newfunctionAST->data.func.type = GetFunctionNum(functionToken.name);
		AddFuncToAST(r, newfunctionAST);
		sp--;
	}

	free(op_stack);

	return r;
}

#endif
