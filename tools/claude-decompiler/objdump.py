#!/usr/bin/env python3

"""
objdump.py is a Python script that processes MIPS object files and displays their disassembled assembly code in a clean, normalized format.

Basic Usage: `./objdump.py file.o`
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
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} file.o", file=sys.stderr)
        sys.exit(1)
        
    filename = sys.argv[1]
    try:
        lines = objdump(filename)
        for line in lines:
            print(line)
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)