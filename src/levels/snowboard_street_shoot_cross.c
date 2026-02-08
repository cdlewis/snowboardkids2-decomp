#include "levels/snowboard_street_shoot_cross.h"
#include "19E80.h"
#include "56910.h"
#include "594E0.h"
#include "5AA90.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
#include "rom_loader.h"
#include "task_scheduler.h"
#include "assets.h"

#define Y_OFFSET 0xFFE80000


typedef struct {
    s8 textureIndex;
    u8 pad1[3];
    s32 x;
    s32 y;
    s32 z;
} ShootCrossTargetEntry;

typedef struct {
    void *transformMatrices; // Transform3D array
    void *vertexData;        // Vtx[4] for rendering quad
    ShootCrossTargetEntry *targets;
    void *spriteAsset;       // Texture data for targets
    s32 *targetPositionData; // Compressed position data
    u8 _pad14[0x2];
    s16 targetCount;
} ShootCrossTargets;

typedef struct {
    u8 _pad[0x24];
    ShootCrossTargets *shootCrossTargets;
} ShootCrossAllocationStruct;

typedef struct {
    void *unk0;
    void *unk4;
    void *unk8;
    void *unkC;
    Player *unk10;
    void *unk14;
    u8 pad18[0xC];
    ShootCrossTargets *shootCrossTargets;
    u8 pad28[0x32];
    u8 hitCount;
} ProjectileAllocation;

typedef struct {
    u8 _pad[0x5C];
    u8 skyDisplayListIndex;
} RotatingSkyAllocation;

typedef struct {
    u8 _pad[0x14];
    s16 courseId;
} ShootCrossTask;

typedef struct {
    /* 0x00 */ Transform3D matrix;
    /* 0x20 */ u8 _pad20[0x1C];
    /* 0x3C */ u16 rotationAngle;
} RotatingSkyRenderArg;

void renderShootCrossTargets(ShootCrossTargets *arg0);
void updateRotatingSky(RotatingSkyRenderArg *arg0);
void cleanupRotatingSky(RotatingSkyArg *rotatingSky);
void cleanupShootCrossTargets(ShootCrossTargets *arg0);
void initShootCrossTargetsCallback(ShootCrossTargets *arg0);
void activateShootCrossTargets(ShootCrossTargets *arg0);

extern Gfx D_8009A780_9B380[];
extern Gfx *gRegionAllocPtr;
extern s16 gGraphicsMode;
extern s32 gLookAtPtr;
extern s32 gTempPosition;
extern void *D_800BBBB0_AD630;

void initShootCrossTargets(ShootCrossTargets *arg0) {
    arg0->spriteAsset = loadAsset_34F9A0();
    arg0->targetPositionData = loadCompressedData(&_4060A0_ROM_START, &_4060A0_ROM_END, 0x160);
    arg0->transformMatrices = NULL;
    setCleanupCallback(cleanupShootCrossTargets);
    setCallback(initShootCrossTargetsCallback);
}

void initShootCrossTargetsCallback(ShootCrossTargets *arg0) {
    s32 i;
    s32 *ptr;
    ShootCrossTargetEntry *targets;
    s32 pad[2];
    s32 offset;
    ShootCrossAllocationStruct *allocation;
    ShootCrossTargetEntry *temp;

    allocation = (ShootCrossAllocationStruct *)getCurrentAllocation();
    arg0->vertexData = &D_800BBBB0_AD630;
    arg0->targets = (ShootCrossTargetEntry *)((s8 *)arg0->targetPositionData + *arg0->targetPositionData);
    targets = *(ShootCrossTargetEntry *volatile *)&arg0->targets;
    arg0->targetCount = 0;

    if (targets->textureIndex >= 0) {
        temp = targets;
        do {
            arg0->targetCount++;
        } while (temp[arg0->targetCount].textureIndex >= 0);
    }

    i = 0;
    arg0->transformMatrices = allocateNodeMemory(arg0->targetCount << 6);

    if (arg0->targetCount > 0) {
        ptr = &gTempPosition;
        do {
            offset = i << 4;
            *((s8 *)(offset + (s32)arg0->targets)) = 0;
            memcpy(ptr, (s8 *)(offset + (s32)arg0->targets + 4), 0xC);
            transform3DToMtx(ptr - 5, (u8 *)arg0->transformMatrices + (i << 6));
            i++;
        } while (i < arg0->targetCount);
    }

    allocation->shootCrossTargets = arg0;
    setCallback(activateShootCrossTargets);
}

void activateShootCrossTargets(ShootCrossTargets *arg0) {
    s32 i;

    for (i = 0; i < arg0->targetCount; i++) {
        checkPositionPlayerCollisionWithPull(&arg0->targets[i].x, 0x180000, 0x300000);
    }

    for (i = 0; i < 4; i++) {
        debugEnqueueCallback((u16)i, 4, renderShootCrossTargets, arg0);
    }
}

void cleanupShootCrossTargets(ShootCrossTargets *arg0) {
    ShootCrossAllocationStruct *allocation = (ShootCrossAllocationStruct *)getCurrentAllocation();
    allocation->shootCrossTargets = NULL;
    arg0->transformMatrices = freeNodeMemory(arg0->transformMatrices);
    arg0->spriteAsset = freeNodeMemory(arg0->spriteAsset);
    arg0->targetPositionData = freeNodeMemory(arg0->targetPositionData);
}

s32 checkProjectileTargetHit(Vec3i *projectilePos, s32 hitRange) {
    s32 pos[3];
    s32 unused[2];
    ProjectileAllocation *allocation;
    ShootCrossTargets *targets;
    s32 range;
    s32 negRange;
    s32 i = 0;
    s32 x, y, z;
    s8 *entry;
    s32 yOffset;
    s32 idx;

    allocation = (ProjectileAllocation *)getCurrentAllocation();
    targets = allocation->shootCrossTargets;

    if (targets) {
        goto check_count;
    }

    return 0;

found:
    *entry = 1;
    allocation->hitCount = allocation->hitCount + 1;
    playTrickSuccessVoice(allocation->unk10);
    return 1;

check_count:
    range = hitRange + 0x140000;

    if (targets->targetCount <= 0) {
        return 0;
    }

    yOffset = Y_OFFSET;
    negRange = -range;

    do {
        memcpy(pos, projectilePos, sizeof(Vec3i));
        idx = i << 4;
        x = pos[0] - targets->targets[i].x;
        pos[0] = x;
        y = (pos[1] + yOffset) - targets->targets[i].y;
        pos[1] = y;
        z = pos[2] - targets->targets[i].z;
        pos[2] = z;

        if (negRange < x) {
            if (x < range) {
                if (negRange < y) {
                    if (y < range) {
                        if (negRange < z) {
                            if (z < range) {
                                if (distance_3d(x, y, z) < range) {
                                    entry = (s8 *)(idx + (s32)targets->targets);
                                    if (*entry == 0) {
                                        goto found;
                                    }
                                    return 2;
                                }
                            }
                        }
                    }
                }
            }
        }

        i++;
    } while (i < targets->targetCount);

    return 0;
}

void renderShootCrossTargets(ShootCrossTargets *arg0) {
    OutputStruct_19E80 tableEntry;
    s32 prevTextureIndex;
    s32 i;
    s32 offset;

    prevTextureIndex = -1;
    gGraphicsMode = -1;
    gSPDisplayList(gRegionAllocPtr++, D_8009A780_9B380);

    for (i = 0; i < arg0->targetCount; i++) {
        offset = i << 4;

        if (isObjectCulled((Vec3i *)((u8 *)arg0->targets + offset + 4)) == 0) {
            s8 textureIndex = *(s8 *)(offset + (s32)arg0->targets);

            if (prevTextureIndex != textureIndex) {
                prevTextureIndex = textureIndex;
                getTableEntryByU16Index(arg0->spriteAsset, (u16)prevTextureIndex, &tableEntry);

                gDPLoadMultiBlock_4b(
                    gRegionAllocPtr++,
                    tableEntry.data_ptr,
                    0,
                    G_TX_RENDERTILE,
                    G_IM_FMT_CI,
                    tableEntry.field1,
                    tableEntry.field2,
                    0,
                    G_TX_CLAMP,
                    G_TX_CLAMP,
                    G_TX_NOMASK,
                    G_TX_NOMASK,
                    G_TX_NOLOD,
                    G_TX_NOLOD
                );

                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, tableEntry.index_ptr);
            }

            gSPMatrix(
                gRegionAllocPtr++,
                (u8 *)arg0->transformMatrices + (i << 6),
                G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW
            );

            gSPMatrix(gRegionAllocPtr++, gLookAtPtr, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);

            gSPVertex(gRegionAllocPtr++, arg0->vertexData, 4, 0);

            gSP2Triangles(gRegionAllocPtr++, 0, 3, 2, 0, 2, 1, 0, 0);
        }
    }
}

void scheduleShootCrossTargetsTask(s32 courseId) {
    ShootCrossTask *task;

    task = (ShootCrossTask *)scheduleTask(initShootCrossTargets, 0, 0, 0x32);
    if (task != NULL) {
        task->courseId = courseId;
    }
}

void initRotatingSky(RotatingSkyArg *rotatingSky) {
    RotatingSkyAllocation *allocation;
    LevelDisplayLists *result;

    allocation = (RotatingSkyAllocation *)getCurrentAllocation();
    result = getSkyDisplayLists3ByIndex(allocation->skyDisplayListIndex);
    rotatingSky->displayLists = (void *)((u32)result + 0x90);
    rotatingSky->uncompressedAsset = loadUncompressedAssetByIndex(0xD);
    rotatingSky->compressedAsset = loadCompressedSegment2AssetByIndex(0xD);
    rotatingSky->posX = 0x25990000;
    rotatingSky->posY = 0x1A2B0000;
    rotatingSky->unk2C = 0;
    rotatingSky->posZ = 0xF7A30000;
    setCleanupCallback(cleanupRotatingSky);
    setCallback(updateRotatingSky);
}

void updateRotatingSky(RotatingSkyRenderArg *arg0) {
    s32 i;

    arg0->rotationAngle -= 0x20;
    createYRotationMatrix(&arg0->matrix, arg0->rotationAngle);

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void cleanupRotatingSky(RotatingSkyArg *rotatingSky) {
    rotatingSky->uncompressedAsset = freeNodeMemory(rotatingSky->uncompressedAsset);
    rotatingSky->compressedAsset = freeNodeMemory(rotatingSky->compressedAsset);
}
