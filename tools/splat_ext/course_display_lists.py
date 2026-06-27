from pathlib import Path
from typing import Optional

from pygfxd import (
    GfxdEndian,
    gfxd_buffer_to_string,
    gfxd_cimg_callback,
    gfxd_dl_callback,
    gfxd_endian,
    gfxd_execute,
    gfxd_f3dex2,
    gfxd_input_buffer,
    gfxd_light_callback,
    gfxd_lookat_callback,
    gfxd_macro_dflt,
    gfxd_macro_fn,
    gfxd_mtx_callback,
    gfxd_output_buffer,
    gfxd_printf,
    gfxd_puts,
    gfxd_target,
    gfxd_timg_callback,
    gfxd_tlut_callback,
    gfxd_vp_callback,
    gfxd_vtx_callback,
    gfxd_zimg_callback,
)
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

    def _macro_fn(self):
        gfxd_puts("    ")
        gfxd_macro_dflt()
        gfxd_puts(",\n")
        return 0

    def _print_addr(self, addr: int, kind: str) -> int:
        gfxd_printf(self._segmented_symbol(addr, kind))
        return 1

    def _tlut_handler(self, addr, idx, count):
        return self._print_addr(addr, "palette")

    def _timg_handler(self, addr, fmt, size, width, height, pal):
        kind = "palette" if fmt == 0 and size == 2 else "texture"
        return self._print_addr(addr, kind)

    def _dl_handler(self, addr):
        return self._print_addr(addr, "display_list")

    def _mtx_handler(self, addr):
        return self._print_addr(addr, "matrix")

    def _lookat_handler(self, addr, count):
        return self._print_addr(addr, "lookat")

    def _light_handler(self, addr, count):
        return self._print_addr(addr, "light")

    def _vtx_handler(self, addr, count):
        return self._print_addr(addr, "vtx")

    def _vp_handler(self, addr):
        return self._print_addr(addr, "viewport")

    def _disassemble_chunk(self, data: bytes) -> str:
        gfxd_input_buffer(data)
        outb = bytes([0] * len(data) * 120)
        outbuf = gfxd_output_buffer(outb, len(outb))

        gfxd_target(gfxd_f3dex2)
        gfxd_endian(GfxdEndian.big, 4)
        gfxd_macro_fn(self._macro_fn)

        gfxd_tlut_callback(self._tlut_handler)
        gfxd_timg_callback(self._timg_handler)
        gfxd_cimg_callback(lambda addr, fmt, size, width: self._print_addr(addr, "color_image"))
        gfxd_zimg_callback(lambda addr: self._print_addr(addr, "depth_image"))
        gfxd_dl_callback(self._dl_handler)
        gfxd_mtx_callback(self._mtx_handler)
        gfxd_lookat_callback(self._lookat_handler)
        gfxd_light_callback(self._light_handler)
        gfxd_vtx_callback(self._vtx_handler)
        gfxd_vp_callback(self._vp_handler)
        gfxd_execute()
        return gfxd_buffer_to_string(outbuf)

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
