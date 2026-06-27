# Course Asset Extractor Limitations

The current course asset extractor is designed to make the known Snowboard Kids 2
course data human-readable and rebuildable. It round-trips the current course
assets byte-for-byte, but it is not a complete semantic editor for every field in
those assets.

## Segment Types

### `course_display_lists`

These are F3DEX display-list command streams used as course segment 1.

Limitations:

- The only extracted display-list files are generated `Gfx[]` C source using
  `gbi.h` macros. This is still low-level display-list data, not a high-level
  model or scene editor.
- Segment 1 display-list targets and segment 2 resource addresses are generated
  as named segmented-address constants. They are rebuildable, but they are not C
  pointers or linker-managed labels inside the decompressed course buffers.
- Vertex address constants may point inside a larger editable vertex resource.
  Editing the vertex `.vtx.s` file can move or resize the containing block only
  if the corresponding manifest offsets and sizes are also kept consistent.
- The extractor disassembles each packed command stream independently. It does
  not validate display-list control flow, detect unreachable streams, or infer
  which stream represents which course object.
- The extractor assumes the original segment is already a valid sequence of
  8-byte display-list commands.

### `course_model_resources`

These are SNO-compressed segment 2 resource banks referenced by course display
lists. The current extractor identifies vertices, CI textures, and RGBA16
palettes from display-list references.

Limitations:

- Classification is driven by display-list references. Data that is not
  referenced by known display-list commands is not semantically decoded.
- The current implementation expects the entire decompressed payload to be
  classified. If a future course resource bank contains unrelated unreferenced
  data, this segment type will need a raw-block fallback.
- Texture support is limited to the formats handled by the shared texture helper
  code.
- The extractor does not infer higher-level model structure. It exposes resource
  pieces, not named course objects or scene graph nodes.

### `track_sector_mesh`

These are SNO-compressed track layout meshes consumed by `parseGameDataLayout`.
They contain track vertices, triangle faces, sector records, and a final config
value.

Limitations:

- Vertices, faces, sector neighbor links, face ranges, and height-face ranges
  are exposed, but several sector fields are still named `unknown_*`.
- The YAML can be edited structurally, but there is no high-level validation for
  gameplay correctness. Invalid face indices, sector ranges, or neighbor links
  can still build.
- The packer preserves the current binary layout. It does not automatically
  update unrelated game logic that may assume course-specific counts or ranges.

### `course_texture_table`

These are SNO-compressed `DataTable_19E80` texture tables used by course scenery
and effects. The extractor emits palettes and CI4 PNG textures.

Limitations:

- Unknown entry bytes are preserved as `unknown_0a` where needed, but their
  meaning is not understood.
- Non-texture gaps with meaningful bytes are preserved as `raw_blocks`, but they
  are not decoded. Turtle Island currently needs this for an unreferenced raw
  block.
- Shared texture offsets are represented as separate entries that may point at
  the same decompressed bytes. If duplicate PNGs for the same offset are edited
  inconsistently, the last written entry wins.
- PNG editing is limited to indexed texture data that can be represented through
  the current palette conversion helpers.

## Rebuild Constraints

- Course display lists, model resources, track sector meshes, and texture tables
  are associated by `course_id` in `snowboardkids2.yaml`. Model payloads and
  their display-list bundles are associated by `model_id`.
- SNO padding and unused tail bytes are preserved for byte-exact round trips.
  Edited data can recompress to a different size. If a rebuilt compressed asset
  grows beyond the original ROM allocation, the ROM layout can shift and checksum
  verification will fail.

## Validation Expectations

After extractor or course asset changes:

- Run `python3 -m splat split snowboardkids2.yaml` to regenerate editable assets.
- Run `./tools/build-and-verify.sh` to confirm the rebuilt ROM still matches.
- If checksum fails, first check whether rebuilt course compressed assets still
  match their original ROM slices before debugging unrelated code or data.
