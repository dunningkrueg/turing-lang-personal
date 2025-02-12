#ifndef AST_H
#define AST_H

typedef enum {
    EXPR_IDENTIFIER,
    EXPR_NUMBER,
    EXPR_STRING,
    EXPR_BINARY,
    EXPR_CALL,
    STMT_LET,
    STMT_RETURN,
    STMT_FUNCTION
} NodeType;

typedef struct Node {
    NodeType type;
    union {
        struct {
            char* name;
        } identifier;
        struct {
            char* value;
        } number;
        struct {
            char* value;
        } string;
        struct {
            struct Node* left;
            char operator;
            struct Node* right;
        } binary;
        struct {
            struct Node* function;
            struct Node** arguments;
            int arg_count;
        } call;
        struct {
            char* name;
            struct Node* value;
        } let;
        struct {
            struct Node* value;
        } return_stmt;
        struct {
            char* name;
            char** params;
            int param_count;
            struct Node** body;
            int body_count;
        } function;
    } as;
} Node;

Node* create_identifier(char* name);
Node* create_number(char* value);
Node* create_string(char* value);
Node* create_binary(Node* left, char operator, Node* right);
Node* create_call(Node* function, Node** arguments, int arg_count);
Node* create_let(char* name, Node* value);
Node* create_return(Node* value);
Node* create_function(char* name, char** params, int param_count, Node** body, int body_count);
void free_node(Node* node);

#endif 