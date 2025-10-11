#!/bin/bash

# Check if argument is provided
if [ $# -eq 0 ]; then
    echo "Usage: $0 <function_name_or_path>"
    exit 1
fi

ARG="$1"

# Check if the argument is already a full path
if [[ "$ARG" == *.s ]]; then
    # It's already a path
    ASM_PATH="$ARG"
else
    # It's just a function name, search for it
    # Add .s extension if not present
    if [[ "$ARG" != *.s ]]; then
        ARG="${ARG}.s"
    fi
    
    # Find the file in asm/nonmatchings or asm/matchings
    ASM_PATH=$(find asm/nonmatchings asm/matchings -name "$ARG" -type f 2>/dev/null | head -1)

    if [ -z "$ASM_PATH" ]; then
        echo "Error: Could not find assembly file for function: $1"
        exit 1
    fi
    
    echo "Found: $ASM_PATH"
fi

# Run the import scripts with the found path
./tools/decomp-permuter/import.py src/temp.c "$ASM_PATH"
