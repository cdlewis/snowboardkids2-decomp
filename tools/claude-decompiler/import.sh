#!/usr/bin/env bash

CLAUDE_DIR="$(pwd)/tools/claude-decompiler"

for dir in nonmatchings/*; do
  ln -sf "${CLAUDE_DIR}"/* "$dir"/

  echo "Linked claude-decompiler files into $dir"
done
