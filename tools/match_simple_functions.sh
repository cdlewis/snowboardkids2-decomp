#!/bin/bash

STOP_PHRASE="Error: All functions are marked as difficult!"

while true; do
  output=$(claude -p "decompile the simplest function in asm/nonmatchings/" 2>&1)
  echo "$output"
  
  if echo "$output" | grep -q $STOP_PHRASE; then
    echo $STOP_PHRASE
    break
  fi
  
  sleep 1
done