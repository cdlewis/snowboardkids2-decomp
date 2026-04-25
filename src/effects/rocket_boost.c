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

typedef struct RocketEffectData RocketEffectData;
void cleanupRocketEffect(RocketEffectData *data);

struct RocketEffectData {
    u8 _pad0[0x20];
    void *unk20;
    void *uncompressedAsset;
    void *compressedAsset;
    s32 unk2C;
    u8 _pad30[0x30];
    void *unk60;
    void *unk64;
    s32 unk68;
    u8 _pad6C[0x16];
    s16 unk82;
};

typedef struct {
    u8 _pad0[0x78];
    SceneModel *sceneModel;
    u8 _pad7C[0x4];
    s16 rotation;
    u8 _pad82[0x2];
    s16 displayDuration;
} RocketEffectTask;

void initRocketEffect(RocketEffectData *data) {
    data->unk20 = &D_80088670_89270;
    data->uncompressedAsset = loadUncompressedData(&_1FB8B0_ROM_START, &_1FB8B0_ROM_END);
    data->compressedAsset = loadCompressedData(&_4CA440_ROM_START, &_4CA440_ROM_END, 0xE90);
    data->unk2C = 0;
    data->unk68 = 0;
    data->unk82 = 0x200;
    data->unk60 = data->uncompressedAsset;
    data->unk64 = data->compressedAsset;
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

void updateRocketEffectFalling(RocketEffectFallingData *arg0) {
    s32 pad[8];
    arg0->unk7C -= 0x8000;
    if (arg0->unk7C <= (s32)0xFFF80000) {
        terminateCurrentTask();
    }
    arg0->unk0.transform.translation.y += arg0->unk7C;
    enqueueDisplayListObject(0, &arg0->unk0);
}

void cleanupRocketEffect(RocketEffectData *data) {
    data->uncompressedAsset = freeNodeMemory(data->uncompressedAsset);
    data->compressedAsset = freeNodeMemory(data->compressedAsset);
}

void spawnRocketEffect(SceneModel *sceneModel, s16 displayDuration) {
    spawnRocketEffectEx(sceneModel, displayDuration, 1, 0, 100);
}

void spawnRocketEffectEx(SceneModel *sceneModel, s16 displayDuration, u8 nodeType, u8 identifierFlag, u8 priority) {
    RocketEffectTask *task;

    task = (RocketEffectTask *)scheduleTask(&initRocketEffect, nodeType, identifierFlag, priority);
    if (task != NULL) {
        task->sceneModel = sceneModel;
        task->rotation = 0;
        task->displayDuration = displayDuration;
    }
}
