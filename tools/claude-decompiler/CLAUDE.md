# CLAUDE.md

## Your Job

You are decompiling Nintendo 64 assembly code from Snowboard Kids 2. Your goal is to generate C code that when compiled 100% matches the given assembly code.

The compiler is GCC 2.7.2. The following compiler flags are being used: -O2 -mips3. We follow the C89 standard. Keep in mind that variable declarations are generally limited to the beginning of blocks or functions.

Repeat the following steps:

1. Run `./claude-compile-and-analyse.sh base.c` to build base.c and get an object dump of the compiled code. You will also get a score, with a score of 100% indicating a perfect match.
2. Look for an area where the control flow and instructions do not match. Consider what the original developers probably intended to write given the function's broader purpose. Print out an explanation for why they don't match.
3. Test your change by creating a new file (base_n.c where `n` is your attempt number).
4. Run `./claude-compile-and-analyse.sh base_n.c` (where `n` is your attempt number).
5. If your possible solution did not improve the match percentage, print why you think it made it worse. Apply this theory to improving the match in your next attempt.

## Project Structure

* You might also find it useful to consider the attempts in output-{diff}-x folders. These are messy / brute force but you might get some ideas from them.

## Coding Guidelines

* Favour struct or array accesses. Avoid pointer arithamtic whenever possible.
* Favour `for` loops over `do` or `while` loops.
* VARIABLE DECLARATIONS MUST APPEAR AT THE START OF THE FUNCTION.
* Never add comments to code.
* Where appropriate, generate structs to match the expected input arguments.
* Never use #include statements.
* Keep variable declarations simple and at the top of functions
* Avoid complex initializers and inline variable declarations
* Use simple arithmetic operations instead of compound ones
* Be explicit with type casts when working with different sized data
* Prefer direct variable assignments over compound expressions
* Use temporary variables instead of complex nested expressions
* Avoid relying on operator precedence - use parentheses to be explicit
