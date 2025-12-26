#!/usr/bin/env python3
"""
Decode N64 controller button bitmasks.
Takes a hex number and returns which buttons are pressed.
"""

import sys

# Button definitions from lib/ultralib/include/PR/os_cont.h
BUTTONS = {
    0x80000: "STICK_LEFT",
    0x40000: "STICK_RIGHT",
    0x20000: "STICK_DOWN",
    0x10000: "STICK_UP",
    0x8000: "A_BUTTON (CONT_A)",
    0x4000: "B_BUTTON (CONT_B)",
    0x2000: "Z_TRIG (CONT_G)",
    0x1000: "START_BUTTON (CONT_START)",
    0x0800: "U_JPAD (CONT_UP)",
    0x0400: "D_JPAD (CONT_DOWN)",
    0x0200: "L_JPAD (CONT_LEFT)",
    0x0100: "R_JPAD (CONT_RIGHT)",
    0x0020: "L_TRIG (CONT_L)",
    0x0010: "R_TRIG (CONT_R)",
    0x0008: "U_CBUTTONS (CONT_E)",
    0x0004: "D_CBUTTONS (CONT_D)",
    0x0002: "L_CBUTTONS (CONT_C)",
    0x0001: "R_CBUTTONS (CONT_F)",
}

def decode_buttons(value):
    """Decode a button bitmask and return list of pressed buttons."""
    if isinstance(value, str):
        # Parse hex string (with or without 0x prefix)
        value = int(value, 16) if not value.startswith('0x') else int(value, 0)

    pressed = []
    remaining = value

    # Check each button mask
    for mask, name in sorted(BUTTONS.items(), reverse=True):
        if value & mask:
            pressed.append((mask, name))
            remaining &= ~mask

    return pressed, remaining

def main():
    if len(sys.argv) < 2:
        print("Usage: python3 decode_buttons.py <hex_value>")
        print("Example: python3 decode_buttons.py 0x10800")
        print("Example: python3 decode_buttons.py 9000")
        return 1

    value = sys.argv[1]

    try:
        pressed, remaining = decode_buttons(value)

        # Parse the original value for display
        num_value = int(value, 16) if not value.startswith('0x') else int(value, 0)

        print(f"Bitmask: {value} (decimal: {num_value}, binary: {bin(num_value)})")
        print()

        if pressed:
            print("Buttons pressed:")
            for mask, name in pressed:
                print(f"  - 0x{mask:04X} = {name}")
        else:
            print("No recognized buttons pressed")

        if remaining != 0:
            print()
            print(f"Warning: Unrecognized bits: 0x{remaining:04X}")

        return 0

    except ValueError as e:
        print(f"Error: Invalid hex value '{value}'")
        print(f"Details: {e}")
        return 1

if __name__ == "__main__":
    sys.exit(main())
