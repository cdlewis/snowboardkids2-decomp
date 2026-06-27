from __future__ import annotations

import struct
from pathlib import Path
from typing import Optional

from splat.segtypes.common.segment import CommonSegment
from splat.util import log, options

from tools.course_assets_common import write_yaml
from tools.sno import decompress_sno_with_consumed


class N64SegTrack_sector_mesh(CommonSegment):
    @staticmethod
    def is_data() -> bool:
        return True

    @property
    def statistics_type(self):
        return "track_sector_mesh"

    def get_linker_section(self) -> str:
        return ".data"

    def get_section_flags(self) -> Optional[str]:
        return "wa"

    def out_path(self) -> Path:
        return options.opts.asset_path / "courses" / "track_sector_meshes" / f"{self.name}.yaml"

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

    def _course_id(self) -> str:
        if not isinstance(self.yaml, dict) or "course_id" not in self.yaml:
            log.error(f"track sector mesh segment {self.name} needs course_id")
        assert isinstance(self.yaml, dict)
        return str(self.yaml["course_id"])

    def split(self, rom_bytes: bytes):
        if self.rom_end is None:
            log.error(f"segment {self.name} needs to know where it ends")
        decompressed_size = self._yaml_int("decompressed_size")
        if decompressed_size is None:
            log.error(f"track sector mesh segment {self.name} needs decompressed_size")
        assert isinstance(self.rom_start, int)
        assert isinstance(self.rom_end, int)
        assert decompressed_size is not None

        compressed = rom_bytes[self.rom_start : self.rom_end]
        data, consumed = decompress_sno_with_consumed(compressed, decompressed_size)
        unused_tail = compressed[consumed:]

        offset = 0
        vertex_count = int.from_bytes(data[offset : offset + 2], "big")
        offset += 2
        vertices = []
        for _ in range(vertex_count):
            x, y, z = struct.unpack(">hhh", data[offset : offset + 6])
            vertices.append({"x": x, "y": y, "z": z})
            offset += 6

        face_count = int.from_bytes(data[offset : offset + 2], "big")
        offset += 2
        faces = []
        for _ in range(face_count):
            v0, v1, v2, flags, surface_index = struct.unpack(">HHHBB", data[offset : offset + 8])
            faces.append({"v0": v0, "v1": v1, "v2": v2, "flags": flags, "surface_index": surface_index})
            offset += 8

        final_value = int.from_bytes(data[offset : offset + 2], "big")
        offset += 2
        if (len(data) - offset) % 0x24 != 0:
            log.error(f"track sector mesh segment {self.name} has trailing size that is not a sector multiple")

        sectors = []
        while offset < len(data):
            chunk = data[offset : offset + 0x24]
            neighbor0, neighbor1, neighbor2, neighbor3 = struct.unpack(">hhhh", chunk[0:8])
            base_face, face_count_2, base_height_face, height_face_count = struct.unpack(">HHHH", chunk[0x0C:0x14])
            sectors.append(
                {
                    "neighbor0": neighbor0,
                    "neighbor1": neighbor1,
                    "neighbor2": neighbor2,
                    "neighbor3": neighbor3,
                    "unknown_08": chunk[0x08:0x0C].hex(),
                    "base_face": base_face,
                    "face_count": face_count_2,
                    "base_height_face": base_height_face,
                    "height_face_count": height_face_count,
                    "vertex0": int.from_bytes(chunk[0x14:0x16], "big"),
                    "unknown_16": chunk[0x16:0x18].hex(),
                    "vertex1": int.from_bytes(chunk[0x18:0x1A], "big"),
                    "vertex2": int.from_bytes(chunk[0x1A:0x1C], "big"),
                    "unknown_1c": chunk[0x1C:0x1E].hex(),
                    "vertex3": int.from_bytes(chunk[0x1E:0x20], "big"),
                    "unknown_20": chunk[0x20:0x24].hex(),
                }
            )
            offset += 0x24

        manifest = {
            "name": self.name,
            "format": "track_sector_mesh",
            "compression": "sno",
            "course_id": self._course_id(),
            "vertices": vertices,
            "faces": faces,
            "final_value": final_value,
            "sectors": sectors,
        }
        if unused_tail:
            manifest["unused_sno_tail"] = unused_tail.hex()
        write_yaml(self.out_path(), manifest)
        self.log(f"Wrote {self.name} to {self.out_path()}")
