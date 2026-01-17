#include "15690.h"
#include "4050.h"
#include "6E840.h"
#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "common.h"
#include "memory_allocator.h"
#include "race_session.h"
#include "task_scheduler.h"
extern u8 gDebugUnlockEnabled;

void resetGameSession(void);

void func_800144DC_150DC(void);
void startOptionsMenu(void);
void func_80014690_15290(void);
void func_80014660_15260(void);
void waitForLogoScreen(void);
void startLogoScreen(void);
void waitForOptionsMenu(void);
void startAttractRace(void);
void waitForAttractRace(void);
void startDemoRace(void);
void waitForDemoRace(void);
void startSelectedGameMode(void);
void startBattleRace(void);
void waitForBattleRace(void);
void initOptionsDefaults(void);

typedef struct {
    s16 unk0;
    u8 unk2;
    u8 pad;
} TaskData_1512C;

void initGameSession(void) {
    u8 allocation;

    D_800AFE8C_A71FC = allocateMemoryNode(0, 0x28, &allocation);
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
    createTaskQueue(initLogoScreen, 0x64);
    setGameStateHandler(waitForLogoScreen);
}

void waitForLogoScreen(void) {
    if ((getSchedulerReturnValue() << 16) != 0) {
        setGameStateHandler(startDemoRace);
    }
}

void startDemoRace(void) {
    D_800AFE8C_A71FC->unk4 = 3;
    createTaskQueue(initRace, 0x96);
    setGameStateHandler(waitForDemoRace);
}

void waitForDemoRace(void) {
    if ((getSchedulerReturnValue() << 16) != 0) {
        D_800AFE8C_A71FC->unk4 = 0;
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
        D_800AFE8C_A71FC_type *ptr = D_800AFE8C_A71FC;
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
    D_800AFE8C_A71FC->unk4 = 2;
    createTaskQueue(initRace, 100);
    setGameStateHandler(waitForAttractRace);
}

void waitForAttractRace(void) {
    if ((getSchedulerReturnValue() << 16) != 0) {
        D_800AFE8C_A71FC->unk5 = (D_800AFE8C_A71FC->unk5 + 1) % 3;
        D_800AFE8C_A71FC->unk5 |= 0xF0;
        setGameStateHandler(func_80014660_15260);
    }
}

void startBattleRace(void) {
    D_800AFE8C_A71FC->unk4 = 0;
    D_800AFE8C_A71FC->saveSlotIndex = 0xF;
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
    u8 val = D_800AFE8C_A71FC->unk4;

    if (val == 0xFF) {
        createRootTaskScheduler(__udiv_w_sdiv(), 0xC8);
    } else if (val == 0) {
        createRootTaskScheduler(initStoryMode, 0xC8);
    } else {
        createRootTaskScheduler(initVersusMode, 0xC8);
    }
}

void initOptionsDefaults(void) {
    D_800AFE8C_A71FC->unk1F = 0;
    D_800AFE8C_A71FC->unk20 = 0;
    D_800AFE8C_A71FC->customLapEnabled = 0;
    D_800AFE8C_A71FC->customLapCount = 3;
}

void resetGameSession(void) {
    s32 i;

    D_800AFE8C_A71FC->unk4 = 0;
    D_800AFE8C_A71FC->saveSlotIndex = 0;

    if (D_800AFE8C_A71FC->customLapEnabled == 1) {
        D_800AFE8C_A71FC->unk9[0x10] = D_800AFE8C_A71FC->customLapCount;
    } else {
        D_800AFE8C_A71FC->unk9[0x10] = 3;
    }

    D_800AFE8C_A71FC->numPlayers = 0;
    D_800AFE8C_A71FC->gold = 0;

    for (i = 0; i < 4; i++) {
        D_800AFE8C_A71FC->unk9[i] = 0;
        D_800AFE8C_A71FC->unk9[i + 4] = 0;
        D_800AFE8C_A71FC->unk9[i + 0xC] = 0;
        D_800AFE8C_A71FC->unk9[i + 8] = 0;
        D_800AFE8C_A71FC->unk9[i + 0x11] = 0;
    }

    D_800AFE8C_A71FC->previousSaveSlot = 0;
    D_800AFE8C_A71FC->unk5 = 0;
    D_800AFE8C_A71FC->pendingUnlockCutscene = 0;
    D_800AFE8C_A71FC->isStoryMode = 0;
    D_800AFE8C_A71FC->creditsCompleted = 0;
}
