from __future__ import annotations

from pathlib import Path
from typing import Optional

from splat.segtypes.common.segment import CommonSegment
from splat.util import log, options

from tools.course_assets_common import write_yaml
from tools.sno import decompress_sno_with_consumed
from tools.tiled_background_common import (
    palette_colors,
    palette_size,
    parse_header,
    read_palette,
    read_tile_entries,
    read_tile_indices,
    tile_byte_size,
    texture_format,
    write_palette,
    write_preview,
    write_tile_png,
)


class N64SegTiled_background(CommonSegment):
    @staticmethod
    def is_data() -> bool:
        return True

    @property
    def statistics_type(self):
        return "tiled_background"

    def get_linker_section(self) -> str:
        return ".data"

    def get_section_flags(self) -> Optional[str]:
        return "wa"

    def out_path(self) -> Path:
        return options.opts.asset_path / "tiled_backgrounds" / f"{self.name}.yaml"

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
            log.error(f"tiled background segment {self.name} needs decompressed_size")
        assert isinstance(self.rom_start, int)
        assert isinstance(self.rom_end, int)
        assert decompressed_size is not None

        compressed = rom_bytes[self.rom_start : self.rom_end]
        data, consumed = decompress_sno_with_consumed(compressed, decompressed_size)
        unused_tail = compressed[consumed:]
        header = parse_header(data)

        ci_mode = header["ci_mode"]
        tile_width = header["tile_width"]
        tile_height = header["tile_height"]
        tile_index_offset = header["tile_index_data_offset"]
        palette_offset = header["palette_data_offset"]
        texture_offset = header["texture_data_offset"]

        if ci_mode not in (0, 1):
            log.error(f"tiled background segment {self.name} has unsupported ci_mode {ci_mode}")
        if not (0x10 <= tile_index_offset <= palette_offset <= texture_offset <= len(data)):
            log.error(f"tiled background segment {self.name} has invalid offsets")

        tile_size = tile_byte_size(tile_width, tile_height, ci_mode)
        pal_size = palette_size(ci_mode)
        palette_count = (texture_offset - palette_offset) // pal_size
        texture_count = (len(data) - texture_offset) // tile_size
        if palette_offset + palette_count * pal_size != texture_offset:
            log.error(f"tiled background segment {self.name} has partial palette data")
        if texture_offset + texture_count * tile_size != len(data):
            log.error(f"tiled background segment {self.name} has partial tile data")

        out_path = self.out_path()
        asset_dir = out_path.parent / self.name
        (asset_dir / "palettes").mkdir(parents=True, exist_ok=True)
        (asset_dir / "tiles").mkdir(parents=True, exist_ok=True)

        palettes = []
        for index in range(palette_count):
            start = palette_offset + index * pal_size
            rel_path = f"{self.name}/palettes/palette_{index:03d}.rgba16.s"
            write_palette(out_path.parent / rel_path, data[start : start + pal_size], start)
            palettes.append(
                {
                    "index": index,
                    "offset": f"0x{start:X}",
                    "path": rel_path,
                    "colors": palette_colors(ci_mode),
                    "format": "rgba16",
                }
            )

        palette_values = {
            palette["index"]: read_palette(out_path.parent / palette["path"]) for palette in palettes
        }
        tile_entries = read_tile_entries(data, 0x10, tile_index_offset)
        tile_palette_indices = {index: 0 for index in range(1, texture_count + 1)}
        for entry in tile_entries:
            texture_index = entry["texture_index"]
            if texture_index != 0 and texture_index not in tile_palette_indices:
                continue
            if texture_index != 0:
                tile_palette_indices[texture_index] = entry["palette_index"]

        tiles = []
        for index in range(1, texture_count + 1):
            start = texture_offset + (index - 1) * tile_size
            rel_path = f"{self.name}/tiles/tile_{index:03d}.png"
            palette_index = tile_palette_indices[index]
            write_tile_png(
                out_path.parent / rel_path,
                data[start : start + tile_size],
                tile_width,
                tile_height,
                ci_mode,
                palette_values[palette_index],
            )
            tiles.append(
                {
                    "index": index,
                    "offset": f"0x{start:X}",
                    "path": rel_path,
                    "width": tile_width,
                    "height": tile_height,
                    "format": texture_format(ci_mode),
                    "palette": palette_index,
                }
            )

        manifest = {
            "name": self.name,
            "format": "tiled_background",
            "compression": "sno",
            "compressed_size": f"0x{len(compressed):X}",
            "decompressed_size": f"0x{decompressed_size:X}",
            "tile_grid_width": header["tile_grid_width"],
            "tile_grid_height": header["tile_grid_height"],
            "tile_width": tile_width,
            "tile_height": tile_height,
            "ci_mode": ci_mode,
            "unknown_08": header["unknown_08"],
            "tile_index_data_offset": f"0x{tile_index_offset:X}",
            "palette_data_offset": f"0x{palette_offset:X}",
            "texture_data_offset": f"0x{texture_offset:X}",
            "palettes": palettes,
            "tiles": tiles,
            "tile_entries": tile_entries,
            "tile_indices": read_tile_indices(data, tile_index_offset, palette_offset),
            "preview": f"{self.name}/preview.png",
        }
        if unused_tail:
            manifest["unused_sno_tail"] = unused_tail.hex()

        write_yaml(out_path, manifest)
        write_preview(out_path, manifest)
        self.log(f"Wrote {self.name} to {out_path}")
