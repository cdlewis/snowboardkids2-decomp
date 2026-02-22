#include "288A0.h"
#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "audio.h"
#include "common.h"
#include "gamestate.h"
#include "graphics.h"
#include "rom_loader.h"
#include "task_scheduler.h"

USE_OVERLAY(_1DA660)
USE_OVERLAY(_41AD80)
USE_OVERLAY(_41A1D0)

u8 storyMapLocationOrder1[] = { 0x04, 0x00, 0x02, 0x08, 0x07, 0x03, 0x05, 0x01, 0x06, 0x00, 0x00, 0x00 };
u8 storyMapLocationOrder2[] = { 0x01, 0x07, 0x02, 0x05, 0x00, 0x06, 0x08, 0x04, 0x03, 0x00, 0x00, 0x00 };
s16 storyMapLocationPositions[] = { 0x00BC, 0x0050, 0x0084, 0x00DE, 0x00F2, 0x00A7, 0x00C7, 0x006C, 0x0102, 0x0000 };
s16 storyMapLocationSpriteIds[] = { 0x0126, 0x0122, 0x0124, 0x0128, 0x0129, 0x0125, 0x0127, 0x0123, 0x012A, 0x0000 };
u8 storyMapLocationConfig[] = { 0x0A, 0x0A, 0x0F, 0x0A, 0x19, 0x0A, 0x0D, 0x0A, 0x19, 0x00, 0x00, 0x00 };
s16 storyMapLocationOrder3[] = { 0x0000, 0x0002, 0x0004, 0x0008, 0x0007, 0x0003, 0x0005, 0x0001, 0x0006, 0x0000 };

extern void func_8001B3E8_1BFE8(void);

extern void initCharacterPreview(void);
extern void initCharacterSelectBoardTask(void);
extern void initCharacterSelectSprites(void);
extern void initCharacterSelectTextureDataLoad(void);
extern void osViExtendVStart(u32);

void func_8001B020_1BC20(void);
void onStoryMapExitToMainMenu(void);
void onStoryMapNormalExit(void);
void storyMapInitFadeIn(void);
void storyMapAwaitFadeIn(void);

typedef struct {
    u8 pad[0x24];
    s16 locationId;
} PreviewTaskNode;

typedef struct {
    ViewportNode viewport0;
    ViewportNode viewport1;
    ViewportNode viewport2;
    void *unk588;
    void *unk58C;
    u16 unk590;
    u16 locationIds[4];
    u8 unk59A[4];
    u8 unk59E[4];
    u8 locationOverlap[4];
    u8 characterIds[4];
    u8 unk5AA;
    u8 unk5AB;
    u8 unk5AC;
    s8 unk5AD;
    s8 unk5AE;
    u8 unk5AF;
    u8 unk5B0;
    u8 unk5B1;
    s8 unk5B2;
    u8 isStoryMapInitializing;
    u8 pad5B4[4];
    u8 unk5B8[9];
    u8 unk5C1[4];
    u8 pad5C5[3];
} StoryMapState;

void loadOverlay_1BBA0(void) {
    LOAD_OVERLAY(_1DA660)
    setGameStateHandler(&func_8001B020_1BC20);
}

void func_8001B020_1BC20(void) {
    StoryMapState *state;
    Transform3D sp20;
    s32 i;
    s32 j;
    s16 *locationPtr;
    PreviewTaskNode *task;

    state = allocateTaskMemory(0x5C8);
    playMusicTrack(0x19);
    setupTaskSchedulerNodes(0x14, 0x14, 0, 0, 0, 0, 0, 0);
    state->unk590 = 0;
    state->isStoryMapInitializing = 1;
    initMenuCameraNode(&state->viewport0, 0, 0xA, 0);
    initMenuCameraNode(&state->viewport1, 8, 0x14, 1);
    initMenuCameraNode(&state->viewport2, 9, 5, 1);
    setViewportFadeValue(NULL, 0xFF, 0);
    func_8006BEDC_6CADC(&sp20, 0, 0x80000, 0x60000, 0, 0, 0);
    setViewportTransformById(state->viewport0.id, &sp20);
    osViExtendVStart(0);

    for (i = 0; i < D_800AFE8C_A71FC->numPlayers; i++) {
        state->unk59E[i] = 0;
        state->unk59A[i] = 0;
        state->unk5C1[i] = 0;
        state->locationIds[i] = storyMapLocationOrder2[D_800AFE8C_A71FC->unk9[i]];
        state->locationOverlap[i] = 0;
        for (j = 0; j < i + 1; j++) {
            if (state->locationIds[i] == state->locationIds[j]) {
                state->locationOverlap[i]++;
            }
        }
    }

    state->unk5AA = 0;
    state->unk5AB = 0;
    state->unk5AC = 0;
    state->unk5AD = 0;
    state->unk5AE = 0;
    state->unk5AF = 0;
    state->unk5B0 = 0;
    state->unk5B1 = 0;
    state->unk5B2 = 0;

    if (D_800AFE8C_A71FC->unk4 == 0) {
        state->unk5B2 = -1;
        state->unk5AE = -1;
        state->unk5AD = -1;
    } else {
        if (EepromSaveData->setting_4E == 0) {
            state->unk5B2 = -1;
        }
        if (EepromSaveData->setting_4F == 0) {
            state->unk5AE = -1;
        }
        if (EepromSaveData->setting_50 == 0) {
            state->unk5AD = -1;
        }
    }

    i = 0;
    locationPtr = storyMapLocationOrder3;
    do {
        task = scheduleTask(initCharacterPreview, 1, 0, 0x5A);
        task->locationId = *locationPtr;
        locationPtr++;
        state->unk5B8[i] = 0;
        i++;
    } while (i < 9);

    scheduleTask(initCharacterSelectBoardTask, 1, 0, 0x5A);
    state->unk588 = loadCompressedData(&_41A1D0_ROM_START, &_41A1D0_ROM_END, 0x1B48);
    state->unk58C = loadCompressedData(&_41AD80_ROM_START, &_41AD80_ROM_END, 0x13FF0);
    scheduleTask(initCharacterSelectSprites, 1, 0, 0x5A);
    scheduleTask(initCharacterSelectTextureDataLoad, 1, 0, 0x5A);

    for (i = 0; i < D_800AFE8C_A71FC->numPlayers; i++) {
        state->characterIds[i] = D_800AFE8C_A71FC->unk9[i];
    }

    setGameStateHandler(storyMapInitFadeIn);
}

void storyMapInitFadeIn(void) {
    GameState *state = (GameState *)getCurrentAllocation();
    if (state->isStoryMapInitializing != 0) {
        return;
    }

    if (getPendingDmaCount() != 0) {
        return;
    }

    setViewportFadeValue(NULL, 0, 0x10);

    setGameStateHandler(storyMapAwaitFadeIn);
}

void storyMapAwaitFadeIn(void) {
    if (!getViewportFadeMode(0)) {
        setGameStateHandler(&func_8001B3E8_1BFE8);
    }
}

INCLUDE_ASM("asm/nonmatchings/1BBA0", func_8001B3E8_1BFE8);

void storyMapAwaitFadeOutAndCleanup(void) {
    void *exitCallback;
    ViewportNode *state = (ViewportNode *)getCurrentAllocation();

    if (getViewportFadeMode(0) == 0) {
        unlinkNode(&state[0]);
        unlinkNode(&state[1]);
        unlinkNode(&state[2]);

        osViExtendVStart(0);

        state[3].unk0.next = freeNodeMemory(state[3].unk0.next);
        state[3].prev = freeNodeMemory(state[3].prev);

        if (state[3].unk8.callback_selector != 0) {
            exitCallback = &onStoryMapExitToMainMenu;
        } else {
            exitCallback = &onStoryMapNormalExit;
        }

        terminateSchedulerWithCallback(exitCallback);
    }
}

void onStoryMapNormalExit(void) {
    s32 exitCode;

    exitCode = 1;
    if ((D_800AFE8C_A71FC->unk4 == 0) && (EepromSaveData->save_slot_status[0] == 5)) {
        exitCode = 0x44;
    }
    returnToParentScheduler(exitCode);
}

void onStoryMapExitToMainMenu(void) {
    returnToParentScheduler(0xFF);
}

void storyMapClearLocationArrivalsIfNoActivePlayers(void) {
    GameState *state = getCurrentAllocation();
    s32 locationIdx;
    s32 playerIdx;
    u8 activePlayerCount;

    for (locationIdx = 0; locationIdx < 9; locationIdx++) {
        activePlayerCount = 0;

        for (playerIdx = 0; playerIdx < D_800AFE8C_A71FC->numPlayers; playerIdx++) {
            if (state->playerSlotState[playerIdx] == 1 || state->playerSlotState[playerIdx] == 3) {
                activePlayerCount++;
            }
        }

        if (activePlayerCount == 0) {
            state->unk5B8[locationIdx] = 0;
        }
    }
}

void storyMapRevertBlockedLocationIndex(s8 *locationIndex) {
    if (*locationIndex == 3) {
        *locationIndex = 2;
    } else if (*locationIndex == 8) {
        *locationIndex = 7;
    } else if (*locationIndex != 4) {
    } else {
        *locationIndex = 0;
    }
}
