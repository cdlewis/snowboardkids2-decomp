#!/usr/bin/env python3
"""Add standard field-offset comments to simple C typedef structs.

This is intentionally conservative: it formats plain field declarations inside
`typedef struct { ... } Name;` blocks and leaves lines it cannot understand
unchanged. Add project-local struct sizes to KNOWN_TYPES as needed.
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path


KNOWN_TYPES = {
    "s8": (1, 1),
    "u8": (1, 1),
    "char": (1, 1),
    "s16": (2, 2),
    "u16": (2, 2),
    "s32": (4, 4),
    "u32": (4, 4),
    "f32": (4, 4),
    "void": (4, 4),
    "Vec3i": (0x0C, 4),
    "Vec3s": (0x06, 2),
    "Transform3D": (0x20, 4),
    "Mtx": (4, 4),
    "Gfx": (8, 4),
    "Player": (4, 4),
    "Node": (0x2C, 4),
    "DisplayLists": (0x10, 4),
    "DisplayListObject": (0x3C, 4),
    "loadAssetMetadata_arg": (0x1C, 4),
    "TexturedBillboardSprite": (0x34, 4),
    "OutputStruct_19E80": (0x14, 4),
    "DataTable_19E80": (4, 4),
    "ItemBoxBurstFrameData": (0x180, 1),
    "TableEntry_19E80": (4, 4),
    "HalfwordBytes": (2, 2),
}

STRUCT_RE = re.compile(r"typedef struct\s*\{\n.*?\n}\s*[A-Za-z_][A-Za-z0-9_]*;", re.S)
OFFSET_PREFIX_RE = re.compile(r"^/\*\s*0x[0-9A-Fa-f]+\s*\*/\s*")
OLD_TRAILING_OFFSET_RE = re.compile(r"^0x[0-9A-Fa-f]+\s*-\s*")


def align(value: int, alignment: int) -> int:
    return (value + alignment - 1) // alignment * alignment


def safe_eval_array_count(expr: str) -> int:
    if not re.fullmatch(r"[0-9xXa-fA-F+\-*/ ()]+", expr):
        raise ValueError(f"unsupported array size expression: {expr}")
    return int(eval(expr, {"__builtins__": {}}, {}))


def split_comment(line: str) -> tuple[str, str]:
    stripped = OFFSET_PREFIX_RE.sub("", line.strip())
    if "//" in stripped:
        code, comment = stripped.split("//", 1)
        return code.rstrip(), "// " + comment.strip()

    match = re.search(r"\s*/\*.*?\*/\s*$", stripped)
    if match:
        code = stripped[: match.start()].rstrip()
        comment = match.group(0).strip()[2:-2].strip()
        comment = OLD_TRAILING_OFFSET_RE.sub("", comment)
        return code, ("// " + comment) if comment else ""

    return stripped, ""


def parse_field(line: str):
    code, comment = split_comment(line)
    code = code.rstrip(";").strip()
    if not code or code.startswith("struct {") or code == "} bytes":
        return None
    original_declaration = code
    code = re.sub(r"\[([^\]]+)\]", lambda match: "[" + match.group(1).replace(" ", "") + "]", code)
    if " " not in code:
        return None

    type_part, name_part = code.rsplit(" ", 1)
    type_part = type_part.strip()
    pointer = False

    if name_part.startswith("*"):
        pointer = True
        name_part = name_part[1:]
    elif type_part.endswith("*volatile"):
        pointer = True
        type_part = type_part[: -len("*volatile")].strip()
    elif type_part.endswith("*"):
        pointer = True
        type_part = type_part[:-1].strip()

    type_part = type_part.replace(" volatile", "").replace("volatile ", "").strip()

    array_suffix = ""
    array_match = re.match(r"([A-Za-z_][A-Za-z0-9_]*)(\[[^\]]+\])$", name_part)
    if array_match:
        name_part = array_match.group(1)
        array_suffix = array_match.group(2)

    if not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", name_part):
        return None

    return type_part, pointer, name_part, array_suffix, comment, original_declaration


def sizeof_type(type_name: str, pointer: bool, known_types: dict[str, tuple[int, int]]) -> tuple[int, int]:
    if pointer:
        return 4, 4
    if type_name not in known_types:
        raise KeyError(type_name)
    return known_types[type_name]


def array_count(array_suffix: str) -> int:
    if not array_suffix:
        return 1
    return safe_eval_array_count(array_suffix[1:-1].strip())


def offset_width(offset: int) -> int:
    if offset <= 0xFF:
        return 2
    if offset <= 0xFFF:
        return 3
    return 4


def format_struct_block(block: str, known_types: dict[str, tuple[int, int]], unknown: set[str]) -> str:
    lines = block.splitlines()
    output = []
    offset = 0
    max_alignment = 1
    changed = False

    for line in lines:
        stripped = line.strip()
        if (
            stripped.startswith("typedef struct")
            or stripped.startswith("}")
            or not stripped
            or stripped.startswith("struct {")
            or stripped == "} bytes;"
        ):
            output.append(line)
            continue

        parsed = parse_field(line)
        if parsed is None:
            output.append(line)
            continue

        type_name, pointer, field_name, array_suffix, comment, original_declaration = parsed
        try:
            size, alignment = sizeof_type(type_name, pointer, known_types)
        except KeyError:
            unknown.add(type_name)
            output.append(line)
            continue

        offset = align(offset, alignment)
        max_alignment = max(max_alignment, alignment)

        declaration = f"{original_declaration};"

        width = offset_width(offset)
        new_line = f"    /* 0x{offset:0{width}X} */ {declaration}"
        if comment:
            new_line += f" {comment}"
        output.append(new_line)

        offset += size * array_count(array_suffix)
        changed = True

    typedef_match = re.search(r"}\s*([A-Za-z_][A-Za-z0-9_]*)\s*;", block)
    if typedef_match and changed:
        known_types[typedef_match.group(1)] = (align(offset, max_alignment), max_alignment)

    return "\n".join(output)


def format_text(text: str) -> tuple[str, set[str]]:
    known_types = dict(KNOWN_TYPES)
    unknown: set[str] = set()
    formatted = STRUCT_RE.sub(lambda match: format_struct_block(match.group(0), known_types, unknown), text)
    return formatted, unknown


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("files", nargs="+", type=Path)
    parser.add_argument("--in-place", action="store_true", help="rewrite files instead of printing formatted text")
    args = parser.parse_args()

    all_unknown: set[str] = set()
    for path in args.files:
        text = path.read_text()
        formatted, unknown = format_text(text)
        all_unknown.update(unknown)
        if args.in_place:
            path.write_text(formatted)
        else:
            print(formatted, end="")

    if all_unknown:
        print("Skipped fields with unknown types: " + ", ".join(sorted(all_unknown)), file=sys.stderr)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
