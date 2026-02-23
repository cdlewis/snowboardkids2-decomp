# Cleanup Function

## Instructions

Look at the $INPUT[1] in $INPUT[0]. Think about what its purpose is in the broader project and how it relates to other functions (as a caller / callee).

Think about what the original developers would've written. This means proper structs, proper struct / array accesses, proper variable names, etc.

Make a list of all the problems with the current function. Try various strategies to fix these problems while preserving the 100% match.

It may require several iterations to get this balance right. This is expected.

It may require significant refactoring of the project. This is expected. Do not give up just because the scope of the necessary changes is large.

Once you have cleaned up the function, run the following command to verify you haven't broken the build:

```
./tools/build-and-verify.sh
```

If you have broken the build. Try to fix it while preserving your changes. This may require several iterations but that's fine.

Commit your changes when you're done.

## Unable to clean up the function

Make several attempts to clean up the function without breaking the build. If you're unable to make progress after 30 attempts, revert all of your changes to ensure the build remains clean then exit.

## Safe Refactoring

### Identify Structs and Arrays

There are many kinds of refactoring that is safe to perform without breaking the match.

The most common example is swapping pointer arithmatic for proper struct / array types. For example:

```
*(s16 *)((u8 *)arg0 + 0x28) -= 1;
```

Could easily be rewritten as:

```
arg0->unk28 -= 1
```

We just need to provide a suitable type definition for `arg0`.

Similarly:

```
temp_a0 = (s8 *)(offset + (s32)arg0->entries)
```

Could be re-written as an array access:

```
temp_a0 = arg0->entries[i]
```

(Note that in the above example offset is computed as essentially 0x10\*i indicating size 0x10 structs as the array items.)

### Local variable names

Local variables within a function can be safely renamed without affecting the match. This can be a great way to communicate the intent behind a function.

### Global variable names

This is safe as long as you use the rename-global skill.

### Creating new structs

Refactoring might be difficult due to the file re-using a shared struct from elsewhere in the project. This re-use may be appropriate. But it also might have been done in error and indeed that might be what is preventing you from properly fixing the hacky pointer code.

GameState is a great example of this. Many functions use something called 'GameState' but they may actually be referring to different types.