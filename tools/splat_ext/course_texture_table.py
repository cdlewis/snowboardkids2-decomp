from __future__ import annotations

from pathlib import Path
from typing import Optional

from splat.segtypes.common.segment import CommonSegment
from splat.util import log, options

from tools.course_assets_common import write_palette_part, write_texture_part, write_yaml
from tools.modelpayload_common import read_palette_s
from tools.sno import decompress_sno_with_consumed


class N64SegCourse_texture_table(CommonSegment):
    @staticmethod
    def is_data() -> bool:
        return True

    @property
    def statistics_type(self):
        return "course_texture_table"

    def get_linker_section(self) -> str:
        return ".data"

    def get_section_flags(self) -> Optional[str]:
        return "wa"

    def out_path(self) -> Path:
        return options.opts.asset_path / "courses" / "texture_tables" / f"{self.name}.yaml"

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

    def _course_id(self) -> str:
        if not isinstance(self.yaml, dict) or "course_id" not in self.yaml:
            log.error(f"course texture table segment {self.name} needs course_id")
        assert isinstance(self.yaml, dict)
        return str(self.yaml["course_id"])

    def split(self, rom_bytes: bytes):
        if self.rom_end is None:
            log.error(f"segment {self.name} needs to know where it ends")
        decompressed_size = self._yaml_int("decompressed_size")
        if decompressed_size is None:
            log.error(f"course texture table segment {self.name} needs decompressed_size")
        assert isinstance(self.rom_start, int)
        assert isinstance(self.rom_end, int)
        assert decompressed_size is not None

        compressed = rom_bytes[self.rom_start : self.rom_end]
        data, consumed = decompress_sno_with_consumed(compressed, decompressed_size)
        unused_tail = compressed[consumed:]

        entry_count = int.from_bytes(data[4:8], "big")
        palette_base = 8 + entry_count * 16
        if palette_base > len(data):
            log.error(f"course texture table segment {self.name} has invalid entry count {entry_count}")

        entries = []
        max_palette_index = 0
        for index in range(entry_count):
            offset = 8 + index * 16
            texture_offset = int.from_bytes(data[offset : offset + 4], "big")
            palette_index = int.from_bytes(data[offset + 4 : offset + 6], "big")
            width = int.from_bytes(data[offset + 6 : offset + 8], "big")
            height = int.from_bytes(data[offset + 8 : offset + 10], "big")
            unknown_0a = data[offset + 10 : offset + 16]
            max_palette_index = max(max_palette_index, palette_index)
            size = (width * height + 1) // 2
            entry = {
                "name": f"texture_{index:02d}",
                "texture_offset": f"0x{texture_offset:X}",
                "palette_index": palette_index,
                "width": width,
                "height": height,
                "size": f"0x{size:X}",
                "format": "ci4",
                "path": f"{self.name}/textures/texture_{index:02d}.png",
            }
            if any(unknown_0a):
                entry["unknown_0a"] = unknown_0a.hex()
            entries.append(entry)

        root = self.out_path().parent / self.name
        (root / "palettes").mkdir(parents=True, exist_ok=True)
        (root / "textures").mkdir(parents=True, exist_ok=True)

        palettes = []
        for index in range(max_palette_index + 1):
            offset = palette_base + index * 0x20
            rel_path = f"{self.name}/palettes/palette_{index:02d}.rgba16.s"
            write_palette_part(self.out_path().parent, rel_path, data[offset : offset + 0x20], offset)
            palettes.append({"name": f"palette_{index:02d}", "offset": f"0x{offset:X}", "path": rel_path, "colors": 16, "format": "rgba16"})

        palette_values = {palette["name"]: read_palette_s(self.out_path().parent / palette["path"]) for palette in palettes}
        for entry in entries:
            texture_offset = int(entry["texture_offset"], 0)
            size = int(entry["size"], 0)
            palette_name = palettes[entry["palette_index"]]["name"]
            write_texture_part(self.out_path().parent, entry["path"], data[texture_offset : texture_offset + size], entry, palette_values[palette_name])

        covered = [(8, 8 + entry_count * 16)]
        covered.extend((int(palette["offset"], 0), int(palette["offset"], 0) + 0x20) for palette in palettes)
        covered.extend((int(entry["texture_offset"], 0), int(entry["texture_offset"], 0) + int(entry["size"], 0)) for entry in entries)
        covered.sort()
        raw_blocks = []
        cursor = 0
        for start, end in covered:
            if start > cursor and any(data[cursor:start]):
                rel_path = f"{self.name}/raw/raw_{cursor:04X}.bin"
                out_path = self.out_path().parent / rel_path
                out_path.parent.mkdir(parents=True, exist_ok=True)
                out_path.write_bytes(data[cursor:start])
                raw_blocks.append({"name": f"raw_{cursor:04X}", "offset": f"0x{cursor:X}", "size": f"0x{start - cursor:X}", "path": rel_path})
            cursor = max(cursor, end)
        if cursor < len(data) and any(data[cursor:]):
            rel_path = f"{self.name}/raw/raw_{cursor:04X}.bin"
            out_path = self.out_path().parent / rel_path
            out_path.parent.mkdir(parents=True, exist_ok=True)
            out_path.write_bytes(data[cursor:])
            raw_blocks.append({"name": f"raw_{cursor:04X}", "offset": f"0x{cursor:X}", "size": f"0x{len(data) - cursor:X}", "path": rel_path})

        manifest = {
            "name": self.name,
            "format": "course_texture_table",
            "compression": "sno",
            "course_id": self._course_id(),
            "decompressed_size": f"0x{decompressed_size:X}",
            "palettes": palettes,
            "entries": entries,
        }
        if raw_blocks:
            manifest["raw_blocks"] = raw_blocks
        if unused_tail:
            manifest["unused_sno_tail"] = unused_tail.hex()
        write_yaml(self.out_path(), manifest)
        self.log(f"Wrote {self.name} to {self.out_path()}")
