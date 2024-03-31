# libprompt

`libprompt` is a library to interpret CLI prompts. It can identify commands, options and option values.

## Summary
```c
#include <prompt.h>

int main(int argc, char *argv[]) {
    // argc => 3
    // argv => ["cli", "cmd", "--option=value"]
    struct Prompt p = parse_prompt(argc, argv);

    printf("%s\n", p.cmd); // => cmd

    printf("%s\n", p.opts[0].key); // => --option
    printf("%s\n", p.opts[0].value); // => value

    return EXIT_SUCCESS;
}
```

## Installation

### Dependencies

You need to be able to compile C code. Installing `build-essential` for Debian-based distros or `base-devel` for Arch-based distros should be sufficient.

### Steps

Just run:
```bash
$ make install
```

To uninstall:
```bash
$ make uninstall
```

## Usage

TODO

## Contributing

Bug reports and pull requests are welcome :)
