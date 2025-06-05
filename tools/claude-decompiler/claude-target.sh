# claude-target.sh dumps the assembly code from the target object
# (the object that we're trying to create matching code for).
python3 ../../tools/decomp-permuter/src/objdump.py target.o | tee target_dump
