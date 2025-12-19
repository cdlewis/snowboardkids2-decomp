---
name: fix-extern-headers
description: All functions in the project should be referenced through included headers. Not using `extern` function definitions. This skill will consolidate function definitions into header files and resolve any type conflicts that arise as a result. Use this skill if you are asked to clean up `extern` headers.
---

# Fix Extern Headers

Conflicting function signatures are a common decompilation artifact as functions are decompiled individually, their callees are often defined as externs in the same file. But as we learn more about their callers and callees need to further refine our function signatures. Resolving these conflicts gives us new information about the codebase and helps push the progress further.

At a minimum we should maintain the following standards:

- All functions in this project should be defined in a header file.
- The header file should be in the `includes/` folder.
- Header files should mirror the C file implementing those functions. For example, function `A` defined in `cars.c` should have a header definition in `cars.h`.
- There should be no extern functions as this indicates a missing header file or function defined in an existing header file.

## Instructions

Look at the source file and line and determine which one of the header rules is being violated:

Attempt to address the underlying issue. This may be as simple as moving a function definition. But watch out! Consolidating header definitions can reveal inconsistencies between different C files.

We can safely assume that the original codebase did not have these inconsistencies and what we are looking at are side-effects of the decompilation effort.

You will need to determine what the 'true' function definition is, one which can satisfy all the different callers of that functions from different parts of the project.

Look at how the function is being called and what it is calling. Think about what the function is intended to do in the project and what the original developers would've written.

Make changes to this function and other functions/structs as necessary. You may need to go through several attempts to arrive at a set of types that satisfies all users and can be successfully built with built-and-verify.sh (i.e. matches 100%).

Once you have fixed the header issue, run the following command to verify you haven't broken the build:

```
./tools/build-and-verify.sh
```

Commit your changes when you're done.

It is expected that adding a function to a header could break the build. Your job is to resolve this issue. Think carefully and do not give up just because your initial refactor breaks things.

This line of thinking is completely wrong and should be ignored:

> sometimes you need "wrong" signatures to get the compiler to generate matching assembly.

That's just lazy. There is a right signature and your job is to find it.

With that said, if it's VERY DIFFICULT to fix and you're unable to make progress after 30 attempts, revert all of your changes to ensure the build remains clean then exit.

## Header Rules

### MISSING_HEADER_DECLARATION

What it means: A function is defined in a C file and used via extern declarations in other files, but it has no declaration in any header file.

Why it's a problem: Functions that are used across multiple files should be declared in a header file, not via ad-hoc extern statements scattered throughout the codebase.

How to fix: Add a proper function declaration to the suggested header file (typically the .h file corresponding to the .c file where the function is defined).

### MISSING_INCLUDE

What it means: A function is properly declared in a header file, but a C file that uses the function has an extern declaration instead of including the header.

Why it's a problem: The file is manually declaring the function with extern instead of including the header that already declares it, creating redundancy and potential for inconsistency.

How to fix: Remove the extern declaration and add an #include directive for the appropriate header file.

### DECLARATION_SHOULD_BE_IN_HEADER

What it means: A function declaration (forward declaration, ending with ;) exists in a C file, but the actual function definition is in a different C file.

Why it's a problem: Forward declarations that are needed across files belong in header files, not in C implementation files.

How to fix: Move the function declaration from the C file to an appropriate header file that both C files can include.

## Resolving type conflicts

In more complex cases, you might need to resolve conflicting definitions of a function. Generally whatever the type of the function is at the point that it is implemented (in the C file) is the best signal on the correct type of the function.

## Common Challenges

### Choosing a type

The golden rule: NEVER generalise types to void\* just to make the build work.

A precise type is preferable to a vague type. For example, we may have a function called:

```
void func_80001640_2240(func_80001688_2288_arg *arg0) {
  arg0->unk89 |= 1;
}
```

If a caller defines an extern version of `func_80001640_2240` that accepts `void*` instead of `func_80001688_2288_arg`. The original definition of the function accepting `func_80001688_2288_arg` should be preferred.

But similarly, a definition such as `func_80001640_2240(SceneModel *arg0);` in an extern may tell us something new about the function. SceneModel looks a lot more appropriate for a type than some function-specific struct.

### Circular dependencies

There are several strategies you can use to resolve circular dependencies:

- If the dependency is defined in a C file, try moving it to the appropriate header file
- If the dependency is between two header files, move the struct to a new, third, header file.

### Sign Extension

The compiler might be doing zero instruction sign extension at the call site.

If you can't get a match on a type of one size (e.g. u16/s16), it's worth trying u32/s32.

## Style guide

- Use `#pragma once` rather than `#ifndef` to guard against multiple imports.
