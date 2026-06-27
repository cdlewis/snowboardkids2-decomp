#!/usr/bin/env python3
from __future__ import annotations

import argparse
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from tools.course_assets_common import load_yaml, pack_track_sector_mesh, parse_hex_bytes
from tools.sno import compress_sno


def main() -> int:
    parser = argparse.ArgumentParser(description="Pack editable track sector mesh data.")
    parser.add_argument("manifest", type=Path)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()

    manifest = load_yaml(args.manifest)
    args.out.parent.mkdir(parents=True, exist_ok=True)
    packed = bytearray(compress_sno(pack_track_sector_mesh(manifest)))
    unused_tail = parse_hex_bytes(manifest.get("unused_sno_tail"))
    if unused_tail:
        packed[-len(unused_tail) :] = unused_tail
    args.out.write_bytes(bytes(packed))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
