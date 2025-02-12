#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void test_lexer();
void test_parser();
void test_codegen();

int main() {
    printf("Running Turing compiler tests...\n\n");

    printf("=== Lexer Tests ===\n");
    test_lexer();
    printf("Lexer tests passed!\n\n");

    printf("=== Parser Tests ===\n");
    test_parser();
    printf("Parser tests passed!\n\n");

    printf("=== CodeGen Tests ===\n");
    test_codegen();
    printf("CodeGen tests passed!\n\n");

    printf("All tests passed successfully!\n");
    return 0;
} 