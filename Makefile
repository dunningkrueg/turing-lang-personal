CC = gcc
CFLAGS = -Wall -Wextra -I./src

SRCS = src/main.c src/lexer.c src/parser.c src/token.c src/ast.c src/codegen.c
TEST_SRCS = test/test_main.c test/test_lexer.c test/test_parser.c test/test_codegen.c
OBJS = $(SRCS:.c=.o)
TEST_OBJS = $(TEST_SRCS:.c=.o)
TARGET = turing
TEST_TARGET = run_tests

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

test: $(TEST_OBJS) $(filter-out src/main.o, $(OBJS))
	$(CC) $(TEST_OBJS) $(filter-out src/main.o, $(OBJS)) -o $(TEST_TARGET)
	./$(TEST_TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TEST_OBJS) $(TARGET) $(TEST_TARGET)

.PHONY: test clean