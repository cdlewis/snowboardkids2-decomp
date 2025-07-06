# claude-compile-and-analyse.sh $file_to_compile.c
# takes a .c file, compiles it, and diffs the resulting object code against the target assembly
set -e

INPUT="$(realpath "$1")"
OUTPUT="$(realpath "${1//.c/.o}")"

pushd /home/chris/code/snowboardkids2-decomp

# Set up / clean up compilation directory
rm -f "src/claude/$1"
mkdir -p "src/claude"

# Move target file to src directory to leverage existing make rule
cp "$INPUT" "src/claude/$1"
make "build/src/claude/${1//.c/.o}"

# Move output back to working directory and clean up
rm "src/claude/$1"
mv "build/src/claude/${1//.c/.o}" $OUTPUT

popd

echo "We got (compiled assembly):"
echo ""
python3 ./objdump.py $OUTPUT | tee base_dump
echo ""

# this should be unnecessary (target should already be in context window). but
# if we don't provide it again Claude tends to *try* getting it anyway.
echo "We want (target assembly):"
echo ""
python3 ./objdump.py target.o | tee target_dump
echo ""

# Use the object files directly with --stack-diffs to show actual differences
python3 dist.py target.o $OUTPUT --stack-diffs