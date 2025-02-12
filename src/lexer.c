#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"
#include "token.h"

Lexer* init_lexer(char* input) {
    Lexer* lexer = malloc(sizeof(Lexer));
    lexer->input = input;
    lexer->position = 0;
    lexer->read_position = 0;
    lexer->current_char = 0;
    lexer->line = 1;
    lexer->column = 1;
    read_char(lexer);
    return lexer;
}

void read_char(Lexer* lexer) {
    if (lexer->read_position >= strlen(lexer->input)) {
        lexer->current_char = 0;
    } else {
        lexer->current_char = lexer->input[lexer->read_position];
    }
    lexer->position = lexer->read_position;
    lexer->read_position++;
    
    if (lexer->current_char == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }
}

char peek_char(Lexer* lexer) {
    if (lexer->read_position >= strlen(lexer->input)) {
        return 0;
    }
    return lexer->input[lexer->read_position];
}

char* read_identifier_string(Lexer* lexer) {
    int start_pos = lexer->position;
    while (isalnum(lexer->current_char) || lexer->current_char == '_') {
        read_char(lexer);
    }
    
    int length = lexer->position - start_pos;
    char* value = malloc(length + 1);
    strncpy(value, &lexer->input[start_pos], length);
    value[length] = 0;
    return value;
}

Token* read_identifier(Lexer* lexer) {
    char* value = read_identifier_string(lexer);
    Token* token;
    
    if (strcmp(value, "let") == 0) {
        token = create_token(TOKEN_LET, value, lexer->line, lexer->column - strlen(value));
    } else if (strcmp(value, "function") == 0) {
        token = create_token(TOKEN_FUNCTION, value, lexer->line, lexer->column - strlen(value));
    } else if (strcmp(value, "return") == 0) {
        token = create_token(TOKEN_RETURN, value, lexer->line, lexer->column - strlen(value));
    } else if (strcmp(value, "print") == 0) {
        token = create_token(TOKEN_PRINT, value, lexer->line, lexer->column - strlen(value));
    } else {
        token = create_token(TOKEN_IDENTIFIER, value, lexer->line, lexer->column - strlen(value));
    }
    
    free(value);
    return token;
}

Token* read_number(Lexer* lexer) {
    int start_pos = lexer->position;
    while (isdigit(lexer->current_char)) {
        read_char(lexer);
    }
    
    int length = lexer->position - start_pos;
    char* value = malloc(length + 1);
    strncpy(value, &lexer->input[start_pos], length);
    value[length] = 0;
    
    Token* token = create_token(TOKEN_NUMBER, value, lexer->line, lexer->column - length);
    free(value);
    return token;
}

Token* read_string(Lexer* lexer) {
    read_char(lexer);  // consume opening quote
    int start_pos = lexer->position;
    
    while (lexer->current_char != '"' && lexer->current_char != 0) {
        read_char(lexer);
    }
    
    int length = lexer->position - start_pos;
    char* value = malloc(length + 1);
    strncpy(value, &lexer->input[start_pos], length);
    value[length] = 0;
    
    Token* token = create_token(TOKEN_STRING, value, lexer->line, lexer->column - length);
    free(value);
    read_char(lexer);  // consume closing quote
    return token;
}

Token* next_token(Lexer* lexer) {
    Token* token;
    
    while (isspace(lexer->current_char)) {
        read_char(lexer);
    }
    
    switch (lexer->current_char) {
        case '+':
            token = create_token(TOKEN_PLUS, "+", lexer->line, lexer->column);
            break;
        case '-':
            token = create_token(TOKEN_MINUS, "-", lexer->line, lexer->column);
            break;
        case '*':
            token = create_token(TOKEN_MULTIPLY, "*", lexer->line, lexer->column);
            break;
        case '/':
            token = create_token(TOKEN_DIVIDE, "/", lexer->line, lexer->column);
            break;
        case '=':
            token = create_token(TOKEN_ASSIGN, "=", lexer->line, lexer->column);
            break;
        case '(':
            token = create_token(TOKEN_LPAREN, "(", lexer->line, lexer->column);
            break;
        case ')':
            token = create_token(TOKEN_RPAREN, ")", lexer->line, lexer->column);
            break;
        case '{':
            token = create_token(TOKEN_LBRACE, "{", lexer->line, lexer->column);
            break;
        case '}':
            token = create_token(TOKEN_RBRACE, "}", lexer->line, lexer->column);
            break;
        case ';':
            token = create_token(TOKEN_SEMICOLON, ";", lexer->line, lexer->column);
            break;
        case ',':
            token = create_token(TOKEN_COMMA, ",", lexer->line, lexer->column);
            break;
        case '"':
            return read_string(lexer);
        case 0:
            token = create_token(TOKEN_EOF, "", lexer->line, lexer->column);
            break;
        default:
            if (isalpha(lexer->current_char)) {
                return read_identifier(lexer);
            } else if (isdigit(lexer->current_char)) {
                return read_number(lexer);
            } else {
                token = create_token(TOKEN_EOF, "", lexer->line, lexer->column);
            }
    }
    
    read_char(lexer);
    return token;
}

void free_lexer(Lexer* lexer) {
    free(lexer);
} 