#!/bin/bash

STOP_PHRASE="Error: All functions are marked as difficult!"
MAX_TIMES=""
STOP_REQUESTED=0
STRICT_MODE=0
DIFFICULT_FUNCTIONS="tools/difficult_functions"

# Trap Ctrl+C (SIGINT) for graceful shutdown
trap 'echo ""; echo "Interrupt received, will stop after current function..."; STOP_REQUESTED=1' INT

while [[ $# -gt 0 ]]; do
  case $1 in
    --times)
      MAX_TIMES="$2"
      shift 2
      ;;
    --strict)
      STRICT_MODE=1
      shift
      ;;
    *)
      echo "Unknown option: $1"
      echo "Usage: $0 [--times X] [--strict]"
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

  simplest_func=$(python3 tools/score_functions.py asm/nonmatchings/ 2>&1)
  if [[ -z "$simplest_func" ]] || echo "$simplest_func" | grep -qF "Error:"; then
    echo "$simplest_func"
    break
  fi

  echo -e "\n[$(date '+%H:%M:%S')] Decompiling $simplest_func...\n" | tee -a "tools/vacuum.log"

  output=$(claude --model opus -p "use the decompile-a-function skill to decompile the function $simplest_func. you are running in non-interactive mode so do not wait for answers to questions. just follow the instructions in the decompile-a-function skill and remember to always commit your changes." 2>&1 | tee -a tools/vacuum.log)
  exit_code=$?
  echo "$output"

  # Clean up git env before the next commit to ensure Claude doesn't accidentally commit broken code

  if ! git diff --quiet -- "$DIFFICULT_FUNCTIONS" && [ -f "$DIFFICULT_FUNCTIONS" ]; then
    echo "Detected uncommitted difficult_functions change" | tee -a "tools/vacuum.log"
    git add "$FILE"
    git commit -m "Update $DIFFICULT_FUNCTIONS"
  fi

  # Check for modified files before reset
  modified_files=$(git diff --name-only)
  if [[ -n "$modified_files" ]]; then
    echo "Files to be reset:" | tee -a "tools/vacuum.log"
    echo "$modified_files" | tee -a "tools/vacuum.log"

    if [[ $STRICT_MODE -eq 1 ]]; then
      echo "ERROR: Strict mode enabled and unexpected files were modified" | tee -a "tools/vacuum.log"
      echo "Halting loop due to unexpected modifications" | tee -a "tools/vacuum.log"
      break
    fi
  fi

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