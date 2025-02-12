#include "../src/parser.h"
#include <assert.h>
#include <string.h>

void test_parser() {
    char* input = "let x = 42;\n"
                  "let y = x + 10;\n"
                  "function add(a, b) {\n"
                  "    return a + b;\n"
                  "}\n";

    Lexer* lexer = init_lexer(input);
    Parser* parser = init_parser(lexer);
    Node* program = parse_program(parser);

    assert(program != NULL);
    assert(program->type == STMT_FUNCTION);
    assert(program->as.function.body_count == 3);

    Node* let_stmt = program->as.function.body[0];
    assert(let_stmt->type == STMT_LET);
    assert(strcmp(let_stmt->as.let.name, "x") == 0);
    assert(let_stmt->as.let.value->type == EXPR_NUMBER);
    assert(strcmp(let_stmt->as.let.value->as.number.value, "42") == 0);

    free_node(program);
    free_parser(parser);
    free_lexer(lexer);
} 