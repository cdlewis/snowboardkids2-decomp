#include "3A1F0.h"
#include "3E160.h"
#include "6E840.h"
#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "common.h"
#include "task_scheduler.h"

typedef struct {
    Node_70B00 unk0[4];
    Node_70B00 unk760[4];
    Node_70B00 unkEC0[4];
    Node_70B00 unk1620;
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
    if (func_8003BB5C_3C75C() != 0) {
        return;
    }
    state->stateCounter = 0;
    func_8006FDA0_709A0(NULL, 0, 10);
    setGameStateHandler(scheduleCharacterSelectTasks);
}

void scheduleCharacterSelectTasks(void) {
    s32 i;
    CharacterSelectTask *task;

    if (func_8006FE10_70A10(0) != 0) {
        return;
    }

    for (i = 0; i < D_800AFE8C_A71FC->numPlayers; i++) {
        task = (CharacterSelectTask *)scheduleTask(func_8002529C_25E9C, 1, i, 0x5A);
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

    if (func_8006FE10_70A10(0) != 0) {
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
    func_8006FDA0_709A0(NULL, 0, 0);
    func_800697F4_6A3F4(1);
}

void onCharacterSelectCancel(void) {
    func_8006FDA0_709A0(NULL, 0, 0);
    func_800697F4_6A3F4(0xFF);
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
