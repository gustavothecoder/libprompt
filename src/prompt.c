#include "prompt.h"

static int is_an_option(char *cmd_string);
static struct Option parse_opt(char *opt_string);
static void remove_break_line(char *str);

int compare_command(char *received, char *expected) {
    return strcmp(received, expected) == 0;
}

struct Prompt parse_prompt(int argc, char *argv[], int (*cmd_table)(char *), int default_cmd) {
    struct Prompt p;
    p.cmd = default_cmd;
    p.opts_sz = 0;

    // We start with 1 because argv[0] will always be the executed binary.
    for (int i = 1, cmd_i = 0; i < argc; i++) {
        if (is_an_option(argv[i])) {
            p.opts[cmd_i] = parse_opt(argv[i]);
            p.opts_sz++;
            cmd_i++;
        } else if (i == 1) {
            p.cmd = cmd_table(argv[1]);
        } else if (p.opts[cmd_i - 1].has_value) {
            strcat(p.opts[cmd_i - 1].value, " ");
            strcat(p.opts[cmd_i - 1].value, argv[i]);
            remove_break_line(p.opts[cmd_i - 1].value);
        }
    }

    return p;
}

static int is_an_option(char *opt_string) {
    return strstr(opt_string, "--") != NULL;
}

static struct Option parse_opt(char *opt_string) {
    struct Option o;
    o.has_value = 0;

    char received_opt[MAX_STR_SIZE];
    strcpy(received_opt, opt_string);
    size_t opt_sz = strlen(received_opt);

    char *value_with_sep = memchr(received_opt, '=', opt_sz);
    if (value_with_sep != NULL) {
        char *only_value = value_with_sep+1;
        remove_break_line(only_value);

        strcpy(o.value, only_value);
        o.has_value = 1;

        memset(value_with_sep, '\0', 1);
    }

    remove_break_line(received_opt);
    strcpy(o.key, received_opt);

    return o;
}

static void remove_break_line(char *str) {
    char *break_line = memchr(str, '\n', strlen(str));
    if (break_line != NULL)
        memset(break_line, '\0', 1);
}
