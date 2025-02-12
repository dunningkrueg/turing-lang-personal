#include <stdlib.h>
#include <string.h>
#include "ast.h"

Node* create_identifier(char* name) {
    if (!name) return NULL;
    Node* node = malloc(sizeof(Node));
    if (!node) return NULL;
    node->type = EXPR_IDENTIFIER;
    node->as.identifier.name = strdup(name);
    return node;
}

Node* create_number(char* value) {
    if (!value) return NULL;
    Node* node = malloc(sizeof(Node));
    if (!node) return NULL;
    node->type = EXPR_NUMBER;
    node->as.number.value = strdup(value);
    return node;
}

Node* create_binary(Node* left, char operator, Node* right) {
    if (!left || !right) return NULL;
    Node* node = malloc(sizeof(Node));
    if (!node) return NULL;
    node->type = EXPR_BINARY;
    node->as.binary.left = left;
    node->as.binary.operator = operator;
    node->as.binary.right = right;
    return node;
}

Node* create_call(Node* function, Node** arguments, int arg_count) {
    Node* node = malloc(sizeof(Node));
    node->type = EXPR_CALL;
    node->as.call.function = function;
    node->as.call.arguments = arguments;
    node->as.call.arg_count = arg_count;
    return node;
}

Node* create_let(char* name, Node* value) {
    if (!name || !value) return NULL;
    Node* node = malloc(sizeof(Node));
    if (!node) return NULL;
    node->type = STMT_LET;
    node->as.let.name = strdup(name);
    node->as.let.value = value;
    return node;
}

Node* create_return(Node* value) {
    Node* node = malloc(sizeof(Node));
    node->type = STMT_RETURN;
    node->as.return_stmt.value = value;
    return node;
}

Node* create_function(char* name, char** params, int param_count, Node** body, int body_count) {
    Node* node = malloc(sizeof(Node));
    node->type = STMT_FUNCTION;
    node->as.function.name = name;
    node->as.function.params = params;
    node->as.function.param_count = param_count;
    node->as.function.body = body;
    node->as.function.body_count = body_count;
    return node;
}

Node* create_string(char* value) {
    if (!value) return NULL;
    Node* node = malloc(sizeof(Node));
    if (!node) return NULL;
    node->type = EXPR_STRING;
    node->as.string.value = strdup(value);
    return node;
}

void free_node(Node* node) {
    if (!node) return;
    
    switch (node->type) {
        case EXPR_IDENTIFIER:
            free(node->as.identifier.name);
            break;
        case EXPR_NUMBER:
            free(node->as.number.value);
            break;
        case EXPR_BINARY:
            free_node(node->as.binary.left);
            free_node(node->as.binary.right);
            break;
        case EXPR_CALL:
            free_node(node->as.call.function);
            for (int i = 0; i < node->as.call.arg_count; i++) {
                free_node(node->as.call.arguments[i]);
            }
            free(node->as.call.arguments);
            break;
        case STMT_LET:
            free(node->as.let.name);
            free_node(node->as.let.value);
            break;
        case STMT_RETURN:
            free_node(node->as.return_stmt.value);
            break;
        case STMT_FUNCTION:
            free(node->as.function.name);
            for (int i = 0; i < node->as.function.param_count; i++) {
                free(node->as.function.params[i]);
            }
            free(node->as.function.params);
            for (int i = 0; i < node->as.function.body_count; i++) {
                free_node(node->as.function.body[i]);
            }
            free(node->as.function.body);
            break;
        default:
            break;
    }
    
    free(node);
} 