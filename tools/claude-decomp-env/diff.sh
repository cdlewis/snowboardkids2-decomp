C_FILE=${1//.o/.c}
O_FILE=${1//.o/_object_dump.s}
O_FILE_NORMALIZED=${1//.o/_object_dump_normalized.s}

if [ ! -f "$O_FILE" ]; then
    echo "Error: $O_FILE does not exist."
    echo "Please run: ./build.sh $C_FILE"
    exit 1
fi

if [ ! -f "$O_FILE_NORMALIZED" ]; then
    python3 ./normalize_asm.py target_object_dump.s > target_object_dump_normalized.s
    python3 ./normalize_asm.py "$O_FILE" > "$O_FILE_NORMALIZED"
fi

diff -u --suppress-common-lines "$O_FILE_NORMALIZED" target_object_dump_normalized.s 2>/dev/null
