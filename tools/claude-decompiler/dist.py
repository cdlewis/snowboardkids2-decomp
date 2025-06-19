#!/usr/bin/env python3

# dist.py takes a base dump and a target dump, computing the distance between them.
# usage: python3 dist.py base_dump target_dump

import argparse
import difflib
import hashlib
import re
from collections import Counter
from typing import Dict, List, Optional, Sequence, Set, Tuple

# Constants for penalties - copied from decomp-permuter's scorer.py
PENALTY_INF = 10**9

PENALTY_STACKDIFF = 1
PENALTY_BRANCHDIFF = 1
PENALTY_REGALLOC = 5
PENALTY_REORDERING = 60
PENALTY_INSERTION = 100
PENALTY_DELETION = 100

# Architecture-specific patterns
# MIPS architecture patterns for the MIPS N64
class MipsArchitecture:
    name = "mips"
    # Regular expression for recognizing stack pointer offsets (used for stack_differences)
    re_sprel = r"(?:\d+\(|\$sp\s*,\s*)(-?(?:0x)?[0-9a-fA-F]+)\(?(?:\$sp)?\)?"
    # Lists of branch instructions
    branch_instructions = [
        "beq", "bne", "blez", "bgtz", "bltz", "bgez", 
        "beqz", "bnez", "bgezal", "bltzal"
    ]
    branch_likely_instructions = [
        "beql", "bnel", "blezl", "bgtzl", "bltzl", "bgezl",
        "beqzl", "bnezl", "bgezall", "bltzall"
    ]

def read_lines(path: str) -> List[str]:
    with open(path, 'r', encoding='utf-8') as f:
        return [line.strip() for line in f if line.strip()]

def read_object_file(path: str, preserve_offsets: bool = True) -> List[str]:
    """Read an object file using objdump"""
    import sys
    import os
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    from objdump import objdump
    lines = objdump(path)
    return lines

class Line:
    """Class to represent a disassembled instruction line with additional metadata"""
    def __init__(self, row: str, has_symbol: bool = False):
        self.row = row
        parts = row.split(None, 1)
        self.mnemonic = parts[0] if parts else ""
        self.has_symbol = has_symbol or self._detect_symbol(row)
    
    def _detect_symbol(self, row: str) -> bool:
        # Simple heuristic to detect if line contains a symbol reference
        return ".text" in row or ".data" in row or ".rodata" in row

def field_matches_any_symbol(field: str, arch: MipsArchitecture) -> bool:
    """Matches symbols pattern from the permuter's scorer function"""
    if arch.name == "mips":
        return "." in field
    return False

def score_files(target_lines: List[str], cand_lines: List[str], *, debug_mode: bool = False) -> Tuple[int, str, float]:
    """Score the differences between target and candidate assembly lines using the permuter's algorithm"""
    # Create Line objects
    target_seq = [Line(line) for line in target_lines]
    cand_seq = [Line(line) for line in cand_lines]
    
    # Use MIPS architecture for this project
    arch = MipsArchitecture()
    
    # For diff matching based on mnemonics
    difflib_differ = difflib.SequenceMatcher(
        a=[line.mnemonic for line in cand_seq], 
        b=[line.mnemonic for line in target_seq], 
        autojunk=False
    )

    num_stack_penalties = 0
    num_branch_penalties = 0
    num_regalloc_penalties = 0
    num_reordering_penalties = 0
    num_insertion_penalties = 0
    num_deletion_penalties = 0
    deletions: List[str] = []
    insertions: List[str] = []

    result_diff: Sequence[Tuple[str, int, int, int, int]] = difflib_differ.get_opcodes()

    equal_no_diff: Set[int] = set()
    total_equal_lines = 0

    def diff_sameline(old_line: Line, new_line: Line) -> bool:
        """Compare two lines with the same mnemonic using the permuter's algorithm"""
        nonlocal num_stack_penalties
        nonlocal num_branch_penalties
        nonlocal num_regalloc_penalties

        old_num_stack_penalties = num_stack_penalties
        old_num_branch_penalties = num_branch_penalties
        old_num_regalloc_penalties = num_regalloc_penalties

        old = old_line.row
        new = new_line.row

        if old == new:
            return False

        # Check for stack pointer differences
        ignore_last_field = False
        oldsp = re.search(arch.re_sprel, old)
        newsp = re.search(arch.re_sprel, new)
        if oldsp and newsp:
            oldrel = int(oldsp.group(1) or "0", 16)
            newrel = int(newsp.group(1) or "0", 16)
            num_stack_penalties += abs(oldrel - newrel)
            ignore_last_field = True

        # Check for branch target differences
        b_instr = arch.branch_instructions
        bl_instr = arch.branch_likely_instructions

        if (
            old_line.mnemonic == new_line.mnemonic
            and (old_line.mnemonic in b_instr or old_line.mnemonic in bl_instr)
            and old_line.row != new_line.row
            and not old_line.has_symbol
            and not new_line.has_symbol
        ):
            old_target = old.split(",")[-1] if "," in old else ""
            new_target = new.split(",")[-1] if "," in new else ""
            if old_target != new_target:
                num_branch_penalties += 1
                ignore_last_field = True

        # Parse fields for comparison
        old_parts = old.split(None, 1)
        new_parts = new.split(None, 1)
        oldfields = old_parts[1].split(",") if len(old_parts) > 1 else []
        newfields = new_parts[1].split(",") if len(new_parts) > 1 else []
        
        if ignore_last_field:
            oldfields = oldfields[:-1]
            newfields = newfields[:-1]
        else:
            # Handle parenthesized fields like in permuter (e.g., "0x38(r7)")
            re_paren = re.compile(r"(?<!%hi)(?<!%lo)\(")
            oldfields = oldfields[:-1] + (
                re_paren.split(oldfields[-1]) if len(oldfields) > 0 else []
            )
            newfields = newfields[:-1] + (
                re_paren.split(newfields[-1]) if len(newfields) > 0 else []
            )

        # Compare fields
        for nf, of in zip(newfields, oldfields):
            if nf != of:
                # If new field matches a symbol pattern and old line has a symbol, ignore this mismatch
                if field_matches_any_symbol(nf, arch) and old_line.has_symbol:
                    continue
                num_regalloc_penalties += 1

        # Penalize any extra fields
        num_regalloc_penalties += abs(len(newfields) - len(oldfields))

        return (
            old_num_regalloc_penalties != num_regalloc_penalties
            or old_num_stack_penalties != num_stack_penalties
            or old_num_branch_penalties != num_branch_penalties
        )

    def diff_insert(line: str) -> None:
        insertions.append(line)

    def diff_delete(line: str) -> None:
        deletions.append(line)

    # Process the diff opcodes
    for tag, i1, i2, j1, j2 in result_diff:
        if tag == "equal":
            for k in range(i2 - i1):
                old_line = target_seq[j1 + k]
                new_line = cand_seq[i1 + k]
                if not diff_sameline(old_line, new_line):
                    equal_no_diff.add(i1 + k)
            total_equal_lines += (i2 - i1)
        if tag in ("replace", "delete"):
            for k in range(i1, i2):
                diff_insert(cand_seq[k].row)
        if tag in ("replace", "insert"):
            for k in range(j1, j2):
                diff_delete(target_seq[k].row)

    # Post-process insertions and deletions to identify reordering
    insertions_co = Counter(insertions)
    deletions_co = Counter(deletions)
    for item in insertions_co + deletions_co:
        ins = insertions_co[item]
        dels = deletions_co[item]
        common = min(ins, dels)
        num_insertion_penalties += ins - common
        num_deletion_penalties += dels - common
        num_reordering_penalties += common

    # Calculate the final score using penalties
    final_score = (
        num_stack_penalties * PENALTY_STACKDIFF
        + num_branch_penalties * PENALTY_BRANCHDIFF
        + num_regalloc_penalties * PENALTY_REGALLOC
        + num_reordering_penalties * PENALTY_REORDERING
        + num_insertion_penalties * PENALTY_INSERTION
        + num_deletion_penalties * PENALTY_DELETION
    )

    # Calculate percentage of lines matched
    total_lines = max(len(target_lines), len(cand_lines))
    if total_lines > 0:
        lines_matched_percentage = (total_equal_lines / total_lines) * 100
    else:
        lines_matched_percentage = 0
        
    # Calculate match score as a percentage (100% means no differences)
    match_percentage = 100.0 if final_score == 0 else max(0, 100.0 - (final_score / total_lines))

    if debug_mode:
        print("\nPenalty Breakdown:")
        print(f" Stack Differences: {num_stack_penalties} x {PENALTY_STACKDIFF}")
        print(f" Branch Differences: {num_branch_penalties} x {PENALTY_BRANCHDIFF}")
        print(f" Register Differences: {num_regalloc_penalties} x {PENALTY_REGALLOC}")
        print(f" Reorderings: {num_reordering_penalties} x {PENALTY_REORDERING}")
        print(f" Insertions: {num_insertion_penalties} x {PENALTY_INSERTION}")
        print(f" Deletions: {num_deletion_penalties} x {PENALTY_DELETION}")
        print(f" Lines Matched: {total_equal_lines}/{total_lines} ({match_percentage:.1f}%)")
        print(f" FINAL SCORE: {final_score}")

    joined_cand = "\n".join(c.row for c in cand_seq)
    return final_score, hashlib.sha256(joined_cand.encode()).hexdigest(), match_percentage

def main():
    parser = argparse.ArgumentParser(description="Score the difference between two assembly files.")
    parser.add_argument("target_file", help="Target assembly text file or object file")
    parser.add_argument("cand_file", help="Candidate assembly text file or object file")
    parser.add_argument("--debug", action="store_true", help="Print debug info")
    parser.add_argument("--stack-diffs", action="store_true", help="Preserve stack differences (for .o files)")
    args = parser.parse_args()

    # Check if we're dealing with object files
    if args.target_file.endswith('.o') and args.cand_file.endswith('.o'):
        # Use objdump to read object files
        target_lines = read_object_file(args.target_file, preserve_offsets=args.stack_diffs)
        cand_lines = read_object_file(args.cand_file, preserve_offsets=args.stack_diffs)
    else:
        # Regular text file processing
        target_lines = read_lines(args.target_file)
        cand_lines = read_lines(args.cand_file)

    score, sha256_hash, match_percentage = score_files(target_lines, cand_lines, debug_mode=args.debug)
    
    # Use the raw score directly as the differences value
    print(f"Score: {match_percentage:.3f}% ({score} differences)")

if __name__ == "__main__":
    main()
