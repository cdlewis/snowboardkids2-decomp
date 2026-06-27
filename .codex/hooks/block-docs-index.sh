#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/codex-hook-lib.sh"

input=$(cat)
file_path=$(printf '%s' "$input" | codex_hook_file_path)

# Block writes to docs/index.html
if [[ "$file_path" == *"docs/index.html" ]]; then
  echo "Blocked: docs/index.html is auto-generated and should not be edited directly." 1>&2
  exit 2
fi

exit 0
