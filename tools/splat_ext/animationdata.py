from __future__ import annotations

from pathlib import Path
import struct
from typing import Optional

from splat.segtypes.common.segment import CommonSegment
from splat.util import log, options

from tools.sno import decompress_sno_with_consumed


def _u32(data: bytes, offset: int) -> int:
    return struct.unpack_from(">I", data, offset)[0]


def _part_name(part_type: str, offset: int) -> str:
    return f"{part_type}_{offset:04X}"


class N64SegAnimationdata(CommonSegment):
    @staticmethod
    def is_data() -> bool:
        return True

    @property
    def statistics_type(self):
        return "animationdata"

    def get_linker_section(self) -> str:
        return ".data"

    def get_section_flags(self) -> Optional[str]:
        return "wa"

    def out_path(self) -> Path:
        return options.opts.asset_path / "animationdata" / self.dir / f"{self.name}.yaml"

    def should_split(self) -> bool:
        return self.extract and (
            options.opts.is_mode_active(self.type)
            or options.opts.is_mode_active("bin")
            or options.opts.is_mode_active("all")
        )

    def _yaml_int(self, key: str) -> Optional[int]:
        if not isinstance(self.yaml, dict) or key not in self.yaml:
            return None
        value = self.yaml[key]
        return int(value, 0) if isinstance(value, str) else int(value)

    def _yaml_str(self, key: str) -> Optional[str]:
        if not isinstance(self.yaml, dict) or key not in self.yaml:
            return None
        return str(self.yaml[key])

    def _raw_range(self, size: int) -> list[dict]:
        return [
            {
                "name": "raw_data",
                "type": "raw_data",
                "offset": 0,
                "size": size,
                "path": f"{self.name}/raw_data.s",
            }
        ]

    def _collect_model_ranges(self, decompressed: bytes) -> list[dict]:
        size = len(decompressed)
        first_offset = _u32(decompressed, 0)
        if first_offset <= 0 or first_offset > size or first_offset % 4 != 0:
            return self._raw_range(size)

        header_offsets = [_u32(decompressed, offset) for offset in range(0, first_offset, 4)]
        data_offsets = sorted({offset for offset in header_offsets if 0 <= offset < size})
        if first_offset not in data_offsets:
            log.error(f"animationdata segment {self.name} header does not reference its first data range")

        boundaries = data_offsets + [size]
        ranges = [
            {
                "name": "offset_table",
                "type": "offset_table",
                "offset": 0,
                "size": first_offset,
                "path": f"{self.name}/offset_table.s",
            }
        ]

        for index, start in enumerate(data_offsets):
            end = boundaries[index + 1]
            if end <= start:
                continue
            if index == 0:
                part_type = "index_data"
            elif index == 1:
                part_type = "frame_data"
            else:
                part_type = "track_table"

            name = _part_name(part_type, start)
            ranges.append(
                {
                    "name": name,
                    "type": part_type,
                    "offset": start,
                    "size": end - start,
                    "path": f"{self.name}/{name}.s",
                }
            )

        return ranges

    def _collect_scene_ranges(self, decompressed: bytes) -> list[dict]:
        size = len(decompressed)
        if size < 8:
            return self._raw_range(size)

        header_offset = _u32(decompressed, 0)
        entry_offset = _u32(decompressed, 4)
        if (
            header_offset != 8
            or entry_offset <= header_offset
            or entry_offset > size
            or entry_offset % 4 != 0
        ):
            return self._raw_range(size)

        return [
            {
                "name": "offset_table",
                "type": "offset_table",
                "offset": 0,
                "size": 8,
                "path": f"{self.name}/offset_table.s",
            },
            {
                "name": _part_name("header_data", header_offset),
                "type": "header_data",
                "offset": header_offset,
                "size": entry_offset - header_offset,
                "path": f"{self.name}/{_part_name('header_data', header_offset)}.s",
            },
            {
                "name": _part_name("entry_data", entry_offset),
                "type": "entry_data",
                "offset": entry_offset,
                "size": size - entry_offset,
                "path": f"{self.name}/{_part_name('entry_data', entry_offset)}.s",
            },
        ]

    def _collect_ranges(self, decompressed: bytes) -> list[dict]:
        kind = self._yaml_str("kind") or "model"
        if kind == "scene":
            return self._collect_scene_ranges(decompressed)
        if kind != "model":
            log.error(f"animationdata segment {self.name} has unsupported kind {kind!r}")
        return self._collect_model_ranges(decompressed)

    def _write_offset_table_s(self, path: Path, data: bytes, parts: list[dict]) -> None:
        names_by_offset = {part["offset"]: part["name"] for part in parts if part["offset"] != 0}
        lines = [
            f"/* ANIMATIONDATA_OFFSET_TABLE {self.name} */",
            "/* First word is also used by getIndexedAnimationDataPtr as the hierarchy/index base offset. */",
            "",
        ]
        for index, offset in enumerate(range(0, len(data), 4)):
            value = _u32(data, offset)
            label = names_by_offset.get(value)
            comment = f"offset[{index}]"
            if label:
                comment += f" -> {label}"
            lines.append(f"    .word 0x{value:08X}  /* {comment} */")
        path.write_text("\n".join(lines) + "\n", encoding="utf-8", newline="\n")

    def _write_index_data_s(self, path: Path, data: bytes, start: int) -> None:
        lines = [
            f"/* ANIMATIONDATA_INDEX_DATA 0x{start:04X} 0x{start + len(data):04X} */",
            "/* byte pairs: bone/index byte, parent/index byte */",
            "",
        ]
        for offset in range(0, len(data), 2):
            chunk = data[offset : offset + 2]
            values = ", ".join(f"0x{value:02X}" for value in chunk)
            lines.append(f"    .byte {values}")
        path.write_text("\n".join(lines) + "\n", encoding="utf-8", newline="\n")

    def _write_hword_s(self, path: Path, data: bytes, start: int, part_type: str) -> None:
        lines = [
            f"/* ANIMATIONDATA_{part_type.upper()} 0x{start:04X} 0x{start + len(data):04X} */",
            "",
        ]
        hword_size = len(data) & ~1
        for offset in range(0, hword_size, 2):
            value = struct.unpack_from(">H", data, offset)[0]
            lines.append(f"    .hword 0x{value:04X}")
        if hword_size != len(data):
            lines.append(f"    .byte 0x{data[-1]:02X}")
        path.write_text("\n".join(lines) + "\n", encoding="utf-8", newline="\n")

    def _write_parts(self, manifest_path: Path, decompressed: bytes, parts: list[dict]) -> None:
        root = manifest_path.parent / self.name
        root.mkdir(parents=True, exist_ok=True)

        for part in parts:
            start = part["offset"]
            end = start + part["size"]
            path = manifest_path.parent / part["path"]
            data = decompressed[start:end]
            if part["type"] == "offset_table":
                self._write_offset_table_s(path, data, parts)
            elif part["type"] == "index_data":
                self._write_index_data_s(path, data, start)
            else:
                self._write_hword_s(path, data, start, part["type"])

    def split(self, rom_bytes: bytes):
        if self.rom_end is None:
            log.error(f"segment {self.name} needs to know where it ends; add a position marker after it")

        if self.size is None or self.size <= 0:
            log.error(f"Segment {self.name} has zero size.")

        decompressed_size = self._yaml_int("decompressed_size")
        if decompressed_size is None:
            log.error(f"animationdata segment {self.name} needs decompressed_size")

        assert isinstance(self.rom_start, int)
        assert isinstance(self.rom_end, int)
        assert decompressed_size is not None

        compressed = rom_bytes[self.rom_start : self.rom_end]
        decompressed, consumed_size = decompress_sno_with_consumed(compressed, decompressed_size)
        unused_sno_tail = compressed[consumed_size:]

        manifest_path = self.out_path()
        parts = self._collect_ranges(decompressed)
        self._write_parts(manifest_path, decompressed, parts)

        lines = [
            f"name: {self.name}",
            f"compressed_size: 0x{self.size:X}",
            f"decompressed_size: 0x{decompressed_size:X}",
            "compression: sno",
            f"kind: {self._yaml_str('kind') or 'model'}",
        ]

        group = self._yaml_str("group")
        if group:
            lines.append(f"group: {group}")

        model_id = self._yaml_str("model_id")
        if model_id:
            lines.append(f"model_id: {model_id}")

        course_id = self._yaml_str("course_id")
        if course_id:
            lines.append(f"course_id: {course_id}")

        if unused_sno_tail and any(unused_sno_tail):
            lines.append(f"unused_sno_tail: {unused_sno_tail.hex()}")

        lines.append("parts:")
        for part in parts:
            lines.append(f"  - name: {part['name']}")
            lines.append(f"    type: {part['type']}")
            lines.append(f"    offset: 0x{part['offset']:X}")
            lines.append(f"    size: 0x{part['size']:X}")
            lines.append(f"    path: {part['path']}")

        manifest_path.write_text("\n".join(lines) + "\n", encoding="utf-8", newline="\n")
        self.log(f"Wrote {self.name} to {manifest_path}")
