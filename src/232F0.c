#include "232F0.h"
#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "common.h"
#include "graphics.h"
#include "race_session.h"
#include "rom_loader.h"
#include "task_scheduler.h"

typedef struct {
    ViewportNode unk0[4];
    ViewportNode unk760[4];
    ViewportNode unkEC0[4];
    ViewportNode unk1620;
    u8 pad17F8[0x1878 - (0x1620 + 0x1D8)];
    void *unk1878;
    void *unk187C;
    u8 pad1880[0x18A0 - 0x1880];
    u16 stateCounter;
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

void scheduleCharacterSelectTasks(void);
void func_80022D74_23974(void);
void onCharacterSelectProceed(void);
void onCharacterSelectCancel(void);

INCLUDE_ASM("asm/nonmatchings/232F0", func_800226F0_232F0);

void awaitCharacterSelectLoad(void) {
    CharacterSelectState *state = (CharacterSelectState *)getCurrentAllocation();

    state->stateCounter++;
    if (state->stateCounter < 3) {
        return;
    }
    state->stateCounter = 2;
    if (getPendingDmaCount() != 0) {
        return;
    }
    state->stateCounter = 0;
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

INCLUDE_ASM("asm/nonmatchings/232F0", func_80022D74_23974);

void cleanupCharacterSelect(void) {
    CharacterSelectState *state = getCurrentAllocation();
    s32 i;

    if (getViewportFadeMode(0) != 0) {
        return;
    }

    unlinkNode(&state->unk1620);

    for (i = 0; i < D_800AFE8C_A71FC->numPlayers; i++) {
        unlinkNode(&state->unk0[i]);
        unlinkNode(&state->unk760[i]);
        unlinkNode(&state->unkEC0[i]);
    }

    state->unk1878 = freeNodeMemory(state->unk1878);
    state->unk187C = freeNodeMemory(state->unk187C);

    if (state->stateCounter == 0x63) {
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

INCLUDE_RODATA("asm/nonmatchings/232F0", D_8009E288_9EE88);
