Update function $ARGUMENTS to have a name that reflects what it does. Analyse the relevent C or assembly code to determine an appropriate name. MAKE SURE THE NAME ISN'T ALREADY USED.

## Steps to rename a function

1. Search for all references to the old function name in src/ and update them to use the new name.
2. Check if INCLUDE_ASM references the function name and update it.
3. Update symbol_addrs.txt to use the new function name. The old function name will be of the form func_$VRAM_$ROM. symbol_addrs.txt expects an entry along the lines of `new_func_name = $vram;`.

## Verify name

Verify your work by building the project again:

```
make clean && make extract && make
```

If the build fails, you missed a step. If the checksum fails, you missed a step. Go back to the renaming steps and try to identify what went wrong.