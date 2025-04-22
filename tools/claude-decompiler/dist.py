#!/usr/bin/env python3

import argparse
import difflib
import hashlib
import re
from collections import Counter
from typing import List, Optional, Sequence, Tuple

# Constants for penalties
PENALTY_INF = 10**9

PENALTY_STACKDIFF = 1
PENALTY_BRANCHDIFF = 1
PENALTY_REGALLOC = 5
PENALTY_REORDERING = 60
PENALTY_INSERTION = 100
PENALTY_DELETION = 100

def read_lines(path: str) -> List[str]:
    with open(path, 'r', encoding='utf-8') as f:
        return [line.strip() for line in f if line.strip()]

def field_matches_any_symbol(field: str) -> bool:
    # Simple check for symbol formats in common architectures
    return "." in field or re.fullmatch(r"^@\d+$", field) is not None

def score_files(target_lines: List[str], cand_lines: List[str], *, debug_mode: bool = False) -> Tuple[int, str]:
    difflib_differ = difflib.SequenceMatcher(a=[l.split()[0] for l in cand_lines], b=[l.split()[0] for l in target_lines], autojunk=False)

    num_stack_penalties = 0
    num_branch_penalties = 0
    num_regalloc_penalties = 0
    num_reordering_penalties = 0
    num_insertion_penalties = 0
    num_deletion_penalties = 0
    deletions: List[str] = []
    insertions: List[str] = []

    result_diff: Sequence[Tuple[str, int, int, int, int]] = difflib_differ.get_opcodes()

    equal_no_diff = set()

    def diff_sameline(old_line: str, new_line: str) -> bool:
        nonlocal num_stack_penalties, num_branch_penalties, num_regalloc_penalties

        old = old_line
        new = new_line

        if old == new:
            return False

        old_parts = old.split(None, 1)
        new_parts = new.split(None, 1)

        old_mnem = old_parts[0] if old_parts else ""
        new_mnem = new_parts[0] if new_parts else ""

        old_fields = old_parts[1].split(",") if len(old_parts) > 1 else []
        new_fields = new_parts[1].split(",") if len(new_parts) > 1 else []

        # Compare fields
        for nf, of in zip(new_fields, old_fields):
            if nf != of:
                if field_matches_any_symbol(nf):
                    continue
                num_regalloc_penalties += 1

        # Extra fields
        num_regalloc_penalties += abs(len(new_fields) - len(old_fields))
        return True

    def diff_insert(line: str) -> None:
        insertions.append(line)

    def diff_delete(line: str) -> None:
        deletions.append(line)

    for tag, i1, i2, j1, j2 in result_diff:
        if tag == "equal":
            for k in range(i2 - i1):
                old_line = target_lines[j1 + k]
                new_line = cand_lines[i1 + k]
                if not diff_sameline(old_line, new_line):
                    equal_no_diff.add(i1 + k)
        if tag in ("replace", "delete"):
            for k in range(i1, i2):
                diff_insert(cand_lines[k])
        if tag in ("replace", "insert"):
            for k in range(j1, j2):
                diff_delete(target_lines[k])

    # Post-process insertions and deletions
    insertions_co = Counter(insertions)
    deletions_co = Counter(deletions)
    for item in insertions_co + deletions_co:
        ins = insertions_co[item]
        dels = deletions_co[item]
        common = min(ins, dels)
        num_insertion_penalties += ins - common
        num_deletion_penalties += dels - common
        num_reordering_penalties += common

    final_score = (
        num_stack_penalties * PENALTY_STACKDIFF
        + num_branch_penalties * PENALTY_BRANCHDIFF
        + num_regalloc_penalties * PENALTY_REGALLOC
        + num_reordering_penalties * PENALTY_REORDERING
        + num_insertion_penalties * PENALTY_INSERTION
        + num_deletion_penalties * PENALTY_DELETION
    )

    if debug_mode:
        print("\nPenalty Breakdown:")
        print(f" Stack Differences: {num_stack_penalties} x {PENALTY_STACKDIFF}")
        print(f" Branch Differences: {num_branch_penalties} x {PENALTY_BRANCHDIFF}")
        print(f" Register Differences: {num_regalloc_penalties} x {PENALTY_REGALLOC}")
        print(f" Reorderings: {num_reordering_penalties} x {PENALTY_REORDERING}")
        print(f" Insertions: {num_insertion_penalties} x {PENALTY_INSERTION}")
        print(f" Deletions: {num_deletion_penalties} x {PENALTY_DELETION}")
        print(f" FINAL SCORE: {final_score}")

    joined_cand = "\n".join(cand_lines)
    return final_score, hashlib.sha256(joined_cand.encode()).hexdigest()

def main():
    parser = argparse.ArgumentParser(description="Score the difference between two assembly files.")
    parser.add_argument("target_file", help="Target assembly text file")
    parser.add_argument("cand_file", help="Candidate assembly text file")
    parser.add_argument("--debug", action="store_true", help="Print debug info")
    args = parser.parse_args()

    target_lines = read_lines(args.target_file)
    cand_lines = read_lines(args.cand_file)

    score, sha256_hash = score_files(target_lines, cand_lines, debug_mode=args.debug)

    print(f"Score: {score}")

if __name__ == "__main__":
    main()
