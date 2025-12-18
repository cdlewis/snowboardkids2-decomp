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


def parse_data_file(filepath: Path) -> List[Tuple[str, int, int, str]]:
    """
    Parse a data assembly file to extract data segment names and ROM offset ranges.

    Returns:
        List of (symbol_name, start_offset, size, status) tuples
        where status is 'matching' or 'nonmatching'
    """
    with open(filepath, 'r') as f:
        lines = f.readlines()

    if not lines:
        return []

    segments = []
    current_symbol = None
    current_status = None
    start_offset = None
    end_offset = None

    for line in lines:
        # Look for symbol declarations: nonmatching D_80088130_88D30 or matching D_80088130_88D30
        symbol_match = re.match(r'(non)?matching\s+(D_[0-9A-Fa-f]+_[0-9A-Fa-f]+)', line.strip())
        if symbol_match:
            # Save previous segment if exists
            if current_symbol and start_offset is not None and end_offset is not None:
                size = end_offset - start_offset
                segments.append((current_symbol, start_offset, size, current_status))

            # Start new segment
            current_symbol = symbol_match.group(2)
            current_status = 'nonmatching' if symbol_match.group(1) else 'matching'
            start_offset = None
            end_offset = None
            continue

        # Look for enddlabel to finalize current segment
        enddlabel_match = re.match(r'enddlabel\s+(D_[0-9A-Fa-f]+_[0-9A-Fa-f]+)', line.strip())
        if enddlabel_match and current_symbol:
            if start_offset is not None and end_offset is not None:
                size = end_offset - start_offset
                segments.append((current_symbol, start_offset, size, current_status))
            current_symbol = None
            current_status = None
            start_offset = None
            end_offset = None
            continue

        # Parse data lines to extract ROM offsets
        # Format: /* 88D30 80088130 00000000 */ or /* 8CB90 8008BF90 */
        if current_symbol:
            comment_match = re.search(r'/\*\s+([0-9A-Fa-f]+)\s+[0-9A-Fa-f]+', line)
            if comment_match:
                offset = int(comment_match.group(1), 16)
                if start_offset is None:
                    start_offset = offset
                end_offset = offset

                # Determine size increment based on data type
                if '.word' in line:
                    end_offset += 4
                elif '.short' in line or '.half' in line:
                    end_offset += 2
                elif '.byte' in line:
                    end_offset += 1

    # Save last segment if exists
    if current_symbol and start_offset is not None and end_offset is not None:
        size = end_offset - start_offset
        segments.append((current_symbol, start_offset, size, current_status))

    return segments


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


def get_all_data_segments() -> List[Tuple[str, int, int, str]]:
    """
    Get all data segments from asm/data.

    Returns:
        List of (symbol_name, start_offset, size, status) tuples
        where status is 'matching' or 'nonmatching'
    """
    data_segments = []

    # Get all data segments
    data_dir = Path('asm/data')
    if data_dir.exists():
        for data_file in data_dir.rglob('*.s'):
            segments = parse_data_file(data_file)
            data_segments.extend(segments)

    # Sort by offset
    data_segments.sort(key=lambda x: x[1])

    return data_segments


def find_nonmatching_symbols(diff_ranges: List[Tuple[int, int]],
                             symbols: List[Tuple[str, int, int, str]]) -> Set[str]:
    """
    Map ROM diff ranges to non-matching symbols (functions or data segments).

    Returns:
        Set of non-matching symbol names
    """
    nonmatching = set()

    for start_diff, end_diff in diff_ranges:
        for symbol_name, symbol_offset, symbol_size, _ in symbols:
            symbol_end = symbol_offset + symbol_size

            # Check if diff range overlaps with this symbol
            if not (end_diff < symbol_offset or start_diff >= symbol_end):
                nonmatching.add(symbol_name)

    return nonmatching


def filter_adjacent_nonmatching(nonmatching_symbols: Set[str],
                                symbols: List[Tuple[str, int, int, str]]) -> List[str]:
    """
    Filter out adjacent non-matching symbols, keeping only the first one.

    If symbols A, B, C are adjacent and all non-matching, only keep A.
    """
    # Create a map of symbol name to index
    symbol_map = {name: i for i, (name, _, _, _) in enumerate(symbols)}

    # Filter out adjacent symbols
    filtered = []
    skip_until = -1

    for i, (name, _, _, _) in enumerate(symbols):
        if name in nonmatching_symbols:
            if i > skip_until:
                filtered.append(name)

                # Check if next symbols are also non-matching
                j = i + 1
                while j < len(symbols) and symbols[j][0] in nonmatching_symbols:
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

    print("Loading data segment information...")
    data_segments = get_all_data_segments()

    if not functions and not data_segments:
        print("Warning: No functions or data segments found")
        sys.exit(1)

    # Calculate total ROM size that contains functions and data
    total_func_size = sum(size for _, _, size, _ in functions)
    total_data_size = sum(size for _, _, size, _ in data_segments)
    total_size = total_func_size + total_data_size

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
    print(f"Total data segments: {len(data_segments)}")

    if diff_ranges:
        print(f"\nNon-matching regions: {len(diff_ranges)}")

        # Debug: show diff ranges
        # print("\nDiff ranges:")
        # for start, end in diff_ranges[:5]:  # Show first 5
        #     print(f"  0x{start:06X} - 0x{end:06X}")

        # Map diff ranges to functions
        nonmatching_funcs = find_nonmatching_symbols(diff_ranges, functions)

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

        # Map diff ranges to data segments
        nonmatching_data = find_nonmatching_symbols(diff_ranges, data_segments)

        if nonmatching_data:
            # Filter adjacent non-matching data segments
            filtered_data = filter_adjacent_nonmatching(nonmatching_data, data_segments)

            print(f"\nNon-matching data segments (excluding adjacent): {len(filtered_data)}")
            print(f"{'='*60}")

            # Create a map for quick lookup
            data_info = {name: (offset, size) for name, offset, size, _ in data_segments}

            for data_name in sorted(filtered_data, key=lambda x: data_info[x][0]):
                offset, size = data_info[data_name]
                print(f"  0x{offset:06X} - {data_name} (size: 0x{size:X})")
    else:
        print("\nPerfect match! All functions and data segments are matching.")

    print(f"{'='*60}\n")


if __name__ == '__main__':
    main()
