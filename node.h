#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <stdlib.h>

typedef enum {
	program,
	literal,
	variable,
	function
} NodeTypes;

typedef enum {
	var,
	set,
	print,
	input,
	addn,
	subn,
	muln,
	divn
} FunctionTypes;

int numargs[] = {1, 2, 1, 1, 2, 2, 2, 2};

char* func_names[] = {"Var", "Set", "Print", "Input", "Add", "Sub", "Mul", "Div"};

FunctionTypes GetFunctionNum(char* name) {
	switch (name[0]) {
	case 'V':
		return var;
	case 'S':
		return set;
	case 'P':
		return print;
	case 'I':
		return input;
	case '+':
		return addn;
	case '-':
		return subn;
	case '*':
		return muln;
	default:
		return divn;
	}
}

typedef struct NodeAST NodeAST;

typedef struct {
	NodeAST** children;
	int num_lines;
} ProgramNode;

typedef struct {
	int value;
} LiteralNode;

typedef struct {
	char* name;
} VariableNode;

typedef struct {
	FunctionTypes type;
	NodeAST** children;
} FunctionNode;

typedef union {
	ProgramNode prog;
	LiteralNode lit;
	VariableNode var;
	FunctionNode func;
}  NodeUnion;

struct NodeAST {
	NodeTypes type;
	NodeUnion data;
};

void PrintAST(NodeAST* n) {
	if (n == NULL)
		return;

	switch (n->type) {
	case program:
		printf("Program:\n");
		for (int i = 0; i < n->data.prog.num_lines; i++) {
			printf("\t");
			PrintAST(n->data.prog.children[i]);
			printf("\n");
		}
		break;
	case literal:
		printf("Literal: %d", n->data.lit.value);
		break;
	case variable:
		printf("Variable: %s", n->data.var.name);
		break;
	case function:
		printf("Function: %s(", func_names[n->data.func.type]);
		for (int child = 0; child < numargs[n->data.func.type]; child++) {
			if (child != 0)
				printf(", ");
			PrintAST(n->data.func.children[child]);
		}
		printf(")");
		break;
	}
}

void FreeAST(NodeAST * n) {
	if (n == NULL) {
		return;
	}

	switch (n->type) {
	case program:
		for (int i = 0; i < n->data.prog.num_lines; i++) {
			FreeAST(n->data.prog.children[i]);
		}
		free(n->data.prog.children);
		break;
	case function:
		for (int child = 0; child < numargs[n->data.func.type]; child++) {
			FreeAST(n->data.func.children[child]);
		}
		free(n->data.func.children);
		break;
	}

	free(n);
}

#endif
