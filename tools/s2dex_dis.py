#!/usr/bin/env python3
"""
S2DEX Command Disassembler

This utility disassembles 64-bit S2DEX command words and outputs the corresponding
macro calls, similar to how gfxdis works for F3DEX2 commands.

Usage:
    ./tools/s2dex_dis.py <64-bit hex value>

Example:
    ./tools/s2dex_dis.py 0x0b00000000000000
    # Output: gsSPObjRenderMode(0x00)
"""

import sys
import argparse

# S2DEX opcode definitions for F3DEX_GBI_2
S2DEX_OPCODES_F3DEX_GBI_2 = {
    0xDA: ("G_OBJ_RECTANGLE_R", "gsSPObjRectangleR", "gSPObjRectangleR", False),
    0xDC: ("G_OBJ_MOVEMEM", "gsSPObjMatrix", "gSPObjMatrix", True),  # or gsSPObjSubMatrix
    0xE4: ("G_RDPHALF_0", "gsRDPHalf_0", "gRDPHalf_0", False),
    0x01: ("G_OBJ_RECTANGLE", "gsSPObjRectangle", "gSPObjRectangle", False),
    0x02: ("G_OBJ_SPRITE", "gsSPObjSprite", "gSPObjSprite", False),
    0x04: ("G_SELECT_DL", "gsSPSelectDL", "gSPSelectDL", True),  # Special multi-word command
    0x05: ("G_OBJ_LOADTXTR", "gsSPObjLoadTxtr", "gSPObjLoadTxtr", False),
    0x06: ("G_OBJ_LDTX_SPRITE", "gsSPObjLoadTxSprite", "gSPObjLoadTxSprite", False),
    0x07: ("G_OBJ_LDTX_RECT", "gsSPObjLoadTxRect", "gSPObjLoadTxRect", False),
    0x08: ("G_OBJ_LDTX_RECT_R", "gsSPObjLoadTxRectR", "gSPObjLoadTxRectR", False),
    0x09: ("G_BG_1CYC", "gsSPBgRect1Cyc", "gSPBgRect1Cyc", False),
    0x0A: ("G_BG_COPY", "gsSPBgRectCopy", "gSPBgRectCopy", False),
    0x0B: ("G_OBJ_RENDERMODE", "gsSPObjRenderMode", "gSPObjRenderMode", False),
}

# S2DEX opcode definitions for standard (non-F3DEX_GBI_2)
S2DEX_OPCODES_STANDARD = {
    0x01: ("G_BG_1CYC", "gsSPBgRect1Cyc", "gSPBgRect1Cyc", False),
    0x02: ("G_BG_COPY", "gsSPBgRectCopy", "gSPBgRectCopy", False),
    0x03: ("G_OBJ_RECTANGLE", "gsSPObjRectangle", "gSPObjRectangle", False),
    0x04: ("G_OBJ_SPRITE", "gsSPObjSprite", "gSPObjSprite", False),
    0x05: ("G_OBJ_MOVEMEM", "gsSPObjMatrix", "gSPObjMatrix", True),
    0xB0: ("G_SELECT_DL", "gsSPSelectDL", "gSPSelectDL", True),
    0xB1: ("G_OBJ_RENDERMODE", "gsSPObjRenderMode", "gSPObjRenderMode", False),
    0xB2: ("G_OBJ_RECTANGLE_R", "gsSPObjRectangleR", "gSPObjRectangleR", False),
    0xC1: ("G_OBJ_LOADTXTR", "gsSPObjLoadTxtr", "gSPObjLoadTxtr", False),
    0xC2: ("G_OBJ_LDTX_SPRITE", "gsSPObjLoadTxSprite", "gSPObjLoadTxSprite", False),
    0xC3: ("G_OBJ_LDTX_RECT", "gsSPObjLoadTxRect", "gSPObjLoadTxRect", False),
    0xC4: ("G_OBJ_LDTX_RECT_R", "gsSPObjLoadTxRectR", "gSPObjLoadTxRectR", False),
    0xE4: ("G_RDPHALF_0", "gsRDPHalf_0", "gRDPHalf_0", False),
}


def parse_hex_value(hex_str):
    """Parse a hex string into a 64-bit integer."""
    hex_str = hex_str.strip().lower()
    if hex_str.startswith("0x"):
        hex_str = hex_str[2:]

    try:
        value = int(hex_str, 16)
        return value
    except ValueError:
        raise ValueError(f"Invalid hex value: {hex_str}")


def extract_opcode(value):
    """Extract opcode from a 64-bit Gfx command word.

    The opcode is in bits 24-31 of the high word (bits 56-63 of the full 64-bit value).
    """
    hi = (value >> 32) & 0xFFFFFFFF
    opcode = (hi >> 24) & 0xFF
    return opcode, hi, (value & 0xFFFFFFFF)


def format_macro_call(gs_macro, args):
    """Format a macro call with arguments."""
    if not args:
        return f"{gs_macro}()"
    return f"{gs_macro}({', '.join(args)})"


def decode_obj_render_mode(mode):
    """Decode S2DEX object render mode flags into symbolic names."""
    flags = {
        0x01: "G_OBJRM_NOTXCLAMP",
        0x02: "G_OBJRM_XLU",
        0x04: "G_OBJRM_ANTIALIAS",
        0x08: "G_OBJRM_BILERP",
        0x10: "G_OBJRM_SHRINKSIZE_1",
        0x20: "G_OBJRM_SHRINKSIZE_2",
        0x40: "G_OBJRM_WIDEN",
    }

    if mode == 0:
        return "0"

    parts = []
    remaining = mode

    for bit, name in flags.items():
        if mode & bit:
            parts.append(name)
            remaining &= ~bit

    if remaining:
        parts.append(f"0x{remaining:02X}")

    if len(parts) == 0:
        return f"0x{mode:02X}"
    elif len(parts) == 1:
        return parts[0]
    else:
        return " | ".join(parts)


def dis_s2dex_command(value, use_f3dex_gbi_2=True):
    """Disassemble a 64-bit S2DEX command word."""
    opcode, hi, lo = extract_opcode(value)

    # Choose opcode table based on variant
    opcode_table = S2DEX_OPCODES_F3DEX_GBI_2 if use_f3dex_gbi_2 else S2DEX_OPCODES_STANDARD

    if opcode not in opcode_table:
        return [f"# Unknown opcode: 0x{opcode:02X} (hi=0x{hi:08X}, lo=0x{lo:08X})"]

    name, gs_macro, g_macro, is_dma = opcode_table[opcode]

    # Handle different command types
    if name == "G_OBJ_RENDERMODE":
        # gsSPObjRenderMode(mode) - mode is in the lo word
        mode = lo & 0xFF
        decoded_mode = decode_obj_render_mode(mode)
        return [format_macro_call(gs_macro, [decoded_mode])]

    elif name == "G_BG_1CYC" or name == "G_BG_COPY":
        # gsSPBgRectCopy(ptr) / gsSPBgRect1Cyc(ptr) - pointer is in lo word
        if lo == 0:
            return [format_macro_call(gs_macro, ["ptr"])]
        return [format_macro_call(gs_macro, [f"0x{lo:08X}"])]

    elif name == "G_OBJ_SPRITE" or name == "G_OBJ_RECTANGLE" or name == "G_OBJ_RECTANGLE_R":
        # gsSPObjSprite(ptr) - pointer is in lo word
        if lo == 0:
            return [format_macro_call(gs_macro, ["ptr"])]
        return [format_macro_call(gs_macro, [f"0x{lo:08X}"])]

    elif name == "G_OBJ_MOVEMEM":
        # gsSPObjMatrix(ptr) or gsSPObjSubMatrix(ptr)
        # Can differentiate by the segment field (bits 16-19 of hi word)
        segment = (hi >> 16) & 0xF
        if lo == 0:
            ptr_arg = "ptr"
        else:
            ptr_arg = f"0x{lo:08X}"

        if segment == 0:
            return [format_macro_call("gsSPObjMatrix", [ptr_arg])]
        elif segment == 2:
            return [format_macro_call("gsSPObjSubMatrix", [ptr_arg])]
        else:
            return [format_macro_call(gs_macro, [ptr_arg])]

    elif name == "G_OBJ_LOADTXTR":
        # gsSPObjLoadTxtr(ptr) - pointer is in lo word
        if lo == 0:
            return [format_macro_call(gs_macro, ["ptr"])]
        return [format_macro_call(gs_macro, [f"0x{lo:08X}"])]

    elif name == "G_OBJ_LDTX_SPRITE":
        # gsSPObjLoadTxSprite(ptr) - pointer is in lo word
        if lo == 0:
            return [format_macro_call(gs_macro, ["ptr"])]
        return [format_macro_call(gs_macro, [f"0x{lo:08X}"])]

    elif name == "G_OBJ_LDTX_RECT":
        # gsSPObjLoadTxRect(ptr) - pointer is in lo word
        if lo == 0:
            return [format_macro_call(gs_macro, ["ptr"])]
        return [format_macro_call(gs_macro, [f"0x{lo:08X}"])]

    elif name == "G_OBJ_LDTX_RECT_R":
        # gsSPObjLoadTxRectR(ptr) - pointer is in lo word
        if lo == 0:
            return [format_macro_call(gs_macro, ["ptr"])]
        return [format_macro_call(gs_macro, [f"0x{lo:08X}"])]

    elif name == "G_SELECT_DL":
        # gsSPSelectDL - multi-word command, would need additional words
        # For now, just indicate it's a multi-word command
        return [f"# {gs_macro}(...) - multi-word command"]

    elif name == "G_RDPHALF_0":
        # gsRDPHalf_0 - part of multi-word commands
        return [f"# {gs_macro}(0x{hi:08X}, 0x{lo:08X}) - part of multi-word command"]

    else:
        # Generic handler
        if lo == 0:
            return [format_macro_call(gs_macro, ["..."])]
        return [format_macro_call(gs_macro, [f"0x{lo:08X}"])]


def main():
    parser = argparse.ArgumentParser(
        description="Disassemble 64-bit S2DEX command words",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  %(prog)s 0x0b00000000000000
  %(prog)s 0x0200000080123456
  %(prog)s --standard 0x0400000000000000
  %(prog)s 0x0500000000000000 0x0600000080123456
        """
    )

    parser.add_argument(
        "hex_values",
        nargs="+",
        help="64-bit hex value(s) to disassemble (e.g., 0x0b00000000000000)"
    )

    parser.add_argument(
        "-s", "--standard",
        action="store_true",
        help="Use standard S2DEX opcodes instead of F3DEX_GBI_2"
    )

    parser.add_argument(
        "-b", "--both",
        action="store_true",
        help="Show both F3DEX_GBI_2 and standard opcode interpretations"
    )

    parser.add_argument(
        "-g", "--g-macro",
        action="store_true",
        help="Output g* macros instead of gs* macros"
    )

    args = parser.parse_args()

    for hex_str in args.hex_values:
        try:
            value = parse_hex_value(hex_str)
            # Remove 0x prefix if present for cleaner output
            display_hex = hex_str.lower().replace("0x", "")
            print(f"; 0x{display_hex}")

            if args.both:
                # Show both interpretations
                print("; F3DEX_GBI_2:")
                for line in dis_s2dex_command(value, use_f3dex_gbi_2=True):
                    print(f"  {line}")
                print("; Standard:")
                for line in dis_s2dex_command(value, use_f3dex_gbi_2=False):
                    print(f"  {line}")
            else:
                # Show single interpretation
                use_f3dex_gbi_2 = not args.standard
                for line in dis_s2dex_command(value, use_f3dex_gbi_2=use_f3dex_gbi_2):
                    # Optionally replace gs* with g*
                    if args.g_macro and line.startswith("gs"):
                        line = "g" + line[2:]
                    print(line)
            print()

        except ValueError as e:
            print(f"Error: {e}", file=sys.stderr)
            sys.exit(1)


if __name__ == "__main__":
    main()
