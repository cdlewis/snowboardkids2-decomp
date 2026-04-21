#include "ui/character_select_ui.h"
#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "common.h"
#include "graphics/graphics.h"
#include "math/geometry.h"
#include "race/race_session.h"
#include "story/map_events.h"
#include "system/rom_loader.h"
#include "system/task_scheduler.h"

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
    s16 animAngles[4];
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
    u8 pad18C0[8];
    u8 unk18C8[4];
    u8 maxMenuOption;
    u8 hasSecretCharacters;
    s8 unk18CE[4];
    u8 cursorIndices[4];
    u8 unk18D6[4];
} CharacterSelectState;

typedef struct {
    u8 pad0[0x52];
    u8 playerIndex;
} CharacterSelectTask;

/* Data */

ColorData D_8008DCD0_8E8D0 = { 0x50, 0x50, 0x50, 0x00, 0x00, 0x50, 0x50, 0x00 };
char D_8008DCD8_8E8D8[] = "PPP";
ColorData D_8008DCDC_8E8DC = { 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x50, 0x50, 0x00 };
s32 D_8008DCE4_8E8E4 = 0x7F7F7F00;
ColorData D_8008DCE8_8E8E8 = { 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x50, 0x50, 0x00 };
s32 D_8008DCF0_8E8F0 = (s32)0xFFFFFF00;

u16 D_8008DCF4_8E8F4[] = { 0x012C, 0x012D, 0x012B, 0x012E };

u16 D_8008DCFC_8E8FC[] = {
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

CharSelectItemData D_8008DD8C_8E98C = {
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
void func_80022D74_23974(void);
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
        state->unk18CE[i] = 0;
        state->frameCounters[i] = 0;
        state->zoomValues[i] = 0x800;
        state->unk18C8[i] = 0;
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

    setGameStateHandler(func_80022D74_23974);
}

// 99.03% https://decomp.me/scratch/Knuos
INCLUDE_ASM("asm/nonmatchings/ui/character_select_ui", func_80022D74_23974);

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

INCLUDE_RODATA("asm/nonmatchings/ui/character_select_ui", D_8009E288_9EE88);
