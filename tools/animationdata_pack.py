#!/usr/bin/env python3
from __future__ import annotations

import argparse
import sys
from pathlib import Path
import re

import yaml

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from tools.sno import compress_sno


def load_yaml(path: Path) -> dict:
    with path.open("r", encoding="utf-8") as f:
        return yaml.safe_load(f)


def parse_int(value) -> int:
    return int(value, 0) if isinstance(value, str) else int(value)


def parse_hex_bytes(value) -> bytes:
    if value is None:
        return b""
    return bytes.fromhex(str(value).strip())


def read_asm_data(path: Path) -> bytes:
    out = bytearray()
    sizes = {
        ".byte": 1,
        ".hword": 2,
        ".word": 4,
    }

    for line in path.read_text(encoding="utf-8").splitlines():
        line = line.split("/*", 1)[0].split("#", 1)[0].strip()
        if not line:
            continue

        match = re.match(r"(\.(?:byte|hword|word))\s+(.+)", line)
        if not match:
            continue

        directive = match.group(1)
        width = sizes[directive]
        for part in match.group(2).split(","):
            value = int(part.strip(), 0)
            out.extend((value & ((1 << (width * 8)) - 1)).to_bytes(width, "big"))

    return bytes(out)


def build_decompressed(manifest_path: Path, manifest: dict) -> bytes:
    size = parse_int(manifest["decompressed_size"])
    parts = manifest.get("parts") or []
    out = bytearray(b"\x00" * size)
    written = bytearray(size)

    for part in sorted(parts, key=lambda item: parse_int(item["offset"])):
        part_type = part.get("type")
        if part_type not in {
            "offset_table",
            "index_data",
            "frame_data",
            "track_table",
            "header_data",
            "entry_data",
            "raw_data",
        }:
            raise ValueError(f"{manifest_path}: unsupported animationdata part type {part_type!r}")

        start = parse_int(part["offset"])
        expected_size = parse_int(part["size"])
        data = read_asm_data(manifest_path.parent / part["path"])
        if len(data) != expected_size:
            raise ValueError(f"{part['path']} is 0x{len(data):X} bytes, expected 0x{expected_size:X}")

        end = start + expected_size
        if start < 0 or end > size:
            raise ValueError(f"{part['name']} range 0x{start:X}-0x{end:X} exceeds payload size")
        if any(written[start:end]):
            raise ValueError(f"{part['name']} overlaps an earlier animationdata part")

        out[start:end] = data
        written[start:end] = b"\x01" * expected_size

    if not all(written):
        first = written.index(0)
        raise ValueError(f"animationdata manifest does not cover decompressed offset 0x{first:X}")

    return bytes(out)


def main() -> int:
    parser = argparse.ArgumentParser(description="Pack an editable animationdata manifest.")
    parser.add_argument("manifest", type=Path)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()

    manifest = load_yaml(args.manifest)
    if manifest.get("compression") != "sno":
        raise ValueError(f"{args.manifest} has unsupported compression {manifest.get('compression')!r}")

    decompressed = build_decompressed(args.manifest, manifest)
    compressed = compress_sno(decompressed)
    trailer = parse_hex_bytes(manifest.get("unused_sno_tail"))

    expected_size = parse_int(manifest["compressed_size"])
    if len(compressed) > expected_size:
        raise ValueError(
            f"{args.manifest} rebuilt compressed data to 0x{len(compressed):X} bytes, "
            f"expected at most 0x{expected_size:X}"
        )
    compressed = bytearray(compressed + b"\x00" * (expected_size - len(compressed)))
    if trailer:
        compressed[-len(trailer) :] = trailer

    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_bytes(bytes(compressed))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
