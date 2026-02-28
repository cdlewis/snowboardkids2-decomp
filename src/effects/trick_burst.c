#include "5520.h"
#include "assets.h"
#include "common.h"
#include "geometry.h"
#include "rom_loader.h"
#include "task_scheduler.h"

void initTrickBurstEffect(TrickBurstEffectTask *arg0);
void updateTrickBurstEffect(TrickBurstEffectTask *arg0);
void cleanupTrickBurstEffect(TrickBurstEffectTask *arg0);

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
    task->base.segment1 = loadUncompressedData(&_2151D0_ROM_START, &_2151D0_ROM_END);
    task->base.segment2 = loadCompressedData(&_663BE0_ROM_START, &_663BE0_ROM_END, 0x200);
    task->base.displayLists = &D_80089520;
    task->unk50 = 1;
    task->alpha = 0x800000;
    task->base.segment3 = 0;
    task->base.envColorAlpha = 0x80;
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
            task->base.envColorAlpha = task->alpha >> 16;
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

    memcpy(&task->base, task->initialMatrix, 0x20);

    scaleMatrix(&task->base.transform, 0x2000, task->currentScale_union.currentScale_shorts[1], 0x2000);

    enqueueTiledTextureRender(task->renderLayer, task);

    if (task->colorIndex > 0) {
        task->colorIndex = task->colorIndex - 1;
    }
}

void cleanupTrickBurstEffect(TrickBurstEffectTask *task) {
    task->vertexData = freeNodeMemory(task->vertexData);
    task->base.segment2 = freeNodeMemory(task->base.segment2);
    task->base.segment1 = freeNodeMemory(task->base.segment1);
}
