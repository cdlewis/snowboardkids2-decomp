#!/usr/bin/env python3
"""Normalize assembly for easier comparison by converting jumptable symbols to .rodata"""
import re
import sys

def normalize_jumptable_references(line: str) -> str:
    """Normalize jump table references to .rodata for consistent comparison"""
    # Pattern to match jump table references like jtbl_8009DDA8_9E9A8
    jtbl_pattern = r'jtbl_[0-9A-Fa-f]+_[0-9A-Fa-f]+'

    # Replace jump table references with .rodata
    normalized = re.sub(jtbl_pattern, '.rodata', line)
    return normalized

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: normalize_asm.py <file>")
        sys.exit(1)

    with open(sys.argv[1], 'r') as f:
        for line in f:
            print(normalize_jumptable_references(line), end='')
