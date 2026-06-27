from __future__ import annotations

from pathlib import Path
from typing import Optional

from splat.segtypes.common.segment import CommonSegment
from splat.util import log, options

from tools.course_assets_common import (
    course_display_list_ranges,
    course_segment_name,
    parse_gfx_words_from_bytes,
    write_palette_part,
    write_parts_manifest,
    write_texture_part,
    write_vtx_part,
)
from tools.modelpayload_common import (
    collect_texture_ranges,
    collect_vtx_ranges,
    complement_ranges,
    non_overlapping_ranges,
    read_palette_s,
)
from tools.sno import decompress_sno_with_consumed


class N64SegCourse_model_resources(CommonSegment):
    @staticmethod
    def is_data() -> bool:
        return True

    @property
    def statistics_type(self):
        return "course_model_resources"

    def get_linker_section(self) -> str:
        return ".data"

    def get_section_flags(self) -> Optional[str]:
        return "wa"

    def out_path(self) -> Path:
        return options.opts.asset_path / "courses" / "model_resources" / f"{self.name}.yaml"

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

    def _level_id(self) -> str:
        if not isinstance(self.yaml, dict) or "level_id" not in self.yaml:
            log.error(f"course model resource segment {self.name} needs level_id")
        assert isinstance(self.yaml, dict)
        return str(self.yaml["level_id"])

    def _display_list_name(self) -> str:
        return course_segment_name(options.opts.base_path / "snowboardkids2.yaml", "course_display_lists", self._level_id())

    def _display_list_bytes(self, rom_bytes: bytes) -> bytes:
        config_path = options.opts.base_path / "snowboardkids2.yaml"
        display_list_name = self._display_list_name()
        ranges = course_display_list_ranges(config_path)
        if display_list_name not in ranges:
            log.error(f"course model resource segment {self.name} references unknown display-list segment {display_list_name}")
        start, end = ranges[display_list_name]
        return rom_bytes[start:end]

    def split(self, rom_bytes: bytes):
        if self.rom_end is None:
            log.error(f"segment {self.name} needs to know where it ends")
        decompressed_size = self._yaml_int("decompressed_size")
        if decompressed_size is None:
            log.error(f"course model resource segment {self.name} needs decompressed_size")
        assert isinstance(self.rom_start, int)
        assert isinstance(self.rom_end, int)
        assert decompressed_size is not None

        compressed = rom_bytes[self.rom_start : self.rom_end]
        decompressed, consumed = decompress_sno_with_consumed(compressed, decompressed_size)
        unused_tail = compressed[consumed:]

        words = parse_gfx_words_from_bytes(self._display_list_bytes(rom_bytes))
        vtx_ranges = collect_vtx_ranges(words, decompressed_size)
        texture_ranges = non_overlapping_ranges(collect_texture_ranges(words, decompressed_size))
        covered: list[tuple[int, int]] = []
        parts: list[dict] = []
        root = self.out_path().parent / self.name
        for folder in ("vtx", "palettes", "textures"):
            (root / folder).mkdir(parents=True, exist_ok=True)

        for start, end in vtx_ranges:
            name = f"vtx_{start:04X}"
            rel_path = f"{self.name}/vtx/{name}.vtx.s"
            write_vtx_part(self.out_path().parent, rel_path, decompressed[start:end], start)
            parts.append({"name": name, "type": "vtx", "offset": f"0x{start:X}", "size": f"0x{end - start:X}", "path": rel_path})
            covered.append((start, end))

        palette_paths: dict[str, Path] = {}
        for item in texture_ranges:
            if item.kind != "palette":
                continue
            rel_path = f"{self.name}/palettes/{item.name}.rgba16.s"
            out_path = self.out_path().parent / rel_path
            write_palette_part(self.out_path().parent, rel_path, decompressed[item.start : item.end], item.start)
            palette_paths[item.name] = out_path
            parts.append(
                {
                    "name": item.name,
                    "type": "palette",
                    "offset": f"0x{item.start:X}",
                    "size": f"0x{item.size:X}",
                    "path": rel_path,
                    "format": item.meta["format"],
                    "colors": item.meta["colors"],
                }
            )
            covered.append((item.start, item.end))

        for item in texture_ranges:
            if item.kind != "texture":
                continue
            palette_name = item.meta.get("palette")
            palette_values = read_palette_s(palette_paths[palette_name]) if palette_name else None
            rel_path = f"{self.name}/textures/{item.name}.png"
            write_texture_part(self.out_path().parent, rel_path, decompressed[item.start : item.end], item.meta, palette_values)
            parts.append(
                {
                    "name": item.name,
                    "type": "texture",
                    "offset": f"0x{item.start:X}",
                    "size": f"0x{item.size:X}",
                    "path": rel_path,
                    "format": item.meta["format"],
                    "width": item.meta["width"],
                    "height": item.meta["height"],
                    "palette": palette_name,
                }
            )
            covered.append((item.start, item.end))

        uncovered = complement_ranges(decompressed_size, covered)
        if uncovered:
            ranges = ", ".join(f"0x{start:X}-0x{end:X}" for start, end in uncovered)
            log.error(f"course model resource segment {self.name} has unclassified ranges: {ranges}")

        parts.sort(key=lambda item: int(str(item["offset"]), 0))
        extra = {"format": "course_model_resources", "compression": "sno", "level_id": self._level_id()}
        if unused_tail:
            extra["unused_sno_tail"] = unused_tail.hex()
        write_parts_manifest(
            self.out_path(),
            self.name,
            decompressed_size,
            parts,
            extra,
        )
        self.log(f"Wrote {self.name} to {self.out_path()}")
