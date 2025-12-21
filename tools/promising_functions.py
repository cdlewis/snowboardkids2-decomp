#!/usr/bin/env python3
"""
Find promising functions to decompile by analyzing match percentages.

This script recursively searches through nonmatchings/ directories to find
match_log.txt files, extracts the best match percentage for each function,
and filters out functions that have already been matched (exist in asm/matchings).
"""

import os
import re
import sys
from pathlib import Path
from typing import Dict, List, Tuple, Optional


def parse_match_log(log_path: Path) -> Optional[float]:
    """
    Parse a match_log.txt file to find the best match percentage.

    Args:
        log_path: Path to the match_log.txt file

    Returns:
        The best match percentage as a float, or None if no valid percentages found
    """
    best_match = None

    try:
        with open(log_path, 'r') as f:
            for line in f:
                # Match lines like "base_1.c 41.265%"
                match = re.search(r'(\d+(?:\.\d+)?)\s*%', line)
                if match:
                    percentage = float(match.group(1))
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


def find_promising_functions(repo_root: Path) -> List[Tuple[str, float]]:
    """
    Find all functions with their best match percentages.

    Args:
        repo_root: Root directory of the repository

    Returns:
        List of (function_name, best_match_percentage) tuples, sorted by percentage descending
    """
    nonmatchings_dir = repo_root / 'nonmatchings'
    matchings_dir = repo_root / 'asm' / 'matchings'

    # Dictionary to track best attempt for each base function
    # Key: base function name, Value: (full function name, percentage)
    best_attempts: Dict[str, Tuple[str, float]] = {}

    # Find all match_log.txt files
    for match_log in nonmatchings_dir.rglob('match_log.txt'):
        function_dir = match_log.parent
        function_name = get_function_name_from_path(function_dir)

        # Skip if function is already matched
        if is_function_matched(function_name, matchings_dir):
            continue

        # Parse the match log
        best_match = parse_match_log(match_log)

        if best_match is not None:
            base_name = get_base_function_name(function_name)

            # Keep only the best attempt for each base function
            if base_name not in best_attempts or best_match > best_attempts[base_name][1]:
                best_attempts[base_name] = (function_name, best_match)

    # Convert to list and sort by percentage descending
    results = [(name, pct) for name, pct in best_attempts.values()]
    results.sort(key=lambda x: x[1], reverse=True)

    return results


def main():
    """Main entry point."""
    # Get repository root (parent of tools directory)
    repo_root = Path(__file__).parent.parent

    # Find promising functions
    functions = find_promising_functions(repo_root)

    if not functions:
        print("No unmatched functions with match logs found.")
        return 0

    # Print header
    print(f"{'Function Name':<40} {'Best Match %':>12}")
    print("-" * 53)

    # Print results
    for function_name, percentage in functions:
        print(f"{function_name:<40} {percentage:>11.2f}%")

    print(f"\nTotal: {len(functions)} unmatched functions")

    return 0


if __name__ == '__main__':
    sys.exit(main())
