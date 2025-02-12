#include <stdlib.h>
#include <string.h>
#include "token.h"

Token* create_token(TokenType type, const char* value, int line, int column) {
    Token* token = malloc(sizeof(Token));
    token->type = type;
    token->value = strdup(value);
    token->line = line;
    token->column = column;
    return token;
}

void free_token(Token* token) {
    free(token->value);
    free(token);
} 