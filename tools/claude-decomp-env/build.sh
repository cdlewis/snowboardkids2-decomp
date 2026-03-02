#!/bin/bash
# build.sh $file_to_compile.c
# takes a .c file, compiles it, and diffs the resulting object code against the target assembly
set -e

INPUT="$(realpath "$1")"
OPT_FLAG="$2"
OBJECT_OUTPUT="$(realpath "${1//.c/.o}")"
ANNOTATED_OUTPUT="$(realpath "${1//.c/_annotated.s}")"
OBJECT_DUMP="${1//.c/_object_dump.s}"

# Check if file contains INCLUDE_ASM
if grep -q "INCLUDE_ASM" "$INPUT"; then
    echo "ERROR: The C file contains INCLUDE_ASM macro!"
    echo ""
    echo "INCLUDE_ASM is NOT a valid decompilation technique."
    echo "You must write actual C code that compiles to matching assembly."
    echo ""
    echo "Using INCLUDE_ASM is an attempt to cheat the matching process by"
    echo "embedding assembly directly. This defeats the entire purpose of"
    echo "decompilation, which is to produce readable, maintainable C code."
    echo ""
    echo "Please rewrite your code as proper C without any INCLUDE_ASM macros."
    exit 1
fi

# Set project root to two directories above this script
SCRIPT_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_PATH/../.." && pwd)"

# Compiler settings (mirroring Makefile)
COMPILER_DIR="$PROJECT_ROOT/tools/gcc_kmc"
CROSS="mips-linux-gnu-"
OBJDUMP="${CROSS}objdump"
TEXTCONV="python3 $PROJECT_ROOT/tools/textconv.py"
CHARMAP="$PROJECT_ROOT/tools/charmap.txt"

ABIFLAG="-mabi=32 -mgp32 -mfp32"
CFLAGS_BASE="$ABIFLAG -mno-abicalls -nostdinc -fno-PIC -G 0 -Wa,-force-n64align -funsigned-char -w -mips3 -EB -fno-builtin -fno-common"
OPT_FLAGS="${OPT_FLAG:--O2}"
IINC="-I $PROJECT_ROOT/include -I $PROJECT_ROOT/lib/ultralib/include -I $PROJECT_ROOT/lib/ultralib/include/PR -I $PROJECT_ROOT/lib/libmus/include/PR -I $PROJECT_ROOT/lib/libmus/src -I $PROJECT_ROOT/lib/f3dex2/PR"
MACROS="-D_LANGUAGE_C -D_MIPS_SZLONG=32 -D_MIPS_SZINT=32 -D_MIPS_SZLONG=32 -D__USE_ISOC99 -DF3DEX_GBI_2 -DNDEBUG -D_FINALROM -DF3DEX_GBI_2"

# Compile from project root so assembler can find include/labels.inc
pushd "$PROJECT_ROOT" > /dev/null

# Run textconv to handle _("string") patterns, then compile
# textconv passes the file through unchanged if it doesn't use font_encoding.h
$TEXTCONV "$CHARMAP" "$INPUT" - | \
    COMPILER_PATH="$COMPILER_DIR" "$COMPILER_DIR/gcc" \
    $CFLAGS_BASE $OPT_FLAGS -fno-asm \
    -I "$PROJECT_ROOT/src/" $IINC $MACROS \
    -x c -c -o "$OBJECT_OUTPUT" -

# Generate annotated assembly (optional - don't fail if this errors)
{
    TEMP_OBJ=$(mktemp)
    COMPILER_PATH="$COMPILER_DIR" "$COMPILER_DIR/gcc" \
        $CFLAGS_BASE $OPT_FLAGS -g -fno-asm \
        -I "$PROJECT_ROOT/src/" $IINC $MACROS \
        -c -o "$TEMP_OBJ" "$INPUT" && \
    $OBJDUMP -d --line-numbers --reloc --source "$TEMP_OBJ" > "$ANNOTATED_OUTPUT"
    rm -f "$TEMP_OBJ"
} 2>/dev/null || true

popd > /dev/null

# Validate the compiled object has actual code
if ! mips-linux-gnu-nm "$OBJECT_OUTPUT" 2>/dev/null | grep -q ' T '; then
    echo "ERROR: Compiled object has no text symbols. Check for type conflicts or include issues."
    echo "Symbols found:"
    mips-linux-gnu-nm "$OBJECT_OUTPUT" 2>/dev/null || true
    exit 1
fi

python3 ./objdump.py target.o > target_object_dump.s
python3 ./objdump.py $OBJECT_OUTPUT > $OBJECT_DUMP
echo "Raw decompiled assembly of $1: ${1//.c/_object_dump.s}"
echo "Decompiled assembly of $1 with C annotations: $ANNOTATED_OUTPUT"

echo $OBJECT_DUMP
echo ${1//.c/_diff}
python3 ./normalize_asm.py target_object_dump.s > target_object_dump_normalized.s
python3 ./normalize_asm.py $OBJECT_DUMP > ${1//.c/_object_dump_normalized.s}
diff -u --suppress-common-lines target_object_dump_normalized.s ${1//.c/_object_dump_normalized.s} > ${1//.c/_diff} || true
echo "Comparison with target file: ${1//.c/_diff}"

SCORE_OUTPUT=$(python3 dist.py target.o $OBJECT_OUTPUT --stack-diffs)
echo "$SCORE_OUTPUT"

# Extract match percentage and log it (only for base_* files)
MATCH_PERCENT=$(echo "$SCORE_OUTPUT" | grep -oP 'Score: \K[0-9.]+')
if [[ $1 =~ base_[0-9]+ ]]; then
    echo "$1 ${MATCH_PERCENT}%" >> match_log.txt
fi
