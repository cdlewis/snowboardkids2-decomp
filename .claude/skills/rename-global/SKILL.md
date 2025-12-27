---
name: rename-global-variable-or-function
description: Renaming a global variable or function requires additional steps in decompilation projects or the build will fail. Use this skill when you want to rename a global variable or function.
---

# Rename a global variable or function

## Instructions

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
