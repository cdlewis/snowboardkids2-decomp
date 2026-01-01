#include "6E840.h"
#include "common.h"
#include "geometry.h"
#include "graphics.h"
#include "task_scheduler.h"

typedef struct {
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
} CameraData;

typedef struct {
    s32 unk0;          /* 0x00 */
    s32 unk4;          /* 0x04 */
    s32 unk8;          /* 0x08 */
    s32 unkC;          /* 0x0C */
    s32 unk10;         /* 0x10 */
    s32 unk14;         /* 0x14 */
    CameraData *unk18; /* 0x18 */
    CameraData *unk1C; /* 0x1C */
    s16 unk20;         /* 0x20 */
    s16 unk22;         /* 0x22 */
    s8 unk24;          /* 0x24 */
    s8 unk25;          /* 0x25 */
} func_8003D210_3DE10_arg;

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
    u8 padding[0x434];
    void *unk434;
    u8 padding1[0xBCA - 0x438];
    u8 unkBCA;
    u8 padding2[0x101C - 0xBCC];
    void *unk101C;
    u8 padding3[0x1C04 - 0x1020];
    void *unk1C04;
} AllocationUnk10;

typedef struct {
    u8 pad0[0x10];
    /* 0x10 */ AllocationUnk10 *players;
} Allocation;

typedef struct {
    Node_70B00 *sceneNode;
    u8 pad4[0xC];
    u8 *playerData;
    u8 pad14[0x65];
    u8 unk79;
} OrbitCameraAllocation;

extern void func_8003C2BC_3CEBC(void);
extern s32 D_8008FEB0_90AB0;
extern u8 identityMatrix[];
void updateOrbitCamera(OrbitCameraState *camera);
void func_8003D0F4_3DCF4(NodeExt *arg0);
void func_8003D210_3DE10(func_8003D210_3DE10_arg *);

void initChaseCameraPosition(ChaseCameraState *camera) {
    Allocation *gameState;
    u8 playerIdx;
    Vec3i behindOffset;
    Vec3i worldOffset;
    Transform3D rotationMatrix;

    gameState = (Allocation *)getCurrentAllocation();

    behindOffset.y = 0;
    behindOffset.x = 0;
    behindOffset.z = 0xFFC00000;

    playerIdx = camera->playerIdx;

    createYRotationMatrix(&rotationMatrix, *(u16 *)((u8 *)gameState->players + (playerIdx * 0xBE8) + 0xA94));

    transformVector2(&behindOffset, &rotationMatrix, &worldOffset);

    playerIdx = camera->playerIdx;

    camera->x = *(s32 *)((u8 *)gameState->players + (playerIdx * 0xBE8) + 0x434) + worldOffset.x;

    playerIdx = camera->playerIdx;

    camera->y = *(s32 *)((u8 *)gameState->players + (playerIdx * 0xBE8) + 0x438) + worldOffset.y;

    playerIdx = camera->playerIdx;

    camera->z = *(s32 *)((u8 *)gameState->players + (playerIdx * 0xBE8) + 0x43C) + worldOffset.z;

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
        func_8006FE28_70A28(allocation->sceneNode, 0xFF, 0xFF, 0xFF);
        func_8006FDA0_709A0(allocation->sceneNode, 0xFF, 0x20);
    }
    currentAngle = camera->rotationAngle;
    if (currentAngle == 0x1EF0) {
        allocation->unk79 = allocation->unk79 - 1;
        func_80069CF8_6A8F8();
        func_8006FDA0_709A0(allocation->sceneNode, 0, 0x10);
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
    func_8006FD3C_7093C(0x64, cameraMatrixPtr);
}

void spawnOrbitCameraTask(void) {
    scheduleTask(initOrbitCamera, 1, 0, 0xF0);
}

extern u8 D_800901CC_90DCC[];
extern u8 D_800901CE_90DCE[];
extern u8 D_800901D0_90DD0[];
extern u8 D_800901D4_90DD4[];

void func_8003D0F4_3DCF4(NodeExt *arg0) {
    s8 temp_v1;
    s32 offset;

    temp_v1 = arg0->unk26;
    offset = temp_v1 * 12;
    arg0->unk18 = *(void **)(D_800901D0_90DD0 + offset);

    temp_v1 = arg0->unk26;
    offset = temp_v1 * 12;
    arg0->unk1C = *(void **)(D_800901D4_90DD4 + offset);

    temp_v1 = arg0->unk26;
    offset = temp_v1 * 12;
    arg0->unk24 = *(u16 *)(D_800901CC_90DCC + offset);

    temp_v1 = arg0->unk26;
    offset = temp_v1 * 12;
    arg0->unk25 = *(u16 *)(D_800901CE_90DCE + offset);

    memcpy(arg0, (u8 *)arg0->unk18 + 4, 0xC);

    arg0->unk20 = *(u16 *)arg0->unk18;
    if (arg0->unk20 == 0) {
        arg0->unk18 = (u8 *)arg0->unk18 + 0x10;
        arg0->unk20 = *(u16 *)arg0->unk18;
    }

    if (arg0->unk1C != NULL) {
        memcpy(&arg0->unkC, (u8 *)arg0->unk1C + 4, 0xC);
        arg0->unk22 = *(u16 *)arg0->unk1C;
        if (arg0->unk22 == 0) {
            arg0->unk1C = (u8 *)arg0->unk1C + 0x10;
            arg0->unk22 = *(u16 *)arg0->unk1C;
        }
    }

    setCallbackWithContinue(func_8003D210_3DE10);
}

void func_8003D210_3DE10(func_8003D210_3DE10_arg *arg0) {
    Allocation *allocation;
    u8 sp10[0x30];

    allocation = getCurrentAllocation();

    if (arg0->unk24 == 0) {
        arg0->unk0 += ((arg0->unk18->unk8 - arg0->unk0) / arg0->unk20);
        arg0->unk4 += ((arg0->unk18->unkC - arg0->unk4) / arg0->unk20);
        arg0->unk8 += ((arg0->unk18->unk10 - arg0->unk8) / arg0->unk20);
        arg0->unk20--;
        if (arg0->unk20 == 0) {
            arg0->unk18 = (CameraData *)((u8 *)arg0->unk18 + 0x10);
            arg0->unk20 = *(s16 *)arg0->unk18;
        }
    }

    switch (arg0->unk25) {
        case 0:
            memcpy(&arg0->unkC, &allocation->players->unk434, 0xC);
            arg0->unk10 += 0x200000;
            break;
        case 1:
            arg0->unkC += ((arg0->unk1C->unk8 - arg0->unkC) / arg0->unk22);
            arg0->unk10 += ((arg0->unk1C->unkC - arg0->unk10) / arg0->unk22);
            arg0->unk14 += ((arg0->unk1C->unk10 - arg0->unk14) / arg0->unk22);
            arg0->unk22--;
            if (arg0->unk22 == 0) {
                arg0->unk1C = (CameraData *)((u8 *)arg0->unk1C + 0x10);
                arg0->unk22 = *(s16 *)arg0->unk1C;
            }
            break;
        case 2:
            memcpy(&arg0->unkC, &allocation->players->unk1C04, 0xC);
            arg0->unk10 += 0x200000;
            break;
        case 3:
            memcpy(&arg0->unkC, &allocation->players->unk101C, 0xC);
            arg0->unk10 += 0x200000;
            break;
    }

    computeLookAtMatrix(arg0, &arg0->unkC, sp10);
    func_8006FD3C_7093C(0x64, sp10);

    if (allocation->players->unkBCA == 0) {
        func_800569A4_575A4(sp10, 0);
    } else {
        setBufferData(sp10, 0x30, 0);
    }
}

void func_8003D51C_3E11C(u8 arg0) {
    NodeExt *task = (NodeExt *)scheduleTask(func_8003D0F4_3DCF4, 0, 2, 0xF0);
    if (task != NULL) {
        task->unk26 = arg0;
    }
}
