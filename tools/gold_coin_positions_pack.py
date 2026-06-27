#!/usr/bin/env python3
from __future__ import annotations

import argparse
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from tools.course_position_common import pack_gold_coin_positions, pack_sno_manifest


def main() -> int:
    parser = argparse.ArgumentParser(description="Pack editable gold coin position data.")
    parser.add_argument("manifest", type=Path)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()

    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_bytes(pack_sno_manifest(args.manifest, pack_gold_coin_positions))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
