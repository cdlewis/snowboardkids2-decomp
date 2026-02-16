---
name: decompile-f3dex
description: Use this skill if you need to decompile a function using F3Dex2. The presence of the Gfx type or gRegionAllocPtr are strongly indicative of an F3Dex2 function.
---

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

The mapping may not always be this clear, however. You can consult @f3dex-reference.md for a full list of f3dex commands and gbi.h for suitable macros to use.

## Aggregate Macros

Be aware that some macros in `gbi.h` aggregate a number of regular F3Dex2 commands. You should always look for patterns of commands that could translate into one of these macros. The use of which is very likely to increase your match percentage. Common examples of this include:

* gDPLoadTLUT (and its variations gDPLoadTLUT*)
* gDPLoadTextureBlock (and its variations gDPLoadTextureBlock*)

But there are many others.