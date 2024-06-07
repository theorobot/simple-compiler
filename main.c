#include <stdio.h>
#include <stdlib.h>

#include "asttoassembly.h"
#include "node.h"
#include "parse.h"
#include "shuntingyard.h"
#include "tokenize.h"
#include "vector.h"

int main() {
	int length = 1024;
	char buffer[length];
	FILE* f = fopen("code.txt", "rb");

	if (f) {
		if (buffer) {
			size_t read_size = fread(buffer, 1, length - 1, f);
			buffer[read_size] = '\0';
		}
		fclose(f);
	}

	StringVector sv = parse_string(buffer);
	NodeTypes* types = Tokenize(&sv);
	NodeAST* a = ShuntingYardAST(types, &sv);

	printf("\n");
	ASTToAssembly(a);
	printf("\n");

	SVFree(&sv);
	free(types);
	FreeAST(a);

	return 0;
}
