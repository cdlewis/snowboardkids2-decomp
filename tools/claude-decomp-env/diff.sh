C_FILE=${1//.o/.c}
O_FILE=${1//.o/_object_dump.s}

if [ ! -f "$O_FILE" ]; then
    echo "Error: $O_FILE does not exist."
    echo "Please run: ./build.sh $C_FILE"
    exit 1
fi

diff -u --suppress-common-lines "$O_FILE" target_object_dump.s 2>/dev/null
