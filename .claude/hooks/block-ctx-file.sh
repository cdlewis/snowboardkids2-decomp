#!/usr/bin/env bash
set -euo pipefail

# stdin: JSON with tool_input containing either .command, .file_path, or .path
input=$(cat)

# Check for file path in Read/Write/Edit tools
file_path=$(echo "$input" | jq -r '.tool_input.file_path // ""')

# Check for command in Bash tool
command=$(echo "$input" | jq -r '.tool_input.command // ""')

# Block if file_path ends with ctx.c
if [[ "$file_path" == *"ctx.c" ]]; then
  echo "Blocked: ctx.c is an automatically generated file for decomp.me and should be ignored." 1>&2
  exit 2
fi

# Block if bash command references ctx.c
if echo "$command" | grep -q 'ctx\.c'; then
  echo "Blocked: ctx.c is an automatically generated file for decomp.me and should be ignored." 1>&2
  exit 2
fi

exit 0
