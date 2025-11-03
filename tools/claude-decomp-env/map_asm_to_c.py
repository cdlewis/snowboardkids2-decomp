#!/usr/bin/env python3
import sys
import re
import os

def parse_annotated_asm(filename):
    """Parse an annotated assembly file and return mappings of addresses/line numbers to C code."""
    address_to_c = {}
    line_to_offset = {}
    current_c_code = []
    current_source_location = None
    instruction_line_num = 0

    with open(filename, 'r') as f:
        lines = f.readlines()

    for line in lines:
        # Remove the line number prefix added by the Read tool (e.g., "    10→")
        cleaned_line = re.sub(r'^\s*\d+→', '', line)

        # Check if this is a C source file marker (e.g., "src/claude/base_4.c:54")
        source_match = re.match(r'^(src/[^:]+\.c):(\d+)', cleaned_line.strip())
        if source_match:
            # Update current source location
            current_source_location = f"{source_match.group(1)}:{source_match.group(2)}"
            # Reset current C code collection
            current_c_code = []
            continue

        # Check if this is an assembly instruction (hex address followed by colon)
        # Format: "   c:	0c000000 	jal	0 <func_8001A478_1B078>"
        asm_match = re.match(r'^\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+', cleaned_line)
        if asm_match:
            # Found assembly instruction, map it to current C code
            addr = int(asm_match.group(1), 16)
            instruction_line_num += 1

            # Map line number to offset
            line_to_offset[instruction_line_num] = addr

            c_code_text = '\n'.join(current_c_code).strip()
            if c_code_text and current_source_location:
                address_to_c[addr] = {
                    'location': current_source_location,
                    'code': c_code_text
                }
            # Don't reset current_c_code here - C code continues between instructions
            continue

        # Check if this is a line with relocation info (starts with tabs/spaces and contains "R_MIPS")
        if re.match(r'^\s+[0-9a-f]+:\s+R_MIPS', cleaned_line):
            # Skip relocation lines
            continue

        # Check if this is a function label or section marker
        if re.match(r'^[0-9a-f]+ <.*>:', cleaned_line) or 'Disassembly of section' in cleaned_line:
            current_c_code = []
            instruction_line_num = 0  # Reset line counter for new function
            continue

        # Check if this is a blank line or just whitespace
        if not cleaned_line.strip():
            continue

        # Check if this looks like actual C code (indented, not empty)
        if cleaned_line.startswith(' ') or cleaned_line.startswith('\t'):
            # This is likely C code - add it to current collection
            current_c_code.append(cleaned_line.rstrip())

    return address_to_c, line_to_offset

def find_c_code(filename, line_input):
    """Find C code corresponding to a line number (decimal)."""
    # Parse the file
    address_to_c, line_to_offset = parse_annotated_asm(filename)

    # Check if input looks like hex (starts with 0x)
    if line_input.lower().startswith('0x'):
        return f"Error: Please use a decimal line number, not a hex offset.\nExample: Use '4' instead of '0xc'"

    # Parse decimal line number
    try:
        line_num = int(line_input, 10)
    except ValueError:
        return f"Error: Invalid line number '{line_input}'. Please use a decimal number."

    # Convert line number to offset
    if line_num not in line_to_offset:
        return f"No instruction found at line {line_num}"

    addr = line_to_offset[line_num]

    # Look up the address
    if addr in address_to_c:
        info = address_to_c[addr]
        return f"{info['location']}\n{info['code']}"
    else:
        return f"No C code found for address 0x{addr:x}"

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("Usage: python asm_to_c.py <object_file> <line_number>")
        print("\nExample:")
        print("  python asm_to_c.py base_4.o 4")
        print("\nNote: Line number must be a decimal number (not hex)")
        sys.exit(1)

    input_file = sys.argv[1]
    line_input = sys.argv[2]

    # Derive the annotated .s filename from the .o file
    if input_file.endswith('.o'):
        # Remove .o extension and add _annotated.s
        base_name = input_file[:-2]
        annotated_file = f"{base_name}_annotated.s"
        c_file = f"{base_name}.c"
    else:
        # If not a .o file, assume it's already the annotated .s file
        annotated_file = input_file
        # Try to derive .c filename for error message
        if input_file.endswith('_annotated.s'):
            c_file = input_file.replace('_annotated.s', '.c')
        else:
            c_file = input_file.replace('.s', '.c')

    # Check if the annotated file exists
    if not os.path.exists(annotated_file):
        print(f"Error: {annotated_file} not found.")
        print(f"Please run: ./build.sh {c_file}")
        sys.exit(1)

    result = find_c_code(annotated_file, line_input)
    print(result)
