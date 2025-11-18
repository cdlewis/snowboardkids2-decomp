# CLAUDE.md

## Your Job

You are decompiling Nintendo 64 assembly code from Snowboard Kids 2. Your goal is to generate C code that when compiled 100% matches the given assembly code.

The compiler is GCC 2.7.2. The following compiler flags are being used: -O2 -mips3. We follow the C89 standard. Keep in mind that variable declarations are generally limited to the beginning of blocks or functions.

Repeat the following steps:

1. Run `./build.sh base.c` to build base.c and get an object dump of the compiled code. You will also get a score, with a score of 100% indicating a perfect match.
2. Look for an area where the control flow and instructions do not match. Consider what the original developers probably intended to write given the function's broader purpose. Print out an explanation for why they don't match.
3. Test your change by creating a new file (base_n.c where `n` is your attempt number).
4. Run `./build.sh base_n.c` (where `n` is your attempt number).
5. If your possible solution did not improve the match percentage, use tools to analyse what went wrong and summarise your theory. Then apply this theory to improving the match in your next attempt.

## Tools

- `./build.sh <base_n>.c`: this script takes a .c file, compiles it, and diffs the resulting object code against the target assembly. Use build.sh to build your C code and check how close you are to a match.
- `./objdump.py <base_n>.o`: dumps the assembly code for the specified file. Use this to dump object the target or base_x files for further analysis.
- `./diff.sh <base_n>.o`: diffs the assembly code for the file against the assembly code for the target file. Use this to identify specific differences between the base_n file and the target file.
- `./map_asm_to_c.py <base_n.o> <line number>`: Once you've identified the problematic assembly code, use this to map it back to the relevant C code.

## Coding Guidelines

- When analysing type errors or conflicts, look to the code in src/ and include/ for guidance. These types work and are used elsewhere in the project. Types in the function you're trying to match could be wrong or misleading. Similarly, any extensions or changes to existing types need to be compatible with the rest of the codebase so exercise caution when making such changes.
- Always use array and struct access. Do not introduce manual pointer arithmatic.
- If you need to cast between pointer types, STOP - you're solving the wrong problem. Fix the struct layout instead.
- Look at what memory offset the assembly is accessing and map it directly to struct fields - don't introduce intermediate types.
- Where appropriate, generate structs to match the expected input arguments, return values, etc.
- Before adding a new type definition, search in the codebase if this struct already exists and reuse them whenever possible.
- Favour `for` loops over `do` or `while` loops.
- VARIABLE DECLARATIONS MUST APPEAR AT THE START OF THE FUNCTION.
- Never add comments to code.
- The only permitted #include is common.h
- Use temporary variables instead of complex nested expressions

Never use pointer casts or manual offset arithmetic to access struct members. When decompiling a function that accesses a struct field:

1. **First**, identify the exact offset being accessed in the assembly
2. **Second**, check if a field exists at that offset in the struct definition
3. **If the field doesn't exist**:
  - Update the struct definition to add the proper field at that offset
  - Adjust padding arrays as needed to maintain correct offsets
  - Verify all offset comments in the struct remain accurate
4. **Then** write the C code using the proper field name

A literal decompilation of the code often produces strange artefacts. Account for and avoid these common pitfalls:

- Control flow often becomes overly complicated:
  - `if { do { } while () }` should just be `while {}`
  - `i = 0; if { do { i++ } while () }` should just be `for {}`
  - `goto X` is likely just a loop or conditional
- Arithmatic is often converted to shifts:
  - `x >> 2` should just be `x / 4`
  - `x << 2` should just be `x * 4`
- Explicit returns in assembly might actually be fall-through returns:
  ```c
  if (condition) {
      // main work
  } else {
      // alternative path
      return X;  // explicit return only here
  }
  return Y;  // fall-through from if-branch
  ```

## Decompiling Tips

- Think about what the function is _doing_ within the game. What is its purpose? Ensure code is logically structured to fulfill that purpose. This is the surest strategy for finding a 100% match.
- Focus on control flow differences over register or stack differences. Register and stack are easy to fix later.
