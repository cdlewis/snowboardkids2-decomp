#include "animation/pulsing_indicator.h"
#include "common.h"
#include "data/model_display_lists.h"
#include "generated/model_gfx.h"
#include "graphics/sprite_table.h"
#include "math/geometry.h"
#include "system/task_scheduler.h"
#include "ui/level_preview_3d.h"

DisplayLists gChestOpenStretchDisplayLists = { 0,
                                               CHEST_OPEN_DISPLAY_LIST_part_0508,
                                               CHEST_OPEN_DISPLAY_LIST_part_0518,
                                               NULL };

void updatePulsingSpriteIndicator(PulsingSpriteState *);
void cleanupPulsingSpriteIndicator(PulsingSpriteState *);

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
    PulsingIndicatorOwner *ownerRef;
    PulsingIndicatorOwner *owner;
    s32 x;
    s32 y;
    s32 z;

    do {
    } while (0);

    sp28[0] = 1;
    sp28[1] = -1;

    if (arg0->owner->isDestroyed == 1) {
        terminateCurrentTask();
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
    x = owner->transformMatrix.translation.x;
    y = owner->transformMatrix.translation.y;
    z = owner->transformMatrix.translation.z;
    updateSpriteAnimation(&arg0->spriteState, 0x10000);

    ownerRef = arg0->owner;
    if (ownerRef->unk88 == 0) {
        return;
    }

    if (ownerRef->displayEnabled == 0) {
        return;
    }

    renderSprite(
        &arg0->spriteState,
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
    memcpy(arg0->transformMatrix, &identityMatrix, sizeof(Transform3D));
    arg0->displayList = loadAssetGroupDisplayList(arg0->owner);
    arg0->vertexData = loadAssetGroupCompressedData(arg0->owner);
    arg0->unk30 = 0;
    arg0->unk24 = &gChestOpenStretchDisplayLists;
    arg0->rotationAngle = 0;
    arg0->stretchAngle = 0;
    setCleanupCallback(&cleanupStretchingModelTask);
    setCallback(&updateStretchingModelTask);
}

void updateStretchingModelTask(StretchingModelTaskState *arg0) {
    Transform3D sp10;
    Transform3D sp30;
    Transform3D sp50;
    s16 stretchScale;
    void *outputMatrix;
    Transform3D *sp10Ptr;
    u16 rotationOffset;

    sp10Ptr = &sp10;
    memcpy(sp10Ptr, &identityMatrix, sizeof(Transform3D));
    memcpy(&sp30, &identityMatrix, sizeof(Transform3D));
    memcpy(&sp50, &((PulsingIndicatorOwner *)arg0->owner)->transformMatrix, sizeof(Transform3D));

    if (((PulsingIndicatorOwner *)arg0->owner)->isDestroyed == 1) {
        terminateCurrentTask();
        return;
    }

    stretchScale = ((s32)(approximateSin(arg0->stretchAngle) * 0x133) >> 8) + 0x3000;
    rotationOffset = ((approximateSin(arg0->rotationAngle) >> 7) * 6) & 0xFFFE;
    createZRotationMatrix(sp10Ptr, rotationOffset);
    scaleMatrix(sp10Ptr, 0x2000, stretchScale, 0x2000);
    outputMatrix = arg0->transformMatrix;
    composeTransform3D(sp10Ptr, &((PulsingIndicatorOwner *)arg0->owner)->transformMatrix, outputMatrix);
    arg0->unk1C = (s32)(arg0->unk1C + 0x33333);
    enqueueModelDisplayList(arg0->owner, outputMatrix);
    arg0->rotationAngle = (s16)(((u16)arg0->rotationAngle + 0xB6) & 0x1FFF);
    arg0->stretchAngle = (s16)(((u16)arg0->stretchAngle + 0x16C) & 0x1FFF);
}

void cleanupStretchingModelTask(StretchingModelTaskState *arg0) {
    arg0->displayList = freeNodeMemory(arg0->displayList);
    arg0->vertexData = freeNodeMemory(arg0->vertexData);
}
