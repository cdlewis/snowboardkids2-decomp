#include "ui/character_select_ui.h"
#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "audio/audio.h"
#include "common.h"
#include "graphics/graphics.h"
#include "math/geometry.h"
#include "os_cont.h"
#include "race/race_session.h"
#include "story/map_events.h"
#include "system/controller_io.h"
#include "system/rom_loader.h"
#include "system/task_scheduler.h"

typedef struct {
    ViewportNode playerViewports[4];
    ViewportNode modelViewports[4];
    ViewportNode iconViewports[4];
    ViewportNode cameraNode;
    Transform3D characterRotations[4];
    void *mainAssets;
    void *iconAssets;
    u16 carouselAngles[4];
    s16 previewSpinAngles[4];
    u8 unused1890[8];
    u16 menuStates[4];
    u16 frameCounters[4];
    u8 charRow[4];
    u8 savedCharRow[4];
    u8 charCol[4];
    u8 savedCharCol[4];
    u8 boardId[4];
    u8 savedBoardId[4];
    u8 scrollDirection[4];
    u8 slideState[4];
    u8 iconDisplayState[4];
    u8 maxMenuOption;
    u8 hasSecretCharacters;
    s8 unlockedSlotIndex[4];
    u8 cursorIndices[4];
    u8 prevCursorIndex[4];
} CharacterSelectState;

typedef struct {
    u8 pad0[0x24];
    u8 iconHidePlayerIndex;
    u8 pad25[3];
    u8 boardModelPlayerIndex;
    u8 pad29[0xB];
    u8 menuPlayerIndex;
    u8 pad35[0x1D];
    u8 iconsPlayerIndex;
    u8 pad53[0x22];
    u8 statsPlayerIndex;
    u8 pad76[0x2B];
    u8 previewModelPlayerIndex;
} CharSelectTaskNode;

extern void initBoardSelectArrows(void);
extern void initBoardSelectCharNames(void);
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

s16 charSelectNamePositions[] = {
    0x0000, 0x0030, (s16)0xFFE8, (s16)0xFFA8, 0x0030, (s16)0xFFD4, (s16)0xFFF0, (s16)0xFFD4,
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

s16 charSelectNameSpritePositions[] = {
    0x0032, 0x0000, 0x0068, (s16)0xFFA8, 0x0070, (s16)0xFFD4, 0x0030, (s16)0xFFD4,
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
    CharSelectTaskNode *task;

    state = (CharacterSelectState *)allocateTaskMemory(0x18E0);
    setupTaskSchedulerNodes(0x30, 8, 4, 8, 0, 0, 0, 0);

    if (gGameSessionContext->gameMode != 0) {
        state->maxMenuOption = 3;
    } else {
        state->maxMenuOption = 2;
    }

    for (i = 0; i < 4; i++) {
        state->menuStates[i] = CHAR_SELECT_MENU_NAV;
        state->unlockedSlotIndex[i] = 0;
        state->frameCounters[i] = 0;
        state->previewSpinAngles[i] = 0x800;
        state->iconDisplayState[i] = 0;
        memcpy((void *)((s32)state + i * 0x20 + 0x17F8), &identityMatrix, sizeof(Transform3D));
        *(s32 *)((s32)state + i * 0x20 + 0x1814) = (s32)0xFFEA0000;
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

    state->mainAssets = loadCompressedData(&menuUiSprites_ROM_START, &playerCountSelectSprites_ROM_START, 0x8A08);
    state->iconAssets = loadCompressedData(&tiledSnowmanAsset_ROM_START, &uiFontSpriteSheet_ROM_START, 0xAE0);

    state->hasSecretCharacters = 0;
    if (countUnlockedSlotsInCategory(3) != 0) {
        state->hasSecretCharacters = 1;
    }

    for (i = 0; i < gGameSessionContext->numPlayers; i++) {
        if (gGameSessionContext->playerBoardIds[4 + i] < 9) {
            state->charRow[i] = gGameSessionContext->playerBoardIds[4 + i] / 3;
            state->charCol[i] = gGameSessionContext->playerBoardIds[4 + i] % 3;
        } else {
            state->charRow[i] = 3;
            state->charCol[i] = gGameSessionContext->playerBoardIds[4 + i] - 9;
        }
        state->savedCharRow[i] = state->charRow[i];
        state->savedCharCol[i] = state->charCol[i];
        boardId = gGameSessionContext->playerBoardIds[12 + i];
        state->boardId[i] = boardId;
        state->savedBoardId[i] = boardId;
        task = (CharSelectTaskNode *)scheduleTask(initCharSelectBoardModel, 0, 0, 0x5A);
        task->boardModelPlayerIndex = i;
        task = (CharSelectTaskNode *)scheduleTask(initCharSelectPreviewModel, 0, 0, 0x5A);
        task->previewModelPlayerIndex = i;
        task = (CharSelectTaskNode *)scheduleTask(initCharSelectMenu, 0, 0, 0x5A);
        task->menuPlayerIndex = i;
        task = (CharSelectTaskNode *)scheduleTask(initCharSelectIconHideSprites, 0, 0, 0x5A);
        task->iconHidePlayerIndex = i;
        task = (CharSelectTaskNode *)scheduleTask(initCharSelectStats, 0, 0, 0x5A);
        task->statsPlayerIndex = i;
        task = (CharSelectTaskNode *)scheduleTask(initCharSelectPlayer2NameSprites, 0, 0, 0x5A);
        task->iconHidePlayerIndex = i;
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
    CharSelectTaskNode *task;

    if (getViewportFadeMode(0) != 0) {
        return;
    }

    for (i = 0; i < gGameSessionContext->numPlayers; i++) {
        task = (CharSelectTaskNode *)scheduleTask(initCharSelectIcons, 1, i, 0x5A);
        if (task != NULL) {
            task->iconsPlayerIndex = i;
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
    CharSelectTaskNode *task;
    CharSelectTaskNode *secTask;
    CharSelectTaskNode *boardTask;
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
                    state->prevCursorIndex[i] = prevCursorIdx;
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
                    rotDir = (state->prevCursorIndex[i] == (state->maxMenuOption - 1)) ? 0x200 : -0x200;
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
                    state->iconDisplayState[i] = 1;
                } else if (state->cursorIndices[i] == (state->maxMenuOption - 2)) {
                    state->menuStates[i] = CHAR_SELECT_READY_CONFIRM;
                }
                break;

            case CHAR_SELECT_CHAR_ROW_BROWSE:
                if (gControllerInputs[i] & CONT_B) {
                    playSoundEffectOnChannelNoPriority(0x2E, i);
                    state->menuStates[i] = CHAR_SELECT_MENU_NAV;
                    task = (CharSelectTaskNode *)scheduleTask(initCharSelectIcons, 1, i, 0x5A);
                    if (task != 0) {
                        task->iconsPlayerIndex = i;
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
                    if (gGameSessionContext->playerBoardIds[4 + i] < 9) {
                        state->charRow[i] = gGameSessionContext->playerBoardIds[4 + i] / 3;
                        state->charCol[i] = gGameSessionContext->playerBoardIds[4 + i] % 3;
                    } else {
                        state->charRow[i] = 3;
                        state->charCol[i] = gGameSessionContext->playerBoardIds[4 + i] - 9;
                    }
                    break;
                }

                prevCharRow = state->charRow[i];
                if (gControllerInputs[i] & (STICK_RIGHT | CONT_RIGHT)) {
                    state->charRow[i]++;
                    if ((state->hasSecretCharacters + 2) < (state->charRow[i] & 0xFF)) {
                        state->charRow[i] = 0;
                    }
                    state->scrollDirection[i] = 0;
                } else if (gControllerInputs[i] & (STICK_LEFT | CONT_LEFT)) {
                    state->charRow[i]--;
                    if ((state->hasSecretCharacters + 2) < (state->charRow[i] & 0xFF)) {
                        state->charRow[i] = state->hasSecretCharacters + 2;
                    }
                    state->scrollDirection[i] = 1;
                }

                if (prevCharRow != state->charRow[i]) {
                    state->menuStates[i] = CHAR_SELECT_CHAR_ROW_SLIDE;
                    state->slideState[i] = 0;
                    state->savedCharRow[i] = prevCharRow;
                    state->savedCharCol[i] = state->charCol[i];
                    if (state->charRow[i] == 3) {
                        for (k = 0; k < 9; k++) {
                            if (EepromSaveData->character_or_settings[9 + k] != 0) {
                                state->charCol[i] = k;
                                break;
                            }
                        }
                    } else {
                        state->charCol[i] = 0;
                    }

                    secTask = (CharSelectTaskNode *)scheduleTask(&initCharSelectSecondarySlot, 2, i, 0x59);
                    if (secTask != 0) {
                        secTask->previewModelPlayerIndex = i;
                    }
                    playSoundEffectOnChannelNoPriority(0x2B, i);
                } else if (gControllerInputs[i] & CONT_A) {
                    state->frameCounters[i] = 0;
                    state->unlockedSlotIndex[i] = 0;
                    state->menuStates[i] = CHAR_SELECT_CHAR_ROW_FLASH;
                    playSoundEffectOnChannelNoPriority(0x2C, i);
                    playSoundEffect(charRowConfirmSoundIds[state->charRow[i]]);
                }
                break;

            case CHAR_SELECT_CHAR_ROW_SLIDE:

            case CHAR_SELECT_CHAR_VARIANT_SLIDE:
                if (state->slideState[i] != 2) {
                    break;
                }

                state->slideState[i] = 0;
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
                        state->iconDisplayState[i] = 2;
                    } else {
                        state->iconDisplayState[i] = 0;
                    }
                }

                if (state->frameCounters[i] != 0x10) {
                    break;
                }

                state->frameCounters[i] = 0;
                state->iconDisplayState[i] = 0;
                if (state->menuStates[i] == CHAR_SELECT_CHAR_ROW_FLASH) {
                    state->menuStates[i] = CHAR_SELECT_CHAR_VARIANT_BROWSE;
                    if (state->charRow[i] == 3) {
                        limit = 9;
                    } else {
                        limit = 3;
                    }
                    numUnlocked = 0;
                    for (j = 0; j < limit; j++) {
                        charIdx = (state->charRow[i] * 3) + j;
                        if (EepromSaveData->character_or_settings[charIdx] != 0) {
                            unlockedSlots[numUnlocked] = charIdx;
                            numUnlocked++;
                        }
                    }

                    j = countUnlockedSlotsInCategory(state->charRow[i]);
                    for (limit = 0; limit < j; limit++) {
                        prevSlotIdx = unlockedSlots[limit];
                        if (state->charRow[i] == 3) {
                            prevSlotIdx -= 9;
                        } else {
                            prevSlotIdx %= 3;
                        }

                        if (prevSlotIdx == state->charCol[i]) {
                            state->unlockedSlotIndex[i] = limit;
                        }
                    }

                    task = (CharSelectTaskNode *)scheduleTask(initCharSelectIcons, 1, i, 0x5A);
                    if (task != 0) {
                        task->iconsPlayerIndex = i;
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
                prevSlotIdx = state->unlockedSlotIndex[i];
                if (state->charRow[i] == 3) {
                    limit = 9;
                } else {
                    limit = 3;
                }

                numUnlocked = 0;
                for (j = 0; j < limit; j++) {
                    charIdx = (state->charRow[i] * 3) + j;
                    if (EepromSaveData->character_or_settings[charIdx] != 0) {
                        unlockedSlots[numUnlocked] = charIdx;
                        numUnlocked++;
                    }
                }

                j = countUnlockedSlotsInCategory(state->charRow[i]);
                if (gControllerInputs[i] & (STICK_RIGHT | CONT_RIGHT)) {
                    state->unlockedSlotIndex[i]++;
                    if (j - 1 < state->unlockedSlotIndex[i]) {
                        state->unlockedSlotIndex[i] = 0;
                    }
                    state->scrollDirection[i] = 0;
                } else if (gControllerInputs[i] & (STICK_LEFT | CONT_LEFT)) {
                    state->unlockedSlotIndex[i]--;
                    if (state->unlockedSlotIndex[i] < 0) {
                        state->unlockedSlotIndex[i] = j - 1;
                    }
                    state->scrollDirection[i] = 1;
                }

                state->charCol[i] = unlockedSlots[state->unlockedSlotIndex[i]];
                if (state->charRow[i] == 3) {
                    state->charCol[i] -= 9;
                } else {
                    state->charCol[i] %= 3;
                }

                if (prevSlotIdx != state->unlockedSlotIndex[i]) {
                    state->menuStates[i] = CHAR_SELECT_CHAR_VARIANT_SLIDE;
                    state->slideState[i] = 0;
                    playSoundEffectOnChannelNoPriority(0x2B, i);
                    state->savedCharRow[i] = state->charRow[i];
                    state->savedCharCol[i] = unlockedSlots[prevSlotIdx];
                    if (state->charRow[i] == 3) {
                        state->savedCharCol[i] -= 9;
                    } else {
                        state->savedCharCol[i] %= 3;
                    }
                    secTask = (CharSelectTaskNode *)scheduleTask(&initCharSelectSecondarySlot, 2, i, 0x59);
                    if (secTask != 0) {
                        secTask->previewModelPlayerIndex = i;
                        break;
                    }
                } else {
                    if (gControllerInputs[i] & CONT_A) {
                        state->frameCounters[i] = 0;
                        state->menuStates[i] = CHAR_SELECT_CHAR_CONFIRMED;
                        playSoundEffectOnChannelNoPriority(0x2C, i);
                        gGameSessionContext->playerBoardIds[4 + i] = unlockedSlots[state->unlockedSlotIndex[i]];
                        gGameSessionContext->playerBoardIds[8 + i] =
                            EepromSaveData->character_or_settings[gGameSessionContext->playerBoardIds[4 + i]] - 1;
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
                    state->boardId[i] = gGameSessionContext->playerBoardIds[12 + i];
                    state->iconDisplayState[i] = 0;
                    break;
                }
                prevBoardId = state->boardId[i];
                if (gControllerInputs[i] & (STICK_RIGHT | CONT_RIGHT)) {
                    state->boardId[i]++;
                    state->scrollDirection[i] = 0;
                } else if (gControllerInputs[i] & (STICK_LEFT | CONT_LEFT)) {
                    state->boardId[i]--;
                    state->scrollDirection[i] = 1;
                }
                state->boardId[i] = state->boardId[i] & 3;
                if (prevBoardId != state->boardId[i]) {
                    state->menuStates[i] = CHAR_SELECT_BOARD_SLIDE;
                    state->slideState[i] = 0;
                    state->savedBoardId[i] = prevBoardId;
                    playSoundEffectOnChannelNoPriority(0x2B, i);
                    boardTask = (CharSelectTaskNode *)scheduleTask(&initCharSelectBoardModelForSlideOut, 3, i, 0x59);
                    if (boardTask != 0) {
                        boardTask->boardModelPlayerIndex = i;
                        break;
                    }
                } else if (gControllerInputs[i] & CONT_A) {
                    state->menuStates[i] = CHAR_SELECT_BOARD_FLASH;
                    gGameSessionContext->playerBoardIds[12 + i] = state->boardId[i];
                    state->frameCounters[i] = 0;
                    playSoundEffectOnChannelNoPriority(0x2C, i);
                    playSoundEffect(boardConfirmSoundIds[state->boardId[i]]);
                }
                break;

            case CHAR_SELECT_BOARD_SLIDE:
                if (state->slideState[i] != 2) {
                    break;
                }
                state->slideState[i] = 0;
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
                    state->iconDisplayState[i] = 0;
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
            gGameSessionContext->playerBoardIds[8 + i] =
                EepromSaveData->character_or_settings[gGameSessionContext->playerBoardIds[4 + i]] - 1;
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

    state->mainAssets = freeNodeMemory(state->mainAssets);
    state->iconAssets = freeNodeMemory(state->iconAssets);

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
        if (EepromSaveData->character_or_settings[category * 3 + i] != 0) {
            count++;
        }
    }

    return count;
}
