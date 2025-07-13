#include "1A770.h"
#include "6E840.h"
#include "common.h"
#include "gamestate.h"
#include "task_scheduler.h"

extern s32 D_419440;
extern s32 D_4196E0;
extern s32 D_45A890;
extern s32 D_45B130;
extern u16 D_8008D6B0_8E2B0;
extern u8 D_800A8CC8_A0038;
extern void *func_80035F80_36B80(s32);
extern void ClocktowerTriggerInit();
extern void func_80000460_1060(s32, void *, void *);
extern void func_8000056C_116C(s32, s32, GameState *);
extern void func_80000710_1310();
extern void func_80000760_1360();
extern void func_80017350_17F50;
extern void func_80018800_19400;
extern void func_80018A90_19690;
extern void func_80018CB0_198B0;
extern void func_80027CA0_288A0(void *, s32, s32, s32);
extern void func_80028480_29080;
extern void func_80036250_36E50;
extern void func_80036A3C_3763C;
extern void func_800574A0_580A0(s32);
extern void func_80057564_58164(s32 a0);
extern void func_800697CC_6A3CC(void (*func)(void));
extern void func_800697F4_6A3F4(s32);
extern void n_alSynRemovePlayer(void *player);
extern void PhoneTriggerInit;
s32 func_8003BB5C_3C75C();

void cleanupTransitionEffect();
void gameStateCleanupHandler();
void gameStateCountdownHandler(void);
void invokeTransitionEffect();
void setShortCountdownAndContinue();
void setupGameStateTransition(s32 /*arg0*/);
void transitionToMainMenu(void);
void transitionToNextGameMode(void);

void initializeGameState(s32 arg0, s32 arg1, s32 arg2) {
    int var_s1;
    GameState *temp_s0;
    volatile u8 padding[0x20];
    temp_s0 = (GameState *)allocateTaskMemory(0x430);
    func_800574A0_580A0(4);
    temp_s0->unk3E8 = 0;
    temp_s0->unk428 = 0;
    temp_s0->unk427 = 0;
    temp_s0->unk429 = 0x63;
    temp_s0->unk425 = 0;
    temp_s0->unk424 = 0;
    temp_s0->unk42E = 0;
    temp_s0->unk3D0 = 0;
    temp_s0->unk3D4 = 0;
    temp_s0->unk3D8 = 0;
    temp_s0->unk42A = 0;
    var_s1 = 0;
    do {
        temp_s0->unk3FE = (s16)(D_8008D6B0_8E2B0 - (((s32)((((short)D_8008D6B0_8E2B0) / 10) << 0x10)) >> 0xF));
        temp_s0->unk403 = 0xFF;
        setupTaskSchedulerNodes(0x30, 0xA, 0x14, 0, 0, 0, 0, 0);
        func_80027CA0_288A0(temp_s0, 0, 0xA, 0);
        func_80027CA0_288A0(((void *)((s32)temp_s0)) + 0x1D8, 8, 0xF, 1);
        scheduleTask(&func_80017350_17F50, 0, 0, 0x5A);
        scheduleTask(&func_80018800_19400, 0, 0, 0x5B);
        scheduleTask(&func_80028480_29080, 0, 0, 0x5B);
        var_s1++;
        var_s1--;
        scheduleTask(&func_80019B70_1A770, 0, 0, 0x5C);
        scheduleTask(&setupGameStateTransition, 0, 0, 0x5D);
        scheduleTask(&func_80018A90_19690, 0, 0, 0x62);
        do {
            *(s8 *)scheduleTask(&func_80018CB0_198B0, 0, 0, 0x64) = var_s1;
            var_s1 += 1;
        } while (var_s1 < 6);
        scheduleTask(&PhoneTriggerInit, 0, 0, 0x64);
    } while (0);
    scheduleTask(&ClocktowerTriggerInit, 0, 0, 0x64);
    scheduleTask(&func_80036A3C_3763C, 0, 0, 0x64);
    scheduleTask(&func_80036250_36E50, 0, 0, 0x64);
    temp_s0->unk3E4 = dmaRequestAndUpdateState(&D_419440, &D_4196E0);
    temp_s0->unk3DC = dmaRequestAndUpdateStateWithSize(&D_45A890, &D_45B130, 0x3108);
    temp_s0->unk3E0 = func_80035F80_36B80(1);
    func_8006FDA0_709A0(NULL, 0xFF, 0);
    setGameStateHandler(&gameStateCountdownHandler);
}

void gameStateCountdownHandler(void) {
    GameState *gameState = (GameState *)getCurrentAllocation();

    if (gameState->unk429 != 0) {
        gameState->unk429--;
    } else {
        if (func_8003BB5C_3C75C() == 0) {
            func_8006FDA0_709A0(NULL, 0, 0x10);
            setGameStateHandler(gameStateCleanupHandler);
        }
    }
}

void gameStateCleanupHandler(void) {
    GameState *gs = (GameState *)getCurrentAllocation();

    u8 val = gs->unk42E;
    if (val != 0) {
        if (val == 2) {
            terminateTasksByTypeAndID(2, 1);
        } else {
            terminateTasksByType(2);
        }
        gs->unk42E = 0;
    }

    if (gs->unk427 != 0) {
        func_80057564_58164(10);
        n_alSynRemovePlayer(gs);
        n_alSynRemovePlayer((void *)((u8 *)gs + 0x1D8));

        gs->unk3E4 = (void *)freeGameStateMemory(gs->unk3E4);
        gs->unk3DC = (void *)freeGameStateMemory(gs->unk3DC);
        gs->unk3E0 = (void *)freeGameStateMemory(gs->unk3E0);

        if (gs->unk427 == 0xFF) {
            func_800697CC_6A3CC(transitionToMainMenu);
        } else {
            u8 tmp = gs->unk425 + 1;
            D_800A8CC8_A0038 = tmp;
            func_800697CC_6A3CC(transitionToNextGameMode);
        }
    }
}

void transitionToNextGameMode(void) {
    func_800697F4_6A3F4(1);
}

void transitionToMainMenu(void) {
    func_800697F4_6A3F4(0xFF);
}

void setupGameStateTransition(s32 arg0) {
    char *sp10[5];
    char *sp28;

    func_8000056C_116C(arg0, 0, getCurrentAllocation());
    setCleanupCallback(&cleanupTransitionEffect);
    func_80000460_1060(arg0, &sp10, &sp28);
    setCallback(&setShortCountdownAndContinue);
}

void setShortCountdownAndContinue(void) {
    ((GameState *)getCurrentAllocation())->unk429 = 2;
    setCallback(&invokeTransitionEffect);
}

void invokeTransitionEffect(void) {
    func_80000760_1360();
}

void cleanupTransitionEffect(void) {
    func_80000710_1310();
}
