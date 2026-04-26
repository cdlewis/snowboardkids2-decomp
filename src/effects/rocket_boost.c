#include "effects/rocket_boost.h"
#include "assets.h"
#include "common.h"
#include "graphics/displaylist.h"
#include "math/geometry.h"
#include "system/task_scheduler.h"
#include "ui/level_preview_3d.h"

extern s32 gFrameCounter;

DisplayLists D_80088670_89270 = { 0, (Gfx *)0x010001E8, NULL, NULL };
DisplayLists D_80088680_89280 = { 0, NULL, (Gfx *)0x01000390, NULL };
DisplayLists D_80088690_89290 = { 0, NULL, NULL, (Gfx *)0x01000540 };
DisplayLists D_800886A0 = { 0, (Gfx *)0x01000118, NULL, NULL };

void cleanupRocketEffect(RocketEffectUpdateData *data);

void initRocketEffect(RocketEffectUpdateData *data) {
    data->body.displayLists = &D_80088670_89270;
    data->body.segment1 = loadUncompressedData(&_1FB8B0_ROM_START, &_1FB8B0_ROM_END);
    data->body.segment2 = loadCompressedData(&_4CA440_ROM_START, &_4CA440_ROM_END, 0xE90);
    data->body.segment3 = NULL;
    data->flame.segment3 = NULL;
    data->scale = 0x200;
    data->flame.segment1 = data->body.segment1;
    data->flame.segment2 = data->body.segment2;
    setCleanupCallback(&cleanupRocketEffect);
    setCallbackWithContinue(&updateRocketEffect);
}

void updateRocketEffect(RocketEffectUpdateData *arg0) {
    s32 pad[8];

    createYRotationMatrix(&gIdentityMatrix32, arg0->yAngle);
    func_8006B084_6BC84(&gIdentityMatrix32, (Transform3D *)&arg0->sceneModel->unk0->unk3C0, &arg0->body.transform);
    scaleMatrix(&arg0->body.transform, arg0->scale, arg0->scale, arg0->scale);

    gScaleMatrix.translation.x = 0;
    gScaleMatrix.translation.y = 0x9CCCC;
    gScaleMatrix.translation.z = 0xFFE44CCD;

    func_8006B084_6BC84(&gScaleMatrix, &arg0->body.transform, &arg0->flame.transform);

    if (gFrameCounter & 1) {
        arg0->flame.displayLists = &D_80088680_89280;
    } else {
        arg0->flame.displayLists = &D_80088690_89290;
    }

    enqueueDisplayListObject(0, &arg0->body);

    if (arg0->scale == 0x2000) {
        enqueueDisplayListObject(0, &arg0->flame);
    }

    if (arg0->displayDuration != 0) {
        if (arg0->scale == 0x2000) {
            arg0->displayDuration--;
        } else {
            arg0->scale += 0x200;
        }
    } else {
        arg0->fallVelocity = 0x40000;
        setCallback(&updateRocketEffectFalling);
    }
}

void updateRocketEffectFalling(RocketEffectUpdateData *arg0) {
    s32 pad[8];
    arg0->fallVelocity -= 0x8000;
    if (arg0->fallVelocity <= (s32)0xFFF80000) {
        terminateCurrentTask();
    }
    arg0->body.transform.translation.y += arg0->fallVelocity;
    enqueueDisplayListObject(0, &arg0->body);
}

void cleanupRocketEffect(RocketEffectUpdateData *data) {
    data->body.segment1 = freeNodeMemory(data->body.segment1);
    data->body.segment2 = freeNodeMemory(data->body.segment2);
}

void spawnRocketEffect(SceneModel *sceneModel, s16 displayDuration) {
    spawnRocketEffectEx(sceneModel, displayDuration, 1, 0, 100);
}

void spawnRocketEffectEx(SceneModel *sceneModel, s16 displayDuration, u8 nodeType, u8 identifierFlag, u8 priority) {
    RocketEffectUpdateData *task;

    task = (RocketEffectUpdateData *)scheduleTask(&initRocketEffect, nodeType, identifierFlag, priority);
    if (task != NULL) {
        task->sceneModel = sceneModel;
        task->yAngle = 0;
        task->displayDuration = displayDuration;
    }
}
