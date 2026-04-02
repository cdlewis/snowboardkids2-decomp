# Data Differ

A tool for comparing binary data between the compiled output and target ROM, similar to `asm-differ/diff.py` but for data sections instead of code.

## Features

- **Symbol-based lookup**: Uses `symbol_addrs.txt` to find data symbols
- **Byte-by-byte comparison**: Shows exact byte differences
- **Size detection**: Automatically reads size from symbol_addrs.txt
- **Color-coded output**: Easy to read terminal output with colors
- **Type hints**: Shows data type if specified in symbol_addrs.txt
- **Find first mismatch**: Scan all symbols in ROM order to find the first non-matching data

## Usage

### Basic Usage

```bash
# Diff a specific symbol
python3 tools/data-differ/data_diff.py gLevelWorldTable

# With explicit size override
python3 tools/data-differ/data_diff.py D_8008D9C0 --size 0x10

# Show all data even if it matches
python3 tools/data-differ/data_diff.py storyMapLocationCoords --show-all

# Custom ROM paths
python3 tools/data-differ/data_diff.py gLevelWorldTable \
  --target-rom snowboardkids2.z64 \
  --compiled-rom build/snowboardkids2.z64
```

### Finding the First Mismatch

When debugging build failures or data mismatches, use `--find-first-mismatch` to automatically scan all symbols and stop at the first one that doesn't match:

```bash
# Scan all symbols in ROM order, stop at first mismatch
python3 tools/data-differ/data_diff.py --find-first-mismatch

# Scan only symbols matching a pattern
python3 tools/data-differ/data_diff.py --find-first-mismatch --filter storyMap
python3 tools/data-differ/data_diff.py --find-first-mismatch --filter gCourse
python3 tools/data-differ/data_diff.py --find-first-mismatch --filter D_8008F

# This is useful after build failures to find which data variable is wrong
./tools/build-and-verify.sh
# If it fails, run:
python3 tools/data-differ/data_diff.py --find-first-mismatch
```

This is particularly useful when:
- The build fails with a checksum mismatch
- You've modified a data structure and need to find which variable is affected
- You're working through a large file with many data symbols

## Examples

### Find first mismatch (most useful for debugging)
```
$ python3 tools/data-differ/data_diff.py --find-first-mismatch
Loading symbols from symbol_addrs.txt...
Scanning 74 symbols for mismatches...
  Checked 74 symbols...

Found mismatch at symbol #23: storyMapLocationCoords

Symbol: storyMapLocationCoords
  VRAM:  0x8008d6ec
  ROM:   0x8e2ec
  Size:  0x28 (40 bytes)

Data differs. Showing comparison:

Target ROM data:
  00 00 00 00 01 00 00 00  02 00 00 00 03 00 00 00
  ...

Compiled data:
  00 00 00 00 01 00 00 00  02 00 00 00 05 00 00 00
  ...

Byte differences:
  Offset 0x000c: target=0x03 (3) vs compiled=0x05 (5)

Total differences: 1 bytes
```

### Matching data
```
$ python3 tools/data-differ/data_diff.py gLevelWorldTable
✓ Data matches perfectly!
```

### Non-matching data
```
$ python3 tools/data-differ/data_diff.py D_8008D9C0
Symbol: D_8008D9C0
  VRAM:  0x8008d9c0
  ROM:   0x8e5c0
  Size:  0x10 (16 bytes)

Data differs. Showing comparison:

Target ROM data:
  01 00 00 00 02 00 00 00  03 00 00 00 04 00 00 00

Compiled data:
  01 00 00 00 02 00 00 00  05 00 00 00 04 00 00 00

Byte differences:
  Offset 0x0008: target=0x03 (3) vs compiled=0x05 (5)

Total differences: 1 bytes
```

## Requirements

- Python 3.6+
- readelf and objdump (from binutils) - for ELF parsing (optional)

## Symbol Format

The tool reads symbols from `symbol_addrs.txt` in this format:

```
symbolName = 0xVRAM; // rom:0xROM size:0xSIZE [type:TYPE]
```

Example:
```
gLevelWorldTable = 0x8008D9C0; // rom:0x8E5C0 size:0x10
storyMapLocationCoords = 0x8008D6EC; // rom:0x8E2EC size:0x28
D_8008F200_8FE00 = 0x8008F200; // rom:0x8FE00 size:0xC
```

## Integration with Build System

After making changes to data in C files, use this tool to verify the data matches:

```bash
# Build the project
./tools/build-and-verify.sh

# Check specific data symbols
python3 tools/data-differ/data_diff.py gLevelWorldTable
python3 tools/data-differ/data_diff.py storyMapLocationCoords
```

## Common Issues

**Symbol not found**: Make sure the symbol exists in `symbol_addrs.txt`. Use `grep` to search for it:
```bash
grep "gLevelWorldTable" symbol_addrs.txt
```

**No size specified**: Add a size annotation to symbol_addrs.txt:
```
gLevelWorldTable = 0x8008D9C0; // rom:0x8E5C0 size:0x10
```

**Compiled ROM not found**: Run the build script first:
```bash
./tools/build-and-verify.sh
```
