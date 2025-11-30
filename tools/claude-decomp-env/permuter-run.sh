#!/usr/bin/env bash
set -euo pipefail

# permuter-import.sh
#
# Usage:
#   permuter-import.sh PERMUTE_DIR [TIMEOUT_MINUTES]
#
# Arguments:
#   PERMUTE_DIR      Path to the permuter directory / target passed to permuter.py.
#   TIMEOUT_MINUTES  Optional timeout in minutes. Defaults to 10 if not provided.

usage() {
  echo "Usage: $0 PERMUTE_DIR [TIMEOUT_MINUTES]" >&2
  echo "  PERMUTE_DIR      Path to permute directory / target for permuter.py" >&2
  echo "  TIMEOUT_MINUTES  Optional timeout in minutes (default: 10)" >&2
  exit 1
}

if [[ $# -lt 1 || $# -gt 2 ]]; then
  echo "error: wrong number of arguments" >&2
  usage
fi

PERMUTE_DIR=$1
TIMEOUT_MIN=${2:-10}

if [[ -z "${PERMUTE_DIR}" ]]; then
  echo "error: PERMUTE_DIR must not be empty" >&2
  usage
fi

if [[ ! -e "${PERMUTE_DIR}" ]]; then
  echo "error: PERMUTE_DIR '${PERMUTE_DIR}' does not exist" >&2
  exit 1
fi

if ! [[ "${TIMEOUT_MIN}" =~ ^[0-9]+$ ]]; then
  echo "error: TIMEOUT_MINUTES must be a positive integer (got '${TIMEOUT_MIN}')" >&2
  exit 1
fi

if ! command -v timeout >/dev/null 2>&1; then
  echo "error: 'timeout' command not found; please install coreutils" >&2
  exit 1
fi

echo "Running permuter on '${PERMUTE_DIR}' with timeout ${TIMEOUT_MIN} minute(s)..." >&2

timeout "${TIMEOUT_MIN}m" python3 ../../tools/decomp-permuter/permuter.py \
  -j 16 \
  --best-only \
  --stop-on-zero \
  --stack-diffs \
  --quiet \
  --algorithm levenshtein \
  "${PERMUTE_DIR}"

status=$?

if [[ ${status} -eq 124 ]]; then
  echo "permuter.py timed out after ${TIMEOUT_MIN} minute(s)" >&2
elif [[ ${status} -ne 0 ]]; then
  echo "permuter.py exited with status ${status}" >&2
fi

exit "${status}"
