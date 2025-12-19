#!/usr/bin/env python3
"""
Detect low quality matches in C code.

A low quality match uses type casting and pointer arithmetic instead of
proper struct field access. This script identifies and ranks functions
by their "badness" score (violations per line of code).

By default, outputs only the worst function.
With --exhaustive, outputs all functions ranked by badness.
"""

import json
import sys
import re
from pathlib import Path
from typing import List, Tuple, Dict, NamedTuple
from collections import defaultdict


class FunctionStats(NamedTuple):
    """Statistics for a function."""
    violations: int
    lines: int

    @property
    def score(self) -> float:
        """Badness score: violations per line."""
        return self.violations / max(self.lines, 1)


def analyze_file_for_violations(filepath: str) -> Dict[str, FunctionStats]:
    """
    Analyze a C file and count violations per function.

    Returns:
        Dictionary mapping function names to FunctionStats
    """
    function_stats = {}
    current_function = None
    current_violations = 0
    current_lines = 0
    brace_depth = 0

    # Patterns to detect
    patterns = [
        # ((type*)ptr)[offset] or ((type*)ptr)[offset / N]
        r'\(\s*\(\s*\w+\s*\*\s*\)\s*[^)]+\)\s*\[\s*[^]]+\]',

        # (type*)ptr + offset or (u8*)ptr + 0x...
        r'\(\s*\w+\s*\*\s*\)\s*[^+\-;,)]+\s*[+\-]\s*(?:0x)?[0-9A-Fa-f]+',

        # *(type*)((u8*)ptr + offset)
        r'\*\s*\(\s*\w+\s*\*\s*\)\s*\(\s*\(\s*u8\s*\*\s*\)',
    ]

    compiled_patterns = [re.compile(p) for p in patterns]

    with open(filepath, 'r') as f:
        for line in f:
            stripped = line.strip()

            # Track current function
            func_match = re.match(r'^(\w+\s+)+(\w+)\s*\([^)]*\)\s*\{', line)
            if func_match and brace_depth == 0:
                current_function = func_match.group(2)
                current_violations = 0
                current_lines = 0
                brace_depth = 1
                continue

            # Track brace depth to know when function ends
            if current_function:
                brace_depth += line.count('{') - line.count('}')

                # Count non-empty, non-comment-only lines
                if stripped and not stripped.startswith('//'):
                    current_lines += 1

                if brace_depth == 0:
                    # Function ended, save stats
                    if current_violations > 0:
                        function_stats[current_function] = FunctionStats(
                            violations=current_violations,
                            lines=current_lines
                        )
                    current_function = None
                    continue

            # Count violations in current function
            if current_function:
                for pattern in compiled_patterns:
                    matches = pattern.findall(line)
                    current_violations += len(matches)

    return function_stats


def analyze_path(path: str) -> Dict[Tuple[str, str], FunctionStats]:
    """
    Analyze a file or directory.

    Returns:
        Dictionary mapping (filepath, function_name) to FunctionStats
    """
    results = {}
    p = Path(path)

    if p.is_file():
        files = [p]
    else:
        files = list(p.rglob('*.c'))

    for filepath in files:
        stats = analyze_file_for_violations(str(filepath))
        for func_name, func_stats in stats.items():
            results[(str(filepath), func_name)] = func_stats

    return results


def rank_functions(stats: Dict[Tuple[str, str], FunctionStats]) -> List[Tuple[str, str, FunctionStats]]:
    """
    Rank functions by badness score (violations per line).

    Returns:
        List of (filepath, function_name, stats) sorted by score descending
    """
    ranked = [
        (filepath, func_name, func_stats)
        for (filepath, func_name), func_stats in stats.items()
    ]
    return sorted(ranked, key=lambda x: x[2].score, reverse=True)


def main():
    import argparse

    parser = argparse.ArgumentParser(
        description="Find functions with low quality pointer arithmetic patterns",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
By default, outputs only the worst function name.
Use --exhaustive to see all functions ranked by badness.

Badness score = violations per line of code (higher is worse).

Examples:
  %(prog)s src/                       # Find worst function
  %(prog)s src/ --exhaustive          # Rank all functions
  %(prog)s src/413E0.c --exhaustive   # Rank functions in one file
        """
    )

    parser.add_argument('path', help='C file or directory to analyze')
    parser.add_argument('--exhaustive', action='store_true',
                       help='Show all functions ranked by badness')
    parser.add_argument('--verbose', '-v', action='store_true',
                       help='Show file paths and detailed stats')
    parser.add_argument('--json', action='store_true',
                       help='Output file:function pairs as JSON array for task runner')
    parser.add_argument('--limit', type=int, default=None,
                       help='Limit the number of results (for --json or --exhaustive)')

    args = parser.parse_args()

    path = Path(args.path)
    if not path.exists():
        print(f"Error: {args.path} does not exist", file=sys.stderr)
        return 1

    # Analyze and rank
    stats = analyze_path(str(path))

    if not stats:
        if not args.json:
            print("No violations found", file=sys.stderr)
        else:
            print(json.dumps([]))
        return 0

    ranked = rank_functions(stats)

    # Handle JSON output mode
    if args.json:
        # Output file:function pairs ranked by badness
        results = []
        for filepath, func_name, func_stats in ranked:
            results.append(f"{filepath}:{func_name}")

        # Apply limit if specified
        if args.limit is not None:
            results = results[:args.limit]

        print(json.dumps(results))
        return 0

    if args.exhaustive:
        # Apply limit if specified
        display_ranked = ranked[:args.limit] if args.limit is not None else ranked

        # Show all functions ranked
        if args.verbose:
            print("Functions ranked by badness (violations per line):")
            print("=" * 90)
            for i, (filepath, func_name, func_stats) in enumerate(display_ranked, 1):
                score_str = f"{func_stats.score:.3f}"
                viols_str = f"{func_stats.violations}/{func_stats.lines}"
                print(f"{i:3d}. {func_name:40s} {score_str:>6s}  ({viols_str:>7s})  {filepath}")
        else:
            for filepath, func_name, func_stats in display_ranked:
                print(func_name)
    else:
        # Show only the worst function
        worst_file, worst_func, worst_stats = ranked[0]
        if args.verbose:
            print(f"{worst_func} (score: {worst_stats.score:.3f}, "
                  f"{worst_stats.violations} violations in {worst_stats.lines} lines, "
                  f"{worst_file})")
        else:
            print(worst_func)

    return 0


if __name__ == '__main__':
    sys.exit(main())
