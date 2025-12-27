# Instructions

Your job is to carefully analyse $ARGUMENT, think about what the original developers intended, understand its purpose and update it to better reflect this purpose.

Specifically you should:

- Rename the function to have a more suitable function name if the current name does not adequately reflect its purpose;
- Update global variables/symbols used by the function if they have overly generic names or names that are at odds with their purpose;
- Update local variables within the function if they have overly generic names or names that do not reflect their purpose;
- Clean up function using better spacing, proper struct/array accesses/types, etc as long as doing so does not break the 100% match we require all functions to maintain.
- Clean up decompilation artefacts where you are able to do so without breaking the 100% match that we require all functions to maintain.

Run the following command to verify your changes are safe:

```
./tools/build-and-verify.sh
```

Once you have successfully made your changes, commit them with a suitable commit message.

## Build errors and checksum mismatches

The project is complicated and you will often encounter a build error or mismatch error on your initial attempt. Keep trying. Maintaining the 100% match is ESSENTIAL. If you think the build or function was not matching initially, you are wrong. It did match and it's your job to keep it matching.

If you are unable to resolve these issues after 30 attempts, you should give up and revert your changes to ensure the project remains in a healthy state.

## When to leave names alone

If suitable name thing already exists -- or the name essentially conveys the purpose of the function/symbol -- you should leave them be.

Avoid renaming functions that correspond to standard library functions. If a function is from the standard library then it is extremely valuable to maintain that naming convention.

# How to rename a global variable or function

Global variables and function names are mapped to specific addresses in memory through the symbol_addrs.txt file at the root of the project. Each symbol should only appear at most once. If a symbol is not present it will be given a default name (usually starting with `func_` or `D_`).

So at a minimum this file will need to be updated when attempting to rename a variable. However additional steps may also be needed.

### Global Variables

Global variables will have a default name format that looks like: `D_<memory address>_<rom address>`. For example `D_800B11A0_1DB740`. Both the memory address and rom adress should be included as annotations when renaming a symbol. This is because different symbols may share a memory address but be located in different overlays. To rename `D_800B11A0_1DB740`, for example, to `MyNewGlobal` we would do:

```
D_800B11A0_1DB740 = 0x800B11A0; // rom:0x1DB740
```

Note the structure of `<new name> = <memory address>; // rom:<rom address>.

You may see additional annotations for size and type. These should be preserved if they exist but you should not need to use them.

### Functions

A similar process is followed for functions. A function will by default have a name structure of `func_<memory address>_<rom address>`. For example: `func_800B00C0_9FF70`. To rename this function to `MyFunction` we would do:

```
MyFunction = 0x800B00C0; // rom:0x9FF70
```

## Validation

To validate your name change, rebuild the project:

```
./tools/build-and-verify.sh
```

# Use thinking
