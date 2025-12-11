# build.sh $file_to_compile.c
# takes a .c file, compiles it, and diffs the resulting object code against the target assembly
set -e

INPUT="$(realpath "$1")"
OBJECT_OUTPUT="$(realpath "${1//.c/.o}")"
ANNOTATED_OUTPUT="$(realpath "${1//.c/_annotated.s}")"
OBJECT_DUMP="${1//.c/_object_dump.s}"

pushd /home/chris/code/snowboardkids2-decomp

# Set up / clean up compilation directory
rm -f "src/claude/$1"
mkdir -p "src/claude"

# Move target file to src directory to leverage existing make rule
cp "$INPUT" "src/claude/$1"
make "build/src/claude/${1//.c/.o}"
make "build/src/claude/${1//.c/_annotated.s}"

# Move output back to working directory and clean up
rm "src/claude/$1"
mv "build/src/claude/${1//.c/.o}" $OBJECT_OUTPUT
mv "build/src/claude/${1//.c/_annotated.s}" $ANNOTATED_OUTPUT

popd

python3 ./objdump.py target.o > target_object_dump.s
python3 ./objdump.py $OBJECT_OUTPUT > $OBJECT_DUMP
echo "Raw decompiled assembly of $1: ${1//.c/_object_dump.s}"
echo "Decompiled assembly of $1 with C annotations: $ANNOTATED_OUTPUT"

echo $OBJECT_DUMP
echo ${1//.c/_diff}
diff -u --suppress-common-lines target_object_dump.s $OBJECT_DUMP || true > ${1//.c/_diff}
echo "Comparison with target file: ${1//.c/_diff}"

SCORE_OUTPUT=$(python3 dist.py target.o $OBJECT_OUTPUT --stack-diffs)
echo "$SCORE_OUTPUT"

# Extract match percentage and log it
MATCH_PERCENT=$(echo "$SCORE_OUTPUT" | grep -oP 'Score: \K[0-9.]+')
echo "$1 ${MATCH_PERCENT}%" >> match_log.txt
