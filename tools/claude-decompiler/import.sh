#!/usr/bin/env bash

# import.sh links the necessary claude helper files to the build directory
# for the target that we're trying to match. this will be run automatically
# as part of setting up the build directory.

CLAUDE_DIR="$(pwd)/tools/claude-decompiler"
CLAUDE_CONFIG="$(pwd)/.claude"

for dir in nonmatchings/*; do
  # Link all files from claude-decompiler directory
  ln -sf "${CLAUDE_DIR}"/* "$dir"/
  
  # Link the .claude config file from the parent directory
  ln -sf "${CLAUDE_CONFIG}" "$dir"/

  echo "Linked claude-decompiler files and .claude config into $dir"
done