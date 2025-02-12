#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"

#define INITIAL_CAPACITY 4096

void append_string(CodeGenerator* gen, const char* str) {
    size_t len = strlen(str);
    while (gen->length + len + 1 > gen->capacity) {
        gen->capacity *= 2;
        gen->output = realloc(gen->output, gen->capacity);
    }
    strcpy(gen->output + gen->length, str);
    gen->length += len;
}

void append_char(CodeGenerator* gen, char c) {
    if (gen->length + 2 > gen->capacity) {
        gen->capacity *= 2;
        gen->output = realloc(gen->output, gen->capacity);
    }
    gen->output[gen->length++] = c;
    gen->output[gen->length] = '\0';
}

CodeGenerator* init_codegen() {
    CodeGenerator* gen = malloc(sizeof(CodeGenerator));
    gen->capacity = INITIAL_CAPACITY;
    gen->output = malloc(gen->capacity);
    gen->length = 0;
    gen->output[0] = '\0';
    return gen;
}

void generate_expression(CodeGenerator* gen, Node* node) {
    char buffer[1024];
    
    if (!node) return;

    switch (node->type) {
        case EXPR_IDENTIFIER:
            append_string(gen, node->as.identifier.name);
            break;
            
        case EXPR_NUMBER:
            append_string(gen, node->as.number.value);
            break;
            
        case EXPR_BINARY:
            append_char(gen, '(');
            generate_expression(gen, node->as.binary.left);
            sprintf(buffer, " %c ", node->as.binary.operator);
            append_string(gen, buffer);
            generate_expression(gen, node->as.binary.right);
            append_char(gen, ')');
            break;
            
        case EXPR_CALL:
            generate_expression(gen, node->as.call.function);
            append_char(gen, '(');
            for (int i = 0; i < node->as.call.arg_count; i++) {
                if (i > 0) append_string(gen, ", ");
                generate_expression(gen, node->as.call.arguments[i]);
            }
            append_char(gen, ')');
            break;
            
        default:
            break;
    }
}

void generate_statement(CodeGenerator* gen, Node* node) {
    if (!node) return;

    switch (node->type) {
        case STMT_LET:
            append_string(gen, "let ");
            append_string(gen, node->as.let.name);
            append_string(gen, " = ");
            generate_expression(gen, node->as.let.value);
            append_string(gen, ";\n");
            break;
            
        case STMT_RETURN:
            append_string(gen, "return ");
            generate_expression(gen, node->as.return_stmt.value);
            append_string(gen, ";\n");
            break;
            
        case STMT_FUNCTION:
            append_string(gen, "function ");
            append_string(gen, node->as.function.name);
            append_string(gen, "(");
            
            for (int i = 0; i < node->as.function.param_count; i++) {
                if (i > 0) append_string(gen, ", ");
                append_string(gen, node->as.function.params[i]);
            }
            
            append_string(gen, ") {\n");
            
            for (int i = 0; i < node->as.function.body_count; i++) {
                append_string(gen, "    ");
                generate_statement(gen, node->as.function.body[i]);
            }
            
            append_string(gen, "}\n");
            break;
            
        default:
            generate_expression(gen, node);
            append_string(gen, ";\n");
            break;
    }
}

void generate_code(CodeGenerator* gen, Node* node) {
    if (!node) return;
    generate_statement(gen, node);
}

char* get_output(CodeGenerator* gen) {
    return strdup(gen->output);
}

void free_codegen(CodeGenerator* gen) {
    if (gen) {
        free(gen->output);
        free(gen);
    }
} 