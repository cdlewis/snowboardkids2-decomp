#!/usr/bin/env bash
set -euo pipefail

# stdin: JSON with .tool_input.command
cmd=$(jq -r '.tool_input.command // ""')

# Block direct splat split invocations
# Matches: python3 -m splat split, python -m splat split, etc.
deny_patterns=(
  '(^|[;&|]\s*)python3?\s+-m\s+splat\s+split(\s|$)'
)

for pat in "${deny_patterns[@]}"; do
  if echo "$cmd" | grep -Eiq "$pat"; then
    echo "Blocked: Direct 'python3 -m splat split' commands are not allowed. This command is automatically run by the build scripts when needed. Use ./tools/build-and-verify.sh to build the project properly." 1>&2
    exit 2
  fi
done

exit 0
