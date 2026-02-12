#include "90F0.h"

#include "assets.h"
#include "common.h"
#include "gbi.h"
#include "geometry.h"
#include "graphics.h"
#include "task_scheduler.h"
#include "ultra64.h"

extern s32 D_8008C920_8D520[];
extern Vec3i gTempPosition;
extern s32 D_8009F1F0_9FDF0;
extern s16 gGraphicsMode;
extern s32 gLookAtPtr;
extern Gfx *gRegionAllocPtr;

s16 gSpriteAssetTable[][10] = {
    { 0x0043, 0x0000, 0x0064, (s16)0xEFD0, 0x0064, (s16)0xF190, 0x0000, 0x01F8, (s16)0x8008, (s16)0xC7F0 },
    { 0x0001, 0x0000, 0x0064, (s16)0x8C60, 0x0064, (s16)0x8EB0, 0x0000, 0x0358, (s16)0x8008, (s16)0xC830 },
    { 0x0002, 0x0000, 0x0064, (s16)0x89F0, 0x0064, (s16)0x8B50, 0x0000, 0x0358, (s16)0x8008, (s16)0xC858 },
    { 0x0001, 0x0000, 0x0064, 0x7930,      0x0064, 0x7F90,      0x0000, 0x0638, (s16)0x8008, (s16)0xC888 },
    { 0x0001, 0x0000, 0x0064, 0x7740,      0x0064, 0x7930,      0x0000, 0x0468, (s16)0x8008, (s16)0xC8B0 },
    { 0x0001, 0x0000, 0x0064, (s16)0x8B50, 0x0064, (s16)0x8C60, 0x0000, 0x0358, (s16)0x8008, (s16)0xC858 },
    { 0x0001, 0x0000, 0x0064, 0x62A0,      0x0064, 0x6850,      0x0000, 0x0A78, (s16)0x8008, (s16)0xC8E0 },
    { 0x0001, 0x0000, 0x0064, 0x48D0,      0x0064, 0x62A0,      0x0000, 0x35D8, (s16)0x8008, (s16)0xC8F0 },
    { 0x0002, 0x0000, 0x0066, 0x3010,      0x0066, 0x3330,      0x0000, 0x0848, (s16)0x8008, (s16)0xC910 },
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

DmaEntry gSpriteDmaTable[] = {
    { &_49BC40_ROM_START, &_49BC40_ROM_END, 0x00000D80 },
    { &_49C200_ROM_START, &_49C200_ROM_END, 0x00000660 },
    { &_49C430_ROM_START, &_49C430_ROM_END, 0x000002A0 },
    { &_49C5B0_ROM_START, &_49C5B0_ROM_END, 0x00000CB0 },
    { &_49CCA0_ROM_START, &_49CCA0_ROM_END, 0x00001940 },
};

DmaEntry *gSpriteDmaTablePtr = gSpriteDmaTable;

s32 gSpriteDmaTableInfo[] = { 0x00000005, 0x00000001, 0x00000000, 0x00000000 };

Gfx gSpriteTextureSetupDL[] = {
    { .words = { 0xD9D0F9FA, 0x00000000 } }, { .words = { 0xD9FFFFFF, 0x00210005 } },
    { .words = { 0xD7000002, 0x80008000 } }, { .words = { 0xE7000000, 0x00000000 } },
    { .words = { 0xE3001201, 0x00002000 } }, { .words = { 0xE3000A01, 0x00100000 } },
    { .words = { 0xFC127FFF, 0xFFFFF238 } }, { .words = { 0xE200001C, 0xC8113078 } },
    { .words = { 0xE3001001, 0x00008000 } }, { .words = { 0xDF000000, 0x00000000 } },
};

Gfx gTranslucentSpriteTextureSetupDL[] = {
    { .words = { 0xD9D0F9FA, 0x00000000 } }, { .words = { 0xD9FFFFFF, 0x00210005 } },
    { .words = { 0xD7000002, 0x80008000 } }, { .words = { 0xE7000000, 0x00000000 } },
    { .words = { 0xE3001201, 0x00002000 } }, { .words = { 0xE3000A01, 0x00100000 } },
    { .words = { 0xFC121BFF, 0xFFFFFE38 } }, { .words = { 0xE200001C, 0xC81049D8 } },
    { .words = { 0xE3001001, 0x00008000 } }, { .words = { 0xDF000000, 0x00000000 } },
};

s32 getSpriteAssetCount(void) {
    return 10;
}

s16 getSpriteAssetId(s32 index) {
    return gSpriteAssetTable[index][0];
}

typedef struct {
    /* 0x00 */ u32 vertices;
    /* 0x04 */ Vec3i position;
    /* 0x10 */ DataTable_19E80 *table;
    /* 0x14 */ u16 index;
    /* 0x16 */ u8 alpha;
    /* 0x17 */ u8 flags;
    /* 0x18 */ Mtx *translationMtx;
    /* 0x1C */ Mtx *scaleMtx;
    /* 0x20 */ Mtx *yRotationMtx;
    /* 0x24 */ Mtx *zRotationMtx;
    /* 0x28 */ s32 scaleX;
    /* 0x2C */ s32 scaleY;
    /* 0x30 */ u16 zRotation;
    /* 0x32 */ s16 texIndex;
} OpaqueSpriteStruct_90F0;

void renderOpaqueSpriteCallback(OpaqueSpriteStruct_90F0 *sprite) {
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

    getTableEntryByU16Index(sprite->table, sprite->index, &textureEntry);
    tlutAddr = (s32)textureEntry.index_ptr + (sprite->texIndex << 5);
    texWidthShift = 0;

    if (gGraphicsMode != 0x202) {
        dim = textureEntry.field1;
    loop_3:
        if (!(dim & 1)) {
            texWidthShift += 1;
            dim = dim >> 1;
            if (texWidthShift < 0x10) {
                goto loop_3;
            }
        }
        dim = textureEntry.field2;
        texHeightShift = 0;
    loop_6:
        if (!(dim & 1)) {
            texHeightShift += 1;
            dim = dim >> 1;
            if (texHeightShift < 0x10) {
                goto loop_6;
            }
        }

        gSPDisplayList(gRegionAllocPtr++, gSpriteTextureSetupDL);
        gDPLoadTextureBlock_4b(
            gRegionAllocPtr++,
            textureEntry.data_ptr,
            G_IM_FMT_CI,
            textureEntry.field1,
            textureEntry.field2,
            0,
            G_TX_CLAMP,
            G_TX_CLAMP,
            texWidthShift,
            texHeightShift,
            0,
            0
        );
        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, tlutAddr);
    } else if (D_8009F1F0_9FDF0 != (s32)textureEntry.data_ptr) {
        dim = textureEntry.field1;
    loop_16:
        if (!(dim & 1)) {
            texWidthShift += 1;
            dim = dim >> 1;
            if (texWidthShift < 0x10) {
                goto loop_16;
            }
        }
        dim = textureEntry.field2;
        texHeightShift = 0;
    loop_19:
        if (!(dim & 1)) {
            texHeightShift += 1;
            dim = dim >> 1;
            if (texHeightShift < 0x10) {
                goto loop_19;
            }
        }

        gDPPipeSync(gRegionAllocPtr++);
        gDPLoadTextureBlock_4b(
            gRegionAllocPtr++,
            textureEntry.data_ptr,
            G_IM_FMT_CI,
            textureEntry.field1,
            textureEntry.field2,
            0,
            G_TX_CLAMP,
            G_TX_CLAMP,
            texWidthShift,
            texHeightShift,
            0,
            0
        );
        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, tlutAddr);
    } else {
        gDPPipeSync(gRegionAllocPtr++);
        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, tlutAddr);
    }

    gGraphicsMode = 0x202;
    memcpy(&D_8009F1F0_9FDF0, &textureEntry, 0xC);

    if (sprite->translationMtx == NULL) {
        sprite->translationMtx = arenaAlloc16(0x40);
    }
    if (sprite->scaleMtx == NULL) {
        sprite->scaleMtx = arenaAlloc16(0x40);
    }
    if (sprite->yRotationMtx == NULL) {
        sprite->yRotationMtx = arenaAlloc16(0x40);
    }
    if (sprite->zRotationMtx == NULL) {
        sprite->zRotationMtx = arenaAlloc16(0x40);
    }
    if (sprite->translationMtx != NULL && sprite->scaleMtx != NULL && sprite->yRotationMtx != NULL &&
        sprite->zRotationMtx != NULL) {
        memcpy(&transform, &identityMatrix, 0x20);
        memcpy(&gTempPosition, &sprite->position, 0xC);
        transform3DToMtx((u8 *)&gTempPosition - 0x14, sprite->translationMtx);

        memcpy(&transform, &identityMatrix, 0x20);
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

        memcpy(&transform, &identityMatrix, 0x20);
        if (sprite->flags & 1) {
            createYRotationMatrix(&transform, 0x1000);
        }
        transform3DToMtx(&transform, sprite->yRotationMtx);

        memcpy(&transform, &identityMatrix, 0x20);
        createZRotationMatrix(&transform, sprite->zRotation);
        transform3DToMtx(&transform, sprite->zRotationMtx);

        gSPMatrix(gRegionAllocPtr++, sprite->translationMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPMatrix(gRegionAllocPtr++, gLookAtPtr, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPMatrix(gRegionAllocPtr++, sprite->scaleMtx, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPMatrix(gRegionAllocPtr++, sprite->yRotationMtx, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPMatrix(gRegionAllocPtr++, sprite->zRotationMtx, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPVertex(gRegionAllocPtr++, sprite->vertices, 4, 0);
        gSP2Triangles(gRegionAllocPtr++, 0, 3, 2, 0, 2, 1, 0, 0);
    }
}

void renderTranslucentSpriteCallback(OpaqueSpriteStruct_90F0 *sprite) {
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

    getTableEntryByU16Index(sprite->table, sprite->index, &textureEntry);
    tlutAddr = (s32)textureEntry.index_ptr + (sprite->texIndex << 5);
    texWidthShift = 0;

    if (gGraphicsMode != 0x203) {
        dim = textureEntry.field1;
    loop_3:
        if (!(dim & 1)) {
            texWidthShift += 1;
            dim = dim >> 1;
            if (texWidthShift < 0x10) {
                goto loop_3;
            }
        }
        dim = textureEntry.field2;
        texHeightShift = 0;
    loop_6:
        if (!(dim & 1)) {
            texHeightShift += 1;
            dim = dim >> 1;
            if (texHeightShift < 0x10) {
                goto loop_6;
            }
        }

        gSPDisplayList(gRegionAllocPtr++, gTranslucentSpriteTextureSetupDL);
        gDPLoadTextureBlock_4b(
            gRegionAllocPtr++,
            textureEntry.data_ptr,
            G_IM_FMT_CI,
            textureEntry.field1,
            textureEntry.field2,
            0,
            G_TX_CLAMP,
            G_TX_CLAMP,
            texWidthShift,
            texHeightShift,
            0,
            0
        );
        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, tlutAddr);
    } else if (D_8009F1F0_9FDF0 != (s32)textureEntry.data_ptr) {
        dim = textureEntry.field1;
    loop_16:
        if (!(dim & 1)) {
            texWidthShift += 1;
            dim = dim >> 1;
            if (texWidthShift < 0x10) {
                goto loop_16;
            }
        }
        dim = textureEntry.field2;
        texHeightShift = 0;
    loop_19:
        if (!(dim & 1)) {
            texHeightShift += 1;
            dim = dim >> 1;
            if (texHeightShift < 0x10) {
                goto loop_19;
            }
        }

        gDPPipeSync(gRegionAllocPtr++);
        gDPLoadTextureBlock_4b(
            gRegionAllocPtr++,
            textureEntry.data_ptr,
            G_IM_FMT_CI,
            textureEntry.field1,
            textureEntry.field2,
            0,
            G_TX_CLAMP,
            G_TX_CLAMP,
            texWidthShift,
            texHeightShift,
            0,
            0
        );
        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, tlutAddr);
    } else {
        gDPPipeSync(gRegionAllocPtr++);
        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, tlutAddr);
    }

    gGraphicsMode = 0x203;
    memcpy(&D_8009F1F0_9FDF0, &textureEntry, 0xC);

    gDPSetEnvColor(gRegionAllocPtr++, 0xFF, 0xFF, 0xFF, sprite->alpha);

    if (sprite->translationMtx == NULL) {
        sprite->translationMtx = arenaAlloc16(0x40);
    }
    if (sprite->scaleMtx == NULL) {
        sprite->scaleMtx = arenaAlloc16(0x40);
    }
    if (sprite->yRotationMtx == NULL) {
        sprite->yRotationMtx = arenaAlloc16(0x40);
    }
    if (sprite->zRotationMtx == NULL) {
        sprite->zRotationMtx = arenaAlloc16(0x40);
    }
    if (sprite->translationMtx != NULL && sprite->scaleMtx != NULL && sprite->yRotationMtx != NULL &&
        sprite->zRotationMtx != NULL) {
        memcpy(&transform, &identityMatrix, 0x20);
        memcpy(&gTempPosition, &sprite->position, 0xC);
        transform3DToMtx((u8 *)&gTempPosition - 0x14, sprite->translationMtx);

        memcpy(&transform, &identityMatrix, 0x20);
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

        memcpy(&transform, &identityMatrix, 0x20);
        if (sprite->flags & 1) {
            createYRotationMatrix(&transform, 0x1000);
        }
        transform3DToMtx(&transform, sprite->yRotationMtx);

        memcpy(&transform, &identityMatrix, 0x20);
        createZRotationMatrix(&transform, sprite->zRotation);
        transform3DToMtx(&transform, sprite->zRotationMtx);

        gSPMatrix(gRegionAllocPtr++, sprite->translationMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPMatrix(gRegionAllocPtr++, gLookAtPtr, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPMatrix(gRegionAllocPtr++, sprite->scaleMtx, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPMatrix(gRegionAllocPtr++, sprite->yRotationMtx, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPMatrix(gRegionAllocPtr++, sprite->zRotationMtx, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPVertex(gRegionAllocPtr++, sprite->vertices, 4, 0);
        gSP2Triangles(gRegionAllocPtr++, 0, 3, 2, 0, 2, 1, 0, 0);
    }
}

INCLUDE_ASM("asm/nonmatchings/90F0", func_80009548_A148);

extern void func_80009548_A148(void);

void enqueueOpaqueSprite(u16 slot, Node *node) {
    node->unk18 = NULL;
    node->unk1C = NULL;
    node->callback = NULL;
    node->cleanupCallback = NULL;
    debugEnqueueCallback(slot, 4, &renderOpaqueSpriteCallback, node);
}

void enqueueTranslucentSprite(u16 slot, Node *node) {
    node->unk18 = NULL;
    node->unk1C = NULL;
    node->callback = NULL;
    node->cleanupCallback = NULL;
    debugEnqueueCallback(slot, 4, &renderTranslucentSpriteCallback, node);
}

void enqueueTransformedSprite(u16 slot, Node *node) {
    node->unk18 = NULL;
    node->unk1C = NULL;
    node->callback = NULL;
    node->cleanupCallback = NULL;
    debugEnqueueCallback(slot, 4, &func_80009548_A148, node);
}

void setSpriteAssetEnabled(SpriteAssetState *state) {
    state->flags |= 1;
}

void clearSpriteAssetEnabled(SpriteAssetState *state) {
    state->flags &= 0xFE;
}

s32 isSpriteAssetEnabled(SpriteAssetState *state) {
    return state->flags & 1;
}

void setSpriteAssetVisible(SpriteAssetState *state) {
    state->flags |= 2;
}

void clearSpriteAssetVisible(SpriteAssetState *state) {
    state->flags &= 0xFD;
}

s32 isSpriteAssetVisible(SpriteAssetState *state) {
    u8 val;

    val = state->flags & 2;
    return val != 0;
}

s32 loadSpriteAsset(SpriteAssetState *state, s16 index) {
    s32 *entry;
    s16 savedIndex;

    savedIndex = index;
    if (savedIndex >= getSpriteAssetCount()) {
        state->spriteData = NULL;
        state->assetIndex = 0;
        return 0;
    }
    entry = &D_8008C920_8D520[savedIndex * 5];
    state->spriteData = loadCompressedData((void *)entry[0], (void *)entry[1], entry[2]);
    state->assetIndex = index;
    state->flags = 0;
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

void setSpriteAnimation(void *arg0, s32 arg1, s32 animIndex, s32 arg3) {
    SpriteAssetState *state = (SpriteAssetState *)arg0;
    s32 *entry;
    s16 savedAnimIndex = animIndex;
    s16 index = animIndex;
    u16 timer;
    void *table;
    u16 delay;

    if (index == -1) {
        state->animSet = NULL;
        return;
    }

    entry = &D_8008C920_8D520[state->assetIndex * 5];
    if (index < *(s16 *)(&entry[4])) {
        state->animSet = (AnimSetEntry *)entry[3] + index;
        state->frameEntries = state->animSet->entries;
        state->animIndex = savedAnimIndex;
        state->currentSpriteFrame = state->frameEntries->unk4;
        state->frameIndex = 0;
        timer = state->frameEntries->unk6;
        state->frameTimer = timer;
        table = state->spriteData;
        delay = state->animSet->initialDelay;
        state->spriteTable = table;
        state->initialDelay = delay;
    }
}

s32 updateSpriteAnimation(void *arg0, s32 arg1) {
    AnimationState *state = (AnimationState *)arg0;
    s32 result = 0;
    s32 looped = 0;
    s16 timer = state->frameTimer;
    AnimationEntry *entry;
    s16 command;

    if (timer > 0) {
        state->frameTimer = timer - 1;
    } else {
        state->frameIndex++;
        if (state->frameIndex >= state->header->frameCount) {
            state->frameIndex = 0;
            looped = 1;
        }

        entry = &state->entries[state->frameIndex];
        command = entry->command;

        if (command != 1) {
            if (command >= 2) {
                goto case_default;
            }
            if (command != 0) {
                goto case_default;
            }
            // command == 0: normal frame
            state->currentSpriteFrame = entry->spriteFrame;
            state->frameTimer = entry->duration;
        } else {
            // command == 1: jump to frame
            state->frameIndex = entry->duration;
            entry = &state->entries[state->frameIndex];
            state->currentSpriteFrame = entry->spriteFrame;
            state->frameTimer = entry->duration;
            result = 1;
        }
        goto end;

    case_default:
        state->frameIndex--;
        entry = &state->entries[state->frameIndex];
        state->currentSpriteFrame = entry->spriteFrame;
        state->frameTimer = entry->duration;
        result = 2;
    }

end:
    if (looped == 1) {
        result = 1;
    }

    return result;
}

void setupAndEnqueueSprite(
    SpriteState *state,
    s32 slot,
    s32 posX,
    s32 posY,
    s32 posZ,
    s32 scaleX,
    s32 scaleY,
    s16 arg7,
    u8 flipH,
    u8 alpha,
    u16 arg10
);

typedef void (*SetupAndEnqueueSprite_t)(SpriteState *, s32, s32, s32, s32, s32, s32, s16, u8, u8, s32);

void renderOpaqueSprite(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s16 arg7, u8 arg8) {
    ((SetupAndEnqueueSprite_t
    )setupAndEnqueueSprite)((SpriteState *)arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, 0xFF, 0);
}

void renderSprite(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s16 arg7, u8 arg8, u8 arg9) {
    s32 pad[2];

    pad[0] = 0;
    ((void (*)(void *, s32, s32, s32, s32, s32, s32, s16, u8, u8)
    )setupAndEnqueueSprite)(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
}

void setupAndEnqueueSprite(
    SpriteState *state,
    s32 slot,
    s32 posX,
    s32 posY,
    s32 posZ,
    s32 scaleX,
    s32 scaleY,
    s16 renderMode,
    u8 flipH,
    u8 alpha,
    u16 renderFlags
) {
    OutputStruct_19E80 sp10;
    SpriteEntry *entry;
    s32 dimensions;
    s32 sign;
    s32 offsetX;

    sign = -(flipH != 0) | 1;
    entry = state->spriteEntries + state->currentFrame;
    offsetX = entry->offsetX;
    state->positionX = posX + ((offsetX * sign) << 16);
    state->positionY = posY + (entry->offsetY << 16);
    state->positionZ = posZ;
    state->textureIndex = entry->textureIndex;
    state->flipHorizontal = flipH;
    state->renderMode = renderMode;
    state->scaleX = scaleX >> 4;
    state->scaleY = scaleY >> 4;
    state->alpha = alpha;
    state->renderFlags = renderFlags;

    getTableEntryByU16Index(state->spriteTable, state->textureIndex, &sp10);

    dimensions = (sp10.field1 << 16) | sp10.field2;

    switch (dimensions) {
        case 0x80008:
            state->displayListData = &gSpriteVtx_8x8;
            break;
        case 0x100010:
            state->displayListData = &gSpriteVtx_16x16;
            break;
        case 0x100020:
            state->displayListData = &gSpriteVtx_16x32;
            break;
        case 0x200010:
            state->displayListData = &gSpriteVtx_32x16;
            break;
        case 0x200020:
            state->displayListData = &gSpriteVtx_32x32;
            break;
        case 0x400020:
            state->displayListData = &gSpriteVtx_64x32;
            break;
        case 0x200040:
            state->displayListData = &gSpriteVtx_32x64;
            break;
        case 0x400040:
            state->displayListData = &gSpriteVtx_64x64;
            break;
    }

    if ((alpha & 0xFF) == 0xFF) {
        ((void (*)(s32, Node *))enqueueOpaqueSprite)(slot, (Node *)&state->displayListData);
    } else {
        ((void (*)(s32, Node *))enqueueTranslucentSprite)(slot, (Node *)&state->displayListData);
    }
}

s32 getTableEntryValue(TableLookupContext *ctx) {
    OutputStruct_19E80 output;

    getTableEntryByU16Index(ctx->table, ctx->index, &output);
    return output.field1;
}
