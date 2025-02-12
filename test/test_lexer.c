#include "../src/lexer.h"
#include <assert.h>
#include <string.h>

void test_lexer() {
    char* input = "let x = 42;\n"
                  "function add(a, b) {\n"
                  "    return a + b;\n"
                  "}\n";

    Lexer* lexer = init_lexer(input);
    Token* token;

    token = next_token(lexer);
    assert(token->type == TOKEN_LET);
    free_token(token);

    token = next_token(lexer);
    assert(token->type == TOKEN_IDENTIFIER);
    assert(strcmp(token->value, "x") == 0);
    free_token(token);

    token = next_token(lexer);
    assert(token->type == TOKEN_ASSIGN);
    free_token(token);

    token = next_token(lexer);
    assert(token->type == TOKEN_NUMBER);
    assert(strcmp(token->value, "42") == 0);
    free_token(token);

    token = next_token(lexer);
    assert(token->type == TOKEN_SEMICOLON);
    free_token(token);

    token = next_token(lexer);
    assert(token->type == TOKEN_FUNCTION);
    free_token(token);

    free_lexer(lexer);
} 