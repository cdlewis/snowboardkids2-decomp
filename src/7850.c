#include "20F0.h"
#include "90F0.h"
#include "common.h"
#include "geometry.h"
#include "memory_allocator.h"
#include "task_scheduler.h"

typedef struct {
    u8 _pad[0x16];
    u16 unk16;
} PulsingSpriteOwnerData;

typedef struct {
    u8 _pad[0x10];
    PulsingSpriteOwnerData *ownerData;
    u8 _pad2[0x18];
    s32 posX;
    s32 posY;
    s32 posZ;
    u8 _pad4[0x4];
    s8 isDestroyed;
    s8 actionMode;
    u8 _pad5;
    s8 displayEnabled;
    u8 _pad6[0x48];
    s8 unk88;
} PulsingSpriteOwner;

typedef struct {
    PulsingSpriteOwner *owner;
    SpriteAssetState spriteState;
    u8 padding[4];
    s32 scale;
    s32 scaleVelocity;
} PulsingSpriteState;

typedef struct {
    u8 _pad[0x18];
    u8 transformMatrix[0x20];
    u8 _pad2[0x4];
    s8 isDestroyed;
} StretchingModelOwner;

typedef struct {
    void *owner;
    u8 transformMatrix[0x18];
    s32 unk1C;
    u8 padding[0x4];
    void *unk24;
    MemoryAllocatorNode *displayList;
    MemoryAllocatorNode *vertexData;
    s32 unk30;
    u8 padding2[0xC];
    s16 rotationAngle;
    s16 stretchAngle;
} StretchingModelTaskState;

void cleanupStretchingModelTask(StretchingModelTaskState *);
void func_8000A190_AD90(s32 *ptr, u16 arg1, s32 x, s32 y, s32 z, s32 scaleX, s32 scaleY, u8 arg7, u8 arg8, u8 arg9);

void updatePulsingSpriteIndicator(PulsingSpriteState *);
void cleanupPulsingSpriteIndicator(PulsingSpriteState *);
void initStretchingModelTask(StretchingModelTaskState *);
void updateStretchingModelTask(StretchingModelTaskState *);
extern void *D_80088720_89320;
extern u8 identityMatrix[];

void initPulsingSpriteIndicator(PulsingSpriteState *arg0) {
    SpriteAssetState *temp_s0;

    temp_s0 = &arg0->spriteState;
    arg0->scale = 0xC80000;
    arg0->scaleVelocity = 0x80000;
    loadSpriteAsset(temp_s0, 7);
    setSpriteAnimation(temp_s0, 0x10000, 0, -1);
    setCleanupCallback(&cleanupPulsingSpriteIndicator);
    setCallback(&updatePulsingSpriteIndicator);
}

void updatePulsingSpriteIndicator(PulsingSpriteState *arg0) {
    s8 sp28[2];
    PulsingSpriteOwner *ownerRef;
    PulsingSpriteOwner *owner;
    s32 x;
    s32 y;
    s32 z;

    do {
    } while (0);

    sp28[0] = 1;
    sp28[1] = -1;

    if (arg0->owner->isDestroyed == 1) {
        func_80069CF8_6A8F8();
        return;
    }

    switch (arg0->owner->actionMode) {
        default:

        case 0:
            arg0->scale += arg0->scaleVelocity;
            if (arg0->scale > 0xC80000) {
                arg0->scaleVelocity = 0xFFF80000;
            } else if (arg0->scale <= 0x63FFFF) {
                arg0->scaleVelocity = 0x80000;
            }
            break;

        case 1:
            arg0->scale += 0xFFF80000;
            if (arg0->scale < 0) {
                arg0->scale = 0;
            }
            break;

        case 2:
            arg0->scale += 0x40000;
            if (arg0->scale > 0xC80000) {
                owner = arg0->owner;
                arg0->scaleVelocity = 0xC80000;
                owner->actionMode = 0;
            }
            break;
    }

    owner = arg0->owner;
    x = owner->posX;
    y = owner->posY;
    z = owner->posZ;
    updateSpriteAnimation(&arg0->spriteState, 0x10000);

    ownerRef = arg0->owner;
    if (ownerRef->unk88 == 0) {
        return;
    }

    if (ownerRef->displayEnabled == 0) {
        return;
    }

    func_8000A190_AD90(
        (s32 *)&arg0->spriteState,
        ownerRef->ownerData->unk16,
        x,
        y,
        z,
        0x10000,
        0x10000,
        0,
        0,
        ((u8 *)(&arg0->scale))[1]
    );
}

void cleanupPulsingSpriteIndicator(PulsingSpriteState *arg0) {
    releaseNodeMemoryRef((void **)&arg0->spriteState);
}

void initStretchingModelTask(StretchingModelTaskState *arg0) {
    memcpy(arg0->transformMatrix, identityMatrix, 0x20);
    arg0->displayList = loadAssetGroupDisplayList(arg0->owner);
    arg0->vertexData = loadAssetGroupVertexData(arg0->owner);
    arg0->unk30 = 0;
    arg0->unk24 = &D_80088720_89320;
    arg0->rotationAngle = 0;
    arg0->stretchAngle = 0;
    setCleanupCallback(&cleanupStretchingModelTask);
    setCallback(&updateStretchingModelTask);
}

void updateStretchingModelTask(StretchingModelTaskState *arg0) {
    Mat3x3Padded sp10;
    Mat3x3Padded sp30;
    Mat3x3Padded sp50;
    s16 stretchScale;
    void *outputMatrix;
    Mat3x3Padded *sp10Ptr;
    u16 rotationOffset;

    sp10Ptr = &sp10;
    memcpy(sp10Ptr, identityMatrix, 0x20);
    memcpy(&sp30, identityMatrix, 0x20);
    memcpy(&sp50, &((StretchingModelOwner *)arg0->owner)->transformMatrix, 0x20);

    if (((StretchingModelOwner *)arg0->owner)->isDestroyed == 1) {
        func_80069CF8_6A8F8();
        return;
    }

    stretchScale = ((s32)(approximateSin(arg0->stretchAngle) * 0x133) >> 8) + 0x3000;
    rotationOffset = ((approximateSin(arg0->rotationAngle) >> 7) * 6) & 0xFFFE;
    createZRotationMatrix(sp10Ptr, rotationOffset);
    scaleMatrix(sp10Ptr, 0x2000, stretchScale, 0x2000);
    outputMatrix = arg0->transformMatrix;
    func_8006B084_6BC84(sp10Ptr, &((StretchingModelOwner *)arg0->owner)->transformMatrix, outputMatrix);
    arg0->unk1C = (s32)(arg0->unk1C + 0x33333);
    enqueueModelDisplayList(arg0->owner, outputMatrix);
    arg0->rotationAngle = (s16)(((u16)arg0->rotationAngle + 0xB6) & 0x1FFF);
    arg0->stretchAngle = (s16)(((u16)arg0->stretchAngle + 0x16C) & 0x1FFF);
}

void cleanupStretchingModelTask(StretchingModelTaskState *arg0) {
    arg0->displayList = freeNodeMemory(arg0->displayList);
    arg0->vertexData = freeNodeMemory(arg0->vertexData);
}
