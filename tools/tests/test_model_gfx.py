import os
import shutil
import struct
import subprocess
import tempfile
import unittest
from pathlib import Path

import yaml

from tools.f3dex2_decode import F3dex2Decoder
from tools.model_gfx_symbols import generate, resolve_payload_references
from tools.splat_ext.gfxbundle import N64SegGfxbundle


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
        """Move an entry point and a vertex part, then inspect the linked bytes."""
        cross = next((p for p in ("mips-linux-gnu-", "mips64-linux-gnu-", "mips64-elf-")
                      if shutil.which(p + "ld")), None)
        if cross is None:
            self.skipTest("MIPS toolchain required")
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            assets = root / "assets"
            gfx = assets / "gfxbin" / "BUNDLE.c"
            gfx.parent.mkdir(parents=True)
            payload = assets / "modelpayload" / "MODEL.yaml"
            payload.parent.mkdir()
            header = root / "include/generated/model_gfx.h"
            linker = root / "payload.ld"

            bundle = object.__new__(N64SegGfxbundle)
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
                manifest["parts"][0]["offset"] = 0x40 if moved else 0x20
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
