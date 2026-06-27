#!/usr/bin/env python3
from __future__ import annotations

import struct
from pathlib import Path
from typing import Iterable

import yaml

from tools.modelpayload_common import (
    read_palette_s,
    read_texture_png,
    read_vtx_s,
    write_palette_s,
    write_texture_png,
    write_vtx_s,
)


def parse_int(value) -> int:
    return int(value, 0) if isinstance(value, str) else int(value)


def load_yaml(path: Path) -> dict:
    with path.open("r", encoding="utf-8") as f:
        return yaml.safe_load(f)


def course_display_list_ranges(config_path: Path) -> dict[str, tuple[int, int]]:
    config = load_yaml(config_path)
    segments = config["segments"]
    ranges: dict[str, tuple[int, int]] = {}

    for index, segment in enumerate(segments):
        if not isinstance(segment, dict) or segment.get("type") != "course_display_lists":
            continue
        start = parse_int(segment["start"])
        end = None
        for next_segment in segments[index + 1 :]:
            if isinstance(next_segment, list) and len(next_segment) == 1:
                end = parse_int(next_segment[0])
                break
            if isinstance(next_segment, dict) and "start" in next_segment:
                end = parse_int(next_segment["start"])
                break
            if isinstance(next_segment, list) and next_segment:
                end = parse_int(next_segment[0])
                break
        if end is None:
            raise ValueError(f"{config_path}: could not find end for course display-list segment {segment['name']}")
        ranges[str(segment["name"])] = (start, end)

    return ranges


def course_segments_by_course_id(config_path: Path, segment_type: str) -> dict[str, str]:
    config = load_yaml(config_path)
    segments = config["segments"]
    names: dict[str, str] = {}

    for segment in segments:
        if not isinstance(segment, dict) or segment.get("type") != segment_type:
            continue
        course_id = segment.get("course_id")
        if course_id is None:
            raise ValueError(f"{config_path}: course segment {segment['name']} is missing course_id")
        course_id = str(course_id)
        if course_id in names:
            raise ValueError(f"{config_path}: duplicate {segment_type} course_id {course_id}")
        names[course_id] = str(segment["name"])

    return names


def course_segment_name(config_path: Path, segment_type: str, course_id: str) -> str:
    names = course_segments_by_course_id(config_path, segment_type)
    if course_id not in names:
        raise ValueError(f"{config_path}: no {segment_type} segment for course_id {course_id}")
    return names[course_id]


def read_course_display_list_bytes(config_path: Path, rom_path: Path, name: str) -> bytes:
    ranges = course_display_list_ranges(config_path)
    if name not in ranges:
        raise ValueError(f"{config_path}: no course_display_lists segment named {name}")
    start, end = ranges[name]
    return rom_path.read_bytes()[start:end]


def write_yaml(path: Path, data: dict) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(yaml.safe_dump(data, sort_keys=False), encoding="utf-8", newline="\n")


def align(value: int, alignment: int = 0x10) -> int:
    return (value + alignment - 1) & ~(alignment - 1)


def parse_hex_bytes(value) -> bytes:
    if value is None:
        return b""
    return bytes.fromhex(str(value).strip())


def parse_gfx_words_from_bytes(data: bytes) -> list[tuple[int, int]]:
    if len(data) % 8 != 0:
        raise ValueError(f"display-list data length 0x{len(data):X} is not a multiple of 8")
    return [
        (int.from_bytes(data[offset : offset + 4], "big"), int.from_bytes(data[offset + 4 : offset + 8], "big"))
        for offset in range(0, len(data), 8)
    ]


def read_hex_or_file(manifest_dir: Path, part: dict) -> bytes:
    if "path" in part:
        return (manifest_dir / part["path"]).read_bytes()
    return bytes.fromhex(str(part.get("data", "")))


def write_binary_s(path: Path, data: bytes, label: str) -> None:
    lines = [
        f"/* Auto-generated from editable course asset sources. */",
        '.section .data, "wa"',
        "",
        f".globl {label}",
        f"{label}:",
    ]
    for offset in range(0, len(data), 16):
        chunk = data[offset : offset + 16]
        values = ", ".join(f"0x{value:02X}" for value in chunk)
        lines.append(f"    .byte {values}")
    lines.extend([f"{label}_end:", f".size {label}, {label}_end - {label}", ""])
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text("\n".join(lines), encoding="utf-8", newline="\n")


def build_parts(manifest_path: Path, manifest: dict) -> bytes:
    size = parse_int(manifest["decompressed_size"])
    out = bytearray(b"\x00" * size)
    written = bytearray(size)
    palettes: dict[str, list[int]] = {}

    parts = sorted(manifest["parts"], key=lambda item: parse_int(item["offset"]))
    for part in parts:
        if part["type"] == "palette":
            palettes[part["name"]] = read_palette_s(manifest_path.parent / part["path"])

    for part in parts:
        start = parse_int(part["offset"])
        expected_size = parse_int(part["size"])
        part_type = part["type"]

        if part_type == "vtx":
            data = read_vtx_s(manifest_path.parent / part["path"])
        elif part_type == "palette":
            data = b"".join(value.to_bytes(2, "big") for value in palettes[part["name"]])
        elif part_type == "texture":
            data = read_texture_png(manifest_path.parent / part["path"], part, palettes.get(part.get("palette")))
        elif part_type == "raw":
            data = read_hex_or_file(manifest_path.parent, part)
        else:
            raise ValueError(f"{manifest_path}: unsupported part type {part_type!r}")

        if len(data) != expected_size:
            raise ValueError(
                f"{manifest_path}: {part['name']} rebuilt to 0x{len(data):X} bytes, expected 0x{expected_size:X}"
            )
        end = start + expected_size
        if start < 0 or end > size:
            raise ValueError(f"{manifest_path}: {part['name']} range 0x{start:X}-0x{end:X} exceeds payload size")
        if any(written[start:end]):
            raise ValueError(f"{manifest_path}: {part['name']} overlaps an earlier part")
        out[start:end] = data
        written[start:end] = b"\x01" * expected_size

    if not all(written):
        first = written.index(0)
        raise ValueError(f"{manifest_path}: manifest does not cover decompressed offset 0x{first:X}")

    return bytes(out)


def write_parts_manifest(path: Path, name: str, decompressed_size: int, parts: list[dict], extra: dict | None = None) -> None:
    data = {
        "name": name,
        "decompressed_size": f"0x{decompressed_size:X}",
        "parts": parts,
    }
    if extra:
        data.update(extra)
    write_yaml(path, data)


def write_vtx_part(base_dir: Path, rel_path: str, data: bytes, start: int) -> None:
    write_vtx_s(base_dir / rel_path, data, start)


def write_palette_part(base_dir: Path, rel_path: str, data: bytes, start: int) -> None:
    write_palette_s(base_dir / rel_path, data, start)


def write_texture_part(base_dir: Path, rel_path: str, data: bytes, meta: dict, palette_values: list[int] | None) -> None:
    write_texture_png(base_dir / rel_path, data, meta, palette_values)


def iter_course_manifests(root: Path) -> Iterable[Path]:
    for folder in ("model_resources", "texture_tables", "track_sector_meshes"):
        yield from sorted((root / folder).glob("*.yaml"))


def pack_track_sector_mesh(manifest: dict) -> bytes:
    out = bytearray()

    vertices = manifest["vertices"]
    out.extend(len(vertices).to_bytes(2, "big"))
    for vertex in vertices:
        out.extend(struct.pack(">hhh", parse_int(vertex["x"]), parse_int(vertex["y"]), parse_int(vertex["z"])))

    faces = manifest["faces"]
    out.extend(len(faces).to_bytes(2, "big"))
    for face in faces:
        out.extend(
            struct.pack(
                ">HHHBB",
                parse_int(face["v0"]),
                parse_int(face["v1"]),
                parse_int(face["v2"]),
                parse_int(face["flags"]),
                parse_int(face["surface_index"]),
            )
        )

    out.extend(parse_int(manifest["final_value"]).to_bytes(2, "big"))
    for sector in manifest["sectors"]:
        out.extend(struct.pack(">hhhh", *(parse_int(sector[f"neighbor{i}"]) for i in range(4))))
        out.extend(bytes.fromhex(str(sector["unknown_08"])))
        out.extend(struct.pack(">HHHH", parse_int(sector["base_face"]), parse_int(sector["face_count"]), parse_int(sector["base_height_face"]), parse_int(sector["height_face_count"])))
        out.extend(parse_int(sector["vertex0"]).to_bytes(2, "big"))
        out.extend(bytes.fromhex(str(sector["unknown_16"])))
        out.extend(parse_int(sector["vertex1"]).to_bytes(2, "big"))
        out.extend(parse_int(sector["vertex2"]).to_bytes(2, "big"))
        out.extend(bytes.fromhex(str(sector["unknown_1c"])))
        out.extend(parse_int(sector["vertex3"]).to_bytes(2, "big"))
        out.extend(bytes.fromhex(str(sector["unknown_20"])))

    return bytes(out)
