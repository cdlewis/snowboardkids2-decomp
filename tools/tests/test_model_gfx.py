import copy
import os
import shutil
import struct
import subprocess
import tempfile
import unittest
from pathlib import Path

import yaml

from tools.f3dex2_decode import F3dex2Decoder
from tools.model_gfx_symbols import generate, normalized_manifest, resolve_payload_references
from tools.splat_ext.gfxbundle import N64SegGfxbundle
from tools.splat_ext.course_display_lists import N64SegCourse_display_lists


ROOT = Path(__file__).resolve().parents[2]


class ModelGfxTests(unittest.TestCase):
    def test_named_payload_reference_and_validation(self):
        manifest = {"name": "MODEL", "parts": [
            {"name": "vertices", "type": "vtx", "offset": 0x20, "size": 0x40},
        ]}
        self.assertEqual(
            resolve_payload_references("BUNDLE_payload_vtx_0030", "BUNDLE", manifest),
            "&MODEL_vertices[1]",
        )
        for offset in (0x10, 0x21, 0x60):
            with self.assertRaises(ValueError):
                resolve_payload_references(f"BUNDLE_payload_vtx_{offset:04X}", "BUNDLE", manifest)

    def test_display_list_and_payload_relocations_follow_edits(self):
        self.check_relocations(N64SegGfxbundle, "gfxbin", "modelpayload")

    def test_course_relocations_follow_edits(self):
        self.check_relocations(N64SegCourse_display_lists, "courses/display_lists", "courses/model_resources")

    def check_relocations(self, segment_class, graphics_directory, payload_directory):
        """Move an entry point and a vertex part, then inspect the linked bytes."""
        cross = next((p for p in ("mips-linux-gnu-", "mips64-linux-gnu-", "mips64-elf-")
                      if shutil.which(p + "ld")), None)
        if cross is None:
            self.skipTest("MIPS toolchain required")
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            assets = root / "assets"
            gfx = assets / graphics_directory / "BUNDLE.c"
            gfx.parent.mkdir(parents=True)
            payload = assets / payload_directory / "MODEL.yaml"
            payload.parent.mkdir(parents=True)
            header = root / "include/generated/model_gfx.h"
            linker = root / "payload.ld"

            bundle = object.__new__(segment_class)
            bundle.name = "BUNDLE"
            bundle.rom_start = 0
            bundle.rom_end = 32
            bundle.out_path = lambda: gfx
            bundle.log = lambda message: None
            bundle.split(struct.pack(">8I", 0x01001002, 0x02000020,
                                     0xDE000000, 0x01000018, 0xDF000000, 0,
                                     0xDF000000, 0))
            manifest = {"name": "MODEL", "parts": [
                {"name": "vertices", "type": "vtx", "offset": 0x20, "size": 16},
            ]}
            gfx.write_text(resolve_payload_references(gfx.read_text(), "BUNDLE", manifest))
            original = gfx.read_text()
            descriptor = root / "descriptor.c"
            descriptor.write_text('#include "common.h"\n#include "generated/model_gfx.h"\n'
                                  'Gfx *descriptor = BUNDLE_part_0018;\n')
            script = root / "layout.ld"
            script.write_text(f'INCLUDE {linker}\nSECTIONS {{\n'
                              '.bundle 0x01000000 : { bundle.o(.data) }\n'
                              '.descriptor 0x80000000 : { descriptor.o(.data) }\n'
                              '/DISCARD/ : { *(.text) *(.bss) *(.reginfo) *(.MIPS.abiflags) }\n}\n')
            env = dict(os.environ, COMPILER_PATH=str(ROOT / "tools/gcc_kmc"))
            compiler = str(ROOT / "tools/gcc_kmc/gcc")
            flags = ["-mabi=32", "-mno-abicalls", "-nostdinc", "-fno-PIC", "-G", "0",
                     "-Wa,-force-n64align", "-w", "-mips3", "-EB", "-D_LANGUAGE_C", "-DF3DEX_GBI_2"]
            for include in (ROOT / "include", ROOT / "lib/ultralib/include", ROOT / "lib/f3dex2/PR", root / "include"):
                flags += ["-I", str(include)]

            def run(*args):
                result = subprocess.run(args, cwd=ROOT if args[0] == compiler else root, env=env, capture_output=True, text=True)
                self.assertEqual(result.returncode, 0, result.stdout + result.stderr)

            for moved in (False, True):
                manifest["parts"][0]["offset"] = "0x40" if moved else "0x20"
                payload.write_text(yaml.safe_dump(manifest))
                gfx.write_text(original.replace("gsSPEndDisplayList()",
                                                "gsDPPipeSync(),\n    gsSPEndDisplayList()", 1)
                               if moved else original)
                generate(assets, header, linker)
                run(compiler, *flags, "-c", str(gfx), "-o", str(root / "bundle.o"))
                run(compiler, *flags, "-c", str(descriptor), "-o", str(root / "descriptor.o"))
                run(cross + "ld", "--no-check-sections", "-T", str(script),
                    "bundle.o", "descriptor.o", "-o", "linked.elf")
                run(cross + "objcopy", "-O", "binary", "-j", ".bundle", "linked.elf", "bundle.bin")
                run(cross + "objcopy", "-O", "binary", "-j", ".descriptor", "linked.elf", "descriptor.bin")
                commands = (root / "bundle.bin").read_bytes()
                address = 0x01000020 if moved else 0x01000018
                self.assertEqual(struct.unpack_from(">I", commands, 4)[0],
                                 0x02000040 if moved else 0x02000020)
                self.assertEqual(struct.unpack_from(">I", commands, 12)[0], address)
                self.assertEqual(struct.unpack_from(">I", (root / "descriptor.bin").read_bytes())[0], address)

    def extract_source(self, words):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "BUNDLE.c"
            data = struct.pack(">" + "I" * len(words), *words)
            bundle = object.__new__(N64SegGfxbundle)
            bundle.name = "BUNDLE"
            bundle.rom_start = 0
            bundle.rom_end = len(data)
            bundle.out_path = lambda: path
            bundle.log = lambda message: None
            bundle.split(data)
            return path.read_text()

    def test_embedded_vertices_are_data_even_when_they_look_like_commands(self):
        # The first coordinate word looks like G_ENDDL. A later load references
        # just the second vertex in the same block, rather than a new array.
        source = self.extract_source([
            0x01002004, 0x01000010, 0xDF000000, 0,
            0xDF000000, 0, 0, 0,
            0, 0, 0, 0,
            0x01001002, 0x01000020, 0xDF000000, 0,
        ])
        self.assertEqual(source.count("Vtx BUNDLE_vertices_0010[]"), 1)
        self.assertIn("gsSPVertex(&BUNDLE_vertices_0010[1], 1, 0)", source)
        self.assertIn("-8448", source)

    def test_display_list_cannot_jump_into_embedded_vertices(self):
        with self.assertRaisesRegex(ValueError, "points into vertex data"):
            self.extract_source([
                0x01001002, 0x01000018,
                0xDE000000, 0x01000018, 0xDF000000, 0,
                0, 0, 0, 0,
            ])

    def test_invalid_display_list_target_fails_extraction(self):
        for target in (0x01000004, 0x01000010):
            with self.subTest(target=target), self.assertRaisesRegex(ValueError, "invalid display-list target"):
                self.extract_source([0xDE000000, target, 0xDF000000, 0])

    def test_invalid_payload_layouts_are_rejected(self):
        original = {"name": "MODEL", "decompressed_size": 0x40, "parts": [
            {"name": "vertices", "type": "vtx", "offset": 0, "size": 0x40},
        ]}
        cases = [
            {"offset": -16}, {"offset": 16}, {"size": 0}, {"size": 24},
            {"name": "bad-name"}, {"type": "unknown"},
        ]
        for changes in cases:
            manifest = copy.deepcopy(original)
            manifest["parts"][0].update(changes)
            with self.subTest(changes=changes), self.assertRaises(ValueError):
                normalized_manifest(manifest)
        for segment in (-1, 16):
            with self.subTest(segment=segment), self.assertRaises(ValueError):
                normalized_manifest(dict(original, rsp_segment=segment))
        manifest = copy.deepcopy(original)
        manifest["parts"].append(dict(manifest["parts"][0]))
        with self.assertRaisesRegex(ValueError, "duplicate part"):
            normalized_manifest(manifest)
        manifest["parts"][1]["name"] = "overlap"
        with self.assertRaisesRegex(ValueError, "overlaps"):
            normalized_manifest(manifest)

    def test_shared_namespace_checks_format_and_segment_but_not_part_order(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            folder = root / "assets/modelpayload"
            folder.mkdir(parents=True)
            original = {"name": "FIRST", "rsp_segment": 3, "decompressed_size": 64,
                        "symbol_namespace": "SELECTED", "parts": [
                {"name": "image", "type": "texture", "offset": 0, "size": 32,
                 "format": "ci4", "width": 8, "height": 8, "palette": "palette"},
                {"name": "palette", "type": "palette", "offset": 32, "size": 32},
            ]}
            (folder / "first.yaml").write_text(yaml.safe_dump(original))
            second = copy.deepcopy(original)
            second["name"] = "SECOND"
            second["decompressed_size"] = "0x40"
            second["parts"].reverse()
            (folder / "second.yaml").write_text(yaml.safe_dump(second))
            generate(root / "assets", root / "symbols.h", root / "symbols.ld")
            self.assertEqual((root / "symbols.ld").read_text().count("SELECTED_image ="), 1)
            for changes in ({"rsp_segment": 2}, {"format": "rgba16"}):
                altered = copy.deepcopy(second)
                if "format" in changes:
                    altered["parts"][1]["format"] = changes["format"]
                else:
                    altered.update(changes)
                (folder / "second.yaml").write_text(yaml.safe_dump(altered))
                with self.subTest(changes=changes), self.assertRaisesRegex(ValueError, "layouts disagree"):
                    generate(root / "assets", root / "symbols.h", root / "symbols.ld")

    def test_distinct_payload_names_cannot_collide_after_joining(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            folder = root / "assets/modelpayload"
            folder.mkdir(parents=True)
            for namespace, part_name, offset in (("A", "B_C", 0), ("A_B", "C", 16)):
                manifest = {"name": namespace, "parts": [
                    {"name": part_name, "type": "vtx", "offset": offset, "size": 16},
                ]}
                (folder / f"{namespace}.yaml").write_text(yaml.safe_dump(manifest))
            with self.assertRaisesRegex(ValueError, "conflicting payload symbol"):
                generate(root / "assets", root / "symbols.h", root / "symbols.ld")

    def test_duplicate_graphics_definitions_are_rejected(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            gfx = root / "assets/gfxbin"
            gfx.mkdir(parents=True)
            (gfx / "one.c").write_text("Gfx same_name[2] = {};\n")
            (gfx / "two.c").write_text("Gfx same_name[] = {};\n")
            with self.assertRaisesRegex(ValueError, "duplicate graphics symbol"):
                generate(root / "assets", root / "symbols.h", root / "symbols.ld")

    def test_unresolved_address_fails_extraction(self):
        def unknown_address(address, kind):
            raise ValueError("unknown asset reference")
        decoder = F3dex2Decoder(unknown_address)
        with self.assertRaisesRegex(ValueError, "unknown asset reference"):
            decoder.decode(struct.pack(">4I", 0xDE000000, 0x04000000, 0xDF000000, 0))

    def test_dynamic_texture_manifests_must_share_layout(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            folder = root / "assets/modelpayload"
            folder.mkdir(parents=True)
            manifest = {"name": "PALETTE_00", "rsp_segment": 3, "symbol_namespace": "COSTUME_TEXTURE",
                        "parts": [{"name": "palette", "type": "palette", "offset": 0x800, "size": 32}]}
            (folder / "first.yaml").write_text(yaml.safe_dump(manifest))
            manifest["parts"][0]["offset"] = 0x900
            (folder / "second.yaml").write_text(yaml.safe_dump(manifest))
            with self.assertRaisesRegex(ValueError, "layouts disagree"):
                generate(root / "assets", root / "symbols.h", root / "symbols.ld")


if __name__ == "__main__":
    unittest.main()
