#!/usr/bin/env bash
set -euo pipefail

# stdin: JSON with .tool_input.command
cmd=$(jq -r '.tool_input.command // ""')

# Block direct make invocations (but not cmake, automake, etc.)
# Matches: make, make clean, make -j4, etc.
deny_patterns=(
  '(^|[;&|]\s*)make(\s|$)'
)

for pat in "${deny_patterns[@]}"; do
  if echo "$cmd" | grep -Eiq "$pat"; then
    echo "Blocked: Direct 'make' commands are not allowed. Please use the build script provided in the instructions. This will be ./tools/build-and-verify.sh to rebuild the whole project or ./build.sh <file> if you're working on a single function in a Claude decomp environment." 1>&2
    exit 2
  fi
done

exit 0
