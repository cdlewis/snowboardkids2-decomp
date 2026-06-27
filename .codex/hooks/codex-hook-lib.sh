#!/usr/bin/env bash

CODEX_HOOK_PROJECT_ROOT="/home/chris/code/project-feature-a"

codex_hook_current_root="$(git rev-parse --show-toplevel 2>/dev/null || true)"
if [[ "$codex_hook_current_root" != "$CODEX_HOOK_PROJECT_ROOT" ]]; then
  exit 0
fi

codex_hook_command() {
  jq -r '
    [
      .tool_input.command?,
      .tool_input.cmd?,
      .input.command?,
      .input.cmd?,
      .arguments.command?,
      .arguments.cmd?,
      .params.command?,
      .params.cmd?,
      .command?,
      .cmd?
    ]
    | map(select(type == "string" and length > 0))
    | first // ""
  '
}

codex_hook_file_path() {
  jq -r '
    [
      .tool_input.file_path?,
      .tool_input.path?,
      .input.file_path?,
      .input.path?,
      .arguments.file_path?,
      .arguments.path?,
      .params.file_path?,
      .params.path?,
      .file_path?,
      .path?
    ]
    | map(select(type == "string" and length > 0))
    | first // ""
  '
}
