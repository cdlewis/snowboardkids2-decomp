from __future__ import annotations

from pathlib import Path
from typing import Optional

from splat.segtypes.common.segment import CommonSegment
from splat.util import log, options

from tools.course_assets_common import write_palette_part, write_texture_part, write_yaml
from tools.modelpayload_common import read_palette_s
from tools.sno import decompress_sno_with_consumed
from tools.sprite_sheet_common import CI4_PALETTE_SIZE, FRAME_ENTRY_SIZE, HEADER_SIZE, parse_frame_entry


class N64SegSprite_sheet(CommonSegment):
    @staticmethod
    def is_data() -> bool:
        return True

    @property
    def statistics_type(self):
        return "sprite_sheet"

    def get_linker_section(self) -> str:
        return ".data"

    def get_section_flags(self) -> Optional[str]:
        return "wa"

    def out_path(self) -> Path:
        return options.opts.asset_path / "sprite_sheets" / f"{self.name}.yaml"

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

    def _frame_names(self) -> dict[int, str]:
        if not isinstance(self.yaml, dict):
            return {}
        names = self.yaml.get("frame_names", {})
        if not isinstance(names, dict):
            log.error(f"sprite sheet segment {self.name} frame_names must be a mapping")
        result = {}
        for key, value in names.items():
            index = int(key, 0) if isinstance(key, str) else int(key)
            result[index] = str(value)
        return result

    def split(self, rom_bytes: bytes):
        if self.rom_end is None:
            log.error(f"segment {self.name} needs to know where it ends")
        decompressed_size = self._yaml_int("decompressed_size")
        if decompressed_size is None:
            log.error(f"sprite sheet segment {self.name} needs decompressed_size")
        assert isinstance(self.rom_start, int)
        assert isinstance(self.rom_end, int)
        assert decompressed_size is not None

        compressed = rom_bytes[self.rom_start : self.rom_end]
        data, consumed = decompress_sno_with_consumed(compressed, decompressed_size)
        unused_tail = compressed[consumed:]
        texture_base = int.from_bytes(data[0:4], "big")
        frame_count = int.from_bytes(data[4:8], "big")
        palette_base = HEADER_SIZE + frame_count * FRAME_ENTRY_SIZE
        if frame_count == 0 or palette_base > len(data):
            log.error(f"sprite sheet segment {self.name} has invalid frame count {frame_count}")

        configured_names = self._frame_names()
        if any(index < 0 or index >= frame_count for index in configured_names):
            log.error(f"sprite sheet segment {self.name} has an out-of-range frame name")
        if len(set(configured_names.values())) != len(configured_names):
            log.error(f"sprite sheet segment {self.name} has duplicate frame names")

        frames = []
        for index in range(frame_count):
            try:
                meta = parse_frame_entry(data, index)
            except ValueError as exc:
                log.error(f"sprite sheet segment {self.name}: {exc}")
            frame_name = configured_names.get(index, f"frame_{index:02X}")
            frames.append(
                {
                    "index": index,
                    "name": frame_name,
                    "texture_offset": f"0x{meta['texture_offset']:X}",
                    "palette_index": meta["palette_index"],
                    "width": meta["width"],
                    "height": meta["height"],
                    "palette_table_index": meta["palette_table_index"],
                    "format_index": meta["format_index"],
                    "pad_0e": meta["pad_0e"],
                    "format": meta["format"],
                    "size": f"0x{meta['size']:X}",
                    "path": f"{self.name}/textures/{frame_name}.png",
                }
            )

        first_texture_offset = min(int(frame["texture_offset"], 0) for frame in frames)
        palette_data_size = first_texture_offset - palette_base
        if palette_data_size < 0 or palette_data_size % CI4_PALETTE_SIZE != 0:
            log.error(f"sprite sheet segment {self.name} has an invalid palette range")
        palette_count = palette_data_size // CI4_PALETTE_SIZE
        if palette_count == 0 or max(frame["palette_index"] for frame in frames) >= palette_count:
            log.error(f"sprite sheet segment {self.name} has an invalid palette reference")

        root = self.out_path().parent / self.name
        (root / "palettes").mkdir(parents=True, exist_ok=True)
        (root / "textures").mkdir(parents=True, exist_ok=True)

        palettes = []
        palette_values = {}
        for index in range(palette_count):
            offset = palette_base + index * CI4_PALETTE_SIZE
            name = f"palette_{index:02d}"
            rel_path = f"{self.name}/palettes/{name}.rgba16.s"
            write_palette_part(self.out_path().parent, rel_path, data[offset : offset + CI4_PALETTE_SIZE], offset)
            palettes.append(
                {
                    "index": index,
                    "name": name,
                    "offset": f"0x{offset:X}",
                    "path": rel_path,
                    "colors": 16,
                    "format": "rgba16",
                }
            )
            palette_values[index] = read_palette_s(self.out_path().parent / rel_path)

        for frame in frames:
            texture_offset = int(frame["texture_offset"], 0)
            texture_size = int(frame["size"], 0)
            write_texture_part(
                self.out_path().parent,
                frame["path"],
                data[texture_offset : texture_offset + texture_size],
                frame,
                palette_values[frame["palette_index"]],
            )

        covered = [(0, palette_base), (palette_base, first_texture_offset)]
        covered.extend(
            (int(frame["texture_offset"], 0), int(frame["texture_offset"], 0) + int(frame["size"], 0))
            for frame in frames
        )
        covered.sort()
        raw_blocks = []
        cursor = 0
        for start, end in covered:
            if start > cursor:
                rel_path = f"{self.name}/raw/raw_{cursor:04X}.bin"
                raw_path = self.out_path().parent / rel_path
                raw_path.parent.mkdir(parents=True, exist_ok=True)
                raw_path.write_bytes(data[cursor:start])
                raw_blocks.append(
                    {
                        "name": f"raw_{cursor:04X}",
                        "offset": f"0x{cursor:X}",
                        "size": f"0x{start - cursor:X}",
                        "path": rel_path,
                    }
                )
            cursor = max(cursor, end)
        if cursor < len(data):
            rel_path = f"{self.name}/raw/raw_{cursor:04X}.bin"
            raw_path = self.out_path().parent / rel_path
            raw_path.parent.mkdir(parents=True, exist_ok=True)
            raw_path.write_bytes(data[cursor:])
            raw_blocks.append(
                {
                    "name": f"raw_{cursor:04X}",
                    "offset": f"0x{cursor:X}",
                    "size": f"0x{len(data) - cursor:X}",
                    "path": rel_path,
                }
            )

        manifest = {
            "name": self.name,
            "format": "sprite_sheet",
            "compression": "sno",
            "decompressed_size": f"0x{decompressed_size:X}",
            "texture_base": f"0x{texture_base:X}",
            "palettes": palettes,
            "frames": frames,
        }
        if raw_blocks:
            manifest["raw_blocks"] = raw_blocks
        if unused_tail:
            manifest["unused_sno_tail"] = unused_tail.hex()
        write_yaml(self.out_path(), manifest)
        self.log(f"Wrote {self.name} to {self.out_path()}")
