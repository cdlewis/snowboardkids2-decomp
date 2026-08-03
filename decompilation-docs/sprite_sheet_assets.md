# Editable Sprite Sheet Assets

The `sprite_sheet` segment type extracts SNO-compressed `SpriteSheetData` assets into indexed CI4 PNG files and shared
RGBA16 palettes. The generated YAML manifest preserves the runtime frame table, texture offsets, palette indices, and
unused compressed tail needed for byte-identical rebuilding.

`menuUiSprites` is currently the only migrated sprite sheet. Its sources are under
`assets/sprite_sheets/menuUiSprites/`; the board-rating stars have descriptive filenames for each mood and resolution.

To rebuild a sheet directly:

```sh
python3 tools/sprite_sheet_pack.py assets/sprite_sheets/menuUiSprites.yaml \
    --out /tmp/menuUiSprites.sno
```

PNG dimensions must match the manifest, and every pixel color must exist in the referenced palette. Run
`./tools/build-and-verify.sh` after editing a sprite or palette.
