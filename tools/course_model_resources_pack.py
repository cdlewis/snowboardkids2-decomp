#!/usr/bin/env python3
from __future__ import annotations

import argparse
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from tools.course_assets_common import build_parts, load_yaml, parse_hex_bytes
from tools.sno import compress_sno


def main() -> int:
    parser = argparse.ArgumentParser(description="Pack editable course model resource assets.")
    parser.add_argument("manifest", type=Path)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()

    manifest = load_yaml(args.manifest)
    data = build_parts(args.manifest, manifest)
    packed = bytearray(compress_sno(data))
    unused_tail = parse_hex_bytes(manifest.get("unused_sno_tail"))
    if unused_tail:
        packed[-len(unused_tail) :] = unused_tail
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_bytes(bytes(packed))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
