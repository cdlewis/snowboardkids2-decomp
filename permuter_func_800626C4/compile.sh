#!/bin/bash
set -e

# Parse args: compile.sh input.c -o output.o
INPUT="$1"
shift
# Skip -o flag
if [ "$1" = "-o" ]; then
    shift
fi
OUTPUT="$1"

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

rm -f "$PROJECT_ROOT/src/claude/permuter_input.c"
mkdir -p "$PROJECT_ROOT/src/claude"
cp "$INPUT" "$PROJECT_ROOT/src/claude/permuter_input.c"
make -C "$PROJECT_ROOT" "build/src/claude/permuter_input.o" 2>/dev/null
cp "$PROJECT_ROOT/build/src/claude/permuter_input.o" "$OUTPUT"
rm -f "$PROJECT_ROOT/src/claude/permuter_input.c"
