#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/codex-hook-lib.sh"

input=$(cat)
file_path=$(printf '%s' "$input" | codex_hook_file_path)
command=$(printf '%s' "$input" | codex_hook_command)

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
