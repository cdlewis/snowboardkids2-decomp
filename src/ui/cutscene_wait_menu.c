#include "ui/cutscene_wait_menu.h"
#include "animation/easing_state.h"
#include "audio/audio.h"
#include "common.h"
#include "common_bss.h"
#include "data/course_data.h"
#include "graphics/clip_text_render.h"
#include "os_cont.h"
#include "race/race_session.h"
#include "system/task_scheduler.h"
#include "text/font_render.h"

void updateCutsceneWaitMenu(CutsceneWaitMenuState *state);
void cleanupCutsceneWaitMenu(CutsceneWaitMenuState *state);

void initMenuFadeIn(CutsceneWaitMenuState *state) {
    void *temp = state->textRenderAsset;

    state->textAlpha.value = 0xFF;
    state->textTransparency.value = 0xFF;
    state->textX = 0;
    state->textY = 0;
    state->textPaletteIndex = 0;
    state->state = 1;
    state->activeTextRenderAsset = temp;
}

void animateMenuExpandHorizontal(CutsceneWaitMenuState *state) {
    state->panelWidth++;
    if (state->panelWidth >= 13) {
        state->state = 2;
    }
    if (state->panelRed < 65) {
        state->panelRed = 64;
    } else {
        state->panelRed -= 16;
    }
}

void animateMenuExpandVertical(CutsceneWaitMenuState *state) {
    s16 temp;
    s16 temp2;

    if (++state->panelHeight >= 6) {
        state->state = 3;
    }

    temp2 = temp = state->panelRed;
    state->panelRed = (temp < 0x41) ? 0x40 : temp2 - 0x10;
}

void *handleMenuSelection(CutsceneWaitMenuState *state) {
    void *temp_v0 = getTable2DEntry(state->textTable, state->waitFrameCount, state->textColumn);
    unsigned long new_var;

    new_var = temp_v0 == 0;
    if (new_var) {
        state->state = 6;
        state->textData = temp_v0;
    } else {
        state->textData = temp_v0;
    }
    if (gControllerInputs[0] & A_BUTTON) {
        playSoundEffect(45);
        state->state = 4;
    }
    return temp_v0;
}

void animateMenuContractVertical(CutsceneWaitMenuState *state) {
    state->panelHeight--;
    if (state->panelHeight < 2) {
        state->state = 5;
    }
    if (state->panelGreen > 0) {
        state->panelGreen -= 16;
    } else {
        state->panelGreen = 0;
    }
}

void animateMenuContractHorizontal(CutsceneWaitMenuState *state) {
    state->panelWidth--;
    if (state->panelWidth < 2) {
        state->state = 6;
    }
    if (state->panelGreen > 0) {
        state->panelGreen -= 16;
    } else {
        state->panelGreen = 0;
    }
}

void initCutsceneWaitMenu(CutsceneWaitMenuState *state) {
    state->state = 0;
    state->textColumn = 0;
    state->panelSpriteAsset = loadAsset_34F7E0();
    state->textRenderAsset = loadTextRenderAsset(1);
    state->textTable = loadDmaAsset(2);
    state->panelWidth = 1;
    state->panelHeight = 1;
    state->panelRed = 0xFF;
    state->panelGreen = 0xC0;
    playSoundEffect(0x2C);
    setCleanupCallback(&cleanupCutsceneWaitMenu);
    setCallback(&updateCutsceneWaitMenu);
}

void updateCutsceneWaitMenu(CutsceneWaitMenuState *state) {
    void *result = 0;
    s32 flag = 0;

    switch (state->state) {
        case 0:
            initMenuFadeIn(state);
            break;
        case 1:
            animateMenuExpandHorizontal(state);
            break;
        case 2:
            animateMenuExpandVertical(state);
            break;
        case 3:
            result = handleMenuSelection(state);
            flag = 1;
            break;
        case 4:
            animateMenuContractVertical(state);
            break;
        case 5:
            animateMenuContractHorizontal(state);
            break;
        case 6:
            state->cutsceneManager->skipAnimation = 0;
            terminateCurrentTask();
            return;
    }

    state->panelX = -(state->panelWidth << 3);
    state->panelY = -(state->panelHeight << 3);

    if (result != 0) {
        s16 temp16;
        state->textX = state->panelX;
        temp16 = state->panelY;
        state->textY = temp16;
        enqueueTextLayout(
            state->activeTextRenderAsset,
            state->textData,
            state->textX,
            temp16,
            state->textAlpha.bytes.low,
            state->textTransparency.bytes.low,
            state->textPaletteIndex,
            1,
            0
        );
    }

    renderTiledSprite3x3(
        state->panelSpriteAsset,
        (s16)(state->panelX),
        (s16)(state->panelY),
        (s16)(state->panelWidth),
        (s16)(state->panelHeight),
        flag,
        (u8)state->panelRed,
        (u8)state->panelGreen,
        1,
        0
    );
}

void cleanupCutsceneWaitMenu(CutsceneWaitMenuState *state) {
    state->textRenderAsset = freeNodeMemory(state->textRenderAsset);
    state->panelSpriteAsset = freeNodeMemory(state->panelSpriteAsset);
    state->textTable = freeNodeMemory(state->textTable);
}
