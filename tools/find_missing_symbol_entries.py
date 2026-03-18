#!/usr/bin/env python3
"""Find C functions missing from symbol_addrs.txt and generate entries for them.

This script scans asm/matchings/ for all func_*.s assembly files and checks if
the corresponding descriptive function names (from snowboardkids2.map) have
entries in symbol_addrs.txt. For missing entries, it generates symbol entries.
"""

import os
import re
import subprocess
import sys

# Patterns
FUNC_PATTERN = re.compile(r'func_[0-9A-Fa-f]{8}_[0-9A-Fa-f]{4,5}')
ASM_ADDRESS_LINE = re.compile(r'/\*\s+([0-9A-Fa-f]+)\s+([0-9A-Fa-f]+)\s+')
SYMBOL_ENTRY_PATTERN = re.compile(r'^(\w+)\s*=\s*0x([0-9A-Fa-f]+);')
MAP_ADDRESS_PATTERN = re.compile(r'^\s*(0x[0-9a-f]+)\s+(\w+)')


def load_symbol_addrs(symbol_addrs_path):
    """Load existing symbol_addrs.txt into a set of function names and a dict of rom offsets.

    Returns a tuple of (symbol_names_set, rom_offsets_dict)
    where rom_offsets_dict maps rom_offset -> virtual_addr
    """
    symbols = set()
    rom_offsets = {}  # rom_offset -> virtual_addr
    try:
        with open(symbol_addrs_path, 'r') as f:
            for line in f:
                line = line.strip()
                if line and not line.startswith('//'):
                    match = SYMBOL_ENTRY_PATTERN.match(line)
                    if match:
                        name = match.group(1)
                        virtual_addr = match.group(2)
                        symbols.add(name)
                        # Extract rom offset if present
                        rom_match = re.search(r'rom:(0x[0-9A-Fa-f]+)', line)
                        if rom_match:
                            rom_offset = rom_match.group(1)
                            # Store the rom offset mapping (with 0x prefix, lowercase)
                            rom_offsets[rom_offset.lower()] = virtual_addr
    except FileNotFoundError:
        print(f"Warning: {symbol_addrs_path} not found")
    return symbols, rom_offsets


def load_map_file(map_path):
    """Load snowboardkids2.map to build address->name mapping.

    Returns a dictionary with lowercase addresses as keys.
    """
    address_to_name = {}
    try:
        with open(map_path, 'r') as f:
            for line in f:
                match = MAP_ADDRESS_PATTERN.match(line)
                if match:
                    addr = match.group(1).lower()
                    name = match.group(2)
                    # Skip entrypoint and other special symbols
                    if not name.startswith('.'):
                        address_to_name[addr] = name
    except FileNotFoundError:
        print(f"Warning: {map_path} not found")
    return address_to_name


def extract_info_from_asm(asm_path):
    """Extract virtual address and rom offset from assembly file.

    Scans for glabel, then parses the next instruction's comment.
    Returns dict with 'virtual_addr' and 'rom_offset', or None if not found.
    """
    virtual_addr = None
    rom_offset = None

    try:
        with open(asm_path, 'r') as f:
            lines = f.readlines()

        found_glabel = False
        for line in lines:
            # Only look for glabel (functions), not jlabel/dlabel (jump tables/data)
            if line.strip().startswith('glabel '):
                found_glabel = True
            # After finding glabel, look for the first instruction with address comment
            elif found_glabel:
                addr_match = ASM_ADDRESS_LINE.search(line)
                if addr_match:
                    rom_offset = '0x' + addr_match.group(1)
                    virtual_addr = '0x' + addr_match.group(2)
                    break

    except (IOError, UnicodeDecodeError) as e:
        print(f"Error reading {asm_path}: {e}", file=sys.stderr)
        return None

    if virtual_addr and rom_offset:
        return {
            'virtual_addr': virtual_addr,
            'rom_offset': rom_offset
        }
    return None


def find_descriptive_name(virtual_addr, map_dict):
    """Look up descriptive function name from address in map file."""
    # Convert to lowercase for lookup
    return map_dict.get(virtual_addr.lower())


def verify_build():
    """Run build-and-verify.sh and return success status."""
    result = subprocess.run(
        ['./tools/build-and-verify.sh'],
        cwd=os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
        capture_output=True,
        text=True
    )
    return result.returncode == 0


def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    project_root = os.path.dirname(script_dir)

    asm_matchings_dir = os.path.join(project_root, 'asm', 'matchings')
    symbol_addrs_path = os.path.join(project_root, 'symbol_addrs.txt')
    map_path = os.path.join(project_root, 'snowboardkids2.map')

    # Parse arguments
    add_mode = '--add' in sys.argv
    skip_verify = '--skip-verify' in sys.argv
    list_mode = '--list' in sys.argv
    verbose = '--verbose' in sys.argv

    # Load existing symbols and map file
    existing_symbols, rom_offsets = load_symbol_addrs(symbol_addrs_path)
    print(f"Loaded {len(existing_symbols)} existing symbols from symbol_addrs.txt")

    address_to_name = load_map_file(map_path)
    print(f"Loaded {len(address_to_name)} address->name mappings from map file")

    # Scan asm/matchings/ for all .s files
    # Use dict to deduplicate by function name
    entries_by_name = {}
    scanned_count = 0
    for root, dirs, files in os.walk(asm_matchings_dir):
        for filename in files:
            if filename.endswith('.s'):
                filepath = os.path.join(root, filename)
                asm_func_name = filename.rstrip('.s')

                # Extract address info from assembly file
                extracted_info = extract_info_from_asm(filepath)
                if not extracted_info:
                    if verbose:
                        print(f"  ✗ {asm_func_name} <- could not extract address info")
                    continue

                virtual_addr = extracted_info['virtual_addr']
                rom_offset = extracted_info['rom_offset']

                # Look up descriptive function name from map file
                descriptive_name = find_descriptive_name(virtual_addr, address_to_name)
                if not descriptive_name:
                    if verbose:
                        print(f"  ✗ {asm_func_name} <- no name found in map for {virtual_addr}")
                    continue

                # Skip if it's still a func_ name (not renamed yet)
                if FUNC_PATTERN.match(descriptive_name):
                    if verbose:
                        print(f"  ~ {asm_func_name} <- still using func_ naming")
                    continue

                # Check if descriptive name already in symbol_addrs.txt
                if descriptive_name in existing_symbols:
                    if verbose:
                        print(f"  ✓ {descriptive_name} <- already in symbol_addrs.txt")
                    continue

                # Check if there's already a symbol at this ROM offset (conflict)
                # rom_offsets stores values with 0x prefix and lowercase
                rom_offset_key = rom_offset.lower()
                if rom_offset_key in rom_offsets:
                    existing_vaddr = rom_offsets[rom_offset_key]
                    if existing_vaddr != virtual_addr:
                        if verbose:
                            print(f"  ✗ {descriptive_name} <- ROM offset conflict: existing symbol at {rom_offset} maps to {existing_vaddr}")
                        continue
                    # Same virtual address - skip this duplicate
                    if verbose:
                        print(f"  ~ {descriptive_name} <- already has entry at this ROM offset")
                    continue

                scanned_count += 1

                # Generate entry without size comment
                # Deduplicate - only keep the first occurrence
                if descriptive_name not in entries_by_name:
                    entry = f"{descriptive_name} = {virtual_addr}; // rom:{rom_offset}"
                    entries_by_name[descriptive_name] = {
                        'entry': entry,
                        'func_name': descriptive_name,
                        'asm_path': filepath,
                        'asm_func_name': asm_func_name
                    }

                    if verbose:
                        print(f"  ✓ {descriptive_name} <- {asm_func_name}")

    entries_to_add = list(entries_by_name.values())

    print(f"Scanned {scanned_count} asm/matchings functions")
    print(f"Found {len(entries_to_add)} functions missing from symbol_addrs.txt")

    if not entries_to_add:
        print("No missing entries found!")
        return 0

    if list_mode:
        print(f"\n{len(entries_to_add)} missing entries:")
        print("-" * 80)
        for item in entries_to_add:
            print(f"{item['entry']}")
        return 0

    print(f"\nGenerated {len(entries_to_add)} symbol entries:")
    print("-" * 80)
    for item in entries_to_add:
        print(f"{item['entry']}")

    if not add_mode:
        print("\n(Analyze mode only - no changes made. Use --add to add entries.)")
        return 0

    # Add entries one at a time
    print("\nAdding entries to symbol_addrs.txt...")
    added_count = 0
    for item in entries_to_add:
        entry = item['entry']
        func_name = item['func_name']

        print(f"\nAdding: {entry}")

        # Append to symbol_addrs.txt
        with open(symbol_addrs_path, 'a') as f:
            f.write(entry + '\n')

        added_count += 1

        # Verify build unless --skip-verify is specified
        if not skip_verify:
            print("  Verifying build...")
            if verify_build():
                print("  ✓ Build verified")
            else:
                print("  ✗ Build failed - removing last entry and stopping")
                # Remove the last entry
                with open(symbol_addrs_path, 'r') as f:
                    lines = f.readlines()
                with open(symbol_addrs_path, 'w') as f:
                    f.writelines(lines[:-1])
                print(f"  Removed: {entry}")
                print(f"\nSuccessfully added {added_count - 1} entries before failure.")
                return 1
        else:
            print("  (Skipping build verification due to --skip-verify)")

    print(f"\nSuccessfully added {added_count} entries to symbol_addrs.txt")
    return 0


if __name__ == '__main__':
    sys.exit(main())
