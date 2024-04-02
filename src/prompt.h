#include <string.h>

#define MAX_STR_SIZE 512
#define MAX_CMD_OPTS 5
#define INVALID_CMD -1

struct Option {
    char key[MAX_STR_SIZE];
    int has_value;
    char value[MAX_STR_SIZE];
};

struct Prompt {
    int cmd;
    int opts_sz;
    struct Option opts[MAX_CMD_OPTS];
};

int compare_command(char *received, char *expected);
struct Prompt parse_prompt(int argc, char *argv[], int (*cmd_table)(char *), int default_cmd);
