---
name: decomp-permuter
description: Decomp-Permuter is a tool that automatically permutes C files to better match a target binary. Use this skill when you are decompiling a function and it is almost matching except for some register differences (i.e. 95%+). Or if you are otherwise unable to make progress on a function decompilation. Do not use it when there are control flow or functional differences; it's often easy to resolve those by hand, and neither the scorer nor the randomizer tends to play well with them.
---

# Permute a Function

## Instructions

These instructions assume you are in the root directory of the Github repo.

### Step 1: Run the permuter

Run the permuter with the `--source-file` (or `-s`) flag pointing to your best matching attempt:

```bash
timeout 300s ./tools/permuter --source-file nonmatchings/<function-name>/base_N.c <function name>
```

This will automatically create a permuter environment and run the permuter for 300 seconds. You can tweak this number to an appropriate time. If you fail to use the `timeout` command the permuter will run forever.

**Requirements for source file:**
- Must compile successfully
- Should already be 95%+ match (permuter works on register allocation, not control flow)
- Only one function body should be implemented (extern declarations for other functions are fine)
- Include all necessary headers at the top

The permuter will report match improvements. These will be located in `nonmatchings/function-name-<optional number>/output-<score>-<optional number>`. The full C code will be in `source.c`. A `diff.txt` file is also available with the changes the permuter made.

## Iterating on a Permuted Function

Look at the changes outputted by the permuter and use them to improve your own C code. Do not attempt to iterate directly on the base.c file. Create a new version of the function (e.g., base_N+1.c) and re-run the permuter with `--source-file` pointing to the new file.

### Beware of permuter artefacts and noise

The permuter is dangerous. It can lead you to local optima. Only incorporate decomp-permuter changes that make logical sense within the context of the function you are matching.

For example, decomp-permuter may tell you to move or delete a line or statement that you know is necessary for the function to make logical sense. This may temporarily bump the match percentage but cannot ever be a path to 100% match.