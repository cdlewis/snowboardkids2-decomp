from __future__ import annotations

from pathlib import Path
from typing import Optional

from splat.segtypes.common.segment import CommonSegment
from splat.util import log, options

from tools.course_position_common import unpack_item_box_positions, write_position_manifest
from tools.sno import decompress_sno_with_consumed


class N64SegItem_box_positions(CommonSegment):
    @staticmethod
    def is_data() -> bool:
        return True

    @property
    def statistics_type(self):
        return "item_box_positions"

    def get_linker_section(self) -> str:
        return ".data"

    def get_section_flags(self) -> Optional[str]:
        return "wa"

    def out_path(self) -> Path:
        return options.opts.asset_path / "course_positions" / "item_boxes" / f"{self.name}.yaml"

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

    def split(self, rom_bytes: bytes):
        if self.rom_end is None:
            log.error(f"segment {self.name} needs to know where it ends")
        decompressed_size = self._yaml_int("decompressed_size")
        if decompressed_size is None:
            log.error(f"item box positions segment {self.name} needs decompressed_size")

        assert isinstance(self.rom_start, int)
        assert isinstance(self.rom_end, int)
        assert decompressed_size is not None

        compressed = rom_bytes[self.rom_start : self.rom_end]
        data, consumed = decompress_sno_with_consumed(compressed, decompressed_size)
        manifest = unpack_item_box_positions(self.name, data, decompressed_size, compressed[consumed:])
        write_position_manifest(self.out_path(), manifest)
        self.log(f"Wrote {self.name} to {self.out_path()}")
