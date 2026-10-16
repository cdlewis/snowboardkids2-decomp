from pathlib import Path
from typing import Optional

from tools.f3dex2_decode import F3dex2Decoder

from splat.segtypes.common.segment import CommonSegment
from splat.util import log, options

from tools.course_assets_common import course_segment_name


class N64SegCourse_display_lists(CommonSegment):
    _cached_model_resource_name: str | None = None

    @staticmethod
    def is_data() -> bool:
        return True

    @property
    def statistics_type(self):
        return "course_display_lists"

    def get_linker_section(self) -> str:
        return ".data"

    def get_section_flags(self) -> Optional[str]:
        return "wa"

    def out_path(self) -> Path:
        return options.opts.asset_path / "courses" / "display_lists" / f"{self.name}.c"

    def should_split(self) -> bool:
        return self.extract and (
            options.opts.is_mode_active(self.type)
            or options.opts.is_mode_active("bin")
            or options.opts.is_mode_active("all")
        )

    def _model_resource_name(self) -> str:
        if self._cached_model_resource_name is not None:
            return self._cached_model_resource_name

        if not isinstance(self.yaml, dict) or "course_id" not in self.yaml:
            log.error(f"course display-list segment {self.name} needs course_id")
        assert isinstance(self.yaml, dict)
        self._cached_model_resource_name = course_segment_name(
            options.opts.base_path / "snowboardkids2.yaml",
            "course_model_resources",
            str(self.yaml["course_id"]),
        )
        return self._cached_model_resource_name

    def _segmented_symbol(self, addr: int, kind: str) -> str:
        segment = (addr >> 24) & 0xFF
        offset = addr & 0x00FFFFFF
        if segment == 1:
            return f"{self.name}_display_list_{offset:04X}"
        if segment == 2:
            return f"{self._model_resource_name()}_{kind}_{offset:04X}"
        return f"0x{addr:08X}"

    def _disassemble_chunk(self, data: bytes) -> str:
        return F3dex2Decoder(self._segmented_symbol).decode(data)

    def _disassemble(self, data: bytes) -> str:
        lines: list[str] = []
        chunk_start = 0
        for offset in range(0, len(data), 8):
            opcode = data[offset]
            if opcode != 0xDF:
                continue
            chunk = data[chunk_start : offset + 8]
            lines.extend(self._disassemble_chunk(chunk).rstrip().splitlines())
            chunk_start = offset + 8

        if chunk_start < len(data):
            lines.extend(self._disassemble_chunk(data[chunk_start:]).rstrip().splitlines())

        return "\n".join(line for line in lines if line)

    def split(self, rom_bytes: bytes):
        if self.rom_end is None:
            log.error(f"segment {self.name} needs to know where it ends")
        assert isinstance(self.rom_start, int)
        assert isinstance(self.rom_end, int)
        data = rom_bytes[self.rom_start : self.rom_end]
        if len(data) % 8 != 0:
            log.error(f"course display-list segment {self.name} size 0x{len(data):X} is not a multiple of 8")

        macro_lines = self._disassemble(data).rstrip().splitlines()
        lines = [
            f"/* Auto-generated from ROM range 0x{self.rom_start:X}-0x{self.rom_end:X}. */",
            "/* Rebuildable F3DEX display-list command stream for course segment 1. */",
            "",
            '#include "common.h"',
            '#include "gbi.h"',
            '#include "generated/course_asset_sizes.h"',
            "",
            f"Gfx {self.name}[] = {{",
        ]
        lines.extend(macro_lines)
        lines.extend(["};", ""])

        out_path = self.out_path()
        out_path.parent.mkdir(parents=True, exist_ok=True)
        out_path.write_text("\n".join(lines), encoding="utf-8", newline="\n")
        self.log(f"Wrote {self.name} to {out_path}")
