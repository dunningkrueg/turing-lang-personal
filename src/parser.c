#include <stdlib.h>
#include <string.h>
#include "parser.h"

typedef enum {
    PREC_LOWEST,
    PREC_EQUALS,
    PREC_LESSGREATER,
    PREC_SUM,
    PREC_PRODUCT,
    PREC_PREFIX,
    PREC_CALL
} Precedence;

Parser* init_parser(Lexer* lexer) {
    Parser* parser = malloc(sizeof(Parser));
    parser->lexer = lexer;
    parser->current_token = next_token(lexer);
    parser->peek_token = next_token(lexer);
    return parser;
}

void next_token_parser(Parser* parser) {
    free_token(parser->current_token);
    parser->current_token = parser->peek_token;
    parser->peek_token = next_token(parser->lexer);
}

int get_precedence(TokenType type) {
    switch (type) {
        case TOKEN_EQUAL: return PREC_EQUALS;
        case TOKEN_PLUS:
        case TOKEN_MINUS: return PREC_SUM;
        case TOKEN_MULTIPLY:
        case TOKEN_DIVIDE: return PREC_PRODUCT;
        case TOKEN_LPAREN: return PREC_CALL;
        default: return PREC_LOWEST;
    }
}

Node* parse_program(Parser* parser) {
    Node** statements = malloc(sizeof(Node*) * 1024);
    int count = 0;

    while (parser->current_token->type != TOKEN_EOF) {
        Node* stmt = parse_statement(parser);
        if (stmt) {
            statements[count++] = stmt;
        }
        next_token_parser(parser);
    }

    Node* program = malloc(sizeof(Node));
    program->type = STMT_FUNCTION;
    program->as.function.name = strdup("program");
    program->as.function.params = NULL;
    program->as.function.param_count = 0;
    program->as.function.body = statements;
    program->as.function.body_count = count;

    return program;
}

Node* parse_statement(Parser* parser) {
    switch (parser->current_token->type) {
        case TOKEN_LET:
            return parse_let_statement(parser);
        case TOKEN_RETURN:
            return parse_return_statement(parser);
        case TOKEN_FUNCTION:
            return parse_function_statement(parser);
        case TOKEN_PRINT:
            return parse_print_statement(parser);
        default:
            return parse_expression_statement(parser);
    }
}

Node* parse_let_statement(Parser* parser) {
    next_token_parser(parser);
    
    if (parser->current_token->type != TOKEN_IDENTIFIER) {
        return NULL;
    }
    
    char* name = strdup(parser->current_token->value);
    next_token_parser(parser);
    
    if (parser->current_token->type != TOKEN_ASSIGN) {
        free(name);
        return NULL;
    }
    
    next_token_parser(parser);
    Node* value = parse_expression(parser, PREC_LOWEST);
    
    if (parser->peek_token->type == TOKEN_SEMICOLON) {
        next_token_parser(parser);
    }
    
    return create_let(name, value);
}

Node* parse_expression(Parser* parser, int precedence) {
    Node* left = parse_prefix(parser);
    
    while (parser->peek_token->type != TOKEN_SEMICOLON && 
           precedence < get_precedence(parser->peek_token->type)) {
        next_token_parser(parser);
        left = parse_infix(parser, left);
    }
    
    return left;
}

Node* parse_prefix(Parser* parser) {
    switch (parser->current_token->type) {
        case TOKEN_IDENTIFIER:
            return create_identifier(strdup(parser->current_token->value));
        case TOKEN_NUMBER:
            return create_number(strdup(parser->current_token->value));
        case TOKEN_LPAREN:
            return parse_grouped_expression(parser);
        default:
            return NULL;
    }
}

Node* parse_infix(Parser* parser, Node* left) {
    TokenType operator_type = parser->current_token->type;
    int precedence = get_precedence(operator_type);
    next_token_parser(parser);
    Node* right = parse_expression(parser, precedence);
    
    char op;
    switch (operator_type) {
        case TOKEN_PLUS: op = '+'; break;
        case TOKEN_MINUS: op = '-'; break;
        case TOKEN_MULTIPLY: op = '*'; break;
        case TOKEN_DIVIDE: op = '/'; break;
        default: op = '?'; break;
    }
    
    return create_binary(left, op, right);
}

Node* parse_return_statement(Parser* parser) {
    next_token_parser(parser);
    
    Node* value = parse_expression(parser, PREC_LOWEST);
    
    if (parser->peek_token->type == TOKEN_SEMICOLON) {
        next_token_parser(parser);
    }
    
    return create_return(value);
}

Node* parse_function_statement(Parser* parser) {
    next_token_parser(parser);
    
    if (parser->current_token->type != TOKEN_IDENTIFIER) {
        return NULL;
    }
    
    char* name = strdup(parser->current_token->value);
    next_token_parser(parser);
    
    if (parser->current_token->type != TOKEN_LPAREN) {
        free(name);
        return NULL;
    }
    
    next_token_parser(parser);
    
    char** params = malloc(sizeof(char*) * 100);
    int param_count = 0;
    
    while (parser->current_token->type != TOKEN_RPAREN) {
        if (parser->current_token->type == TOKEN_IDENTIFIER) {
            params[param_count++] = strdup(parser->current_token->value);
        }
        next_token_parser(parser);
        
        if (parser->current_token->type == TOKEN_COMMA) {
            next_token_parser(parser);
        }
    }
    
    next_token_parser(parser);
    
    if (parser->current_token->type != TOKEN_LBRACE) {
        free(name);
        for (int i = 0; i < param_count; i++) {
            free(params[i]);
        }
        free(params);
        return NULL;
    }
    
    next_token_parser(parser);
    
    Node** body = malloc(sizeof(Node*) * 100);
    int body_count = 0;
    
    while (parser->current_token->type != TOKEN_RBRACE) {
        Node* stmt = parse_statement(parser);
        if (stmt) {
            body[body_count++] = stmt;
        }
        next_token_parser(parser);
    }
    
    return create_function(name, params, param_count, body, body_count);
}

Node* parse_expression_statement(Parser* parser) {
    Node* expression = parse_expression(parser, PREC_LOWEST);
    
    if (parser->peek_token->type == TOKEN_SEMICOLON) {
        next_token_parser(parser);
    }
    
    return expression;
}

Node* parse_grouped_expression(Parser* parser) {
    next_token_parser(parser);
    
    Node* expression = parse_expression(parser, PREC_LOWEST);
    
    if (parser->peek_token->type != TOKEN_RPAREN) {
        return NULL;
    }
    
    next_token_parser(parser);
    return expression;
}

Node* parse_print_statement(Parser* parser) {
    next_token_parser(parser);  // consume print token
    
    if (parser->current_token->type != TOKEN_LPAREN) {
        return NULL;
    }
    
    next_token_parser(parser);  // consume (
    
    Node** arguments = malloc(sizeof(Node*) * 1);
    int arg_count = 0;
    
    if (parser->current_token->type == TOKEN_STRING) {
        Node* string = create_string(parser->current_token->value);
        arguments[arg_count++] = string;
        next_token_parser(parser);
    } else {
        Node* expr = parse_expression(parser, PREC_LOWEST);
        if (expr) {
            arguments[arg_count++] = expr;
        }
    }
    
    if (parser->current_token->type != TOKEN_RPAREN) {
        free(arguments);
        return NULL;
    }
    
    next_token_parser(parser);  // consume )
    
    Node* print_func = create_identifier("print");
    return create_call(print_func, arguments, arg_count);
}

void free_parser(Parser* parser) {
    free_token(parser->current_token);
    free_token(parser->peek_token);
    free(parser);
} 