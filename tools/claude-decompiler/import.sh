for dir in nonmatchings/*; do
  cp tools/claude-decompiler/* "$dir"
  echo "Imported to $dir"
done