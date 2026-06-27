from pathlib import Path
from typing import Optional

from splat.segtypes.common.segment import CommonSegment
from splat.util import log, options

from tools.splat_ext.gfxbundle import GBI_COMMAND_NAMES, REFERENCE_COMMANDS


class N64SegCourse_display_lists(CommonSegment):
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
        return options.opts.asset_path / "courses" / "display_lists" / f"{self.name}.s"

    def should_split(self) -> bool:
        return self.extract and (
            options.opts.is_mode_active(self.type)
            or options.opts.is_mode_active("bin")
            or options.opts.is_mode_active("all")
        )

    def _comment(self, offset: int, w0: int, w1: int) -> str:
        opcode = (w0 >> 24) & 0xFF
        comment = f"0x{offset:04X}: {GBI_COMMAND_NAMES.get(opcode, f'UNKNOWN_0x{opcode:02X}')}"
        if opcode in REFERENCE_COMMANDS:
            segment = (w1 >> 24) & 0xFF
            segment_offset = w1 & 0x00FFFFFF
            comment += f" {REFERENCE_COMMANDS[opcode]}=seg{segment}:0x{segment_offset:X}"
        return comment

    def split(self, rom_bytes: bytes):
        if self.rom_end is None:
            log.error(f"segment {self.name} needs to know where it ends")
        assert isinstance(self.rom_start, int)
        assert isinstance(self.rom_end, int)
        data = rom_bytes[self.rom_start : self.rom_end]
        if len(data) % 8 != 0:
            log.error(f"course display-list segment {self.name} size 0x{len(data):X} is not a multiple of 8")

        lines = [
            f"/* Auto-generated from ROM range 0x{self.rom_start:X}-0x{self.rom_end:X}. */",
            "/* Rebuildable F3DEX display-list command stream for course segment 1. */",
            "",
            '.section .data, "wa"',
            "",
            f".globl {self.name}",
            f"{self.name}:",
        ]
        for offset in range(0, len(data), 8):
            w0 = int.from_bytes(data[offset : offset + 4], "big")
            w1 = int.from_bytes(data[offset + 4 : offset + 8], "big")
            lines.append(f"    .word 0x{w0:08X}, 0x{w1:08X}  /* {self._comment(offset, w0, w1)} */")
        lines.extend([f"{self.name}_end:", f".size {self.name}, {self.name}_end - {self.name}", ""])

        out_path = self.out_path()
        out_path.parent.mkdir(parents=True, exist_ok=True)
        out_path.write_text("\n".join(lines), encoding="utf-8", newline="\n")
        self.log(f"Wrote {self.name} to {out_path}")
