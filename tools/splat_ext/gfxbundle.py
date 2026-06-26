from pathlib import Path
from typing import Optional

from splat.segtypes.common.segment import CommonSegment
from splat.util import log, options


GBI_COMMAND_NAMES = {
    0x00: "G_NOOP",
    0x01: "G_VTX",
    0x02: "G_MODIFYVTX",
    0x03: "G_CULLDL",
    0x04: "G_BRANCH_Z",
    0x05: "G_TRI1",
    0x06: "G_TRI2",
    0x07: "G_QUAD",
    0x08: "G_LINE3D",
    0xD7: "G_TEXTURE",
    0xD9: "G_GEOMETRYMODE",
    0xDA: "G_MTX",
    0xDB: "G_MOVEWORD",
    0xDC: "G_MOVEMEM",
    0xDD: "G_LOAD_UCODE",
    0xDE: "G_DL",
    0xDF: "G_ENDDL",
    0xE1: "G_RDPHALF_1",
    0xE2: "G_SETOTHERMODE_L",
    0xE3: "G_SETOTHERMODE_H",
    0xE6: "G_RDPLOADSYNC",
    0xE7: "G_RDPPIPESYNC",
    0xE8: "G_RDPTILESYNC",
    0xF0: "G_LOADTLUT",
    0xF2: "G_SETTILESIZE",
    0xF3: "G_LOADBLOCK",
    0xF4: "G_LOADTILE",
    0xF5: "G_SETTILE",
    0xF7: "G_SETFILLCOLOR",
    0xF8: "G_SETFOGCOLOR",
    0xFA: "G_SETPRIMCOLOR",
    0xFB: "G_SETENVCOLOR",
    0xFC: "G_SETCOMBINE",
    0xFD: "G_SETTIMG",
}

REFERENCE_COMMANDS = {
    0x01: "vtx",
    0xDA: "mtx",
    0xDC: "movemem",
    0xDD: "ucode",
    0xDE: "dl",
    0xFD: "timg",
}


class N64SegGfxbundle(CommonSegment):
    @staticmethod
    def is_data() -> bool:
        return True

    @property
    def statistics_type(self):
        return "gfxbundle"

    def get_linker_section(self) -> str:
        return ".data"

    def get_section_flags(self) -> Optional[str]:
        return "wa"

    def out_path(self) -> Path:
        return options.opts.asset_path / "gfxbin" / self.dir / f"{self.name}.s"

    def should_split(self) -> bool:
        return self.extract and (
            options.opts.is_mode_active(self.type)
            or options.opts.is_mode_active("bin")
            or options.opts.is_mode_active("all")
        )

    def _format_comment(self, offset: int, w0: int, w1: int) -> str:
        opcode = (w0 >> 24) & 0xFF
        name = GBI_COMMAND_NAMES.get(opcode, f"UNKNOWN_0x{opcode:02X}")
        comment = f"0x{offset:04X}: {name}"

        if opcode in REFERENCE_COMMANDS:
            segment = (w1 >> 24) & 0xFF
            segment_offset = w1 & 0x00FFFFFF
            comment += f" {REFERENCE_COMMANDS[opcode]}=seg{segment}:0x{segment_offset:X}"

        return comment

    def _validate(self, data: bytes) -> None:
        if len(data) % 8 != 0:
            log.error(
                f"gfxbundle segment {self.name} length 0x{len(data):X} is not a multiple of 8"
            )

        invalid = []
        enddl_count = 0
        for offset in range(0, len(data), 8):
            opcode = data[offset]
            if opcode == 0xDF:
                enddl_count += 1
            if opcode not in GBI_COMMAND_NAMES:
                w0 = int.from_bytes(data[offset : offset + 4], "big")
                w1 = int.from_bytes(data[offset + 4 : offset + 8], "big")
                invalid.append((offset, opcode, w0, w1))

        if invalid:
            examples = ", ".join(
                f"0x{offset:X}:0x{opcode:02X}"
                for offset, opcode, _w0, _w1 in invalid[:8]
            )
            log.error(
                f"gfxbundle segment {self.name} has {len(invalid)} unknown opcodes: {examples}"
            )

        if enddl_count == 0:
            log.error(f"gfxbundle segment {self.name} has no G_ENDDL commands")

    def split(self, rom_bytes: bytes):
        if self.rom_end is None:
            log.error(
                f"segment {self.name} needs to know where it ends; add a position marker after it"
            )

        if self.size is None or self.size <= 0:
            log.error(f"Segment {self.name} has zero size.")

        assert isinstance(self.rom_start, int)
        assert isinstance(self.rom_end, int)

        data = rom_bytes[self.rom_start : self.rom_end]
        self._validate(data)

        out_path = self.out_path()
        out_path.parent.mkdir(parents=True, exist_ok=True)

        lines = [
            f"/* Auto-generated from ROM range 0x{self.rom_start:X}-0x{self.rom_end:X}. */",
            "/* This file is rebuildable source for a bundled F3DEX2 display-list segment. */",
            "",
            '.section .data, "wa"',
            "",
            f".globl {self.name}",
            f"{self.name}:",
            f"{self.name}_part_0000:",
        ]

        next_part_offset: Optional[int] = None
        for offset in range(0, len(data), 8):
            if next_part_offset == offset:
                lines.append("")
                lines.append(f"{self.name}_part_{offset:04X}:")
                next_part_offset = None

            w0 = int.from_bytes(data[offset : offset + 4], "big")
            w1 = int.from_bytes(data[offset + 4 : offset + 8], "big")
            comment = self._format_comment(offset, w0, w1)
            lines.append(f"    .word 0x{w0:08X}, 0x{w1:08X}  /* {comment} */")

            if data[offset] == 0xDF and offset + 8 < len(data):
                next_part_offset = offset + 8

        lines.extend(
            [
                f"{self.name}_end:",
                f".size {self.name}, {self.name}_end - {self.name}",
                "",
            ]
        )

        out_path.write_text("\n".join(lines), encoding="utf-8", newline="\n")
        self.log(f"Wrote {self.name} to {out_path}")
