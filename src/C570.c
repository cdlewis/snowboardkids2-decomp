#include "C570.h"
#include "common.h"
#include "displaylist.h"
#include "geometry.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_1FB8B0);
USE_ASSET(_4CA440);

extern void *D_80088670_89270;
extern void *D_80088680_89280;
extern void *D_80088690_89290;
extern Transform3D D_8009A8B0_9B4B0;
extern s32 D_8009A8A4_9B4A4;
extern s32 D_8009A8A8_9B4A8;
extern s32 D_8009A8AC_9B4AC;
extern s32 gFrameCounter;

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
    void *temp_v0;
    s32 *ptr;

    createYRotationMatrix(&D_8009A8B0_9B4B0, arg0->unk80);
    temp_v0 = (void *)((u8 *)(*arg0->unk78) + 0x3C0);
    func_8006B084_6BC84(&D_8009A8B0_9B4B0, temp_v0, arg0);
    scaleMatrix((Transform3D *)&arg0->unk0, arg0->unk82, arg0->unk82, arg0->unk82);

    ptr = &D_8009A8A4_9B4A4;
    *ptr = 0;
    D_8009A8A8_9B4A8 = 0x9CCCC;
    D_8009A8AC_9B4AC = 0xFFE44CCD;

    func_8006B084_6BC84(ptr - 5, arg0, &arg0->unk3C);

    if (gFrameCounter & 1) {
        arg0->unk3C.unk20 = (DisplayLists *)&D_80088680_89280;
    } else {
        arg0->unk3C.unk20 = (DisplayLists *)&D_80088690_89290;
    }

    enqueueDisplayListObject(0, &arg0->unk0);

    if (arg0->unk82 == 0x2000) {
        enqueueDisplayListObject(0, &arg0->unk3C);
    }

    if (arg0->unk84 != 0) {
        if (arg0->unk82 == 0x2000) {
            arg0->unk84--;
        } else {
            arg0->unk82 = arg0->unk82 + 0x200;
        }
    } else {
        arg0->unk7C = 0x40000;
        setCallback(&updateRocketEffectFalling);
    }
}

void updateRocketEffectFalling(RocketEffectFallingData *arg0) {
    s32 pad[8];
    arg0->unk7C -= 0x8000;
    if (arg0->unk7C <= (s32)0xFFF80000) {
        func_80069CF8_6A8F8();
    }
    arg0->unk0.unk10.position.y += arg0->unk7C;
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
