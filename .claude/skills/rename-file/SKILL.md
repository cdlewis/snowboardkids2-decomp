---
name: rename-a-c-file-or-header-file
description: Renaming a C file or associated header (.h) file  requires additional steps in decompilation projects or the build will fail. Use this skill when you want to rename a C file and/or a header file.
---

# Rename a C file or a Header File

## Instructions

C files and their associated header files are mapped to specific ROM addresses via the Splat project configuration in `snowboardkids2.yaml`.

A file will typically have a code segment, such as:

```
- [0x4A40, c]
```

It may also have data and/or rodata segments. Data segment(s) are mapped to the code via the third argument. The name specified in the third argument must match the name of the code segment. In this case the code segment has no name so it defaults to using its ROM address as a name (4A40). For example:

```
- [0x8CBA0, .data, 4A40]
```

To rename 4A40.c to credits.c, we would need to do the following:

```
- [0x4A40, c, credits]

...

- [0x8CBA0, .data, credits]
```

Naturally, the reverse is also true. To rename a segment that has already been named, you must change each instance of that name to the new name. In this case you would need to change 'credits' to your desired new file name.

## Custom Make Rules

In rare cases a C file will have a custom compiler flags. Be sure to check the project `Makefile` and update the C file's name there also.
