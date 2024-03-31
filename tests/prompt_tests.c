#include "_cmocka.h"
#include "../src/prompt.h"

int cmd_table(char *str_cmd) {
    if (strcmp(str_cmd, "pulls") == 0)
        return 1;
    else
        return -1;
}

static void test_parsing_prompt_with_command(void **state) {
    int argc = 2;
    char *fake_argv[argc];
    fake_argv[0] = "gh";
    fake_argv[1] = "pulls";

    struct Prompt r = parse_prompt(argc, fake_argv, cmd_table);

    assert_int_equal(r.cmd, 1);
    assert_int_equal(r.opts_sz, 0);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_parsing_prompt_with_command)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
