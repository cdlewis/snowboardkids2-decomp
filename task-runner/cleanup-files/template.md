# Instructions

Your job is to carefully analyse the structs, variables, functions etc in $INPUT and improve the overall file structure.

Specifically you should:

- Ensure that the declarations in the file are grouped together and in the proper order (macro definitions, structs, function definitions, then global variables/externs)
- Identify opportunities to consolidate struct definitions into a single struct. Functions were decopmiled separately and often given dedicated structs for their input / allocations but this doesn't reflect how the original developers would've written them
- Identify opportunities to shift struct / function definitions to $INPUT's header file. If something is already defined in the header, and $INPUT includes the header (it must), then there's no need to redefine the function in the file.

Run the following command to verify your changes are safe:

```
./tools/build-and-verify.sh
```

Once you have successfully made your changes, commit them with a suitable commit message.

## Build errors and checksum mismatches

The project is complicated and you will often encounter a build error or mismatch error on your initial attempt. Keep trying. Maintaining the 100% match is ESSENTIAL. If you think the build or function was not matching initially, you are wrong. It did match and it's your job to keep it matching.

If you are unable to resolve these issues after 30 attempts, you should give up and revert your changes to ensure the project remains in a healthy state.
