#!/usr/bin/env bash
set -euo pipefail

# setup-permuter.sh
#
# Usage:
#   setup-permuter.sh SOURCE_C
#
# Arguments:
#   SOURCE_C   Path to the C file you want to permute.
#
# Example:
#   ./setup-permuter.sh base.c
#
# After running this, you can invoke run-permuter.sh (or your permuter
# driver script) on the generated permute directory.

usage() {
  echo "Usage: $0 SOURCE_C" >&2
  echo "  SOURCE_C   Path to the C file to permute" >&2
  exit 1
}

# --- argument parsing & validation ---

if [[ $# -ne 1 ]]; then
  echo "error: wrong number of arguments" >&2
  usage
fi

SOURCE_C=$1

if [[ -z "${SOURCE_C}" ]]; then
  echo "error: SOURCE_C must not be empty" >&2
  usage
fi

if [[ ! -f "${SOURCE_C}" ]]; then
  echo "error: SOURCE_C '${SOURCE_C}' does not exist or is not a regular file" >&2
  exit 1
fi

if [[ ! -f ../../tools/decomp-permuter/import.py ]]; then
  echo "error: ../../tools/decomp-permuter/import.py not found" >&2
  exit 1
fi

if ! command -v python3 >/dev/null 2>&1; then
  echo "error: python3 not found on PATH" >&2
  exit 1
fi

echo "Setting up permute environment for '${SOURCE_C}' using target.s..." >&2

python3 ../../tools/decomp-permuter/import.py "${SOURCE_C}" target.s
