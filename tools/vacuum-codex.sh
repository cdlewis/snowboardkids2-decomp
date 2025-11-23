#!/bin/bash

STOP_PHRASE="Error: All functions are marked as difficult!"
MAX_TIMES=""

# Parse command line arguments
while [[ $# -gt 0 ]]; do
  case $1 in
    --times)
      MAX_TIMES="$2"
      shift 2
      ;;
    *)
      echo "Unknown option: $1"
      echo "Usage: $0 [--times X]"
      exit 1
      ;;
  esac
done

# Validate MAX_TIMES if provided
if [[ -n "$MAX_TIMES" ]] && ! [[ "$MAX_TIMES" =~ ^[0-9]+$ ]]; then
  echo "Error: --times argument must be a positive integer"
  exit 1
fi

# Initialize counters
count=0
consecutive_failures=0

while true; do
  # Check if we've reached the maximum number of times
  if [[ -n "$MAX_TIMES" ]] && [[ $count -ge $MAX_TIMES ]]; then
    echo "Reached maximum iterations: $MAX_TIMES"
    break
  fi

  output=$(codex exec --yolo "decompile the simplest function in asm/nonmatchings. read CLAUDE.md for instructions/context. use thinking. do not wait for approvals" 2>&1)
  exit_code=$?
  echo "$output"

  if echo "$output" | grep -q "$STOP_PHRASE"; then
    echo $STOP_PHRASE
    break
  fi

  # Track consecutive failures
  if [[ $exit_code -ne 0 ]]; then
    ((consecutive_failures++))
    echo "Claude returned exit code: $exit_code (consecutive failures: $consecutive_failures)"

    if [[ $consecutive_failures -ge 3 ]]; then
      echo "Three consecutive failures detected. Sleeping for 5 minutes..."
      sleep 300
      consecutive_failures=0
    else
      sleep 1
    fi
  else
    consecutive_failures=0
    sleep 1
  fi

  ((count++))
done

echo "Total iterations: $count"