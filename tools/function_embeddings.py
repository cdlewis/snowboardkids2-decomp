#!/usr/bin/env python3
"""
Generate 2D scatter plot data showing functions projected into vector space using UMAP.

This script scans asm/matchings/ and asm/nonmatchings/ directories, extracts features
from assembly functions, and applies UMAP dimensionality reduction to create 2D coordinates
suitable for visualization.

Usage:
    python3 tools/function_embeddings.py
    python3 tools/function_embeddings.py --format csv
    python3 tools/function_embeddings.py --output embeddings.json
    python3 tools/function_embeddings.py --plot
    python3 tools/function_embeddings.py --plot --plot-output plot.png
    python3 tools/function_embeddings.py --excalidraw-output embeddings.excalidraw
"""

import sys
import os
import re
import json
import argparse
import hashlib
from pathlib import Path
from dataclasses import dataclass, field
from typing import List, Dict, Set, Tuple, Optional
from collections import Counter


# =============================================================================
# Reuse ParsedFunction and parsing logic from find_similar_functions.py
# =============================================================================

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
    '$t0': '$tN', '$t1': '$tN', '$t2': '$tN', '$t3': '$tN',
    '$t4': '$tN', '$t5': '$tN', '$t6': '$tN', '$t7': '$tN',
    '$t8': '$tN', '$t9': '$tN',
    '$s0': '$sN', '$s1': '$sN', '$s2': '$sN', '$s3': '$sN',
    '$s4': '$sN', '$s5': '$sN', '$s6': '$sN', '$s7': '$sN',
    '$a0': '$aN', '$a1': '$aN', '$a2': '$aN', '$a3': '$aN',
    '$v0': '$vN', '$v1': '$vN',
    '$sp': '$sp', '$fp': '$fp', '$ra': '$ra', '$zero': '$zero',
    '$gp': '$gp', '$at': '$at',
}


def normalize_instruction(instruction: str) -> str:
    """Normalize an instruction for comparison by abstracting away specific values."""
    normalized = instruction.strip()

    if '/*' in normalized:
        normalized = INSTRUCTION_PATTERN.match(normalized)
        if normalized:
            normalized = normalized.group(1).strip()
        else:
            return ""

    if '#' in normalized:
        normalized = normalized.split('#')[0].strip()

    for reg, replacement in REGISTER_CLASSES.items():
        normalized = re.sub(r'\b' + re.escape(reg) + r'\b', replacement, normalized)

    normalized = re.sub(r'%hi\([^)]+\)', '%hi(SYM)', normalized)
    normalized = re.sub(r'%lo\([^)]+\)', '%lo(SYM)', normalized)
    normalized = re.sub(r'\b0x[0-9A-Fa-f]{5,}\b', 'ADDR', normalized)
    normalized = re.sub(r'\.L[0-9A-Fa-f_]+', '.LABEL', normalized)
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

        if LABEL_PATTERN.match(line):
            func.label_count += 1
            opcodes.append('LABEL')
            continue

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

            if BRANCH_PATTERN.search(instr):
                func.branch_count += 1

            if JUMP_PATTERN.search(instr):
                func.jump_count += 1
                jal_match = JAL_PATTERN.search(instr)
                if jal_match:
                    func.called_functions.append(jal_match.group(1))

            stack_match = STACK_PATTERN.search(instr)
            if stack_match:
                func.stack_size = int(stack_match.group(1), 16)

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

    # Build control flow signature
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


def build_function_index(search_dir: str) -> List[ParsedFunction]:
    """Build an index of all functions in a directory."""
    functions = []
    for asm_file in Path(search_dir).glob('**/*.s'):
        functions.extend(parse_asm_file(str(asm_file)))
    return functions


# =============================================================================
# Feature extraction for embeddings
# =============================================================================

# All possible MIPS opcodes for one-hot encoding
ALL_OPCODES = set([
    'nop', 'add', 'addu', 'sub', 'subu', 'addi', 'addiu', 'mul', 'mult', 'multu',
    'div', 'divu', 'and', 'or', 'xor', 'nor', 'andi', 'ori', 'xori',
    'sll', 'srl', 'sra', 'sllv', 'srlv', 'srav', 'lui',
    'lw', 'lh', 'lhu', 'lb', 'lbu', 'sw', 'sh', 'sb',
    'beq', 'bne', 'blez', 'bgtz', 'bltz', 'bgez', 'bnel', 'beql', 'blezl', 'bgtzl',
    'bltzl', 'bgezl', 'bltzal', 'bgezal', 'bltzall', 'bgezall',
    'j', 'jal', 'jalr', 'jr',
    'slt', 'sltu', 'slti', 'sltiu',
    'mfhi', 'mthi', 'mflo', 'mtlo',
    'cop0', 'cop1', 'cop2', 'lwc1', 'swc1', 'mtc1', 'mfc1',
    'ctc1', 'cfc1', 'bc1t', 'bc1f', 'bc1tl', 'bc1fl',
    'add.s', 'sub.s', 'mul.s', 'div.s', 'sqrt.s', 'abs.s', 'mov.s', 'neg.s',
    'cvt.s.w', 'cvt.w.s', 'cvt.d.s', 'cvt.s.d',
    'lwc2', 'swc2',
    'sync', 'syscall', 'break',
])


def extract_opcode_frequencies(func: ParsedFunction, all_opcodes: Set[str]) -> Dict[str, float]:
    """Extract normalized opcode frequencies from a function."""
    opcodes = []
    for instr in func.raw_instructions:
        parts = instr.strip().split()
        if parts:
            opcode = parts[0].lower()
            # Normalize some pseudo-opcodes
            if opcode in ('move', 'not'):
                # These are typically aliases
                continue
            opcodes.append(opcode)

    if not opcodes:
        return {op: 0.0 for op in all_opcodes}

    total = len(opcodes)
    counter = Counter(opcodes)

    # Normalize to frequencies
    return {op: counter.get(op, 0) / total for op in all_opcodes}


def extract_control_flow_features(func: ParsedFunction) -> List[float]:
    """Extract control flow pattern features."""
    if func.instruction_count == 0:
        return [0.0] * 10

    # Ratios and structural properties
    branch_ratio = func.branch_count / func.instruction_count
    jump_ratio = func.jump_count / func.instruction_count
    label_ratio = func.label_count / func.instruction_count
    called_func_count = len(func.called_functions)

    # Control flow sequence statistics
    cf_tokens = func.control_flow_signature.split()
    cf_density = len(cf_tokens) / func.instruction_count if func.instruction_count > 0 else 0

    # Data access patterns
    unique_offsets = len(set(func.data_access_offsets))
    data_access_count = len(func.data_access_offsets)
    data_access_ratio = data_access_count / func.instruction_count if func.instruction_count > 0 else 0

    # Stack frame indicator
    has_stack = 1.0 if func.stack_size > 0 else 0.0
    stack_normalized = func.stack_size / 256.0  # Normalize by typical max stack

    # Code density (instructions vs labels)
    code_density = func.instruction_count / (func.instruction_count + func.label_count) if (func.instruction_count + func.label_count) > 0 else 1.0

    return [
        branch_ratio,
        jump_ratio,
        label_ratio,
        cf_density,
        min(called_func_count / 10.0, 1.0),  # Cap at 1.0
        data_access_ratio,
        min(unique_offsets / 20.0, 1.0),  # Cap at 1.0
        has_stack,
        min(stack_normalized, 1.0),
        code_density,
    ]


def get_basic_opcode_features(func: ParsedFunction) -> List[float]:
    """Get basic numerical features from a function."""
    return [
        func.instruction_count,
        func.branch_count,
        func.jump_count,
        func.label_count,
        min(func.stack_size / 10.0, 100.0),  # Normalized stack size
        len(func.called_functions),
        len(func.data_access_offsets),
        len(set(func.data_access_offsets)),  # Unique offsets
    ]


def function_to_feature_vector(
    func: ParsedFunction,
    all_opcodes: Set[str],
    ngram_vocabulary: Optional[Dict[str, int]] = None
) -> List[float]:
    """
    Convert a ParsedFunction to a numerical feature vector.

    Features include:
    - Basic metrics (instruction count, branches, jumps, etc.)
    - Opcode frequencies (one-hot encoded)
    - Control flow pattern features
    - Instruction n-gram embeddings (if vocabulary provided)
    """
    features = []

    # 1. Basic numerical features
    features.extend(get_basic_opcode_features(func))

    # 2. Opcode frequencies
    opcode_freqs = extract_opcode_frequencies(func, all_opcodes)
    features.extend(opcode_freqs[op] for op in sorted(all_opcodes))

    # 3. Control flow features
    features.extend(extract_control_flow_features(func))

    # 4. N-gram features (optional, if vocabulary provided)
    if ngram_vocabulary:
        ngram_features = extract_ngram_features(func, ngram_vocabulary)
        features.extend(ngram_features)

    return features


def build_ngram_vocabulary(functions: List[ParsedFunction], max_ngrams: int = 500) -> Dict[str, int]:
    """
    Build a vocabulary of the most common instruction n-grams using TF-IDF.

    Returns a mapping from n-gram string to index in feature vector.
    """
    from sklearn.feature_extraction.text import TfidfVectorizer

    # Convert n-grams to strings for TF-IDF
    ngram_documents = []
    for func in functions:
        # Convert n-gram tuples to strings
        ngrams = [' '.join(ngram) for ngram in func.instruction_ngrams]
        ngram_documents.append(' '.join(ngrams))

    # Use TF-IDF to find important n-grams
    vectorizer = TfidfVectorizer(
        token_pattern=r'\S+',
        max_features=max_ngrams,
        norm='l2'
    )

    try:
        vectorizer.fit(ngram_documents)
        return {vocab: idx for idx, vocab in enumerate(vectorizer.vocabulary_)}
    except Exception:
        # If TF-IDF fails, return empty vocab
        return {}


def extract_ngram_features(func: ParsedFunction, vocabulary: Dict[str, int], vocab_size: int = 0) -> List[float]:
    """Extract n-gram TF-IDF features for a function."""
    if not vocabulary:
        return []

    if vocab_size == 0:
        vocab_size = len(vocabulary)

    # Create n-gram string
    ngrams = [' '.join(ngram) for ngram in func.instruction_ngrams]
    ngram_text = ' '.join(ngrams)

    # Simple binary presence for now (could be enhanced with actual TF-IDF)
    features = [0.0] * vocab_size
    for ngram_str in ngrams:
        if ngram_str in vocabulary:
            idx = vocabulary[ngram_str]
            if idx < vocab_size:
                features[idx] = 1.0

    return features


# =============================================================================
# UMAP Dimensionality Reduction
# =============================================================================

def apply_umap(
    feature_vectors: List[List[float]],
    n_neighbors: int = 15,
    min_dist: float = 0.1,
    metric: str = 'euclidean',
    random_state: int = 42
) -> Tuple[List[float], List[float]]:
    """
    Apply UMAP dimensionality reduction to project high-dimensional vectors to 2D.

    Returns:
        Tuple of (umap_1 coordinates, umap_2 coordinates)
    """
    import numpy as np
    from umap import UMAP

    # Convert to numpy array
    X = np.array(feature_vectors)

    if len(X) < 2:
        # Not enough points for UMAP
        return [0.0] * len(X), [0.0] * len(X)

    # Handle single-feature case
    if X.shape[1] < 2:
        # Pad with zeros if needed
        if X.shape[1] == 0:
            X = np.zeros((len(X), 1))
        # Already 1D, just return with zeros
        return X[:, 0].tolist(), [0.0] * len(X)

    # Adjust n_neighbors if dataset is small
    actual_n_neighbors = min(n_neighbors, len(X) - 1)
    if actual_n_neighbors < 2:
        actual_n_neighbors = 2

    try:
        reducer = UMAP(
            n_neighbors=actual_n_neighbors,
            min_dist=min_dist,
            metric=metric,
            n_components=2,
            random_state=random_state,
            n_jobs=1,
        )
        embedding = reducer.fit_transform(X)

        return embedding[:, 0].tolist(), embedding[:, 1].tolist()

    except Exception as e:
        print(f"Warning: UMAP failed ({e}), using fallback", file=sys.stderr)
        # Fallback: use first two PCA components
        from sklearn.decomposition import PCA
        pca = PCA(n_components=2, random_state=random_state)
        embedding = pca.fit_transform(X)
        return embedding[:, 0].tolist(), embedding[:, 1].tolist()


# =============================================================================
# Output formatting
# =============================================================================

def output_json(results: List[Dict], output_path: Optional[str] = None) -> None:
    """Output results as JSON."""
    data = [
        {
            "name": r["name"],
            "status": r["status"],
            "umap_1": round(r["umap_1"], 6),
            "umap_2": round(r["umap_2"], 6),
        }
        for r in results
    ]

    json_str = json.dumps(data, indent=2)

    if output_path:
        with open(output_path, 'w') as f:
            f.write(json_str)
        print(f"Output written to {output_path}", file=sys.stderr)
    else:
        print(json_str)


def output_csv(results: List[Dict], output_path: Optional[str] = None) -> None:
    """Output results as CSV."""
    import csv

    fieldnames = ["name", "status", "umap_1", "umap_2"]

    if output_path:
        with open(output_path, 'w', newline='') as f:
            writer = csv.DictWriter(f, fieldnames=fieldnames)
            writer.writeheader()
            for r in results:
                writer.writerow({
                    "name": r["name"],
                    "status": r["status"],
                    "umap_1": round(r["umap_1"], 6),
                    "umap_2": round(r["umap_2"], 6),
                })
        print(f"Output written to {output_path}", file=sys.stderr)
    else:
        # Write to stdout
        writer = csv.DictWriter(sys.stdout, fieldnames=fieldnames)
        writer.writeheader()
        for r in results:
            writer.writerow({
                "name": r["name"],
                "status": r["status"],
                "umap_1": round(r["umap_1"], 6),
                "umap_2": round(r["umap_2"], 6),
            })


def generate_element_id() -> str:
    """Generate a unique Excalidraw element ID."""
    import uuid
    return hashlib.md5(uuid.uuid4().bytes).hexdigest()[:16]


def output_excalidraw(
    results: List[Dict],
    output_path: str,
    canvas_width: int = 1200,
    canvas_height: int = 1200,
    padding: int = 20,
    title: str = "Function Embeddings",
    instruction_counts: Optional[List[int]] = None,
) -> None:
    """
    Output results as an Excalidraw JSON file.

    Creates a scatter plot with ellipse elements for each function.
    Matching functions are green, non-matching functions are pink/red.

    Circle sizes are based on instruction counts, with larger circles
    rendered first (smaller circles appear on top).
    """
    if not results:
        print("No results to visualize", file=sys.stderr)
        return

    # Extract coordinates
    umap_1_values = [r["umap_1"] for r in results]
    umap_2_values = [r["umap_2"] for r in results]

    # Find min/max for scaling
    min_x, max_x = min(umap_1_values), max(umap_1_values)
    min_y, max_y = min(umap_2_values), max(umap_2_values)

    # Calculate range, avoiding division by zero
    range_x = max_x - min_x if max_x != min_x else 1.0
    range_y = max_y - min_y if max_y != min_y else 1.0

    # Available drawing area (subtract padding)
    draw_width = canvas_width - 2 * padding
    draw_height = canvas_height - 2 * padding

    def scale_to_canvas(umap_x: float, umap_y: float) -> Tuple[int, int]:
        """Scale UMAP coordinates to canvas positions."""
        # Flip Y axis (Excalidraw Y increases downward, UMAP typically upward)
        canvas_x = padding + int((umap_x - min_x) / range_x * draw_width)
        canvas_y = padding + int((1.0 - (umap_y - min_y) / range_y) * draw_height)
        return canvas_x, canvas_y

    elements = []
    element_id_counter = 0

    # Split results by status
    matching = [r for r in results if r["status"] == "matching"]
    nonmatching = [r for r in results if r["status"] == "non-matching"]

    # Calculate sizes based on instruction counts
    import math
    import random
    if instruction_counts is None:
        instruction_counts = [10] * len(results)

    # Size formula: max(12, min(50, 8 + log(count) * 3))
    sizes = [max(12, min(50, 8 + math.log(max(1, count)) * 3)) for count in instruction_counts]

    # Separate matching and non-matching
    matching_indices = [i for i, r in enumerate(results) if r["status"] == "matching"]
    nonmatching_indices = [i for i, r in enumerate(results) if r["status"] == "non-matching"]

    # Sort each group by size (largest first)
    matching_indices.sort(key=lambda i: sizes[i], reverse=True)
    nonmatching_indices.sort(key=lambda i: sizes[i], reverse=True)

    # Combine: matching first (bottom layer), then non-matching (top layer)
    # This ensures non-matching functions are always visible on top
    sorted_indices = matching_indices + nonmatching_indices

    # Track occupied positions to add jitter for overlapping circles
    # Key: (grid_x, grid_y) where grid quantizes positions to size-based buckets
    occupied_positions: Dict[Tuple[int, int], int] = {}

    # Helper to create ellipse element
    def create_ellipse(x: int, y: int, is_matching: bool, size: float, name: str) -> Dict:
        nonlocal element_id_counter
        element_id_counter += 1

        if is_matching:
            stroke_color = "#2f9e44"  # Dark green
            bg_color = "#b2f2bb"  # Light green
        else:
            stroke_color = "#e03131"  # Dark pink/red
            bg_color = "#ffc9c9"  # Light pink

        size_int = int(size)
        return {
            "type": "ellipse",
            "id": generate_element_id(),
            "x": x - size_int // 2,
            "y": y - size_int // 2,
            "width": size_int,
            "height": size_int,
            "strokeColor": stroke_color,
            "backgroundColor": bg_color,
            "fillStyle": "solid",
            "strokeWidth": 2,
            "strokeStyle": "solid",
            "roughness": 1,
            "opacity": 100,
            "groupIds": [],
            "frameId": None,
            "roundness": {"type": 3},
            "seed": element_id_counter,
            "version": 1,
            "versionNonce": element_id_counter,
            "isDeleted": False,
            "boundElements": None,
            "updated": 1,
            "link": None,
            "locked": False,
        }

    # Create ellipse elements for each function
    # Matching first (by size), then non-matching (by size) so non-matching are always on top
    for idx in sorted_indices:
        r = results[idx]
        canvas_x, canvas_y = scale_to_canvas(r["umap_1"], r["umap_2"])
        is_matching = r["status"] == "matching"
        size = sizes[idx]

        # Add jitter to prevent complete overlap
        # Use grid-based collision detection with spacing proportional to circle size
        grid_spacing = max(8, int(size * 0.5))  # Smaller grid = better collision detection
        grid_x = canvas_x // grid_spacing
        grid_y = canvas_y // grid_spacing

        # Calculate offset based on how many circles are already in this grid cell
        offset_count = occupied_positions.get((grid_x, grid_y), 0)
        occupied_positions[(grid_x, grid_y)] = offset_count + 1

        # Spiral jitter: distribute overlapping circles in a small spiral pattern
        if offset_count > 0:
            jitter_max = int(size * 0.6)  # Jitter up to 60% of circle size
            angle = offset_count * 2.5  # More angle for tighter spiral
            radius = min(jitter_max, 3 + offset_count * 3)  # Start at 3px, grow faster
            jitter_x = int(radius * math.cos(angle))
            jitter_y = int(radius * math.sin(angle))
            canvas_x += jitter_x
            canvas_y += jitter_y

        elements.append(create_ellipse(canvas_x, canvas_y, is_matching, size, r["name"]))

    # Add legend
    legend_x = padding
    legend_y = canvas_height - padding - 60

    # Legend background
    elements.append({
        "type": "rectangle",
        "id": generate_element_id(),
        "x": legend_x - 20,
        "y": legend_y - 20,
        "width": 250,
        "height": 80,
        "strokeColor": "#000000",
        "backgroundColor": "#ffffff",
        "fillStyle": "solid",
        "strokeWidth": 2,
        "strokeStyle": "solid",
        "roughness": 1,
        "opacity": 100,
        "groupIds": [],
        "frameId": None,
        "roundness": {"type": 3},
        "seed": element_id_counter + 1,
        "version": 1,
        "versionNonce": element_id_counter + 1,
        "isDeleted": False,
        "boundElements": None,
        "updated": 1,
        "link": None,
        "locked": False,
    })

    # Legend - matching example (use a fixed size for legend)
    legend_match_size = 14.0
    legend_nonmatch_size = 14.0

    # Helper for legend ellipses
    def create_legend_ellipse(x: int, y: int, is_matching: bool) -> Dict:
        nonlocal element_id_counter
        element_id_counter += 1

        if is_matching:
            stroke_color = "#2f9e44"  # Dark green
            bg_color = "#b2f2bb"  # Light green
        else:
            stroke_color = "#e03131"  # Dark pink/red
            bg_color = "#ffc9c9"  # Light pink

        size = 14
        return {
            "type": "ellipse",
            "id": generate_element_id(),
            "x": x - size // 2,
            "y": y - size // 2,
            "width": size,
            "height": size,
            "strokeColor": stroke_color,
            "backgroundColor": bg_color,
            "fillStyle": "solid",
            "strokeWidth": 2,
            "strokeStyle": "solid",
            "roughness": 1,
            "opacity": 100,
            "groupIds": [],
            "frameId": None,
            "roundness": {"type": 3},
            "seed": element_id_counter,
            "version": 1,
            "versionNonce": element_id_counter,
            "isDeleted": False,
            "boundElements": None,
            "updated": 1,
            "link": None,
            "locked": False,
        }

    elements.append(create_legend_ellipse(legend_x + 20, legend_y, True))
    elements.append({
        "type": "text",
        "id": generate_element_id(),
        "x": legend_x + 40,
        "y": legend_y - 10,
        "width": 150,
        "height": 25,
        "strokeColor": "#1e1e1e",
        "backgroundColor": "transparent",
        "fillStyle": "solid",
        "strokeWidth": 2,
        "strokeStyle": "solid",
        "roughness": 1,
        "opacity": 100,
        "groupIds": [],
        "frameId": None,
        "text": f"Matching ({len(matching)})",
        "fontSize": 20,
        "fontFamily": 1,
        "textAlign": "left",
        "verticalAlign": "middle",
        "seed": element_id_counter + 2,
        "version": 1,
        "versionNonce": element_id_counter + 2,
        "isDeleted": False,
        "boundElements": None,
        "updated": 1,
        "link": None,
        "locked": False,
    })

    # Legend - non-matching example
    elements.append(create_legend_ellipse(legend_x + 20, legend_y + 30, False))
    elements.append({
        "type": "text",
        "id": generate_element_id(),
        "x": legend_x + 40,
        "y": legend_y + 20,
        "width": 180,
        "height": 25,
        "strokeColor": "#1e1e1e",
        "backgroundColor": "transparent",
        "fillStyle": "solid",
        "strokeWidth": 2,
        "strokeStyle": "solid",
        "roughness": 1,
        "opacity": 100,
        "groupIds": [],
        "frameId": None,
        "text": f"Non-matching ({len(nonmatching)})",
        "fontSize": 20,
        "fontFamily": 1,
        "textAlign": "left",
        "verticalAlign": "middle",
        "seed": element_id_counter + 3,
        "version": 1,
        "versionNonce": element_id_counter + 3,
        "isDeleted": False,
        "boundElements": None,
        "updated": 1,
        "link": None,
        "locked": False,
    })

    # Title
    elements.append({
        "type": "text",
        "id": generate_element_id(),
        "x": padding,
        "y": 30,
        "width": 400,
        "height": 30,
        "strokeColor": "#1e1e1e",
        "backgroundColor": "transparent",
        "fillStyle": "solid",
        "strokeWidth": 2,
        "strokeStyle": "solid",
        "roughness": 1,
        "opacity": 100,
        "groupIds": [],
        "frameId": None,
        "text": title,
        "fontSize": 24,
        "fontFamily": 1,
        "textAlign": "left",
        "verticalAlign": "middle",
        "seed": element_id_counter + 4,
        "version": 1,
        "versionNonce": element_id_counter + 4,
        "isDeleted": False,
        "boundElements": None,
        "updated": 1,
        "link": None,
        "locked": False,
    })

    # Build Excalidraw JSON structure
    excalidraw_data = {
        "type": "excalidraw",
        "version": 2,
        "source": "https://excalidraw.com",
        "elements": elements,
        "appState": {
            "gridSize": None,
            "viewBackgroundColor": "#ffffff",
            "zoom": {"value": 1},
            "showHelp": False,
            "objectsSnapModeEnabled": False,
            "fileId": "function_embeddings",
        },
        "files": {},
    }

    with open(output_path, 'w') as f:
        json.dump(excalidraw_data, f, indent=2)

    print(f"Excalidraw file written to {output_path}", file=sys.stderr)
    print(f"Import at: https://excalidraw.com", file=sys.stderr)


def create_xkcd_plot(
    results: List[Dict],
    output_path: str,
    title: str = "Function Embeddings",
    figsize: Tuple[int, int] = (12, 8),
    dpi: int = 150,
) -> None:
    """
    Create an XKCD-style scatter plot of function embeddings.

    Non-matching functions are highlighted since they're the minority.
    """
    try:
        import matplotlib.pyplot as plt
    except ImportError:
        print("Error: matplotlib is required for plotting. Install with: pip install matplotlib", file=sys.stderr)
        sys.exit(1)

    # Split results by status
    matching = [r for r in results if r["status"] == "matching"]
    nonmatching = [r for r in results if r["status"] == "non-matching"]

    # Extract coordinates
    match_x = [r["umap_1"] for r in matching]
    match_y = [r["umap_2"] for r in matching]
    nonmatch_x = [r["umap_1"] for r in nonmatching]
    nonmatch_y = [r["umap_2"] for r in nonmatching]

    # Enable XKCD style
    with plt.xkcd():
        fig, ax = plt.subplots(figsize=figsize, dpi=dpi)

        # Plot matching functions as subtle gray dots (background)
        if matching:
            ax.scatter(
                match_x,
                match_y,
                c='#CCCCCC',
                s=20,
                alpha=0.6,
                label=f"Matching ({len(matching)})",
                edgecolors='none',
            )

        # Plot non-matching functions as prominent red/orange dots
        if nonmatching:
            ax.scatter(
                nonmatch_x,
                nonmatch_y,
                c='#E74C3C',
                s=60,
                alpha=0.9,
                label=f"Non-matching ({len(nonmatching)})",
                edgecolors='#C0392B',
                linewidths=1.5,
                zorder=10,  # Draw on top
            )

        # Add labels and legend
        ax.set_title(title, fontsize=16, pad=20)
        ax.set_xlabel("UMAP-1", fontsize=12)
        ax.set_ylabel("UMAP-2", fontsize=12)
        ax.legend(loc='best', fontsize=10)

        # Remove grid for cleaner look
        ax.grid(False)

        # Add subtle border
        for spine in ax.spines.values():
            spine.set_edgecolor('#555555')
            spine.set_linewidth(1.5)

        # Adjust layout
        plt.tight_layout()

        # Save the plot
        fig.savefig(output_path, bbox_inches='tight', facecolor='white')
        plt.close(fig)

    print(f"Plot saved to {output_path}", file=sys.stderr)


# =============================================================================
# Main
# =============================================================================

def get_project_root() -> str:
    """Get the project root directory."""
    script_dir = os.path.dirname(os.path.abspath(__file__))
    return os.path.dirname(script_dir)


def main():
    parser = argparse.ArgumentParser(
        description="Generate 2D scatter plot data of functions using UMAP.",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
    python3 tools/function_embeddings.py
    python3 tools/function_embeddings.py --format csv
    python3 tools/function_embeddings.py --output embeddings.json
    python3 tools/function_embeddings.py --format csv --output embeddings.csv
    python3 tools/function_embeddings.py --n-neighbors 30 --min-dist 0.2
    python3 tools/function_embeddings.py --plot
    python3 tools/function_embeddings.py --plot --plot-output plot.png
    python3 tools/function_embeddings.py --excalidraw-output embeddings.excalidraw
    python3 tools/function_embeddings.py --excalidraw-output plot.excalidraw --excalidraw-size 3000
        """
    )
    parser.add_argument(
        '--format', '-f',
        choices=['json', 'csv'],
        default='json',
        help='Output format (default: json)'
    )
    parser.add_argument(
        '--output', '-o',
        help='Output file path (default: stdout)'
    )
    parser.add_argument(
        '--n-neighbors',
        type=int,
        default=15,
        help='UMAP n_neighbors parameter (default: 15)'
    )
    parser.add_argument(
        '--min-dist',
        type=float,
        default=0.1,
        dest='min_dist',
        help='UMAP min_dist parameter (default: 0.1)'
    )
    parser.add_argument(
        '--metric',
        type=str,
        default='euclidean',
        choices=['euclidean', 'manhattan', 'cosine', 'correlation'],
        help='UMAP distance metric (default: euclidean)'
    )
    parser.add_argument(
        '--random-state',
        type=int,
        default=42,
        dest='random_state',
        help='Random state for reproducibility (default: 42)'
    )
    parser.add_argument(
        '--max-ngrams',
        type=int,
        default=200,
        dest='max_ngrams',
        help='Maximum number of n-gram features to include (default: 200, 0 to disable)'
    )
    parser.add_argument(
        '--verbose', '-v',
        action='store_true',
        help='Show progress information'
    )
    parser.add_argument(
        '--plot',
        action='store_true',
        help='Generate an XKCD-style scatter plot visualization'
    )
    parser.add_argument(
        '--plot-output',
        type=str,
        default='function_embeddings.png',
        help='Output path for the plot (default: function_embeddings.png)'
    )
    parser.add_argument(
        '--plot-title',
        type=str,
        default='Function Embeddings',
        help='Title for the plot (default: "Function Embeddings")'
    )
    parser.add_argument(
        '--dpi',
        type=int,
        default=150,
        help='DPI for the plot output (default: 150)'
    )
    parser.add_argument(
        '--excalidraw-output',
        type=str,
        help='Output path for Excalidraw JSON file (.excalidraw)'
    )
    parser.add_argument(
        '--excalidraw-size',
        type=int,
        default=1200,
        metavar='SIZE',
        help='Canvas size for Excalidraw output (default: 1200, creates SIZExSIZE canvas)'
    )

    args = parser.parse_args()

    project_root = get_project_root()
    matchings_dir = os.path.join(project_root, 'asm', 'matchings')
    nonmatchings_dir = os.path.join(project_root, 'asm', 'nonmatchings')

    # Step 1: Parse all functions
    if args.verbose:
        print(f"Parsing matching functions from {matchings_dir}...", file=sys.stderr)

    matching_functions = build_function_index(matchings_dir)

    if args.verbose:
        print(f"  Found {len(matching_functions)} matching functions", file=sys.stderr)
        print(f"Parsing non-matching functions from {nonmatchings_dir}...", file=sys.stderr)

    nonmatching_functions = build_function_index(nonmatchings_dir)

    if args.verbose:
        print(f"  Found {len(nonmatching_functions)} non-matching functions", file=sys.stderr)

    all_functions = matching_functions + nonmatching_functions

    if not all_functions:
        print("Error: No functions found", file=sys.stderr)
        sys.exit(1)

    if args.verbose:
        print(f"Total functions: {len(all_functions)}", file=sys.stderr)

    # Step 2: Build n-gram vocabulary (if enabled)
    ngram_vocabulary = {}
    if args.max_ngrams > 0:
        if args.verbose:
            print(f"Building n-gram vocabulary (max {args.max_ngrams})...", file=sys.stderr)

        ngram_vocabulary = build_ngram_vocabulary(all_functions, args.max_ngrams)

        if args.verbose:
            print(f"  Vocabulary size: {len(ngram_vocabulary)}", file=sys.stderr)

    # Step 3: Create feature vectors
    if args.verbose:
        print("Creating feature vectors...", file=sys.stderr)

    feature_vectors = []
    for func in all_functions:
        vec = function_to_feature_vector(func, ALL_OPCODES, ngram_vocabulary)
        feature_vectors.append(vec)

    if args.verbose:
        print(f"  Feature vector dimension: {len(feature_vectors[0]) if feature_vectors else 0}", file=sys.stderr)

    # Step 4: Apply UMAP
    if args.verbose:
        print("Applying UMAP dimensionality reduction...", file=sys.stderr)

    umap_1, umap_2 = apply_umap(
        feature_vectors,
        n_neighbors=args.n_neighbors,
        min_dist=args.min_dist,
        metric=args.metric,
        random_state=args.random_state
    )

    # Step 5: Combine results
    results = []
    instruction_counts = []

    for i, func in enumerate(matching_functions):
        results.append({
            "name": func.name,
            "status": "matching",
            "umap_1": umap_1[i],
            "umap_2": umap_2[i],
        })
        instruction_counts.append(func.instruction_count)

    for i, func in enumerate(nonmatching_functions):
        idx = len(matching_functions) + i
        results.append({
            "name": func.name,
            "status": "non-matching",
            "umap_1": umap_1[idx],
            "umap_2": umap_2[idx],
        })
        instruction_counts.append(func.instruction_count)

    # Step 6: Output
    if args.verbose:
        print(f"Outputting {len(results)} function embeddings...", file=sys.stderr)

    # Only output JSON/CSV if not in plot-only mode
    if not args.plot or args.output:
        if args.format == 'csv':
            output_csv(results, args.output)
        else:
            output_json(results, args.output)

    # Step 7: Generate plot if requested
    if args.plot:
        if args.verbose:
            print("Generating XKCD-style plot...", file=sys.stderr)
        create_xkcd_plot(
            results,
            args.plot_output,
            title=args.plot_title,
            dpi=args.dpi,
        )

    # Step 8: Generate Excalidraw file if requested
    if args.excalidraw_output:
        if args.verbose:
            print("Generating Excalidraw JSON...", file=sys.stderr)
        output_excalidraw(
            results,
            args.excalidraw_output,
            canvas_width=args.excalidraw_size,
            canvas_height=args.excalidraw_size,
            title=args.plot_title,
            instruction_counts=instruction_counts,
        )

    if args.verbose:
        matching_count = sum(1 for r in results if r["status"] == "matching")
        nonmatching_count = sum(1 for r in results if r["status"] == "non-matching")
        print(f"Done: {matching_count} matching, {nonmatching_count} non-matching", file=sys.stderr)


if __name__ == '__main__':
    main()
