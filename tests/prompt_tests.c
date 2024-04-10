#include "_cmocka.h"
#include "../src/prompt.h"

int cmd_table(char *str_cmd) {
    if (compare_command(str_cmd, "pulls"))
        return 1;
    else if (strstr(str_cmd, ".c"))
        return RAW_ARG;
    else
        return INVALID_CMD;
}

static void test_command_comparison(void **state) {
    int r1 = compare_command("different", "command");
    int r2 = compare_command("command", "command");
    int r3 = compare_command("Command", "command");

    assert_int_equal(r1, 0);
    assert_int_equal(r2, 1);
    assert_int_equal(r3, 0);
}

static void test_parsing_prompt_with_command(void **state) {
    int argc = 2;
    char *fake_argv[argc];
    fake_argv[0] = "gh";
    fake_argv[1] = "pulls";

    struct Prompt r = parse_prompt(argc, fake_argv, cmd_table, INVALID_CMD);

    assert_int_equal(r.cmd, 1);
    assert_int_equal(r.opts_sz, 0);
}

static void test_parsing_prompt_with_no_command(void **state) {
    int argc = 1;
    char *fake_argv[argc];
    fake_argv[0] = "gh";

    struct Prompt r = parse_prompt(argc, fake_argv, cmd_table, INVALID_CMD);

    assert_int_equal(r.cmd, INVALID_CMD);
    assert_int_equal(r.opts_sz, 0);
}

static void test_parsing_prompt_with_no_command_but_with_default(void **state) {
    int argc = 1;
    char *fake_argv[argc];
    fake_argv[0] = "gh";
    int default_cmd = 1;

    struct Prompt r = parse_prompt(argc, fake_argv, cmd_table, default_cmd);

    assert_int_equal(r.cmd, default_cmd);
    assert_int_equal(r.opts_sz, 0);
}

static void test_parsing_prompt_with_no_command_but_with_default_and_options(void **state) {
    int argc = 3;
    char *fake_argv[argc];
    fake_argv[0] = "gh";
    fake_argv[1] = "--flag";
    fake_argv[2] = "--option=value\n";
    int default_cmd = 1;

    struct Prompt r = parse_prompt(argc, fake_argv, cmd_table, default_cmd);

    assert_int_equal(r.cmd, default_cmd);

    assert_int_equal(r.opts_sz, 2);

    assert_string_equal(r.opts[0].key, "--flag");
    assert_int_equal(r.opts[0].has_value, 0);

    assert_string_equal(r.opts[1].key, "--option");
    assert_int_equal(r.opts[1].has_value, 1);
    assert_string_equal(r.opts[1].value, "value");
}

static void test_parsing_prompt_with_no_command_and_argument(void **state) {
    int argc = 2;
    char *fake_argv[argc];
    fake_argv[0] = "trash";
    fake_argv[1] = "./this/is/a/file.c\n";
    int default_cmd = 1;

    struct Prompt r = parse_prompt(argc, fake_argv, cmd_table, default_cmd);

    assert_int_equal(r.cmd, default_cmd);

    assert_true(r.has_raw_arg);
    assert_string_equal(r.raw_arg, "./this/is/a/file.c\n");
}

static void test_parsing_prompt_with_no_command_and_multiple_arguments(void **state) {
    int argc = 3;
    char *fake_argv[argc];
    fake_argv[0] = "trash";
    fake_argv[1] = "./this/is/a/file.c";
    fake_argv[2] = "./this/is/a/file/too.c\n";
    int default_cmd = 1;

    struct Prompt r = parse_prompt(argc, fake_argv, cmd_table, default_cmd);

    assert_int_equal(r.cmd, default_cmd);

    assert_true(r.has_raw_arg);
    assert_string_equal(r.raw_arg, "./this/is/a/file.c ./this/is/a/file/too.c\n");
}

static void test_parsing_prompt_with_not_recognized_command(void **state) {
    int argc = 2;
    char *fake_argv[argc];
    fake_argv[0] = "gh";
    fake_argv[1] = "not_recognized";

    struct Prompt r = parse_prompt(argc, fake_argv, cmd_table, INVALID_CMD);

    assert_int_equal(r.cmd, INVALID_CMD);
    assert_int_equal(r.opts_sz, 0);
}

static void test_parsing_prompt_with_flag(void **state) {
    int argc = 3;
    char *fake_argv[argc];
    fake_argv[0] = "gh";
    fake_argv[1] = "pulls";
    fake_argv[2] = "--to-review";

    struct Prompt r = parse_prompt(argc, fake_argv, cmd_table, INVALID_CMD);

    assert_int_equal(r.cmd, 1);
    assert_int_equal(r.opts_sz, 1);
    assert_string_equal(r.opts[0].key, "--to-review");
    assert_int_equal(r.opts[0].has_value, 0);
}

static void test_parsing_prompt_with_option(void **state) {
    int argc = 3;
    char *fake_argv[argc];
    fake_argv[0] = "gh";
    fake_argv[1] = "pulls";
    fake_argv[2] = "--author=gustavothecoder\n";

    struct Prompt r = parse_prompt(argc, fake_argv, cmd_table, INVALID_CMD);

    assert_int_equal(r.cmd, 1);
    assert_int_equal(r.opts_sz, 1);
    assert_string_equal(r.opts[0].key, "--author");
    assert_int_equal(r.opts[0].has_value, 1);
    assert_string_equal(r.opts[0].value, "gustavothecoder");
}

static void test_parsing_prompt_with_flag_and_option(void **state) {
    int argc = 4;
    char *fake_argv[argc];
    fake_argv[0] = "gh";
    fake_argv[1] = "pulls";
    fake_argv[2] = "--author=gustavothecoder";
    fake_argv[3] = "--closed\n";

    struct Prompt r = parse_prompt(argc, fake_argv, cmd_table, INVALID_CMD);

    assert_int_equal(r.cmd, 1);

    assert_int_equal(r.opts_sz, 2);

    assert_string_equal(r.opts[0].key, "--author");
    assert_int_equal(r.opts[0].has_value, 1);
    assert_string_equal(r.opts[0].value, "gustavothecoder");

    assert_string_equal(r.opts[1].key, "--closed");
    assert_int_equal(r.opts[1].has_value, 0);
}

static void test_parsing_prompt_with_spaces(void **state) {
    int argc = 7;
    char *fake_argv[argc];
    fake_argv[0] = "gh";
    fake_argv[1] = "pulls";
    fake_argv[2] = "--title=This";
    fake_argv[3] = "title";
    fake_argv[4] = "is";
    fake_argv[5] = "an";
    fake_argv[6] = "example\n";

    struct Prompt r = parse_prompt(argc, fake_argv, cmd_table, INVALID_CMD);

    assert_int_equal(r.cmd, 1);

    assert_int_equal(r.opts_sz, 1);

    assert_string_equal(r.opts[0].key, "--title");
    assert_int_equal(r.opts[0].has_value, 1);
    assert_string_equal(r.opts[0].value, "This title is an example");
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_command_comparison),
        cmocka_unit_test(test_parsing_prompt_with_command),
        cmocka_unit_test(test_parsing_prompt_with_no_command),
        cmocka_unit_test(test_parsing_prompt_with_no_command_but_with_default),
        cmocka_unit_test(test_parsing_prompt_with_no_command_but_with_default_and_options),
        cmocka_unit_test(test_parsing_prompt_with_no_command_and_argument),
        cmocka_unit_test(test_parsing_prompt_with_no_command_and_multiple_arguments),
        cmocka_unit_test(test_parsing_prompt_with_not_recognized_command),
        cmocka_unit_test(test_parsing_prompt_with_flag),
        cmocka_unit_test(test_parsing_prompt_with_option),
        cmocka_unit_test(test_parsing_prompt_with_flag_and_option),
        cmocka_unit_test(test_parsing_prompt_with_spaces)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
