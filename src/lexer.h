#ifndef LEXER_H
#define LEXER_H

#include "token.h"

typedef struct {
    char* input;
    int position;
    int read_position;
    char current_char;
    int line;
    int column;
} Lexer;

Lexer* init_lexer(char* input);
void free_lexer(Lexer* lexer);
Token* next_token(Lexer* lexer);
void read_char(Lexer* lexer);
char peek_char(Lexer* lexer);
Token* read_identifier(Lexer* lexer);
Token* read_number(Lexer* lexer);
Token* read_string(Lexer* lexer);
char* read_identifier_string(Lexer* lexer);

#endif 