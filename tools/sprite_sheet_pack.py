#!/usr/bin/env python3
from __future__ import annotations

import argparse
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from tools.course_assets_common import load_yaml, parse_hex_bytes
from tools.sno import compress_sno, decompress_sno
from tools.sprite_sheet_common import build_sprite_sheet


def main() -> int:
    parser = argparse.ArgumentParser(description="Pack an editable SpriteSheetData asset.")
    parser.add_argument("manifest", type=Path)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()

    manifest = load_yaml(args.manifest)
    decompressed = build_sprite_sheet(args.manifest, manifest)
    packed = bytearray(compress_sno(decompressed))
    unused_tail = parse_hex_bytes(manifest.get("unused_sno_tail"))
    if unused_tail:
        if len(unused_tail) > len(packed):
            raise ValueError(f"{args.manifest}: unused SNO tail is larger than the packed asset")
        packed[-len(unused_tail) :] = unused_tail
    try:
        rebuilt = decompress_sno(bytes(packed), len(decompressed))
    except ValueError as exc:
        raise ValueError(f"{args.manifest}: preserved SNO tail overlaps compressed data") from exc
    if rebuilt != decompressed:
        raise ValueError(f"{args.manifest}: packed asset does not decompress to the manifest data")

    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_bytes(packed)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
