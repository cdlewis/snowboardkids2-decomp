#!/usr/bin/env bash

# import.sh links the necessary claude helper files to the build directory
# for the target that we're trying to match. this will be run automatically
# as part of setting up the build directory.

ARG="$1"
CLAUDE_DIR="$(pwd)/tools/claude"
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
    echo "Usage: $0 <function_name_or_path_or_scratch_id>"
    exit 1
fi

# Check if the argument is a decomp.me scratch ID (numeric or alphanumeric with 5+ chars)
# Scratch IDs are typically 5-6 alphanumeric characters
if [[ "$ARG" =~ ^[0-9]+$ ]] || [[ "$ARG" =~ ^[a-zA-Z0-9]+$ && ${#ARG} -ge 5 && ${#ARG} -le 10 ]]; then
    echo "Detected scratch ID: $ARG"
    echo "Fetching from decomp.me API..."
    
    # Fetch the scratch data from decomp.me API
    SCRATCH_DATA=$(curl -s "https://decomp.me/api/scratch/$ARG")
    
    if [ $? -ne 0 ] || [ -z "$SCRATCH_DATA" ]; then
        echo "Error: Failed to fetch scratch data from decomp.me"
        exit 1
    fi
    
    # Check if we got an error response
    if echo "$SCRATCH_DATA" | grep -q '"error"'; then
        echo "Error: Scratch not found or API error"
        echo "$SCRATCH_DATA"
        exit 1
    fi
    
    # Extract diff_label and source_code from JSON response
    DIFF_LABEL=$(echo "$SCRATCH_DATA" | python3 -c "import sys, json; data = json.load(sys.stdin); print(data.get('diff_label', ''))" 2>/dev/null)
    SOURCE_CODE=$(echo "$SCRATCH_DATA" | python3 -c "import sys, json; data = json.load(sys.stdin); print(data.get('source_code', ''))" 2>/dev/null)
    
    if [ -z "$DIFF_LABEL" ] || [ -z "$SOURCE_CODE" ]; then
        echo "Error: Could not extract diff_label or source_code from API response"
        exit 1
    fi
    
    echo "Found function: $DIFF_LABEL"
    
    # Use diff_label as the function name for ASM lookup
    FUNC_NAME="$DIFF_LABEL"
    USE_SCRATCH_SOURCE=true
else
    # Regular function name or path handling
    FUNC_NAME="$ARG"
    USE_SCRATCH_SOURCE=false
fi

# Check if the argument is already a full path
if [[ "$FUNC_NAME" == *.s ]]; then
    # It's already a path
    ASM_PATH="$FUNC_NAME"
else
    # It's just a function name, search for it
    # Add .s extension if not present
    if [[ "$FUNC_NAME" != *.s ]]; then
        FUNC_NAME="${FUNC_NAME}.s"
    fi

    # Find the file in asm/nonmatchings or asm/matchings
    ASM_PATH=$(find asm/nonmatchings asm/matchings -name "$FUNC_NAME" -type f 2>/dev/null | head -1)

    if [ -z "$ASM_PATH" ]; then
        echo "Error: Could not find assembly file for function: $FUNC_NAME"
        exit 1
    fi

    echo "Found: $ASM_PATH"
fi

# Setup working directory
# Use the function name (without .s extension) for directory naming
if [ "$USE_SCRATCH_SOURCE" = true ]; then
    # Strip .s extension if present from DIFF_LABEL for directory name
    DIR_NAME="${DIFF_LABEL%.s}"
else
    # Use original argument for directory name
    DIR_NAME="$1"
fi
created_dir=$(create_unique_dir "$DIR_NAME")
echo "Created: $created_dir"

# Populate with base / target files
if [ "$USE_SCRATCH_SOURCE" = true ]; then
    # Write the source code from the scratch to base.c
    echo "#include \"common.h\"" > "$created_dir/base.c"
    echo "$SOURCE_CODE" >> "$created_dir/base.c"
    echo "Using source code from decomp.me scratch"
else
    # Use temp.c as before
    cp src/temp.c "$created_dir/base.c"
fi
cat "${CLAUDE_DIR}/prelude.inc" "${CLAUDE_DIR}/../../include/macro.inc" "$ASM_PATH" > "$created_dir/target.s"
mips-linux-gnu-as -EB -march=vr4300 -mtune=vr4300 -Iinclude -o "$created_dir/target.o" "$created_dir/target.s"
echo "Populated with target and base files"

# Add claude decomp helpers
ln -sf "${CLAUDE_DIR}"/* "$created_dir/"
ln -sf "${CLAUDE_CONFIG}" "$created_dir/"
echo "Linked claude decompiler files and .claude config into $created_dir"

cd "$created_dir"
