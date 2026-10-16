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
            if kind == "vtx":
                for start, end in self._vertex_blocks:
                    if start <= offset < end and (offset - start) % 16 == 0:
                        name = f"{self.name}_vertices_{start:04X}"
                        index = (offset - start) // 16
                        return f"&{name}[{index}]"
                raise ValueError(f"{self.name}: vertex reference outside embedded vertex data")
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
        # Discover embedded vertices before looking for command boundaries.
        # Vertex coordinates/colors can have the same bytes as G_ENDDL or G_DL.
        loads = []
        for i, (w0, w1) in enumerate(words):
            if w0 >> 24 == 1 and w1 >> 24 == 1:
                start = w1 & 0xFFFFFF
                count = (w0 >> 12) & 0xFF
                loads.append((i * 8, start, start + count * 16))
        possible_blocks = [(start, end) for _, start, end in loads
                           if start < end <= len(data) and start % 8 == 0]
        loads = [(pos, start, end) for pos, start, end in loads
                 if not any(a <= pos < b for a, b in possible_blocks)]
        self._vertex_blocks = []
        for _, start, end in sorted(loads, key=lambda item: item[1]):
            if not start < end <= len(data) or start % 8:
                raise ValueError(f"{self.name}: invalid embedded vertex range {start:#x}-{end:#x}")
            if self._vertex_blocks and start < self._vertex_blocks[-1][1]:
                a, b = self._vertex_blocks[-1]
                if (start - a) % 16:
                    raise ValueError(f"{self.name}: misaligned overlapping embedded vertices")
                self._vertex_blocks[-1] = (a, max(b, end))
            else:
                self._vertex_blocks.append((start, end))
        boundaries = {0, len(data)}
        for start, end in self._vertex_blocks:
            boundaries.update((start, end))
        for i, (w0, w1) in enumerate(words):
            if any(start <= i * 8 < end for start, end in self._vertex_blocks):
                continue
            opcode = w0 >> 24
            if opcode == 0xDF:
                boundaries.add((i + 1) * 8)
            if opcode in (0xDE, 0xE1) and w1 >> 24 == 1:
                offset = w1 & 0xFFFFFF
                if offset >= len(data) or offset % 8:
                    raise ValueError(f"{self.name}: invalid display-list target {offset:#x}")
                if any(start <= offset < end for start, end in self._vertex_blocks):
                    raise ValueError(f"{self.name}: display-list target points into vertex data")
                boundaries.add(offset)
        self._entries = {off: f"{self.name}_part_{off:04X}" for off in boundaries if off < len(data)}
        vertex_counts = {}
        for start, end in self._vertex_blocks:
            self._entries[start] = f"{self.name}_vertices_{start:04X}"
            vertex_counts[start] = (end - start) // 16
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
