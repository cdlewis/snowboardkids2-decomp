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

void func_800168BC_174BC(ModelEntityRenderState *arg0);
void func_800168D8_174D8(EffectState *arg0);

extern Transform3D D_8008D5C4_8E1C4[];
extern u16 *D_8008D534_8E134[2];

void func_80016C28_17828(Struct16B68 *arg0);
void func_80016A00_17600(Struct16B68 *arg0);
void func_80016B68_17768(Struct16B68 *arg0);
void func_80016D0C_1790C(Struct16B68 *arg0);
void func_80016DE0_179E0(Struct16B68 *arg0);
void func_80016964_17564(Struct16B68 *arg0);
void func_80016E70_17A70(Struct16B68 *arg0);

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

void func_80016860_17460(ModelEntity *arg0) {
    ColorData sp10[3];
    ColorData sp28;
    GameState *alloc;

    alloc = (GameState *)getCurrentAllocation();
    initModelEntity(arg0, 2, &alloc->audioPlayer2);
    setCleanupCallback(func_800168D8_174D8);
    setupModelEntityLighting(arg0, sp10, &sp28);
    setCallback(func_800168BC_174BC);
}

void func_800168BC_174BC(ModelEntityRenderState *state) {
    renderModelEntity(state);
}

void func_800168D8_174D8(EffectState *arg0) {
    freeEffectResources(arg0);
}

void func_800168F4_174F4(Struct16B68 *arg0) {
    GameState *alloc;
    s32 temp;

    alloc = (GameState *)getCurrentAllocation();
    arg0->unk0 = createSceneModel(arg0->unk2F + 0x32, &alloc->audioPlayer2);
    setCleanupCallback(func_80016E70_17A70);
    temp = 0x8000;
    arg0->unk32 = 0;
    arg0->unk24 = temp;
    arg0->unk2E = 0;
    temp = (s32)D_8008D534_8E134[0];
    arg0->unk31 = 0;
    arg0->unk28 = (u16 *)temp;
    setCallback(func_80016964_17564);
}

void func_80016964_17564(Struct16B68 *arg0) {
    getCurrentAllocation();
    applyTransformToModel(arg0->unk0, &D_8008D5C4_8E1C4[arg0->unk2F]);
    memcpy(&arg0->unk4, &D_8008D5C4_8E1C4[arg0->unk2F], 0x20);
    arg0->unk2C = *arg0->unk28;
    arg0->unk28 = arg0->unk28 + 1;
    setModelAnimation(arg0->unk0, arg0->unk2C);
    updateModelGeometry(arg0->unk0);
    setCallback(func_80016A00_17600);
}

void func_80016A00_17600(Struct16B68 *arg0) {
    GameState *alloc;
    s32 clearResult;
    u16 animValue;

    alloc = (GameState *)getCurrentAllocation();
    clearResult = clearModelRotation(arg0->unk0);
    updateModelGeometry(arg0->unk0);

    if (clearResult != 0) {
        animValue = *arg0->unk28;
        arg0->unk28 = arg0->unk28 + 1;

        if (animValue == 0xFFFF) {
            arg0->unk2E = (arg0->unk2E + 1) & 1;
            arg0->unk28 = D_8008D534_8E134[arg0->unk2E];
            animValue = *arg0->unk28;
            arg0->unk28 = arg0->unk28 + 1;

            if (arg0->unk2E == 1) {
                if (alloc->unk3C1 != 0) {
                    alloc->unk3C1 = 1;
                }

                if (arg0->unk2F != 5) {
                    if (arg0->unk2F != 4) {
                        setAnimationIndex(arg0->unk0, 0);
                    } else {
                        setAnimationIndex(arg0->unk0, 4);
                    }
                }
            } else {
                setAnimationIndex(arg0->unk0, -1);
            }
        }

        arg0->unk2C = animValue;
        setModelAnimation(arg0->unk0, (s16)animValue);
    }

    if (arg0->unk2F == 6) {
        if (alloc->unlockAllCheatProgress == 0xF0) {
            setCallback(func_80016B68_17768);
            return;
        }
    }

    if (arg0->unk2F == 5) {
        if (alloc->partialUnlockCheatProgress == 0xF0) {
            setCallback(func_80016D0C_1790C);
        }
    }
}

void func_80016B68_17768(Struct16B68 *arg0) {
    s32 clearResult;
    u16 animValue;

    getCurrentAllocation();
    clearResult = clearModelRotation(arg0->unk0);
    updateModelGeometry(arg0->unk0);

    if (clearResult == 0) {
        return;
    }

    if (arg0->unk2C == 4) {
        setModelAnimation(arg0->unk0, 8);
        setCallback(func_80016C28_17828);
        return;
    }

    animValue = *arg0->unk28;
    if (animValue != 0xFFFF) {
        arg0->unk2C = animValue;
        arg0->unk28 += 1;
    } else {
        arg0->unk2C = 8;
        setCallback(func_80016C28_17828);
    }
    setModelAnimation(arg0->unk0, (s16)arg0->unk2C);
}

void func_80016C28_17828(Struct16B68 *arg0) {
    s32 clearResult;

    getCurrentAllocation();
    clearResult = clearModelRotation(arg0->unk0);
    updateModelGeometry(arg0->unk0);

    switch (arg0->unk32) {
        case 0:
            if (clearResult != 0) {
                setModelAnimation(arg0->unk0, 9);
                arg0->unk32 = 1;
            }
            break;
        case 1: {
            s32 temp = arg0->unk24;
            s32 div = temp / 100;
            temp = temp + div * 9;
            arg0->unk24 = temp;
            arg0->unk4.translation.y = arg0->unk4.translation.y + temp;
            applyTransformToModel(arg0->unk0, &arg0->unk4);
            if (arg0->unk4.translation.y > 0x57FFFF) {
                func_80069CF8_6A8F8();
            }
        } break;
    }
}

void func_80016D0C_1790C(Struct16B68 *arg0) {
    s32 clearResult;
    u16 animValue;

    getCurrentAllocation();
    clearResult = clearModelRotation(arg0->unk0);
    updateModelGeometry(arg0->unk0);

    if (clearResult == 0) {
        return;
    }

    if (arg0->unk2C == 4) {
        arg0->unk2C = 8;
        arg0->unk31 = 0x11;
        setModelAnimation(arg0->unk0, 8);
        setCallback(func_80016DE0_179E0);
        return;
    }

    animValue = *arg0->unk28;
    if (animValue != 0xFFFF) {
        arg0->unk2C = animValue;
        arg0->unk28 += 1;
    } else {
        arg0->unk2C = 8;
        arg0->unk31 = 0x11;
        setCallback(func_80016DE0_179E0);
    }
    setModelAnimation(arg0->unk0, (s16)arg0->unk2C);
}

void func_80016DE0_179E0(Struct16B68 *arg0) {
    s32 clearResult;

    clearResult = clearModelRotation(arg0->unk0);
    updateModelGeometry(arg0->unk0);

    if (clearResult != 0 && arg0->unk2C == 8) {
        arg0->unk2C++;
        setModelAnimation(arg0->unk0, (s16)arg0->unk2C);
    }

    if (arg0->unk31 != 0) {
        arg0->unk31--;
        if (arg0->unk31 == 0) {
            setAnimationIndex(arg0->unk0, 2);
        }
    }
}

void func_80016E70_17A70(Struct16B68 *arg0) {
    arg0->unk0 = destroySceneModel(arg0->unk0);
}
