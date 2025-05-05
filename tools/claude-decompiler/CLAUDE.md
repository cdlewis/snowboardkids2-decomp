# CLAUDE.md

## Your Job

You are decompiling Nintendo 64 assembly code from Snowboard Kids 2. Your goal is to generate C code that when compiled 100% matches the given assembly code. Do not worry about stack addresses being exactly correct. Or stack size being exactly correct. Or register usage being exactly correct.

The compiler is GCC 2.7.2. The following compiler flags are being used: -O2 -mips3. We follow the C89 standard. Keep in mind that variable declarations are generally limited to the beginning of blocks or functions.

Repeat the following steps:

1. Run `./claude-compile-and-analyse.sh base.c` to build base.c and get an object dump of the compiled code. You will also get a score, a score of 0 is a perfect score indicating a 100% match.
2. Pick a single line that doesn't match. Think about why it doesn't match. Try to focus on lines with a functional difference rather than different registers.
3. Create a new file with your changes (base_n.c where `n` is your attempt number).
4. Run `./claude-compile-and-analyse.sh base_n.c` (where `n` is your attempt number).
5. Repeat until the files are a 100% match, as indicated by the score in `claude-compile-and-analyse.sh`.

## Coding Guidelines

* Never add comments to code.
* Where appropriate, generate structs to match the expected input arguments.
* Never use #include statements.
* Never add comments.