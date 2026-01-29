#include "6E840.h"
#include "common.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
#include "task_scheduler.h"

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
    s32 x;           /* 0x00 */
    s32 y;           /* 0x04 */
    s32 z;           /* 0x08 */
    u8 playerIdx;    /* 0x0C */
    u8 padD[0x3];    /* 0x0D */
    s32 distance;    /* 0x10 */
    s32 height;      /* 0x14 */
    s32 targetY;     /* 0x18 */
    s32 minDistance; /* 0x1C */
    s32 unk20;       /* 0x20 */
    s32 unk24;       /* 0x24 */
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

extern void func_8003C2BC_3CEBC(void);
extern s32 D_8008FEB0_90AB0;
extern u8 identityMatrix[];
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

    setCallbackWithContinue(func_8003C2BC_3CEBC);
}

INCLUDE_ASM("asm/nonmatchings/3CD70", func_8003C2BC_3CEBC);

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
    memcpy(cameraMatrixPtr, identityMatrix, 0x20);
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
    memcpy(&rotationMatrix.translation, &((Transform3D *)identityMatrix)->translation, 0xC);
    func_8006B084_6BC84(cameraMatrixPtr, &rotationMatrix, &tempMatrix);
    func_8006B084_6BC84(&tempMatrix, allocation->playerData + 0x950, cameraMatrixPtr);
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

    computeLookAtMatrix(camera, &camera->targetX, cameraMatrix);
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
