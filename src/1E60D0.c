#include "10AD0.h"
#include "6E840.h"
#include "common.h"
#include "rom_loader.h"
#include "task_scheduler.h"

USE_ASSET(_646910);

typedef struct {
    s8 isComplete;
    s8 activeLayerCount;
    s8 layersRemaining;
    s8 fadeOutTriggered;
} ScreenTransitionState;

typedef struct {
    void *state;
} ScreenTransitionTask;

typedef struct {
    u8 padding[0x4];
    void *assetData;
    s32 alpha;
    s16 layerIndex;
    s16 padE;
    s16 horizontalOffset;
    s16 fadeFlag;
    void *assetDataCopy;
    s16 initialY;
    s16 xPosition;
    s16 targetX;
    s16 yVelocity;
    s16 maxAlpha;
    s8 pad0;
    s8 pad1;
    s8 alphaValue;
} TransitionLayerState;

typedef struct {
    s8 isComplete;
    s8 activeLayerCount;
    s32 unk4;
} TransitionCleanupData;

typedef struct {
    s32 unk0;
    TransitionCleanupData *cleanupData;
} TransitionSpawnerCleanupArg;

typedef struct {
    ScreenTransitionState *state;
    u8 padding[0x4];
    union {
        s32 word;
        s16 halfword;
    } delayTimer;
    s32 unkC;
    s32 unk10;
    u8 padding2[0x10];
    s8 alphaValue;
} TransitionSpawnerState;

typedef struct {
    s32 unk0;
    void *assetData;
    s16 delayTimer;
} TransitionInitState;

typedef struct {
    void *state;
    s32 unk4;
    s32 unk8;
    s16 layerIndex;
} TransitionLayerTask;

extern s8 s_TransitionLayerInitialY[];

void freeTransitionData(TransitionCleanupData **);
void updateTransitionFadeIn(TransitionSpawnerState *);
void updateTransitionFadeOut(TransitionSpawnerState *);
void updateTransitionSpawner(TransitionSpawnerState *);
void cleanupTransitionSpawner(TransitionSpawnerCleanupArg *);
void initTransitionTask(TransitionInitState *);
void cleanupTransitionLayer(TransitionCleanupData **);

void initScreenTransition(ScreenTransitionState *state) {
    ScreenTransitionTask *task;

    state->isComplete = 0;
    state->activeLayerCount = 0;
    state->layersRemaining = 3;
    state->fadeOutTriggered = 0;
    task = (ScreenTransitionTask *)scheduleTask(&initTransitionTask, 0, 0, 0);
    if (task != NULL) {
        task->state = state;
    }
}

void loadTransitionAsset(void **assetData) {
    *assetData = loadCompressedData(&_646910_ROM_START, &_646910_ROM_END, 0x658);
}

void freeTransitionData(TransitionCleanupData **data) {
    *data = freeNodeMemory(*data);
}

void initTransitionLayer(TransitionLayerState *layer) {
    s8 initialY;

    setCleanupCallback(&cleanupTransitionLayer);
    loadTransitionAsset(&layer->assetData);

    layer->fadeFlag = 0;
    layer->horizontalOffset = (layer->layerIndex - 1) * 32;
    layer->assetDataCopy = layer->assetData;

    initialY = s_TransitionLayerInitialY[layer->layerIndex];

    layer->targetX = 0x400;
    layer->xPosition = 0x400;
    layer->yVelocity = 0;
    layer->maxAlpha = 0xFF;
    layer->pad0 = 0;
    layer->pad1 = 0;
    layer->alphaValue = 0;
    layer->alpha = 0;
    layer->initialY = initialY;

    setCallback(&updateTransitionFadeIn);
}

void updateTransitionFadeOut(TransitionSpawnerState *state) {
    s32 alpha;
    s32 new_var;

    alpha = state->delayTimer.word;
    if (alpha <= 0) {
        func_80069CF8_6A8F8();
        return;
    }

    alpha -= 0xA0000;
    new_var = alpha;
    state->delayTimer.word = alpha;
    if (alpha < 0) {
        state->delayTimer.word = 0;
    }

    alpha = state->delayTimer.halfword;
    new_var = alpha;
    state->alphaValue = (s8)new_var;

    debugEnqueueCallback(1, 0, &func_80011924_12524, &state->unk10);
}

void updateTransitionFadeIn(TransitionSpawnerState *state) {
    s32 alpha;

    if (state->state->fadeOutTriggered == 1) {
        setCallback(&updateTransitionFadeOut);
    }

    alpha = state->delayTimer.word + 0xA0000;
    state->delayTimer.word = alpha;
    if (alpha > 0xFF0000) {
        state->delayTimer.word = 0xFF0000;
    }

    alpha = state->delayTimer.halfword;
    state->alphaValue = (s8)alpha;

    debugEnqueueCallback(1, 0, &func_80011924_12524, &state->unk10);
}

void cleanupTransitionLayer(TransitionCleanupData **data) {
    TransitionCleanupData *cleanupData = *data;
    cleanupData->activeLayerCount--;
    freeTransitionData(data + 1);
}

void initTransitionTask(TransitionInitState *state) {
    loadTransitionAsset(&state->assetData);
    state->delayTimer = 0;
    setCleanupCallback(&cleanupTransitionSpawner);
    setCallback(&updateTransitionSpawner);
}

void updateTransitionSpawner(TransitionSpawnerState *state) {
    ScreenTransitionState *transitionState;
    TransitionLayerTask *layerTask;

    transitionState = state->state;
    if (transitionState->layersRemaining == 0) {
        if (state->delayTimer.halfword == 0) {
            transitionState->fadeOutTriggered = 1;

            if (state->state->activeLayerCount != 0) {
                return;
            }
            state->state->isComplete = 1;
            func_80069CF8_6A8F8();
            return;
        } else {
            state->delayTimer.halfword--;
        }
    } else if (state->delayTimer.halfword == 0) {
        layerTask = (TransitionLayerTask *)scheduleTask(&initTransitionLayer, 0, 0, 0);
        if (layerTask != 0) {
            layerTask->state = (void *)state->state;
            layerTask->layerIndex = (s16)(3 - ((s8)((u8)state->state->layersRemaining)));
            state->state->activeLayerCount++;
        }

        state->state->layersRemaining--;

        if (state->state->layersRemaining == 0) {
            state->delayTimer.halfword = 0x78;
        } else {
            state->delayTimer.halfword = 0x14;
        }
    } else {
        state->delayTimer.halfword--;
    }
}

void cleanupTransitionSpawner(TransitionSpawnerCleanupArg *arg) {
    freeTransitionData(&arg->cleanupData);
}
