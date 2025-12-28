#!/usr/bin/env python3
"""
Find similar assembly functions to assist with decompilation.

Given an unmatched function, finds matched functions with similar patterns
that can serve as reference for decompilation.

Usage:
    python3 tools/find_similar_functions.py <function_name>
    python3 tools/find_similar_functions.py func_80014440_15040 --top 10
    python3 tools/find_similar_functions.py func_80014440_15040 --threshold 0.5
"""

import sys
import os
import re
import argparse
from pathlib import Path
from dataclasses import dataclass, field
from typing import List, Dict, Tuple, Optional, Set
from collections import Counter


@dataclass
class ParsedFunction:
    """Stores parsed assembly function data."""
    name: str
    file_path: str
    raw_instructions: List[str] = field(default_factory=list)
    normalized_instructions: List[str] = field(default_factory=list)

    # Structural metrics
    instruction_count: int = 0
    branch_count: int = 0
    jump_count: int = 0
    label_count: int = 0
    stack_size: int = 0

    # Extracted features
    control_flow_signature: str = ""
    data_access_offsets: List[int] = field(default_factory=list)
    called_functions: List[str] = field(default_factory=list)
    instruction_ngrams: Set[Tuple[str, ...]] = field(default_factory=set)


@dataclass
class SimilarityResult:
    """Stores similarity comparison results."""
    function: ParsedFunction
    total_score: float
    instruction_score: float
    control_flow_score: float
    data_access_score: float
    structural_score: float


# Regex patterns
INSTRUCTION_PATTERN = re.compile(r'/\*\s*[0-9A-Fa-f]+\s+[0-9A-Fa-f]+\s+[0-9A-Fa-f]+\s*\*/\s*(.+)')
LABEL_PATTERN = re.compile(r'^\s*\.L[0-9A-Fa-f_]+:')
GLABEL_PATTERN = re.compile(r'glabel\s+(\S+)')
BRANCH_PATTERN = re.compile(r'\b(beq|bne|bnez|beqz|blez|bgtz|bltz|bgez|blt|bgt|ble|bge|bltzal|bgezal)\b')
JUMP_PATTERN = re.compile(r'\b(jal|j)\b')
JAL_PATTERN = re.compile(r'\bjal\s+(\S+)')
STACK_PATTERN = re.compile(r'addiu\s+\$sp,\s*\$sp,\s*-0x([0-9A-Fa-f]+)')
MEMORY_ACCESS_PATTERN = re.compile(r'(-?0x[0-9A-Fa-f]+|-?\d+)\s*\(\s*\$\w+\s*\)')

# Register normalization mappings
REGISTER_CLASSES = {
    # Temporary registers -> $tN
    '$t0': '$tN', '$t1': '$tN', '$t2': '$tN', '$t3': '$tN',
    '$t4': '$tN', '$t5': '$tN', '$t6': '$tN', '$t7': '$tN',
    '$t8': '$tN', '$t9': '$tN',
    # Saved registers -> $sN
    '$s0': '$sN', '$s1': '$sN', '$s2': '$sN', '$s3': '$sN',
    '$s4': '$sN', '$s5': '$sN', '$s6': '$sN', '$s7': '$sN',
    # Argument registers -> $aN
    '$a0': '$aN', '$a1': '$aN', '$a2': '$aN', '$a3': '$aN',
    # Return value registers -> $vN
    '$v0': '$vN', '$v1': '$vN',
    # Keep special registers as-is
    '$sp': '$sp', '$fp': '$fp', '$ra': '$ra', '$zero': '$zero',
    '$gp': '$gp', '$at': '$at',
}


def normalize_instruction(instruction: str) -> str:
    """
    Normalize an instruction for comparison by abstracting away specific values.
    """
    normalized = instruction.strip()

    # Extract just the opcode and operands (remove comments)
    if '/*' in normalized:
        normalized = INSTRUCTION_PATTERN.match(normalized)
        if normalized:
            normalized = normalized.group(1).strip()
        else:
            return ""

    # Remove trailing comments
    if '#' in normalized:
        normalized = normalized.split('#')[0].strip()

    # Normalize registers
    for reg, replacement in REGISTER_CLASSES.items():
        normalized = re.sub(r'\b' + re.escape(reg) + r'\b', replacement, normalized)

    # Normalize addresses in %hi/%lo
    normalized = re.sub(r'%hi\([^)]+\)', '%hi(SYM)', normalized)
    normalized = re.sub(r'%lo\([^)]+\)', '%lo(SYM)', normalized)

    # Normalize immediate values (but keep memory offsets)
    # Replace large hex immediates with placeholder
    normalized = re.sub(r'\b0x[0-9A-Fa-f]{5,}\b', 'ADDR', normalized)

    # Normalize branch/jump targets
    normalized = re.sub(r'\.L[0-9A-Fa-f_]+', '.LABEL', normalized)

    # Normalize function call targets (but keep for separate analysis)
    normalized = re.sub(r'\bjal\s+\S+', 'jal FUNC', normalized)

    return normalized


def extract_opcode(instruction: str) -> str:
    """Extract just the opcode from an instruction."""
    match = INSTRUCTION_PATTERN.match(instruction)
    if match:
        parts = match.group(1).strip().split()
        if parts:
            return parts[0]
    return ""


def parse_function_content(name: str, content: str, file_path: str) -> Optional[ParsedFunction]:
    """Parse function content and extract features."""
    func = ParsedFunction(name=name, file_path=file_path)

    lines = content.split('\n')
    opcodes = []

    for line in lines:
        line = line.strip()

        if not line or line.startswith('glabel') or line.startswith('endlabel') or line.startswith('nonmatching'):
            continue

        # Count labels
        if LABEL_PATTERN.match(line):
            func.label_count += 1
            opcodes.append('LABEL')
            continue

        # Process instructions
        instr_match = INSTRUCTION_PATTERN.match(line)
        if instr_match:
            instr = instr_match.group(1).strip()
            func.instruction_count += 1
            func.raw_instructions.append(instr)

            normalized = normalize_instruction(line)
            func.normalized_instructions.append(normalized)

            opcode = extract_opcode(line)
            if opcode:
                opcodes.append(opcode)

            # Count branches
            if BRANCH_PATTERN.search(instr):
                func.branch_count += 1

            # Count jumps and extract called functions
            if JUMP_PATTERN.search(instr):
                func.jump_count += 1
                jal_match = JAL_PATTERN.search(instr)
                if jal_match:
                    func.called_functions.append(jal_match.group(1))

            # Extract stack size
            stack_match = STACK_PATTERN.search(instr)
            if stack_match:
                func.stack_size = int(stack_match.group(1), 16)

            # Extract memory access offsets
            for mem_match in MEMORY_ACCESS_PATTERN.finditer(instr):
                offset_str = mem_match.group(1)
                try:
                    if offset_str.startswith('0x') or offset_str.startswith('-0x'):
                        offset = int(offset_str, 16)
                    else:
                        offset = int(offset_str)
                    func.data_access_offsets.append(offset)
                except ValueError:
                    pass

    # Build control flow signature (sequence of branch/jump opcodes)
    cf_opcodes = []
    for op in opcodes:
        if op in ('beq', 'bne', 'bnez', 'beqz', 'blez', 'bgtz', 'bltz', 'bgez',
                  'blt', 'bgt', 'ble', 'bge', 'j', 'jal', 'jr', 'LABEL'):
            cf_opcodes.append(op)
    func.control_flow_signature = ' '.join(cf_opcodes)

    # Build instruction n-grams (trigrams)
    if len(func.normalized_instructions) >= 3:
        for i in range(len(func.normalized_instructions) - 2):
            ngram = tuple(func.normalized_instructions[i:i+3])
            func.instruction_ngrams.add(ngram)

    return func


def parse_asm_file(file_path: str) -> List[ParsedFunction]:
    """Parse an assembly file and return all functions."""
    functions = []

    try:
        with open(file_path, 'r') as f:
            content = f.read()

        # Find all function blocks
        glabels = list(re.finditer(GLABEL_PATTERN, content))

        if not glabels:
            return functions

        for i, match in enumerate(glabels):
            func_name = match.group(1)
            start_pos = match.start()

            # Skip data/jump table entries
            if func_name.startswith('D_') or func_name.startswith('jtbl_'):
                continue

            end_pos = glabels[i + 1].start() if i + 1 < len(glabels) else len(content)
            func_content = content[start_pos:end_pos]

            func = parse_function_content(func_name, func_content, file_path)
            if func and func.instruction_count > 0:
                functions.append(func)

    except Exception as e:
        print(f"Error parsing {file_path}: {e}", file=sys.stderr)

    return functions


def find_function(name: str, search_dirs: List[str]) -> Optional[ParsedFunction]:
    """Find and parse a specific function by name."""
    for search_dir in search_dirs:
        for asm_file in Path(search_dir).glob('**/*.s'):
            functions = parse_asm_file(str(asm_file))
            for func in functions:
                if func.name == name:
                    return func
    return None


def build_function_index(search_dir: str) -> List[ParsedFunction]:
    """Build an index of all functions in a directory."""
    functions = []
    for asm_file in Path(search_dir).glob('**/*.s'):
        functions.extend(parse_asm_file(str(asm_file)))
    return functions


def jaccard_similarity(set_a: set, set_b: set) -> float:
    """Calculate Jaccard similarity between two sets."""
    if not set_a and not set_b:
        return 1.0
    if not set_a or not set_b:
        return 0.0
    intersection = len(set_a & set_b)
    union = len(set_a | set_b)
    return intersection / union if union > 0 else 0.0


def levenshtein_ratio(seq_a: List[str], seq_b: List[str]) -> float:
    """Calculate normalized Levenshtein similarity (1 - normalized distance)."""
    if not seq_a and not seq_b:
        return 1.0
    if not seq_a or not seq_b:
        return 0.0

    len_a, len_b = len(seq_a), len(seq_b)

    # Use dynamic programming for edit distance
    # Optimize for memory by using only two rows
    prev_row = list(range(len_b + 1))
    curr_row = [0] * (len_b + 1)

    for i in range(1, len_a + 1):
        curr_row[0] = i
        for j in range(1, len_b + 1):
            cost = 0 if seq_a[i-1] == seq_b[j-1] else 1
            curr_row[j] = min(
                prev_row[j] + 1,      # deletion
                curr_row[j-1] + 1,    # insertion
                prev_row[j-1] + cost  # substitution
            )
        prev_row, curr_row = curr_row, prev_row

    distance = prev_row[len_b]
    max_len = max(len_a, len_b)
    return 1.0 - (distance / max_len)


def instruction_similarity(func_a: ParsedFunction, func_b: ParsedFunction) -> float:
    """Calculate instruction sequence similarity."""
    # Use n-gram Jaccard for efficiency on larger functions
    if func_a.instruction_ngrams and func_b.instruction_ngrams:
        ngram_sim = jaccard_similarity(func_a.instruction_ngrams, func_b.instruction_ngrams)
    else:
        ngram_sim = 0.0

    # For smaller functions, also use edit distance
    if len(func_a.normalized_instructions) < 50 and len(func_b.normalized_instructions) < 50:
        edit_sim = levenshtein_ratio(func_a.normalized_instructions, func_b.normalized_instructions)
        return (ngram_sim + edit_sim) / 2

    return ngram_sim


def control_flow_similarity(func_a: ParsedFunction, func_b: ParsedFunction) -> float:
    """Calculate control flow similarity."""
    # Compare control flow signatures using edit distance
    sig_a = func_a.control_flow_signature.split()
    sig_b = func_b.control_flow_signature.split()

    if not sig_a and not sig_b:
        return 1.0

    sig_sim = levenshtein_ratio(sig_a, sig_b)

    # Also compare branch/label ratios
    ratio_a = func_a.branch_count / max(func_a.instruction_count, 1)
    ratio_b = func_b.branch_count / max(func_b.instruction_count, 1)
    ratio_diff = abs(ratio_a - ratio_b)
    ratio_sim = max(0, 1.0 - ratio_diff * 5)  # Scale difference

    return (sig_sim * 0.7 + ratio_sim * 0.3)


def data_access_similarity(func_a: ParsedFunction, func_b: ParsedFunction) -> float:
    """Calculate data access pattern similarity."""
    offsets_a = set(func_a.data_access_offsets)
    offsets_b = set(func_b.data_access_offsets)

    if not offsets_a and not offsets_b:
        return 1.0

    # Jaccard on exact offset matches
    exact_sim = jaccard_similarity(offsets_a, offsets_b)

    # Also check for similar offset patterns (e.g., sequential struct access)
    # Normalize offsets to relative positions
    if offsets_a and offsets_b:
        sorted_a = sorted(offsets_a)
        sorted_b = sorted(offsets_b)

        # Compare offset deltas (pattern of struct field spacing)
        deltas_a = set(sorted_a[i+1] - sorted_a[i] for i in range(len(sorted_a)-1))
        deltas_b = set(sorted_b[i+1] - sorted_b[i] for i in range(len(sorted_b)-1))

        delta_sim = jaccard_similarity(deltas_a, deltas_b)

        return (exact_sim * 0.6 + delta_sim * 0.4)

    return exact_sim


def structural_similarity(func_a: ParsedFunction, func_b: ParsedFunction) -> float:
    """Calculate structural similarity based on metrics."""
    # Compare instruction counts
    instr_ratio = min(func_a.instruction_count, func_b.instruction_count) / max(func_a.instruction_count, func_b.instruction_count, 1)

    # Compare branch counts
    max_branch = max(func_a.branch_count, func_b.branch_count)
    branch_sim = 1.0 - abs(func_a.branch_count - func_b.branch_count) / max(max_branch, 1)

    # Compare jump counts
    max_jump = max(func_a.jump_count, func_b.jump_count)
    jump_sim = 1.0 - abs(func_a.jump_count - func_b.jump_count) / max(max_jump, 1)

    # Compare stack sizes
    if func_a.stack_size == func_b.stack_size:
        stack_sim = 1.0
    elif func_a.stack_size == 0 or func_b.stack_size == 0:
        stack_sim = 0.5
    else:
        stack_sim = min(func_a.stack_size, func_b.stack_size) / max(func_a.stack_size, func_b.stack_size)

    return (instr_ratio * 0.3 + branch_sim * 0.25 + jump_sim * 0.25 + stack_sim * 0.2)


def calculate_similarity(query: ParsedFunction, candidate: ParsedFunction) -> SimilarityResult:
    """Calculate overall similarity between two functions."""
    instr_sim = instruction_similarity(query, candidate)
    cf_sim = control_flow_similarity(query, candidate)
    data_sim = data_access_similarity(query, candidate)
    struct_sim = structural_similarity(query, candidate)

    # Weighted combination
    total = (
        instr_sim * 0.35 +
        cf_sim * 0.25 +
        data_sim * 0.20 +
        struct_sim * 0.20
    )

    return SimilarityResult(
        function=candidate,
        total_score=total,
        instruction_score=instr_sim,
        control_flow_score=cf_sim,
        data_access_score=data_sim,
        structural_score=struct_sim
    )


def find_similar_functions(query: ParsedFunction, candidates: List[ParsedFunction],
                          top_n: int = 10, threshold: float = 0.0) -> List[SimilarityResult]:
    """Find the most similar functions to the query."""
    results = []

    for candidate in candidates:
        if candidate.name == query.name:
            continue

        result = calculate_similarity(query, candidate)
        if result.total_score >= threshold:
            results.append(result)

    # Sort by total score descending
    results.sort(key=lambda r: r.total_score, reverse=True)

    return results[:top_n]


def get_c_source_for_function(func_name: str, project_root: str) -> Optional[str]:
    """Try to find the C source file containing a matched function."""
    # Search for the function name in src/ directory
    src_dir = os.path.join(project_root, 'src')
    if not os.path.isdir(src_dir):
        return None

    for c_file in Path(src_dir).glob('**/*.c'):
        try:
            with open(c_file, 'r') as f:
                content = f.read()
                if func_name in content:
                    return str(c_file)
        except:
            pass

    return None


# --- Programmatic API for use by other scripts ---

_cached_matchings_index: Optional[List[ParsedFunction]] = None
_cached_project_root: Optional[str] = None


def get_project_root() -> str:
    """Get the project root directory."""
    script_dir = os.path.dirname(os.path.abspath(__file__))
    return os.path.dirname(script_dir)


def get_matchings_index(project_root: Optional[str] = None) -> List[ParsedFunction]:
    """Get or build the cached index of matched functions."""
    global _cached_matchings_index, _cached_project_root

    if project_root is None:
        project_root = get_project_root()

    # Return cached index if available for same project
    if _cached_matchings_index is not None and _cached_project_root == project_root:
        return _cached_matchings_index

    matchings_dir = os.path.join(project_root, 'asm', 'matchings')
    _cached_matchings_index = build_function_index(matchings_dir)
    _cached_project_root = project_root

    return _cached_matchings_index


def get_best_match_for_function(func_name: str, project_root: Optional[str] = None) -> Optional[Tuple[str, float]]:
    """
    Find the best matching function for a given function name.

    Args:
        func_name: Name of the function to find matches for
        project_root: Optional project root path

    Returns:
        Tuple of (best_match_name, similarity_score) or None if no match found
    """
    if project_root is None:
        project_root = get_project_root()

    nonmatchings_dir = os.path.join(project_root, 'asm', 'nonmatchings')
    matchings_dir = os.path.join(project_root, 'asm', 'matchings')

    # Find the query function
    query = find_function(func_name, [nonmatchings_dir, matchings_dir])
    if not query:
        return None

    # Get matchings index
    candidates = get_matchings_index(project_root)
    if not candidates:
        return None

    # Find similar functions
    results = find_similar_functions(query, candidates, top_n=1, threshold=0.0)

    if results:
        return (results[0].function.name, results[0].total_score)

    return None


def get_best_match_for_parsed_function(query: ParsedFunction, project_root: Optional[str] = None) -> Optional[Tuple[str, float]]:
    """
    Find the best matching function for an already-parsed function.

    Args:
        query: ParsedFunction object to find matches for
        project_root: Optional project root path

    Returns:
        Tuple of (best_match_name, similarity_score) or None if no match found
    """
    if project_root is None:
        project_root = get_project_root()

    # Get matchings index
    candidates = get_matchings_index(project_root)
    if not candidates:
        return None

    # Find similar functions
    results = find_similar_functions(query, candidates, top_n=1, threshold=0.0)

    if results:
        return (results[0].function.name, results[0].total_score)

    return None


def main():
    parser = argparse.ArgumentParser(
        description="Find similar assembly functions to assist with decompilation.",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
    python3 tools/find_similar_functions.py func_8000FED0_10AD0
    python3 tools/find_similar_functions.py func_8000FED0_10AD0 --top 5
    python3 tools/find_similar_functions.py func_8000FED0_10AD0 --threshold 0.5
        """
    )
    parser.add_argument(
        'function_name',
        help='Name of the function to find similar matches for'
    )
    parser.add_argument(
        '--top', '-n',
        type=int,
        default=10,
        help='Number of top matches to show (default: 10)'
    )
    parser.add_argument(
        '--threshold', '-t',
        type=float,
        default=0.0,
        help='Minimum similarity threshold (0.0-1.0, default: 0.0)'
    )
    parser.add_argument(
        '--verbose', '-v',
        action='store_true',
        help='Show detailed similarity breakdown'
    )

    args = parser.parse_args()

    # Determine project root
    script_dir = os.path.dirname(os.path.abspath(__file__))
    project_root = os.path.dirname(script_dir)

    nonmatchings_dir = os.path.join(project_root, 'asm', 'nonmatchings')
    matchings_dir = os.path.join(project_root, 'asm', 'matchings')

    # Find the query function
    print(f"Searching for function: {args.function_name}...", file=sys.stderr)
    query = find_function(args.function_name, [nonmatchings_dir, matchings_dir])

    if not query:
        print(f"Error: Function '{args.function_name}' not found", file=sys.stderr)
        sys.exit(1)

    print(f"Found query function with {query.instruction_count} instructions", file=sys.stderr)

    # Build index of matching functions
    print(f"Indexing matched functions...", file=sys.stderr)
    candidates = build_function_index(matchings_dir)
    print(f"Indexed {len(candidates)} functions", file=sys.stderr)

    if not candidates:
        print("Error: No matched functions found to compare against", file=sys.stderr)
        sys.exit(1)

    # Find similar functions
    print(f"Calculating similarities...\n", file=sys.stderr)
    results = find_similar_functions(query, candidates, top_n=args.top, threshold=args.threshold)

    if not results:
        print("No similar functions found above threshold")
        sys.exit(0)

    # Display results
    print(f"Similar functions to {query.name}:\n")

    for i, result in enumerate(results, 1):
        func = result.function
        print(f"{i}. {func.name} (score: {result.total_score:.3f})")

        if args.verbose:
            print(f"   Instruction similarity: {result.instruction_score:.3f}")
            print(f"   Control flow:           {result.control_flow_score:.3f}")
            print(f"   Data access:            {result.data_access_score:.3f}")
            print(f"   Structural:             {result.structural_score:.3f}")

        print(f"   File: {func.file_path}")

        c_source = get_c_source_for_function(func.name, project_root)
        if c_source:
            print(f"   C source: {c_source}")

        print()


if __name__ == '__main__':
    main()
