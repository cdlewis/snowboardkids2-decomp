#include "1DFAA0.h"
#include "1DD170.h"
#include "20F0.h"
#include "68CF0.h"
#include "6E840.h"
#include "main.h"
#include "task_scheduler.h"

typedef struct {
    void *start;
    void *end;
    u32 size;
    u32 padding;
} D_800BA960_1E7A10_node;

typedef struct {
    s8 unk0;
    s8 unk1;
    s16 unk2;
    s32 unk4;
    u8 padding[0xDC];
    s16 unkE4;
} TaskData;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
} StateEntryItem;

typedef struct {
    u8 padding0[0xC];
    /* 0xC */ u16 current_index;
    u8 padding[0x5];
    /* 0x13 */ u8 unk13;
    u8 padding3[0xC];
    StateEntryItem items[2];
    s32 unk34;
    /* 0x38 */ u16 next_index;
    u8 padding2[0x2];
    s16 unk3C;
    s8 unk3E;
    s8 unk3F;
} StateEntry;

typedef struct {
    u8 padding[0xFF7];
    s8 unkFF7;
} func_800B29F0_1DFAA0_arg;

typedef struct {
    u8 padding[0x16];
    u16 unk16;
} UIResource;

typedef struct {
    s32 unk00;
    void *unk04;
    u8 pad08[0x86];
    s16 angle;
    u8 pad90[0x18];
} CutsceneSlotData;

typedef struct {
    u8 padding[0x48];
    void *model;
    CutsceneSlotData slotData;
} CutsceneSlot;

typedef struct {
    /* 0x0 */ UIResource *uiResource;
    /* 0x4 */ s8 pad4[0x8];
    /* 0xC */ void *sceneContext;
    /* 0x10 */ s8 pad10[0x3C];
    /* 0x4C */ void *unk4C;
    /* 0x50 */ s8 pad50[0x48];
    /* 0x98 */ u16 currentFrame;
    /* 0x9A */ u16 maxFrame;
    /* 0x9C */ u16 endFrame;
    /* 0x9E */ s8 pad9E[0x2];
    /* 0xA0 */ s8 debugText[0x8];
    /* 0xA8 */ CutsceneSlot slots[16];
    /* 0xFE8 */ void *textRenderer;
    /* 0xFEC */ s8 padFEC[0x8];
    /* 0xFF4 */ s8 showDebugInfo;
    /* 0xFF5 */ s8 enableTransparency;
    /* 0xFF6 */ s8 unused_FF6;
    /* 0xFF7 */ s8 skipAnimation;
    /* 0xFF8 */ s8 padFF8[0x220];
    /* 0x1218 */ void *shadowModel;
    /* 0x121C */ void *reflectionModel;
    /* 0x1220 */ s32 cameraAnimationTimer;
} CutsceneManager;

extern StateEntry *D_800BAEBC_1E7F6C;
extern s32 D_800AB050_A23C0;
extern StateEntry D_800BAEC8_1E7F78[];
extern u8 D_800BAF06_1E7FB6;
extern u8 D_800BAEB0_1E7F60;
extern s8 D_800BAEB4_1E7F64;
extern D_800BA960_1E7A10_node D_800BA960_1E7A10[];
extern s8 D_800BAE00_1E7EB0[];

extern s32 initializeSlotState(void *, void *, s32 slot);
extern s32 updateSlotData(void *, s32 slot);
extern s32 syncModelFromSlot(void *, void *);
extern s32 advanceSceneManager(void *);
extern s32 finalizeAnimationLoop(void *);
extern s32 setupSlotTransform(void *);

u8 getCutsceneSlotCount(void);
s32 findEventAtFrame(u8 a0, u16 a1);
StateEntryItem *func_800B34B0_1E0560(u8);
s32 func_800B3D24_1E0DD4(u8, u16);
StateEntry *getStateEntry(u16);
void func_800B477C_1E182C(void);

void func_800B29F0_1DFAA0(func_800B29F0_1DFAA0_arg *arg0) {
    arg0->unkFF7 = 1;
}

void func_800B29FC_1DFAAC(func_800B29F0_1DFAA0_arg *arg0) {
    if (arg0->unkFF7 != 0 && (D_800AB050_A23C0 & 0x8000)) {
        arg0->unkFF7 = 0;
    }
}

typedef struct {
    u8 padding[0xEC];
    s8 unkEC;
    u8 padding2[0x0];
    s32 unkF4;
} func_800B2A24_1DFAD4_arg;
void func_800B2A24_1DFAD4(func_800B2A24_1DFAD4_arg *arg0, s16 arg1) {
    func_800B2A24_1DFAD4_arg *new_var = arg0 + arg1;
    new_var->unkEC = 1;
}

void func_800B2A50_1DFB00(func_800B2A24_1DFAD4_arg *arg0, s16 arg1) {
    func_800B2A24_1DFAD4_arg *new_var = arg0 + arg1;
    new_var->unkEC = 0;
}

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B2A78_1DFB28);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B2AA0);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B2C78);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B2D04);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B2D68_1DFE18);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B2DCC_1DFE7C);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B2E48_1DFEF8);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B2F2C);

s32 processCutsceneFrame(CutsceneManager *uiManager) {
    s32 slotIndex;
    CutsceneSlot *currentModel;
    s32 i;
    CutsceneSlotData *slotData;
    s16 xOffset;
    s32 zScale;
    StateEntry *stateEntry;
    s32 shouldInit;
    u16 temp;
    s32 animTimer;
    u16 frameResult;
    u8 tempByte;

    if (uiManager->showDebugInfo) {
        sprintf(uiManager->debugText, D_800BAE00_1E7EB0, uiManager->currentFrame);
        debugEnqueueCallback(uiManager->uiResource->unk16, 6, &renderTextPalette, &uiManager->textRenderer);
    }

    while (uiManager->currentFrame <= uiManager->maxFrame && !uiManager->skipAnimation) {
        for (i = 0, slotIndex = 0; i < getCutsceneSlotCount(); i++) {
            currentModel = uiManager->slots[i].model;
            slotData = &uiManager->slots[i].slotData;

            frameResult = findEventAtFrame(i, uiManager->currentFrame);
            if ((frameResult & 0xFFFF) != 0xFFFF) {
                stateEntry = getStateEntry(frameResult);
                shouldInit = 1;

                if (uiManager->currentFrame < uiManager->maxFrame) {
                    tempByte = stateEntry->unk3E - 4;

                    if (tempByte < 2) {
                        shouldInit = 0;
                    } else {
                        temp = *(u16 *)&stateEntry->unk3E;
                        if (temp == 0x801) {
                            temp = 0;
                            shouldInit = temp;
                        }
                    }
                }

                if (shouldInit) {
                    initializeSlotState(stateEntry, uiManager, slotIndex >> 24);
                }
            }

            updateSlotData(uiManager, slotIndex >> 24);
            syncModelFromSlot(slotData, currentModel);

            slotIndex += 0x1000000;
        }

        uiManager->currentFrame++;
        advanceSceneManager(uiManager->sceneContext);
    }

    finalizeAnimationLoop(uiManager->sceneContext);

    if (!uiManager->skipAnimation) {
        uiManager->maxFrame++;
    }

    for (i = 0; i < getCutsceneSlotCount(); i++) {
        currentModel = uiManager->slots[i].model;
        slotData = &uiManager->slots[i].slotData;

        if (currentModel) {
            setupSlotTransform(slotData);
            applyTransformToModel(currentModel, &slotData->unk04);
            if (uiManager->enableTransparency) {
                setModelVisibility(currentModel, 1);
            } else {
                setModelVisibility(currentModel, 0);
            }

            updateModelGeometry(currentModel);

            if (slotData->angle != 0) {
                setModelRotation(currentModel, slotData->angle);
            } else {
                clearModelRotation(currentModel);
            }
        }
    }

    animTimer = uiManager->cameraAnimationTimer;
    xOffset = -((animTimer * 120) >> 16);
    zScale = (animTimer * 119) >> 16;

    setModelRenderMode(uiManager->pad10, uiManager->enableTransparency);
    setModelCameraTransform(uiManager->uiResource, 0, 0, -0xA0, xOffset, 0x9F, zScale);
    setModelCameraTransform(uiManager->shadowModel, 0, 0, -0xA0, xOffset, 0x9F, zScale);
    setModelCameraTransform(uiManager->reflectionModel, 0, 0, -0xA0, xOffset, 0x9F, zScale);

    return (uiManager->currentFrame <= uiManager->endFrame) ? 1 : 0;
}

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3360);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3420_1E04D0);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3430_1E04E0);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3440_1E04F0);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3450_1E0500);

u8 getCutsceneSlotCount(void) {
    return D_800BAEBC_1E7F6C->unk13;
}

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3470_1E0520);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3480_1E0530);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3490_1E0540);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B34A0_1E0550);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B34B0_1E0560);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B34D0_1E0580);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B34E0_1E0590);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B34F0);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3500_1E05B0);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3510_1E05C0);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3540_1E05F0);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3570);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B36C0);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3734_1E07E4);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3790_1E0840);

u16 func_800B384C_1E08FC(void) {
    u16 index = D_800BAEBC_1E7F6C->current_index;
    StateEntry *currentEntry = &D_800BAEBC_1E7F6C[index];

    D_800BAEBC_1E7F6C->current_index = currentEntry[3].next_index;

    return index;
}

void resetScriptState(u8 *arg0) {
    s32 i;

    for (i = 0x37; i >= 0; i--) {
        arg0[i] = 0;
    }
}

void func_800B388C_1E093C(s32 arg0) {
    StateEntry *temp;

    resetScriptState(D_800BAEBC_1E7F6C[arg0 + 3].padding0);

    temp = (D_800BAEBC_1E7F6C + arg0 + 3);
    temp->unk3C = 0;

    temp = D_800BAEBC_1E7F6C + arg0 + 3;
    temp->unk3E = 0;

    temp = D_800BAEBC_1E7F6C + arg0 + 3;
    temp->unk3F = 0;
}

INCLUDE_ASM("asm/nonmatchings/1DFAA0", initializeCutsceneSystem);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3B40);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3B68_1E0C18);

s32 findEventAtFrame(u8 a0, u16 a1) {
    StateEntry *base;
    StateEntry *temp;
    u16 current;

    base = D_800BAEBC_1E7F6C;
    current = base->items[a0].unk0;

    while (current != 0xFFFF) {
        temp = getStateEntry(current);
        if (a1 < (u16)temp->unk3C) {
            return 0xFFFF;
        }

        temp = getStateEntry(current);
        if ((u16)temp->unk3C == a1) {
            return current;
        }

        temp = getStateEntry(current);
        current = temp->next_index;
    }

    return 0xFFFF;
}

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3D24_1E0DD4);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3E58_1E0F08);

StateEntry *getStateEntry(u16 arg0) {
    return &D_800BAEBC_1E7F6C[arg0 + 3];
}

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3F64_1E1014);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B3FFC_1E10AC);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B41E0_1E1290);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B4258_1E1308);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B4288_1E1338);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B4294_1E1344);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B42B0_1E1360);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B42E8_1E1398);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B4320_1E13D0);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B4378_1E1428);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B44A8_1E1558);

void func_800B4534_1E15E4(s32 arg0, s32 arg1) {
    s32 temp_s0;
    StateEntry *dest;
    StateEntry *new_var;
    s32 i;

    new_var = D_800BAEC8_1E7F78;
    temp_s0 = (-((~func_800B34B0_1E0560(arg0)->unk4) != 0)) | 1;

    if (D_800BAEB0_1E7F60 != 0 && getCategorySkipValue(D_800BAF06_1E7FB6) != temp_s0) {
        s32 var_a0 = findEventAtFrame(arg0, arg1);
        if ((var_a0 & 0xFFFF) == 0xFFFF) {
            var_a0 = func_800B3D24_1E0DD4(arg0, arg1);
        }

        dest = getStateEntry(var_a0);
        for (i = 0; i < 0x38; i++) {
            dest->padding0[i] = new_var->padding0[i];
        }

        dest->unk3E = new_var->unk3E;
        dest->unk3F = new_var->unk3F;

        D_800BAEB4_1E7F64 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B462C_1E16DC);

void *func_800B4680_1E1730(s8 arg0) {
    D_800BA960_1E7A10_node *node;

    if (arg0 < 0x10) {
        node = &D_800BA960_1E7A10[arg0];
        if (D_800BA960_1E7A10[arg0].start != NULL) {
            return dmaRequestAndUpdateStateWithSize(node->start, node->end, node->size);
        }
    }

    return NULL;
}

void func_800B46E0(s32 arg0, s8 arg1, s16 arg2) {
    TaskData *task;

    if (arg2 == 0) {
        return;
    }

    if (arg1 >= 0x10) {
        return;
    }

    if (D_800BA960_1E7A10[arg1].start == NULL) {
        return;
    }

    task = (TaskData *)scheduleTask(&func_800B477C_1E182C, 1, 0, 0x64);
    if (task != NULL) {
        task->unk0 = 0;
        task->unk1 = arg1;
        task->unk2 = arg2;
        task->unk4 = arg0;
        task->unkE4 = 0xFF;
    }
}

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B477C_1E182C);

INCLUDE_ASM("asm/nonmatchings/1DFAA0", func_800B4914_1E19C4);

typedef struct {
    u8 padding[0x8];
    void *unk8;
} func_800B4ACC_1E1B7C_arg;
void func_800B4ACC_1E1B7C(func_800B4ACC_1E1B7C_arg *arg0) {
    freeNodeMemory(arg0->unk8);
}
