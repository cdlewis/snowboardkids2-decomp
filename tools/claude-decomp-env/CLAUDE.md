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
- `gfxdis.f3dex2 -d 0xXXXXXXXXXXXXXXXX`: disassemble an f3dex2 microcode instruction into an approximately correct `gbi.h` macro. For example `gfxdis.f3dex2 -d DA38000300000000` produces `gsSPMatrix(0x00000000, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW)`.

## Coding Guidelines

- When analysing type errors or conflicts, look to the code in src/ and include/ for guidance. These types work and are used elsewhere in the project. Types in the function you're trying to match could be wrong or misleading. Similarly, any extensions or changes to existing types need to be compatible with the rest of the codebase so exercise caution when making such changes.
- Always use array and struct access. Do not introduce manual pointer arithmatic.
- If you need to cast between pointer types, STOP - you're solving the wrong problem. Fix the struct layout instead.
- If you need to cast a pointer to an integer type (e.g., `(s32)ptr`), STOP - check if the struct either has a suitable field (or array of fields) that you can use instead.
- Look at what memory offset the assembly is accessing and map it directly to struct fields - don't introduce intermediate types.
- Where appropriate, generate structs to match the expected input arguments, return values, etc.
- Before adding a new type definition, search in the codebase if this struct already exists and reuse them whenever possible.
- Favour `for` loops over `do` or `while` loops.
- VARIABLE DECLARATIONS MUST APPEAR AT THE START OF THE FUNCTION.
- Never add comments to code.
- Use temporary variables instead of complex nested expressions

🚨 CRITICAL: NEVER USE POINTER CASTS OR OFFSET ARITHMETIC 🚨
THIS IS THE #1 RULE. VIOLATING THIS RULE MEANS AUTOMATIC FAILURE. Examples of this include:

- `return *(u8 *)(arg0 + 0xC1);`
- `*(s32*)((u8*)arg0 + 0x34) = value;`
- `*(u8*)(arg0 + 0xC1);`
- `*((s16*)ptr + 2);`

When you see assembly accessing an offset:

- STOP - Do NOT write pointer arithmetic
- FIND or CREATE a struct with a field at that exact offset
- USE the struct field directly
  Instead, when decompiling a function that accesses a struct field:

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

### Large Functions

Do not be intimidated by large functions (hundreds of lines of assembly)! These can often be quite straightforward, you just need to approach the problem methodically.

- Get the function to compile, this might require filling in missing typing information, undefined functions, etc.
- Focus on getting the control flow correct. Look at the code in `base.c` for clues but also look at the assembly code in `target.s`. Control flows generated by m2c can be convoluted and misleading. As a super smart LLM you'll be able to get a better sense of the true control flow by comparing `base.c` to your understanding of the control flow in `target.s`
  - Create a checklist of problems with the control flow.
  - Work through your checklist item-by-item focusing on logical correctness and match percentage.
- Large structs are easy. Often there will be significant gaps between fields so size isn't that important anyway. Just try to get those field accesses correct.
- Look for clues in how the function is called and/or how it calls other functions.

### Functions using gRegionAllocPtr

gRegionAllocPtr is indicative of a dynamically constructed Fast3DEX2 (also known as f3dex2) display list commands. Fast3DEX2 display list commands are microcode that are passed to the RSP for rendering. They are all 8 bytes long.

Special care needs to be taken with functions constructing display lists as the original developer likely used specific macros from `lib/f3dex2/PR/gbi.h`. These macro calls need to be replicated in order for the function to match.

The first step is to identify the commands in question, for example a naive decompilation might produce:

```
    temp_v1 = gRegionAllocPtr;
    temp_a0 = D_800A8B14_9FE84;
    temp_v1->words.w0 = 0xDA380003;
    temp_a1 = (s32)arg0->unk30;
    temp_v1->words.w1 = temp_a1;
```

But we can see here that there's really just one f3dex command. It starts with 0xDA380003 and uses temp_a1 as an argument. We can use `gfxdis.f3dex2` to help us identify what it is:

```
gfxdis.f3dex2 -d DA38000300000000
```

Which gives us:

```
gsSPMatrix(0x00000000, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

```

This is close to what we want. But it needs a few improvements:

1. We need the variant of this macro that also accepts a display list pointer: `gSPMatrix`.
2. We need to replace 0x00000000 with our actual variable: arg0->unk30.

This ultimately will give us:

```
gSPMatrix(gRegionAllocPtr++, (s32)arg0->unk30, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
```

Note the inline increment of gRegionAllocPtr, this is very common in commands dealing with f3dex2 microcode.

The mapping may not always be this clear, however. You can consult `f3dex-reference.md` for a full list of f3dex commands and gbi.h for suitable macros to use.
