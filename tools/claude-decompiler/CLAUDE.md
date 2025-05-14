# CLAUDE.md

## Your Job

You are decompiling Nintendo 64 assembly code from Snowboard Kids 2. Your goal is to generate C code that when compiled 100% matches the given assembly code.

The compiler is GCC 2.7.2. The following compiler flags are being used: -O2 -mips3. We follow the C89 standard. Keep in mind that variable declarations are generally limited to the beginning of blocks or functions.

Repeat the following steps:

1. Run `./claude-compile-and-analyse.sh base.c` to build base.c and get an object dump of the compiled code. You will also get a score, with a score of 100% indicating a perfect match.
2. Look for an area where the control flow and instructions do not match. Think about why they don't match. Make a list of possible solutions, consider what the original developers probably intended to write given the function's broader purpose.
3. Test your change by creating a new file (base_n.c where `n` is your attempt number).
4. Run `./claude-compile-and-analyse.sh base_n.c` (where `n` is your attempt number).
5. If your possible solution did not improve the match rate. Move on to the next item on your list. Keep trying different approaches until you're able to fully match the target C code.

## Project Structure

* You might also find it useful to consider the attempts in output-{diff}-x folders. These are messy / brute force but you might get some ideas from them.

## Coding Guidelines

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

## Effective Instructions for LLMs on Assembly Matching Tasks

When guiding an LLM to match compiled C code with target assembly:

1. Start with thorough analysis:
  - Compare the original compiled output with target assembly line by line
  - Identify key differences in register allocation, control flow, and
  instruction sequences

2. Focus on variable declarations and assignments:
  - Place all declarations at the beginning of functions (C89 standard)
  - Use variable assignments strategically to influence register allocation
  - Try combined assignments (e.g., var1 = var2 = value) to influence register
  usage patterns

3. Leverage explicit register hints:
  - Create temporary variables that are used only briefly to direct register
  allocation
  - Try assigning the same value to multiple variables with different names

4. Control block structure carefully:
  - Experiment with nesting blocks versus flattening them
  - Add/remove explicit scopes with braces {}
  - Use do-while loops instead of for loops in some cases

5. Try variable type variations:
  - Switch between s32 and u32 for comparisons and assignments
  - Use explicit casts to control instruction generation

6. Look for patterns in success cases:
  - When a change improves the score, amplify that pattern
  - Try to identify what specific code constructs led to matching instructions

7. Use methodical iterations:
  - Make one change at a time to isolate its effect
  - Keep track of each change and its resulting score
  - Combine successful changes from multiple attempts

8. Examine compiler quirks:
  - Older compilers (like GCC 2.7.2) have specific optimization patterns
  - Study how different C constructs translate to assembly with these older
  compilers

9. Prioritize areas with largest differences:
  - Focus first on sections with many mismatched instructions
  - Don't get stuck optimizing fully matching sections

10. Leverage existing success examples:
  - Use previously matched code patterns as templates
  - Directly copy working structures from successful attempts