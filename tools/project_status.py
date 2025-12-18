#!/usr/bin/env python3
"""
Project status tool for Snowboard Kids 2 decompilation.

Compares the original ROM with the built ROM to identify non-matching functions
and calculate overall match percentage.
"""

import os
import sys
from pathlib import Path
from typing import List, Tuple, Set
import re


def compare_roms(original_path: str, built_path: str) -> List[Tuple[int, int]]:
    """
    Compare two ROM files and return ranges of differing bytes.

    Returns:
        List of (start_offset, end_offset) tuples for non-matching regions
    """
    with open(original_path, 'rb') as f1, open(built_path, 'rb') as f2:
        original = f1.read()
        built = f2.read()

    if len(original) != len(built):
        print(f"Warning: ROM sizes differ: {len(original)} vs {len(built)}")
        min_len = min(len(original), len(built))
    else:
        min_len = len(original)

    # Find ranges of differing bytes
    ranges = []
    start = None

    for i in range(min_len):
        if original[i] != built[i]:
            if start is None:
                start = i
        else:
            if start is not None:
                ranges.append((start, i - 1))
                start = None

    # Handle case where difference extends to end of file
    if start is not None:
        ranges.append((start, min_len - 1))

    return ranges


def parse_asm_file(filepath: Path) -> Tuple[str, int, int]:
    """
    Parse an assembly file to extract function name and ROM offset range.

    Returns:
        (function_name, start_offset, size)
    """
    with open(filepath, 'r') as f:
        lines = f.readlines()

    if not lines:
        return None, 0, 0

    # First line should be like: nonmatching initializeGameEntity, 0x5A8
    first_line = lines[0].strip()
    match = re.match(r'(non)?matching\s+(\S+),\s+0x([0-9A-Fa-f]+)', first_line)
    if not match:
        return None, 0, 0

    function_name = match.group(2)
    size = int(match.group(3), 16)

    # Find the first instruction comment to get ROM offset
    # Format: /* 266C 80001A6C 27BDFF98 */
    start_offset = None
    for line in lines:
        comment_match = re.search(r'/\*\s+([0-9A-Fa-f]+)\s+[0-9A-Fa-f]+\s+[0-9A-Fa-f]+\s+\*/', line)
        if comment_match:
            start_offset = int(comment_match.group(1), 16)
            break

    if start_offset is None:
        return function_name, 0, size

    return function_name, start_offset, size


def get_all_functions() -> List[Tuple[str, int, int, str]]:
    """
    Get all functions from asm/matchings and asm/nonmatchings.

    Returns:
        List of (function_name, start_offset, size, status) tuples
        where status is 'matching' or 'nonmatching'
    """
    functions = []

    # Get matching functions
    matching_dir = Path('asm/matchings')
    if matching_dir.exists():
        for asm_file in matching_dir.rglob('*.s'):
            name, offset, size = parse_asm_file(asm_file)
            if name and offset > 0:
                functions.append((name, offset, size, 'matching'))

    # Get non-matching functions
    nonmatching_dir = Path('asm/nonmatchings')
    if nonmatching_dir.exists():
        for asm_file in nonmatching_dir.rglob('*.s'):
            name, offset, size = parse_asm_file(asm_file)
            if name and offset > 0:
                functions.append((name, offset, size, 'nonmatching'))

    # Sort by offset
    functions.sort(key=lambda x: x[1])

    return functions


def find_nonmatching_functions(diff_ranges: List[Tuple[int, int]],
                               functions: List[Tuple[str, int, int, str]]) -> Set[str]:
    """
    Map ROM diff ranges to non-matching functions.

    Returns:
        Set of non-matching function names
    """
    nonmatching = set()

    for start_diff, end_diff in diff_ranges:
        for func_name, func_offset, func_size, _ in functions:
            func_end = func_offset + func_size

            # Check if diff range overlaps with this function
            if not (end_diff < func_offset or start_diff >= func_end):
                nonmatching.add(func_name)

    return nonmatching


def filter_adjacent_nonmatching(nonmatching_funcs: Set[str],
                                functions: List[Tuple[str, int, int, str]]) -> List[str]:
    """
    Filter out adjacent non-matching functions, keeping only the first one.

    If functions A, B, C are adjacent and all non-matching, only keep A.
    """
    # Create a map of function name to index
    func_map = {name: i for i, (name, _, _, _) in enumerate(functions)}

    # Filter out adjacent functions
    filtered = []
    skip_until = -1

    for i, (name, _, _, _) in enumerate(functions):
        if name in nonmatching_funcs:
            if i > skip_until:
                filtered.append(name)

                # Check if next functions are also non-matching
                j = i + 1
                while j < len(functions) and functions[j][0] in nonmatching_funcs:
                    j += 1
                skip_until = j - 1

    return filtered


def main():
    # Check if ROMs exist
    original_rom = 'snowboardkids2.z64'
    built_rom = 'build/snowboardkids2.z64'

    if not os.path.exists(original_rom):
        print(f"Error: {original_rom} not found")
        sys.exit(1)

    if not os.path.exists(built_rom):
        print(f"Error: {built_rom} not found")
        print("Run 'make' first to build the ROM")
        sys.exit(1)

    print("Comparing ROMs...")
    diff_ranges = compare_roms(original_rom, built_rom)

    print("Loading function information...")
    functions = get_all_functions()

    if not functions:
        print("Warning: No functions found in asm/matchings or asm/nonmatchings")
        sys.exit(1)

    # Calculate total ROM size that contains functions
    total_size = sum(size for _, _, size, _ in functions)

    # Calculate matching bytes
    diff_bytes = sum(end - start + 1 for start, end in diff_ranges)
    matching_bytes = total_size - diff_bytes
    match_percentage = (matching_bytes / total_size * 100) if total_size > 0 else 0

    print(f"\n{'='*60}")
    print(f"Project Status: {match_percentage:.2f}% matched")
    print(f"{'='*60}")
    print(f"Matching bytes: {matching_bytes:,} / {total_size:,}")
    print(f"Non-matching bytes: {diff_bytes:,}")
    print(f"Total functions: {len(functions)}")

    if diff_ranges:
        print(f"\nNon-matching regions: {len(diff_ranges)}")

        # Map diff ranges to functions
        nonmatching_funcs = find_nonmatching_functions(diff_ranges, functions)

        if nonmatching_funcs:
            # Filter adjacent non-matching functions
            filtered_funcs = filter_adjacent_nonmatching(nonmatching_funcs, functions)

            print(f"\nNon-matching functions (excluding adjacent): {len(filtered_funcs)}")
            print(f"{'='*60}")

            # Create a map for quick lookup
            func_info = {name: (offset, size) for name, offset, size, _ in functions}

            for func_name in sorted(filtered_funcs, key=lambda x: func_info[x][0]):
                offset, size = func_info[func_name]
                print(f"  0x{offset:06X} - {func_name} (size: 0x{size:X})")
    else:
        print("\nPerfect match! All functions are matching.")

    print(f"{'='*60}\n")


if __name__ == '__main__':
    main()
