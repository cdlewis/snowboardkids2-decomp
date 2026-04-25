#include "audio/audio.h"
#include "common.h"
#include "gamestate.h"
#include "graphics/graphics.h"
#include "math/geometry.h"
#include "race/track_collision.h"
#include "race/track_height.h"
#include "system/task_scheduler.h"

#define SQUARE(x) (s64) x *x

typedef struct {
    s16 duration; /* 0x00 */
    s16 pad02;    /* 0x02 */
    s32 x;        /* 0x04 */
    s32 y;        /* 0x08 */
    s32 z;        /* 0x0C */
} CameraKeyframe;

typedef struct {
    s32 posX;                     /* 0x00 */
    s32 posY;                     /* 0x04 */
    s32 posZ;                     /* 0x08 */
    s32 targetX;                  /* 0x0C */
    s32 targetY;                  /* 0x10 */
    s32 targetZ;                  /* 0x14 */
    CameraKeyframe *posKeyframes; /* 0x18 */
    CameraKeyframe *tgtKeyframes; /* 0x1C */
    s16 posFramesLeft;            /* 0x20 */
    s16 tgtFramesLeft;            /* 0x22 */
    s8 posMode;                   /* 0x24 */
    s8 tgtMode;                   /* 0x25 */
    s8 pathIndex;                 /* 0x26 */
} ScriptedCameraState;

typedef struct {
    s16 rotationAngle;
} OrbitCameraState;

typedef struct {
    s32 x;                    /* 0x00 */
    s32 y;                    /* 0x04 */
    s32 z;                    /* 0x08 */
    u8 playerIdx;             /* 0x0C */
    u8 viewportIdx;           /* 0x0D */
    u8 padE[0x2];             /* 0x0E */
    s32 distance;             /* 0x10 */
    s32 height;               /* 0x14 */
    s32 targetY;              /* 0x18 */
    s32 minDistance;          /* 0x1C */
    s32 unk20;                /* 0x20 */
    s32 unk24;                /* 0x24 */
    Transform3D lookAtMatrix; /* 0x28 */
} ChaseCameraState;

typedef struct {
    ViewportNode *sceneNode;
    u8 pad4[0xC];
    u8 *playerData;
    u8 pad14[0x65];
    u8 unk79;
} OrbitCameraAllocation;

// Custom allocation struct for use in updateScriptedCamera
typedef struct {
    u8 pad0[0x434];
    Vec3i worldPos; /* 0x434 */
    u8 pad1[0xBCA - 0x440];
    u8 unkBCA; /* 0xBCA */
    u8 pad2[0x101C - 0xBCB];
    Vec3i unk101C; /* 0x101C */
    u8 pad3[0x1C04 - 0x1028];
    Vec3i unk1C04; /* 0x1C04 */
} PlayerDataExt;

typedef struct {
    u8 pad0[0x10];
    PlayerDataExt *players;
} ScriptedCameraAllocation;

void updateChaseCamera(ChaseCameraState *camera);
extern s32 D_8008FEB0_90AB0;
void updateOrbitCamera(OrbitCameraState *camera);
void initScriptedCamera(ScriptedCameraState *camera);
void updateScriptedCamera(ScriptedCameraState *camera);

void initChaseCameraPosition(ChaseCameraState *camera) {
    GameState *gameState;
    u8 playerIdx;
    Vec3i behindOffset;
    Vec3i worldOffset;
    Transform3D rotationMatrix;

    gameState = getCurrentAllocation();

    behindOffset.y = 0;
    behindOffset.x = 0;
    behindOffset.z = 0xFFC00000;

    playerIdx = camera->playerIdx;
    createYRotationMatrix(&rotationMatrix, gameState->players[playerIdx].rotY);

    transformVector2(&behindOffset, &rotationMatrix, &worldOffset);

    playerIdx = camera->playerIdx;
    camera->x = gameState->players[playerIdx].worldPos.x + worldOffset.x;

    playerIdx = camera->playerIdx;
    camera->y = gameState->players[playerIdx].worldPos.y + worldOffset.y;

    playerIdx = camera->playerIdx;
    camera->z = gameState->players[playerIdx].worldPos.z + worldOffset.z;

    camera->distance = 0x600000;
    camera->minDistance = 0x20000;
    camera->targetY = 0x2C0000;
    camera->unk20 = 0;
    camera->unk24 = 0;
    camera->height = 0x1E0000;

    setCallbackWithContinue(updateChaseCamera);
}

void updateChaseCamera(ChaseCameraState *camera) {
    GameState *gs;
    Transform3D rotationMatrix;
    Vec3i behindOffset;
    Vec3i worldOffset;
    s32 diffX;
    s32 diffY;
    s32 diffZ;
    s32 dist;
    s32 targetDist;
    s32 maxDist;
    s32 heightDiff;
    s32 trackHeight;
    Vec3i *temp;

    gs = getCurrentAllocation();

    if (gs->gamePaused != 0) {
        return;
    }

    if (gs->players[camera->playerIdx].finishAnimState != 0) {
        behindOffset.y = 0;
        behindOffset.x = 0;
        behindOffset.z = 0xFFC00000;

        if (gs->players[camera->playerIdx].finishAnimState == 2) {
            createYRotationMatrix(&rotationMatrix, 0x1000);
        } else {
            createYRotationMatrix(&rotationMatrix, gs->players[camera->playerIdx].rotY);
        }
        transformVector2(&behindOffset, &rotationMatrix, &worldOffset);

        camera->x = gs->players[camera->playerIdx].worldPos.x + worldOffset.x;
        camera->y = gs->players[camera->playerIdx].worldPos.y + worldOffset.y;
        camera->z = gs->players[camera->playerIdx].worldPos.z + worldOffset.z;
        gs->players[camera->playerIdx].finishAnimState = 0;
    }

    diffY = gs->players[camera->playerIdx].worldPos.y - camera->y;
    if (diffY >= 0x4001) {
        diffY = 0x4000;
    }
    if (diffY < -0x4000) {
        diffY = -0x4000;
    }
    camera->y += diffY;

    if ((u8)gs->showGoldReward != 0) {
        if (camera->minDistance <= 0x1FFFF) {
            camera->minDistance = camera->minDistance + 0x1000;
        }

        camera->distance += (0x960000 - camera->distance) / 0x20;
        camera->height += (0x600000 - camera->height) / 0x20;

        transformVector2(&camera->minDistance, &camera->lookAtMatrix, &behindOffset);

        camera->x += behindOffset.x;
        camera->z += behindOffset.z;

        if (camera->targetY != 0x400000) {
            camera->targetY += (0x400000 - camera->targetY) / 0x20;
        }
    } else {
        camera->distance += (0x600000 - camera->distance) / 0x20;

        if (gs->players[camera->playerIdx].animFlags & 0x1000) {
            heightDiff = 0x1A0000 - camera->targetY;
            if (heightDiff >= 0x6001) {
                heightDiff = 0x6000;
            }
            if (heightDiff < -0x6000) {
                heightDiff = -0x6000;
            }
            camera->targetY += heightDiff;
        } else {
            heightDiff = 0x2C0000 - camera->targetY;
            if (heightDiff >= 0x3001) {
                heightDiff = 0x3000;
            }
            if (heightDiff < -0x3000) {
                heightDiff = -0x3000;
            }
            camera->targetY += heightDiff;
        }
    }

    diffX = camera->x - gs->players[camera->playerIdx].worldPos.x;
    diffZ = camera->z - gs->players[camera->playerIdx].worldPos.z;

    dist = isqrt64(SQUARE(diffX) + SQUARE(diffZ));

    targetDist = camera->height;
    if (dist < targetDist) {
        if (dist == 0) {
            diffZ = 1;
            dist = 1;
        }
        diffX = (s64)diffX * targetDist / dist;
        diffZ = (s64)diffZ * targetDist / dist;

        camera->x = diffX + gs->players[camera->playerIdx].worldPos.x;
        camera->z = diffZ + gs->players[camera->playerIdx].worldPos.z;
    }

    diffX = camera->x - gs->players[camera->playerIdx].worldPos.x;
    diffY = camera->y - gs->players[camera->playerIdx].worldPos.y;
    diffZ = camera->z - gs->players[camera->playerIdx].worldPos.z;

    dist = isqrt64(SQUARE(diffX) + SQUARE(diffY) + SQUARE(diffZ));

    maxDist = camera->distance + 0x10000;
    if (maxDist < dist) {
        diffX = (s64)diffX * maxDist / dist;
        diffY = (s64)diffY * maxDist / dist;
        diffZ = (s64)diffZ * maxDist / dist;

        camera->x = diffX + gs->players[camera->playerIdx].worldPos.x;
        camera->y = diffY + gs->players[camera->playerIdx].worldPos.y;
        camera->z = diffZ + gs->players[camera->playerIdx].worldPos.z;
    }

    if (dist < camera->distance) {
        diffX = (s64)diffX * camera->distance / dist;
        diffY = (s64)diffY * camera->distance / dist;
        diffZ = (s64)diffZ * camera->distance / dist;

        camera->x = diffX + gs->players[camera->playerIdx].worldPos.x;
        camera->y = diffY + gs->players[camera->playerIdx].worldPos.y;
        camera->z = diffZ + gs->players[camera->playerIdx].worldPos.z;
    }

    computeLookAtMatrix((Vec3i *)camera, &gs->players[camera->playerIdx].worldPos, &camera->lookAtMatrix);

    diffX = (s64)camera->lookAtMatrix.m[1][0] * camera->targetY / 0x2000;
    diffY = (s64)camera->lookAtMatrix.m[1][1] * camera->targetY / 0x2000;
    diffZ = (s64)camera->lookAtMatrix.m[1][2] * camera->targetY / 0x2000;

    camera->lookAtMatrix.translation.x = camera->lookAtMatrix.translation.x + diffX;
    camera->lookAtMatrix.translation.y = camera->lookAtMatrix.translation.y + diffY;
    camera->lookAtMatrix.translation.z = diffZ + camera->lookAtMatrix.translation.z;

    if (!(gs->players[camera->playerIdx].animFlags & 0x200)) {
        trackHeight = getOrUpdatePlayerSectorIndex(
            &gs->players[camera->playerIdx],
            &gs->gameData,
            gs->players[camera->playerIdx].sectorIndex,
            camera
        );
        trackHeight =
            getTrackHeightInSector(&gs->gameData, trackHeight & 0xFFFF, &camera->lookAtMatrix.translation, 0x100000) +
            0xA0000;
        if (camera->lookAtMatrix.translation.y < trackHeight) {
            camera->y += trackHeight - camera->lookAtMatrix.translation.y;
            computeLookAtMatrix((Vec3i *)camera, &gs->players[camera->playerIdx].worldPos, &camera->lookAtMatrix);

            diffX = (s64)camera->lookAtMatrix.m[1][0] * camera->targetY / 0x2000;
            diffY = (s64)camera->lookAtMatrix.m[1][1] * camera->targetY / 0x2000;
            diffZ = (s64)camera->lookAtMatrix.m[1][2] * camera->targetY / 0x2000;

            camera->lookAtMatrix.translation.x += diffX;
            camera->lookAtMatrix.translation.y += diffY;
            camera->lookAtMatrix.translation.z += diffZ;
        }
    }

    if ((gs->players[camera->playerIdx].inputButtons & 0x10) && !(gs->players[camera->playerIdx].animFlags & 0x100)) {
        camera->lookAtMatrix.m[0][0] = -camera->lookAtMatrix.m[0][0];
        camera->lookAtMatrix.m[0][1] = -camera->lookAtMatrix.m[0][1];
        camera->lookAtMatrix.m[0][2] = -camera->lookAtMatrix.m[0][2];
        camera->lookAtMatrix.m[2][0] = -camera->lookAtMatrix.m[2][0];
        camera->lookAtMatrix.m[2][1] = -camera->lookAtMatrix.m[2][1];
        camera->lookAtMatrix.m[2][2] = -camera->lookAtMatrix.m[2][2];

        camera->lookAtMatrix.translation.x = gs->players[camera->playerIdx].worldPos.x;
        camera->lookAtMatrix.translation.y = gs->players[camera->playerIdx].worldPos.y + 0x300000;
        camera->lookAtMatrix.translation.z = gs->players[camera->playerIdx].worldPos.z;
    }

    temp = &rotationMatrix.translation;
    setViewportTransformById(camera->viewportIdx + 0x64, &camera->lookAtMatrix);
    if (gs->players == gs->players) {}
    memcpy(temp, (void *)(camera->playerIdx * (s32)sizeof(Player) + (s32)gs->players + 0x434), sizeof(Vec3i));

    if (gs->players[camera->viewportIdx].unkBCA == 0) {
        queueBufferDataNoFlags((u8 *)&camera->lookAtMatrix, (s8)camera->viewportIdx);
    } else {
        setBufferData(&camera->lookAtMatrix, 0x30, (s32)(s8)camera->viewportIdx);
    }
}

void spawnChaseCameraTask(u8 playerIdx) {
    Node *node = scheduleTask(initChaseCameraPosition, 1, 0, 0xD2);
    if (node != NULL) {
        node->unkC = playerIdx;
        node->field_D = playerIdx;
    }
}

void initOrbitCamera(OrbitCameraState *camera) {
    getCurrentAllocation();
    camera->rotationAngle = 0;
    setCallbackWithContinue(updateOrbitCamera);
}

void updateOrbitCamera(OrbitCameraState *camera) {
    OrbitCameraAllocation *allocation;
    Transform3D cameraMatrix;
    Transform3D rotationMatrix;
    Transform3D tempMatrix;
    Vec3i translationOffset;
    s16 newAngle;
    s16 currentAngle;
    Transform3D *cameraMatrixPtr;

    allocation = getCurrentAllocation();
    cameraMatrixPtr = &cameraMatrix;
    memcpy(cameraMatrixPtr, &identityMatrix, 0x20);
    cameraMatrix.translation.y = 0x1E0000;
    cameraMatrix.translation.z = 0x870000;
    newAngle = camera->rotationAngle + 0x58;
    camera->rotationAngle = newAngle;
    if (newAngle == 0x12E8) {
        setViewportEnvColor(allocation->sceneNode, 0xFF, 0xFF, 0xFF);
        setViewportFadeValue(allocation->sceneNode, 0xFF, 0x20);
    }
    currentAngle = camera->rotationAngle;
    if (currentAngle == 0x1EF0) {
        allocation->unk79 = allocation->unk79 - 1;
        terminateCurrentTask();
        setViewportFadeValue(allocation->sceneNode, 0, 0x10);
        return;
    }
    createYRotationMatrix(&rotationMatrix, (currentAngle + 0x1000) & 0xFFFF);
    memcpy(&rotationMatrix.translation, &identityMatrix.translation, 0xC);
    func_8006B084_6BC84(cameraMatrixPtr, &rotationMatrix, &tempMatrix);
    func_8006B084_6BC84(&tempMatrix, (Transform3D *)(allocation->playerData + 0x950), cameraMatrixPtr);
    transformVector2(&D_8008FEB0_90AB0, allocation->playerData + 0x950, &translationOffset);
    cameraMatrix.translation.x = cameraMatrix.translation.x + translationOffset.x;
    cameraMatrix.translation.y = cameraMatrix.translation.y + translationOffset.y;
    cameraMatrix.translation.z = cameraMatrix.translation.z + translationOffset.z;
    setViewportTransformById(0x64, cameraMatrixPtr);
}

void spawnOrbitCameraTask(void) {
    scheduleTask(initOrbitCamera, 1, 0, 0xF0);
}

extern u8 sCameraPathPosMode[];
extern u8 sCameraPathTgtMode[];
extern u8 sCameraPathPosData[];
extern u8 sCameraPathTgtData[];

void initScriptedCamera(ScriptedCameraState *camera) {
    s8 pathIdx;
    s32 offset;

    pathIdx = camera->pathIndex;
    offset = pathIdx * 12;
    camera->posKeyframes = *(void **)(sCameraPathPosData + offset);

    pathIdx = camera->pathIndex;
    offset = pathIdx * 12;
    camera->tgtKeyframes = *(void **)(sCameraPathTgtData + offset);

    pathIdx = camera->pathIndex;
    offset = pathIdx * 12;
    camera->posMode = *(u16 *)(sCameraPathPosMode + offset);

    pathIdx = camera->pathIndex;
    offset = pathIdx * 12;
    camera->tgtMode = *(u16 *)(sCameraPathTgtMode + offset);

    memcpy(camera, (u8 *)camera->posKeyframes + 4, 0xC);

    camera->posFramesLeft = *(u16 *)camera->posKeyframes;
    if (camera->posFramesLeft == 0) {
        camera->posKeyframes = (CameraKeyframe *)((u8 *)camera->posKeyframes + 0x10);
        camera->posFramesLeft = *(u16 *)camera->posKeyframes;
    }

    if (camera->tgtKeyframes != NULL) {
        memcpy(&camera->targetX, (u8 *)camera->tgtKeyframes + 4, 0xC);
        camera->tgtFramesLeft = *(u16 *)camera->tgtKeyframes;
        if (camera->tgtFramesLeft == 0) {
            camera->tgtKeyframes = (CameraKeyframe *)((u8 *)camera->tgtKeyframes + 0x10);
            camera->tgtFramesLeft = *(u16 *)camera->tgtKeyframes;
        }
    }

    setCallbackWithContinue(updateScriptedCamera);
}

void updateScriptedCamera(ScriptedCameraState *camera) {
    ScriptedCameraAllocation *allocation;
    u8 cameraMatrix[0x30];

    allocation = getCurrentAllocation();

    if (camera->posMode == 0) {
        camera->posX += ((camera->posKeyframes->x - camera->posX) / camera->posFramesLeft);
        camera->posY += ((camera->posKeyframes->y - camera->posY) / camera->posFramesLeft);
        camera->posZ += ((camera->posKeyframes->z - camera->posZ) / camera->posFramesLeft);
        camera->posFramesLeft--;
        if (camera->posFramesLeft == 0) {
            camera->posKeyframes = (CameraKeyframe *)((u8 *)camera->posKeyframes + 0x10);
            camera->posFramesLeft = *(s16 *)camera->posKeyframes;
        }
    }

    switch (camera->tgtMode) {
        case 0:
            memcpy(&camera->targetX, &allocation->players->worldPos, 0xC);
            camera->targetY += 0x200000;
            break;
        case 1:
            camera->targetX += ((camera->tgtKeyframes->x - camera->targetX) / camera->tgtFramesLeft);
            camera->targetY += ((camera->tgtKeyframes->y - camera->targetY) / camera->tgtFramesLeft);
            camera->targetZ += ((camera->tgtKeyframes->z - camera->targetZ) / camera->tgtFramesLeft);
            camera->tgtFramesLeft--;
            if (camera->tgtFramesLeft == 0) {
                camera->tgtKeyframes = (CameraKeyframe *)((u8 *)camera->tgtKeyframes + 0x10);
                camera->tgtFramesLeft = *(s16 *)camera->tgtKeyframes;
            }
            break;
        case 2:
            memcpy(&camera->targetX, &allocation->players->unk1C04, 0xC);
            camera->targetY += 0x200000;
            break;
        case 3:
            memcpy(&camera->targetX, &allocation->players->unk101C, 0xC);
            camera->targetY += 0x200000;
            break;
    }

    computeLookAtMatrix((Vec3i *)camera, (Vec3i *)&camera->targetX, (Transform3D *)cameraMatrix);
    setViewportTransformById(0x64, cameraMatrix);

    if (allocation->players->unkBCA == 0) {
        queueBufferDataNoFlags(cameraMatrix, 0);
    } else {
        setBufferData(cameraMatrix, 0x30, 0);
    }
}

void spawnScriptedCameraTask(u8 pathIndex) {
    ScriptedCameraState *task = (ScriptedCameraState *)scheduleTask(initScriptedCamera, 0, 2, 0xF0);
    if (task != NULL) {
        task->pathIndex = pathIndex;
    }
}
