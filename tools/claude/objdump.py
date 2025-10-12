#!/usr/bin/env python3

"""
objdump.py is a Python script that processes MIPS object files and displays their disassembled assembly code in a clean, normalized format.

Basic Usage: `./objdump.py file.o`
With line numbers: `./objdump.py file_annotated.s --from-dump`
"""

import re
import subprocess
import sys
import shutil
from typing import List, Optional

# Configuration
SKIP_LINES = 1
RE_INT = re.compile(r"-?[0-9]+")
RE_COMMENT = re.compile(r"<.*?>")
RE_SPREL = re.compile(r"(?<=,)([0-9]+|0x[0-9a-f]+)\((sp|s8)\)")
RE_SOURCE_LINE = re.compile(r"^[a-zA-Z0-9_/\.\-]+\.[ch]:\d+")  # Matches "src/temp.c:336"
RE_ASM_LINE = re.compile(r"^\s*([0-9a-f]+):\s+([0-9a-f]+)\s+(.+)$")  # Matches " 794:    2402001e     li    v0,30"
RE_RELOC_LINE = re.compile(r"^\s+([0-9a-f]+):\s+(R_MIPS_\S+)\s+(.+)$")  # Matches "            7a0: R_MIPS_26    setGameStateHandler"

# MIPS objdump settings
OBJDUMP_EXECUTABLES = [
    "mips-linux-gnu-objdump",
    "mips64-linux-gnu-objdump",
    "mips64-elf-objdump",
]
OBJDUMP_ARGS = ["-drz", "-m", "mips:4300"]

BRANCH_INSTRUCTIONS = {
    "b", "j", "beq", "bne", "beqz", "bnez", "bgez", "bgtz", "blez", "bltz",
    "bc1t", "bc1f", "beql", "bnel", "beqzl", "bnezl", "bgezl", "bgtzl", 
    "blezl", "bltzl", "bc1tl", "bc1fl"
}


def find_objdump_executable() -> str:
    for executable in OBJDUMP_EXECUTABLES:
        if shutil.which(executable):
            return executable
    raise Exception(f"Could not find any objdump executables: {OBJDUMP_EXECUTABLES}")


def parse_relocated_line(line: str) -> tuple[str, str, str]:
    for c in ",\t ":
        if c in line:
            split_pos = line.rindex(c)
            break
    else:
        raise Exception(f"failed to parse relocated line: {line}")
    
    before = line[:split_pos + 1]
    after = line[split_pos + 1:]
    
    paren_pos = after.find("(")
    if paren_pos == -1:
        imm, after = after, ""
    else:
        imm, after = after[:paren_pos], after[paren_pos:]
    
    return before, "0" if imm == "0x0" else imm, after


def process_mips_reloc(reloc_row: str, repl: str, imm: str) -> str:
    if "R_MIPS_JALR" in reloc_row or "R_MIPS_NONE" in reloc_row:
        return repl
        
    if imm not in ("0", "imm", "addr"):
        try:
            imm_val = int(imm, 0)
            repl += f"+{imm}" if imm_val > 0 else imm
        except ValueError:
            # Handle cases where imm might be a hex string without 0x prefix
            try:
                imm_val = int(imm, 16)
                repl += f"+0x{imm}" if imm_val > 0 else f"-0x{imm}"
            except ValueError:
                # If still can't parse, just append as-is
                repl += f"+{imm}"
        
    if any(r in reloc_row for r in ["R_MIPS_LO16", "R_MIPS_LITERAL", "R_MIPS_GPREL16"]):
        return f"%lo({repl})"
    elif "R_MIPS_GOT16" in reloc_row:
        return f"%got({repl})"
    elif "R_MIPS_CALL16" in reloc_row:
        return f"%call16({repl})"
    elif "R_MIPS_HI16" in reloc_row:
        return f"%hi({repl})"
    elif "R_MIPS_26" in reloc_row:
        return repl
    else:
        raise Exception(f"unknown relocation type '{reloc_row}'")


def process_reloc(reloc_row: str, prev_line: str) -> Optional[str]:
    if prev_line == "<skipped>":
        return None
    
    before, imm, after = parse_relocated_line(prev_line)
    symbol = reloc_row.split()[-1]
    
    if "R_MIPS_" in reloc_row:
        new_symbol = process_mips_reloc(reloc_row, symbol, imm)
        return before + new_symbol + after
    else:
        raise Exception(f"unknown relocation type: {reloc_row}")


def normalize_jumptable_references(line: str) -> str:
    """Normalize jump table references to .rodata for consistent comparison"""
    # Pattern to match jump table references like jtbl_8009E998_9F598
    jtbl_pattern = r'jtbl_[0-9A-Fa-f]+_[0-9A-Fa-f]+'
    
    # Replace jump table references with .rodata
    normalized = re.sub(jtbl_pattern, '.rodata', line)
    return normalized


def convert_numbers_to_hex(line: str) -> str:
    """Convert decimal numbers to hex, avoiding variable names and other text"""
    forbidden_chars = set("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_")
    
    def replace_func(match):
        full = match.group(0)
        if len(full) <= 1:
            return full
        start, end = match.span()
        if start > 0 and line[start - 1] in forbidden_chars:
            return full
        if end < len(line) and line[end] in forbidden_chars:
            return full
        return hex(int(full))
    
    return RE_INT.sub(replace_func, line)


def process_annotated_dump_lines(lines: List[str], include_source: bool = True) -> List[str]:
    """Process objdump output that includes line numbers and source code."""
    output_lines = []
    current_source_file = None

    i = 0
    while i < len(lines):
        line = lines[i].rstrip()
        i += 1

        # Skip empty lines and header lines
        if not line or "file format" in line or "Disassembly of section" in line:
            continue

        # Check if this is a source file:line marker
        source_match = RE_SOURCE_LINE.match(line)
        if source_match:
            current_source_file = line
            if include_source:
                output_lines.append(f"# {line}")
            continue

        # Check if this is a function label (ends with :>)
        if line.endswith(">:") or (">:" in line and "<" in line):
            continue

        # Check if this is a source code line (indented, no hex address)
        if line.startswith("        ") and not RE_ASM_LINE.match(line) and not RE_RELOC_LINE.match(line):
            # This is source code - optionally include as comment
            if include_source:
                output_lines.append(f"# {line.strip()}")
            continue

        # Check if this is a relocation line
        reloc_match = RE_RELOC_LINE.match(line)
        if reloc_match:
            # Process relocation and apply to previous instruction
            if output_lines and not output_lines[-1].startswith("#"):
                modified_line = process_reloc(line, output_lines[-1])
                if modified_line:
                    output_lines[-1] = modified_line
            continue

        # Check if this is an assembly instruction line
        asm_match = RE_ASM_LINE.match(line)
        if asm_match:
            addr, hexcode, instruction = asm_match.groups()

            # Remove comments from instruction
            instruction = RE_COMMENT.sub("", instruction).strip()

            # Parse mnemonic and arguments
            if "\t" in instruction:
                mnemonic, args = instruction.split("\t", 1)
            else:
                parts = instruction.split(None, 1)
                mnemonic = parts[0].strip()
                args = parts[1].strip() if len(parts) > 1 else ""

            formatted = f"{mnemonic}\t{args}".replace("\t", " " * 4) if args else mnemonic

            # Convert decimal numbers to hex (but preserve branch targets)
            if mnemonic not in BRANCH_INSTRUCTIONS:
                formatted = convert_numbers_to_hex(formatted)

            # Normalize jump table references
            formatted = normalize_jumptable_references(formatted)

            output_lines.append(formatted)

    # Remove trailing nops
    while output_lines and output_lines[-1].startswith("nop"):
        output_lines.pop()

    return output_lines


def process_objdump_lines(lines: List[str]) -> List[str]:
    output_lines = []

    for i, line in enumerate(lines):
        if i < SKIP_LINES:
            continue

        line = line.rstrip()
        if ">:" in line or not line:
            continue

        # Remove comments and extract instruction part
        line = RE_COMMENT.sub("", line).rstrip()
        line = "\t".join(line.split("\t")[2:])  # Remove address and hex bytes
        if not line:
            continue

        # Parse mnemonic and arguments
        if "\t" in line:
            mnemonic, args = line.split("\t", 1)
        else:
            parts = line.split(" ", 1)
            mnemonic = parts[0].strip()
            args = parts[1].strip() if len(parts) > 1 else ""

        line = f"{mnemonic}\t{args}".replace("\t", " " * 4) if args else mnemonic

        # Handle relocations
        if "R_MIPS_" in line:
            if output_lines:
                modified_line = process_reloc(line, output_lines[-1])
                if modified_line:
                    output_lines[-1] = modified_line
            continue

        # Convert decimal numbers to hex (but preserve stack/register differences)
        if mnemonic not in BRANCH_INSTRUCTIONS:
            line = convert_numbers_to_hex(line)

        # Normalize jump table references to .rodata for consistent comparison
        line = normalize_jumptable_references(line)

        output_lines.append(line)

    # Remove trailing nops
    while output_lines and output_lines[-1].startswith("nop"):
        output_lines.pop()

    return output_lines


def objdump(filename: str) -> List[str]:
    executable = find_objdump_executable()
    result = subprocess.check_output([executable] + OBJDUMP_ARGS + [filename])
    lines = result.decode("utf-8").splitlines()
    return process_objdump_lines(lines)


if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(
        description="Process MIPS object files and display disassembled assembly code"
    )
    parser.add_argument("file", help="Object file (.o) or annotated dump file (.s)")
    parser.add_argument(
        "--from-dump",
        action="store_true",
        help="Process an already-dumped file with line numbers (from objdump --line-numbers --source)"
    )
    parser.add_argument(
        "--no-source",
        action="store_true",
        help="When processing annotated dumps, exclude source file:line and source code comments"
    )

    args = parser.parse_args()

    try:
        if args.from_dump:
            # Process an annotated dump file
            with open(args.file, 'r') as f:
                lines = f.readlines()
            output_lines = process_annotated_dump_lines(lines, include_source=not args.no_source)
        else:
            # Process a .o file with objdump
            output_lines = objdump(args.file)

        for line in output_lines:
            print(line)

    except FileNotFoundError:
        print(f"Error: File '{args.file}' not found", file=sys.stderr)
        sys.exit(1)
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        import traceback
        traceback.print_exc()
        sys.exit(1)