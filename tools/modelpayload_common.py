#!/usr/bin/env python3
from __future__ import annotations

import re
import struct
from dataclasses import dataclass
from dataclasses import replace
from pathlib import Path
from typing import Iterable


FMT_RGBA = 0
FMT_CI = 2
SIZ_4B = 0
SIZ_8B = 1
SIZ_16B = 2


@dataclass(frozen=True)
class Range:
    start: int
    end: int
    kind: str
    name: str
    meta: dict

    @property
    def size(self) -> int:
        return self.end - self.start


def parse_gfx_words(path: Path) -> list[tuple[int, int]]:
    words: list[tuple[int, int]] = []
    word_re = re.compile(r"\.word\s+([^,]+),\s*([^/\s]+)")
    for line in path.read_text(encoding="utf-8").splitlines():
        match = word_re.search(line)
        if not match:
            continue
        words.append((int(match.group(1), 0), int(match.group(2), 0)))
    return words


def merge_ranges(ranges: Iterable[tuple[int, int]]) -> list[tuple[int, int]]:
    ordered = sorted(ranges)
    merged: list[tuple[int, int]] = []
    for start, end in ordered:
        if start >= end:
            continue
        if not merged or start > merged[-1][1]:
            merged.append((start, end))
        else:
            prev_start, prev_end = merged[-1]
            merged[-1] = (prev_start, max(prev_end, end))
    return merged


def collect_vtx_ranges(words: Iterable[tuple[int, int]], size: int) -> list[tuple[int, int]]:
    ranges: list[tuple[int, int]] = []
    for w0, w1 in words:
        if (w0 >> 24) != 0x01 or (w1 >> 24) != 0x02:
            continue
        count = (w0 >> 12) & 0xFF
        start = w1 & 0x00FFFFFF
        end = start + count * 0x10
        if 0 <= start < end <= size:
            ranges.append((start, end))
    return merge_ranges(ranges)


def _fmt_name(fmt: int) -> str:
    return {FMT_RGBA: "rgba", FMT_CI: "ci"}.get(fmt, f"fmt{fmt}")


def _siz_name(siz: int) -> str:
    return {SIZ_4B: "4", SIZ_8B: "8", SIZ_16B: "16"}.get(siz, str(siz))


def collect_texture_ranges(words: Iterable[tuple[int, int]], size: int) -> list[Range]:
    ranges: list[Range] = []
    last_timg: dict | None = None
    last_palette: dict | None = None
    tile_formats: dict[int, tuple[int, int]] = {}
    seen: set[tuple] = set()

    for w0, w1 in words:
        opcode = w0 >> 24

        if opcode == 0xFD and (w1 >> 24) == 0x02:
            last_timg = {
                "offset": w1 & 0x00FFFFFF,
                "fmt": (w0 >> 21) & 0x7,
                "siz": (w0 >> 19) & 0x3,
            }
        elif opcode == 0xF5:
            tile = (w1 >> 24) & 0x7
            tile_formats[tile] = ((w0 >> 21) & 0x7, (w0 >> 19) & 0x3)
        elif opcode == 0xF0 and last_timg:
            count = ((w1 >> 14) & 0x3FF) + 1
            start = last_timg["offset"]
            end = start + count * 2
            if 0 <= start < end <= size:
                key = ("palette", start, end)
                if key not in seen:
                    seen.add(key)
                    name = f"palette_{start:04X}"
                    ranges.append(
                        Range(start, end, "palette", name, {"colors": count, "format": "rgba16"})
                    )
                    last_palette = {"name": name, "start": start, "colors": count}
        elif opcode == 0xF2 and last_timg:
            tile = (w1 >> 24) & 0x7
            fmt, siz = tile_formats.get(tile, (last_timg["fmt"], last_timg["siz"]))
            width = ((w1 >> 12) & 0xFFF) // 4 + 1
            height = (w1 & 0xFFF) // 4 + 1

            if width <= 0 or height <= 0:
                continue

            byte_size: int | None = None
            format_name: str | None = None
            if fmt == FMT_CI and siz == SIZ_4B:
                byte_size = (width * height + 1) // 2
                format_name = "ci4"
            elif fmt == FMT_CI and siz == SIZ_8B:
                byte_size = width * height
                format_name = "ci8"
            elif fmt == FMT_RGBA and siz == SIZ_16B:
                byte_size = width * height * 2
                format_name = "rgba16"

            if byte_size is None or format_name is None:
                continue

            start = last_timg["offset"]
            end = start + byte_size
            if not (0 <= start < end <= size):
                continue

            key = ("texture", start, end, format_name, width, height, last_palette["name"] if last_palette else "")
            if key in seen:
                continue
            seen.add(key)
            ranges.append(
                Range(
                    start,
                    end,
                    "texture",
                    f"texture_{start:04X}",
                    {
                        "format": format_name,
                        "width": width,
                        "height": height,
                        "palette": last_palette["name"] if format_name.startswith("ci") and last_palette else None,
                        "source_format": f"{_fmt_name(fmt)}{_siz_name(siz)}",
                    },
                )
            )

    return resolve_ci_texture_palettes(ranges)


def resolve_ci_texture_palettes(ranges: Iterable[Range]) -> list[Range]:
    items = list(ranges)
    palettes = sorted((item for item in items if item.kind == "palette"), key=lambda item: item.start)
    resolved: list[Range] = []

    for item in items:
        if item.kind != "texture" or not str(item.meta.get("format", "")).startswith("ci"):
            resolved.append(item)
            continue
        if item.meta.get("palette"):
            resolved.append(item)
            continue

        candidates = [palette for palette in palettes if palette.start >= item.end]
        if not candidates:
            resolved.append(item)
            continue

        palette = min(candidates, key=lambda candidate: candidate.start - item.end)
        meta = dict(item.meta)
        meta["palette"] = palette.name
        resolved.append(replace(item, meta=meta))

    deduped: dict[tuple, Range] = {}
    order: list[tuple] = []
    for item in resolved:
        if item.kind == "texture":
            key = (
                item.kind,
                item.start,
                item.end,
                item.meta.get("format"),
                item.meta.get("width"),
                item.meta.get("height"),
            )
        else:
            key = (item.kind, item.start, item.end)

        old = deduped.get(key)
        if old is None:
            deduped[key] = item
            order.append(key)
        elif not old.meta.get("palette") and item.meta.get("palette"):
            deduped[key] = item

    return [deduped[key] for key in order]


def rgba16_to_rgba(value: int) -> tuple[int, int, int, int]:
    r = ((value >> 11) & 0x1F) * 255 // 31
    g = ((value >> 6) & 0x1F) * 255 // 31
    b = ((value >> 1) & 0x1F) * 255 // 31
    a = 255 if value & 1 else 0
    return r, g, b, a


def rgba_to_rgba16(color: tuple[int, int, int, int]) -> int:
    r, g, b, a = color
    return ((r * 31 + 127) // 255) << 11 | ((g * 31 + 127) // 255) << 6 | ((b * 31 + 127) // 255) << 1 | (1 if a >= 128 else 0)


def write_vtx_s(path: Path, data: bytes, start: int) -> None:
    lines = [
        f"/* MODEL_VTX_RANGE 0x{start:04X} 0x{start + len(data):04X} */",
        "/* x, y, z, flag, s, t, r, g, b, a */",
        "",
    ]
    for i in range(0, len(data), 16):
        x, y, z, flag, s, t = struct.unpack(">hhhHhh", data[i : i + 12])
        r, g, b, a = data[i + 12 : i + 16]
        lines.append(f"    .hword {x}, {y}, {z}, 0x{flag:04X}, {s}, {t}")
        lines.append(f"    .byte 0x{r:02X}, 0x{g:02X}, 0x{b:02X}, 0x{a:02X}")
    path.write_text("\n".join(lines) + "\n", encoding="utf-8", newline="\n")


def read_vtx_s(path: Path) -> bytes:
    values: list[int] = []
    for line in path.read_text(encoding="utf-8").splitlines():
        line = line.split("/*", 1)[0].split("#", 1)[0].strip()
        if not line.startswith((".hword", ".byte")):
            continue
        values.extend(int(part.strip(), 0) for part in line.split(None, 1)[1].split(","))

    if len(values) % 10 != 0:
        raise ValueError(f"{path} has {len(values)} scalar values; expected groups of 10")

    out = bytearray()
    for i in range(0, len(values), 10):
        x, y, z, flag, s, t, r, g, b, a = values[i : i + 10]
        out.extend(struct.pack(">hhhHhh", x, y, z, flag, s, t))
        out.extend(bytes((r & 0xFF, g & 0xFF, b & 0xFF, a & 0xFF)))
    return bytes(out)


def write_palette_s(path: Path, data: bytes, start: int) -> None:
    lines = [f"/* MODEL_PALETTE_RANGE 0x{start:04X} 0x{start + len(data):04X} */", ""]
    for i in range(0, len(data), 2):
        value = int.from_bytes(data[i : i + 2], "big")
        r, g, b, a = rgba16_to_rgba(value)
        lines.append(f"    .hword 0x{value:04X}  /* rgba({r}, {g}, {b}, {a}) */")
    path.write_text("\n".join(lines) + "\n", encoding="utf-8", newline="\n")


def read_palette_s(path: Path) -> list[int]:
    values: list[int] = []
    for line in path.read_text(encoding="utf-8").splitlines():
        line = line.split("/*", 1)[0].split("#", 1)[0].strip()
        if not line.startswith(".hword"):
            continue
        values.extend(int(part.strip(), 0) & 0xFFFF for part in line.split(None, 1)[1].split(","))
    return values


def write_texture_png(path: Path, data: bytes, meta: dict, palette_values: list[int] | None) -> None:
    try:
        from PIL import Image
    except ImportError as exc:
        raise RuntimeError("Pillow is required to extract modelpayload PNG textures") from exc

    width = int(meta["width"])
    height = int(meta["height"])
    fmt = meta["format"]

    if fmt == "rgba16":
        pixels = [rgba16_to_rgba(int.from_bytes(data[i : i + 2], "big")) for i in range(0, len(data), 2)]
        image = Image.new("RGBA", (width, height))
        image.putdata(pixels)
    elif fmt == "ci4":
        assert palette_values is not None
        indices = []
        for byte in data:
            indices.append((byte >> 4) & 0xF)
            indices.append(byte & 0xF)
        image = Image.new("P", (width, height))
        image.putdata(indices[: width * height])
    elif fmt == "ci8":
        assert palette_values is not None
        image = Image.new("P", (width, height))
        image.putdata(list(data[: width * height]))
    else:
        raise ValueError(f"unsupported texture format {fmt}")

    if fmt.startswith("ci"):
        assert palette_values is not None
        rgba_palette = [rgba16_to_rgba(value) for value in palette_values]
        rgb_palette: list[int] = []
        alpha_palette: list[int] = []
        for r, g, b, a in rgba_palette:
            rgb_palette.extend((r, g, b))
            alpha_palette.append(a)
        rgb_palette.extend([0] * (768 - len(rgb_palette)))
        alpha_palette.extend([255] * (256 - len(alpha_palette)))
        image.putpalette(rgb_palette)
        image.info["transparency"] = bytes(alpha_palette)

    image.save(path)


def read_texture_png(path: Path, meta: dict, palette_values: list[int] | None) -> bytes:
    try:
        from PIL import Image
    except ImportError as exc:
        raise RuntimeError("Pillow is required to pack modelpayload PNG textures") from exc

    image = Image.open(path)
    width = int(meta["width"])
    height = int(meta["height"])
    if image.size != (width, height):
        raise ValueError(f"{path} is {image.size}, expected {(width, height)}")

    fmt = meta["format"]

    if fmt == "rgba16":
        pixels = list(image.convert("RGBA").getdata())
        out = bytearray()
        for color in pixels:
            out.extend(rgba_to_rgba16(color).to_bytes(2, "big"))
        return bytes(out)

    if palette_values is None:
        raise ValueError(f"{path} needs a palette")

    if image.mode == "P":
        indices = list(image.getdata())
    else:
        pixels = list(image.convert("RGBA").getdata())
        palette = {rgba16_to_rgba(value): index for index, value in enumerate(palette_values)}
        indices = []
        for pixel_index, color in enumerate(pixels):
            if color not in palette:
                raise ValueError(f"{path} pixel {pixel_index} color {color} is not present in its palette")
            indices.append(palette[color])

    if fmt == "ci8":
        return bytes(indices)

    if fmt == "ci4":
        out = bytearray()
        for i in range(0, len(indices), 2):
            hi = indices[i]
            lo = indices[i + 1] if i + 1 < len(indices) else 0
            if hi > 0xF or lo > 0xF:
                raise ValueError(f"{path} uses palette index outside CI4 range")
            out.append((hi << 4) | lo)
        return bytes(out)

    raise ValueError(f"unsupported texture format {fmt}")


def range_overlaps(a: tuple[int, int], b: tuple[int, int]) -> bool:
    return a[0] < b[1] and b[0] < a[1]


def non_overlapping_ranges(ranges: Iterable[Range]) -> list[Range]:
    accepted: list[Range] = []
    for item in sorted(ranges, key=lambda r: (r.start, r.end, r.kind)):
        if any(range_overlaps((item.start, item.end), (old.start, old.end)) for old in accepted):
            continue
        accepted.append(item)
    return accepted


def complement_ranges(size: int, covered: Iterable[tuple[int, int]]) -> list[tuple[int, int]]:
    merged = merge_ranges(covered)
    raw: list[tuple[int, int]] = []
    pos = 0
    for start, end in merged:
        if pos < start:
            raw.append((pos, start))
        pos = max(pos, end)
    if pos < size:
        raw.append((pos, size))
    return raw
