#!/usr/bin/env bash

# import.sh links the necessary claude helper files to the build directory
# for the target that we're trying to match. this will be run automatically
# as part of setting up the build directory.

ARG="$1"
CLAUDE_DIR="$(pwd)/tools/claude-decompiler"
CLAUDE_CONFIG="$(pwd)/.claude"

create_unique_dir() {
    mkdir -p nonmatchings

    local base_dir="nonmatchings/$1"
    local dir="$base_dir"
    local counter=2

    while [ -d "$dir" ]; do
        dir="$base_dir-$counter"
        ((counter++))
    done

    mkdir -p "$dir"
    echo "$dir"
}

# Check if argument is provided
if [ $# -eq 0 ]; then
    echo "Usage: $0 <function_name_or_path>"
    exit 1
fi

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

    # Find the file in asm/nonmatchings
    ASM_PATH=$(find asm/nonmatchings -name "$ARG" -type f | head -1)

    if [ -z "$ASM_PATH" ]; then
        echo "Error: Could not find assembly file for function: $1"
        exit 1
    fi

    echo "Found: $ASM_PATH"
fi

# Setup working directory
created_dir=$(create_unique_dir "$1")
echo "Created: $created_dir"

# Populate with base / target files
cp src/temp.c "$created_dir/base.c"
cat "${CLAUDE_DIR}/prelude.inc" "$ASM_PATH" > "$created_dir/target.s"
mips-linux-gnu-as -EB -march=vr4300 -mtune=vr4300 -Iinclude -o "$created_dir/target.o" "$created_dir/target.s"
echo "Populated with target and base files"

# Add claude decomp helpers
ln -sf "${CLAUDE_DIR}"/* "$created_dir/"
ln -sf "${CLAUDE_CONFIG}" "$created_dir/"
echo "Linked claude-decompiler files and .claude config into $created_dir"
