"""Extract editable F3DEX2 bundles with relocatable display-list entry points."""
import struct
from pathlib import Path
from typing import Optional

from splat.segtypes.common.segment import CommonSegment
from splat.util import log, options

from tools.f3dex2_decode import F3dex2Decoder


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
        return options.opts.asset_path / "gfxbin" / self.dir / f"{self.name}.c"

    def should_split(self) -> bool:
        return self.extract and any(options.opts.is_mode_active(mode) for mode in (self.type, "bin", "all"))

    def _address(self, addr: int, kind: str) -> str:
        segment, offset = addr >> 24, addr & 0xFFFFFF
        if segment == 1:
            if offset not in self._entries:
                raise ValueError(f"{self.name}: reference to missing entry 0x{offset:X}")
            return self._entries[offset]
        if segment == 2:
            # Resolved to named payload parts during extraction.
            return f"{self.name}_payload_{kind}_{offset:04X}"
        if segment == 3:
            # The runtime-selected costume texture manifests share this namespace.
            return f"COSTUME_TEXTURE_{kind}_{offset:04X}"
        raise ValueError(f"{self.name}: unsupported {kind} address {addr:#x}")

    def split(self, rom_bytes: bytes):
        assert isinstance(self.rom_start, int)
        assert isinstance(self.rom_end, int)
        data = rom_bytes[self.rom_start:self.rom_end]
        if not data or len(data) % 8:
            log.error(f"{self.name}: invalid F3DEX2 bundle size")
        words = [(int.from_bytes(data[i:i+4], "big"), int.from_bytes(data[i+4:i+8], "big"))
                 for i in range(0, len(data), 8)]
        boundaries = {0, len(data)}
        vertices = set()
        for i, (w0, w1) in enumerate(words):
            opcode = w0 >> 24
            if opcode == 0xDF:
                boundaries.add((i + 1) * 8)
            if opcode in (0x01, 0xDE, 0xE1) and w1 >> 24 == 1:
                offset = w1 & 0xFFFFFF
                boundaries.add(offset)
                if opcode == 1:
                    count = (w0 >> 12) & 0xFF
                    vertices.add((offset, count))
                    boundaries.add(offset + count * 16)
        self._entries = {off: f"{self.name}_part_{off:04X}" for off in boundaries if off < len(data)}
        for off, _ in vertices:
            self._entries[off] = f"{self.name}_vertices_{off:04X}"
        vertex_counts = dict(vertices)
        lines = [f"/* Extracted from ROM range 0x{self.rom_start:X}-0x{self.rom_end:X}. */",
                 '#include "common.h"', '#include "gbi.h"',
                 '#include "generated/model_gfx.h"', '']
        decoder = F3dex2Decoder(self._address)
        ordered = sorted(boundaries)
        for start, end in zip(ordered, ordered[1:]):
            name = self._entries[start]
            if start in vertex_counts:
                if end - start != vertex_counts[start] * 16:
                    raise ValueError(f"{self.name}: overlapping embedded vertex ranges")
                lines.append(f"Vtx {name}[] __attribute__((aligned(8))) = {{")
                for pos in range(start, end, 16):
                    x, y, z, flag, s, t, r, g, b, a = struct.unpack('>hhhHhhBBBB', data[pos:pos+16])
                    lines.append("    {{{%d, %d, %d}, %d, {%d, %d}, {%d, %d, %d, %d}}}," % (x, y, z, flag, s, t, r, g, b, a))
            else:
                lines.append(f"Gfx {name}[] __attribute__((aligned(8))) = {{")
                decoded = decoder.decode(data[start:end])
                if 'gsDPWord' in decoded or 'gsDPNoOpTag' in decoded:
                    raise ValueError(f"{self.name}: undecoded command at {start:#x}")
                lines.extend(decoded.rstrip().splitlines())
            lines.extend(['};', ''])
        path = self.out_path()
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text('\n'.join(lines), encoding='utf-8')
        self.log(f"Wrote {self.name} to {path}")
