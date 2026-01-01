#include "1A770.h"
#include "288A0.h"
#include "36B80.h"
#include "3A1F0.h"
#include "6E840.h"
#include "clocktrigger_tower.h"
#include "common.h"
#include "gamestate.h"
#include "graphics.h"
#include "main.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_419440);
USE_ASSET(_45A890);

extern u16 D_8008D6B0_8E2B0;
extern u8 storyMapLocationIndex;
extern void storyMapCameraTask;
extern void func_80018800_19400;
extern void initStoryMapMiniCamera;
extern void GenericTriggerInit;
extern void initStoryMapRandomEvent;
extern void initStoryMapLocationIndicator;
extern void initStoryMapSpecialLocationTrigger;
extern void PhoneTriggerInit;

void cleanupTransitionEffect(EffectState *);
void gameStateCleanupHandler(void);
void gameStateCountdownHandler(void);
void invokeTransitionEffect(ModelEntityRenderState *);
void setShortCountdownAndContinue(void);
void setupGameStateTransition(ModelEntity *);
void transitionToMainMenu(void);
void transitionToNextGameMode(void);

typedef struct {
    Node_70B00 node1;
    Node_70B00 node2;
    u8 padding2[0x20];
    s32 unk3D0;
    s32 unk3D4;
    s32 unk3D8;
    void *unk3DC;
    void *unk3E0;
    void *unk3E4;
    s16 unk3E8;
    u8 padding3[0x14];
    s16 unk3FE;
    u8 padding4[0x3];
    u8 unk403;
    u8 padding5[0x20];
    u8 unk424;
    u8 unk425;
    u8 unk426;
    u8 unk427;
    u8 unk428;
    u8 unk429;
    u8 unk42A;
    u8 padding6[0x3];
    u8 unk42E;
} task_memory_430;

void initializeGameState(void) {
    int var_s1;
    task_memory_430 *temp_s0;
    volatile u8 padding[0x20];
    temp_s0 = (task_memory_430 *)allocateTaskMemory(0x430);
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
        initMenuCameraNode(&temp_s0->node1, 0, 0xA, 0);
        initMenuCameraNode(&temp_s0->node2, 8, 0xF, 1);
        scheduleTask(&storyMapCameraTask, 0, 0, 0x5A);
        scheduleTask(&func_80018800_19400, 0, 0, 0x5B);
        scheduleTask(&initStoryMapRandomEvent, 0, 0, 0x5B);
        var_s1++;
        var_s1--;
        scheduleTask(&storyMapDecorModelTask, 0, 0, 0x5C);
        scheduleTask(&setupGameStateTransition, 0, 0, 0x5D);
        scheduleTask(&initStoryMapMiniCamera, 0, 0, 0x62);
        do {
            *(s8 *)scheduleTask(&GenericTriggerInit, 0, 0, 0x64) = var_s1;
            var_s1 += 1;
        } while (var_s1 < 6);
        scheduleTask(&PhoneTriggerInit, 0, 0, 0x64);
    } while (0);
    scheduleTask(&ClocktowerTriggerInit, 0, 0, 0x64);
    scheduleTask(&initStoryMapSpecialLocationTrigger, 0, 0, 0x64);
    scheduleTask(&initStoryMapLocationIndicator, 0, 0, 0x64);
    temp_s0->unk3E4 = loadUncompressedData(&_419440_ROM_START, &_419440_ROM_END);
    temp_s0->unk3DC = loadCompressedData(&_45A890_ROM_START, &_45A890_ROM_END, 0x3108);
    temp_s0->unk3E0 = loadTextRenderAsset(1);
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
    task_memory_430 *gs = (task_memory_430 *)getCurrentAllocation();

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
        unlinkNode(&gs->node1);
        unlinkNode(&gs->node2);

        gs->unk3E4 = freeNodeMemory(gs->unk3E4);
        gs->unk3DC = freeNodeMemory(gs->unk3DC);
        gs->unk3E0 = freeNodeMemory(gs->unk3E0);

        if (gs->unk427 == 0xFF) {
            terminateSchedulerWithCallback(transitionToMainMenu);
        } else {
            u8 tmp = gs->unk425 + 1;
            storyMapLocationIndex = tmp;
            terminateSchedulerWithCallback(transitionToNextGameMode);
        }
    }
}

void transitionToNextGameMode(void) {
    func_800697F4_6A3F4(1);
}

void transitionToMainMenu(void) {
    func_800697F4_6A3F4(0xFF);
}

void setupGameStateTransition(ModelEntity *arg0) {
    ColorData sp10[3];
    ColorData sp28;

    initModelEntity(arg0, 0, getCurrentAllocation());
    setCleanupCallback(&cleanupTransitionEffect);
    setupModelEntityLighting(arg0, &sp10[0], &sp28);
    setCallback(&setShortCountdownAndContinue);
}

void setShortCountdownAndContinue(void) {
    ((GameState *)getCurrentAllocation())->unk429 = 2;
    setCallback(&invokeTransitionEffect);
}

void invokeTransitionEffect(ModelEntityRenderState *state) {
    renderModelEntity(state);
}

void cleanupTransitionEffect(EffectState *arg0) {
    freeEffectResources(arg0);
}
