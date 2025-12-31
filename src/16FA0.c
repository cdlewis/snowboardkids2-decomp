#include "16FA0.h"
#include "10AD0.h"
#include "20F0.h"
#include "38C90.h"
#include "6E840.h"
#include "common.h"
#include "gamestate.h"
#include "main.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_414CF0);
USE_ASSET(_418520);

extern u8 gConnectedControllerMask;

void renderTitleEffectModel(ModelEntityRenderState *arg0);
void cleanupTitleEffectModel(EffectState *arg0);

extern Transform3D gTitleCharacterTransforms[];
extern u16 *gTitleCharacterAnimSequences[2];

void updateCharacterFlyAway(TitleCharacterState *arg0);
void updateTitleCharacterAnimation(TitleCharacterState *arg0);
void handleUnlockAllCharacterAnim(TitleCharacterState *arg0);
void handlePartialUnlockCharacterAnim(TitleCharacterState *arg0);
void updatePartialUnlockAnim(TitleCharacterState *arg0);
void setupTitleCharacterTransform(TitleCharacterState *arg0);
void cleanupTitleCharacterModel(TitleCharacterState *arg0);

void enqueueTitleLogoRender(TitleLogoTask *);
void cleanupTitleLogoTask(TitleLogoTask *);

void cleanupTitleLogoTask(TitleLogoTask *arg0) {
    arg0->assetData = freeNodeMemory(arg0->assetData);
}

void enqueueTitleLogoRender(TitleLogoTask *arg0) {
    debugEnqueueCallback(8, 0, func_80038420_39020, arg0);
}

void initTitleLogoRenderState(TitleLogoTask *arg0) {
    func_800394BC_3A0BC(arg0, (s32)arg0->assetData);
    arg0->unk2 = 0x10;
    setCallback(enqueueTitleLogoRender);
}

void loadTitleLogoAsset(TitleLogoTask *arg0) {
    arg0->assetData = loadCompressedData(&_414CF0_ROM_START, &_414CF0_ROM_END, 0x7B50);
    setCleanupCallback(cleanupTitleLogoTask);
    setCallback(initTitleLogoRenderState);
}

INCLUDE_ASM("asm/nonmatchings/16FA0", func_80016488_17088);

typedef struct {
    u8 pad0[0xA];
    s16 alpha;
    void *unkC;
} ControllerSlotElement;

typedef struct {
    ControllerSlotElement elements[8];
} ControllerSlotDisplay;

void updateControllerSlotHighlights(ControllerSlotDisplay *arg0) {
    GameState *state;
    s32 i;
    s32 numControllers;
    s32 selectedSlot;
    s32 unused[2];
    ControllerSlotElement *slot;
    s32 selectedAlpha;
    s32 unselectedAlpha;

    state = (GameState *)getCurrentAllocation();

    if (gConnectedControllerMask != 0) {
        if (state->unk3BD != 0) {
            numControllers = 2;
        } else {
            numControllers = state->unk3BC;
        }

        if (numControllers != 0) {
            i = 0;
            selectedAlpha = 0xFF;
            unselectedAlpha = 0x80;
            do {
                selectedSlot = state->unk3BB;
                if (i == selectedSlot) {
                    slot = &arg0->elements[i + (state->unk3BD << 2)];
                    slot->alpha = selectedAlpha;
                } else {
                    slot = &arg0->elements[i + (state->unk3BD << 2)];
                    slot->alpha = unselectedAlpha;
                }

                debugEnqueueCallback(8, 1, func_80012518_13118, &arg0->elements[i + (state->unk3BD << 2)]);
                i++;
            } while (i < numControllers);
        }
    }

    debugEnqueueCallback(8, 1, func_8000FED0_10AD0, &arg0->elements[6]);
    debugEnqueueCallback(8, 1, func_8000FED0_10AD0, &arg0->elements[6].unkC);
}

void cleanupControllerSlotDisplay(void **arg0) {
    arg0[1] = freeNodeMemory(arg0[1]);
}

void cleanupPressStartPrompt(void **);

void func_800167B0_173B0(Struct16728 *);

void func_80016728_17328(Struct16728 *arg0) {
    void *dmaResult;

    dmaResult = loadCompressedData(&_418520_ROM_START, &_418520_ROM_END, 0x2238);
    setCleanupCallback(cleanupPressStartPrompt);
    arg0->unk0 = 0x58;
    arg0->unk2 = 0x30;
    arg0->unk8 = 0x7;
    arg0->unk4 = dmaResult;
    arg0->unkC = 0x1E;
    arg0->unkE = 0;
    arg0->unkD = 0;
    setCallback(func_800167B0_173B0);
}

void func_800167B0_173B0(Struct16728 *arg0) {
    if (arg0->unkC == 0) {
        arg0->unkD++;
        if ((arg0->unkD & 1) == 0) {
            arg0->unkE = (arg0->unkE + 1) & 1;
            arg0->unk8 = arg0->unkE + 7;
        }
        if (arg0->unkD == 0x10) {
            arg0->unkD = 0;
            arg0->unkC = 0x1E;
        }
    } else {
        arg0->unkC--;
    }
    debugEnqueueCallback(8, 1, func_8000FED0_10AD0, arg0);
}

void cleanupPressStartPrompt(void **arg0) {
    arg0[1] = freeNodeMemory(arg0[1]);
}

void initTitleEffectModel(ModelEntity *arg0) {
    ColorData sp10[3];
    ColorData sp28;
    GameState *alloc;

    alloc = (GameState *)getCurrentAllocation();
    initModelEntity(arg0, 2, &alloc->audioPlayer2);
    setCleanupCallback(cleanupTitleEffectModel);
    setupModelEntityLighting(arg0, sp10, &sp28);
    setCallback(renderTitleEffectModel);
}

void renderTitleEffectModel(ModelEntityRenderState *state) {
    renderModelEntity(state);
}

void cleanupTitleEffectModel(EffectState *arg0) {
    freeEffectResources(arg0);
}

void initTitleCharacterModel(TitleCharacterState *arg0) {
    GameState *alloc;
    s32 temp;

    alloc = (GameState *)getCurrentAllocation();
    arg0->sceneModel = createSceneModel(arg0->characterIndex + 0x32, &alloc->audioPlayer2);
    setCleanupCallback(cleanupTitleCharacterModel);
    temp = 0x8000;
    arg0->flyAwayState = 0;
    arg0->yVelocity = temp;
    arg0->animSequenceIndex = 0;
    temp = (s32)gTitleCharacterAnimSequences[0];
    arg0->animTimer = 0;
    arg0->animSequencePtr = (u16 *)temp;
    setCallback(setupTitleCharacterTransform);
}

void setupTitleCharacterTransform(TitleCharacterState *arg0) {
    getCurrentAllocation();
    applyTransformToModel(arg0->sceneModel, &gTitleCharacterTransforms[arg0->characterIndex]);
    memcpy(&arg0->transform, &gTitleCharacterTransforms[arg0->characterIndex], 0x20);
    arg0->currentAnim = *arg0->animSequencePtr;
    arg0->animSequencePtr = arg0->animSequencePtr + 1;
    setModelAnimation(arg0->sceneModel, arg0->currentAnim);
    updateModelGeometry(arg0->sceneModel);
    setCallback(updateTitleCharacterAnimation);
}

void updateTitleCharacterAnimation(TitleCharacterState *arg0) {
    GameState *alloc;
    s32 clearResult;
    u16 animValue;

    alloc = (GameState *)getCurrentAllocation();
    clearResult = clearModelRotation(arg0->sceneModel);
    updateModelGeometry(arg0->sceneModel);

    if (clearResult != 0) {
        animValue = *arg0->animSequencePtr;
        arg0->animSequencePtr = arg0->animSequencePtr + 1;

        if (animValue == 0xFFFF) {
            arg0->animSequenceIndex = (arg0->animSequenceIndex + 1) & 1;
            arg0->animSequencePtr = gTitleCharacterAnimSequences[arg0->animSequenceIndex];
            animValue = *arg0->animSequencePtr;
            arg0->animSequencePtr = arg0->animSequencePtr + 1;

            if (arg0->animSequenceIndex == 1) {
                if (alloc->unk3C1 != 0) {
                    alloc->unk3C1 = 1;
                }

                if (arg0->characterIndex != 5) {
                    if (arg0->characterIndex != 4) {
                        setAnimationIndex(arg0->sceneModel, 0);
                    } else {
                        setAnimationIndex(arg0->sceneModel, 4);
                    }
                }
            } else {
                setAnimationIndex(arg0->sceneModel, -1);
            }
        }

        arg0->currentAnim = animValue;
        setModelAnimation(arg0->sceneModel, (s16)animValue);
    }

    if (arg0->characterIndex == 6) {
        if (alloc->unlockAllCheatProgress == 0xF0) {
            setCallback(handleUnlockAllCharacterAnim);
            return;
        }
    }

    if (arg0->characterIndex == 5) {
        if (alloc->partialUnlockCheatProgress == 0xF0) {
            setCallback(handlePartialUnlockCharacterAnim);
        }
    }
}

void handleUnlockAllCharacterAnim(TitleCharacterState *arg0) {
    s32 clearResult;
    u16 animValue;

    getCurrentAllocation();
    clearResult = clearModelRotation(arg0->sceneModel);
    updateModelGeometry(arg0->sceneModel);

    if (clearResult == 0) {
        return;
    }

    if (arg0->currentAnim == 4) {
        setModelAnimation(arg0->sceneModel, 8);
        setCallback(updateCharacterFlyAway);
        return;
    }

    animValue = *arg0->animSequencePtr;
    if (animValue != 0xFFFF) {
        arg0->currentAnim = animValue;
        arg0->animSequencePtr += 1;
    } else {
        arg0->currentAnim = 8;
        setCallback(updateCharacterFlyAway);
    }
    setModelAnimation(arg0->sceneModel, (s16)arg0->currentAnim);
}

void updateCharacterFlyAway(TitleCharacterState *arg0) {
    s32 clearResult;

    getCurrentAllocation();
    clearResult = clearModelRotation(arg0->sceneModel);
    updateModelGeometry(arg0->sceneModel);

    switch (arg0->flyAwayState) {
        case 0:
            if (clearResult != 0) {
                setModelAnimation(arg0->sceneModel, 9);
                arg0->flyAwayState = 1;
            }
            break;
        case 1: {
            s32 temp = arg0->yVelocity;
            s32 div = temp / 100;
            temp = temp + div * 9;
            arg0->yVelocity = temp;
            arg0->transform.translation.y = arg0->transform.translation.y + temp;
            applyTransformToModel(arg0->sceneModel, &arg0->transform);
            if (arg0->transform.translation.y > 0x57FFFF) {
                func_80069CF8_6A8F8();
            }
        } break;
    }
}

void handlePartialUnlockCharacterAnim(TitleCharacterState *arg0) {
    s32 clearResult;
    u16 animValue;

    getCurrentAllocation();
    clearResult = clearModelRotation(arg0->sceneModel);
    updateModelGeometry(arg0->sceneModel);

    if (clearResult == 0) {
        return;
    }

    if (arg0->currentAnim == 4) {
        arg0->currentAnim = 8;
        arg0->animTimer = 0x11;
        setModelAnimation(arg0->sceneModel, 8);
        setCallback(updatePartialUnlockAnim);
        return;
    }

    animValue = *arg0->animSequencePtr;
    if (animValue != 0xFFFF) {
        arg0->currentAnim = animValue;
        arg0->animSequencePtr += 1;
    } else {
        arg0->currentAnim = 8;
        arg0->animTimer = 0x11;
        setCallback(updatePartialUnlockAnim);
    }
    setModelAnimation(arg0->sceneModel, (s16)arg0->currentAnim);
}

void updatePartialUnlockAnim(TitleCharacterState *arg0) {
    s32 clearResult;

    clearResult = clearModelRotation(arg0->sceneModel);
    updateModelGeometry(arg0->sceneModel);

    if (clearResult != 0 && arg0->currentAnim == 8) {
        arg0->currentAnim++;
        setModelAnimation(arg0->sceneModel, (s16)arg0->currentAnim);
    }

    if (arg0->animTimer != 0) {
        arg0->animTimer--;
        if (arg0->animTimer == 0) {
            setAnimationIndex(arg0->sceneModel, 2);
        }
    }
}

void cleanupTitleCharacterModel(TitleCharacterState *arg0) {
    arg0->sceneModel = destroySceneModel(arg0->sceneModel);
}
