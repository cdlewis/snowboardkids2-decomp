#pragma once

#include "common.h"
#include "cutscene/cutscene_manager.h"
#include "effects/scrolling_texture.h"
#include "graphics/graphics.h"

typedef struct {
    s16 frameCount;
    u8 playbackState;
    u8 exitRequested;
    s32 unk4;
    ViewportNode sceneNode;
    ViewportNode overlayNode;
    ViewportNode uiNode;
    ColorData lightColors;
    u8 padding598[0x10];
    ColorData ambientColor;
    union {
        CutsceneManager cutsceneManager;
        u8 cutsceneBuffer[0x1224];
    } cutsceneData;
    u8 paddingAfterManager[0x4];
    void *frameDataPtr;
    s16 fadeCounter;
    s16 delayCounter;
    ScreenTransitionState transitionState;
} CutsceneTaskMemory;

typedef union {
    struct {
        s16 lower;
        s16 upper;
    } half;
    s32 full;
} CompositeInt;

typedef struct {
    s16 slotIndex;
    s16 cutsceneType;
    s16 frameIndex;
    s16 unk6;
    CompositeInt slotSelection;
    CompositeInt typeSelection;
} CutsceneState;

typedef enum {
    CUTSCENE_STATE_INIT_DMA,
    CUTSCENE_STATE_WAIT_DMA,
    CUTSCENE_STATE_INIT_MANAGER,
    CUTSCENE_STATE_START_PLAYBACK,
    CUTSCENE_STATE_DELAY,
    CUTSCENE_STATE_PLAYING,
    CUTSCENE_STATE_SKIP_START,
    CUTSCENE_STATE_SKIP_FADE,
    CUTSCENE_STATE_TERMINATE_TASKS,
    CUTSCENE_STATE_WAIT_TERMINATE,
    CUTSCENE_STATE_CLEANUP,
    CUTSCENE_STATE_EXIT
} CutsceneStateEnum;

extern CutsceneState gCutsceneState;

void *__udiv_w_sdiv(void);
void loadCutsceneOverlay(void);
void setCutsceneSelection(s16 slotIndex, s16 cutsceneType);
