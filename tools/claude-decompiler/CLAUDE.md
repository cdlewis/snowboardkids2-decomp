# CLAUDE.md

## Your Job

You are decompiling Nintendo 64 assembly code from Snowboard Kids 2. Your goal is to generate C code that when compiled 100% matches the given assembly code. Do not worry about stack addresses being exactly correct. Or stack size being exactly correct. Or register usage being exactly correct.

The compiler is GCC 2.7.2. The following compiler flags are being used: -O2 -mips3. We follow the C89 standard. Keep in mind that variable declarations are generally limited to the beginning of blocks or functions.

Relevent files:
* `base.c` holds our best guess at the correct C code. Only update `base.c`.
* `base_original.c` is a copy of base.c that you can reference

In terms of code structure:
* Never add comments to code.
* Where appropriate, generate structs to match the expected input arguments.
* Never use #include statements.
* Never add comments.

You can view the target code by running `./claude-target.sh`.

Repeat the following steps until you get an exact match or are no longer able to make progress:

1. Run `./claude-compile-and-analyse.sh` to build base.c and get an object dump of the compiled code. You will also get a score, a score of 0 is a perfect score indicating a 100% match.
2. Analyze the difference between source and target files. Identify one difference and focus on correcting it. 
3. Update base.c to better match the target assembly code. Focus on small changes to one area at a time that incrementally improve the match. Focus on code structure rather over getting the registers exactly right.
5. Go to step 1

## Tools

* `./claude-target.sh`: view the target code.
* `./claude-compile-and-analyse.sh`: build base.c and get an object dump of the compiled code.