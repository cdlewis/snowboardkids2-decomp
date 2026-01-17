#include "5520.h"
#include "common.h"
#include "geometry.h"
#include "rom_loader.h"
#include "task_scheduler.h"

USE_ASSET(_2151D0);
USE_ASSET(_663BE0);
USE_ASSET(_6637B0);

typedef struct {
    u8 _pad0[0x24];
    void *textureData;
    void *compressedData;
    u8 _pad2C[0x10];
    void *vertexData;
} TrickBurstEffectTaskCleanupArg;

void initTrickBurstEffect(TrickBurstEffectTask *arg0);
void updateTrickBurstEffect(TrickBurstEffectTask *arg0);
void cleanupTrickBurstEffect(TrickBurstEffectTaskCleanupArg *arg0);

void spawnTrickBurstEffect(u16 renderLayer, void *transformMatrix, s16 colorIndex, s32 targetScale) {
    TrickBurstEffectTask *task;

    task = (TrickBurstEffectTask *)scheduleTask(&initTrickBurstEffect, 1, 0, 0);

    if (task != NULL) {
        task->renderLayer = renderLayer;
        memcpy(task->initialMatrix, transformMatrix, 0x20);
        task->colorIndex = colorIndex;
        task->currentScale_union.currentScale = 0;
        task->targetScale = targetScale;
        task->animState = 0;
    }
}

void initTrickBurstEffect(TrickBurstEffectTask *task) {
    setCleanupCallback(cleanupTrickBurstEffect);
    task->textureData = loadUncompressedData(&_2151D0_ROM_START, &_2151D0_ROM_END);
    task->compressedData = loadCompressedData(&_663BE0_ROM_START, &_663BE0_ROM_END, 0x200);
    task->displayLists = &D_80089520;
    task->unk50 = 1;
    task->alpha = 0x800000;
    task->unk2C = 0;
    task->alphaHighByte = 0x80;
    task->vertexData = loadCompressedData(&_6637B0_ROM_START, &_6637B0_ROM_END, 0x858);
    task->scrollUUnion.scrollU = 0;
    task->scrollVUnion.scrollV = 0;
    task->scrollUSpeed = 0;
    task->scrollVSpeed = 0x10;
    task->unk4C = 0;
    task->unk4E = 0;
    setCallback(updateTrickBurstEffect);
}

void updateTrickBurstEffect(TrickBurstEffectTask *task) {

    switch (task->animState) {
        case 0:
            task->currentScale_union.currentScale += 0x222;
            if (task->targetScale < task->currentScale_union.currentScale) {
                task->currentScale_union.currentScale = task->targetScale;
                task->animState = 1;
            }
            break;
        case 1:
            if (task->colorIndex == 0) {
                task->animState = 2;
            }
            break;
        case 2:
            task->alpha += 0xFFFA0000;
            task->alphaHighByte = task->alpha >> 16;
            if (task->alpha < 0) {
                task->alpha = 0;
                terminateCurrentTask();
                return;
            }
            break;
    }

    task->scrollUUnion.scrollU = task->scrollUUnion.scrollU + task->scrollUSpeed;
    task->scrollVUnion.scrollV = task->scrollVUnion.scrollV + task->scrollVSpeed;
    task->scrollUUnion.scrollU = task->scrollUUnion.scrollU_bytes[1];
    task->scrollVUnion.scrollV = task->scrollVUnion.scrollV_bytes[1];

    memcpy(task, task->initialMatrix, 0x20);

    scaleMatrix((Transform3D *)task, 0x2000, task->currentScale_union.currentScale_shorts[1], 0x2000);

    enqueueTiledTextureRender(task->renderLayer, task);

    if (task->colorIndex > 0) {
        task->colorIndex = task->colorIndex - 1;
    }
}

void cleanupTrickBurstEffect(TrickBurstEffectTaskCleanupArg *task) {
    task->vertexData = freeNodeMemory(task->vertexData);
    task->compressedData = freeNodeMemory(task->compressedData);
    task->textureData = freeNodeMemory(task->textureData);
}
