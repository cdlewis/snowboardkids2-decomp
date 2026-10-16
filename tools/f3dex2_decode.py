"""Decode F3DEX2 commands with aggregate macros and caller-supplied symbols."""

from pygfxd import (
    GfxdCap,
    GfxdEndian,
    gfxd_buffer_to_string,
    gfxd_cimg_callback,
    gfxd_dl_callback,
    gfxd_disable,
    gfxd_enable,
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


class F3dex2Decoder:
    def __init__(self, address):
        self.address = address

    def _macro_fn(self):
        gfxd_puts("    ")
        gfxd_macro_dflt()
        gfxd_puts(",\n")
        return 0

    def _print_addr(self, addr: int, kind: str) -> int:
        # Exceptions cannot propagate through pygfxd's ctypes callbacks.
        # Record the failure and report it after execution returns to Python.
        try:
            symbol = self.address(addr, kind)
        except Exception as error:
            self.address_error = error
            symbol = "0"
        gfxd_printf(symbol)
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

    def decode(self, data: bytes) -> str:
        self.address_error = None
        gfxd_input_buffer(data)
        outb = bytes([0] * len(data) * 120)
        outbuf = gfxd_output_buffer(outb, len(outb))

        gfxd_enable(GfxdCap.stop_on_invalid)
        gfxd_disable(GfxdCap.stop_on_end)
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
        if gfxd_execute() != 0:
            raise ValueError("invalid F3DEX2 command stream")
        if self.address_error is not None:
            raise self.address_error
        return gfxd_buffer_to_string(outbuf)

