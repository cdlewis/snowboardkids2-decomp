#include "56910.h"
#include "common.h"
#include "displaylist.h"
#include "geometry.h"
#include "task_scheduler.h"

typedef struct {
    u8 _pad[0x14];
    s32 posX;
    s32 posY;
    s32 posZ;
    void *displayLists;
    void *uncompressedAsset;
    void *compressedAsset;
    s32 unk2C;
} RotatingSkyArg;

typedef struct {
    /* 0x00 */ Transform3D matrix;
    /* 0x20 */ u8 _pad20[0x1C];
    /* 0x3C */ u16 rotationAngle;
} RotatingSkyRenderArg;

static void updateRotatingSky(RotatingSkyRenderArg *arg0);
static void cleanupRotatingSky(RotatingSkyArg *arg0);

void func_800BB2B0_B2750(RotatingSkyArg *arg0) {
    GameState *allocation;
    LevelDisplayLists *result;

    allocation = (GameState *)getCurrentAllocation();
    result = getSkyDisplayLists3ByIndex(allocation->memoryPoolId);
    arg0->displayLists = (void *)((u32)result + 0x90);
    arg0->uncompressedAsset = loadUncompressedAssetByIndex(0xC);
    arg0->compressedAsset = loadCompressedSegment2AssetByIndex(0xC);
    arg0->posX = 0x25990000;
    arg0->posY = 0x1A2B0000;
    arg0->unk2C = 0;
    arg0->posZ = 0xF7A30000;
    setCleanupCallback(cleanupRotatingSky);
    setCallback(updateRotatingSky);
}

static void updateRotatingSky(RotatingSkyRenderArg *arg0) {
    s32 i;

    arg0->rotationAngle -= 0x20;
    createYRotationMatrix(&arg0->matrix, arg0->rotationAngle);

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

static void cleanupRotatingSky(RotatingSkyArg *arg0) {
    arg0->uncompressedAsset = freeNodeMemory(arg0->uncompressedAsset);
    arg0->compressedAsset = freeNodeMemory(arg0->compressedAsset);
}
