#!/usr/bin/env bash
set -euo pipefail

# Ensure we're inside a git repo
if ! git rev-parse --git-dir > /dev/null 2>&1; then
  echo "Error: not inside a git repository."
  exit 1
fi

REPO_ROOT="$(git rev-parse --show-toplevel)"
GIT_DIR="$(git rev-parse --git-dir)"
HOOKS_DIR="$GIT_DIR/hooks"
HOOK_PATH="$HOOKS_DIR/pre-commit"
SOURCE_HOOK="$REPO_ROOT/tools/git-hooks/pre-commit"

mkdir -p "$HOOKS_DIR"

if [[ ! -x "$SOURCE_HOOK" ]]; then
  echo "Error: hook script '$SOURCE_HOOK' is missing or not executable."
  exit 1
fi

# If there's already a hook, check if it's already pointing at our script
if [[ -e "$HOOK_PATH" || -L "$HOOK_PATH" ]]; then
  # If it's already the right symlink, we're done
  if [[ -L "$HOOK_PATH" && "$(readlink "$HOOK_PATH")" == "$SOURCE_HOOK" ]]; then
    echo "pre-commit hook already installed."
    exit 0
  fi

  # Otherwise, back it up
  BACKUP="$HOOK_PATH.bak.$(date +%s)"
  echo "Existing pre-commit hook found, backing up to $BACKUP"
  mv "$HOOK_PATH" "$BACKUP"
fi

ln -s "$SOURCE_HOOK" "$HOOK_PATH"
echo "Installed pre-commit hook -> $SOURCE_HOOK"

