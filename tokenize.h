#ifndef TOKENIZE_H
#define TOKENIZE_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"
#include "parse.h"
#include "vector.h"

NodeTypes* Tokenize(StringVector* sv) {
	NodeTypes* types = (NodeTypes*)malloc(sv->size * sizeof(NodeTypes));
	for (int token = 0; token < sv->size - 1; token++) {
		char* token_string = sv->start[token];
		if (token_string[0] > '0' && token_string[0] < '9') {
			types[token] = literal;
		} else if ((token_string[0] >= 'a' && token_string[0] <= 'z') || (token_string[0] >= 'A' && token_string[0] <= 'Z')) {
			if (!strcmp(token_string, "Var") || !strcmp(token_string, "Set") || !strcmp(token_string, "Input") || !strcmp(token_string, "Print")) {
				types[token] = function;
			} else {
				types[token] = variable;
			}
		} else {
			types[token] = function;
		}
	}

	return types;
}

#endif
