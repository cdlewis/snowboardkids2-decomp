#!/bin/bash

while true; do
  output=$(claude -p "decompile the simplest function in asm/nonmatchings/" 2>&1)
  echo "$output"
  
  if echo "$output" | grep -q "Error: All functions are marked as difficult!"; then
    echo "All functions marked as difficult - stopping loop"
    break
  fi
  
  sleep 1
done