echo "We got:"
echo ""
python3 ../../tools/decomp-permuter/src/objdump.py $1.o | tee base_dump
echo ""

# avoid printing target every time, it wastes tokens
python3 ../../tools/decomp-permuter/src/objdump.py target.o > target_dump

python3 dist.py base_dump target_dump