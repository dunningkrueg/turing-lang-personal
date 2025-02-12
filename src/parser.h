#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

typedef struct {
    Lexer* lexer;
    Token* current_token;
    Token* peek_token;
} Parser;

Parser* init_parser(Lexer* lexer);
void free_parser(Parser* parser);
void next_token_parser(Parser* parser);

Node* parse_program(Parser* parser);
Node* parse_statement(Parser* parser);
Node* parse_expression(Parser* parser, int precedence);

Node* parse_let_statement(Parser* parser);
Node* parse_return_statement(Parser* parser);
Node* parse_function_statement(Parser* parser);
Node* parse_expression_statement(Parser* parser);
Node* parse_grouped_expression(Parser* parser);
Node* parse_print_statement(Parser* parser);

Node* parse_prefix(Parser* parser);
Node* parse_infix(Parser* parser, Node* left);

#endif 