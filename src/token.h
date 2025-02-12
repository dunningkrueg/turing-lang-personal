#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_EOF,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_ASSIGN,
    TOKEN_EQUAL,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_SEMICOLON,
    TOKEN_COMMA,
    TOKEN_LET,
    TOKEN_FUNCTION,
    TOKEN_RETURN,
    TOKEN_PRINT
} TokenType;

typedef struct {
    TokenType type;
    char* value;
    int line;
    int column;
} Token;

Token* create_token(TokenType type, const char* value, int line, int column);
void free_token(Token* token);

#endif 