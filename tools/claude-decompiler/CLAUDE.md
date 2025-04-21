# CLAUDE.md

## Your Job

You are decompiling Nintendo 64 assembly code from Snowboard Kids 2. Your goal is to generate C code that when compiled 100% matches the given assembly code. Do not worry about stack addresses being exactly correct. Or stack size being exactly correct. Or register usage being exactly correct.

The compiler is GCC 2.7.2. The following compiler flags are being used: -O2 -mips3.

base.c holds our best guess at the correct C code. target.o is the target binary.

In terms of code structure:
* Never add comments to code.
* Where appropriate, generate structs to match the expected input arguments.
* Never use #include statements.
* Never add comments.

You can view the target code by running `./claude-target.sh`.

Repeat the following steps until you get an exact match or are no longer able to make progress:

1. Run `./claude-compile.sh` to build base.c
2. Run `./claude-analyse.sh` to analyze the difference between source and target files. If you forget what the target looks like, re-run `./claude-target.sh`.
3. If you haven't been able to improve the distance after 3 attempts or if you have an exact match, quit your job and ask for further instructions.
4. Update base.c to better match the target assembly code
5. Go to step 1
