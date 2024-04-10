#include <string.h>

#define MAX_STR_SIZE 512
#define MAX_CMD_OPTS 5
#define INVALID_CMD -1
#define RAW_ARG -2

struct Option {
    char key[MAX_STR_SIZE];
    int has_value;
    char value[MAX_STR_SIZE];
};

// binary [cmd || bin_arg] [opts]
struct Prompt {
    int cmd;

    int has_raw_arg;
    char raw_arg[MAX_STR_SIZE];

    int opts_sz;
    struct Option opts[MAX_CMD_OPTS];
};

int compare_command(char *received, char *expected);
struct Prompt parse_prompt(int argc, char *argv[], int (*cmd_table)(char *), int default_cmd);
