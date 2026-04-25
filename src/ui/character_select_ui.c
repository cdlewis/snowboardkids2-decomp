#include "ui/character_select_ui.h"
#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "audio/audio.h"
#include "common.h"
#include "graphics/graphics.h"
#include "math/geometry.h"
#include "race/race_session.h"
#include "story/map_events.h"
#include "system/rom_loader.h"
#include "system/task_scheduler.h"

extern s32 gControllerInputs[4];
extern void initCharSelectSecondarySlot(void);
extern void initCharSelectBoardModelForSlideOut(void);
void cleanupCharacterSelect(void);

typedef struct {
    u16 x;
    u16 y;
} Vec2_u16;

typedef struct {
    u16 x;
    u16 y;
    s16 inc;
} PositionConfig_DDBE;

typedef struct {
    u16 y;
    u16 x;
    u16 inc;
} PositionConfig_DDE6;

typedef struct {
    u16 x;
    u16 y;
    u16 inc;
} PositionConfig_DE1A;

typedef struct {
    ViewportNode playerViewports[4];
    ViewportNode modelViewports[4];
    ViewportNode iconViewports[4];
    ViewportNode cameraNode;
    Transform3D pad17F8[4];
    void *mainAssets;
    void *iconAssets;
    u16 animAngles[4];
    s16 zoomValues[4];
    u8 pad1890[8];
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
    u8 pad0[0x52];
    u8 playerIndex;
} CharacterSelectTask;

typedef struct {
    u8 pad0[0xA1];
    u8 playerIndex;
} SecondarySlotTask;

typedef struct {
    u8 pad0[0x28];
    u8 playerIndex;
} BoardModelTask;

/* Data */

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

CharSelectModelPositions D_8008DD2C_8E92C = {
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

CharSelectStatsPositions D_8008DDEC_8E9EC = {
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

CharSelectArrowData D_8008DE18_8EA18 = {
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

s16 D_8008DE54_8EA54[] = {
    0x0000, 0x0030, (s16)0xFFE8, (s16)0xFFA8, 0x0030, (s16)0xFFD4, (s16)0xFFF0, (s16)0xFFD4,
};

CharSelectBoardData D_8008DE64_8EA64 = {
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

s16 D_8008DE9C_8EA9C[] = {
    0x0032, 0x0000, 0x0068, (s16)0xFFA8, 0x0070, (s16)0xFFD4, 0x0030, (s16)0xFFD4,
};

CharSelectAnimData D_8008DEAC_8EAAC = {
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

/* Function declarations */

void awaitCharacterSelectLoad(void);
void scheduleCharacterSelectTasks(void);
void updateCharacterSelect(void);
void onCharacterSelectProceed(void);
void onCharacterSelectCancel(void);

USE_ASSET(_4237C0);
USE_ASSET(_426EF0);
USE_ASSET(_458E30);
USE_ASSET(_459310);

extern void initBoardSelectArrows(void);
extern void initBoardSelectCharNames(void);
extern void initCharSelectArrows(void);
extern void initCharSelectBackgroundEffect(void);
extern void initCharSelectBoardModel(void);
extern void initCharSelectIconHideSprites(void);
extern void initCharSelectMenu(void);
extern void initCharSelectNameSprites(void);
extern void initCharSelectPlayer2NameSprites(void);
extern void initCharSelectPlayerLabels(void);
extern void initCharSelectPlayerNumbers(void);
extern void initCharSelectPreviewModel(void);
extern void initCharSelectStats(void);

typedef struct {
    u8 pad0[0x24];
    u8 unk24;
    u8 pad25[3];
    u8 unk28;
    u8 pad29[0xB];
    u8 unk34;
    u8 pad35[0x40];
    u8 unk75;
    u8 pad76[0x2B];
    u8 unkA1;
} GenericTaskNode;

void initCharacterSelectScreen(void) {
    CharacterSelectState *state;
    Transform3D transform;
    s32 i;
    s32 numOptions;
    u8 boardId;
    GenericTaskNode *task;

    state = (CharacterSelectState *)allocateTaskMemory(0x18E0);
    setupTaskSchedulerNodes(0x30, 8, 4, 8, 0, 0, 0, 0);

    if (D_800AFE8C_A71FC->gameMode != 0) {
        state->maxMenuOption = 3;
    } else {
        state->maxMenuOption = 2;
    }

    for (i = 0; i < 4; i++) {
        state->menuStates[i] = 0;
        state->unlockedSlotIndex[i] = 0;
        state->frameCounters[i] = 0;
        state->zoomValues[i] = 0x800;
        state->iconDisplayState[i] = 0;
        memcpy((void *)((s32)state + i * 0x20 + 0x17F8), &identityMatrix, 0x20);
        *(s32 *)((s32)state + i * 0x20 + 0x1814) = (s32)0xFFEA0000;
        state->animAngles[i] = 0;
        state->cursorIndices[i] = (s8)(state->maxMenuOption - 2);
    }

    initMenuCameraNode(&state->cameraNode, 7, 1, 1);

    switch (D_800AFE8C_A71FC->numPlayers) {
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

    if (D_800AFE8C_A71FC->numPlayers == 1) {
        createViewportTransform(&transform, 0, 0, 0x540000, 0, 0, 0);
    } else if (D_800AFE8C_A71FC->numPlayers == 2) {
        createViewportTransform(&transform, 0, 0, 0x480000, 0, 0, 0);
    } else {
        createViewportTransform(&transform, 0, 0, 0x380000, 0, 0, 0);
    }

    for (i = 0; i < D_800AFE8C_A71FC->numPlayers; i++) {
        setViewportTransformById(state->playerViewports[i].id, &transform);
    }

    state->mainAssets = loadCompressedData(&_4237C0_ROM_START, &_426EF0_ROM_START, 0x8A08);
    state->iconAssets = loadCompressedData(&_458E30_ROM_START, &_459310_ROM_START, 0xAE0);

    state->hasSecretCharacters = 0;
    if (countUnlockedSlotsInCategory(3) != 0) {
        state->hasSecretCharacters = 1;
    }

    for (i = 0; i < D_800AFE8C_A71FC->numPlayers; i++) {
        if (D_800AFE8C_A71FC->playerBoardIds[4 + i] < 9) {
            state->charRow[i] = D_800AFE8C_A71FC->playerBoardIds[4 + i] / 3;
            state->charCol[i] = D_800AFE8C_A71FC->playerBoardIds[4 + i] % 3;
        } else {
            state->charRow[i] = 3;
            state->charCol[i] = D_800AFE8C_A71FC->playerBoardIds[4 + i] - 9;
        }
        state->savedCharRow[i] = state->charRow[i];
        state->savedCharCol[i] = state->charCol[i];
        boardId = D_800AFE8C_A71FC->playerBoardIds[12 + i];
        state->boardId[i] = boardId;
        state->savedBoardId[i] = boardId;
        task = (GenericTaskNode *)scheduleTask(initCharSelectBoardModel, 0, 0, 0x5A);
        task->unk28 = i;
        task = (GenericTaskNode *)scheduleTask(initCharSelectPreviewModel, 0, 0, 0x5A);
        task->unkA1 = i;
        task = (GenericTaskNode *)scheduleTask(initCharSelectMenu, 0, 0, 0x5A);
        task->unk34 = i;
        task = (GenericTaskNode *)scheduleTask(initCharSelectIconHideSprites, 0, 0, 0x5A);
        task->unk24 = i;
        task = (GenericTaskNode *)scheduleTask(initCharSelectStats, 0, 0, 0x5A);
        task->unk75 = i;
        task = (GenericTaskNode *)scheduleTask(initCharSelectPlayer2NameSprites, 0, 0, 0x5A);
        task->unk24 = i;
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
    CharacterSelectTask *task;

    if (getViewportFadeMode(0) != 0) {
        return;
    }

    for (i = 0; i < D_800AFE8C_A71FC->numPlayers; i++) {
        task = (CharacterSelectTask *)scheduleTask(initCharSelectIcons, 1, i, 0x5A);
        if (task != NULL) {
            task->playerIndex = i;
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
    CharacterSelectTask *task;
    SecondarySlotTask *secTask;
    BoardModelTask *boardTask;
    s32 *temp3;
    void *eepromResult;
    state = (CharacterSelectState *)getCurrentAllocation();

    cancelCount = 0;
    confirmedCount = 0;
    for (i = 0; i < D_800AFE8C_A71FC->numPlayers; i++) {
        prevCursorIdx = state->cursorIndices[i];
        switch (state->menuStates[i]) {
            case 0:
                if (gControllerInputs[i] & 0x40100) {
                    if (state->cursorIndices[i] < (state->maxMenuOption - 1)) {
                        state->cursorIndices[i]++;
                    }
                } else if (gControllerInputs[i] & 0x80200) {
                    if (state->cursorIndices[i] > 0) {
                        state->cursorIndices[i]--;
                    }
                }
                if (prevCursorIdx != state->cursorIndices[i]) {
                    state->menuStates[i] = 1;
                    state->prevCursorIndex[i] = prevCursorIdx;
                    playSoundEffectOnChannelNoPriority(0x2B, i);
                } else if (gControllerInputs[i] & 0x8000) {
                    state->menuStates[i] = 2;
                    playSoundEffectOnChannelNoPriority(0x2C, i);
                } else if (gControllerInputs[i] & 0x4000) {
                    playSoundEffect(0x2E);
                    for (j = 0; j < D_800AFE8C_A71FC->numPlayers; j++) {
                        state->menuStates[j] = 0x14;
                    }

                    i = j;
                }

                break;

            case 1:
                if (state->cursorIndices[i] == (state->maxMenuOption - 1)) {
                    rotDir = -0x200;
                } else if (state->cursorIndices[i] == (state->maxMenuOption - 3)) {
                    rotDir = 0x200;
                } else {
                    rotDir = (state->prevCursorIndex[i] == (state->maxMenuOption - 1)) ? 0x200 : -0x200;
                }

                state->animAngles[i] = (state->animAngles[i] + rotDir) & 0x1FFF;
                if (state->animAngles[i] == 0x1800 || state->animAngles[i] == 0x800 || state->animAngles[i] == 0) {
                    state->menuStates[i] = 0;
                }
                createYRotationMatrix(&state->pad17F8[i], state->animAngles[i]);
                break;

            case 2:
                state->frameCounters[i]++;
                if (state->frameCounters[i] != 0x10) {
                    break;
                }

                state->menuStates[i] = 3;
                state->frameCounters[i] = 0;
                if (state->cursorIndices[i] == (state->maxMenuOption - 1)) {
                    setViewportLightColors(
                        state->playerViewports[i].id,
                        1,
                        &charSelectDimLight,
                        (ColorData *)charSelectDimAmbientStr
                    );
                    terminateTasksByTypeAndID(1, i & 0xFF);
                } else if (state->cursorIndices[i] == (state->maxMenuOption - 3)) {
                    state->menuStates[i] = 0xF;
                    state->iconDisplayState[i] = 1;
                } else if (state->cursorIndices[i] == (state->maxMenuOption - 2)) {
                    state->menuStates[i] = 0x19;
                }
                break;

            case 3:
                if (gControllerInputs[i] & 0x4000) {
                    playSoundEffectOnChannelNoPriority(0x2E, i);
                    state->menuStates[i] = 0;
                    task = (CharacterSelectTask *)scheduleTask(initCharSelectIcons, 1, i, 0x5A);
                    if (task != 0) {
                        task->playerIndex = i;
                    }
                    state->cursorIndices[i] = state->maxMenuOption - 2;
                    state->animAngles[i] = 0;
                    createYRotationMatrix(&state->pad17F8[i], 0);
                    setViewportLightColors(
                        state->playerViewports[i].id,
                        1,
                        &charSelectNormalLight,
                        (ColorData *)(&charSelectNormalAmbient)
                    );
                    if (D_800AFE8C_A71FC->playerBoardIds[4 + i] < 9) {
                        state->charRow[i] = D_800AFE8C_A71FC->playerBoardIds[4 + i] / 3;
                        state->charCol[i] = D_800AFE8C_A71FC->playerBoardIds[4 + i] % 3;
                    } else {
                        state->charRow[i] = 3;
                        state->charCol[i] = D_800AFE8C_A71FC->playerBoardIds[4 + i] - 9;
                    }
                    break;
                }

                prevCharRow = state->charRow[i];
                if (gControllerInputs[i] & 0x40100) {
                    state->charRow[i]++;
                    if ((state->hasSecretCharacters + 2) < (state->charRow[i] & 0xFF)) {
                        state->charRow[i] = 0;
                    }
                    state->scrollDirection[i] = 0;
                } else if (gControllerInputs[i] & 0x80200) {
                    state->charRow[i]--;
                    if ((state->hasSecretCharacters + 2) < (state->charRow[i] & 0xFF)) {
                        state->charRow[i] = state->hasSecretCharacters + 2;
                    }
                    state->scrollDirection[i] = 1;
                }

                if (prevCharRow != state->charRow[i]) {
                    state->menuStates[i] = 4;
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

                    secTask = (SecondarySlotTask *)scheduleTask(&initCharSelectSecondarySlot, 2, i, 0x59);
                    if (secTask != 0) {
                        secTask->playerIndex = i;
                    }
                    playSoundEffectOnChannelNoPriority(0x2B, i);
                } else if (gControllerInputs[i] & 0x8000) {
                    state->frameCounters[i] = 0;
                    state->unlockedSlotIndex[i] = 0;
                    state->menuStates[i] = 5;
                    playSoundEffectOnChannelNoPriority(0x2C, i);
                    playSoundEffect(charRowConfirmSoundIds[state->charRow[i]]);
                }
                break;

            case 4:

            case 9:
                if (state->slideState[i] != 2) {
                    break;
                }

                state->slideState[i] = 0;
                if (state->menuStates[i] == 4) {
                    state->menuStates[i] = 3;
                } else {
                    state->menuStates[i] = 10;
                }
                break;

            case 5:

            case 6:
                state->frameCounters[i]++;
                if (state->menuStates[i] == 6) {
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
                if (state->menuStates[i] == 5) {
                    state->menuStates[i] = 10;
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

                    task = (CharacterSelectTask *)scheduleTask(initCharSelectIcons, 1, i, 0x5A);
                    if (task != 0) {
                        task->playerIndex = i;
                        break;
                    }
                } else {
                    state->menuStates[i] = 0;
                    state->cursorIndices[i] = state->maxMenuOption - 2;
                    state->animAngles[i] = 0;
                    createYRotationMatrix(&state->pad17F8[i], 0);
                    setViewportLightColors(
                        state->playerViewports[i].id,
                        1,
                        &charSelectNormalLight,
                        (ColorData *)(&charSelectNormalAmbient)
                    );
                }
                break;

            case 10:
                if (gControllerInputs[i] & 0x4000) {
                    playSoundEffectOnChannelNoPriority(0x2E, i);
                    state->menuStates[i] = 3;
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
                if (gControllerInputs[i] & 0x40100) {
                    state->unlockedSlotIndex[i]++;
                    if (j - 1 < state->unlockedSlotIndex[i]) {
                        state->unlockedSlotIndex[i] = 0;
                    }
                    state->scrollDirection[i] = 0;
                } else if (gControllerInputs[i] & 0x80200) {
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
                    state->menuStates[i] = 9;
                    state->slideState[i] = 0;
                    playSoundEffectOnChannelNoPriority(0x2B, i);
                    state->savedCharRow[i] = state->charRow[i];
                    state->savedCharCol[i] = unlockedSlots[prevSlotIdx];
                    if (state->charRow[i] == 3) {
                        state->savedCharCol[i] -= 9;
                    } else {
                        state->savedCharCol[i] %= 3;
                    }
                    secTask = (SecondarySlotTask *)scheduleTask(&initCharSelectSecondarySlot, 2, i, 0x59);
                    if (secTask != 0) {
                        secTask->playerIndex = i;
                        break;
                    }
                } else {
                    if (gControllerInputs[i] & 0x8000) {
                        state->frameCounters[i] = 0;
                        state->menuStates[i] = 6;
                        playSoundEffectOnChannelNoPriority(0x2C, i);
                        D_800AFE8C_A71FC->playerBoardIds[4 + i] = unlockedSlots[state->unlockedSlotIndex[i]];
                        D_800AFE8C_A71FC->playerBoardIds[8 + i] =
                            EepromSaveData->character_or_settings[D_800AFE8C_A71FC->playerBoardIds[4 + i]] - 1;
                    }
                    break;
                }
                break;

            case 15:
                if (gControllerInputs[i] & 0x4000) {
                    playSoundEffectOnChannelNoPriority(0x2E, i);
                    state->menuStates[i] = 0;
                    state->cursorIndices[i] = state->maxMenuOption - 2;
                    state->animAngles[i] = 0;
                    createYRotationMatrix(&state->pad17F8[i], 0);
                    state->boardId[i] = D_800AFE8C_A71FC->playerBoardIds[12 + i];
                    state->iconDisplayState[i] = 0;
                    break;
                }
                prevBoardId = state->boardId[i];
                if (gControllerInputs[i] & 0x40100) {
                    state->boardId[i]++;
                    state->scrollDirection[i] = 0;
                } else if (gControllerInputs[i] & 0x80200) {
                    state->boardId[i]--;
                    state->scrollDirection[i] = 1;
                }
                state->boardId[i] = state->boardId[i] & 3;
                if (prevBoardId != state->boardId[i]) {
                    state->menuStates[i] = 0x10;
                    state->slideState[i] = 0;
                    state->savedBoardId[i] = prevBoardId;
                    playSoundEffectOnChannelNoPriority(0x2B, i);
                    boardTask = (BoardModelTask *)scheduleTask(&initCharSelectBoardModelForSlideOut, 3, i, 0x59);
                    if (boardTask != 0) {
                        boardTask->playerIndex = i;
                        break;
                    }
                } else if (gControllerInputs[i] & 0x8000) {
                    state->menuStates[i] = 0x11;
                    D_800AFE8C_A71FC->playerBoardIds[12 + i] = state->boardId[i];
                    state->frameCounters[i] = 0;
                    playSoundEffectOnChannelNoPriority(0x2C, i);
                    playSoundEffect(boardConfirmSoundIds[state->boardId[i]]);
                }
                break;

            case 16:
                if (state->slideState[i] != 2) {
                    break;
                }
                state->slideState[i] = 0;
                state->menuStates[i] = 0xF;
                break;

            case 17:
                state->frameCounters[i]++;
                if (state->frameCounters[i] & 1) {
                    setViewportLightColors(
                        state->playerViewports[i].id,
                        1,
                        &charSelectFlashLight,
                        (ColorData *)(&charSelectFlashAmbient)
                    );
                } else {
                    setViewportLightColors(
                        state->playerViewports[i].id,
                        1,
                        &charSelectNormalLight,
                        (ColorData *)(&charSelectNormalAmbient)
                    );
                }
                if (state->frameCounters[i] == 0x10) {
                    state->frameCounters[i] = 0;
                    state->menuStates[i] = 0;
                    state->cursorIndices[i] = state->maxMenuOption - 2;
                    state->iconDisplayState[i] = 0;
                    state->animAngles[i] = 0;
                    createYRotationMatrix(&state->pad17F8[i], 0);
                }
                break;

            case 20:
                confirmedCount++;
                break;

            case 25:
                if (D_800AFE8C_A71FC->numPlayers >= 2) {
                    state->menuStates[i] = 0x1A;
                } else {
                    state->menuStates[i] = 0x1B;
                    cancelCount++;
                }
                break;

            case 27:
                if (gControllerInputs[i] & 0x4000 && D_800AFE8C_A71FC->numPlayers != 1) {
                    playSoundEffectOnChannelNoPriority(0x2E, i);
                    state->menuStates[i] = 0x1E;
                } else {
                    cancelCount++;
                }
                break;

            case 30:
                break;
        }

        if (state->cursorIndices[i] == (state->maxMenuOption - 1) && state->menuStates[i] != 1 &&
            state->menuStates[i] != 6) {
            state->zoomValues[i] = (state->zoomValues[i] + 0x28) & 0x1FFF;
        } else {
            state->zoomValues[i] = 0x800;
        }
    }

    if (confirmedCount == D_800AFE8C_A71FC->numPlayers) {
        setMusicFadeOut(0xA);
        state->frameCounters[0] = 0x63;
        setGameStateHandler(cleanupCharacterSelect);
        setViewportFadeValue(0, 0xFF, 8);
        return;
    }
    if (cancelCount == D_800AFE8C_A71FC->numPlayers) {
        setMusicFadeOut(0xA);
        state->frameCounters[0] = 0;
        for (i = 0; i < D_800AFE8C_A71FC->numPlayers; i++) {
            D_800AFE8C_A71FC->playerBoardIds[8 + i] =
                EepromSaveData->character_or_settings[D_800AFE8C_A71FC->playerBoardIds[4 + i]] - 1;
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

    for (i = 0; i < D_800AFE8C_A71FC->numPlayers; i++) {
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
