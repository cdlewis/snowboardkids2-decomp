# CLAUDE.md

## Your Job

You are decompiling Nintendo 64 assembly code from Snowboard Kids 2. Your goal is to generate C code for `$functionName` that, when compiled, 100% matches the given assembly code.

The compiler is GCC 2.7.2 with flags `-O2 -mips3`. We follow the C89 standard.

### Laying the Foundation

Before doing anything else, create a subagent to gather context about the $functionName and ensure `base.c` is compilable. 

MAKE THE MINIMAL SET OF CHANGES NECESSARY TO COMPILE `base.c`. This is important for getting an accurate baseline match percentage.

Specifically, the subagent should:

<subagent-instructions>
1. Explore how $functionName is used in the codebase. Look at ../../src, ../../include as well as the unmatched code (../../asm/nonmatching). Write a summary of what the $functionName is and how it's used to `LEARNINGS.md`.
2. Ensure that base.c compiles successfully. Ensure that any missing types are present. base.c should only depend on "common.h". Any other missing types should be provided inline rather than via #include statements. Do not stop until base.c can be successfuly built. Report status and a brief summary of your findings upon completion.
3. Report back on its progress and findings
</subagent-instructions>

### Build Loop

After base.c builds successfully, repeat the following steps:

1. Run `./build.sh base.c` to build and get a diff against the target assembly. A score of 100% indicates a perfect match.
2. Come up with a plan to improve the match. Look for areas where the control flow and instructions do not match. Consider what the original developers intended to write given the function's broader purpose.
3. Create a new file (`base_n.c` where `n` is your attempt number) with changes you expect to improve the match. Start small and work incrementally — if you test multiple changes at once they may interact poorly.
4. Return to step 2 and continue working to improve the match percentage. Record key learnings in LEARNINGS.md. Keep going until you reach a 100% match or are unable to make progress (e.g. 10 attempts without any improvement to the match percentage).

## Tools

- `./build.sh <file>.c` — compile a .c file and diff the resulting object code against the target assembly.
- `./objdump.py <file>.o` — dump the assembly code for the specified object file.
- `./diff.sh <file>.o` — diff the assembly of the given object file against the target.
- `./map_asm_to_c.py <file>.o <line>` — map an assembly line back to the relevant C code.
- `gfxdis.f3dex2 -d 0xXXXXXXXXXXXXXXXX` — disassemble an f3dex2 microcode instruction into an approximate `gbi.h` macro. For example, `gfxdis.f3dex2 -d DA38000300000000` produces `gsSPMatrix(0x00000000, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW)`.
- `../../tools/s2dex_dis.py 0x0200000080123456` - disassemble s2dex microcode instruction into an approximate gs2dex.h macro.

## Coding Guidelines

### Types and Structs
- Look to existing code in `src/` and `include/` for guidance on types. Types in the function you're matching could be wrong or misleading.
- Before adding a new type definition, search the codebase first and reuse existing structs whenever possible.
- Where appropriate, generate structs to match expected input arguments, return values, etc.
- Exercise caution when extending or changing existing types — they must remain compatible with the rest of the codebase.

### Struct Field Access

Always prefer struct field or array accesses over pointer arithmatic.

Common mistakes:
- `return *(u8 *)(arg0 + 0xC1);`
- `*(s32*)((u8*)arg0 + 0x34) = value;`
- `*((s16*)ptr + 2);`

Instead, check if a field exists at that offset in the struct definition that you could use. If the field doesn't exist, update the struct definition: add the proper field, adjust padding arrays, and verify all `/* 0xNN */` offset comments remain accurate

### Style
- Use `for` loops over `do` or `while` loops.
- Use temporary variables to break up complex nested expressions, but don't introduce unnecessary re-assignments of the same value across multiple variables.
- Variable declarations must appear at the start of the function.
- Never add comments to code.

## Decompilation Strategy

Learnings from past decompilations can be found at `DECOMPILATION_LEARNINGS.md`.

### General Approach
- Think about what the function is *doing* within the game. What is its purpose? Structure the code to fulfill that purpose — this is the surest path to a 100% match.
- Focus on control flow differences over register or stack differences. Register and stack issues are easy to fix later.
- Look for clues in how the function is called and how it calls other functions.

### Cleaning Up Decompilation Artefacts
Literal decompilation often produces artefacts. Watch for these common patterns:

<artefact name="for-loops">
m2c struggles with `for` loops. The compiler often pulls out the condition so it can bail early if it's never met. Note that the comparison operator may also change (e.g. `<` becomes `<=`).

This code:
```c
for (i = 0; i < 10; i++) {
    // stuff
}
```

Often decompiles as:
```c
i = 0;
if (i <= 10) {
    do {
        // stuff
        i++;
    } while (i <= 10);
}
```
</artefact>

<artefact name="gotos">
Developers rarely, if ever, write GOTO statements but they show up often in decompilation output because many different kinds of control flow are represented as branches and jumps in assembly. Assume that GOTOs are just a decompilation artefact.
</artefact>

<artefact name="duplicated-variables">
It's far more likely that a single variable is being reused rather than many temporary variables. Literal decompilation and permuting often produce unnecessary re-assignments that hurt the match rate.
</artefact>

<artefact name="shifts-instead-of-arithmetic">
Arithmetic is often converted to shifts:
- `x >> 2` → `x / 4`
- `x << 2` → `x * 4`
</artefact>

<artefact name="false-returns">
Explicit returns in assembly might actually be fall-through returns:
```c
if (condition) {
    // main work
} else {
    // alternative path
    return X;
}
return Y;  // fall-through from if-branch
```
</artefact>

<artefact name="gcc-272-codegen">
GCC 2.7.2 has specific codegen patterns that show up in the target assembly. Recognise these so you don't fight the compiler:

- **Signed division:** dividing a signed integer by a power of two produces a shift-and-bias pattern (shift right to extract the sign bit, add it, then arithmetic shift). Write the natural division (`x / 4`) and let the compiler emit this pattern — don't try to match the shifts manually.
- **Branchless operations:** the compiler sometimes generates branchless code (e.g. `slti` + `addu`) for simple conditionals. A ternary or `if/else` in C will often produce the same output.
- **`do{}while(0)` blocks:** wrapping code in `do{}while(0)` can influence register allocation and instruction scheduling without changing semantics. Use this as a last resort when register assignment won't cooperate.
- **Dead stores:** GCC 2.7.2 sometimes expects a variable to be written before a branch even if the value is overwritten later. Adding an initialisation (e.g. `result = 0`) before a conditional can fix mismatches.
</artefact>

### Large Functions
Don't be intimidated — these are often straightforward if approached methodically:

1. Get the function to compile first, filling in missing types, undefined functions, etc.
2. Focus on control flow. Compare `base.c` against `target.s` — m2c's generated control flow can be convoluted and misleading. Make a checklist of problems and work through them one by one.
3. Large structs are easy. Often there are significant gaps between fields, so just focus on getting the field accesses correct.

### Functions Using gRegionAllocPtr
Snowboard Kids 2 uses two sets of microcode: f3dex2 and s2dex. Use the `decompile-microcode` skill if you suspect a function is using RDP display lists.
