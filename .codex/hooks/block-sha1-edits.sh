#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/codex-hook-lib.sh"

input=$(cat)
file_path=$(printf '%s' "$input" | codex_hook_file_path)

# Block edits to snowboardkids2.sha1
if [[ "$file_path" == *snowboardkids2.sha1 ]]; then
  echo "Blocked: snowboardkids2.sha1 is a reference checksum file and should not be edited." 1>&2
  exit 2
fi

exit 0
