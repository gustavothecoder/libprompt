CC = gcc
CC_FLAGS = -g -o
LIB_DIR=/usr/lib/x86_64-linux-gnu
HEADER_DIR=/usr/include

build_tests:
	$(CC) $(CC_FLAGS) tests.out ./tests/*.c ./src/prompt.c -lcmocka
install:
	cp ./src/prompt.h $(HEADER_DIR)/prompt.h
	$(CC) $(CC_FLAGS) $(LIB_DIR)/libprompt.so -fpic -shared ./src/*.c
uninstall:
	rm -f $(HEADER_DIR)/prompt.h
	rm -f $(LIB_DIR)/libprompt.so
clean:
	rm -f ./*.out
