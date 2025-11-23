#!/usr/bin/env python3
"""
Analyze git commit logs to find and score matched functions.

Usage:
    python3 tools/score_matched_functions.py <commit_hash>
    python3 tools/score_matched_functions.py HEAD~20
    python3 tools/score_matched_functions.py abc123f

This script:
1. Finds all commits newer than the provided commit hash
2. Filters for commits containing 'match' (case-insensitive) without 🧑 emoji
3. Extracts potential function names from commit messages
4. Scores each function that exists in the codebase
5. Prints a sorted list of functions
"""

import sys
import subprocess
import re
from pathlib import Path
from typing import List, Tuple, Optional

# Add tools directory to path to import score_functions
tools_dir = Path(__file__).parent
sys.path.insert(0, str(tools_dir))

from score_functions import score_folder


def get_commits_since(commit_hash: str) -> List[Tuple[str, str]]:
    """Get all commit hashes and messages since the provided commit.

    Returns:
        List of (hash, message) tuples
    """
    try:
        result = subprocess.run(
            ['git', 'log', '--oneline', f'{commit_hash}..HEAD'],
            capture_output=True,
            text=True,
            check=True
        )

        commits = []
        for line in result.stdout.strip().split('\n'):
            if not line:
                continue
            # Split on first space to separate hash from message
            parts = line.split(' ', 1)
            if len(parts) == 2:
                commits.append((parts[0], parts[1]))

        return commits
    except subprocess.CalledProcessError as e:
        print(f"Error running git log: {e}", file=sys.stderr)
        sys.exit(1)


def should_process_commit(message: str) -> bool:
    """Check if commit message should be processed.

    Returns True if:
    - Contains 'match' (case-insensitive)
    - Does NOT contain 🧑 emoji
    """
    if '🧑' in message:
        return False

    if 'match' not in message.lower():
        return False

    return True


def extract_potential_functions(message: str) -> List[str]:
    """Extract potential function names from commit message.

    Function names typically match patterns like:
    - func_80032330_32F30
    - __MusIntFifoProcessCommand
    """
    # Split message into words
    words = message.split()

    # Common function name patterns
    patterns = [
        r'^func_[0-9A-Fa-f]+_[0-9A-Fa-f]+$',  # func_80032330_32F30
        r'^[a-zA-Z_][a-zA-Z0-9_]*$',           # Standard C identifiers
    ]

    potential_functions = []
    for word in words:
        # Remove common punctuation
        cleaned = word.strip('.,;:!?')

        # Check if it matches any function pattern
        for pattern in patterns:
            if re.match(pattern, cleaned):
                potential_functions.append(cleaned)
                break

    return potential_functions


def score_function(func_name: str) -> Optional[float]:
    """Score a specific function if it exists.

    Returns:
        Score as float, or None if function doesn't exist
    """
    try:
        # Score all functions in asm/ directory (exhaustive mode to find individual functions)
        asm_dir = Path(__file__).parent.parent / 'asm'
        scores = score_folder(str(asm_dir), exhaustive=True)

        # Find matching function
        for score in scores:
            if score.name == func_name:
                return score.total_score

        return None
    except Exception as e:
        print(f"Error scoring function {func_name}: {e}", file=sys.stderr)
        return None


def main():
    if len(sys.argv) != 2:
        print("Usage: python3 tools/score_matched_functions.py <commit_hash>", file=sys.stderr)
        print("Example: python3 tools/score_matched_functions.py HEAD~20", file=sys.stderr)
        sys.exit(1)

    commit_hash = sys.argv[1]

    # Get commits since the provided hash
    commits = get_commits_since(commit_hash)

    if not commits:
        print(f"No commits found since {commit_hash}", file=sys.stderr)
        sys.exit(0)

    # Cache all scores to avoid re-scanning for each function
    asm_dir = Path(__file__).parent.parent / 'asm'
    all_scores = score_folder(str(asm_dir), exhaustive=True)
    score_map = {s.name: s.total_score for s in all_scores}
    print(f"Loaded {len(score_map)} functions.\n", file=sys.stderr)

    # Process commits
    found_functions = {}  # func_name -> score

    for commit_hash, message in commits:
        if not should_process_commit(message):
            continue

        # Extract potential function names
        potential_funcs = extract_potential_functions(message)

        for func_name in potential_funcs:
            if func_name in score_map:
                found_functions[func_name] = score_map[func_name]

    # Print results sorted by score
    if not found_functions:
        print("\nNo valid functions found in matching commits.", file=sys.stderr)
        sys.exit(0)

    # Sort by score (ascending - simplest first)
    sorted_functions = sorted(found_functions.items(), key=lambda x: x[1])

    for func_name, score in sorted_functions:
        print(func_name)


if __name__ == '__main__':
    main()
