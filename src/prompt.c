#include "prompt.h"

struct Prompt parse_prompt(int argc, char *argv[], int (*cmd_table)(char *)) {
    struct Prompt p;
    p.cmd = cmd_table(argv[1]); // The first argv[0] will always be the executed binary.
    p.opts_sz = 0;
    return p;
}
