#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/codex-hook-lib.sh"

input=$(cat)
file_path=$(printf '%s' "$input" | codex_hook_file_path)
command=$(printf '%s' "$input" | codex_hook_command)

# Block if file_path is in the asm/ directory
if [[ "$file_path" == asm/* || "$file_path" == */asm/* ]]; then
  echo "Blocked: Files in asm/ are automatically generated and should not be edited directly. To rename a symbol, update symbol_addrs.txt. To regenerate the assembly files, run ./tools/build-and-verify.sh." 1>&2
  exit 2
fi

if echo "$command" | grep -Eq '(^|[[:space:];&|])[^;&|]*asm/'; then
  echo "Blocked: Bash commands must not modify or target asm/ directly. Files in asm/ are automatically generated. To rename a symbol, update symbol_addrs.txt." 1>&2
  exit 2
fi

exit 0
