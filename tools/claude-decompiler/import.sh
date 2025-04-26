for dir in nonmatchings/*; do
  cp tools/claude-decompiler/* "$dir"
  cp "$dir/base.c" "$dir/base_original.c"
  echo "Imported to $dir"
done