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

typedef struct {
    s16 posMode;             /* 0x00 */
    s16 tgtMode;             /* 0x02 */
    CameraKeyframe *posData; /* 0x04 */
    CameraKeyframe *tgtData; /* 0x08 */
} CameraPath;

Vec3i D_8008FEB0_90AB0 = { 0xFFF00000, 0x00000000, 0x00200000 };

CameraKeyframe D_8008FEBC_90ABC[] = {
    { 0x0000, 0x0000, 0xF165807E, 0x1DA0BFDB, 0x01ABD8F2 },
    { 0x005A, 0x0000, 0xF3AB2D0A, 0x1C3E7396, 0xFFC39E0D },
    { 0x005A, 0x0000, 0xF3AB2D0A, 0x1C3E7396, 0xFFC39E0D },
};

CameraKeyframe D_8008FEEC_90AEC[] = {
    { 0x0000, 0x0000, 0xF2FB7FE5, 0x1C642725, 0x0112CC78 },
    { 0x0017, 0x0000, 0xF3710909, 0x1C5A277C, 0x005B742F },
    { 0x0043, 0x0000, 0xF64B1061, 0x1BB7A7BD, 0xFCE4CE44 },
    { 0x0043, 0x0000, 0xF64B1061, 0x1BB7A7BD, 0xFCE4CE44 },
};

CameraKeyframe D_8008FF2C_90B2C[] = {
    { 0x0000, 0x0000, 0xF2FC1BE5, 0x1C46E925, 0x00D9E278 },
    { 0x0017, 0x0000, 0xF38E7771, 0x1C3CEB24, 0x000CBB3F },
    { 0x0043, 0x0000, 0xF682D489, 0x1BA2EFE5, 0xFCA2A1AC },
    { 0x0043, 0x0000, 0xF682D489, 0x1BA2EFE5, 0xFCA2A1AC },
};

CameraKeyframe D_8008FF6C_90B6C[] = {
    { 0x00B4, 0x0000, 0xF69ED811, 0x1B9CD87D, 0xFC3F1B58 },
};

CameraKeyframe D_8008FF7C_90B7C[] = {
    { 0x0000, 0x0000, 0xF7007AE1, 0x1B77AA1C, 0xFC10AFC0 },
    { 0x0037, 0x0000, 0xF70F4841, 0x1B966EAC, 0xFBECC9D0 },
    { 0x0037, 0x0000, 0xF70F4841, 0x1B966EAC, 0xFBECC9D0 },
};

CameraKeyframe D_8008FFAC_90BAC[] = {
    { 0x0000, 0x0000, 0xF6DC3611, 0x1B9F407C, 0xFC3C0158 },
    { 0x0061, 0x0000, 0xF6DC3611, 0x1B783534, 0xFC3C0158 },
    { 0x0061, 0x0000, 0xF6DC3611, 0x1B783534, 0xFC3C0158 },
};

CameraKeyframe D_8008FFDC_90BDC[] = {
    { 0x0000, 0x0000, 0xFAE8AD06, 0x1A020751, 0xF47058DE },
    { 0x0062, 0x0000, 0xFC48A354, 0x1985029E, 0xF2D0EC6E },
    { 0x0062, 0x0000, 0xFC48A354, 0x1985029E, 0xF2D0EC6E },
};

CameraKeyframe D_8009000C_90C0C[] = {
    { 0x0000, 0x0000, 0xDA8026C3, 0x0A156458, 0xE3EFE99D },
    { 0x0023, 0x0000, 0xD8F04184, 0x091ADC32, 0xE2618680 },
    { 0x0023, 0x0000, 0xD8F04184, 0x091ADC32, 0xE2618680 },
};

CameraKeyframe D_8009003C_90C3C[] = {
    { 0x0000, 0x0000, 0xD8E0DF54, 0x08F61B0A, 0xE1A4BCC0 },
    { 0x0046, 0x0000, 0xD8C7913A, 0x07388681, 0xDD873E1B },
    { 0x0046, 0x0000, 0xD8C7913A, 0x07388681, 0xDD873E1B },
};

CameraKeyframe D_8009006C_90C6C[] = {
    { 0x0000, 0x0000, 0xF28F3E39, 0x1428030C, 0xE9004421 },
    { 0x0034, 0x0000, 0xF01A4F69, 0x1305B077, 0xE8A12BA8 },
    { 0x0034, 0x0000, 0xF01A4F69, 0x1305B077, 0xE8A12BA8 },
};

CameraKeyframe D_8009009C_90C9C[] = {
    { 0x0000, 0x0000, 0xF1B54DE9, 0x14CA0FF7, 0xE8DB8658 },
    { 0x004C, 0x0000, 0xEFA88C41, 0x14475543, 0xE85521A7 },
    { 0x004C, 0x0000, 0xEFA88C41, 0x14475543, 0xE85521A7 },
};

CameraKeyframe D_800900CC_90CCC[] = {
    { 0x0000, 0x0000, 0x0070E766, 0x26736A7C, 0x18A1B778 },
    { 0x000A, 0x0000, 0x0012BC39, 0x265D90CA, 0x184CA6AD },
    { 0x0034, 0x0000, 0xFC1592F5, 0x24A497ED, 0x14E6A5A3 },
    { 0x0034, 0x0000, 0xFC1592F5, 0x24A497ED, 0x14E6A5A3 },
};

CameraKeyframe D_8009010C_90D0C[] = {
    { 0x0000, 0x0000, 0xFC4F38AD, 0x251574E5, 0x159C6A79 },
    { 0x002A, 0x0000, 0xF97DCE2B, 0x23787CF9, 0x126E2072 },
    { 0x002A, 0x0000, 0xF97DCE2B, 0x23787CF9, 0x126E2072 },
};

CameraKeyframe D_8009013C_90D3C[] = {
    { 0x0000, 0x0000, 0xFBDCF44F, 0x247D5E9A, 0x14589B9A },
    { 0x000D, 0x0000, 0xFB1F92F9, 0x241B6C98, 0x139D6C75 },
    { 0x001A, 0x0000, 0xF9C477BF, 0x2381AE1F, 0x1256693D },
    { 0x001A, 0x0000, 0xF9C477BF, 0x2381AE1F, 0x1256693D },
};

CameraKeyframe D_8009017C_90D7C[] = {
    { 0x0000, 0x0000, 0xD7ECFA9B, 0x0408F2CF, 0xD4F59FB1 },
    { 0x0018, 0x0000, 0xD84159C1, 0x03D67D1E, 0xD441FB89 },
    { 0x0018, 0x0000, 0xD84159C1, 0x03D67D1E, 0xD441FB89 },
};

CameraKeyframe D_800901AC_90DAC[] = {
    { 0x0000, 0x0000, 0xD7E192BA, 0x038707A5, 0xD194D367 },
    { 0x012C, 0x0000, 0xD7E192BA, 0x038707A5, 0xD194D367 },
};

CameraPath sCameraPaths[] = {
    { 0, 0, D_8008FEBC_90ABC, NULL             },
    { 0, 1, D_8008FEEC_90AEC, D_8008FF2C_90B2C },
    { 0, 0, D_8008FF6C_90B6C, NULL             },
    { 0, 1, D_8008FF7C_90B7C, D_8008FFAC_90BAC },
    { 0, 2, D_8008FFDC_90BDC, NULL             },
    { 0, 0, D_8009000C_90C0C, NULL             },
    { 0, 0, D_8009003C_90C3C, NULL             },
    { 0, 0, D_8009006C_90C6C, NULL             },
    { 0, 0, D_8009009C_90C9C, NULL             },
    { 0, 3, D_800900CC_90CCC, NULL             },
    { 0, 0, D_8009010C_90D0C, NULL             },
    { 0, 3, D_8009013C_90D3C, NULL             },
    { 0, 0, D_8009017C_90D7C, NULL             },
    { 0, 0, D_800901AC_90DAC, NULL             },
    { 0, 0, NULL,             NULL             },
};

void updateChaseCamera(ChaseCameraState *camera);
void updateOrbitCamera(OrbitCameraState *camera);
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
    memcpy(cameraMatrixPtr, &identityMatrix, sizeof(Transform3D));
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
    memcpy(&rotationMatrix.translation, &identityMatrix.translation, sizeof(Vec3i));
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

void initScriptedCamera(ScriptedCameraState *camera) {
    s8 pathIdx;
    s32 offset;

    pathIdx = camera->pathIndex;
    camera->posKeyframes = sCameraPaths[pathIdx].posData;

    pathIdx = camera->pathIndex;
    camera->tgtKeyframes = sCameraPaths[pathIdx].tgtData;

    pathIdx = camera->pathIndex;
    camera->posMode = sCameraPaths[pathIdx].posMode;

    pathIdx = camera->pathIndex;
    camera->tgtMode = sCameraPaths[pathIdx].tgtMode;

    memcpy(camera, (u8 *)camera->posKeyframes + 4, sizeof(Vec3i));

    camera->posFramesLeft = *(u16 *)camera->posKeyframes;
    if (camera->posFramesLeft == 0) {
        camera->posKeyframes = (CameraKeyframe *)((u8 *)camera->posKeyframes + 0x10);
        camera->posFramesLeft = *(u16 *)camera->posKeyframes;
    }

    if (camera->tgtKeyframes != NULL) {
        memcpy(&camera->targetX, (u8 *)camera->tgtKeyframes + 4, sizeof(Vec3i));
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
            memcpy(&camera->targetX, &allocation->players->worldPos, sizeof(Vec3i));
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
            memcpy(&camera->targetX, &allocation->players->unk1C04, sizeof(Vec3i));
            camera->targetY += 0x200000;
            break;
        case 3:
            memcpy(&camera->targetX, &allocation->players->unk101C, sizeof(Vec3i));
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
