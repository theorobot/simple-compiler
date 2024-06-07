#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char** start;
	int size;
	int memory_allocated;
} StringVector;

StringVector CreateSV(int mem_alloc) {
	StringVector sv;
	sv.start = (char**)malloc(mem_alloc * sizeof(char*));
	if (sv.start == NULL) {
		fprintf(stderr, "Memory Allocation Failed\n");
		exit(1);
	}
	sv.memory_allocated = mem_alloc;
	sv.size = 0;
	return sv;
}

void SVPushBack(StringVector* sv, const char* string) {
	if (sv->size >= sv->memory_allocated) {
		char** new_start = (char**)realloc(sv->start, sv->memory_allocated * 2 * sizeof(char*));
		if (new_start == NULL) {
			fprintf(stderr, "Memory Reallocation Failed\n");
			exit(1);
		}
		sv->start = new_start;
		sv->memory_allocated *= 2;
	}

	sv->start[sv->size] = strdup(string);
	if (sv->start[sv->size] == NULL) {
		fprintf(stderr, "Memory Allocation Failed for string\n");
		exit(1);
	}
	sv->size++;
}

void SVDeleteItem(StringVector* sv, int item) {
	if (item < 0 || item >= sv->size) {
		fprintf(stderr, "Invalid index for deletion\n");
		exit(1);
	}
	free(sv->start[item]);
	for (int i = item; i < sv->size - 1; i++) {
		sv->start[i] = sv->start[i + 1];
	}
	sv->size--;
}

char* SVGetItem(StringVector* sv, int item) {
	if (item >= sv->size) {
		fprintf(stderr, "Can't Access %dth element of vector of size %d\n", item, sv->size);
		exit(1);
	}
	return sv->start[item];
}

void SVFree(StringVector* sv) {
	for (int i = 0; i < sv->size; i++) {
		free(sv->start[i]);
	}
	free(sv->start);
}

#endif