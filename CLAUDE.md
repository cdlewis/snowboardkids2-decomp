# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Repository Overview

This is a matching decompilation project for Snowboard Kids 2 (N64). The goal is to create C code that, when compiled, produces the exact same assembly as the original game ROM.

## Project Structure

- `src` decompiled (or partially decompiled) C code
- `include` headers for decompiled C code
- `asm` unmatched asm code extracted from the rom. Each file contains a separate function.
- `lib` library code such as Ultralib which we call and link against
- `assets` binary asset blobs extracted from the rom
- `include` common headers included in all C and/or assembly code

## Tools

- `diff.py` you can view the difference between the compiled and target assembly code of a given function by running `python3 tools/asm-differ/diff.py --no-pager <function name>`
- `./tools/claude <function name>` spin up a decompilation environment for a given function.
- `python3 tools/score_functions.py <directory>` find the easiest function to decompile in a given directory (and its subdirectories).

## Tasks

### Decompile directory to C code

You may be given a directory containing assembly files either in its own directory or its subdirectories.

1. Use the score_functions.py tool to find the easiest function. Start with this one.
2. Follow the instructions in the `Decompile assembly to C code` of this document.
3. If you are able to get a perfect matching decompilation, commit the change with the message `matched <function name>` and return to step (1). If you cannot get a perfect match after several attempts, add the function name to `tools/difficult_functions` (function names should be separated by newlines) and return to step (1). By adding the function name to difficult_functions it will no longer be considered by the score_functions.py tool. You should also revert any changes you've made adding the function to the C file. We do not want to save incomplete matches.

### Decompile assembly to C code

You may be given a function and asked to decompile it to C code.

First we need to spin up a decomp environment for the function, run:

```
./tools/claude <function name>
```

Move to the directory created by the script. This will be `nonmatchings/<function name>-<number (optional)>`.

Use the tools in this directory to match the function.

Once you have a matching function, update the C code to use it. The C code will be importing an assembly file, something along the lines of `INCLUDE_ASM/asm/nonmatchings/<function name>`. Replace this with the actual C code.

If the function is defined in a header file (located in include/), this will also need to be updated.

Update the rest of the project to fix any build issues.

After adding your decompiled function, check for any redundant extern declarations:

1. **Search for existing declarations**: For each extern function you used, search the codebase to see if it's already declared in a header file:

   - Use `grep -r "void functionName" include/` to search headers
   - Use `grep -r "void functionName" src/*.h` to search source headers

2. **Remove redundant externs**: If a function is already declared in an included header file, remove your extern declaration to avoid duplication

3. **Verify the build still works** after removing redundant externs

Example: If you added `extern void setCallback(void *);` but `task_scheduler.h` (which is already included) declares it, remove your extern declaration.

**IMPORTANT - Verification Requirements:**

1. **NEVER declare success based only on local environment matching.** Matching in the nonmatchings directory does NOT guarantee the full project matches.

2. **ALWAYS verify the complete build** by running:

   ```
   make clean && make extract && make -j1
   shasum -c snowboardkids2.sha1
   ```

3. **SUCCESS CRITERIA**: The ONLY acceptable success condition is:

   ```
   build/snowboardkids2.z64: OK
   ```

   If this check fails, the decompilation is NOT complete, even if individual functions appear to match.

4. **When modifying struct definitions:**

   - Search the entire codebase for other references to the same struct
   - Check if other functions access fields at nearby offsets
   - Verify ALL affected functions still match after struct changes
   - Example: If you add a field at offset 0x14, search for all functions accessing that struct and verify they still compile to the correct offsets

5. **If the checksum fails after your changes:**
   - Use `python3 tools/asm-differ/diff.py --no-pager <function>` to check ALL functions in the modified file(s)
   - Look for functions that access the same structs you modified
   - Fix any mismatches before declaring success
