---
name: match-data-file
description: Match data file describes how to add dat asegments to a C file. Use the match data file skill when you need to match a data file.
---

# Match a Data File

## Update snowboardkids2.yaml

Code, data, rodata and bss are all defined as separate segments in `snowboardkids2.yaml`. The first step of matching a new data segment is to update the appropriate data segment entry to mark is as defined in the existing C file (code segment).

For example if you are asked to match the data segment for code segment `0x8A40`:

```
- [0x88D30, data, 8A40]
```

Should be changed to:

```
- [0x88D30, .data, 8A40]
```

## Rebuild the Project (Build Fails)

Rebuild the project by running:

```
./tools/build-and-verify.sh
```

The build will now fail because we have not yet added the expected data. But these error messages can be instructive for telling us what data is needed, etc.

## Update C File

Examine the assembly code for the data segment, this is located in `asm/data/`. So our above example would be located in `asm/data/8A40.data.s`.

Take the labeled data from the data file and add the appropriate variable definitions to the C file (`src/8A40.c in our example).

Note that the C file will have missing data definitions already but they will be `externs`. You should add the variable definitions in the order in which they appear in the data file (do not follow the ordering of the externs).

## Build and Fix Matching Issues

Rebuild the project by running:

```
./tools/build-and-verify.sh
```

You will likely encounter build errors or checksum failures. Work through the errors and resolve any checksum mismatches.
