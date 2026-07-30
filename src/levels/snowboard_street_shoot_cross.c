#include "levels/snowboard_street_shoot_cross.h"
#include "assets.h"
#include "common.h"
#include "data/course_data.h"
#include "data/data_table.h"
#include "gamestate.h"
#include "gbi.h"
#include "graphics/displaylist.h"
#include "graphics/graphics.h"
#include "math/geometry.h"
#include "race/hit_reactions.h"
#include "race/track_collision.h"
#include "system/task_scheduler.h"

#define Y_OFFSET 0xFFE80000

void renderShootCrossTargets(ShootCrossTargets *arg0);
void updateRotatingSky(SnowboardStreetRotatingSky *rotatingSky);
void cleanupRotatingSky(SnowboardStreetRotatingSky *rotatingSky);
void cleanupShootCrossTargets(ShootCrossTargets *arg0);
void initShootCrossTargetsCallback(ShootCrossTargets *arg0);
void activateShootCrossTargets(ShootCrossTargets *arg0);

extern Gfx D_8009A780_9B380[];
extern Gfx *gDisplayListAllocPtr;

extern s32 gLookAtPtr;
Vtx D_800BBBB0_AD630[4] = {
    { { { -24, 48, 0 }, 0, { -16, -16 }, { 255, 255, 255, 255 } } },
    { { { 24, 48, 0 }, 0, { 2032, -16 }, { 255, 255, 255, 255 } } },
    { { { 24, 0, 0 }, 0, { 2032, 2032 }, { 255, 255, 255, 255 } } },
    { { { -24, 0, 0 }, 0, { -16, 2032 }, { 255, 255, 255, 255 } } },
};

void initShootCrossTargets(ShootCrossTargets *arg0) {
    arg0->spriteTable = loadShootCrossSprites();
    arg0->positionData = loadCompressedData(&targetPositionData_ROM_START, &targetPositionData_ROM_END, 0x160);
    arg0->transformMatrices = NULL;
    setCleanupCallback(cleanupShootCrossTargets);
    setCallback(initShootCrossTargetsCallback);
}

void initShootCrossTargetsCallback(ShootCrossTargets *arg0) {
    s32 i;
    s32 *ptr;
    ShootCrossTarget *targets;
    s32 pad[2];
    s32 offset;
    GameState *gameState;
    ShootCrossTarget *temp;

    gameState = (GameState *)getCurrentAllocation();
    arg0->quadVertices = D_800BBBB0_AD630;
    arg0->targets = (ShootCrossTarget *)((s8 *)arg0->positionData + *arg0->positionData);
    targets = *(ShootCrossTarget *volatile *)&arg0->targets;
    arg0->targetCount = 0;

    if (targets->state >= 0) {
        temp = targets;
        do {
            arg0->targetCount++;
        } while (temp[arg0->targetCount].state >= 0);
    }

    i = 0;
    arg0->transformMatrices = allocateNodeMemory(arg0->targetCount << 6);

    if (arg0->targetCount > 0) {
        ptr = (s32 *)&gScaleMatrix.translation;
        do {
            offset = i << 4;
            *((s8 *)(offset + (s32)arg0->targets)) = 0;
            memcpy(ptr, (s8 *)(offset + (s32)arg0->targets + 4), sizeof(Vec3i));
            transform3DToMtx(ptr - 5, (u8 *)arg0->transformMatrices + (i << 6));
            i++;
        } while (i < arg0->targetCount);
    }

    gameState->shootCrossTargets = arg0;
    setCallback(activateShootCrossTargets);
}

void activateShootCrossTargets(ShootCrossTargets *arg0) {
    s32 i;

    for (i = 0; i < arg0->targetCount; i++) {
        checkPositionPlayerCollisionWithPull(&arg0->targets[i].position, 0x180000, 0x300000);
    }

    for (i = 0; i < 4; i++) {
        enqueueCallbackBySlotIndex((u16)i, 4, renderShootCrossTargets, arg0);
    }
}

void cleanupShootCrossTargets(ShootCrossTargets *arg0) {
    GameState *gameState = (GameState *)getCurrentAllocation();
    gameState->shootCrossTargets = NULL;
    arg0->transformMatrices = freeNodeMemory(arg0->transformMatrices);
    arg0->spriteTable = freeNodeMemory(arg0->spriteTable);
    arg0->positionData = freeNodeMemory(arg0->positionData);
}

s32 checkProjectileTargetHit(Vec3i *projectilePos, s32 hitRange) {
    s32 pos[3];
    s32 unused[2];
    GameState *gameState;
    ShootCrossTargets *targets;
    s32 range;
    s32 negRange;
    s32 i = 0;
    s32 x, y, z;
    s8 *entry;
    s32 yOffset;
    s32 idx;

    gameState = (GameState *)getCurrentAllocation();
    targets = gameState->shootCrossTargets;

    if (targets) {
        goto check_count;
    }

    return 0;

found:
    *entry = 1;
    gameState->shootCrossTargetsHit = gameState->shootCrossTargetsHit + 1;
    playTrickSuccessVoice(gameState->players);
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
        x = pos[0] - targets->targets[i].position.x;
        pos[0] = x;
        y = (pos[1] + yOffset) - targets->targets[i].position.y;
        pos[1] = y;
        z = pos[2] - targets->targets[i].position.z;
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
    gSPDisplayList(gDisplayListAllocPtr++, D_8009A780_9B380);

    for (i = 0; i < arg0->targetCount; i++) {
        offset = i << 4;

        if (isObjectCulled(&arg0->targets[i].position) == 0) {
            s8 textureIndex = arg0->targets[i].state;

            if (prevTextureIndex != textureIndex) {
                prevTextureIndex = textureIndex;
                getTableEntryByU16Index(arg0->spriteTable, (u16)prevTextureIndex, &tableEntry);

                gDPLoadMultiBlock_4b(
                    gDisplayListAllocPtr++,
                    tableEntry.data_ptr,
                    0,
                    G_TX_RENDERTILE,
                    G_IM_FMT_CI,
                    tableEntry.width,
                    tableEntry.height,
                    0,
                    G_TX_CLAMP,
                    G_TX_CLAMP,
                    G_TX_NOMASK,
                    G_TX_NOMASK,
                    G_TX_NOLOD,
                    G_TX_NOLOD
                );

                gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, tableEntry.index_ptr);
            }

            gSPMatrix(gDisplayListAllocPtr++, &arg0->transformMatrices[i], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

            gSPMatrix(gDisplayListAllocPtr++, gLookAtPtr, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);

            gSPVertex(gDisplayListAllocPtr++, arg0->quadVertices, 4, 0);

            gSP2Triangles(gDisplayListAllocPtr++, 0, 3, 2, 0, 2, 1, 0, 0);
        }
    }
}

void scheduleShootCrossTargetsTask(s32 courseId) {
    CourseTaskParams *task;

    task = (CourseTaskParams *)scheduleTask(initShootCrossTargets, 0, 0, 0x32);
    if (task != NULL) {
        task->courseId = courseId;
    }
}

void initRotatingSky(SnowboardStreetRotatingSky *rotatingSky) {
    GameState *gameState;
    DisplayListObject *displayObject;

    gameState = (GameState *)getCurrentAllocation();
    displayObject = &rotatingSky->displayObject;
    displayObject->displayLists = &getSkyDisplayLists3ByIndex(gameState->memoryPoolId)->sceneryDisplayLists1;
    displayObject->segment1 = loadUncompressedAssetByIndex(0xD);
    displayObject->segment2 = loadCompressedSegment2AssetByIndex(0xD);
    displayObject->transform.translation.x = 0x25990000;
    displayObject->transform.translation.y = 0x1A2B0000;
    displayObject->segment3 = NULL;
    displayObject->transform.translation.z = 0xF7A30000;
    setCleanupCallback(cleanupRotatingSky);
    setCallback(updateRotatingSky);
}

void updateRotatingSky(SnowboardStreetRotatingSky *rotatingSky) {
    s32 i;

    rotatingSky->rotationAngle -= 0x20;
    createYRotationMatrix(&rotatingSky->displayObject.transform, rotatingSky->rotationAngle);

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, &rotatingSky->displayObject);
    }
}

void cleanupRotatingSky(SnowboardStreetRotatingSky *rotatingSky) {
    DisplayListObject *displayObject = &rotatingSky->displayObject;

    displayObject->segment1 = freeNodeMemory(displayObject->segment1);
    displayObject->segment2 = freeNodeMemory(displayObject->segment2);
}
