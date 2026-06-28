#!/usr/bin/env python3
from __future__ import annotations

from pathlib import Path

from tools.course_assets_common import parse_int
from tools.modelpayload_common import read_palette_s, read_texture_png, write_palette_s, write_texture_png


def palette_size(ci_mode: int) -> int:
    return 0x20 if ci_mode == 0 else 0x200


def palette_colors(ci_mode: int) -> int:
    return 16 if ci_mode == 0 else 256


def texture_format(ci_mode: int) -> str:
    return "ci4" if ci_mode == 0 else "ci8"


def tile_byte_size(width: int, height: int, ci_mode: int) -> int:
    if ci_mode == 0:
        return (width * height + 1) // 2
    return width * height


def parse_header(data: bytes) -> dict:
    return {
        "tile_grid_width": int.from_bytes(data[0:2], "big"),
        "tile_grid_height": int.from_bytes(data[2:4], "big"),
        "tile_width": data[4],
        "tile_height": data[5],
        "ci_mode": int.from_bytes(data[6:8], "big"),
        "unknown_08": data[8:10].hex(),
        "tile_index_data_offset": int.from_bytes(data[10:12], "big"),
        "palette_data_offset": int.from_bytes(data[12:14], "big"),
        "texture_data_offset": int.from_bytes(data[14:16], "big"),
    }


def make_header(manifest: dict) -> bytes:
    out = bytearray(0x10)
    out[0:2] = parse_int(manifest["tile_grid_width"]).to_bytes(2, "big")
    out[2:4] = parse_int(manifest["tile_grid_height"]).to_bytes(2, "big")
    out[4] = parse_int(manifest["tile_width"]) & 0xFF
    out[5] = parse_int(manifest["tile_height"]) & 0xFF
    out[6:8] = parse_int(manifest["ci_mode"]).to_bytes(2, "big")
    out[8:10] = bytes.fromhex(str(manifest.get("unknown_08", "0000")))
    out[10:12] = parse_int(manifest["tile_index_data_offset"]).to_bytes(2, "big")
    out[12:14] = parse_int(manifest["palette_data_offset"]).to_bytes(2, "big")
    out[14:16] = parse_int(manifest["texture_data_offset"]).to_bytes(2, "big")
    return bytes(out)


def read_tile_entries(data: bytes, start: int, end: int) -> list[dict]:
    entries = []
    for index, offset in enumerate(range(start, end, 4)):
        entries.append(
            {
                "index": index,
                "texture_index": int.from_bytes(data[offset : offset + 2], "big"),
                "palette_index": data[offset + 2],
                "flip_mode": data[offset + 3],
            }
        )
    return entries


def write_tile_entries(out: bytearray, start: int, entries: list[dict]) -> None:
    for index, entry in enumerate(sorted(entries, key=lambda item: parse_int(item["index"]))):
        offset = start + index * 4
        out[offset : offset + 2] = parse_int(entry["texture_index"]).to_bytes(2, "big")
        out[offset + 2] = parse_int(entry["palette_index"]) & 0xFF
        out[offset + 3] = parse_int(entry["flip_mode"]) & 0xFF


def read_tile_indices(data: bytes, start: int, end: int) -> list[int]:
    return [int.from_bytes(data[offset : offset + 2], "big") for offset in range(start, end, 2)]


def write_tile_indices(out: bytearray, start: int, values: list) -> None:
    for index, value in enumerate(values):
        offset = start + index * 2
        out[offset : offset + 2] = parse_int(value).to_bytes(2, "big")


def write_preview(manifest_path: Path, manifest: dict) -> None:
    try:
        from PIL import Image, ImageOps
    except ImportError as exc:
        raise RuntimeError("Pillow is required to write tiled background previews") from exc

    width = parse_int(manifest["tile_grid_width"])
    height = parse_int(manifest["tile_grid_height"])
    tile_width = parse_int(manifest["tile_width"])
    tile_height = parse_int(manifest["tile_height"])
    entries = {parse_int(entry["index"]): entry for entry in manifest["tile_entries"]}
    indices = manifest["tile_indices"]
    tiles = {parse_int(tile["index"]): tile for tile in manifest["tiles"]}

    image = Image.new("RGBA", (width * tile_width, height * tile_height), (0, 0, 0, 0))
    for y in range(height):
        for x in range(width):
            tile_entry_index = parse_int(indices[y * width + x])
            if tile_entry_index == 0:
                continue
            entry = entries[tile_entry_index]
            texture_index = parse_int(entry["texture_index"])
            if texture_index == 0:
                continue
            tile = Image.open(manifest_path.parent / tiles[texture_index]["path"]).convert("RGBA")
            flip_mode = parse_int(entry["flip_mode"])
            if flip_mode in (1, 3):
                tile = ImageOps.mirror(tile)
            if flip_mode in (2, 3):
                tile = ImageOps.flip(tile)
            image.paste(tile, (x * tile_width, y * tile_height), tile)

    preview = manifest.get("preview", "preview.png")
    image.save(manifest_path.parent / preview)


def write_palette(path: Path, data: bytes, start: int) -> None:
    write_palette_s(path, data, start)


def read_palette(path: Path) -> list[int]:
    return read_palette_s(path)


def write_tile_png(path: Path, data: bytes, width: int, height: int, ci_mode: int, palette_values: list[int]) -> None:
    write_texture_png(path, data, {"width": width, "height": height, "format": texture_format(ci_mode)}, palette_values)


def read_tile_png(path: Path, width: int, height: int, ci_mode: int, palette_values: list[int]) -> bytes:
    return read_texture_png(path, {"width": width, "height": height, "format": texture_format(ci_mode)}, palette_values)
