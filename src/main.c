#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lexer.h"
#include "ast.h"
#include "parser.h"
#include "codegen.h"

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define RESET   "\x1b[0m"

typedef struct {
    char* name;
    Node* value;
} Variable;

typedef struct {
    Variable* variables;
    int count;
    int capacity;
} Environment;

Environment* create_environment() {
    Environment* env = malloc(sizeof(Environment));
    env->capacity = 16;
    env->count = 0;
    env->variables = malloc(sizeof(Variable) * env->capacity);
    return env;
}

void set_variable(Environment* env, char* name, Node* value) {
    for (int i = 0; i < env->count; i++) {
        if (strcmp(env->variables[i].name, name) == 0) {
            free_node(env->variables[i].value);
            env->variables[i].value = value;
            return;
        }
    }

    if (env->count >= env->capacity) {
        env->capacity *= 2;
        env->variables = realloc(env->variables, sizeof(Variable) * env->capacity);
    }

    env->variables[env->count].name = strdup(name);
    env->variables[env->count].value = value;
    env->count++;
}

Node* get_variable(Environment* env, char* name) {
    for (int i = 0; i < env->count; i++) {
        if (strcmp(env->variables[i].name, name) == 0) {
            return env->variables[i].value;
        }
    }
    return NULL;
}

void free_environment(Environment* env) {
    for (int i = 0; i < env->count; i++) {
        free(env->variables[i].name);
        free_node(env->variables[i].value);
    }
    free(env->variables);
    free(env);
}

Node* evaluate_expression(Environment* env, Node* node) {
    if (!node) return NULL;

    switch (node->type) {
        case EXPR_NUMBER:
            return create_number(node->as.number.value);
        case EXPR_STRING:
            return create_string(node->as.string.value);
        case EXPR_IDENTIFIER: {
            Node* value = get_variable(env, node->as.identifier.name);
            if (!value) {
                printf(RED "Error: Undefined variable '%s'\n" RESET, node->as.identifier.name);
                return NULL;
            }
            return value;
        }
        case EXPR_BINARY: {
            Node* left = evaluate_expression(env, node->as.binary.left);
            Node* right = evaluate_expression(env, node->as.binary.right);
            if (!left || !right) return NULL;

            int left_val = atoi(left->as.number.value);
            int right_val = atoi(right->as.number.value);
            int result;

            switch (node->as.binary.operator) {
                case '+': result = left_val + right_val; break;
                case '-': result = left_val - right_val; break;
                case '*': result = left_val * right_val; break;
                case '/':
                    if (right_val == 0) {
                        printf(RED "Error: Division by zero\n" RESET);
                        return NULL;
                    }
                    result = left_val / right_val;
                    break;
                default:
                    printf(RED "Error: Unknown operator '%c'\n" RESET, node->as.binary.operator);
                    return NULL;
            }

            char value[32];
            sprintf(value, "%d", result);
            return create_number(value);
        }
        default:
            return NULL;
    }
}

void execute_statement(Environment* env, Node* node) {
    if (!node) return;

    switch (node->type) {
        case STMT_LET: {
            Node* value = evaluate_expression(env, node->as.let.value);
            if (value) {
                set_variable(env, node->as.let.name, value);
            }
            break;
        }
        case EXPR_CALL: {
            if (strcmp(node->as.call.function->as.identifier.name, "print") == 0) {
                for (int i = 0; i < node->as.call.arg_count; i++) {
                    Node* arg = evaluate_expression(env, node->as.call.arguments[i]);
                    if (arg) {
                        if (arg->type == EXPR_STRING) {
                            printf("%s", arg->as.string.value);
                        } else if (arg->type == EXPR_NUMBER) {
                            printf("%s", arg->as.number.value);
                        }
                    }
                }
                printf("\n");
            }
            break;
        }
        case STMT_FUNCTION: {
            for (int i = 0; i < node->as.function.body_count; i++) {
                execute_statement(env, node->as.function.body[i]);
            }
            break;
        }
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf(RED "Usage: turing <filename.tr>\n" RESET);
        return 1;
    }

    char* filename = argv[1];
    if (!strstr(filename, ".tr")) {
        printf(RED "Error: File must have .tr extension\n" RESET);
        return 1;
    }

    clock_t start = clock();

    FILE* file = fopen(filename, "r");
    if (!file) {
        printf(RED "Error: Cannot open file %s\n" RESET, filename);
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* input = malloc(size + 1);
    fread(input, 1, size, file);
    input[size] = 0;
    fclose(file);

    printf(BLUE "Compiling %s...\n" RESET, filename);

    Lexer* lexer = init_lexer(input);
    Parser* parser = init_parser(lexer);
    Node* ast = parse_program(parser);

    if (!ast) {
        printf(RED "Error: Failed to parse program\n" RESET);
        free(input);
        free_lexer(lexer);
        free_parser(parser);
        return 1;
    }

    Environment* env = create_environment();
    execute_statement(env, ast);

    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    printf(GREEN "\nBuild successful" RESET " in %.3f seconds\n", time_spent);

    free_environment(env);
    free_node(ast);
    free_parser(parser);
    free_lexer(lexer);
    free(input);

    return 0;
} 