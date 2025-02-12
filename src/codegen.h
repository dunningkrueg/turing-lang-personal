#ifndef CODEGEN_H
#define CODEGEN_H

#include <stddef.h>
#include "ast.h"

typedef struct {
    char* output;
    size_t capacity;
    size_t length;
} CodeGenerator;

CodeGenerator* init_codegen();
void free_codegen(CodeGenerator* gen);
void generate_code(CodeGenerator* gen, Node* node);
char* get_output(CodeGenerator* gen);

#endif 