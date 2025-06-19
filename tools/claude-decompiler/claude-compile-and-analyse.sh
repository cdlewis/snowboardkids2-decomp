# claude-compile-and-analyse.sh $file_to_compile.c
# takes a .c file, compiles it, and diffs the resulting object code against the target assembly
set -e

./compile.sh $1 1 $1.o

echo "We got:"
echo ""
python3 objdump_exact.py $1.o | tee base_dump
echo ""

# this should be unnecessary (target should already be in context window). but
# if we don't provide it again Claude tends to *try* getting it anyway.
python3 objdump_exact.py target.o | tee target_dump

# Use the object files directly with --stack-diffs to show actual differences
python3 dist.py target.o $1.o --stack-diffs