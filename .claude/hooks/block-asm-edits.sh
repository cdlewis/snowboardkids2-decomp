#!/usr/bin/env bash
set -euo pipefail

# stdin: JSON with tool_input containing .file_path
input=$(cat)

# Check for file path in Read/Write/Edit tools
file_path=$(echo "$input" | jq -r '.tool_input.file_path // ""')

# Block if file_path is in the asm/ directory
if [[ "$file_path" == */asm/* ]]; then
  echo "Blocked: Files in asm/ are automatically generated and should not be edited directly. Regenerate them instead." 1>&2
  exit 2
fi

# Block edits to snowboardkids2.sha1
if [[ "$file_path" == *snowboardkids2.sha1 ]]; then
  echo "Blocked: snowboardkids2.sha1 is a reference checksum file and should not be edited." 1>&2
  exit 2
fi

exit 0
