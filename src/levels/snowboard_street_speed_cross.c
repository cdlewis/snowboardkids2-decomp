#include "common.h"
#include "data/course_data.h"
#include "graphics/displaylist.h"
#include "math/geometry.h"
#include "system/task_scheduler.h"

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

static void updateSpeedCrossRotatingSky(RotatingSkyRenderArg *sky);
static void cleanupSpeedCrossRotatingSky(RotatingSkyArg *sky);

void initSpeedCrossRotatingSky(RotatingSkyArg *sky) {
    GameState *gameState;
    LevelDisplayLists *displayLists;

    gameState = (GameState *)getCurrentAllocation();
    displayLists = getSkyDisplayLists3ByIndex(gameState->memoryPoolId);
    sky->displayLists = (void *)((u32)displayLists + 0x90);
    sky->uncompressedAsset = loadUncompressedAssetByIndex(0xC);
    sky->compressedAsset = loadCompressedSegment2AssetByIndex(0xC);
    sky->posX = 0x25990000;
    sky->posY = 0x1A2B0000;
    sky->unk2C = 0;
    sky->posZ = 0xF7A30000;
    setCleanupCallback(cleanupSpeedCrossRotatingSky);
    setCallback(updateSpeedCrossRotatingSky);
}

static void updateSpeedCrossRotatingSky(RotatingSkyRenderArg *sky) {
    s32 i;

    sky->rotationAngle -= 0x20;
    createYRotationMatrix(&sky->matrix, sky->rotationAngle);

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)sky);
    }
}

static void cleanupSpeedCrossRotatingSky(RotatingSkyArg *sky) {
    sky->uncompressedAsset = freeNodeMemory(sky->uncompressedAsset);
    sky->compressedAsset = freeNodeMemory(sky->compressedAsset);
}
