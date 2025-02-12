#include "../src/codegen.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

void test_simple_expression() {
    CodeGenerator* gen = init_codegen();
    assert(gen != NULL);
    
    Node* number = create_number("42");
    assert(number != NULL);
    
    generate_code(gen, number);
    char* output = get_output(gen);
    assert(output != NULL);
    assert(strstr(output, "42") != NULL);
    
    free(output);
    free_node(number);
    free_codegen(gen);
}

void test_binary_expression() {
    CodeGenerator* gen = init_codegen();
    assert(gen != NULL);
    
    Node* left = create_number("10");
    Node* right = create_number("20");
    Node* binary = create_binary(left, '+', right);
    assert(binary != NULL);
    
    generate_code(gen, binary);
    char* output = get_output(gen);
    assert(output != NULL);
    assert(strstr(output, "(10 + 20)") != NULL);
    
    free(output);
    free_node(binary);
    free_codegen(gen);
}

void test_let_statement() {
    CodeGenerator* gen = init_codegen();
    assert(gen != NULL);
    
    Node* number = create_number("42");
    Node* identifier = create_identifier("x");
    Node* binary = create_binary(identifier, '+', number);
    Node* let = create_let("result", binary);
    assert(let != NULL);
    
    generate_code(gen, let);
    char* output = get_output(gen);
    assert(output != NULL);
    assert(strstr(output, "let result = (x + 42)") != NULL);
    
    free(output);
    free_node(let);
    free_codegen(gen);
}

void test_codegen() {
    printf("Running CodeGen tests...\n");
    
    printf("Testing simple expression...\n");
    test_simple_expression();
    
    printf("Testing binary expression...\n");
    test_binary_expression();
    
    printf("Testing let statement...\n");
    test_let_statement();
    
    printf("All CodeGen tests passed!\n");
} 