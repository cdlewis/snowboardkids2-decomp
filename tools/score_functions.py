#!/usr/bin/env python3
"""
Score assembly functions by complexity and find the simplest one to decompile.

Usage:
    python3 tools/score_functions.py <folder_path>
    python3 tools/score_functions.py asm/nonmatchings/displaylist
"""

import sys
import os
import re
from pathlib import Path
from dataclasses import dataclass
from typing import List, Tuple


@dataclass
class FunctionScore:
    """Stores complexity metrics for a function."""
    name: str
    file_path: str
    instruction_count: int = 0
    branch_count: int = 0
    jump_count: int = 0
    label_count: int = 0
    stack_size: int = 0

    @property
    def total_score(self) -> float:
        """Calculate total complexity score."""
        return (
            self.instruction_count * 1.0 +
            self.branch_count * 3.0 +
            self.jump_count * 2.0 +
            self.label_count * 2.0 +
            self.stack_size * 0.1
        )

    def __str__(self) -> str:
        return (
            f"{self.name:50s} | Score: {self.total_score:7.1f} | "
            f"Instructions: {self.instruction_count:3d} | "
            f"Branches: {self.branch_count:2d} | "
            f"Jumps: {self.jump_count:2d} | "
            f"Labels: {self.label_count:2d} | "
            f"Stack: {self.stack_size:4d}"
        )


def analyze_function(file_path: str) -> FunctionScore:
    """Analyze an assembly file and return complexity metrics."""

    # Branch instructions (conditional)
    branch_patterns = [
        r'\b(beq|bne|bnez|beqz|blez|bgtz|bltz|bgez|blt|bgt|ble|bge|bltzal|bgezal)\b'
    ]

    # Jump instructions
    jump_patterns = [
        r'\bjal\b',  # Function calls
        r'\bj\b'     # Unconditional jumps (not jr - that's return)
    ]

    # Local label pattern (e.g., .L800095A8_A1A8:)
    label_pattern = r'^\s*\.L[0-9A-Fa-f_]+:'

    # Instruction pattern (lines with actual assembly)
    instruction_pattern = r'/\*\s*[0-9A-Fa-f]+\s+[0-9A-Fa-f]+\s+[0-9A-Fa-f]+\s+\*/'

    # Stack allocation pattern (addiu $sp, $sp, -0xNN)
    stack_pattern = r'addiu\s+\$sp,\s*\$sp,\s*-0x([0-9A-Fa-f]+)'

    # Extract function name from file
    filename = os.path.basename(file_path)
    func_name = filename.replace('.s', '')

    score = FunctionScore(name=func_name, file_path=file_path)

    try:
        with open(file_path, 'r') as f:
            for line in f:
                line = line.strip()

                # Skip empty lines, glabel, endlabel, and comments
                if not line or line.startswith('glabel') or line.startswith('endlabel') or line.startswith('nonmatching'):
                    continue

                # Count local labels
                if re.match(label_pattern, line):
                    score.label_count += 1
                    continue

                # Count instructions
                if re.search(instruction_pattern, line):
                    score.instruction_count += 1

                    # Check for branches
                    for pattern in branch_patterns:
                        if re.search(pattern, line):
                            score.branch_count += 1
                            break

                    # Check for jumps
                    for pattern in jump_patterns:
                        if re.search(pattern, line):
                            score.jump_count += 1
                            break

                    # Check for stack allocation
                    stack_match = re.search(stack_pattern, line)
                    if stack_match:
                        score.stack_size = int(stack_match.group(1), 16)

    except Exception as e:
        print(f"Error analyzing {file_path}: {e}", file=sys.stderr)

    return score


def score_folder(folder_path: str) -> List[FunctionScore]:
    """Score all assembly functions in a folder."""

    if not os.path.isdir(folder_path):
        print(f"Error: '{folder_path}' is not a valid directory", file=sys.stderr)
        sys.exit(1)

    # Find all .s files
    asm_files = list(Path(folder_path).glob('*.s'))

    if not asm_files:
        print(f"Error: No .s files found in '{folder_path}'", file=sys.stderr)
        sys.exit(1)

    print(f"Analyzing {len(asm_files)} functions in {folder_path}...\n")

    # Score each function
    scores = []
    for asm_file in asm_files:
        score = analyze_function(str(asm_file))
        scores.append(score)

    # Sort by complexity (lowest first)
    scores.sort(key=lambda s: s.total_score)

    return scores


def load_difficult_functions(difficult_file: str) -> set:
    """Load the list of difficult functions to exclude."""
    difficult_functions = set()

    if os.path.exists(difficult_file):
        try:
            with open(difficult_file, 'r') as f:
                for line in f:
                    line = line.strip()
                    if line:  # Skip empty lines
                        difficult_functions.add(line)
        except Exception as e:
            print(f"Warning: Could not read {difficult_file}: {e}", file=sys.stderr)

    return difficult_functions


def main():
    if len(sys.argv) != 2:
        print("Usage: python3 tools/score_functions.py <folder_path>")
        print("Example: python3 tools/score_functions.py asm/nonmatchings/displaylist")
        sys.exit(1)

    folder_path = sys.argv[1]
    scores = score_folder(folder_path)

    # Load difficult functions to exclude
    script_dir = os.path.dirname(os.path.abspath(__file__))
    difficult_file = os.path.join(script_dir, 'difficult_functions')
    difficult_functions = load_difficult_functions(difficult_file)

    if difficult_functions:
        print(f"Excluding {len(difficult_functions)} difficult function(s)\n")

    # Filter out difficult functions
    filtered_scores = [s for s in scores if s.name not in difficult_functions]

    if not filtered_scores:
        print("Error: All functions are marked as difficult!", file=sys.stderr)
        sys.exit(1)

    simplest = filtered_scores[0]
    print(f"SIMPLEST FUNCTION: {simplest.name}")
    print(f"{simplest}")

if __name__ == '__main__':
    main()
