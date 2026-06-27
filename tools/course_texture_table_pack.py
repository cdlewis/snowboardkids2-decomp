#!/usr/bin/env python3
from __future__ import annotations

import argparse
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from tools.course_assets_common import load_yaml, parse_hex_bytes, parse_int, read_hex_or_file
from tools.modelpayload_common import read_palette_s, read_texture_png
from tools.sno import compress_sno


def main() -> int:
    parser = argparse.ArgumentParser(description="Pack editable course texture tables.")
    parser.add_argument("manifest", type=Path)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()

    manifest = load_yaml(args.manifest)
    size = parse_int(manifest["decompressed_size"])
    out = bytearray(b"\x00" * size)
    entries = manifest["entries"]
    palettes = manifest["palettes"]

    out[4:8] = len(entries).to_bytes(4, "big")
    palette_values: dict[str, list[int]] = {}
    for index, palette in enumerate(palettes):
        path = args.manifest.parent / palette["path"]
        values = read_palette_s(path)
        palette_values[palette["name"]] = values
        start = parse_int(palette["offset"])
        raw = b"".join(value.to_bytes(2, "big") for value in values)
        out[start : start + len(raw)] = raw

    for index, entry in enumerate(entries):
        start = 8 + index * 16
        texture_offset = parse_int(entry["texture_offset"])
        palette_index = parse_int(entry["palette_index"])
        width = parse_int(entry["width"])
        height = parse_int(entry["height"])
        unknown_0a = parse_hex_bytes(entry.get("unknown_0a"))
        if len(unknown_0a) > 6:
            raise ValueError(f"{entry['name']} unknown_0a is 0x{len(unknown_0a):X} bytes, expected at most 0x6")
        palette_name = palettes[palette_index]["name"]
        texture = read_texture_png(args.manifest.parent / entry["path"], entry, palette_values[palette_name])
        expected_size = parse_int(entry["size"])
        if len(texture) != expected_size:
            raise ValueError(f"{entry['path']} rebuilt to 0x{len(texture):X} bytes, expected 0x{expected_size:X}")
        out[start : start + 16] = (
            texture_offset.to_bytes(4, "big")
            + palette_index.to_bytes(2, "big")
            + width.to_bytes(2, "big")
            + height.to_bytes(2, "big")
            + unknown_0a.ljust(6, b"\x00")
        )
        out[texture_offset : texture_offset + len(texture)] = texture

    for block in manifest.get("raw_blocks", []):
        start = parse_int(block["offset"])
        expected_size = parse_int(block["size"])
        data = read_hex_or_file(args.manifest.parent, block)
        if len(data) != expected_size:
            raise ValueError(f"{block['name']} rebuilt to 0x{len(data):X} bytes, expected 0x{expected_size:X}")
        out[start : start + expected_size] = data

    args.out.parent.mkdir(parents=True, exist_ok=True)
    packed = bytearray(compress_sno(bytes(out)))
    unused_tail = parse_hex_bytes(manifest.get("unused_sno_tail"))
    if unused_tail:
        packed[-len(unused_tail) :] = unused_tail
    args.out.write_bytes(bytes(packed))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
