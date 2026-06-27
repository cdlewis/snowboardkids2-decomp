#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/codex-hook-lib.sh"

input=$(cat)
cmd=$(printf '%s' "$input" | codex_hook_command)

# Block list (add as needed)
deny_patterns=(
  'git\s+commit\s+--no-verify'
)

for pat in "${deny_patterns[@]}"; do
  if echo "$cmd" | grep -Eiq "$pat"; then
    echo "Blocked command: matches denied pattern '$pat'. Use a safer alternative or explain why it's necessary." 1>&2
    exit 2
  fi
done

exit 0
