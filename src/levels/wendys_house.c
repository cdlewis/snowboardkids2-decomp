#include "levels/wendys_house.h"
#include "../race_session.h"
#include "56910.h"
#include "5AA90.h"
#include "audio.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "rand.h"
#include "task_scheduler.h"

extern Vec3i gWendysHouseProjectileTargetPositions[];

typedef struct {
    u8 _pad[0x5C];
    u8 memoryPoolId;
} AllocB5668;

typedef struct {
    u8 _pad0[0x14];
    Vec3i position;
    void *displayLists;
    void *uncompressedAsset;
    void *compressedAsset;
    s32 animTimer;
    u8 _pad1[0xC];
    Vec3i velocity;
    u16 rotX;
    u16 rotY;
    s16 targetIndex;
    s16 projectileState;
} WendysHouseProjectileTaskState;

typedef struct {
    u8 _pad[0x24];
    void *uncompressedAsset;
    void *compressedAsset;
} TaskAssetState;

typedef struct {
    s16 matrix[6];
    u8 _pad[0x8];
    s32 posX;
    s32 posY;
    s32 posZ;
    void *displayLists;
    void *uncompressedAsset;
    void *compressedAsset;
    s32 unk2C;
    u8 _pad2[0xC];
    s16 oscillationAngle;
    u8 _pad3[0x2];
    s32 localOffsetX;
    s32 localOffsetZ;
    s32 unk48;
} RotatingPlatformTaskState;

typedef struct {
    u8 pad[0x4C];
    s16 unk4C;
} Task;

void updateWendysHouseProjectileSpawner(WendysHouseProjectileSpawnerState *arg0);
void updateWendysHouseProjectileTask(WendysHouseProjectileTaskState *arg0);
void initWendysHouseProjectileTask(WendysHouseProjectileTaskState *arg0);
void cleanupWendysHouseProjectileTask(TaskAssetState *arg0);
void cleanupRotatingPlatformTask(TaskAssetState *arg0);
void updateRotatingPlatformTask(RotatingPlatformTaskState *arg0);

void initRotatingPlatformTask(RotatingPlatformTaskState *arg0) {
    s32 pad[3];
    GameState *gameState = getCurrentAllocation();

    arg0->displayLists = (void *)((u32)getSkyDisplayLists3ByIndex(gameState->memoryPoolId) + 0xA0);
    arg0->uncompressedAsset = loadUncompressedAssetByIndex(gameState->memoryPoolId);
    arg0->compressedAsset = loadCompressedSegment2AssetByIndex(gameState->memoryPoolId);
    arg0->posX = 0x03E90000;
    arg0->posY = 0x1D500000;
    arg0->posZ = 0xF8460000;
    arg0->unk2C = 0;
    arg0->oscillationAngle = 0;
    arg0->unk48 = 0;
    arg0->localOffsetX = 0;
    arg0->localOffsetZ = 0xFE6A0000;
    setCleanupCallback(cleanupRotatingPlatformTask);
    setCallback(updateRotatingPlatformTask);
}

void updateRotatingPlatformTask(RotatingPlatformTaskState *arg0) {
    s32 i;
    Vec3i temp;
    GameState *gameState = getCurrentAllocation();

    if (gameState->gamePaused == 0) {
        arg0->oscillationAngle += 0x40;
    }

    createRotationMatrixYZ(arg0->matrix, (u16)(approximateSin(arg0->oscillationAngle) >> 4), 0xF800);
    transformVector((s16 *)&arg0->localOffsetX, arg0->matrix, &temp);
    handleOrientedAreaCollision(&temp, 0x500000, 0x100000, -0x800);

    if (gameState->gamePaused == 0) {
        if ((arg0->oscillationAngle & 0xFFF) == 0) {
            queueSoundAtPosition(&temp, 0x27);
        }
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void cleanupRotatingPlatformTask(TaskAssetState *arg0) {
    arg0->uncompressedAsset = freeNodeMemory(arg0->uncompressedAsset);
    arg0->compressedAsset = freeNodeMemory(arg0->compressedAsset);
}

void initWendysHouseProjectileTask(WendysHouseProjectileTaskState *arg0) {
    AllocB5668 *gameState;
    void *temp;
    s32 randVal;
    s32 diff;
    gameState = (AllocB5668 *)getCurrentAllocation();
    arg0->uncompressedAsset = loadUncompressedAssetByIndex(gameState->memoryPoolId);
    arg0->compressedAsset = loadCompressedSegment2AssetByIndex(gameState->memoryPoolId);
    arg0->animTimer = 0;
    temp = getSkyDisplayLists3ByIndex(gameState->memoryPoolId);
    randVal = (randA() & 1) << 4;
    arg0->displayLists = temp + ((randVal) + 0xB0);
    arg0->position.x = 0x225BCB0C;
    arg0->position.y = gWendysHouseProjectileTargetPositions[((s16)arg0->targetIndex)].y + 0x1E8000;
    arg0->position.z = 0xF14F9599;
    diff = gWendysHouseProjectileTargetPositions[arg0->targetIndex].x - arg0->position.x;
    if (diff < 0) {
        diff += 0x3F;
    }
    arg0->velocity.x = diff >> 6;
    diff = gWendysHouseProjectileTargetPositions[arg0->targetIndex].z - arg0->position.z;
    if (diff < 0) {
        diff += 0x3F;
    }
    arg0->velocity.z = 6;
    arg0->velocity.z = diff >> arg0->velocity.z;
    arg0->velocity.y = 0x180000;
    arg0->rotX = atan2Fixed(-arg0->velocity.x, -arg0->velocity.z);
    arg0->projectileState = 0;
    setCleanupCallback(cleanupWendysHouseProjectileTask);
    setCallback(updateWendysHouseProjectileTask);
}

void updateWendysHouseProjectileTask(WendysHouseProjectileTaskState *arg0) {
    GameState *gameState;
    s32 i;

    gameState = getCurrentAllocation();

    if (gameState->gamePaused != 0) {
        goto end;
    }

    switch (arg0->projectileState) {
        case 0:
            arg0->position.x += arg0->velocity.x;
            arg0->position.y += arg0->velocity.y;
            arg0->position.z += arg0->velocity.z;
            arg0->velocity.y += (s32)0xFFFF4000;
            if (arg0->velocity.y < (s32)0xFFE80000) {
                arg0->velocity.y = 0xC0000;
                arg0->projectileState += 1;
                queueSoundAtPosition(&arg0->position, 0x28);
            }
            arg0->rotY += 0x100;
            for (i = 0; i < gameState->numPlayers; i++) {
                if (isPointInPlayerCollisionSphere(&gameState->players[i], (Vec3i *)&arg0->position, 0x180000) != 0) {
                    setPlayerBouncedBackState(&gameState->players[i]);
                }
            }
            break;
        case 1:
            arg0->position.x += arg0->velocity.x;
            arg0->position.y += arg0->velocity.y;
            arg0->position.z += arg0->velocity.z;
            for (i = 0; i < gameState->numPlayers; i++) {
                isPlayerInRangeAndPull((Vec3i *)&arg0->position, 0x180000, &gameState->players[i]);
            }
            arg0->velocity.y += (s32)0xFFFF4000;
            if (arg0->velocity.y < (s32)0xFFE20000) {
                terminateCurrentTask();
            }
            arg0->rotY += 0x100;
            break;
    }

end:
    createCombinedRotationMatrix(arg0, arg0->rotY, arg0->rotX);
    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void cleanupWendysHouseProjectileTask(TaskAssetState *arg0) {
    arg0->uncompressedAsset = freeNodeMemory(arg0->uncompressedAsset);
    arg0->compressedAsset = freeNodeMemory(arg0->compressedAsset);
}

void initWendysHouseProjectileSpawner(WendysHouseProjectileSpawnerState *arg0) {
    arg0->unk2 = 0x50;
    arg0->unk0 = 0;
    arg0->unk4 = 0;
    arg0->unk6 = 0x28;
    setCallback(updateWendysHouseProjectileSpawner);
}

void updateWendysHouseProjectileSpawner(WendysHouseProjectileSpawnerState *arg0) {
    Task *task;
    GameState *gameState = getCurrentAllocation();

    if (gameState->gamePaused == 0) {
        if (arg0->unk2 == 0) {
            arg0->unk0++;
            if (arg0->unk0 == 3) {
                arg0->unk0 = 0;
            }
            task = scheduleTask(initWendysHouseProjectileTask, 0, 0, 0x5F);
            if (task != NULL) {
                task->unk4C = arg0->unk0;
            }
            arg0->unk2 = 0x50;
        } else {
            arg0->unk2--;
        }

        if (arg0->unk6 == 0) {
            arg0->unk4++;
            if (arg0->unk4 == 3) {
                arg0->unk4 = 0;
            }
            task = scheduleTask(initWendysHouseProjectileTask, 0, 0, 0x5F);
            if (task != NULL) {
                task->unk4C = arg0->unk4 + 3;
            }
            arg0->unk6 = 0x50;
        } else {
            arg0->unk6--;
        }
    }
}
