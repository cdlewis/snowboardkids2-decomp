#!/usr/bin/env python3
"""
Font character mapping for Snowboard Kids 2.

Text encoding format:
- Bits 12-15: Character width in pixels (0 = default 12)
- Bits 0-11: Character index into font table (0-93)
- Control codes:
  - 0xFFFB - spacing
  - 0xFFFD - newline
  - 0xFFFF - end
"""

# Character index to character mapping
# Based on visual inspection of decompressed font texture

# Big glyphs (indices 0-35)
BIG_GLYPHS = {
    0: '0',
    1: '1',
    2: '2',
    3: '3',
    4: '4',
    5: '5',
    6: '6',
    7: '7',
    8: '8',
    9: '9',
    10: 'A',
    11: 'B',
    12: 'C',
    13: 'D',
    14: 'E',
    15: 'F',
    16: 'G',
    17: 'H',
    18: 'I',
    19: 'J',
    20: 'K',
    21: 'L',
    22: 'M',
    23: 'N',
    24: 'O',
    25: 'P',
    26: 'Q',
    27: 'R',
    28: 'S',
    29: 'T',
    30: 'U',
    31: 'V',
    32: 'W',
    33: 'X',
    34: 'Y',
    35: 'Z',
}

# Small glyphs (indices 36-93)
SMALL_GLYPHS = {
    36: '0',  # small digit
    37: '1',  # small digit
    38: '2',  # small digit
    39: '3',  # small digit
    40: '4',  # small digit
    41: '5',  # small digit
    42: '6',  # small digit
    43: '7',  # small digit
    44: '8',  # small digit
    45: '9',  # small digit
    46: 'a',
    47: 'b',
    48: 'c',
    49: 'd',
    50: 'e',
    51: 'f',
    52: 'g',
    53: 'h',
    54: 'i',
    55: 'j',
    56: 'k',
    57: 'l',
    58: 'm',
    59: 'n',
    60: 'o',
    61: 'p',
    62: 'q',
    63: 'r',
    64: 's',
    65: 't',
    66: 'u',
    67: 'v',
    68: 'w',
    69: 'x',
    70: 'y',
    71: 'z',
    72: '!',  # confirmed
    73: '"',
    74: '#',
    75: '`',
    76: '*',
    77: '+',
    78: ',',
    79: '-',
    80: '.',  # confirmed
    81: '/',
    82: ';',
    83: '?',
    84: '\u201C',  # open quote
    85: '@',  # confirmed (close quote or at-sign)
    86: '\u256D',  # ╭ quarter circle top-left (box corner)
    87: '\u256F',  # ╯ quarter circle bottom-right (box corner)
    88: '~',  # confirmed
    89: '(',  # confirmed
    90: ')',  # confirmed
    91: '&',  # confirmed
    92: '\u00B7',  # middle dot (superscripted period)
    93: '\u25CE',  # circle with exclamation - confirmed
}

# Combined mapping
CHAR_MAP = {**BIG_GLYPHS, **SMALL_GLYPHS}

# Reverse mapping (character to index)
# Note: some characters appear twice (big and small versions)
CHAR_TO_INDEX = {}
for idx, char in CHAR_MAP.items():
    if char is not None:
        if char not in CHAR_TO_INDEX:
            CHAR_TO_INDEX[char] = []
        CHAR_TO_INDEX[char].append(idx)


def decode_text(encoded_data: bytes) -> str:
    """Decode encoded text data to a string."""
    result = []
    i = 0

    while i < len(encoded_data):
        if i + 1 >= len(encoded_data):
            break

        # Read 16-bit big-endian value
        code = (encoded_data[i] << 8) | encoded_data[i + 1]
        i += 2

        if code == 0xFFFF:
            break  # End marker
        elif code == 0xFFFD:
            result.append('\n')
        elif code == 0xFFFB:
            result.append(' ')  # spacing
        elif code == 0xFFFE:
            result.append(' ')  # spacing
        elif code == 0xFFFC:
            pass  # Skip marker
        else:
            # Extract width (bits 12-15) and index (bits 0-11)
            width = (code >> 12) & 0xF
            char_index = code & 0xFFF

            if width == 0:
                width = 12  # Default width

            if char_index in CHAR_MAP and CHAR_MAP[char_index] is not None:
                result.append(CHAR_MAP[char_index])
            else:
                result.append(f'[{char_index}]')

    return ''.join(result)


def encode_text(text: str, use_big: bool = True) -> bytes:
    """Encode a string to the game's text format."""
    result = []

    for char in text:
        if char == '\n':
            result.append(0xFFFD)
        elif char == ' ':
            result.append(0xFFFB)
        else:
            # Find character index
            indices = CHAR_TO_INDEX.get(char, [])
            if indices:
                # Prefer big or small based on use_big flag
                if use_big:
                    idx = next((i for i in indices if i < 36), indices[0])
                else:
                    idx = next((i for i in indices if i >= 36), indices[0])

                # Encode with default width (12 = 0, so bits 12-15 are 0)
                result.append(idx)
            else:
                # Unknown character, skip
                print(f"Warning: Unknown character '{char}'")

    # Add end marker
    result.append(0xFFFF)

    # Convert to bytes (big-endian)
    return b''.join((code >> 8).to_bytes(1, 'big') + (code & 0xFF).to_bytes(1, 'big') for code in result)


if __name__ == '__main__':
    # Test the mapping
    print("Big Glyphs (0-35):")
    for i in range(36):
        char = CHAR_MAP.get(i, '?')
        print(f"  {i:2d}: '{char}'")

    print("\nSmall Glyphs (36-93):")
    for i in range(36, 94):
        char = CHAR_MAP.get(i, '?')
        print(f"  {i:2d}: '{char}'")

    print("\nCharacter to Index mapping:")
    for char, indices in sorted(CHAR_TO_INDEX.items()):
        print(f"  '{char}': {indices}")
