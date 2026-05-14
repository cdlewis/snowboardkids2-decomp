#!/usr/bin/env bash
set -euo pipefail

# stdin: JSON with tool_input containing .file_path
input=$(cat)

file_path=$(echo "$input" | jq -r '.tool_input.file_path // ""')

# Block writes to docs/index.html
if [[ "$file_path" == *"docs/index.html" ]]; then
  echo "Blocked: docs/index.html is auto-generated and should not be edited directly." 1>&2
  exit 2
fi

exit 0
