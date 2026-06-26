#!/usr/bin/env python3
from __future__ import annotations

import argparse
import sys
from pathlib import Path

import yaml

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from tools.modelpayload_common import (
    read_palette_s,
    read_texture_png,
    read_vtx_s,
)
from tools.sno import compress_sno


def load_yaml(path: Path) -> dict:
    with path.open("r", encoding="utf-8") as f:
        return yaml.safe_load(f)


def parse_int(value) -> int:
    return int(value, 0) if isinstance(value, str) else int(value)


def build_part(manifest_dir: Path, manifest: dict, part: dict, palettes: dict[str, list[int]]) -> bytes:
    part_path = manifest_dir / part["path"]
    part_type = part["type"]

    if part_type == "raw":
        return part_path.read_bytes()

    if part_type == "vtx":
        return read_vtx_s(part_path)

    if part_type == "palette":
        values = read_palette_s(part_path)
        out = bytearray()
        for value in values:
            out.extend(value.to_bytes(2, "big"))
        palettes[part["name"]] = values
        return bytes(out)

    if part_type == "texture":
        palette_values = palettes.get(part.get("palette"))
        return read_texture_png(part_path, part, palette_values)

    raise ValueError(f"unsupported modelpayload part type {part_type}")


def build_decompressed(manifest_path: Path, manifest: dict) -> bytes:
    size = parse_int(manifest["decompressed_size"])
    out = bytearray(b"\x00" * size)
    written = bytearray(size)
    palettes: dict[str, list[int]] = {}

    ordered_parts = sorted(manifest["parts"], key=lambda p: parse_int(p["offset"]))
    for part in ordered_parts:
        if part["type"] != "palette":
            continue
        part_path = manifest_path.parent / part["path"]
        palettes[part["name"]] = read_palette_s(part_path)

    for part in ordered_parts:
        start = parse_int(part["offset"])
        expected_size = parse_int(part["size"])
        data = build_part(manifest_path.parent, manifest, part, palettes)
        if len(data) != expected_size:
            raise ValueError(
                f"{part['path']} rebuilt to 0x{len(data):X} bytes, expected 0x{expected_size:X}"
            )

        end = start + expected_size
        if start < 0 or end > size:
            raise ValueError(f"{part['name']} range 0x{start:X}-0x{end:X} exceeds payload size")
        if any(written[start:end]):
            raise ValueError(f"{part['name']} overlaps an earlier modelpayload part")

        out[start:end] = data
        written[start:end] = b"\x01" * expected_size

    if not all(written):
        first = written.index(0)
        raise ValueError(f"modelpayload manifest does not cover decompressed offset 0x{first:X}")

    return bytes(out)


def main() -> int:
    parser = argparse.ArgumentParser(description="Pack an editable modelpayload manifest.")
    parser.add_argument("manifest", type=Path)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()

    manifest = load_yaml(args.manifest)
    if manifest.get("compression") != "sno":
        raise ValueError(f"{args.manifest} has unsupported compression {manifest.get('compression')!r}")

    decompressed = build_decompressed(args.manifest, manifest)
    compressed = compress_sno(decompressed)

    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_bytes(compressed)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
