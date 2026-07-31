#include "ui/character_select_ui.h"
#include "audio/audio.h"
#include "common.h"
#include "common_bss.h"
#include "gamestate.h"
#include "graphics/graphics.h"
#include "math/geometry.h"
#include "os_cont.h"
#include "race/race_session.h"
#include "story/map_events.h"
#include "system/rom_loader.h"
#include "system/task_scheduler.h"
#include "ui/character_select_gfx.h"
#include "ui/save_data.h"

void awaitCharacterSelectLoad(void);
void scheduleCharacterSelectTasks(void);
void updateCharacterSelect(void);
void cleanupCharacterSelect(void);
void onCharacterSelectProceed(void);
void onCharacterSelectCancel(void);

USE_ASSET(menuUiSprites);
USE_ASSET(playerCountSelectSprites);
USE_ASSET(tiledSnowmanAsset);
USE_ASSET(uiFontSpriteSheet);

ColorData charSelectDimLight = { 0x50, 0x50, 0x50, 0x00, 0x00, 0x50, 0x50, 0x00 };
char charSelectDimAmbientStr[] = "PPP";
ColorData charSelectNormalLight = { 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x50, 0x50, 0x00 };
s32 charSelectNormalAmbient = 0x7F7F7F00;
ColorData charSelectFlashLight = { 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x50, 0x50, 0x00 };
s32 charSelectFlashAmbient = (s32)0xFFFFFF00;

u16 charRowConfirmSoundIds[] = { 0x012C, 0x012D, 0x012B, 0x012E };

u16 boardConfirmSoundIds[] = {
    0x012F, 0x0130, 0x0132, 0x0131, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x5050, 0x5000,
    0x0050, 0x5000, 0x5050, 0x5000, 0xFFFF, 0xFF00, 0x0050, 0x5000, 0x7F7F, 0x7F00, 0xFFFF, 0xFF00,
};

CharSelectModelPositions charSelectModelPositions = {
    {
     0x0050, 0x5000,
     (s16)0xFFFF,
     (s16)0xFF00,
     0x0060, 0x0000,
     (s16)0xFFA0,
     0x0000, 0x0070,
     0x0000, (s16)0xFFB0,
     0x0000, 0x0060,
     0x0000, (s16)0xFFA0,
     0x0000, 0x0060,
     },
    {
     { 0x0000, (s16)0xFFA0, 0x0000 },
     { (s16)0xFF90, 0x0040, 0x0050 },
     { (s16)0xFF80, 0x0024, 0x0028 },
     { (s16)0xFFD8, 0x0020, 0x0024 },
     { (s16)0xFFD8, 0x0020, 0x0024 },
     },
};

CharSelectIconPositions charSelectIconPositions = {
    { (s16)0xFFB0,                         0x0040, 0x0058 },
    {
     { 0x0000, (s16)0xFF88, (s16)0xFFD0 },
     { 0x0028, 0x0000, (s16)0xFFF0 },
     { 0x0018, (s16)0xFFC0, (s16)0xFFE8 },
     { 0x0014, (s16)0xFFC0, (s16)0xFFE8 },
     },
    0x0014,
};

CharSelectItemData charSelectItemData = {
    3,
    4,
    {
      0x04, 0x05, 0x04, 0x04, 0x07, 0x04, 0x04, 0x02, 0x05, 0x05, 0x04, 0x05, 0x05, 0x06, 0x05, 0x05,
      0x04, 0x03, 0x03, 0x06, 0x03, 0x03, 0x08, 0x03, 0x03, 0x08, 0x05, 0x05, 0x06, 0x05, 0x0A, 0x07,
      0x01, 0x04, 0x07, 0x04, 0x04, 0x03, 0x04, 0x04, 0x07, 0x04, 0x04, 0x01, 0x05, 0x05, 0x09, 0x04,
      },
    {
      0x040A, 0x010A,
      0x0000, (s16)0xFFA8,
      (s16)0xFFD0,
      0x0028, 0x0018,
      (s16)0xFFF0,
      0x0018, (s16)0xFFD8,
      (s16)0xFFE8,
      0x0014, (s16)0xFFD8,
      },
};

PlayerNumberPositions playerNumberPositions = {
    {
     (s16)0xFFE8,
     0x0014, (s16)0xFF80,
     (s16)0xFFA8,
     (s16)0xFF78,
     (s16)0xFFD4,
     (s16)0xFFC0,
     },
    { (s16)0xFFD4, (s16)0xFFC0, (s16)0xFFD4 },
};

CharSelectStatsPositions charSelectStatsPositions = {
    {
     (s16)0xFFA0,
     (s16)0xFFC8,
     0x0028, 0x0010,
     (s16)0xFFEC,
     0x0018,         (s16)0xFFD0,
     (s16)0xFFE4,
     0x0014, (s16)0xFFD0,
     (s16)0xFFE4,
     },
    {
     0x0014,           0x0008,
     0x0010, 0x001C,
     0x0028,      0x0034,
     0x0040,                      0x004C,
     0x0058, 0x0064,
     0x0070, },
};

CharSelectArrowData charSelectArrowDataTable = {
    { 0x00, 0x04 },
    { 0x0005, 0x0006 },
    {
     0x00,     0x00,     (s8)0xFF, (s8)0xD0, (s8)0xFF, (s8)0xF8, 0x00,     0x50,     (s8)0xFF, (s8)0x98,
     (s8)0xFF, (s8)0xF0, 0x00,     0x50,     (s8)0xFF, (s8)0xD0, (s8)0xFF, (s8)0xF0, 0x00,     0x50,
     (s8)0xFF, (s8)0xD0, (s8)0xFF, (s8)0xF0, 0x00,     0x50,     0x00,     0x20,
     },
    {
     0x0018, 0x0018,
     0x0018, (s16)0xFFD0,
     (s16)0xFFFC,
     0x0050, (s16)0xFFA8,
     (s16)0xFFF4,
     0x0040, (s16)0xFFE4,
     0x0000, 0x0030,
     (s16)0xFFE4,
     },
};

Vec2s charSelectNamePositions[] = {
    { 0x0000,      0x0030      },
    { (s16)0xFFE8, (s16)0xFFA8 },
    { 0x0030,      (s16)0xFFD4 },
    { (s16)0xFFF0, (s16)0xFFD4 },
};

CharSelectBoardData charSelectBoardDataTable = {
    {
     (s16)0xFFF0,
     (s16)0xFFD4,
     (s16)0xFFE0,
     (s16)0xFFF8,
     (s16)0xFFA8,
     (s16)0xFFF0,
     (s16)0xFFE0,
     (s16)0xFFF0,
     (s16)0xFFE0,
     (s16)0xFFF0,
     0x0028, },
    {
     0x0020, 0x0020,
     0x0018, 0x0020,
     0x0018, 0x0020,
     0x0020, 0x0020,
     0x0000, 0x002C,
     0x0035, 0x0029,
     0x0030, 0x0035,
     0x002A, 0x002D,
     0x003B, },
};

Vec2s charSelectNameSpritePositions[] = {
    { 0x0032, 0x0000      },
    { 0x0068, (s16)0xFFA8 },
    { 0x0070, (s16)0xFFD4 },
    { 0x0030, (s16)0xFFD4 },
};

CharSelectAnimData charSelectAnimDataTable = {
    { 0x0030 },
    {
     (s16)0xFFD4,
     (s16)0xFFF8,
     0x0018, (s16)0xFFC0,
     0x0008, (s16)0xFFF8,
     0x0004, (s16)0xFFF8,
     0x0004, (s16)0xFF80,
     (s16)0xFFA8,
     (s16)0xFF78,
     (s16)0xFFD4,
     (s16)0xFFC0,
     (s16)0xFFD4,
     (s16)0xFFC0,
     },
    {
     (s16)0xFFD4,
     (s16)0xFF98,
     (s16)0xFFD8,
     0x0028, 0x0008,
     (s16)0xFFF8,
     0x0018, (s16)0xFFC8,
     (s16)0xFFF0,
     0x0014, (s16)0xFFC8,
     (s16)0xFFF0,
     0x0014, 0x0000,
     0x0000, 0x0000,
     0x0000, },
};

void initCharacterSelectScreen(void) {
    CharacterSelectState *state;
    Transform3D transform;
    s32 i;
    s32 numOptions;
    u8 boardId;
    void *task;

    state = (CharacterSelectState *)allocateTaskMemory(0x18E0);
    setupTaskSchedulerNodes(0x30, 8, 4, 8, 0, 0, 0, 0);

    if (gGameSessionContext->gameMode != 0) {
        state->maxMenuOption = 3;
    } else {
        state->maxMenuOption = 2;
    }

    for (i = 0; i < 4; i++) {
        state->menuStates[i] = CHAR_SELECT_MENU_NAV;
        state->unlockedSlotIndices[i] = 0;
        state->frameCounters[i] = 0;
        state->previewSpinAngles[i] = 0x800;
        state->iconDisplayStates[i] = 0;
        memcpy(CHARACTER_SELECT_ROTATION_AT(state, i), &identityMatrix, sizeof(Transform3D));
        CHARACTER_SELECT_ROTATION_AT(state, i)->translation.z = (s32)0xFFEA0000;
        state->carouselAngles[i] = 0;
        state->cursorIndices[i] = (s8)(state->maxMenuOption - 2);
    }

    initMenuCameraNode(&state->cameraNode, 7, 1, 1);

    switch (gGameSessionContext->numPlayers) {
        case 1:
            initMenuCameraNode(&state->playerViewports[0], 0, 10, 0);
            initMenuCameraNode(&state->modelViewports[0], 8, 8, 1);
            initMenuCameraNode(&state->iconViewports[0], 12, 20, 1);
            break;
        case 2:
            initSplitScreen2P(&state->playerViewports[0], 0, 10, 0);
            setModelCameraTransform(&state->playerViewports[0], -0x30, -0x35, -0x70, -0x34, 0xD0, 0x34);
            setModelCameraTransform(&state->playerViewports[1], -0x30, 0x35, -0x70, -0x34, 0xD0, 0x34);
            initSplitScreen2P(&state->modelViewports[0], 8, 8, 1);
            initSplitScreen2P(&state->iconViewports[0], 12, 20, 1);
            break;
        case 3:
        case 4:
            initSplitScreen3P4P(&state->playerViewports[0], 0, 10, 0);
            initSplitScreen3P4P(&state->modelViewports[0], 8, 8, 1);
            initSplitScreen3P4P(&state->iconViewports[0], 12, 20, 1);
            break;
    }

    setViewportFadeValue(NULL, 0xFF, 0);

    if (gGameSessionContext->numPlayers == 1) {
        createViewportTransform(&transform, 0, 0, 0x540000, 0, 0, 0);
    } else if (gGameSessionContext->numPlayers == 2) {
        createViewportTransform(&transform, 0, 0, 0x480000, 0, 0, 0);
    } else {
        createViewportTransform(&transform, 0, 0, 0x380000, 0, 0, 0);
    }

    for (i = 0; i < gGameSessionContext->numPlayers; i++) {
        setViewportTransformById(state->playerViewports[i].viewportId, &transform);
    }

    state->menuSpriteAsset = loadCompressedData(&menuUiSprites_ROM_START, &playerCountSelectSprites_ROM_START, 0x8A08);
    state->iconSpriteAsset = loadCompressedData(&tiledSnowmanAsset_ROM_START, &uiFontSpriteSheet_ROM_START, 0xAE0);

    state->hasSecretCharacters = 0;
    if (countUnlockedSlotsInCategory(3) != 0) {
        state->hasSecretCharacters = 1;
    }

    for (i = 0; i < gGameSessionContext->numPlayers; i++) {
        if (gGameSessionContext->snowboardIds[i] < 9) {
            state->characterCategories[i] = gGameSessionContext->snowboardIds[i] / 3;
            state->characterVariants[i] = gGameSessionContext->snowboardIds[i] % 3;
        } else {
            state->characterCategories[i] = 3;
            state->characterVariants[i] = gGameSessionContext->snowboardIds[i] - 9;
        }
        state->previousCharacterCategories[i] = state->characterCategories[i];
        state->previousCharacterVariants[i] = state->characterVariants[i];
        boardId = gGameSessionContext->boardModelIds[i];
        state->boardIds[i] = boardId;
        state->previousBoardIds[i] = boardId;
        task = scheduleTask(initCharSelectBoardModel, 0, 0, 0x5A);
        ((CharSelectBoardPreview *)task)->playerIndex = i;
        task = scheduleTask(initCharSelectPreviewModel, 0, 0, 0x5A);
        ((CharSelectPreviewTaskState *)task)->playerIndex = i;
        task = scheduleTask(initCharSelectMenu, 0, 0, 0x5A);
        ((SelectionMenuState *)task)->playerIndex = i;
        task = scheduleTask(initCharSelectIconHideSprites, 0, 0, 0x5A);
        ((CharSelectIconHideState *)task)->playerIndex = i;
        task = scheduleTask(initCharSelectStats, 0, 0, 0x5A);
        ((CharSelectStatsState *)task)->playerIndex = i;
        task = scheduleTask(initCharSelectPlayer2NameSprites, 0, 0, 0x5A);
        ((P2NameAnimationState *)task)->playerIndex = i;
    }

    scheduleTask(initCharSelectPlayerLabels, 0, 0, 0x5A);
    scheduleTask(initCharSelectArrows, 0, 0, 0x5A);
    scheduleTask(initBoardSelectArrows, 0, 0, 0x5A);
    scheduleTask(initCharSelectPlayerNumbers, 0, 0, 0x5A);
    scheduleTask(initBoardSelectCharNames, 0, 0, 0x5A);
    scheduleTask(initCharSelectNameSprites, 0, 0, 0x5A);
    scheduleTask(initCharSelectBackgroundEffect, 0, 0, 0x5A);
    setGameStateHandler(awaitCharacterSelectLoad);
}

void awaitCharacterSelectLoad(void) {
    CharacterSelectState *state = (CharacterSelectState *)getCurrentAllocation();

    state->frameCounters[0]++;
    if (state->frameCounters[0] < 3) {
        return;
    }
    state->frameCounters[0] = 2;
    if (getPendingDmaCount() != 0) {
        return;
    }
    state->frameCounters[0] = 0;
    setViewportFadeValue(NULL, 0, 10);
    setGameStateHandler(scheduleCharacterSelectTasks);
}

void scheduleCharacterSelectTasks(void) {
    s32 i;
    void *task;

    if (getViewportFadeMode(0) != 0) {
        return;
    }

    for (i = 0; i < gGameSessionContext->numPlayers; i++) {
        task = scheduleTask(initCharSelectIcons, 1, i, 0x5A);
        if (task != NULL) {
            ((CharSelectIconsState *)task)->playerIndex = i;
        }
    }

    setGameStateHandler(updateCharacterSelect);
}

void updateCharacterSelect(void) {
    CharacterSelectState *state;
    s32 cancelCount;
    s32 i;
    u8 prevCharRow;
    u8 prevBoardId;
    u8 prevCursorIdx;
    u8 prevSlotIdx;
    s32 rotDir;
    s32 angle;
    s32 numUnlocked;
    s32 limit;
    s32 j;
    s32 k;
    s32 confirmedCount;
    u8 unlockedSlots[10];
    u32 charIdx;
    void *task;
    void *secondaryTask;
    void *boardTask;
    s32 *temp3;
    void *eepromResult;
    state = (CharacterSelectState *)getCurrentAllocation();

    cancelCount = 0;
    confirmedCount = 0;
    for (i = 0; i < gGameSessionContext->numPlayers; i++) {
        prevCursorIdx = state->cursorIndices[i];
        switch (state->menuStates[i]) {
            case CHAR_SELECT_MENU_NAV:
                if (gControllerInputs[i] & (STICK_RIGHT | CONT_RIGHT)) {
                    if (state->cursorIndices[i] < (state->maxMenuOption - 1)) {
                        state->cursorIndices[i]++;
                    }
                } else if (gControllerInputs[i] & (STICK_LEFT | CONT_LEFT)) {
                    if (state->cursorIndices[i] > 0) {
                        state->cursorIndices[i]--;
                    }
                }
                if (prevCursorIdx != state->cursorIndices[i]) {
                    state->menuStates[i] = CHAR_SELECT_MENU_ROTATING;
                    state->previousCursorIndices[i] = prevCursorIdx;
                    playSoundEffectOnChannelNoPriority(0x2B, i);
                } else if (gControllerInputs[i] & CONT_A) {
                    state->menuStates[i] = CHAR_SELECT_MENU_CONFIRMING;
                    playSoundEffectOnChannelNoPriority(0x2C, i);
                } else if (gControllerInputs[i] & CONT_B) {
                    playSoundEffect(0x2E);
                    for (j = 0; j < gGameSessionContext->numPlayers; j++) {
                        state->menuStates[j] = CHAR_SELECT_CANCEL_EXIT;
                    }

                    i = j;
                }

                break;

            case CHAR_SELECT_MENU_ROTATING:
                if (state->cursorIndices[i] == (state->maxMenuOption - 1)) {
                    rotDir = -0x200;
                } else if (state->cursorIndices[i] == (state->maxMenuOption - 3)) {
                    rotDir = 0x200;
                } else {
                    rotDir = (state->previousCursorIndices[i] == (state->maxMenuOption - 1)) ? 0x200 : -0x200;
                }

                state->carouselAngles[i] = (state->carouselAngles[i] + rotDir) & 0x1FFF;
                if (state->carouselAngles[i] == 0x1800 || state->carouselAngles[i] == 0x800 ||
                    state->carouselAngles[i] == 0) {
                    state->menuStates[i] = CHAR_SELECT_MENU_NAV;
                }
                createYRotationMatrix(&state->characterRotations[i], state->carouselAngles[i]);
                break;

            case CHAR_SELECT_MENU_CONFIRMING:
                state->frameCounters[i]++;
                if (state->frameCounters[i] != 0x10) {
                    break;
                }

                state->menuStates[i] = CHAR_SELECT_CHAR_ROW_BROWSE;
                state->frameCounters[i] = 0;
                if (state->cursorIndices[i] == (state->maxMenuOption - 1)) {
                    setViewportLightColors(
                        state->playerViewports[i].viewportId,
                        1,
                        &charSelectDimLight,
                        (ColorData *)charSelectDimAmbientStr
                    );
                    terminateTasksByTypeAndID(1, i & 0xFF);
                } else if (state->cursorIndices[i] == (state->maxMenuOption - 3)) {
                    state->menuStates[i] = CHAR_SELECT_BOARD_BROWSE;
                    state->iconDisplayStates[i] = 1;
                } else if (state->cursorIndices[i] == (state->maxMenuOption - 2)) {
                    state->menuStates[i] = CHAR_SELECT_READY_CONFIRM;
                }
                break;

            case CHAR_SELECT_CHAR_ROW_BROWSE:
                if (gControllerInputs[i] & CONT_B) {
                    playSoundEffectOnChannelNoPriority(0x2E, i);
                    state->menuStates[i] = CHAR_SELECT_MENU_NAV;
                    task = scheduleTask(initCharSelectIcons, 1, i, 0x5A);
                    if (task != 0) {
                        ((CharSelectIconsState *)task)->playerIndex = i;
                    }
                    state->cursorIndices[i] = state->maxMenuOption - 2;
                    state->carouselAngles[i] = 0;
                    createYRotationMatrix(&state->characterRotations[i], 0);
                    setViewportLightColors(
                        state->playerViewports[i].viewportId,
                        1,
                        &charSelectNormalLight,
                        (ColorData *)(&charSelectNormalAmbient)
                    );
                    if (gGameSessionContext->snowboardIds[i] < 9) {
                        state->characterCategories[i] = gGameSessionContext->snowboardIds[i] / 3;
                        state->characterVariants[i] = gGameSessionContext->snowboardIds[i] % 3;
                    } else {
                        state->characterCategories[i] = 3;
                        state->characterVariants[i] = gGameSessionContext->snowboardIds[i] - 9;
                    }
                    break;
                }

                prevCharRow = state->characterCategories[i];
                if (gControllerInputs[i] & (STICK_RIGHT | CONT_RIGHT)) {
                    state->characterCategories[i]++;
                    if ((state->hasSecretCharacters + 2) < (state->characterCategories[i] & 0xFF)) {
                        state->characterCategories[i] = 0;
                    }
                    state->scrollDirections[i] = 0;
                } else if (gControllerInputs[i] & (STICK_LEFT | CONT_LEFT)) {
                    state->characterCategories[i]--;
                    if ((state->hasSecretCharacters + 2) < (state->characterCategories[i] & 0xFF)) {
                        state->characterCategories[i] = state->hasSecretCharacters + 2;
                    }
                    state->scrollDirections[i] = 1;
                }

                if (prevCharRow != state->characterCategories[i]) {
                    state->menuStates[i] = CHAR_SELECT_CHAR_ROW_SLIDE;
                    state->completedSlides[i] = 0;
                    state->previousCharacterCategories[i] = prevCharRow;
                    state->previousCharacterVariants[i] = state->characterVariants[i];
                    if (state->characterCategories[i] == 3) {
                        for (k = 0; k < 9; k++) {
                            if (EepromSaveData->characterPaletteIds[9 + k] != 0) {
                                state->characterVariants[i] = k;
                                break;
                            }
                        }
                    } else {
                        state->characterVariants[i] = 0;
                    }

                    secondaryTask = scheduleTask(&initCharSelectSecondarySlot, 2, i, 0x59);
                    if (secondaryTask != 0) {
                        ((CharSelectPreviewTaskState *)secondaryTask)->playerIndex = i;
                    }
                    playSoundEffectOnChannelNoPriority(0x2B, i);
                } else if (gControllerInputs[i] & CONT_A) {
                    state->frameCounters[i] = 0;
                    state->unlockedSlotIndices[i] = 0;
                    state->menuStates[i] = CHAR_SELECT_CHAR_ROW_FLASH;
                    playSoundEffectOnChannelNoPriority(0x2C, i);
                    playSoundEffect(charRowConfirmSoundIds[state->characterCategories[i]]);
                }
                break;

            case CHAR_SELECT_CHAR_ROW_SLIDE:

            case CHAR_SELECT_CHAR_VARIANT_SLIDE:
                if (state->completedSlides[i] != 2) {
                    break;
                }

                state->completedSlides[i] = 0;
                if (state->menuStates[i] == CHAR_SELECT_CHAR_ROW_SLIDE) {
                    state->menuStates[i] = CHAR_SELECT_CHAR_ROW_BROWSE;
                } else {
                    state->menuStates[i] = CHAR_SELECT_CHAR_VARIANT_BROWSE;
                }
                break;

            case CHAR_SELECT_CHAR_ROW_FLASH:

            case CHAR_SELECT_CHAR_CONFIRMED:
                state->frameCounters[i]++;
                if (state->menuStates[i] == CHAR_SELECT_CHAR_CONFIRMED) {
                    if (state->frameCounters[i] & 1) {
                        state->iconDisplayStates[i] = 2;
                    } else {
                        state->iconDisplayStates[i] = 0;
                    }
                }

                if (state->frameCounters[i] != 0x10) {
                    break;
                }

                state->frameCounters[i] = 0;
                state->iconDisplayStates[i] = 0;
                if (state->menuStates[i] == CHAR_SELECT_CHAR_ROW_FLASH) {
                    state->menuStates[i] = CHAR_SELECT_CHAR_VARIANT_BROWSE;
                    if (state->characterCategories[i] == 3) {
                        limit = 9;
                    } else {
                        limit = 3;
                    }
                    numUnlocked = 0;
                    for (j = 0; j < limit; j++) {
                        charIdx = (state->characterCategories[i] * 3) + j;
                        if (EepromSaveData->characterPaletteIds[charIdx] != 0) {
                            unlockedSlots[numUnlocked] = charIdx;
                            numUnlocked++;
                        }
                    }

                    j = countUnlockedSlotsInCategory(state->characterCategories[i]);
                    for (limit = 0; limit < j; limit++) {
                        prevSlotIdx = unlockedSlots[limit];
                        if (state->characterCategories[i] == 3) {
                            prevSlotIdx -= 9;
                        } else {
                            prevSlotIdx %= 3;
                        }

                        if (prevSlotIdx == state->characterVariants[i]) {
                            state->unlockedSlotIndices[i] = limit;
                        }
                    }

                    task = scheduleTask(initCharSelectIcons, 1, i, 0x5A);
                    if (task != 0) {
                        ((CharSelectIconsState *)task)->playerIndex = i;
                        break;
                    }
                } else {
                    state->menuStates[i] = CHAR_SELECT_MENU_NAV;
                    state->cursorIndices[i] = state->maxMenuOption - 2;
                    state->carouselAngles[i] = 0;
                    createYRotationMatrix(&state->characterRotations[i], 0);
                    setViewportLightColors(
                        state->playerViewports[i].viewportId,
                        1,
                        &charSelectNormalLight,
                        (ColorData *)(&charSelectNormalAmbient)
                    );
                }
                break;

            case CHAR_SELECT_CHAR_VARIANT_BROWSE:
                if (gControllerInputs[i] & CONT_B) {
                    playSoundEffectOnChannelNoPriority(0x2E, i);
                    state->menuStates[i] = CHAR_SELECT_CHAR_ROW_BROWSE;
                    terminateTasksByTypeAndID(1, i & 0xFF);
                    break;
                }
                prevSlotIdx = state->unlockedSlotIndices[i];
                if (state->characterCategories[i] == 3) {
                    limit = 9;
                } else {
                    limit = 3;
                }

                numUnlocked = 0;
                for (j = 0; j < limit; j++) {
                    charIdx = (state->characterCategories[i] * 3) + j;
                    if (EepromSaveData->characterPaletteIds[charIdx] != 0) {
                        unlockedSlots[numUnlocked] = charIdx;
                        numUnlocked++;
                    }
                }

                j = countUnlockedSlotsInCategory(state->characterCategories[i]);
                if (gControllerInputs[i] & (STICK_RIGHT | CONT_RIGHT)) {
                    state->unlockedSlotIndices[i]++;
                    if (j - 1 < state->unlockedSlotIndices[i]) {
                        state->unlockedSlotIndices[i] = 0;
                    }
                    state->scrollDirections[i] = 0;
                } else if (gControllerInputs[i] & (STICK_LEFT | CONT_LEFT)) {
                    state->unlockedSlotIndices[i]--;
                    if (state->unlockedSlotIndices[i] < 0) {
                        state->unlockedSlotIndices[i] = j - 1;
                    }
                    state->scrollDirections[i] = 1;
                }

                state->characterVariants[i] = unlockedSlots[state->unlockedSlotIndices[i]];
                if (state->characterCategories[i] == 3) {
                    state->characterVariants[i] -= 9;
                } else {
                    state->characterVariants[i] %= 3;
                }

                if (prevSlotIdx != state->unlockedSlotIndices[i]) {
                    state->menuStates[i] = CHAR_SELECT_CHAR_VARIANT_SLIDE;
                    state->completedSlides[i] = 0;
                    playSoundEffectOnChannelNoPriority(0x2B, i);
                    state->previousCharacterCategories[i] = state->characterCategories[i];
                    state->previousCharacterVariants[i] = unlockedSlots[prevSlotIdx];
                    if (state->characterCategories[i] == 3) {
                        state->previousCharacterVariants[i] -= 9;
                    } else {
                        state->previousCharacterVariants[i] %= 3;
                    }
                    secondaryTask = scheduleTask(&initCharSelectSecondarySlot, 2, i, 0x59);
                    if (secondaryTask != 0) {
                        ((CharSelectPreviewTaskState *)secondaryTask)->playerIndex = i;
                        break;
                    }
                } else {
                    if (gControllerInputs[i] & CONT_A) {
                        state->frameCounters[i] = 0;
                        state->menuStates[i] = CHAR_SELECT_CHAR_CONFIRMED;
                        playSoundEffectOnChannelNoPriority(0x2C, i);
                        gGameSessionContext->snowboardIds[i] = unlockedSlots[state->unlockedSlotIndices[i]];
                        gGameSessionContext->colorSlots[i] =
                            EepromSaveData->characterPaletteIds[gGameSessionContext->snowboardIds[i]] - 1;
                    }
                    break;
                }
                break;

            case CHAR_SELECT_BOARD_BROWSE:
                if (gControllerInputs[i] & CONT_B) {
                    playSoundEffectOnChannelNoPriority(0x2E, i);
                    state->menuStates[i] = CHAR_SELECT_MENU_NAV;
                    state->cursorIndices[i] = state->maxMenuOption - 2;
                    state->carouselAngles[i] = 0;
                    createYRotationMatrix(&state->characterRotations[i], 0);
                    state->boardIds[i] = gGameSessionContext->boardModelIds[i];
                    state->iconDisplayStates[i] = 0;
                    break;
                }
                prevBoardId = state->boardIds[i];
                if (gControllerInputs[i] & (STICK_RIGHT | CONT_RIGHT)) {
                    state->boardIds[i]++;
                    state->scrollDirections[i] = 0;
                } else if (gControllerInputs[i] & (STICK_LEFT | CONT_LEFT)) {
                    state->boardIds[i]--;
                    state->scrollDirections[i] = 1;
                }
                state->boardIds[i] = state->boardIds[i] & 3;
                if (prevBoardId != state->boardIds[i]) {
                    state->menuStates[i] = CHAR_SELECT_BOARD_SLIDE;
                    state->completedSlides[i] = 0;
                    state->previousBoardIds[i] = prevBoardId;
                    playSoundEffectOnChannelNoPriority(0x2B, i);
                    boardTask = scheduleTask(&initCharSelectBoardModelForSlideOut, 3, i, 0x59);
                    if (boardTask != 0) {
                        ((CharSelectBoardPreview *)boardTask)->playerIndex = i;
                        break;
                    }
                } else if (gControllerInputs[i] & CONT_A) {
                    state->menuStates[i] = CHAR_SELECT_BOARD_FLASH;
                    gGameSessionContext->boardModelIds[i] = state->boardIds[i];
                    state->frameCounters[i] = 0;
                    playSoundEffectOnChannelNoPriority(0x2C, i);
                    playSoundEffect(boardConfirmSoundIds[state->boardIds[i]]);
                }
                break;

            case CHAR_SELECT_BOARD_SLIDE:
                if (state->completedSlides[i] != 2) {
                    break;
                }
                state->completedSlides[i] = 0;
                state->menuStates[i] = CHAR_SELECT_BOARD_BROWSE;
                break;

            case CHAR_SELECT_BOARD_FLASH:
                state->frameCounters[i]++;
                if (state->frameCounters[i] & 1) {
                    setViewportLightColors(
                        state->playerViewports[i].viewportId,
                        1,
                        &charSelectFlashLight,
                        (ColorData *)(&charSelectFlashAmbient)
                    );
                } else {
                    setViewportLightColors(
                        state->playerViewports[i].viewportId,
                        1,
                        &charSelectNormalLight,
                        (ColorData *)(&charSelectNormalAmbient)
                    );
                }
                if (state->frameCounters[i] == 0x10) {
                    state->frameCounters[i] = 0;
                    state->menuStates[i] = CHAR_SELECT_MENU_NAV;
                    state->cursorIndices[i] = state->maxMenuOption - 2;
                    state->iconDisplayStates[i] = 0;
                    state->carouselAngles[i] = 0;
                    createYRotationMatrix(&state->characterRotations[i], 0);
                }
                break;

            case CHAR_SELECT_CANCEL_EXIT:
                confirmedCount++;
                break;

            case CHAR_SELECT_READY_CONFIRM:
                if (gGameSessionContext->numPlayers >= 2) {
                    state->menuStates[i] = CHAR_SELECT_P2_NAME_REVEAL;
                } else {
                    state->menuStates[i] = CHAR_SELECT_READY_WAIT;
                    cancelCount++;
                }
                break;

            case CHAR_SELECT_READY_WAIT:
                if (gControllerInputs[i] & CONT_B && gGameSessionContext->numPlayers != 1) {
                    playSoundEffectOnChannelNoPriority(0x2E, i);
                    state->menuStates[i] = CHAR_SELECT_P2_CANCEL;
                } else {
                    cancelCount++;
                }
                break;

            case CHAR_SELECT_P2_CANCEL:
                break;
        }

        if (state->cursorIndices[i] == (state->maxMenuOption - 1) &&
            state->menuStates[i] != CHAR_SELECT_MENU_ROTATING && state->menuStates[i] != CHAR_SELECT_CHAR_CONFIRMED) {
            state->previewSpinAngles[i] = (state->previewSpinAngles[i] + 0x28) & 0x1FFF;
        } else {
            state->previewSpinAngles[i] = 0x800;
        }
    }

    if (confirmedCount == gGameSessionContext->numPlayers) {
        setMusicFadeOut(0xA);
        state->frameCounters[0] = 0x63;
        setGameStateHandler(cleanupCharacterSelect);
        setViewportFadeValue(0, 0xFF, 8);
        return;
    }
    if (cancelCount == gGameSessionContext->numPlayers) {
        setMusicFadeOut(0xA);
        state->frameCounters[0] = 0;
        for (i = 0; i < gGameSessionContext->numPlayers; i++) {
            gGameSessionContext->colorSlots[i] =
                EepromSaveData->characterPaletteIds[gGameSessionContext->snowboardIds[i]] - 1;
        }

        setGameStateHandler(cleanupCharacterSelect);
        setViewportFadeValue(0, 0xFF, 0x10);
        playSoundEffectOnChannelNoPriority(0x2D, 0);
    }
}
void cleanupCharacterSelect(void) {
    CharacterSelectState *state = getCurrentAllocation();
    s32 i;

    if (getViewportFadeMode(0) != 0) {
        return;
    }

    unlinkNode(&state->cameraNode);

    for (i = 0; i < gGameSessionContext->numPlayers; i++) {
        unlinkNode(&state->playerViewports[i]);
        unlinkNode(&state->modelViewports[i]);
        unlinkNode(&state->iconViewports[i]);
    }

    state->menuSpriteAsset = freeNodeMemory(state->menuSpriteAsset);
    state->iconSpriteAsset = freeNodeMemory(state->iconSpriteAsset);

    if (state->frameCounters[0] == 0x63) {
        terminateSchedulerWithCallback(onCharacterSelectCancel);
    } else {
        terminateSchedulerWithCallback(onCharacterSelectProceed);
    }
}

void onCharacterSelectProceed(void) {
    setViewportFadeValue(NULL, 0, 0);
    returnToParentScheduler(1);
}

void onCharacterSelectCancel(void) {
    setViewportFadeValue(NULL, 0, 0);
    returnToParentScheduler(0xFF);
}

s32 countUnlockedSlotsInCategory(u8 category) {
    s32 i;
    s32 count = 0;
    s32 limit = 3;

    if (category == 3) {
        limit = 9;
    }

    for (i = 0; i < limit; i++) {
        if (EepromSaveData->characterPaletteIds[category * 3 + i] != 0) {
            count++;
        }
    }

    return count;
}
