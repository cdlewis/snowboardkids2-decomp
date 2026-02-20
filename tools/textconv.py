#!/usr/bin/env python3
"""
Text conversion tool for Snowboard Kids 2.

Converts _("string") syntax to hex bytes using a charmap file.
This allows text strings to be written in a readable format while
producing the exact byte sequences expected by the game's font system.

Usage:
    python3 tools/textconv.py tools/charmap.txt input.c output.c

The tool finds all _("string") patterns and converts them to hex byte arrays.
An END terminator (0xFF, 0xFF) is automatically appended to each string.
Digit sizing is context-aware:
    - After lowercase letter: small digit (e.g., "by 8:30" -> small 8)
    - After uppercase letter: big digit (e.g., "LEVEL 1" -> big 1)
    - Start of string (no preceding letter): small digit
    - Explicit override: {0B}-{9B} for big, {0s}-{9s} for small
"""

import sys
import re
from typing import Dict, List, Tuple, Optional


def unescape_char(char: str) -> str:
    """Convert escape sequences in a character to actual characters."""
    if char == '\\n':
        return '\n'
    elif char == '\\0':
        return '\0'
    elif char == '\\t':
        return '\t'
    elif char == '\\\\':
        return '\\'
    return char


def parse_charmap(filepath: str) -> Dict[str, Tuple[int, int]]:
    """Parse charmap file and return mapping of characters to (high, low) byte tuples."""
    charmap = {}

    with open(filepath, 'r') as f:
        for line in f:
            line = line.strip()

            # Skip empty lines and comments
            if not line or line.startswith('//'):
                continue

            # Parse: 'char' = 0xHH, 0xLL or '_name' = 0xHH, 0xLL
            match = re.match(r"('([^']*)'|(\w+))\s*=\s*(0x[0-9A-Fa-f]+)\s*,\s*(0x[0-9A-Fa-f]+)", line)
            if match:
                if match.group(2) is not None:
                    char = match.group(2)
                else:
                    char = match.group(3)
                # Unescape special characters
                char = unescape_char(char)
                high = int(match.group(4), 16)
                low = int(match.group(5), 16)
                charmap[char] = (high, low)

    return charmap


def find_preceding_letter(text: str, pos: int) -> Optional[str]:
    """
    Find the nearest letter before position pos in text.
    Returns 'lower' for lowercase, 'upper' for uppercase, or None if no letter found.
    """
    for i in range(pos - 1, -1, -1):
        c = text[i]
        if c.islower():
            return 'lower'
        elif c.isupper():
            return 'upper'
        # Continue scanning past spaces, punctuation, etc.
    return None


def convert_string(text: str, charmap: Dict[str, Tuple[int, int]]) -> List[int]:
    """
    Convert a string to a list of byte values using the charmap.

    Digit sizing rules:
    - {0B} through {9B}: explicitly big digit
    - {0s} through {9s}: explicitly small digit
    - After lowercase letter: small digit
    - After uppercase letter: big digit
    - No preceding letter (string start): small digit
    """
    result = []
    i = 0

    while i < len(text):
        c = text[i]

        # Handle explicit digit overrides {0B}-{9B} and {0s}-{9s}
        if c == '{' and i + 2 < len(text) and text[i + 2] in 'BSbs' and text[i + 3] == '}':
            digit = text[i + 1]
            if digit.isdigit():
                size = text[i + 2].upper()
                key = f'_{digit}{size}'
                if key in charmap:
                    high, low = charmap[key]
                    result.extend([high, low])
                i += 4
                continue

        # Handle escape sequences
        if c == '\\' and i + 1 < len(text):
            next_char = text[i + 1]
            if next_char == 'n':
                if '\n' in charmap:
                    high, low = charmap['\n']
                    result.extend([high, low])
                i += 2
                continue
            elif next_char == '0':
                if '\0' in charmap:
                    high, low = charmap['\0']
                    result.extend([high, low])
                i += 2
                continue
            elif next_char == 't':
                # Tab - treat as space
                if ' ' in charmap:
                    high, low = charmap[' ']
                    result.extend([high, low])
                i += 2
                continue
            elif next_char == '\\':
                c = '\\'
                i += 1
            elif next_char == '"':
                c = '"'
                i += 1
            elif next_char == '{':
                c = '{'
                i += 1
            elif next_char == '}':
                c = '}'
                i += 1
            else:
                i += 1
                continue

        # Handle digits with context-aware sizing
        if c.isdigit():
            letter_type = find_preceding_letter(text, i)

            # Determine which key to use
            if letter_type == 'upper':
                key = f'_{c}B'  # Big digit after uppercase
            else:
                key = f'_{c}s'  # Small digit after lowercase or at start

            if key in charmap:
                high, low = charmap[key]
                result.extend([high, low])
            else:
                raise ValueError(f"Unknown digit mapping for '{c}' with context '{letter_type}'")
            i += 1
            continue

        # Handle actual null character (from unescaped \0)
        if c == '\0':
            if '\0' in charmap:
                high, low = charmap['\0']
                result.extend([high, low])
            i += 1
            continue

        # Handle actual newline character (from unescaped \n)
        if c == '\n':
            if '\n' in charmap:
                high, low = charmap['\n']
                result.extend([high, low])
            i += 1
            continue

        # Regular character lookup
        if c in charmap:
            high, low = charmap[c]
            result.extend([high, low])
        else:
            raise ValueError(f"Unknown character '{c}' (ord={ord(c)}) at position {i}")
        i += 1

    return result


def format_bytes_as_array(bytes_list: List[int]) -> str:
    """Format a list of bytes as a C array initializer."""
    return ', '.join(f'0x{b:02X}' for b in bytes_list)


def process_file(input_path: str, output_path: str, charmap: Dict[str, Tuple[int, int]]) -> None:
    """
    Process a C file, converting all _("string") patterns to hex byte arrays.

    This preserves all other content including comments and regular strings.
    Only processes files that include font_encoding.h.
    """
    if input_path == '-' or input_path == '/dev/stdin':
        content = sys.stdin.read()
    else:
        with open(input_path, 'r') as f:
            content = f.read()

    # Skip files that don't include font_encoding.h
    if '#include' not in content or 'font_encoding.h' not in content:
        # Just copy the file through unchanged
        if output_path == '-' or output_path == '/dev/stdout':
            sys.stdout.write(content)
        else:
            with open(output_path, 'w') as f:
                f.write(content)
        return

    result = []
    pos = 0

    # Pattern to find _("string") - being careful about escape sequences
    # We need to handle nested quotes and escapes properly
    # Use negative lookbehind to avoid matching inside identifiers like __asm__(
    pattern = re.compile(r'(?<![a-zA-Z0-9_])_\("')

    while pos < len(content):
        match = pattern.search(content, pos)

        if match is None:
            # No more matches, copy rest of file
            result.append(content[pos:])
            break

        # Copy content before match
        result.append(content[pos:match.start()])

        # Check if we're inside a comment (simple check)
        # Look back for // or /* without closing
        before = content[:match.start()]
        line_start = before.rfind('\n') + 1
        line_before = before[line_start:]

        # Skip if in single-line comment
        if '//' in line_before:
            result.append(match.group())
            pos = match.end()
            continue

        # Skip if in multi-line comment (simplified check)
        if '/*' in before:
            last_open = before.rfind('/*')
            last_close = before.rfind('*/')
            if last_open > last_close:
                result.append(match.group())
                pos = match.end()
                continue

        # Find the end of the string
        string_start = match.end()  # Position after opening "
        string_content = []

        i = string_start
        while i < len(content):
            c = content[i]
            if c == '\\' and i + 1 < len(content):
                # Escape sequence
                string_content.append(c)
                string_content.append(content[i + 1])
                i += 2
            elif c == '"':
                # End of string
                break
            else:
                string_content.append(c)
                i += 1

        string_text = ''.join(string_content)

        # Convert the string to bytes (auto-append END terminator)
        try:
            bytes_list = convert_string(string_text, charmap)
            if '\0' in charmap:
                high, low = charmap['\0']
                bytes_list.extend([high, low])
            result.append(format_bytes_as_array(bytes_list))
        except ValueError as e:
            # On error, keep original and report
            print(f"Error converting string at position {match.start()}: {e}", file=sys.stderr)
            print(f"  String: {string_text[:50]}...", file=sys.stderr)
            result.append(match.group())
            result.append(string_text)
            result.append('"')

        # Move past closing quote and the closing parenthesis
        pos = i + 1  # Move past closing quote
        if pos < len(content) and content[pos] == ')':
            pos += 1  # Move past closing parenthesis

    # Write output
    if output_path == '-' or output_path == '/dev/stdout':
        sys.stdout.write(''.join(result))
    else:
        with open(output_path, 'w') as f:
            f.write(''.join(result))


def main():
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <charmap.txt> <input.c> [output.c]", file=sys.stderr)
        print("  If output.c is not specified, prints converted strings to stdout.", file=sys.stderr)
        sys.exit(1)

    charmap_path = sys.argv[1]
    input_path = sys.argv[2]
    output_path = sys.argv[3] if len(sys.argv) > 3 else None

    # Load charmap
    charmap = parse_charmap(charmap_path)

    if output_path:
        process_file(input_path, output_path, charmap)
        if output_path != '-' and output_path != '/dev/stdout':
            print(f"Converted {input_path} -> {output_path}", file=sys.stderr)
    else:
        # Just test conversion of strings passed on command line
        with open(input_path, 'r') as f:
            for line in f:
                # Extract _("...") patterns and show conversion
                for match in re.finditer(r'_\("([^"]*)"', line):
                    text = match.group(1)
                    # Unescape
                    text = text.replace('\\n', '\n').replace('\\0', '\0').replace('\\t', '\t')
                    text = text.replace('\\"', '"').replace('\\\\', '\\')
                    try:
                        bytes_list = convert_string(text, charmap)
                        print(f'_{repr(text)}: {format_bytes_as_array(bytes_list)}')
                    except ValueError as e:
                        print(f'Error: {e}')


if __name__ == '__main__':
    main()
