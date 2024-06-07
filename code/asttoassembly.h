#ifndef ASTTOASSEMBLY_H
#define ASTTOASSEMBLY_H

#include <stdio.h>

#include "node.h"

void ASTToAssembly(NodeAST* n) {
	if (n == NULL)
		return;

	switch (n->type) {
	case literal:
		printf("    mov eax, %d\n", n->data.lit.value);
		printf("    push eax\n");
		break;

	case variable:
		printf("    mov eax, [%s]\n", n->data.var.name);
		printf("    push eax\n");
		break;

	case function:

		switch (n->data.func.type) {
		case set:
			ASTToAssembly(n->data.func.children[1]);
			printf("    pop eax\n");
			printf("    mov [%s], eax\n", n->data.func.children[0]->data.var.name);
			break;
		case print:
			ASTToAssembly(n->data.func.children[0]);
			printf("    pop eax\n");
			printf("    call iprintLF\n");
			break;
		case input:
			printf("    mov edx, 255\n");
			printf("    mov ecx, sinput\n");
			printf("    mov ebx, 0\n");
			printf("    mov eax, 3\n");
			printf("    int 80h\n");
			printf("    mov eax, sinput\n");
			printf("    call atoi\n");
			printf("    mov [%s], eax\n", n->data.func.children[0]->data.var.name);
			break;
		case addn:
			for (int child = 0; child < numargs[n->data.func.type]; child++) {
				ASTToAssembly(n->data.func.children[child]);
			}
			printf("    pop ebx\n");
			printf("    pop eax\n");
			printf("    add eax, ebx\n");
			printf("    push eax\n");
			break;
		case subn:
			for (int child = 0; child < numargs[n->data.func.type]; child++) {
				ASTToAssembly(n->data.func.children[child]);
			}
			printf("    pop ebx\n");
			printf("    pop eax\n");
			printf("    sub eax, ebx\n");
			printf("    push eax\n");
			break;
		case muln:
			for (int child = 0; child < numargs[n->data.func.type]; child++) {
				ASTToAssembly(n->data.func.children[child]);
			}
			printf("    pop ebx\n");
			printf("    pop eax\n");
			printf("    mul ebx\n");
			printf("    push eax\n");
			break;
		case divn:
			for (int child = 0; child < numargs[n->data.func.type]; child++) {
				ASTToAssembly(n->data.func.children[child]);
			}
			printf("    pop ebx\n");
			printf("    pop eax\n");
			printf("    xor edx, edx\n");
			printf("    div ebx\n");
			printf("    push eax\n");
			break;
		}
		break;

	case program:
		printf("section .data\n");
		for (int i = 0; i < n->data.prog.num_lines; i++) {
			if (n->data.prog.children[i]->type == function && n->data.prog.children[i]->data.func.type == var) {
				printf("    %s: dd 0\n", n->data.prog.children[i]->data.func.children[0]->data.var.name);
			}
		}

		printf("section .bss\n");
		printf("    sinput: resb 255\n");

		printf("section .text\n");
		printf("    global _start\n");

		printf("_start:\n");

		for (int i = 0; i < n->data.prog.num_lines; i++) {
			ASTToAssembly(n->data.prog.children[i]);
		}

		printf("    call quit\n\n");

		const char *code = 
        "slen:\n"
        "    push ebx\n"
        "    mov ebx, eax\n"
        "\n"
        "nextchar:\n"
        "    cmp byte [eax], 0\n"
        "    jz finished\n"
        "    inc eax\n"
        "    jmp nextchar\n"
        "\n"
        "finished:\n"
        "    sub eax, ebx\n"
        "    pop ebx\n"
        "    ret\n"
        "\n"
        "sprint:\n"
        "    push edx\n"
        "    push ecx\n"
        "    push ebx\n"
        "    push eax\n"
        "    call slen\n"
        "    mov edx, eax\n"
        "    pop eax\n"
        "    mov ecx, eax\n"
        "    mov ebx, 1\n"
        "    mov eax, 4\n"
        "    int 80h\n"
        "    pop ebx\n"
        "    pop ecx\n"
        "    pop edx\n"
        "    ret\n"
        "\n"
        "sprintLF:\n"
        "    call sprint\n"
        "    push eax\n"
        "    mov eax, 0Ah\n"
        "    push eax\n"
        "    mov eax, esp\n"
        "    call sprint\n"
        "    pop eax\n"
        "    pop eax\n"
        "    ret\n"
        "\n"
        "iprint:\n"
        "    push eax\n"
        "    push ecx\n"
        "    push edx\n"
        "    push esi\n"
        "    mov ecx, 0\n"
        "divideLoop:\n"
        "    inc ecx\n"
        "    mov edx, 0\n"
        "    mov esi, 10\n"
        "    idiv esi\n"
        "    add edx, 48\n"
        "    push edx\n"
        "    cmp eax, 0\n"
        "    jnz divideLoop\n"
        "printLoop:\n"
        "    dec ecx\n"
        "    mov eax, esp\n"
        "    call sprint\n"
        "    pop eax\n"
        "    cmp ecx, 0\n"
        "    jnz printLoop\n"
        "    pop esi\n"
        "    pop edx\n"
        "    pop ecx\n"
        "    pop eax\n"
        "    ret\n"
        "\n"
        "iprintLF:\n"
        "    call iprint\n"
        "    push eax\n"
        "    mov eax, 0Ah\n"
        "    push eax\n"
        "    mov eax, esp\n"
        "    call sprint\n"
        "    pop eax\n"
        "    pop eax\n"
        "    ret\n"
        "\n"
        "atoi:\n"
        "    push ebx\n"
        "    push ecx\n"
        "    push edx\n"
        "    push esi\n"
        "    mov esi, eax\n"
        "    mov eax, 0\n"
        "    mov ecx, 0\n"
        ".multiplyLoop:\n"
        "    xor ebx, ebx\n"
        "    mov bl, [esi + ecx]\n"
        "    cmp bl, 0\n"
        "    je .finished\n"
        "    cmp bl, 48\n"
        "    jl .finished\n"
        "    cmp bl, 57\n"
        "    jg .finished\n"
        "    sub bl, 48\n"
        "    imul eax, eax, 10\n"
        "    add eax, ebx\n"
        "    inc ecx\n"
        "    jmp .multiplyLoop\n"
        ".finished:\n"
        "    pop esi\n"
        "    pop edx\n"
        "    pop ecx\n"
        "    pop ebx\n"
        "    ret\n"
        "\n"
        "quit:\n"
        "    mov ebx, 0\n"
        "    mov eax, 1\n"
        "    int 80h\n"
        "    ret\n";

        printf("%s", code);
		break;
	}
}

#endif
