#!/usr/bin/env python3
"""
Find promising functions to decompile by analyzing match percentages.

This script recursively searches through nonmatchings/ directories to find
match_log.txt files, extracts the best match percentage for each function,
and filters out functions that have already been matched (exist in asm/matchings).
"""

import json
import os
import re
import sys
from pathlib import Path
from typing import Dict, List, Tuple, Optional

# Unicode frog emoji
FROG_EMOJI = "\U0001F438"


def parse_match_log(log_path: Path, root_dir: Path) -> Optional[float]:
    """
    Parse a match_log.txt file to find the best match percentage.

    Only includes percentages for files that actually exist in the same directory,
    and only if the function still exists in asm/nonmatchings (i.e., hasn't been matched yet).

    Args:
        log_path: Path to the match_log.txt file
        root_dir: Root directory of the project (to check asm/nonmatchings)

    Returns:
        The best match percentage as a float, or None if no valid percentages found
    """
    best_match = None
    log_dir = log_path.parent
    asm_nonmatchings = root_dir / "asm" / "nonmatchings"

    # Extract function name from directory (e.g., "func_8001234-2" -> "func_8001234")
    func_dir_name = log_dir.name
    func_name = func_dir_name.split('-')[0]

    # Check if function still exists in asm/nonmatchings (search recursively)
    func_exists = any(asm_nonmatchings.rglob(f"{func_name}.s"))

    try:
        with open(log_path, 'r') as f:
            for line in f:
                # Match lines like "base_1.c 41.265%"
                match = re.search(r'([a-zA-Z0-9_/]+\.c)\s+(\d+(?:\.\d+)?)\s*%', line)
                if match:
                    filename = match.group(1)
                    percentage = float(match.group(2))

                    # Only consider this percentage if the file exists AND function is still in nonmatchings
                    if func_exists and (log_dir / filename).exists():
                        if best_match is None or percentage > best_match:
                            best_match = percentage
    except (IOError, ValueError):
        pass

    return best_match


def get_function_name_from_path(path: Path) -> str:
    """
    Extract the function name from a nonmatchings directory path.

    Args:
        path: Path to the nonmatchings directory

    Returns:
        The function name (directory name)
    """
    return path.name


def is_function_matched(function_name: str, matchings_dir: Path) -> bool:
    """
    Check if a function has already been matched.

    Args:
        function_name: The name of the function to check
        matchings_dir: Path to the asm/matchings directory

    Returns:
        True if the function exists in asm/matchings, False otherwise
    """
    # Remove any suffix like "-2", "-3" from the function name for matching
    base_name = re.sub(r'-\d+$', '', function_name)

    # Search for .s files containing the function name
    if matchings_dir.exists():
        for s_file in matchings_dir.rglob('*.s'):
            if base_name in s_file.name:
                return True

    return False


def get_base_function_name(function_name: str) -> str:
    """
    Extract the base function name by removing attempt suffixes like -2, -3, etc.

    Args:
        function_name: The full function name (may include suffix)

    Returns:
        The base function name without suffix
    """
    return re.sub(r'-\d+$', '', function_name)


def count_asm_instructions(asm_path: Path) -> int:
    """
    Count the number of assembly instructions in a .s file.

    Instructions are lines that contain /* (the address/comment marker).

    Args:
        asm_path: Path to the .s file

    Returns:
        The number of instructions, or 0 if the file cannot be read
    """
    if not asm_path.exists():
        return 0

    try:
        with open(asm_path, 'r') as f:
            return sum(1 for line in f if '/*' in line)
    except (IOError, ValueError):
        return 0


def is_data_file(asm_path: Path) -> bool:
    """
    Check if a .s file is data (rodata) rather than a function.

    Args:
        asm_path: Path to the .s file

    Returns:
        True if the file appears to be data, False if it's a function
    """
    if not asm_path.exists():
        return False

    try:
        with open(asm_path, 'r') as f:
            content = f.read()
            # Check for .rodata section indicator
            if '.section .rodata' in content:
                return True
            # Check for data directives without function prologue
            if '.word ' in content or '.byte ' in content or '.asciz ' in content:
                # If it has glabel/nonmatching and typical function start, it's a function
                if 'glabel ' in content or 'nonmatching ' in content:
                    return False
                return True
            return False
    except IOError:
        return False


def load_decompme_scratches(scratches_path: Path) -> Dict[str, float]:
    """
    Load decomp.me scratches and build a lookup dict of function name -> best match percentage.

    Args:
        scratches_path: Path to the scratches.json file

    Returns:
        Dict mapping function name to best (highest) match percentage, or empty dict if file not found
    """
    if not scratches_path.exists():
        return {}

    try:
        with open(scratches_path, 'r') as f:
            scratches = json.load(f)

        # Build mapping: function_name -> best (highest) percentage
        # Multiple scratches may exist for the same function; keep the best one
        result: Dict[str, float] = {}
        for scratch in scratches:
            name = scratch.get('name')
            score = scratch.get('score')
            max_score = scratch.get('max_score')
            if name is not None and score is not None and max_score is not None and max_score > 0:
                # Calculate match percentage
                percentage = ((max_score - score) / max_score) * 100
                if name not in result or percentage > result[name]:
                    result[name] = percentage

        return result
    except (IOError, json.JSONDecodeError):
        return {}


def find_promising_functions(repo_root: Path, decompme_scratches: Dict[str, float]) -> List[Tuple[str, float, int, Optional[float]]]:
    """
    Find all functions with their best match percentages and instruction counts.

    Args:
        repo_root: Root directory of the repository
        decompme_scratches: Dict mapping function name to best decomp.me match percentage

    Returns:
        List of (function_name, best_match_percentage, instruction_count, decompme_percentage) tuples,
        sorted by percentage descending
    """
    nonmatchings_dir = repo_root / 'nonmatchings'
    matchings_dir = repo_root / 'asm' / 'matchings'
    asm_nonmatchings = repo_root / 'asm' / 'nonmatchings'

    # Dictionary to track best attempt for each base function
    # Key: base function name, Value: (full function name, percentage, instruction_count, decompme_percentage)
    best_attempts: Dict[str, Tuple[str, float, int, Optional[float]]] = {}

    # Find all match_log.txt files
    for match_log in nonmatchings_dir.rglob('match_log.txt'):
        function_dir = match_log.parent
        function_name = get_function_name_from_path(function_dir)

        # Skip if function is already matched
        if is_function_matched(function_name, matchings_dir):
            continue

        # Parse the match log
        best_match = parse_match_log(match_log, repo_root)

        if best_match is not None:
            base_name = get_base_function_name(function_name)

            # Find the corresponding .s file in asm/nonmatchings to count instructions
            asm_file = None
            for s_file in asm_nonmatchings.rglob(f'{base_name}.s'):
                asm_file = s_file
                break

            instruction_count = count_asm_instructions(asm_file) if asm_file else 0

            # Look up decomp.me percentage for this function
            decompme_percentage = decompme_scratches.get(base_name)

            # Keep only the best attempt for each base function
            if (base_name not in best_attempts or
                best_match > best_attempts[base_name][1]):
                best_attempts[base_name] = (function_name, best_match, instruction_count, decompme_percentage)

    # Convert to list and sort by percentage descending
    results = list(best_attempts.values())
    results.sort(key=lambda x: x[1], reverse=True)

    return results


def find_untouched_functions(
    repo_root: Path,
    promising_functions: List[Tuple[str, float, int, Optional[float]]],
    decompme_scratches: Dict[str, float]
) -> List[Tuple[str, int]]:
    """
    Find functions that have no local attempts and no decomp.me scratches.

    Args:
        repo_root: Root directory of the repository
        promising_functions: List of functions already in the promising list
        decompme_scratches: Dict mapping function name to best decomp.me score

    Returns:
        List of (function_name, instruction_count) tuples, sorted by instruction count ascending
    """
    asm_nonmatchings = repo_root / 'asm' / 'nonmatchings'
    matchings_dir = repo_root / 'asm' / 'matchings'

    # Get set of function names already in promising list
    promising_names = {get_base_function_name(name) for name, _, _, _ in promising_functions}

    # Find all .s files in asm/nonmatchings
    untouched = []
    for s_file in asm_nonmatchings.rglob('*.s'):
        func_name = s_file.stem  # filename without .s extension

        # Skip data symbols (D_ prefix) and jtbl (jump tables)
        if func_name.startswith('D_') or func_name.startswith('jtbl_'):
            continue

        # Skip if the file is actually data (rodata section)
        if is_data_file(s_file):
            continue

        # Skip if already matched
        if is_function_matched(func_name, matchings_dir):
            continue

        # Skip if in promising functions
        if func_name in promising_names:
            continue

        # Skip if has decomp.me scratch
        if func_name in decompme_scratches:
            continue

        # Count instructions and add to list
        instr_count = count_asm_instructions(s_file)
        untouched.append((func_name, instr_count))

    # Sort by instruction count ascending (easier functions first)
    untouched.sort(key=lambda x: x[1])

    return untouched


def main():
    """Main entry point."""
    # Get repository root (parent of tools directory)
    repo_root = Path(__file__).parent.parent
    tools_dir = Path(__file__).parent

    # Load decomp.me scratches
    scratches_path = tools_dir / 'scratches.json'
    decompme_scratches = load_decompme_scratches(scratches_path)
    if decompme_scratches:
        print(f"Loaded {len(decompme_scratches)} decomp.me scratches")
    else:
        print(f"Warning: Could not load {scratches_path}")

    # Find promising functions
    functions = find_promising_functions(repo_root, decompme_scratches)

    if not functions:
        print("No unmatched functions with match logs found.")
        return 0

    # Print header
    print(f"{'Function Name':<40} {'Ins':>5} {'Best Match %':>12}  {'decomp.me':<20}")
    print("-" * 83)

    # Print results
    for function_name, percentage, instruction_count, decompme_percentage in functions:
        if decompme_percentage is not None:
            # Use mushroom emoji if local match is better than decomp.me
            if percentage > decompme_percentage:
                decompme_str = f"🍄 ({decompme_percentage:.2f}%)"
            else:
                decompme_str = f"({decompme_percentage:.2f}%)"
        else:
            decompme_str = ""
        print(f"{function_name:<40} {instruction_count:>4} {percentage:>11.2f}%  {decompme_str:<20}")

    print(f"\nTotal: {len(functions)} unmatched functions with attempts")

    # Find untouched functions (no local attempts, no decomp.me scratches)
    untouched = find_untouched_functions(repo_root, functions, decompme_scratches)

    if untouched:
        print(f"\n{'=' * 78}")
        print("UNTOUCHED FUNCTIONS (no local attempts, no decomp.me scratches)")
        print(f"{'=' * 78}")
        print(f"{'Function Name':<40} {'Ins':>5}")
        print("-" * 50)

        for func_name, instr_count in untouched:
            print(f"{func_name:<40} {instr_count:>4}")

        print(f"\nTotal: {len(untouched)} untouched functions")

    return 0


if __name__ == '__main__':
    sys.exit(main())
