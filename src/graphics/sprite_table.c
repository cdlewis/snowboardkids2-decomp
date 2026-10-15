#include "graphics/sprite_table.h"

#include "assets.h"
#include "common.h"
#include "data/data_table.h"
#include "gbi.h"
#include "graphics/graphics.h"
#include "math/geometry.h"
#include "mbi.h"
#include "system/task_scheduler.h"

USE_ASSET(_6448D0);
USE_ASSET(_647930);
USE_ASSET(_648C60);
USE_ASSET(_64EFD0);

typedef struct {
    /* 0x00 */ u32 vertexData;
    /* 0x04 */ Vec3i position;
    /* 0x10 */ DataTable_19E80 *dataTable;
    /* 0x14 */ u16 textureIndex;
    /* 0x16 */ u8 alpha;
    /* 0x17 */ u8 flags;
    /* 0x18 */ Mtx *translationMtx;
    /* 0x1C */ Mtx *scaleMtx;
    /* 0x20 */ Mtx *yRotationMtx;
    /* 0x24 */ Mtx *zRotationMtx;
    /* 0x28 */ s32 scaleX;
    /* 0x2C */ s32 scaleY;
    /* 0x30 */ u16 zRotation;
    /* 0x32 */ s16 paletteIndex;
    /* 0x34 */ Transform3D transform;
} SpriteRenderState;

typedef void (*SetupAndEnqueueSprite_t)(SpriteAssetState *, s32, s32, s32, s32, s32, s32, s16, u8, u8, s32);

/*
 * This object starts with the final word of the preceding sprite DMA entry.
 * Each row then contains the ROM fields for the following entry.
 */
typedef struct {
    /* 0x00 */ s16 previousAssetId;
    /* 0x02 */ s16 unused;
    /* 0x04 */ void *nextRomStart;
    /* 0x08 */ void *nextRomEnd;
    /* 0x0C */ s32 nextDecompressedSize;
    /* 0x10 */ SpriteAnimationSet *nextAnimationSets;
} SpriteAssetTableLink;

extern SpriteAnimationSet D_8008C7F0_8D3F0;
extern SpriteAnimationSet D_8008C830_8D430;
extern SpriteAnimationSet D_8008C858_8D458;
extern SpriteAnimationSet D_8008C888_8D488;
extern SpriteAnimationSet D_8008C8B0_8D4B0;
extern SpriteAnimationSet D_8008C8E0_8D4E0;
extern SpriteAnimationSet D_8008C8F0_8D4F0;
extern SpriteAnimationSet D_8008C910_8D510;

extern s32 D_8008C920_8D520[];
extern s32 gLookAtPtr;
extern Gfx *gDisplayListAllocPtr;

static OutputStruct_19E80 gCachedSpriteTextureEntry;

void setupAndEnqueueSprite(
    SpriteAssetState *state,
    s32 slot,
    s32 posX,
    s32 posY,
    s32 posZ,
    s32 scaleX,
    s32 scaleY,
    s16 arg7,
    u8 flipH,
    u8 alpha,
    s16 arg10
);

SpriteAssetTableLink gSpriteAssetTable[] = {
    { 0x43, 0, &_64EFD0_ROM_START,                   &_64EFD0_ROM_END,                   0x1F8,  &D_8008C7F0_8D3F0 },
    { 1,    0, &_648C60_ROM_START,                   &_648C60_ROM_END,                   0x358,  &D_8008C830_8D430 },
    { 2,    0, &BOMB_SOUND_SEQUENCE_DATA_ROM_START,  &BOMB_SOUND_SEQUENCE_DATA_ROM_END,  0x358,  &D_8008C858_8D458 },
    { 1,    0, &_647930_ROM_START,                   &_647930_ROM_END,                   0x638,  &D_8008C888_8D488 },
    { 1,    0, &CAKE_SOUND_SEQUENCE_DATA_ROM_START,  &CAKE_SOUND_SEQUENCE_DATA_ROM_END,  0x468,  &D_8008C8B0_8D4B0 },
    { 1,    0, &BOMB2_SOUND_SEQUENCE_DATA_ROM_START, &BOMB2_SOUND_SEQUENCE_DATA_ROM_END, 0x358,  &D_8008C858_8D458 },
    { 1,    0, &GHOST_SOUND_SEQUENCE_DATA_ROM_START, &GHOST_SOUND_SEQUENCE_DATA_ROM_END, 0xA78,  &D_8008C8E0_8D4E0 },
    { 1,    0, &_6448D0_ROM_START,                   &_6448D0_ROM_END,                   0x35D8, &D_8008C8F0_8D4F0 },
    { 2,    0, &POST2_SOUND_SEQUENCE_DATA_ROM_START, &POST2_SOUND_SEQUENCE_DATA_ROM_END, 0x848,  &D_8008C910_8D510 },
};

s16 gSpriteAssetTablePadding[] = { 0x0002, 0x0000 };

s32 gSpriteVtx_8x8[] = {
    (s32)0xFFF00010, 0x00000000, 0x00000000, (s32)0xFFFFFFFF, 0x00100010,      0x00000000, 0x02000000, (s32)0xFFFFFFFF,
    0x0010FFF0,      0x00000000, 0x02000200, (s32)0xFFFFFFFF, (s32)0xFFF0FFF0, 0x00000000, 0x00000200, (s32)0xFFFFFFFF,
};

s32 gSpriteVtx_16x16[] = {
    (s32)0xFFE00020, 0x00000000, 0x00000000, (s32)0xFFFFFFFF, 0x00200020,      0x00000000, 0x04000000, (s32)0xFFFFFFFF,
    0x0020FFE0,      0x00000000, 0x04000400, (s32)0xFFFFFFFF, (s32)0xFFE0FFE0, 0x00000000, 0x00000400, (s32)0xFFFFFFFF,
};

s32 gSpriteVtx_16x32[] = {
    (s32)0xFFE00040, 0x00000000, 0x00000000, (s32)0xFFFFFFFF, 0x00200040,      0x00000000, 0x04000000, (s32)0xFFFFFFFF,
    0x0020FFC0,      0x00000000, 0x04000800, (s32)0xFFFFFFFF, (s32)0xFFE0FFC0, 0x00000000, 0x00000800, (s32)0xFFFFFFFF,
};

s32 gSpriteVtx_32x16[] = {
    (s32)0xFFC00020, 0x00000000, 0x00000000, (s32)0xFFFFFFFF, 0x00400020,      0x00000000, 0x08000000, (s32)0xFFFFFFFF,
    0x0040FFE0,      0x00000000, 0x08000400, (s32)0xFFFFFFFF, (s32)0xFFC0FFE0, 0x00000000, 0x00000400, (s32)0xFFFFFFFF,
};

s32 gSpriteVtx_32x32[] = {
    (s32)0xFFC00040, 0x00000000, 0x00000000, (s32)0xFFFFFFFF, 0x00400040,      0x00000000, 0x08000000, (s32)0xFFFFFFFF,
    0x0040FFC0,      0x00000000, 0x08000800, (s32)0xFFFFFFFF, (s32)0xFFC0FFC0, 0x00000000, 0x00000800, (s32)0xFFFFFFFF,
};

s32 gSpriteVtx_64x32[] = {
    (s32)0xFF800040, 0x00000000, 0x00000000, (s32)0xFFFFFFFF, 0x00800040,      0x00000000, 0x10000000, (s32)0xFFFFFFFF,
    0x0080FFC0,      0x00000000, 0x10000800, (s32)0xFFFFFFFF, (s32)0xFF80FFC0, 0x00000000, 0x00000800, (s32)0xFFFFFFFF,
};

s32 gSpriteVtx_32x64[] = {
    (s32)0xFFC00080, 0x00000000, 0x00000000, (s32)0xFFFFFFFF, 0x00400080,      0x00000000, 0x08000000, (s32)0xFFFFFFFF,
    0x0040FF80,      0x00000000, 0x08001000, (s32)0xFFFFFFFF, (s32)0xFFC0FF80, 0x00000000, 0x00001000, (s32)0xFFFFFFFF,
};

s32 gSpriteVtx_64x64[] = {
    (s32)0xFF800080, 0x00000000,      0x00000000, (s32)0xFFFFFFFF, 0x00800080, 0x00000000,
    0x10000000,      (s32)0xFFFFFFFF, 0x0080FF80, 0x00000000,      0x10001000, (s32)0xFFFFFFFF,
    (s32)0xFF80FF80, 0x00000000,      0x00001000, (s32)0xFFFFFFFF, 0x00000000, 0x00000000,
};

SpriteDmaEntry gSpriteDmaTable[] = {
    { &SPRITE_DMA_ASSET_00_ROM_START, &SPRITE_DMA_ASSET_00_ROM_END, 0x00000D80 },
    { &SPRITE_DMA_ASSET_01_ROM_START, &SPRITE_DMA_ASSET_01_ROM_END, 0x00000660 },
    { &SPRITE_DMA_ASSET_02_ROM_START, &SPRITE_DMA_ASSET_02_ROM_END, 0x000002A0 },
    { &SPRITE_DMA_ASSET_03_ROM_START, &SPRITE_DMA_ASSET_03_ROM_END, 0x00000CB0 },
    { &SPRITE_DMA_ASSET_04_ROM_START, &SPRITE_DMA_ASSET_04_ROM_END, 0x00001940 },
};

SpriteDmaEntry *gSpriteDmaTablePtr = gSpriteDmaTable;

s32 gSpriteDmaTableInfo[] = { 0x00000005, 0x00000001, 0x00000000, 0x00000000 };

Gfx gSpriteTextureSetupDL[] = {
    gsSPClearGeometryMode(
        G_ZBUFFER | G_SHADE | G_CULL_FRONT | G_CULL_BACK | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR |
        G_SHADING_SMOOTH
    ),
    gsSPSetGeometryMode(G_ZBUFFER | G_SHADE | G_FOG | G_SHADING_SMOOTH),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetCycleType(G_CYC_2CYCLE),
    gsDPSetCombineMode(G_CC_MODULATEIDECALA, G_CC_PASS2),
    gsDPSetRenderMode(
        AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_X_ALPHA | ALPHA_CVG_SEL |
            GBL_c1(G_BL_CLR_FOG, G_BL_A_SHADE, G_BL_CLR_IN, G_BL_1MA),
        AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_X_ALPHA | ALPHA_CVG_SEL |
            GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)
    ),
    gsDPSetTextureLUT(G_TT_RGBA16),
    gsSPEndDisplayList(),
};

Gfx gTranslucentSpriteTextureSetupDL[] = {
    gsSPClearGeometryMode(
        G_ZBUFFER | G_SHADE | G_CULL_FRONT | G_CULL_BACK | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR |
        G_SHADING_SMOOTH
    ),
    gsSPSetGeometryMode(G_ZBUFFER | G_SHADE | G_FOG | G_SHADING_SMOOTH),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetCycleType(G_CYC_2CYCLE),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, ENVIRONMENT, 0, 0, 0, 0, COMBINED, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(
        AA_EN | Z_CMP | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL |
            GBL_c1(G_BL_CLR_FOG, G_BL_A_SHADE, G_BL_CLR_IN, G_BL_1MA),
        AA_EN | Z_CMP | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL |
            GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)
    ),
    gsDPSetTextureLUT(G_TT_RGBA16),
    gsSPEndDisplayList(),
};

s32 getSpriteAssetCount(void) {
    return 10;
}

s16 getSpriteAssetId(s32 index) {
    return gSpriteAssetTable[index].previousAssetId;
}

void renderOpaqueSpriteCallback(SpriteRenderState *sprite) {
    OutputStruct_19E80 textureEntry;
    Transform3D transform;
    s32 texWidthShift;
    s32 texHeightShift;
    s32 tlutAddr;
    s32 dim;
    s32 scaleXRaw;
    s32 scaleYRaw;
    s16 scaleX;
    s16 scaleY;

    if (isObjectCulled(&sprite->position) != 0) {
        return;
    }

    getTableEntryByU16Index(sprite->dataTable, sprite->textureIndex, &textureEntry);
    tlutAddr = (s32)textureEntry.index_ptr + (sprite->paletteIndex << 5);
    texWidthShift = 0;

    if (gGraphicsMode != GRAPHICS_MODE_OPAQUE_SPRITE) {
        dim = textureEntry.width;
    loop_3:
        if (!(dim & 1)) {
            texWidthShift += 1;
            dim = dim >> 1;
            if (texWidthShift < 0x10) {
                goto loop_3;
            }
        }
        dim = textureEntry.height;
        texHeightShift = 0;
    loop_6:
        if (!(dim & 1)) {
            texHeightShift += 1;
            dim = dim >> 1;
            if (texHeightShift < 0x10) {
                goto loop_6;
            }
        }

        gSPDisplayList(gDisplayListAllocPtr++, gSpriteTextureSetupDL);
        gDPLoadTextureBlock_4b(
            gDisplayListAllocPtr++,
            textureEntry.data_ptr,
            G_IM_FMT_CI,
            textureEntry.width,
            textureEntry.height,
            0,
            G_TX_CLAMP,
            G_TX_CLAMP,
            texWidthShift,
            texHeightShift,
            0,
            0
        );
        gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, tlutAddr);
    } else if (gCachedSpriteTextureEntry.data_ptr != textureEntry.data_ptr) {
        dim = textureEntry.width;
    loop_16:
        if (!(dim & 1)) {
            texWidthShift += 1;
            dim = dim >> 1;
            if (texWidthShift < 0x10) {
                goto loop_16;
            }
        }
        dim = textureEntry.height;
        texHeightShift = 0;
    loop_19:
        if (!(dim & 1)) {
            texHeightShift += 1;
            dim = dim >> 1;
            if (texHeightShift < 0x10) {
                goto loop_19;
            }
        }

        gDPPipeSync(gDisplayListAllocPtr++);
        gDPLoadTextureBlock_4b(
            gDisplayListAllocPtr++,
            textureEntry.data_ptr,
            G_IM_FMT_CI,
            textureEntry.width,
            textureEntry.height,
            0,
            G_TX_CLAMP,
            G_TX_CLAMP,
            texWidthShift,
            texHeightShift,
            0,
            0
        );
        gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, tlutAddr);
    } else {
        gDPPipeSync(gDisplayListAllocPtr++);
        gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, tlutAddr);
    }

    gGraphicsMode = GRAPHICS_MODE_OPAQUE_SPRITE;
    memcpy(&gCachedSpriteTextureEntry, &textureEntry, sizeof(Vec3i));

    if (sprite->translationMtx == NULL) {
        sprite->translationMtx = graphicsAlloc(sizeof(Mtx));
    }
    if (sprite->scaleMtx == NULL) {
        sprite->scaleMtx = graphicsAlloc(sizeof(Mtx));
    }
    if (sprite->yRotationMtx == NULL) {
        sprite->yRotationMtx = graphicsAlloc(sizeof(Mtx));
    }
    if (sprite->zRotationMtx == NULL) {
        sprite->zRotationMtx = graphicsAlloc(sizeof(Mtx));
    }
    if (sprite->translationMtx != NULL && sprite->scaleMtx != NULL && sprite->yRotationMtx != NULL &&
        sprite->zRotationMtx != NULL) {
        memcpy(&transform, &identityMatrix, sizeof(Transform3D));
        memcpy(&gScaleMatrix.translation, &sprite->position, sizeof(Vec3i));
        transform3DToMtx(&gScaleMatrix, sprite->translationMtx);

        memcpy(&transform, &identityMatrix, sizeof(Transform3D));
        scaleXRaw = sprite->scaleX;
        if (scaleXRaw < 0) {
            scaleXRaw += 3;
        }
        scaleX = (scaleXRaw << 14) >> 16;
        scaleYRaw = sprite->scaleY;
        if (scaleYRaw < 0) {
            scaleYRaw += 3;
        }
        scaleY = (scaleYRaw << 14) >> 16;
        scaleMatrix(&transform, scaleX, scaleY, 0x2000);
        transform3DToMtx(&transform, sprite->scaleMtx);

        memcpy(&transform, &identityMatrix, sizeof(Transform3D));
        if (sprite->flags & 1) {
            createYRotationMatrix(&transform, 0x1000);
        }
        transform3DToMtx(&transform, sprite->yRotationMtx);

        memcpy(&transform, &identityMatrix, sizeof(Transform3D));
        createZRotationMatrix(&transform, sprite->zRotation);
        transform3DToMtx(&transform, sprite->zRotationMtx);

        gSPMatrix(gDisplayListAllocPtr++, sprite->translationMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPMatrix(gDisplayListAllocPtr++, gLookAtPtr, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPMatrix(gDisplayListAllocPtr++, sprite->scaleMtx, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPMatrix(gDisplayListAllocPtr++, sprite->yRotationMtx, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPMatrix(gDisplayListAllocPtr++, sprite->zRotationMtx, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPVertex(gDisplayListAllocPtr++, sprite->vertexData, 4, 0);
        gSP2Triangles(gDisplayListAllocPtr++, 0, 3, 2, 0, 2, 1, 0, 0);
    }
}

void renderTranslucentSpriteCallback(SpriteRenderState *sprite) {
    OutputStruct_19E80 textureEntry;
    Transform3D transform;
    s32 texWidthShift;
    s32 texHeightShift;
    s32 tlutAddr;
    s32 dim;
    s32 scaleXRaw;
    s32 scaleYRaw;
    s16 scaleX;
    s16 scaleY;

    if (isObjectCulled(&sprite->position) != 0) {
        return;
    }

    getTableEntryByU16Index(sprite->dataTable, sprite->textureIndex, &textureEntry);
    tlutAddr = (s32)textureEntry.index_ptr + (sprite->paletteIndex << 5);
    texWidthShift = 0;

    if (gGraphicsMode != GRAPHICS_MODE_TRANSLUCENT_SPRITE) {
        dim = textureEntry.width;
    loop_3:
        if (!(dim & 1)) {
            texWidthShift += 1;
            dim = dim >> 1;
            if (texWidthShift < 0x10) {
                goto loop_3;
            }
        }
        dim = textureEntry.height;
        texHeightShift = 0;
    loop_6:
        if (!(dim & 1)) {
            texHeightShift += 1;
            dim = dim >> 1;
            if (texHeightShift < 0x10) {
                goto loop_6;
            }
        }

        gSPDisplayList(gDisplayListAllocPtr++, gTranslucentSpriteTextureSetupDL);
        gDPLoadTextureBlock_4b(
            gDisplayListAllocPtr++,
            textureEntry.data_ptr,
            G_IM_FMT_CI,
            textureEntry.width,
            textureEntry.height,
            0,
            G_TX_CLAMP,
            G_TX_CLAMP,
            texWidthShift,
            texHeightShift,
            0,
            0
        );
        gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, tlutAddr);
    } else if (gCachedSpriteTextureEntry.data_ptr != textureEntry.data_ptr) {
        dim = textureEntry.width;
    loop_16:
        if (!(dim & 1)) {
            texWidthShift += 1;
            dim = dim >> 1;
            if (texWidthShift < 0x10) {
                goto loop_16;
            }
        }
        dim = textureEntry.height;
        texHeightShift = 0;
    loop_19:
        if (!(dim & 1)) {
            texHeightShift += 1;
            dim = dim >> 1;
            if (texHeightShift < 0x10) {
                goto loop_19;
            }
        }

        gDPPipeSync(gDisplayListAllocPtr++);
        gDPLoadTextureBlock_4b(
            gDisplayListAllocPtr++,
            textureEntry.data_ptr,
            G_IM_FMT_CI,
            textureEntry.width,
            textureEntry.height,
            0,
            G_TX_CLAMP,
            G_TX_CLAMP,
            texWidthShift,
            texHeightShift,
            0,
            0
        );
        gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, tlutAddr);
    } else {
        gDPPipeSync(gDisplayListAllocPtr++);
        gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, tlutAddr);
    }

    gGraphicsMode = GRAPHICS_MODE_TRANSLUCENT_SPRITE;
    memcpy(&gCachedSpriteTextureEntry, &textureEntry, sizeof(Vec3i));

    gDPSetEnvColor(gDisplayListAllocPtr++, 0xFF, 0xFF, 0xFF, sprite->alpha);

    if (sprite->translationMtx == NULL) {
        sprite->translationMtx = graphicsAlloc(sizeof(Mtx));
    }
    if (sprite->scaleMtx == NULL) {
        sprite->scaleMtx = graphicsAlloc(sizeof(Mtx));
    }
    if (sprite->yRotationMtx == NULL) {
        sprite->yRotationMtx = graphicsAlloc(sizeof(Mtx));
    }
    if (sprite->zRotationMtx == NULL) {
        sprite->zRotationMtx = graphicsAlloc(sizeof(Mtx));
    }
    if (sprite->translationMtx != NULL && sprite->scaleMtx != NULL && sprite->yRotationMtx != NULL &&
        sprite->zRotationMtx != NULL) {
        memcpy(&transform, &identityMatrix, sizeof(Transform3D));
        memcpy(&gScaleMatrix.translation, &sprite->position, sizeof(Vec3i));
        transform3DToMtx(&gScaleMatrix, sprite->translationMtx);

        memcpy(&transform, &identityMatrix, sizeof(Transform3D));
        scaleXRaw = sprite->scaleX;
        if (scaleXRaw < 0) {
            scaleXRaw += 3;
        }
        scaleX = (scaleXRaw << 14) >> 16;
        scaleYRaw = sprite->scaleY;
        if (scaleYRaw < 0) {
            scaleYRaw += 3;
        }
        scaleY = (scaleYRaw << 14) >> 16;
        scaleMatrix(&transform, scaleX, scaleY, 0x2000);
        transform3DToMtx(&transform, sprite->scaleMtx);

        memcpy(&transform, &identityMatrix, sizeof(Transform3D));
        if (sprite->flags & 1) {
            createYRotationMatrix(&transform, 0x1000);
        }
        transform3DToMtx(&transform, sprite->yRotationMtx);

        memcpy(&transform, &identityMatrix, sizeof(Transform3D));
        createZRotationMatrix(&transform, sprite->zRotation);
        transform3DToMtx(&transform, sprite->zRotationMtx);

        gSPMatrix(gDisplayListAllocPtr++, sprite->translationMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPMatrix(gDisplayListAllocPtr++, gLookAtPtr, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPMatrix(gDisplayListAllocPtr++, sprite->scaleMtx, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPMatrix(gDisplayListAllocPtr++, sprite->yRotationMtx, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPMatrix(gDisplayListAllocPtr++, sprite->zRotationMtx, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPVertex(gDisplayListAllocPtr++, sprite->vertexData, 4, 0);
        gSP2Triangles(gDisplayListAllocPtr++, 0, 3, 2, 0, 2, 1, 0, 0);
    }
}

void renderTransformedSpriteCallback(SpriteRenderState *sprite) {
    OutputStruct_19E80 textureEntry;
    Transform3D transform;
    s32 texWidthShift;
    s32 texHeightShift;
    s32 tlutAddr;
    s32 dim;
    s32 scaleXRaw;
    s32 scaleYRaw;
    s16 scaleX;
    s16 scaleY;

    if (isObjectCulled(&sprite->position) != 0) {
        return;
    }

    getTableEntryByU16Index(sprite->dataTable, sprite->textureIndex, &textureEntry);
    tlutAddr = (s32)textureEntry.index_ptr + (sprite->paletteIndex << 5);
    texWidthShift = 0;

    if (gGraphicsMode != GRAPHICS_MODE_TRANSLUCENT_SPRITE) {
        dim = textureEntry.width;
    loop_3:
        if (!(dim & 1)) {
            texWidthShift += 1;
            dim = dim >> 1;
            if (texWidthShift < 0x10) {
                goto loop_3;
            }
        }
        dim = textureEntry.height;
        texHeightShift = 0;
    loop_6:
        if (!(dim & 1)) {
            texHeightShift += 1;
            dim = dim >> 1;
            if (texHeightShift < 0x10) {
                goto loop_6;
            }
        }

        gSPDisplayList(gDisplayListAllocPtr++, gTranslucentSpriteTextureSetupDL);
        gDPLoadTextureBlock_4b(
            gDisplayListAllocPtr++,
            textureEntry.data_ptr,
            G_IM_FMT_CI,
            textureEntry.width,
            textureEntry.height,
            0,
            G_TX_CLAMP,
            G_TX_CLAMP,
            texWidthShift,
            texHeightShift,
            0,
            0
        );
        gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, tlutAddr);
    } else if (gCachedSpriteTextureEntry.data_ptr != textureEntry.data_ptr) {
        dim = textureEntry.width;
    loop_16:
        if (!(dim & 1)) {
            texWidthShift += 1;
            dim = dim >> 1;
            if (texWidthShift < 0x10) {
                goto loop_16;
            }
        }
        dim = textureEntry.height;
        texHeightShift = 0;
    loop_19:
        if (!(dim & 1)) {
            texHeightShift += 1;
            dim = dim >> 1;
            if (texHeightShift < 0x10) {
                goto loop_19;
            }
        }

        gDPPipeSync(gDisplayListAllocPtr++);
        gDPLoadTextureBlock_4b(
            gDisplayListAllocPtr++,
            textureEntry.data_ptr,
            G_IM_FMT_CI,
            textureEntry.width,
            textureEntry.height,
            0,
            G_TX_CLAMP,
            G_TX_CLAMP,
            texWidthShift,
            texHeightShift,
            0,
            0
        );
        gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, tlutAddr);
    } else {
        gDPPipeSync(gDisplayListAllocPtr++);
        gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, tlutAddr);
    }

    gGraphicsMode = GRAPHICS_MODE_TRANSLUCENT_SPRITE;
    memcpy(&gCachedSpriteTextureEntry, &textureEntry, sizeof(Vec3i));

    gDPSetEnvColor(gDisplayListAllocPtr++, 0xFF, 0xFF, 0xFF, sprite->alpha);

    if (sprite->translationMtx == NULL) {
        sprite->translationMtx = graphicsAlloc(sizeof(Mtx));
    }
    if (sprite->scaleMtx == NULL) {
        sprite->scaleMtx = graphicsAlloc(sizeof(Mtx));
    }
    if (sprite->yRotationMtx == NULL) {
        sprite->yRotationMtx = graphicsAlloc(sizeof(Mtx));
    }
    if (sprite->zRotationMtx == NULL) {
        sprite->zRotationMtx = graphicsAlloc(sizeof(Mtx));
    }
    if (sprite->translationMtx != NULL && sprite->scaleMtx != NULL && sprite->yRotationMtx != NULL &&
        sprite->zRotationMtx != NULL) {
        memcpy(&transform, &identityMatrix, sizeof(Transform3D));
        transform3DToMtx(&sprite->transform, sprite->translationMtx);

        memcpy(&transform, &identityMatrix, sizeof(Transform3D));
        scaleXRaw = sprite->scaleX;
        if (scaleXRaw < 0) {
            scaleXRaw += 3;
        }
        scaleX = (scaleXRaw << 14) >> 16;
        scaleYRaw = sprite->scaleY;
        if (scaleYRaw < 0) {
            scaleYRaw += 3;
        }
        scaleY = (scaleYRaw << 14) >> 16;
        scaleMatrix(&transform, scaleX, scaleY, 0x2000);
        transform3DToMtx(&transform, sprite->scaleMtx);

        memcpy(&transform, &identityMatrix, sizeof(Transform3D));
        if (sprite->flags & 1) {
            createYRotationMatrix(&transform, 0x1000);
        }
        transform3DToMtx(&transform, sprite->yRotationMtx);

        memcpy(&transform, &identityMatrix, sizeof(Transform3D));
        createZRotationMatrix(&transform, sprite->zRotation);
        transform3DToMtx(&transform, sprite->zRotationMtx);

        gSPMatrix(gDisplayListAllocPtr++, sprite->translationMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPMatrix(gDisplayListAllocPtr++, gLookAtPtr, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPMatrix(gDisplayListAllocPtr++, sprite->scaleMtx, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPMatrix(gDisplayListAllocPtr++, sprite->yRotationMtx, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPMatrix(gDisplayListAllocPtr++, sprite->zRotationMtx, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPVertex(gDisplayListAllocPtr++, sprite->vertexData, 4, 0);
        gSP2Triangles(gDisplayListAllocPtr++, 0, 3, 2, 0, 2, 1, 0, 0);
    }
}

void enqueueOpaqueSprite(u16 slot, Node *node) {
    node->unk18 = NULL;
    node->unk1C = NULL;
    node->callback = NULL;
    node->cleanupCallback = NULL;
    pushViewportCallbackBySlot(slot, VIEWPORT_CALLBACK_LAYER_SPRITES, &renderOpaqueSpriteCallback, node);
}

void enqueueTranslucentSprite(u16 slot, Node *node) {
    node->unk18 = NULL;
    node->unk1C = NULL;
    node->callback = NULL;
    node->cleanupCallback = NULL;
    pushViewportCallbackBySlot(slot, VIEWPORT_CALLBACK_LAYER_SPRITES, &renderTranslucentSpriteCallback, node);
}

void enqueueTransformedSprite(u16 slot, Node *node) {
    node->unk18 = NULL;
    node->unk1C = NULL;
    node->callback = NULL;
    node->cleanupCallback = NULL;
    pushViewportCallbackBySlot(slot, VIEWPORT_CALLBACK_LAYER_SPRITES, &renderTransformedSpriteCallback, node);
}

void setSpriteAssetEnabled(SpriteAssetState *state) {
    state->statusFlags |= SPRITE_ASSET_ENABLED;
}

void clearSpriteAssetEnabled(SpriteAssetState *state) {
    state->statusFlags &= ~SPRITE_ASSET_ENABLED;
}

s32 isSpriteAssetEnabled(SpriteAssetState *state) {
    return state->statusFlags & SPRITE_ASSET_ENABLED;
}

void setSpriteAssetVisible(SpriteAssetState *state) {
    state->statusFlags |= SPRITE_ASSET_VISIBLE;
}

void clearSpriteAssetVisible(SpriteAssetState *state) {
    state->statusFlags &= ~SPRITE_ASSET_VISIBLE;
}

s32 isSpriteAssetVisible(SpriteAssetState *state) {
    u8 val;

    val = state->statusFlags & SPRITE_ASSET_VISIBLE;
    return val != 0;
}

s32 loadSpriteAsset(SpriteAssetState *state, s16 index) {
    s32 *entry;
    s16 savedIndex;

    savedIndex = index;
    if (savedIndex >= getSpriteAssetCount()) {
        state->assetData = NULL;
        state->assetIndex = 0;
        return 0;
    }
    entry = &D_8008C920_8D520[savedIndex * 5];
    state->assetData = loadCompressedData((void *)entry[0], (void *)entry[1], entry[2]);
    state->assetIndex = index;
    state->statusFlags = 0;
    return 1;
}

void *loadSpriteAssetData(s16 index) {
    s32 *entry;

    if (index >= getSpriteAssetCount()) {
        return NULL;
    }
    entry = &D_8008C920_8D520[index * 5];
    return loadCompressedData((void *)entry[0], (void *)entry[1], entry[2]);
}

void releaseNodeMemoryRef(void **ptr) {
    if (ptr != NULL) {
        *ptr = freeNodeMemory(*ptr);
    }
}

void setSpriteAnimation(SpriteAssetState *state, s32 playbackRate, s32 animationIndex, s32 loopFrame) {
    s32 *entry;
    s16 savedAnimIndex = animationIndex;
    s16 index = animationIndex;
    u16 timer;
    void *table;
    u16 delay;

    if (index == -1) {
        state->animationSet = NULL;
        return;
    }

    entry = &D_8008C920_8D520[state->assetIndex * 5];
    if (index < *(s16 *)(&entry[4])) {
        state->animationSet = (SpriteAnimationSet *)entry[3] + index;
        state->frames = state->animationSet->frames;
        state->animationIndex = savedAnimIndex;
        state->currentSpriteFrame = state->frames->image.spriteFrame;
        state->frameIndex = 0;
        timer = state->frames->duration;
        state->frameTimer = timer;
        table = state->assetData;
        delay = state->animationSet->initialDelay;
        state->textureTable = table;
        state->initialDelay = delay;
    }
}

s32 updateSpriteAnimation(SpriteAssetState *state, s32 playbackRate) {
    s32 result = 0;
    s32 looped = 0;
    s16 timer = state->frameTimer;
    SpriteAssetFrame *entry;
    s16 command;

    if (timer > 0) {
        state->frameTimer = timer - 1;
    } else {
        state->frameIndex++;
        if (state->frameIndex >= state->animationSet->frameCount.signedValue) {
            state->frameIndex = 0;
            looped = 1;
        }

        entry = &state->frames[state->frameIndex];
        command = entry->command;

        if (command != 1) {
            if (command >= 2) {
                goto case_default;
            }
            if (command != 0) {
                goto case_default;
            }
            // command == 0: normal frame
            state->currentSpriteFrame = entry->image.spriteFrame;
            state->frameTimer = entry->duration;
        } else {
            // command == 1: jump to frame
            state->frameIndex = entry->duration;
            entry = &state->frames[state->frameIndex];
            state->currentSpriteFrame = entry->image.spriteFrame;
            state->frameTimer = entry->duration;
            result = 1;
        }
        goto end;

    case_default:
        state->frameIndex--;
        entry = &state->frames[state->frameIndex];
        state->currentSpriteFrame = entry->image.spriteFrame;
        state->frameTimer = entry->duration;
        result = 2;
    }

end:
    if (looped == 1) {
        result = 1;
    }

    return result;
}

void renderOpaqueSprite(
    SpriteAssetState *state,
    s32 slot,
    s32 posX,
    s32 posY,
    s32 posZ,
    s32 scaleX,
    s32 scaleY,
    s16 renderMode,
    u8 flipHorizontal
) {
    ((SetupAndEnqueueSprite_t)
         setupAndEnqueueSprite)(state, slot, posX, posY, posZ, scaleX, scaleY, renderMode, flipHorizontal, 0xFF, 0);
}

void renderSprite(
    SpriteAssetState *state,
    s32 slot,
    s32 posX,
    s32 posY,
    s32 posZ,
    s32 scaleX,
    s32 scaleY,
    s16 renderMode,
    u8 flipHorizontal,
    u8 alpha
) {
    s32 pad[2];

    pad[0] = 0;
    ((void (*)(void *, s32, s32, s32, s32, s32, s32, s16, u8, u8))
         setupAndEnqueueSprite)(state, slot, posX, posY, posZ, scaleX, scaleY, renderMode, flipHorizontal, alpha);
}

void setupAndEnqueueSprite(
    SpriteAssetState *state,
    s32 slot,
    s32 posX,
    s32 posY,
    s32 posZ,
    s32 scaleX,
    s32 scaleY,
    s16 renderMode,
    u8 flipH,
    u8 alpha,
    s16 renderFlags
) {
    OutputStruct_19E80 sp10;
    SpriteAssetFrame *entry;
    s32 dimensions;
    s32 sign;
    s32 offsetX;

    sign = -(flipH != 0) | 1;
    entry = state->frames + state->frameIndex;
    offsetX = entry->offset.components.x;
    state->position.x = posX + ((offsetX * sign) << 16);
    state->position.y = posY + (entry->offset.components.y << 16);
    state->position.z = posZ;
    state->textureIndex = entry->image.textureIndex;
    state->flipHorizontal = flipH;
    state->renderMode = renderMode;
    state->scaleX = scaleX >> 4;
    state->scaleY = scaleY >> 4;
    state->alpha = alpha;
    state->renderFlags = renderFlags;

    getTableEntryByU16Index(state->textureTable, state->textureIndex, &sp10);

    dimensions = (sp10.width << 16) | sp10.height;

    switch (dimensions) {
        case 0x80008:
            state->vertexData = &gSpriteVtx_8x8;
            break;
        case 0x100010:
            state->vertexData = &gSpriteVtx_16x16;
            break;
        case 0x100020:
            state->vertexData = &gSpriteVtx_16x32;
            break;
        case 0x200010:
            state->vertexData = &gSpriteVtx_32x16;
            break;
        case 0x200020:
            state->vertexData = &gSpriteVtx_32x32;
            break;
        case 0x400020:
            state->vertexData = &gSpriteVtx_64x32;
            break;
        case 0x200040:
            state->vertexData = &gSpriteVtx_32x64;
            break;
        case 0x400040:
            state->vertexData = &gSpriteVtx_64x64;
            break;
    }

    if ((alpha & 0xFF) == 0xFF) {
        ((void (*)(s32, Node *))enqueueOpaqueSprite)(slot, (Node *)&state->vertexData);
    } else {
        ((void (*)(s32, Node *))enqueueTranslucentSprite)(slot, (Node *)&state->vertexData);
    }
}

s32 getSpriteFrameWidth(SpriteAssetState *state) {
    OutputStruct_19E80 output;

    getTableEntryByU16Index(state->assetData, state->animationIndex, &output);
    return output.width;
}
