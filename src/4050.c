#include "1DFAA0.h"
#include "1E60D0.h"
#include "3E160.h"
#include "6E840.h"
#include "common.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_OVERLAY(cutscene);

typedef union {
    struct {
        s16 lower;
        s16 upper;
    } half;
    s32 full;
} CompositeInt;

typedef struct {
    s16 slotIndex;
    s16 cutsceneType;
    s16 frameIndex;
    s16 unk6;
    CompositeInt slotSelection;
    CompositeInt typeSelection;
} CutsceneState;

CutsceneState gCutsceneState = { .slotIndex = 0,
                                 .cutsceneType = 0,
                                 .frameIndex = 0,
                                 .unk6 = 0,
                                 .slotSelection = { .half = { .lower = 0, .upper = 0 } },
                                 .typeSelection = { .half = { .lower = 0, .upper = 2 } } };

extern s16 D_800AB070_A23E0; // gCutsceneSlotIndex
extern s16 D_800AFEF0_A7260; // gCutsceneType
typedef struct {
    s16 frameCount;
    s8 unk2;
    u8 exitRequested;
    s32 unk4;
    Node_70B00 sceneNode;
    Node_70B00 overlayNode;
    Node_70B00 uiNode;
    u8 padding1[0x20];
    u8 cutsceneBuffer;
    u8 padding[0x122F];
    func_800B9020_arg unk17E0;
} CutsceneTaskMemory;

void awaitCutsceneTransitionComplete(void);
void func_80003898_4498(void);
void loadCutsceneOverlay(void);
void runCutsceneFrame(void);
void signalCutsceneComplete(void);
void func_80003D30_4930(void);
void initCutscenePlayback(void);
void initCutsceneRenderer(void);

void setCutsceneSelection(s16 slotIndex, s16 cutsceneType) {
    D_800AB070_A23E0 = slotIndex;
    D_800AFEF0_A7260 = cutsceneType;
}

void *__udiv_w_sdiv(void) {
    return NULL;
}

void setCutsceneParameters(s16 slotIndex, s16 cutsceneType, s16 frameIndex) {
    gCutsceneState.slotIndex = slotIndex;
    gCutsceneState.cutsceneType = cutsceneType;
    gCutsceneState.frameIndex = frameIndex;
}

void loadCutsceneOverlay(void) {
    LOAD_OVERLAY(cutscene)
    setGameStateHandler(&initCutsceneRenderer);
}

void initCutsceneRenderer(void) {
    Node_70B00 *overlayNodePtr;
    Node_70B00 *nodePtr;
    Node_70B00 *uiNodePtr;
    f32 scale = 1.0f;
    u8 lightBuffer[0x20];

    struct {
        CutsceneTaskMemory *taskMemory;
    } s;
    s.taskMemory = (CutsceneTaskMemory *)allocateTaskMemory(0x17E8);
    s.taskMemory->unk2 = 0;
    s.taskMemory->frameCount = 0;
    s.taskMemory->exitRequested = 0;
    setupTaskSchedulerNodes(0x40, 0x30, 4, 4, 4, 0, 0, 0);

    nodePtr = &s.taskMemory->sceneNode;
    func_8006FAA4_706A4(nodePtr, 0, 0, 10, scale);
    func_8006F9BC_705BC(nodePtr, scale, scale);
    func_8006FEF8_70AF8(nodePtr, 1);
    setModelCameraTransform(nodePtr, 0, 0, -160, -120, 159, 119);
    func_8006FA0C_7060C(nodePtr, 40.0f, 1.3333334f, 10.0f, 10000.0f);
    func_8006BEDC_6CADC(lightBuffer, 0, 0, 0x01000000, 0, 0, 0);

    overlayNodePtr = &s.taskMemory->overlayNode;
    func_8006FD3C_7093C(s.taskMemory->sceneNode.id, lightBuffer);
    func_8006FDA0_709A0(NULL, 0, 0);

    nodePtr = overlayNodePtr;
    func_8006FAA4_706A4(nodePtr, 0, 1, 11, 0);
    func_8006F9BC_705BC(nodePtr, scale, scale);

    func_8006FEF8_70AF8(nodePtr, 1);
    setModelCameraTransform(nodePtr, 0, 0, -160, -120, 159, 119);
    func_8006FE28_70A28(nodePtr, 0, 0, 0);
    func_8006FDA0_709A0(nodePtr, 0xFF, 0);
    func_8006FDA0_709A0(nodePtr, 0, 8);

    uiNodePtr = &s.taskMemory->uiNode;
    func_8006FAA4_706A4(uiNodePtr, 0, 3, 8, 0);
    func_8006F9BC_705BC(uiNodePtr, scale, scale);
    func_8006FEF8_70AF8(uiNodePtr, 1);
    setModelCameraTransform(uiNodePtr, 0, 0, -160, -120, 159, 119);
    func_8006FE28_70A28(uiNodePtr, 0, 0, 0);
    func_8006FDA0_709A0(uiNodePtr, 0xFF, 0);
    func_8006FDA0_709A0(uiNodePtr, 0, 8);
    setGameStateHandler(runCutsceneFrame);
}

void runCutsceneFrame(void) {
    CutsceneTaskMemory *taskMemory = (CutsceneTaskMemory *)getCurrentAllocation();

    if (taskMemory->frameCount >= func_800B3360(D_800AB070_A23E0, D_800AFEF0_A7260) || taskMemory->exitRequested != 0) {
        if (D_800AB070_A23E0 == 0xB && D_800AFEF0_A7260 == 1) {
            func_8006FDA0_709A0(&taskMemory->overlayNode, 0, 0);
            func_800B9020(&taskMemory->unk17E0);
            setGameStateHandler(&awaitCutsceneTransitionComplete);

            return;
        }

        func_80057564_58164(2);
        unlinkNode(&taskMemory->uiNode);
        unlinkNode(&taskMemory->overlayNode);
        unlinkNode(&taskMemory->sceneNode);
        terminateSchedulerWithCallback(&signalCutsceneComplete);

        return;
    }

    setCutsceneParameters(D_800AB070_A23E0, D_800AFEF0_A7260, taskMemory->frameCount);
    taskMemory->frameCount++;
    initializeCutsceneSystem(&taskMemory->cutsceneBuffer);
    setGameStateHandler(&func_80003898_4498);
}

INCLUDE_ASM("asm/nonmatchings/4050", func_80003898_4498);

void awaitCutsceneTransitionComplete(void) {
    CutsceneTaskMemory *taskMemory;

    taskMemory = (CutsceneTaskMemory *)getCurrentAllocation();
    if (taskMemory->unk17E0.unk0 != 0) {
        unlinkNode(&taskMemory->uiNode);
        unlinkNode(&taskMemory->overlayNode);
        unlinkNode(&taskMemory->sceneNode);
        terminateSchedulerWithCallback(&signalCutsceneComplete);
    }
}

void signalCutsceneComplete(void) {
    func_800697F4_6A3F4(1);
}

void beginCutsceneSequence(void) {
    setGameStateHandler(&initCutscenePlayback);
}

void initCutscenePlayback(void) {
    if ((gCutsceneState.slotSelection.full == 5) && (gCutsceneState.typeSelection.full == 0)) {
        gCutsceneState.typeSelection.full = 1;
    }
    setCutsceneSelection(gCutsceneState.slotSelection.half.upper, gCutsceneState.typeSelection.half.upper);
    createTaskQueue(&loadCutsceneOverlay, 0x64);
    setGameStateHandler(&func_80003D30_4930);
}

INCLUDE_ASM("asm/nonmatchings/4050", func_80003D30_4930);

void returnToMainGame(void) {
    func_800693C4_69FC4(&func_80014480_15080, 0xC8);
}
