#!/bin/bash

MAX_TIMES=""
STOP_REQUESTED=0

# Trap Ctrl+C (SIGINT) for graceful shutdown
trap 'echo ""; echo "Interrupt received, will stop after current function..."; STOP_REQUESTED=1' INT

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

if [[ -n "$MAX_TIMES" ]] && ! [[ "$MAX_TIMES" =~ ^[0-9]+$ ]]; then
  echo "Error: --times argument must be a positive integer"
  exit 1
fi

count=0
consecutive_failures=0

while true; do
  if [[ $STOP_REQUESTED -eq 1 ]]; then
    echo "Stopping gracefully."
    break
  fi

  if [[ -n "$MAX_TIMES" ]] && [[ $count -ge $MAX_TIMES ]]; then
    echo "Reached maximum iterations: $MAX_TIMES"
    break
  fi

  output=$(claude -p "clean up extern headers" 2>&1 | tee -a tools/vacuum-headers.log)
  exit_code=$?
  echo "$output"

  # Clean up git env before the next commit to ensure Claude doesn't accidentally commit broken code

  git reset --hard HEAD

  # Give up if Claude still somehow managed to do it anyway
  
  ./tools/build-and-verify.sh || break

  if [[ $STOP_REQUESTED -eq 1 ]]; then
    echo "Stopping gracefully."
    break
  fi

  if echo "$output" | grep -qF "$STOP_PHRASE"; then
    echo "$STOP_PHRASE"
    break
  fi

  if [[ $exit_code -ne 0 ]]; then
    ((consecutive_failures++))
    if [[ $consecutive_failures -ge 3 ]]; then
      echo ">= 3 consecutive failures detected. Sleeping for 5 minutes..."
      sleep 300
    fi
  else
    consecutive_failures=0
  fi

  ((count++))
done

echo "Total iterations: $count"