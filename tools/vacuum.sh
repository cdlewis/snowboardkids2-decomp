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

# Initialize counter
count=0

while true; do
  # Check if we've reached the maximum number of times
  if [[ -n "$MAX_TIMES" ]] && [[ $count -ge $MAX_TIMES ]]; then
    echo "Reached maximum iterations: $MAX_TIMES"
    break
  fi
  
  output=$(claude -p "decompile the simplest function in asm/nonmatchings/" 2>&1)
  echo "$output"
  
  if echo "$output" | grep -q "$STOP_PHRASE"; then
    echo $STOP_PHRASE
    break
  fi
  
  ((count++))
  sleep 1
done

echo "Total iterations: $count"