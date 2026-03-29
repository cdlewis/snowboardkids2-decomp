---
name: decompile-microcode
description: Use this skill if you need to decompile a function using F3Dex2 or S2Dex. The presence of the Gfx type or gRegionAllocPtr are strongly indicative of an F3Dex2 or S2Dex microcode function.
---

# Determining which Microcode is used

gRegionAllocPtr is indicative of a dynamically constructed Fast3DEX2 or S2Dex display list commands. Display list commands are microcode that are passed to the RSP for rendering. They are all 8 bytes long.

Because Snowboard Kids 2 uses two different sets of microcode, we must first determine which it is. There are three key techniques for determining this:

1. Nature of the function: F3Dex2 is used for 3D objects whereas S2Dex is designed for 2D sprites. Choose the microcode that best reflects the purpose of the function.
2. Disassembly: check if the microcode can only be disassembled into valid F3Dex2 or S2Dex instructions.
3. Look directly at gbi.h and gs2dex.h for clues.

# Decompiling F3Dex2 Code

gRegionAllocPtr is indicative of a dynamically constructed Fast3DEX2 (also known as f3dex2) display list commands. Fast3DEX2 display list commands are microcode that are passed to the RSP for rendering. They are all 8 bytes long.

Special care needs to be taken with functions constructing display lists as the original developer likely used specific macros from `lib/f3dex2/PR/gbi.h`. These macro calls need to be replicated in order for the function to match.

The first step is to identify the commands in question, for example a naive decompilation might produce:

```
    temp_v1 = gRegionAllocPtr;
    temp_a0 = D_800A8B14_9FE84;
    temp_v1->words.w0 = 0xDA380003;
    temp_a1 = (s32)arg0->unk30;
    temp_v1->words.w1 = temp_a1;
```

But we can see here that there's really just one f3dex command. It starts with 0xDA380003 and uses temp_a1 as an argument. We can use `gfxdis.f3dex2` to help us identify what it is:

```
gfxdis.f3dex2 -d DA38000300000000
```

Which gives us:

```
gsSPMatrix(0x00000000, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

```

This is close to what we want. But it needs a few improvements:

1. We need the variant of this macro that also accepts a display list pointer: `gSPMatrix`.
2. We need to replace 0x00000000 with our actual variable: arg0->unk30.

This ultimately will give us:

```
gSPMatrix(gRegionAllocPtr++, (s32)arg0->unk30, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
```

Note the inline increment of gRegionAllocPtr, this is very common in commands dealing with f3dex2 microcode.

The mapping may not always be this clear, however. You can consult @f3dex2-reference.md for a full list of F3DEX2 commands and `gbi.h` for suitable macros to use.

## Aggregate Macros

Be aware that some macros in `gbi.h` aggregate a number of regular F3Dex2 commands. You should always look for patterns of commands that could translate into one of these macros. The use of which is very likely to increase your match percentage. Common examples of this include:

* gDPLoadTLUT (and its variations gDPLoadTLUT*)
* gDPLoadTextureBlock (and its variations gDPLoadTextureBlock*)

But there are many others.

# Decompiling S2Dex2 Code

S2DEX (Sprite 2D Extension) microcode enables Super NES-like sprite and background rendering on the N64. Unlike F3DEX2 which handles 3D geometry, S2DEX is used for 2D sprites, rectangles, and background planes. The macros come from `lib/ultralib/include/PR/gs2dex.h`.

S2DEX commands also use gRegionAllocPtr for dynamic display list construction. The identification process is the same — look for 8-byte command words being written through gRegionAllocPtr.

For example, a naive decompilation might produce:

```
    temp_v1 = gRegionAllocPtr;
    temp_v1->words.w0 = 0x02000000;
    temp_v1->words.w1 = (u32)&sprite;
```

This is an S2DEX command starting with 0x02. We can use `s2dex_dis.py` to identify it:

```
../../tools/s2dex_dis.py 0x0200000080123456
```

Which gives us:

```
gsSPObjSprite(0x80123456)
```

So the macro is `gSPObjSprite`. Adding the display list pointer and inline increment:

```
gSPObjSprite(gRegionAllocPtr++, &sprite);
```

Note the inline increment of gRegionAllocPtr — this is the same pattern used with F3DEX2 commands.

You can consult @s2dex-reference.md for a full list of S2DEX commands and `gs2dex.h` for suitable macros to use.

## S2DEX Data Structures

S2DEX commands operate on specific data structures defined in `gs2dex.h`. Understanding these is critical for matching:

* `uObjSprite` (24 bytes) — 2D sprite object with position, scale, texture info, and display flags
* `uObjBg` / `uObjScaleBg` (40 bytes) — background plane definitions (non-scalable and scalable variants)
* `uObjMtx` (24 bytes) — 2D affine transformation matrix (A, B, C, D, X, Y, BaseScaleX, BaseScaleY)
* `uObjSubMtx` (8 bytes) — partial matrix update (X, Y, BaseScaleX, BaseScaleY only)
* `uObjTxtr` (24 bytes) — texture loading descriptor (block, tile, or TLUT variants)
* `uObjTxSprite` (48 bytes) — combined texture load + sprite draw (uObjTxtr + uObjSprite)

When the assembly accesses fields of these structs, create or extend the appropriate struct definition rather than using pointer arithmetic.

## Compound Commands

Several S2DEX macros combine a texture load with a draw operation into a single compound command. These are very likely to appear in the original code when a texture load immediately precedes a draw:

* `gSPObjLoadTxSprite` — load texture + draw sprite (takes a `uObjTxSprite*`)
* `gSPObjLoadTxRect` — load texture + draw rectangle (takes a `uObjTxSprite*`)
* `gSPObjLoadTxRectR` — load texture + draw rotated rectangle (takes a `uObjTxSprite*`)

If you see a `gSPObjLoadTxtr` immediately followed by a `gSPObjSprite`, `gSPObjRectangle`, or `gSPObjRectangleR`, the original code likely used the compound form instead.