from __future__ import annotations

from pathlib import Path
from typing import Optional

from splat.segtypes.common.segment import CommonSegment
from splat.util import log, options

from tools.modelpayload_common import (
    collect_texture_ranges,
    collect_vtx_ranges,
    complement_ranges,
    non_overlapping_ranges,
    parse_gfx_words,
    read_palette_s,
    write_palette_s,
    write_texture_png,
    write_vtx_s,
)
from tools.sno import decompress_sno_with_consumed


class N64SegModelpayload(CommonSegment):
    @staticmethod
    def is_data() -> bool:
        return True

    @property
    def statistics_type(self):
        return "modelpayload"

    def get_linker_section(self) -> str:
        return ".data"

    def get_section_flags(self) -> Optional[str]:
        return "wa"

    def out_path(self) -> Path:
        return options.opts.asset_path / "modelpayload" / self.dir / f"{self.name}.yaml"

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

    def _yaml_str(self, key: str) -> Optional[str]:
        if not isinstance(self.yaml, dict) or key not in self.yaml:
            return None
        return str(self.yaml[key])

    def _display_list_path(self) -> Optional[Path]:
        display_list = self._yaml_str("display_list") or self._yaml_str("pair_segment")
        if not display_list:
            return None
        path = options.opts.asset_path / "gfxbin" / f"{display_list}.s"
        return path if path.exists() else None

    def _write_manifest(self, path: Path, parts: list[dict], unused_sno_tail: bytes) -> None:
        lines = [
            f"name: {self.name}",
            f"compressed_size: 0x{self.size:X}",
            f"decompressed_size: 0x{self._yaml_int('decompressed_size'):X}",
            "compression: sno",
        ]

        if unused_sno_tail and any(unused_sno_tail):
            lines.append(f"unused_sno_tail: {unused_sno_tail.hex()}")

        display_list = self._yaml_str("display_list") or self._yaml_str("pair_segment")
        if display_list:
            lines.append(f"display_list: {display_list}")

        lines.append("parts:")
        for part in parts:
            lines.append(f"  - name: {part['name']}")
            lines.append(f"    type: {part['type']}")
            lines.append(f"    offset: 0x{part['offset']:X}")
            lines.append(f"    size: 0x{part['size']:X}")
            lines.append(f"    path: {part['path']}")
            for key in ("format", "width", "height", "palette", "colors"):
                if key in part and part[key] is not None:
                    value = part[key]
                    lines.append(f"    {key}: {value}")

        path.write_text("\n".join(lines) + "\n", encoding="utf-8", newline="\n")

    def split(self, rom_bytes: bytes):
        if self.rom_end is None:
            log.error(
                f"segment {self.name} needs to know where it ends; add a position marker after it"
            )
        if self.size is None or self.size <= 0:
            log.error(f"Segment {self.name} has zero size.")

        decompressed_size = self._yaml_int("decompressed_size")
        if decompressed_size is None:
            log.error(f"modelpayload segment {self.name} needs decompressed_size")

        assert isinstance(self.rom_start, int)
        assert isinstance(self.rom_end, int)
        assert decompressed_size is not None

        compressed = rom_bytes[self.rom_start : self.rom_end]
        decompressed, consumed_size = decompress_sno_with_consumed(compressed, decompressed_size)
        unused_sno_tail = compressed[consumed_size:]

        manifest_path = self.out_path()
        root = manifest_path.parent / self.name
        root.mkdir(parents=True, exist_ok=True)
        (root / "vtx").mkdir(exist_ok=True)
        (root / "palettes").mkdir(exist_ok=True)
        (root / "textures").mkdir(exist_ok=True)

        words: list[tuple[int, int]] = []
        display_list_path = self._display_list_path()
        if display_list_path is not None:
            words = parse_gfx_words(display_list_path)

        vtx_ranges = collect_vtx_ranges(words, decompressed_size)
        texture_ranges = non_overlapping_ranges(collect_texture_ranges(words, decompressed_size))

        parts: list[dict] = []
        covered: list[tuple[int, int]] = []
        palette_paths: dict[str, Path] = {}

        for start, end in vtx_ranges:
            name = f"vtx_{start:04X}"
            rel_path = f"{self.name}/vtx/{name}.vtx.s"
            write_vtx_s(manifest_path.parent / rel_path, decompressed[start:end], start)
            parts.append(
                {
                    "name": name,
                    "type": "vtx",
                    "offset": start,
                    "size": end - start,
                    "path": rel_path,
                }
            )
            covered.append((start, end))

        for item in texture_ranges:
            if item.kind != "palette":
                continue
            rel_path = f"{self.name}/palettes/{item.name}.rgba16.s"
            out_path = manifest_path.parent / rel_path
            write_palette_s(out_path, decompressed[item.start : item.end], item.start)
            palette_paths[item.name] = out_path
            parts.append(
                {
                    "name": item.name,
                    "type": "palette",
                    "offset": item.start,
                    "size": item.size,
                    "path": rel_path,
                    "format": item.meta["format"],
                    "colors": item.meta["colors"],
                }
            )
            covered.append((item.start, item.end))

        for item in texture_ranges:
            if item.kind != "texture":
                continue

            palette_values = None
            palette_name = item.meta.get("palette")
            if palette_name:
                palette_path = palette_paths.get(palette_name)
                if palette_path is None:
                    continue
                palette_values = read_palette_s(palette_path)
            elif item.meta["format"].startswith("ci"):
                continue

            rel_path = f"{self.name}/textures/{item.name}.png"
            write_texture_png(
                manifest_path.parent / rel_path,
                decompressed[item.start : item.end],
                item.meta,
                palette_values,
            )
            parts.append(
                {
                    "name": item.name,
                    "type": "texture",
                    "offset": item.start,
                    "size": item.size,
                    "path": rel_path,
                    "format": item.meta["format"],
                    "width": item.meta["width"],
                    "height": item.meta["height"],
                    "palette": palette_name,
                }
            )
            covered.append((item.start, item.end))

        uncovered_ranges = complement_ranges(decompressed_size, covered)
        if uncovered_ranges:
            formatted_ranges = ", ".join(
                f"0x{start:X}-0x{end:X}" for start, end in uncovered_ranges
            )
            log.error(f"modelpayload segment {self.name} has unclassified ranges: {formatted_ranges}")

        parts.sort(key=lambda item: item["offset"])
        self._write_manifest(manifest_path, parts, unused_sno_tail)
        self.log(f"Wrote {self.name} to {manifest_path}")
