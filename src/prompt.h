#include <string.h>

#define MAX_STR_SIZE 512
#define MAX_CMD_OPTS 5

struct Option {
    char key[MAX_STR_SIZE];
    char value[MAX_STR_SIZE];
};

struct Prompt {
    int cmd;
    int opts_sz;
    struct Option opts[MAX_CMD_OPTS];
};

struct Prompt parse_prompt(int argc, char *argv[], int (*cmd_table)(char *));
