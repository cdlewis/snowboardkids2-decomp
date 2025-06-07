#!/usr/bin/env bash

# import.sh links the necessary claude helper files to the build directory
# for the target that we're trying to match. this will be run automatically
# as part of setting up the build directory.

CLAUDE_DIR="$(pwd)/tools/claude-decompiler"

for dir in nonmatchings/*; do
  ln -sf "${CLAUDE_DIR}"/* "$dir"/
  ln -sf "${CLAUDE_DIR}"../.claude "$dir"/

  echo "Linked claude-decompiler files into $dir"
done
