#pragma once

#include "cutscene/cutscene_manager.h"
#include "effects/scrolling_texture.h"
#include "common.h"
#include "graphics/graphics.h"
#include "core/main.h"

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

void* __udiv_w_sdiv(void);
void loadCutsceneOverlay(void);
void setCutsceneSelection(s16 slotIndex, s16 cutsceneType);
