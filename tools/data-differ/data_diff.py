#!/usr/bin/env python3
"""
Data differ tool for comparing binary data between compiled output and target ROM.

Similar to asm-differ/diff.py but for data sections instead of code.
Usage: python3 tools/data-differ/data_diff.py <symbol_name>
"""

import argparse
import sys
import os
import struct
from pathlib import Path
from typing import Dict, List, Optional, Tuple

# Colors for terminal output
class Colors:
    RESET = '\033[0m'
    RED = '\033[91m'
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    BLUE = '\033[94m'
    CYAN = '\033[96m'
    BOLD = '\033[1m'


def load_symbol_addrs(symbol_addrs_path: str) -> Dict[str, Tuple[int, int, Optional[str]]]:
    """
    Parse symbol_addrs.txt file.
    Returns dict mapping symbol name -> (vram_addr, rom_addr, size)
    Size may be None if not specified.
    """
    symbols = {}
    with open(symbol_addrs_path, 'r') as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith('#'):
                continue

            # Parse format: symbolName = 0xVRAM; // rom:0xROM [size:0xSIZE] [type:TYPE]
            if '=' in line:
                parts = line.split('=')
                name = parts[0].strip()
                rest = parts[1] if len(parts) > 1 else ""

                vram_addr = None
                rom_addr = None
                size = None
                data_type = None

                # Extract VRAM address
                if '0x' in rest:
                    vram_str = rest.split('0x')[1].split(';')[0]
                    vram_addr = int(vram_str, 16)

                # Extract ROM address
                if 'rom:' in rest:
                    rom_str = rest.split('rom:')[1].split()[0]
                    rom_addr = int(rom_str, 16)

                # Extract size
                if 'size:' in rest:
                    size_str = rest.split('size:')[1].split()[0]
                    size = int(size_str, 16)

                # Extract type
                if 'type:' in rest:
                    data_type = rest.split('type:')[1].strip()

                if vram_addr is not None:
                    symbols[name] = (vram_addr, rom_addr, size, data_type)

    return symbols


def parse_vram_to_rom_map(map_path: str) -> Dict[int, Tuple[int, int]]:
    """
    Parse linker map file to build VRAM to ROM address mapping.
    Returns dict mapping VRAM address -> (ROM_address, size)
    for each section.
    """
    vram_to_rom = {}
    with open(map_path, 'r') as f:
        for line in f:
            # Look for section definitions with load addresses
            # Format: .section  0xVRAM  0xSIZE load address 0xROM
            if 'load address' in line and '0x80' in line:
                parts = line.split()
                if len(parts) >= 6:
                    try:
                        section_name = parts[0]
                        vram = int(parts[1], 16)
                        size = int(parts[2], 16)
                        # Find the load address - skip 'load' and 'address' words
                        load_idx = parts.index('load') + 2
                        rom = int(parts[load_idx], 16)
                        vram_to_rom[vram] = (rom, size)
                    except (ValueError, IndexError):
                        continue
    return vram_to_rom


def get_rom_for_vram(vram_addr: int, vram_to_rom_map: Dict[int, Tuple[int, int]]) -> Optional[int]:
    """
    Convert VRAM address to ROM address using the section mapping.
    Returns None if the address doesn't fall within any mapped section.
    """
    # Sort VRAM addresses in descending order to find the right section
    sorted_vrams = sorted(vram_to_rom_map.keys(), reverse=True)

    for section_vram in sorted_vrams:
        rom_addr, section_size = vram_to_rom_map[section_vram]
        if vram_addr >= section_vram:
            offset = vram_addr - section_vram
            # Check if within section bounds
            if offset < section_size:
                return rom_addr + offset

    return None


def parse_map_symbols(elf_path: str) -> Dict[str, Tuple[int, int, int]]:
    """
    Parse ELF file to extract all data symbols with VRAM address and size.
    Returns dict mapping symbol name -> (vram_addr, size, section_index)

    Uses readelf to get OBJECT type symbols which are data variables.
    """
    import subprocess

    symbols = {}

    try:
        result = subprocess.run(
            ['readelf', '-s', '--wide', elf_path],
            capture_output=True,
            text=True,
            check=True
        )

        for line in result.stdout.split('\n'):
            if not line or 'Symbol table' in line or 'Num:' in line:
                continue

            parts = line.split()
            # Format: num: value size type bind vis ndx name
            # Example: 13162: 8008f200    12 OBJECT  GLOBAL DEFAULT    5 D_8008F200_8FE00
            if len(parts) >= 8 and ':' in parts[0]:
                try:
                    value = int(parts[1], 16)
                    size = int(parts[2], 10)
                    type_str = parts[3]  # OBJECT, FUNC, etc.
                    name = parts[7] if len(parts) > 7 else ""

                    # Skip non-object symbols (functions, etc.)
                    if type_str != 'OBJECT':
                        continue
                    if not name or name.startswith('$'):
                        continue
                    if value == 0:
                        continue
                    # Skip symbols with size 0
                    if size == 0:
                        continue

                    symbols[name] = (value, size, 0)

                except (ValueError, IndexError):
                    continue

    except (subprocess.CalledProcessError, FileNotFoundError) as e:
        print(f"{Colors.YELLOW}Warning: Could not run readelf: {e}{Colors.RESET}")

    return symbols


def parse_elf_symbols(elf_path: str) -> Dict[str, Tuple[int, int]]:
    """
    Parse ELF file to get symbol addresses and sizes.
    Returns dict mapping symbol name -> (vram_addr, size)
    """
    symbols = {}
    try:
        import subprocess
        result = subprocess.run(
            ['readelf', '-s', '-W', elf_path],
            capture_output=True,
            text=True,
            check=True
        )

        for line in result.stdout.split('\n'):
            # Skip header and empty lines
            if not line or 'Symbol table' in line or 'Num:' in line or line.startswith(' '):
                continue

            parts = line.split()
            if len(parts) >= 7:
                # Format: Value  Size Type    Bind Vis Ndx Name
                try:
                    value = int(parts[1], 16)
                    size = int(parts[2], 10)
                    name = parts[7] if len(parts) > 7 else ""

                    # Skip special symbols
                    if name and not name.startswith('$') and value != 0:
                        symbols[name] = (value, size)
                except (ValueError, IndexError):
                    continue

    except (subprocess.CalledProcessError, FileNotFoundError) as e:
        print(f"{Colors.YELLOW}Warning: Could not parse ELF symbols: {e}{Colors.RESET}")

    return symbols


def read_data_from_rom(rom_path: str, rom_addr: int, size: int) -> bytes:
    """Read data bytes from ROM file at given ROM address."""
    with open(rom_path, 'rb') as f:
        f.seek(rom_addr)
        return f.read(size)


def read_data_from_elf(elf_path: str, vram_addr: int, size: int) -> bytes:
    """Read data bytes from ELF file at given VRAM address."""
    try:
        import subprocess
        # Use objdump to extract data at the specific address
        result = subprocess.run(
            ['objdump', '-s', '-j', '.data', '-j', '.rodata', elf_path],
            capture_output=True,
            text=True,
            check=True
        )

        # Parse objdump output to find our data
        # This is a simplified approach - in reality, we'd need to properly
        # map vram_addr to file offset
        for line in result.stdout.split('\n'):
            if ' ' in line:
                parts = line.split()
                if parts and len(parts[0]) == 8:
                    try:
                        addr = int(parts[0], 16)
                        if addr == vram_addr:
                            # Extract hex bytes
                            bytes_data = []
                            for part in parts[1:-1]:  # Skip address and ascii
                                if len(part) == 2 and all(c in '0123456789abcdefABCDEF' for c in part):
                                    bytes_data.append(int(part, 16))
                            if bytes_data:
                                return bytes(bytes_data[:size])
                    except ValueError:
                        continue

    except (subprocess.CalledProcessError, FileNotFoundError):
        pass

    # Fallback: read from build binary
    return b''


def format_bytes(data: bytes, width: int = 16) -> List[str]:
    """Format bytes into hex display lines."""
    lines = []
    for i in range(0, len(data), width):
        chunk = data[i:i+width]
        hex_part = ' '.join(f'{b:02x}' for b in chunk)
        ascii_part = ''.join(chr(b) if 32 <= b < 127 else '.' for b in chunk)
        lines.append(f"  {hex_part:<{width*3-1}}  {ascii_part}")
    return lines


def guess_data_type(data: bytes, size: int) -> str:
    """Guess the data type based on size and content."""
    if size == 1:
        return "u8"
    elif size == 2:
        return "u16"
    elif size == 4:
        # Check if it looks like a float
        try:
            val = struct.unpack('>f', data)[0]
            if not val.is_integer() or abs(val) > 1000000:
                return "float"
        except:
            pass
        return "s32"
    elif size % 4 == 0:
        return f"s32[{size//4}]"
    else:
        return f"u8[{size}]"


def format_value(data: bytes, data_type: Optional[str]) -> str:
    """Format data value based on type."""
    if data_type == 'char' or data_type == 'string':
        try:
            return f'"{data.rstrip(b"\\x00").decode("ascii", errors="replace")}"'
        except:
            pass

    if len(data) == 1:
        return str(data[0])
    elif len(data) == 2:
        return str(struct.unpack('>H', data)[0])
    elif len(data) == 4:
        # Try as int first
        val = struct.unpack('>I', data)[0]
        return f"{val} (0x{val:08x})"
    else:
        # Show as hex array
        return ' '.join(f'{b:02x}' for b in data[:16]) + ('...' if len(data) > 16 else '')


def check_data_match(target_rom: str, compiled_rom: str, rom_addr: int, size: int) -> Tuple[bool, bytes, bytes]:
    """Check if data matches between target and compiled ROMs."""
    try:
        target_data = read_data_from_rom(target_rom, rom_addr, size)
        compiled_data = read_data_from_rom(compiled_rom, rom_addr, size)
        return (target_data == compiled_data, target_data, compiled_data)
    except Exception:
        return (False, b'', b'')


def diff_data(target_data: bytes, compiled_data: bytes, symbol: str, data_type: Optional[str]) -> bool:
    """Compare two data byte arrays and display differences."""
    matches = True

    if target_data == compiled_data:
        print(f"{Colors.GREEN}{Colors.BOLD}✓ Data matches perfectly!{Colors.RESET}")
        return True

    print(f"{Colors.YELLOW}Data differs. Showing comparison:{Colors.RESET}\n")

    # Determine display width
    width = 16

    # Calculate max length
    max_len = max(len(target_data), len(compiled_data))

    if max_len <= 32:
        # Small data: show complete hex dump
        print(f"{Colors.CYAN}Target ROM data:{Colors.RESET}")
        for line in format_bytes(target_data, width):
            print(line)

        print(f"\n{Colors.CYAN}Compiled data:{Colors.RESET}")
        for line in format_bytes(compiled_data, width):
            print(line)
    else:
        # Large data: show first and last 16 bytes, with summary of differences
        print(f"{Colors.CYAN}Target ROM data (showing first and last 16 bytes of {len(target_data)} total):{Colors.RESET}")
        for line in format_bytes(target_data[:16], width):
            print(line)
        if len(target_data) > 16:
            print(f"  ... ({len(target_data) - 32} more bytes) ...")
            for line in format_bytes(target_data[-16:], width):
                print(line)

        print(f"\n{Colors.CYAN}Compiled data (showing first and last 16 bytes of {len(compiled_data)} total):{Colors.RESET}")
        for line in format_bytes(compiled_data[:16], width):
            print(line)
        if len(compiled_data) > 16:
            print(f"  ... ({len(compiled_data) - 32} more bytes) ...")
            for line in format_bytes(compiled_data[-16:], width):
                print(line)

    # Show byte-by-byte differences
    print(f"\n{Colors.BOLD}Byte differences:{Colors.RESET}")
    diff_count = 0
    max_diffs = 64  # Limit output

    for i in range(min(len(target_data), len(compiled_data))):
        if target_data[i] != compiled_data[i]:
            if diff_count < max_diffs:
                target_byte = target_data[i]
                compiled_byte = compiled_data[i]
                print(f"  Offset 0x{i:04x}: {Colors.RED}target=0x{target_byte:02x} ({target_byte}){Colors.RESET} "
                      f"vs {Colors.GREEN}compiled=0x{compiled_byte:02x} ({compiled_byte}){Colors.RESET}")
            diff_count += 1

    if len(target_data) != len(compiled_data):
        print(f"  {Colors.YELLOW}Size mismatch: target has {len(target_data)} bytes, compiled has {len(compiled_data)} bytes{Colors.RESET}")

    if diff_count > max_diffs:
        print(f"  ... and {diff_count - max_diffs} more differences")

    if diff_count > 0:
        print(f"\n{Colors.RED}{Colors.BOLD}Total differences: {diff_count} bytes{Colors.RESET}")

    return False


def find_first_mismatch(
    map_path: str,
    elf_path: str,
    target_rom: str,
    compiled_rom: str,
    filter_pattern: Optional[str] = None
) -> Optional[Tuple[str, int, int, int, str, bytes, bytes]]:
    """
    Find the first symbol (in ROM address order) that doesn't match.
    Parses the full ELF symbol table using readelf, not just symbol_addrs.txt.
    Returns tuple of (symbol_name, vram_addr, rom_addr, size, data_type, target_data, compiled_data)
    or None if all match.
    """
    # Build VRAM to ROM address mapping from linker map
    print(f"{Colors.BLUE}Parsing linker map...{Colors.RESET}")
    vram_to_rom_map = parse_vram_to_rom_map(map_path)

    # Get all symbols from ELF
    print(f"{Colors.BLUE}Extracting symbols from ELF...{Colors.RESET}")
    elf_symbols = parse_map_symbols(elf_path)

    # Filter symbols and convert VRAM to ROM addresses
    valid_symbols = []
    for name, (vram, size, dtype) in elf_symbols.items():
        if filter_pattern is None or filter_pattern.lower() in name.lower():
            rom_addr = get_rom_for_vram(vram, vram_to_rom_map)
            if rom_addr is not None:
                valid_symbols.append((name, vram, rom_addr, size, dtype))

    # Sort by ROM address
    valid_symbols.sort(key=lambda x: x[2])

    print(f"{Colors.BLUE}Scanning {len(valid_symbols)} symbols for mismatches...{Colors.RESET}")

    checked = 0
    for name, vram, rom, size, dtype in valid_symbols:
        checked += 1
        matches, target_data, compiled_data = check_data_match(target_rom, compiled_rom, rom, size)

        if not matches:
            print(f"\n{Colors.RED}{Colors.BOLD}Found mismatch at symbol #{checked}: {name}{Colors.RESET}")
            return (name, vram, rom, size, dtype, target_data, compiled_data)

        # Progress indicator
        if checked % 100 == 0:
            print(f"  Checked {checked} symbols...", end='\r')

    print(f"\n{Colors.GREEN}{Colors.BOLD}✓ All {checked} symbols match!{Colors.RESET}")
    return None


def main():
    parser = argparse.ArgumentParser(
        description="Diff binary data between compiled output and target ROM",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  python3 tools/data-differ/data_diff.py gLevelWorldTable
  python3 tools/data-differ/data_diff.py storyMapLocationCoords
  python3 tools/data-differ/data_diff.py D_8008D9C0
  python3 tools/data-differ/data_diff.py --find-first-mismatch
  python3 tools/data-differ/data_diff.py --find-first-mismatch --filter course
        """
    )

    parser.add_argument(
        "symbol",
        nargs='?',
        help="Symbol name to diff (e.g., gLevelWorldTable, D_8008D9C0). Not required with --find-first-mismatch."
    )

    parser.add_argument(
        "--find-first-mismatch",
        action="store_true",
        help="Scan all symbols in ROM order and show the first one that doesn't match"
    )

    parser.add_argument(
        "--filter",
        metavar="PATTERN",
        help="When using --find-first-mismatch, only check symbols matching this pattern"
    )

    parser.add_argument(
        "--target-rom",
        default="snowboardkids2.z64",
        help="Path to target ROM file (default: snowboardkids2.z64)"
    )

    parser.add_argument(
        "--compiled-rom",
        default="build/snowboardkids2.z64",
        help="Path to compiled ROM file (default: build/snowboardkids2.z64)"
    )

    parser.add_argument(
        "--elf",
        default="build/snowboardkids2.elf",
        help="Path to compiled ELF file (default: build/snowboardkids2.elf)"
    )

    parser.add_argument(
        "--symbol-addrs",
        default="symbol_addrs.txt",
        help="Path to symbol_addrs.txt (default: symbol_addrs.txt)"
    )

    parser.add_argument(
        "--map-file",
        default="snowboardkids2.map",
        help="Path to linker map file (default: snowboardkids2.map)"
    )

    parser.add_argument(
        "--size",
        type=lambda x: int(x, 0),
        help="Override data size (e.g., 0x10, 16, 0b10000)"
    )

    parser.add_argument(
        "--show-all",
        action="store_true",
        help="Show all data instead of just differences"
    )

    args = parser.parse_args()

    # Validate arguments
    if args.find_first_mismatch and args.symbol:
        print(f"{Colors.YELLOW}Warning: --find-first-mismatch ignores the symbol argument{Colors.RESET}", file=sys.stderr)

    if not args.find_first_mismatch and not args.symbol:
        parser.print_help()
        sys.exit(1)

    # Change to project root
    script_dir = Path(__file__).parent
    project_root = script_dir.parent.parent
    os.chdir(project_root)

    # Resolve file paths
    target_rom = Path(args.target_rom)
    compiled_rom = Path(args.compiled_rom)
    symbol_addrs = Path(args.symbol_addrs)
    map_file = Path(args.map_file)

    # Check if files exist
    if not target_rom.exists():
        print(f"{Colors.RED}Error: Target ROM not found: {target_rom}{Colors.RESET}", file=sys.stderr)
        sys.exit(1)

    if not compiled_rom.exists():
        print(f"{Colors.RED}Error: Compiled ROM not found: {compiled_rom}{Colors.RESET}", file=sys.stderr)
        print(f"{Colors.YELLOW}Hint: Run ./tools/build-and-verify.sh first{Colors.RESET}", file=sys.stderr)
        sys.exit(1)

    # Handle --find-first-mismatch mode (doesn't need symbol_addrs.txt)
    if args.find_first_mismatch:
        if not map_file.exists():
            print(f"{Colors.RED}Error: Map file not found: {map_file}{Colors.RESET}", file=sys.stderr)
            sys.exit(1)

        elf_file = Path(args.elf)
        if not elf_file.exists():
            print(f"{Colors.RED}Error: ELF file not found: {elf_file}{Colors.RESET}", file=sys.stderr)
            sys.exit(1)

        result = find_first_mismatch(
            str(map_file),
            str(elf_file),
            str(target_rom),
            str(compiled_rom),
            args.filter
        )

        if result is None:
            # All matched
            sys.exit(0)

        # Found a mismatch, display it
        symbol_name, vram_addr, rom_addr, size, data_type, target_data, compiled_data = result
        print(f"{Colors.BOLD}Symbol: {symbol_name}{Colors.RESET}")
        print(f"  VRAM:  0x{vram_addr:08x}")
        print(f"  ROM:   0x{rom_addr:08x}")
        print(f"  Size:  0x{size:x} ({size} bytes)")
        if data_type:
            print(f"  Type:  {data_type}")
        print()

        matches = diff_data(target_data, compiled_data, symbol_name, data_type)
        sys.exit(0 if matches else 1)

    # For single symbol mode, need symbol_addrs.txt
    if not symbol_addrs.exists():
        print(f"{Colors.RED}Error: Symbol addresses file not found: {symbol_addrs}{Colors.RESET}", file=sys.stderr)
        sys.exit(1)

    # Load symbols from symbol_addrs.txt for single symbol lookup
    print(f"{Colors.BLUE}Loading symbols from {symbol_addrs}...{Colors.RESET}")
    symbols = load_symbol_addrs(str(symbol_addrs))

    # Find the requested symbol
    if not args.symbol:
        print(f"{Colors.RED}Error: No symbol specified. Use --find-first-mismatch to scan all symbols.{Colors.RESET}", file=sys.stderr)
        sys.exit(1)

    symbol_name = args.symbol
    if symbol_name not in symbols:
        # Try to find partial matches
        matches = [s for s in symbols.keys() if symbol_name.lower() in s.lower()]
        if matches:
            print(f"{Colors.YELLOW}Symbol '{symbol_name}' not found. Did you mean:{Colors.RESET}")
            for m in matches[:10]:
                print(f"  - {m}")
            if len(matches) > 10:
                print(f"  ... and {len(matches) - 10} more")
        else:
            print(f"{Colors.RED}Error: Symbol '{symbol_name}' not found in symbol_addrs.txt{Colors.RESET}", file=sys.stderr)
        sys.exit(1)

    vram_addr, rom_addr, size, data_type = symbols[symbol_name]

    # Use override size if provided
    if args.size is not None:
        size = args.size

    if size is None:
        print(f"{Colors.RED}Error: No size specified for symbol '{symbol_name}'. Use --size to specify.{Colors.RESET}", file=sys.stderr)
        sys.exit(1)

    print(f"{Colors.BOLD}Symbol: {symbol_name}{Colors.RESET}")
    print(f"  VRAM:  0x{vram_addr:08x}")
    print(f"  ROM:   0x{rom_addr:08x}")
    print(f"  Size:  0x{size:x} ({size} bytes)")
    if data_type:
        print(f"  Type:  {data_type}")
    print()

    # Read data from both ROMs
    try:
        target_data = read_data_from_rom(str(target_rom), rom_addr, size)
        compiled_data = read_data_from_rom(str(compiled_rom), rom_addr, size)
    except FileNotFoundError as e:
        print(f"{Colors.RED}Error: {e}{Colors.RESET}", file=sys.stderr)
        sys.exit(1)
    except Exception as e:
        print(f"{Colors.RED}Error reading ROM: {e}{Colors.RESET}", file=sys.stderr)
        sys.exit(1)

    # Show data comparison
    if args.show_all or target_data != compiled_data:
        matches = diff_data(target_data, compiled_data, symbol_name, data_type)
    else:
        print(f"{Colors.GREEN}{Colors.BOLD}✓ Data matches perfectly!{Colors.RESET}")
        matches = True

    sys.exit(0 if matches else 1)


if __name__ == "__main__":
    main()
