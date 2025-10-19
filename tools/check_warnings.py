#!/usr/bin/env python3
"""
Check that the number of -Wincompatible-pointer-types warnings doesn't exceed the baseline.
This allows us to prevent new warnings while gradually fixing existing ones.
"""

import sys
import re
import argparse
import os

# Maximum allowed warnings (update this as you fix warnings)
MAX_WARNINGS = 27

def count_warnings(log_file):
    """Count incompatible-pointer-types warnings in build output."""
    warning_pattern = re.compile(r"warning:.*incompatible pointer types", re.IGNORECASE)

    warning_count = 0
    warnings = []

    # If log file doesn't exist, no files were compiled, so no warnings
    if not os.path.exists(log_file):
        return 0, []

    with open(log_file, 'r') as f:
        for line in f:
            if warning_pattern.search(line):
                warning_count += 1
                warnings.append(line.strip())

    return warning_count, warnings

def main():
    parser = argparse.ArgumentParser(description='Check warning count against baseline')
    parser.add_argument('log_file', help='Build log file to check')
    parser.add_argument('--max', type=int, default=MAX_WARNINGS,
                       help=f'Maximum allowed warnings (default: {MAX_WARNINGS})')
    parser.add_argument('--show-all', action='store_true',
                       help='Show all warnings found')

    args = parser.parse_args()

    count, warnings = count_warnings(args.log_file)

    print(f"\n{'='*70}")
    print(f"Incompatible Pointer Types Check")
    print(f"{'='*70}")
    print(f"Warnings found: {count}")
    print(f"Maximum allowed: {args.max}")

    if count > args.max:
        print(f"\n❌ FAILED: Found {count - args.max} more warning(s) than allowed!")
        print(f"\nPlease fix the new incompatible pointer type warnings.")
        print(f"Or if you've fixed warnings, update MAX_WARNINGS in tools/check_warnings.py")
        if args.show_all:
            print(f"\nAll warnings:")
            for w in warnings:
                print(f"  {w}")
        return 1
    elif count < args.max:
        print(f"\n✅ Great! You fixed {args.max - count} warning(s)!")
        print(f"Consider updating MAX_WARNINGS to {count} in tools/check_warnings.py")
        return 0
    else:
        print(f"\n✅ PASSED: Warning count at baseline")
        return 0

if __name__ == '__main__':
    sys.exit(main())
