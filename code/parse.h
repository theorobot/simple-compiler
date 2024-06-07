#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"

int type(char c) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || (c >= '0' && c <= '9'))
        return 0;
    else
        return -1;
}

StringVector parse_string(const char* line) {
    const char* letter_ptr = line;
    StringVector tokenized = CreateSV(10);

    while (*letter_ptr != '\0') {
        if (*letter_ptr == '\n') {
            letter_ptr++;
            continue;
        }
        char token[2];

        token[0] = *letter_ptr;
        token[1] = '\0';

        if (tokenized.size > 0 && type(tokenized.start[tokenized.size - 1][0]) == type(*letter_ptr) && type(*letter_ptr) != -1) {
            int last_tokenized_size = strlen(tokenized.start[tokenized.size - 1]);
            char* new_token = (char*)realloc(tokenized.start[tokenized.size - 1], last_tokenized_size + 2);
            if (new_token == NULL) {
                fprintf(stderr, "Memory Reallocation Failed (tokenize.h)\n");
                for (int i = 0; i < tokenized.size; i++) {
                    free(SVGetItem(&tokenized, i));
                }
                free(tokenized.start);
                exit(1);
            }
            tokenized.start[tokenized.size - 1] = new_token;
            tokenized.start[tokenized.size - 1][last_tokenized_size] = *letter_ptr;
            tokenized.start[tokenized.size - 1][last_tokenized_size + 1] = '\0';
        } else {
            if (tokenized.size > 0 && (tokenized.start[tokenized.size - 1][0] == ' ' || tokenized.start[tokenized.size - 1][0] == '\n' || tokenized.start[tokenized.size - 1][0] == ';'))
                SVDeleteItem(&tokenized, tokenized.size - 1);

            //char* strdup_token = strdup(token);
            SVPushBack(&tokenized, token);
        }

        letter_ptr++;
    }

    return tokenized;
}

#endif
