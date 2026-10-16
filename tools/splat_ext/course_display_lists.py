"""Course display lists use the same relocatable F3DEX2 bundles as models."""
from pathlib import Path

from splat.util import options

from tools.splat_ext.gfxbundle import N64SegGfxbundle


class N64SegCourse_display_lists(N64SegGfxbundle):
    @property
    def statistics_type(self):
        return "course_display_lists"

    def out_path(self) -> Path:
        return options.opts.asset_path / "courses" / "display_lists" / f"{self.name}.c"
