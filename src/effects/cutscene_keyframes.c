#include "effects/cutscene_keyframes.h"
#include "audio/audio.h"
#include "common.h"
#include "cutscene/1DFAA0.h"
#include "effects/scrolling_texture.h"
#include "graphics/graphics.h"
#include "math/geometry.h"
#include "os_cont.h"
#include "race/race_session.h"
#include "system/rom_loader.h"
#include "system/task_scheduler.h"

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

extern s16 gCutsceneSlotIndex;
extern s16 gCutsceneType;
extern s32 gControllerInputs;
extern s32 gButtonsPressed;

void awaitCutsceneTransitionComplete(void);
void updateCutscenePlayback(void);

void runCutsceneFrame(void);
void signalCutsceneComplete(void);
void awaitCutsceneTaskComplete(void);
void initCutscenePlayback(void);
void initCutsceneRenderer(void);
void returnToMainGame(void);

void setCutsceneSelection(s16 slotIndex, s16 cutsceneType) {
    gCutsceneSlotIndex = slotIndex;
    gCutsceneType = cutsceneType;
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
    ViewportNode *overlayNodePtr;
    ViewportNode *nodePtr;
    ViewportNode *uiNodePtr;
    f32 scale = 1.0f;
    u8 lightBuffer[0x20];

    struct {
        CutsceneTaskMemory *taskMemory;
    } s;
    s.taskMemory = (CutsceneTaskMemory *)allocateTaskMemory(0x17E8);
    s.taskMemory->playbackState = 0;
    s.taskMemory->frameCount = 0;
    s.taskMemory->exitRequested = 0;
    setupTaskSchedulerNodes(0x40, 0x30, 4, 4, 4, 0, 0, 0);

    nodePtr = &s.taskMemory->sceneNode;
    initViewportNode(nodePtr, 0, 0, 10, scale);
    setViewportScale(nodePtr, scale, scale);
    setViewportId(nodePtr, 1);
    setModelCameraTransform(nodePtr, 0, 0, -160, -120, 159, 119);
    setViewportPerspective(nodePtr, 40.0f, 1.3333334f, 10.0f, 10000.0f);
    createViewportTransform(lightBuffer, 0, 0, 0x01000000, 0, 0, 0);

    overlayNodePtr = &s.taskMemory->overlayNode;
    setViewportTransformById(s.taskMemory->sceneNode.id, lightBuffer);
    setViewportFadeValue(NULL, 0, 0);

    nodePtr = overlayNodePtr;
    initViewportNode(nodePtr, 0, 1, 11, 0);
    setViewportScale(nodePtr, scale, scale);

    setViewportId(nodePtr, 1);
    setModelCameraTransform(nodePtr, 0, 0, -160, -120, 159, 119);
    setViewportEnvColor(nodePtr, 0, 0, 0);
    setViewportFadeValue(nodePtr, 0xFF, 0);
    setViewportFadeValue(nodePtr, 0, 8);

    uiNodePtr = &s.taskMemory->uiNode;
    initViewportNode(uiNodePtr, 0, 3, 8, 0);
    setViewportScale(uiNodePtr, scale, scale);
    setViewportId(uiNodePtr, 1);
    setModelCameraTransform(uiNodePtr, 0, 0, -160, -120, 159, 119);
    setViewportEnvColor(uiNodePtr, 0, 0, 0);
    setViewportFadeValue(uiNodePtr, 0xFF, 0);
    setViewportFadeValue(uiNodePtr, 0, 8);
    setGameStateHandler(runCutsceneFrame);
}

void runCutsceneFrame(void) {
    CutsceneTaskMemory *taskMemory = (CutsceneTaskMemory *)getCurrentAllocation();

    if (taskMemory->frameCount >= getCutsceneFrameCount(gCutsceneSlotIndex, gCutsceneType) ||
        taskMemory->exitRequested != 0) {
        if (gCutsceneSlotIndex == 0xB && gCutsceneType == 1) {
            setViewportFadeValue(&taskMemory->overlayNode, 0, 0);
            initScreenTransition(&taskMemory->transitionState);
            setGameStateHandler(&awaitCutsceneTransitionComplete);

            return;
        }

        setMusicFadeOut(2);
        unlinkNode(&taskMemory->uiNode);
        unlinkNode(&taskMemory->overlayNode);
        unlinkNode(&taskMemory->sceneNode);
        terminateSchedulerWithCallback(&signalCutsceneComplete);

        return;
    }

    setCutsceneParameters(gCutsceneSlotIndex, gCutsceneType, taskMemory->frameCount);
    taskMemory->frameCount++;
    initializeCutsceneSystem(&taskMemory->cutsceneData);
    setGameStateHandler(&updateCutscenePlayback);
}

typedef enum {
    CUTSCENE_STATE_INIT_DMA,
    CUTSCENE_STATE_WAIT_DMA,
    CUTSCENE_STATE_INIT_MANAGER,
    CUTSCENE_STATE_START_PLAYBACK,
    CUTSCENE_STATE_DELAY,
    CUTSCENE_STATE_PLAYING,
    CUTSCENE_STATE_SKIP_START,
    CUTSCENE_STATE_SKIP_FADE,
    CUTSCENE_STATE_TERMINATE_TASKS,
    CUTSCENE_STATE_WAIT_TERMINATE,
    CUTSCENE_STATE_CLEANUP,
    CUTSCENE_STATE_EXIT
} CutsceneStateEnum;

void updateCutscenePlayback(void) {
    CutsceneTaskMemory *state;
    s32 slotIndex, cutsceneType, frameIndex;

    state = (CutsceneTaskMemory *)getCurrentAllocation();

    switch (state->playbackState) {
        case CUTSCENE_STATE_INIT_DMA:
            slotIndex = gCutsceneState.slotIndex;
            cutsceneType = gCutsceneState.cutsceneType;
            frameIndex = gCutsceneState.frameIndex;
            state->delayCounter = 4;
            state->frameDataPtr = loadCutsceneFrameData(slotIndex, cutsceneType, frameIndex);
            if (state->frameDataPtr == NULL) {
                state->playbackState = CUTSCENE_STATE_EXIT;
            } else {
                state->playbackState = CUTSCENE_STATE_WAIT_DMA;
            }
            break;

        case CUTSCENE_STATE_WAIT_DMA:
            if (verifyAndLoadCutsceneState(state->frameDataPtr) == 0) {
                state->playbackState = CUTSCENE_STATE_TERMINATE_TASKS;
            } else {
                state->playbackState = CUTSCENE_STATE_INIT_MANAGER;
            }
            break;

        case CUTSCENE_STATE_INIT_MANAGER:
            initCutsceneManager(
                &state->cutsceneData.cutsceneManager,
                &state->sceneNode,
                &state->overlayNode,
                &state->uiNode
            );
            hideAllSlotModels((CutsceneSlot *)&state->cutsceneData.cutsceneManager);
            state->playbackState = CUTSCENE_STATE_DELAY;
            break;

        case CUTSCENE_STATE_DELAY:
            if (state->delayCounter == 0) {
                state->playbackState = CUTSCENE_STATE_START_PLAYBACK;
            } else {
                state->delayCounter--;
            }
            break;

        case CUTSCENE_STATE_START_PLAYBACK:
            if (gCutsceneType == 0 && state->frameCount == 1) {
                startCutsceneFadeEffect(1, (s8)gCutsceneSlotIndex, getCutsceneConfigByte() & 0xFF);
            }
            scheduleTransparentModelRender(
                &state->cutsceneData.cutsceneManager,
                (ModelEntityRenderState *)&state->cutsceneData.cutsceneManager.unk10.modelEntity
            );
            setupModelEntityLighting(
                &state->cutsceneData.cutsceneManager.unk10.modelEntity,
                &state->lightColors,
                &state->ambientColor
            );
            osViExtendVStart(0);
            prepareCutsceneForPlayback(
                &state->cutsceneData.cutsceneManager,
                &state->sceneNode,
                &state->lightColors,
                &state->ambientColor,
                0,
                0
            );
            state->playbackState = CUTSCENE_STATE_PLAYING;
            /* fallthrough */

        case CUTSCENE_STATE_PLAYING:
            if (processCutsceneFrame(&state->cutsceneData.cutsceneManager) == 0) {
                state->playbackState = CUTSCENE_STATE_TERMINATE_TASKS;
            }
            if (gControllerInputs & START_BUTTON) {
                state->playbackState = CUTSCENE_STATE_SKIP_START;
            }
            break;

        case CUTSCENE_STATE_SKIP_START:
            if (processCutsceneFrame(&state->cutsceneData.cutsceneManager) == 0) {
                state->playbackState = CUTSCENE_STATE_TERMINATE_TASKS;
            }
            state->fadeCounter = 0x10;
            setMusicFadeOut(0x10);
            stopAllSoundEffectsAndClearQueues(0x10);
            setViewportEnvColor(&state->sceneNode, 0, 0, 0);
            setViewportFadeValue(&state->sceneNode, 0xFF, 0x10);
            setViewportEnvColor(&state->overlayNode, 0, 0, 0);
            setViewportFadeValue(&state->overlayNode, 0xFF, 0x10);
            setViewportEnvColor(&state->uiNode, 0, 0, 0);
            setViewportFadeValue(&state->uiNode, 0xFF, 0x10);
            state->exitRequested = 1;
            state->playbackState = CUTSCENE_STATE_SKIP_FADE;
            break;

        case CUTSCENE_STATE_SKIP_FADE:
            if (processCutsceneFrame(&state->cutsceneData.cutsceneManager) == 0) {
                state->playbackState = CUTSCENE_STATE_TERMINATE_TASKS;
            }
            if (state->fadeCounter == 0) {
                state->playbackState = CUTSCENE_STATE_TERMINATE_TASKS;
            }
            state->fadeCounter--;
            break;

        case CUTSCENE_STATE_TERMINATE_TASKS:
            terminateTasksByType(0);
            terminateTasksByType(2);
            terminateTasksByType(3);
            terminateTasksByType(1);
            state->playbackState = CUTSCENE_STATE_WAIT_TERMINATE;
            break;

        case CUTSCENE_STATE_WAIT_TERMINATE:
            state->playbackState = CUTSCENE_STATE_CLEANUP;
            break;

        case CUTSCENE_STATE_CLEANUP:
            cleanupCutsceneManager(&state->cutsceneData.cutsceneManager);
            state->playbackState = CUTSCENE_STATE_EXIT;
            break;

        default:
        case CUTSCENE_STATE_EXIT:
            osViExtendVStart(0);
            setModelCameraTransform(&state->sceneNode, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
            setModelCameraTransform(&state->overlayNode, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
            setModelCameraTransform(&state->uiNode, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
            state->playbackState = CUTSCENE_STATE_INIT_DMA;
            stopAllSoundEffectsAndClearQueues(2);
            cleanupCutsceneSystem();
            setGameStateHandler(runCutsceneFrame);
            break;
    }
}

void awaitCutsceneTransitionComplete(void) {
    CutsceneTaskMemory *taskMemory;

    taskMemory = (CutsceneTaskMemory *)getCurrentAllocation();
    if (taskMemory->transitionState.isComplete != 0) {
        unlinkNode(&taskMemory->uiNode);
        unlinkNode(&taskMemory->overlayNode);
        unlinkNode(&taskMemory->sceneNode);
        terminateSchedulerWithCallback(&signalCutsceneComplete);
    }
}

void signalCutsceneComplete(void) {
    returnToParentScheduler(1);
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
    setGameStateHandler(&awaitCutsceneTaskComplete);
}

void awaitCutsceneTaskComplete(void) {
    s32 typeSelection;
    CutsceneState *state;

    if ((getSchedulerReturnValue() << 16) != 0) {
        if (gButtonsPressed & B_BUTTON) {
            terminateSchedulerWithCallback(returnToMainGame);
            return;
        }

        state = &gCutsceneState;
        typeSelection = state->typeSelection.full;

        if (typeSelection == 2) {
            state->slotSelection.full = (state->slotSelection.full + 1) % 14;
            typeSelection = typeSelection;
        }

        state->typeSelection.full = typeSelection + 1;

        if (typeSelection + 1 >= 3) {
            state->typeSelection.full = 0;
        }

        state->slotSelection.full = 0;
        state->typeSelection.full = 2;
        setGameStateHandler(initCutscenePlayback);
    }
}

void returnToMainGame(void) {
    createRootTaskScheduler(&initGameSession, 200);
}
