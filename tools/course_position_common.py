#!/usr/bin/env python3
from __future__ import annotations

import struct
from pathlib import Path
from typing import Any

from tools.course_assets_common import load_yaml, parse_hex_bytes, parse_int, write_yaml
from tools.sno import compress_sno


DATA_OFFSET = 4
RECORD_SIZE = 0x10
GOLD_COIN_SENTINEL = bytes.fromhex("ff000000000000000000000000000000")
ITEM_BOX_SENTINEL = bytes.fromhex("ffff0000000000000000000000000000")

BOX_TYPE_TO_VALUE = {
    "red_weapon": 0,
    "blue_secondary": 1,
}
VALUE_TO_BOX_TYPE = {value: name for name, value in BOX_TYPE_TO_VALUE.items()}


def _vec3i_from_entry(entry: dict[str, Any]) -> tuple[int, int, int]:
    position = entry["position"]
    return (parse_int(position["x"]), parse_int(position["y"]), parse_int(position["z"]))


def _signed_16(value: int) -> int:
    value &= 0xFFFF
    return value - 0x10000 if value >= 0x8000 else value


def _append_padding(out: bytearray, decompressed_size: int, path: Path | None = None) -> bytes:
    if len(out) > decompressed_size:
        source = f"{path}: " if path is not None else ""
        raise ValueError(
            f"{source}rebuilt payload is 0x{len(out):X} bytes, larger than decompressed_size 0x{decompressed_size:X}"
        )
    out.extend(b"\x00" * (decompressed_size - len(out)))
    return bytes(out)


def unpack_gold_coin_positions(name: str, data: bytes, decompressed_size: int, unused_sno_tail: bytes) -> dict:
    offset = int.from_bytes(data[:4], "big")
    if offset != DATA_OFFSET:
        raise ValueError(f"{name}: unsupported gold coin data offset 0x{offset:X}")

    entries = []
    cursor = offset
    while cursor + RECORD_SIZE <= len(data):
        record = data[cursor : cursor + RECORD_SIZE]
        visible = struct.unpack(">b", record[0:1])[0]
        if visible == -1:
            break
        processed = struct.unpack(">b", record[1:2])[0]
        respawn_timer, x, y, z = struct.unpack(">hiii", record[2:16])
        entries.append(
            {
                "visible": visible,
                "processed": processed,
                "respawn_timer": respawn_timer,
                "position": {"x": x, "y": y, "z": z},
            }
        )
        cursor += RECORD_SIZE
    else:
        raise ValueError(f"{name}: gold coin data has no sentinel record")

    manifest = {
        "name": name,
        "format": "gold_coin_positions",
        "compression": "sno",
        "decompressed_size": f"0x{decompressed_size:X}",
        "entries": entries,
    }
    if unused_sno_tail and any(unused_sno_tail):
        manifest["unused_sno_tail"] = unused_sno_tail.hex()
    return manifest


def unpack_item_box_positions(name: str, data: bytes, decompressed_size: int, unused_sno_tail: bytes) -> dict:
    offset = int.from_bytes(data[:4], "big")
    if offset != DATA_OFFSET:
        raise ValueError(f"{name}: unsupported item box data offset 0x{offset:X}")

    entries = []
    cursor = offset
    while cursor + RECORD_SIZE <= len(data):
        record = data[cursor : cursor + RECORD_SIZE]
        box_type_value = struct.unpack(">h", record[0:2])[0]
        if box_type_value == -1:
            break
        if box_type_value not in VALUE_TO_BOX_TYPE:
            raise ValueError(f"{name}: unsupported item box type {box_type_value} at decompressed offset 0x{cursor:X}")
        rotation_angle, x, y, z = struct.unpack(">hiii", record[2:16])
        entries.append(
            {
                "box_type": VALUE_TO_BOX_TYPE[box_type_value],
                "rotation_angle": f"0x{rotation_angle & 0xFFFF:04X}",
                "position": {"x": x, "y": y, "z": z},
            }
        )
        cursor += RECORD_SIZE
    else:
        raise ValueError(f"{name}: item box data has no sentinel record")

    manifest = {
        "name": name,
        "format": "item_box_positions",
        "compression": "sno",
        "decompressed_size": f"0x{decompressed_size:X}",
        "entries": entries,
    }
    if unused_sno_tail and any(unused_sno_tail):
        manifest["unused_sno_tail"] = unused_sno_tail.hex()
    return manifest


def pack_gold_coin_positions(manifest: dict, path: Path | None = None) -> bytes:
    decompressed_size = parse_int(manifest["decompressed_size"])
    out = bytearray(DATA_OFFSET.to_bytes(4, "big"))

    for entry in manifest["entries"]:
        out.extend(
            struct.pack(
                ">bbhiii",
                parse_int(entry.get("visible", 0)),
                parse_int(entry.get("processed", 0)),
                parse_int(entry.get("respawn_timer", 0)),
                *_vec3i_from_entry(entry),
            )
        )

    out.extend(GOLD_COIN_SENTINEL)
    return _append_padding(out, decompressed_size, path)


def pack_item_box_positions(manifest: dict, path: Path | None = None) -> bytes:
    decompressed_size = parse_int(manifest["decompressed_size"])
    out = bytearray(DATA_OFFSET.to_bytes(4, "big"))

    for entry in manifest["entries"]:
        box_type = str(entry["box_type"])
        if box_type not in BOX_TYPE_TO_VALUE:
            source = f"{path}: " if path is not None else ""
            raise ValueError(f"{source}unsupported item box box_type {box_type!r}")
        out.extend(
            struct.pack(
                ">hhiii",
                BOX_TYPE_TO_VALUE[box_type],
                _signed_16(parse_int(entry["rotation_angle"])),
                *_vec3i_from_entry(entry),
            )
        )

    out.extend(ITEM_BOX_SENTINEL)
    return _append_padding(out, decompressed_size, path)


def write_position_manifest(path: Path, manifest: dict) -> None:
    write_yaml(path, manifest)


def pack_sno_manifest(path: Path, packer) -> bytes:
    manifest = load_yaml(path)
    packed = bytearray(compress_sno(packer(manifest, path)))
    unused_tail = parse_hex_bytes(manifest.get("unused_sno_tail"))
    if unused_tail:
        if len(unused_tail) > len(packed):
            raise ValueError(f"{path}: unused_sno_tail is longer than compressed output")
        packed[-len(unused_tail) :] = unused_tail
    return bytes(packed)
