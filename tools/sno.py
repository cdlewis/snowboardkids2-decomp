#!/usr/bin/env python3
from __future__ import annotations

import argparse
from pathlib import Path


MAX_OFFSET = 0xFFF
MAX_COUNT = 0xF
ALIGNMENT = 0x10


def align(value: int, alignment: int = ALIGNMENT) -> int:
    return (value + alignment - 1) & ~(alignment - 1)


def decompress_sno(data: bytes, output_size: int) -> bytes:
    out = bytearray()
    src = 0

    while len(out) < output_size:
        if src + 1 >= len(data):
            raise ValueError(
                f"Sno stream ended at 0x{src:X}, before producing 0x{output_size:X} bytes"
            )

        first = data[src]
        second = data[src + 1]
        src += 2

        if first == 0:
            out.append(second)
            continue

        count = first >> 4
        offset = ((first & 0x0F) << 8) | second
        if count == 0 or offset == 0:
            raise ValueError(f"invalid Sno copy command count={count} offset={offset}")
        if offset > len(out):
            raise ValueError(
                f"invalid Sno back-reference at output 0x{len(out):X}: offset 0x{offset:X}"
            )

        for _ in range(count):
            out.append(out[-offset])
            if len(out) >= output_size:
                break

    return bytes(out)


def _best_match(data: bytes, pos: int, positions_by_byte: list[list[int]]) -> tuple[int, int]:
    best_count = 0
    best_offset = 0
    window_start = max(0, pos - MAX_OFFSET)

    for candidate in reversed(positions_by_byte[data[pos]]):
        if candidate < window_start:
            break

        offset = pos - candidate
        count = 0
        while (
            count < MAX_COUNT
            and pos + count < len(data)
            and data[candidate + count] == data[pos + count]
        ):
            count += 1

        if count > best_count:
            best_count = count
            best_offset = offset
            if count == MAX_COUNT:
                break

    return best_count, best_offset


def compress_sno(data: bytes) -> bytes:
    out = bytearray()
    positions_by_byte: list[list[int]] = [[] for _ in range(0x100)]
    pos = 0

    while pos < len(data):
        count, offset = _best_match(data, pos, positions_by_byte)

        if count > 0:
            out.append(((count & 0x0F) << 4) | (offset >> 8))
            out.append(offset & 0xFF)
            for value in data[pos : pos + count]:
                positions_by_byte[value].append(pos)
                pos += 1
        else:
            out.append(0)
            out.append(data[pos])
            positions_by_byte[data[pos]].append(pos)
            pos += 1

    out.extend(b"\x00" * (align(len(out)) - len(out)))
    return bytes(out)


def selftest() -> int:
    assets = [
        ("SLASH_BOARD0_COMPRESSED_DATA", 0x350C70, 0x352A40, 0x45A0),
        ("COSTUME_SLOT_00_COMPRESSED_DATA", 0x3F6BB0, 0x3F6D90, 0x320),
        ("MROBO_COMPRESSED_DATA", 0x4A2BC0, 0x4A3E80, 0x4070),
        ("CHAIR_COMPRESSED_DATA", 0x4A73F0, 0x4A7600, 0x560),
    ]
    rom = Path("snowboardkids2.z64").read_bytes()

    for name, start, end, output_size in assets:
        compressed = rom[start:end]
        decompressed = decompress_sno(compressed, output_size)
        recompressed = compress_sno(decompressed)
        if recompressed != compressed:
            for i, (actual, expected) in enumerate(zip(recompressed, compressed)):
                if actual != expected:
                    raise AssertionError(
                        f"{name} mismatch at 0x{i:X}: got 0x{actual:02X}, expected 0x{expected:02X}"
                    )
            raise AssertionError(
                f"{name} size mismatch: got 0x{len(recompressed):X}, expected 0x{len(compressed):X}"
            )
        print(f"{name}: OK")

    return 0


def main() -> int:
    parser = argparse.ArgumentParser(description="Compress and decompress Snowboard Kids 2 Sno data.")
    subparsers = parser.add_subparsers(dest="command", required=True)

    decompress_parser = subparsers.add_parser("decompress")
    decompress_parser.add_argument("input", type=Path)
    decompress_parser.add_argument("output", type=Path)
    decompress_parser.add_argument("--size", type=lambda value: int(value, 0), required=True)

    compress_parser = subparsers.add_parser("compress")
    compress_parser.add_argument("input", type=Path)
    compress_parser.add_argument("output", type=Path)

    subparsers.add_parser("selftest")

    args = parser.parse_args()

    if args.command == "decompress":
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_bytes(decompress_sno(args.input.read_bytes(), args.size))
        return 0

    if args.command == "compress":
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_bytes(compress_sno(args.input.read_bytes()))
        return 0

    return selftest()


if __name__ == "__main__":
    raise SystemExit(main())
