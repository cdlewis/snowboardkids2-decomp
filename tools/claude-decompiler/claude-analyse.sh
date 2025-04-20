echo "We got:"
echo ""
python3 ../../tools/decomp-permuter/src/objdump.py base.o | tee base_dump
echo ""

echo "We want:"
echo ""
python3 ../../tools/decomp-permuter/src/objdump.py target.o | tee target_dump
echo ""

python3 dist.py base_dump target_dump