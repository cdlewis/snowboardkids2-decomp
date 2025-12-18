# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Repository Overview

This is a matching decompilation project for Snowboard Kids 2 (N64). The goal is to create C code that, when compiled, produces the exact same assembly as the original game ROM.

## Project Structure

- `src` decompiled (or partially decompiled) C code.
- `src/cutscene` C functions and structures related to cutscene code
- `src/levels` C functions and structures related to cutscene code
- `include` headers for decompiled C code
- `asm/nonmatchings` unmatched asm code extracted from the rom. Each file contains a separate function.
- `asm/matchings` decompiled assembly code for already matched C functions. We keep this around as it's sometimes convenient to inspect.
- `lib` library code such as Ultralib which we call and link against
- `assets` binary asset blobs extracted from the rom
- `include` common headers included in all C and/or assembly code

## Tools

- `./tools/build-and-verify.sh` build the project and verify that it matches the target.
- `diff.py` you can view the difference between the compiled and target assembly code of a given function by running `python3 tools/asm-differ/diff.py --no-pager <function name>`
- `./tools/claude --bootstrap <function name>` spin up a decompilation environment for a given function.
- `python3 tools/score_functions.py <directory>` find the easiest function to decompile in a given directory (and its subdirectories).
- `python3 tools/check_pointer_arithmetic.py <file or directory>` detect pointer arithmetic with casts that should be replaced with struct field access. Use `--strict` to fail on violations.
- `python3 tools/project_status.py` shows functions that are currently non-matching. Use this if `./tools/build-and-verify.sh` is failing and you're not sure which function is the problem.

## Tasks

### Decompile assembly to C code

You may be given a function and asked to decompile it to C code.

#### Step 1

First we need to spin up a decomp environment for the function, run:

```
./tools/claude --bootstrap <function name>
```

Move to the directory created by the script. This will be `nonmatchings/<function name>-<number (optional)>`.

Use the tools in this directory to match the function. You may need to make several attempts. Each attempt should be in a new file (base*1.c, base_2.c, ... base_n.c, etc). It's okay to give up if you're unable to match after \_30* attempts.

#### Step 2a (unable to match, log and revert changes)

If you cannot get a perfect match after 30 attempts, add the function name to `tools/difficult_functions` along with the number of attempts and best match percentage (function names should be separated by newlines). This should be in the form `\n<function name> <number of attempts to match> <best match percentage>\n`. By adding the function name to difficult_functions. You must also revert any changes you've made adding the function to the C file or other project files (we do not want to save incomplete matches).

#### Step 2b (successful match, integrate changes into project)

If you are able to match the function, update the C code to use it. The C code will be importing an assembly file, something along the lines of `INCLUDE_ASM/asm/nonmatchings/<function name>`. Replace this with the actual C code.

- Update the rest of the project to fix any build issues.
- If the function is defined in a header file (located in include/), this will also need to be updated. These other usages may teach you about the correct type of your function arguments or return types. DO NOT JUST MAKE EVERYTHING void\*!.
- Make sure to search for any existing function / struct declarations in the project (under src/ and include/). We do not want duplicate or redundent declarations.

Verify that the project still builds successfully by running `./tools/build-and-verify.sh`. If this check fails, the decompilation is NOT complete, even if individual functions appear to match.

- If the checksum fails after your changes, use `python3 tools/asm-differ/diff.py --no-pager <function>` to check ALL functions in the modified file(s). Look for functions that access the same structs you modified. Fix any mismatches before declaring success.
- If you are unable to fix the build issues the match should be marked with all changes reverted and the function name added to tools/difficult_functions in line with the instructions in step 2a.

#### Step 3: Commit

If you are able to get a perfect matching decompilation, commit the change with the message `matched <function name> <attempts>`.

If you were not able to get a perfect matching decompilation, commit your changes to tools/difficult_functions.

Respect any pre-commit hooks that prevent you from committing your change. A failed hook indicates that you have not correctly updates the C code.

You are done. Do not attemp to find the next closest match.

## Validation Checklist

Before declaring any changes to C code complete (including decompiling functions), verify:

- [ ] No pointer arithmetic with manual offset calculations
- [ ] All struct field accesses use `->` or `.` operators
- [ ] No `void*` parameters that should be typed structs
- [ ] Struct sizes match the assembly access patterns
- [ ] `./tools/build-and-verify.sh` succeeds

## Code Quality Standards

### Avoid Pointer Arithmetic

When you see pointer arithmetic patterns like `*(type*)((u8*)ptr + offset)`:

1. **Identify the access pattern:**

   - What offset is being accessed? (e.g., `0xC` means field at offset 12)
   - Is it accessing an array element? (e.g., `arg1 * 36` means 36-byte elements)
   - What field within the element? (e.g., `+ 0xA` means field at offset 10)

2. **Create appropriate structs:**

   - Define the element struct with correct size and field offsets
   - Define the container struct with pointer at correct offset
   - Use meaningful names or `unk[Offset]` naming convention

3. **Verify struct sizes:**

   - Calculate total size to ensure it matches the multiplier in pointer arithmetic
   - Example: `arg1 * 36` means struct must be exactly 36 (0x24) bytes

### Struct Modification and Extension

When modifying struct definitions:

- Search the entire codebase for other references to the same struct
- Check if other functions access fields at nearby offsets
- Verify ALL affected functions still match after struct changes
- Example: If you add a field at offset 0x14, search for all functions accessing that struct and verify they still compile to the correct offsets

### Avoid Redundent Declarations

After adding your decompiled function, check for any redundant extern declarations:

1. **Search for existing declarations**: For each extern function you used, search the codebase to see if it's already declared in a header file:

   - Use `grep -r "void functionName" include/` to search headers
   - Use `grep -r "void functionName" src/*.h` to search source headers

2. **Remove redundant externs**: If a function is already declared in an included header file, remove your extern declaration to avoid duplication

3. **Verify the build still works** after removing redundant externs

Example: If you added `extern void setCallback(void *);` but `task_scheduler.h` (which is already included) declares it, remove your extern declaration.

## Decompilation tips

### Assets

Typically small 5-6 length symbols (e.g. D_4237C0) are asset addresses. Another strong hint that they are assets if if they are passed to `dmaRequestAndUpdateStateWithSize`.

Use `USE_ASSET(symbol)` to load symbols, for example D_4237C0 and dmaRequestAndUpdateStateWithSize would become:

```
USE_ASSET(_4237C0);

dmaRequestAndUpdateStateWithSize(_4237C0_ROM_START, _4237C0_ROM_END);
```

Failure to handle assets properly will almost certainly guarantee a mismatch.
