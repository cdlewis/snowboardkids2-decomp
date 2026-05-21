#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "common.h"
#include "core/session_manager.h"
#include "effects/cutscene_keyframes.h"
#include "graphics/graphics.h"
#include "race/race_session.h"
#include "system/memory_allocator.h"
#include "system/task_scheduler.h"
typedef struct {
    s16 unk0;
    u8 unk2;
    u8 pad;
} TaskData_1512C;

extern u8 gDebugUnlockEnabled;

void func_800144DC_150DC(void);
void func_80014660_15260(void);
void func_80014690_15290(void);
void startLogoScreen(void);
void waitForLogoScreen(void);
void startDemoRace(void);
void waitForDemoRace(void);
void startAttractRace(void);
void waitForAttractRace(void);
void startBattleRace(void);
void waitForBattleRace(void);
void startOptionsMenu(void);
void waitForOptionsMenu(void);
void startSelectedGameMode(void);
void initOptionsDefaults(void);
void resetGameSession(void);

void initGameSession(void) {
    u8 allocation;

    gGameSessionContext = allocateMemoryNode(0, 0x28, &allocation);
    EepromSaveData = allocateMemoryNode(0, 0x5C, &allocation);
    gDebugUnlockEnabled = 0;
    initOptionsDefaults();
    setGameStateHandlerWithContinue(func_800144DC_150DC);
}

void func_800144DC_150DC(void) {
    TaskData_1512C *data = allocateTaskMemory(4);
    setViewportFadeValue(NULL, 0, 0);
    data->unk0 = 0;
    data->unk2 = 0;
    resetGameSession();
    setGameStateHandlerWithContinue(startLogoScreen);
}

void func_8001452C_1512C(void) {
    TaskData_1512C *data = allocateTaskMemory(4);
    setViewportFadeValue(NULL, 0, 0);
    data->unk0 = 0;
    data->unk2 = 0;
    resetGameSession();
    setGameStateHandlerWithContinue(func_80014660_15260);
}

void startLogoScreen(void) {
    createTaskQueue(initLogoSplash, 0x64);
    setGameStateHandler(waitForLogoScreen);
}

void waitForLogoScreen(void) {
    if ((getSchedulerReturnValue() << 16) != 0) {
        setGameStateHandler(startDemoRace);
    }
}

void startDemoRace(void) {
    gGameSessionContext->gameMode = 3;
    createTaskQueue(initRace, 0x96);
    setGameStateHandler(waitForDemoRace);
}

void waitForDemoRace(void) {
    if ((getSchedulerReturnValue() << 16) != 0) {
        gGameSessionContext->gameMode = 0;
        setGameStateHandler(func_80014660_15260);
    }
}

void func_80014660_15260(void) {
    createTaskQueue(initTitleScreen, 0x64);
    setGameStateHandler(func_80014690_15290);
}

void func_80014690_15290(void) {
    s16 result = getSchedulerReturnValue();

    if (result == 0) {
        return;
    }

    if (result == 4) {
        GameSessionContext *ptr = gGameSessionContext;
        u8 val = ptr->unk5;

        if (val >= 3) {
            ptr->unk5 = val & 0xF;
            setGameStateHandler(startDemoRace);
        } else {
            setGameStateHandler(startAttractRace);
        }
    } else if (result == 2) {
        setGameStateHandler(startBattleRace);
    } else if (result == 3) {
        setGameStateHandler(startOptionsMenu);
    } else {
        terminateSchedulerWithCallback(startSelectedGameMode);
    }
}

void startAttractRace(void) {
    gGameSessionContext->gameMode = 2;
    createTaskQueue(initRace, 100);
    setGameStateHandler(waitForAttractRace);
}

void waitForAttractRace(void) {
    if ((getSchedulerReturnValue() << 16) != 0) {
        gGameSessionContext->unk5 = (gGameSessionContext->unk5 + 1) % 3;
        gGameSessionContext->unk5 |= 0xF0;
        setGameStateHandler(func_80014660_15260);
    }
}

void startBattleRace(void) {
    gGameSessionContext->gameMode = 0;
    gGameSessionContext->saveSlotIndex = 0xF;
    createTaskQueue(initRace, 100);
    setGameStateHandler(waitForBattleRace);
}

void waitForBattleRace(void) {
    s16 result = getSchedulerReturnValue();

    if (result == 0) {
        return;
    }

    if (result == 1) {
        setGameStateHandler(startBattleRace);
    } else {
        setGameStateHandler(func_80014660_15260);
    }
}

void startOptionsMenu(void) {
    createTaskQueue(initOptionsMenu, 0x64);
    setGameStateHandler(waitForOptionsMenu);
}

void waitForOptionsMenu(void) {
    if ((getSchedulerReturnValue() << 16) != 0) {
        setGameStateHandler(func_80014660_15260);
    }
}

void startSelectedGameMode(void) {
    u8 val = gGameSessionContext->gameMode;

    if (val == 0xFF) {
        createRootTaskScheduler(__udiv_w_sdiv(), 0xC8);
    } else if (val == 0) {
        createRootTaskScheduler(initStoryMode, 0xC8);
    } else {
        createRootTaskScheduler(initVersusMode, 0xC8);
    }
}

void initOptionsDefaults(void) {
    gGameSessionContext->optionToggle1 = 0;
    gGameSessionContext->optionToggle2 = 0;
    gGameSessionContext->customLapEnabled = 0;
    gGameSessionContext->customLapCount = 3;
}

void resetGameSession(void) {
    s32 i;

    gGameSessionContext->gameMode = 0;
    gGameSessionContext->saveSlotIndex = 0;

    if (gGameSessionContext->customLapEnabled == 1) {
        gGameSessionContext->playerBoardIds[0x10] = gGameSessionContext->customLapCount;
    } else {
        gGameSessionContext->playerBoardIds[0x10] = 3;
    }

    gGameSessionContext->numPlayers = 0;
    gGameSessionContext->gold = 0;

    for (i = 0; i < 4; i++) {
        gGameSessionContext->playerBoardIds[i] = 0;
        gGameSessionContext->playerBoardIds[i + 4] = 0;
        gGameSessionContext->playerBoardIds[i + 0xC] = 0;
        gGameSessionContext->playerBoardIds[i + 8] = 0;
        gGameSessionContext->playerBoardIds[i + 0x11] = 0;
    }

    gGameSessionContext->previousSaveSlot = 0;
    gGameSessionContext->unk5 = 0;
    gGameSessionContext->pendingUnlockCutscene = 0;
    gGameSessionContext->isStoryMode = 0;
    gGameSessionContext->creditsCompleted = 0;
}
