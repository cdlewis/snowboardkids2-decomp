for dir in nonmatchings/*; do
  cp tools/claude-decompiler/claude-analyse.sh "$dir"
  cp tools/claude-decompiler/claude-compile.sh "$dir"
  cp tools/claude-decompiler/CLAUDE.md "$dir"
  echo "Imported to $dir"
done