from __future__ import annotations

import struct
from pathlib import Path

from tools.course_assets_common import parse_int
from tools.modelpayload_common import read_palette_s, read_texture_png


HEADER_SIZE = 8
FRAME_ENTRY_SIZE = 0x10
CI4_PALETTE_SIZE = 0x20


def parse_frame_entry(data: bytes, index: int) -> dict:
    offset = HEADER_SIZE + index * FRAME_ENTRY_SIZE
    texture_offset, palette_index, width, height, palette_table_index, format_index, pad_0e = struct.unpack_from(
        ">I6H", data, offset
    )
    if palette_table_index != 0 or format_index != 0:
        raise ValueError(
            f"frame 0x{index:X} uses unsupported sprite format "
            f"palette_table_index={palette_table_index}, format_index={format_index}"
        )
    if width == 0 or height == 0:
        raise ValueError(f"frame 0x{index:X} has invalid dimensions {width}x{height}")
    return {
        "texture_offset": texture_offset,
        "palette_index": palette_index,
        "width": width,
        "height": height,
        "palette_table_index": palette_table_index,
        "format_index": format_index,
        "pad_0e": pad_0e,
        "format": "ci4",
        "size": (width * height + 1) // 2,
    }


def build_sprite_sheet(manifest_path: Path, manifest: dict) -> bytes:
    size = parse_int(manifest["decompressed_size"])
    frames = manifest["frames"]
    palettes = manifest["palettes"]
    palette_base = HEADER_SIZE + len(frames) * FRAME_ENTRY_SIZE
    out = bytearray(size)
    written = bytearray(size)

    def write_range(start: int, data: bytes, description: str, allow_identical: bool = False) -> None:
        end = start + len(data)
        if start < 0 or end > size:
            raise ValueError(f"{manifest_path}: {description} range 0x{start:X}-0x{end:X} exceeds payload size")
        if any(written[start:end]):
            if allow_identical and out[start:end] == data:
                return
            raise ValueError(f"{manifest_path}: {description} overlaps an earlier range")
        out[start:end] = data
        written[start:end] = b"\x01" * len(data)

    header = parse_int(manifest.get("texture_base", 0)).to_bytes(4, "big") + len(frames).to_bytes(4, "big")
    write_range(0, header, "header")

    palette_values: dict[str, list[int]] = {}
    palette_indices: dict[int, str] = {}
    for palette in palettes:
        index = parse_int(palette["index"])
        if index in palette_indices:
            raise ValueError(f"{manifest_path}: duplicate palette index {index}")
        values = read_palette_s(manifest_path.parent / palette["path"])
        if len(values) != 16:
            raise ValueError(f"{palette['path']} contains {len(values)} colors, expected 16")
        expected_offset = palette_base + index * CI4_PALETTE_SIZE
        offset = parse_int(palette["offset"])
        if offset != expected_offset:
            raise ValueError(
                f"{palette['path']} has offset 0x{offset:X}, expected contiguous palette offset 0x{expected_offset:X}"
            )
        raw = b"".join(value.to_bytes(2, "big") for value in values)
        write_range(offset, raw, f"palette {index}")
        palette_values[palette["name"]] = values
        palette_indices[index] = palette["name"]

    for index, frame in enumerate(frames):
        manifest_index = parse_int(frame.get("index", index))
        if manifest_index != index:
            raise ValueError(f"{manifest_path}: frame {index} declares index {manifest_index}")
        palette_index = parse_int(frame["palette_index"])
        if palette_index not in palette_indices:
            raise ValueError(f"{frame['name']} references missing palette index {palette_index}")
        width = parse_int(frame["width"])
        height = parse_int(frame["height"])
        if width <= 0 or height <= 0:
            raise ValueError(f"{frame['name']} has invalid dimensions {width}x{height}")
        if frame.get("format") != "ci4":
            raise ValueError(f"{frame['name']} uses unsupported texture format {frame.get('format')!r}")
        palette_table_index = parse_int(frame.get("palette_table_index", 0))
        format_index = parse_int(frame.get("format_index", 0))
        if palette_table_index != 0 or format_index != 0:
            raise ValueError(
                f"{frame['name']} uses unsupported sprite format "
                f"palette_table_index={palette_table_index}, format_index={format_index}"
            )
        texture_offset = parse_int(frame["texture_offset"])
        texture = read_texture_png(
            manifest_path.parent / frame["path"], frame, palette_values[palette_indices[palette_index]]
        )
        expected_size = parse_int(frame["size"])
        calculated_size = (width * height + 1) // 2
        if expected_size != calculated_size or len(texture) != expected_size:
            raise ValueError(
                f"{frame['path']} rebuilt to 0x{len(texture):X} bytes; "
                f"manifest/calculated sizes are 0x{expected_size:X}/0x{calculated_size:X}"
            )
        entry = struct.pack(
            ">I6H",
            texture_offset,
            palette_index,
            width,
            height,
            palette_table_index,
            format_index,
            parse_int(frame.get("pad_0e", 0)),
        )
        write_range(HEADER_SIZE + index * FRAME_ENTRY_SIZE, entry, f"frame entry {index}")
        write_range(texture_offset, texture, f"frame texture {index}", allow_identical=True)

    for block in manifest.get("raw_blocks", []):
        start = parse_int(block["offset"])
        expected_size = parse_int(block["size"])
        data = (manifest_path.parent / block["path"]).read_bytes()
        if len(data) != expected_size:
            raise ValueError(f"{block['path']} is 0x{len(data):X} bytes, expected 0x{expected_size:X}")
        write_range(start, data, f"raw block {block['name']}")

    if not all(written):
        first = written.index(0)
        raise ValueError(f"{manifest_path}: manifest does not cover decompressed offset 0x{first:X}")
    return bytes(out)
