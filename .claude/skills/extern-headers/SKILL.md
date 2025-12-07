---
name: fix-extern-headers
description: All functions in the project should be referenced through included headers. Not using `extern` function definitions. This skill will clean up extern up `extern` header references. Use this skill if you are asked to clean up `extern` headers.
---

# Fix Extern Headers

Project goals:

- All functions in this project should be defined in a header file.
- The header file should be in the `includes/` folder.
- Header files should mirror the C file implementing those functions. For example, function `A` defined in `cars.c` should have a header definition in `cars.h`.

## Instructions

Run the following command to get a list of warnings to fix:

```
python3 tools/analyse_headers.py src --limit 1
```

Go through each warning and address the underlying issue. This might require you to create a new header file mirroring the C file. It might require you to define a function in the appropriate header file.

Once you have fixed the header issues, run the following command to verify you haven't broken the build:

```
./tools/build-and-verify.sh
```

Command your changes when you're done.

## Resolving type conflicts

In more complex cases, you might need to resolve conflicting definitions of a function. Generally whatever the type of the function is at the point that it is implemented (in the C file) is the best signal on the correct type of the function.

A precise type is preferable to a vague type. For example, we may have a function called:

```
void func_80001640_2240(func_80001688_2288_arg *arg0) {
  arg0->unk89 |= 1;
}
```

If a caller defines an extern version of `func_80001640_2240` that accepts `void*` instead of `func_80001688_2288_arg`. The original definition of the function accepting `func_80001688_2288_arg` should be preferred.

But similarly, a definition such as `func_80001640_2240(SceneModel *arg0);` in an extern may tell us something new about the function. SceneModel looks a lot more appropriate for a type than some function-specific struct.

The golden rule: NEVER generalise types to void* just to make the build work.