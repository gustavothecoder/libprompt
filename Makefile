CC = gcc
CC_FLAGS = -g -o

build_tests:
	$(CC) $(CC_FLAGS) tests.out ./tests/*.c ./src/prompt.c -lcmocka
install:
uninstall:
clean:
	rm -f ./*.out
