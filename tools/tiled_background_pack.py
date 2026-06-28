#!/usr/bin/env python3
from __future__ import annotations

import argparse
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from tools.course_assets_common import load_yaml, parse_hex_bytes, parse_int
from tools.sno import compress_sno
from tools.tiled_background_common import (
    make_header,
    palette_size,
    read_palette,
    read_tile_png,
    tile_byte_size,
    write_preview,
    write_tile_entries,
    write_tile_indices,
)


def main() -> int:
    parser = argparse.ArgumentParser(description="Pack editable tiled background assets.")
    parser.add_argument("manifest", type=Path)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()

    manifest = load_yaml(args.manifest)
    size = parse_int(manifest["decompressed_size"])
    out = bytearray(b"\x00" * size)

    tile_index_offset = parse_int(manifest["tile_index_data_offset"])
    palette_offset = parse_int(manifest["palette_data_offset"])
    texture_offset = parse_int(manifest["texture_data_offset"])
    tile_width = parse_int(manifest["tile_width"])
    tile_height = parse_int(manifest["tile_height"])
    ci_mode = parse_int(manifest["ci_mode"])
    pal_size = palette_size(ci_mode)
    tile_size = tile_byte_size(tile_width, tile_height, ci_mode)

    out[0:0x10] = make_header(manifest)
    write_tile_entries(out, 0x10, manifest["tile_entries"])
    if 0x10 + len(manifest["tile_entries"]) * 4 != tile_index_offset:
        raise ValueError(f"{args.manifest}: tile_entries do not end at tile_index_data_offset")

    write_tile_indices(out, tile_index_offset, manifest["tile_indices"])
    if tile_index_offset + len(manifest["tile_indices"]) * 2 != palette_offset:
        raise ValueError(f"{args.manifest}: tile_indices do not end at palette_data_offset")

    palette_values: dict[int, list[int]] = {}
    for palette in manifest["palettes"]:
        index = parse_int(palette["index"])
        values = read_palette(args.manifest.parent / palette["path"])
        if len(values) * 2 != pal_size:
            raise ValueError(f"{palette['path']} rebuilt to 0x{len(values) * 2:X} bytes, expected 0x{pal_size:X}")
        palette_values[index] = values
        start = palette_offset + index * pal_size
        out[start : start + pal_size] = b"".join(value.to_bytes(2, "big") for value in values)

    for tile in manifest["tiles"]:
        index = parse_int(tile["index"])
        palette_index = parse_int(tile.get("palette", 0))
        if palette_index not in palette_values:
            raise ValueError(f"{tile['path']} references missing palette {palette_index}")
        data = read_tile_png(
            args.manifest.parent / tile["path"],
            tile_width,
            tile_height,
            ci_mode,
            palette_values[palette_index],
        )
        if len(data) != tile_size:
            raise ValueError(f"{tile['path']} rebuilt to 0x{len(data):X} bytes, expected 0x{tile_size:X}")
        start = texture_offset + (index - 1) * tile_size
        out[start : start + tile_size] = data

    args.out.parent.mkdir(parents=True, exist_ok=True)
    packed = bytearray(compress_sno(bytes(out)))
    unused_tail = parse_hex_bytes(manifest.get("unused_sno_tail"))
    if unused_tail:
        packed[-len(unused_tail) :] = unused_tail
    args.out.write_bytes(bytes(packed))

    if manifest.get("preview"):
        write_preview(args.manifest, manifest)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
