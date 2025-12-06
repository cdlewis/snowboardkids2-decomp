#include "1DFAA0.h"
#include "1DD170.h"
#include "20F0.h"
#include "68CF0.h"
#include "6E840.h"
#include "main.h"
#include "task_scheduler.h"

typedef struct {
    u8 padding[0xC];
    void *unkC;
    func_80000710_1310_arg unk10;
    u8 padding2[0x20];
    s32 unkB8;
    u8 padding3[0x2C];
    u16 unkE8;
    u8 padding4[0x6];
    SceneModel *unkF0;
    u8 padding5[0x0];
} func_800B2C78_arg;

typedef struct {
    u8 padding[0x2D8];
    void *unk2D8;
    void *unk2DC;
    s32 unk2E0;
} func_800B2AA0_slot16;

extern StateEntry *D_800BAEBC_1E7F6C;
extern s32 D_800BAEB8_1E7F68;
extern StateEntry *D_800BA93C_1E79EC;
extern StateEntry *D_800BA95C_1E7A0C;
extern s32 gButtonsPressed[];
extern StateEntry D_800BAEC8_1E7F78[];
extern u8 D_800BAF06_1E7FB6;
extern u8 D_800BAEB0_1E7F60;
extern s16 D_800BAEB2_1E7F62;
extern s8 D_800BAEB4_1E7F64;
extern s16 D_800BAEC0_1E7F70;
extern D_800BA960_1E7A10_node D_800BA960_1E7A10[];
extern s8 D_800BAE00_1E7EB0[];
extern u8 identityMatrix[];

extern s32 initializeSlotState(StateEntry *, CutsceneManager *, s32);
extern s32 updateSlotData(void *, s32 slot);
extern s32 syncModelFromSlot(void *, void *);
extern s16 advanceSceneManager(void *);
extern s32 finalizeAnimationLoop(void *);
extern s32 setupSlotTransform(void *);
extern void func_800B5BFC_1E2CAC(void *);
extern void n_alSeqpDelete(s32 *);
extern s32 func_800084D8_90D8(StateEntry *, s32, s32);
extern s32 func_800084E0_90E0(StateEntry *, s32, s32);
extern s32 func_8003A284_3AE84(s32, StateEntry **);
extern s32 func_8003A28C_3AE8C(void);

u8 getCutsceneSlotCount(void);
s32 findEventAtFrame(u8 a0, u16 a1);
s32 func_800B3D24_1E0DD4(u8, u16);
StateEntry *getStateEntry(u16);
u16 func_800B3B68_1E0C18(u8, u16, s32);
void func_800B477C_1E182C(void *);
void func_800B4534_1E15E4(s32, s32);
void func_800B7760_1E4810(void *, s32, s32);
void func_800B77C4_1E4874(void *, s32, s32);
void func_800B7828_1E48D8(void *, s32, s32);

s16 func_800B3490_1E0540(void);
StateEntryItem *func_800B34B0_1E0560(s32 arg0);
extern s32 func_80001904_2504(s16);
extern void func_8000150C_210C(void *);
extern void func_800B66B4_1E3764(void *);
extern void *func_800B5B38_1E2BE8(u16);
extern void func_80003000_3C00(void *, u8, void *, u8, void *);
extern void func_800B5B30_1E2BE0(void *, s8);
extern void func_8000C440_D040(void *, s32, s32, u16, s32, s32, s32);
extern void func_8000C278_CE78(void *);

void func_800B29F0_1DFAA0(func_800B29F0_1DFAA0_arg *arg0) {
    arg0->unkFF7 = 1;
}

void func_800B29FC_1DFAAC(func_800B29F0_1DFAA0_arg *arg0) {
    if (arg0->unkFF7 != 0 && (gButtonsPressed[0] & A_BUTTON)) {
        arg0->unkFF7 = 0;
    }
}

void func_800B2A24_1DFAD4(CutsceneManager *arg0, s16 arg1) {
    arg0->slots[arg1].unk44 = 1;
}

void func_800B2A50_1DFB00(CutsceneManager *arg0, s16 arg1) {
    arg0->slots[arg1].unk44 = 0;
}

CutsceneSlot *func_800B2A78_1DFB28(CutsceneManager *arg0, s16 arg1) {
    return &arg0->slots[arg1];
}

void func_800B2AA0(func_800B2C78_arg *arg0, Node_70B00 *arg1, void *arg2, void *arg3) {
    s32 i;
    s16 initialValue;
    s32 needsVisibility;
    StateEntryItem *slot;
    s32 slotOffset;

    initialValue = func_800B3490_1E0540();
    needsVisibility = func_8000056C_116C((func_8000056C_116C_arg *)&arg0->unk10, initialValue, arg1);
    setModelRenderMode((setModelRenderMode_arg *)&arg0->unk10, 0);

    i = 0;
    slotOffset = 0xA8;

    while (i < (getCutsceneSlotCount() & 0xFF)) {
        slot = func_800B34B0_1E0560(i);
        arg0[i].unkE8 = slot->unk4;

        if ((s16)slot->unk4 != -1) {
            if (func_80001904_2504(slot->unk4)) {
                arg0[i].unkF0 = func_800019B8_25B8(slot->unk4, arg1, slot->unk6, slot->unk7, slot->unk8, slot->unk9);
            } else {
                arg0[i].unkF0 = func_8000198C_258C(slot->unk4, arg1);
            }

            if (needsVisibility) {
                func_8000150C_210C(arg0[i].unkF0);
            }
        } else {
            arg0[i].unkF0 = NULL;
        }

        func_800B66B4_1E3764((u8 *)arg0 + slotOffset + 0x4C);

        slotOffset += 0xF4;
        i++;
    }

    arg0->unkC = func_800B5B38_1E2BE8(arg1->id);

    if (needsVisibility) {
        func_80003000_3C00(arg0, 0, (void *)0x8000, 1, (void *)0x10000);
        func_800B5B30_1E2BE0(arg0->unkC, 1);
    }

    func_8000C440_D040((u8 *)arg0 + 0xFF8, 2, 0xC, arg1->id, 0, 2, 0);
    func_8000C278_CE78((u8 *)arg0 + 0xFF8);

    ((func_800B2AA0_slot16 *)&arg0[16])->unk2D8 = arg2;
    ((func_800B2AA0_slot16 *)&arg0[16])->unk2DC = arg3;
    ((func_800B2AA0_slot16 *)&arg0[16])->unk2E0 = 0x10000;
}

void func_800B2C78(func_800B2C78_arg *arg0) {
    s32 i;

    n_alSeqpDelete(&arg0[16].unkB8);

    func_800B5BFC_1E2CAC(arg0->unkC);

    for (i = 0; i < (getCutsceneSlotCount() & 0xFF); i++) {
        if (arg0[i].unkF0 != 0) {
            arg0[i].unkF0 = func_80002014_2C14(arg0[i].unkF0);
        }
    }

    func_80000710_1310(&arg0->unk10);
}

void func_800B2D04(func_800B2C78_arg *arg0) {
    s32 i;

    for (i = 0; i < getCutsceneSlotCount(); i++) {
        if (arg0[i].unkF0 != NULL) {
            setModelVisibility(arg0[i].unkF0, 0);
        }
    }
}

void func_800B2D68_1DFE18(func_800B2C78_arg *arg0) {
    s32 i;

    for (i = 0; i < getCutsceneSlotCount(); i++) {
        if (arg0[i].unkF0 != NULL) {
            setModelVisibility(arg0[i].unkF0, 1);
        }
    }
}

void func_800B2DCC_1DFE7C(func_800B2C78_arg *arg0) {
    s32 i;
    func_800B2C78_arg *ptr;

    i = 0;
    ptr = arg0;

    while (i < (getCutsceneSlotCount() & 0xFF)) {
        func_800B34B0_1E0560(i);

        if (ptr[i].unkF0 != NULL) {
            applyTransformToModel(ptr[i].unkF0, (applyTransformToModel_arg1 *)identityMatrix);
            func_800021B8_2DB8(ptr[i].unkF0, 0);
        }

        i++;
    }
}

void func_800B2E48_1DFEF8(func_800B2C78_arg *arg0) {
    s32 i;
    func_800B2C78_arg *currentSlot;
    s32 slotOffset;
    void *model;
    s32 temp_offset;
    u8 *slotDataA8;
    u8 *transformPtr;

    i = 0;
    currentSlot = arg0;
    slotOffset = 0;

    while (i < (getCutsceneSlotCount() & 0xFF)) {
        func_800B34B0_1E0560(i);
        model = currentSlot->unkF0;

        if (model != NULL) {
            temp_offset = slotOffset + 0xA8;
            slotDataA8 = (u8 *)arg0 + temp_offset;
            transformPtr = slotDataA8 + 0x4C;

            func_800B7760_1E4810(transformPtr, 0x10000, 0);
            func_800B77C4_1E4874(transformPtr, 0x10000, 0);
            func_800B7828_1E48D8(transformPtr, 0x10000, 0);
            setupSlotTransform(transformPtr);
            applyTransformToModel(model, (applyTransformToModel_arg1 *)(slotDataA8 + 0x50));
        }

        currentSlot = (func_800B2C78_arg *)((u8 *)currentSlot + 0xF4);
        slotOffset += 0xF4;
        i++;
    }
}

INCLUDE_ASM("asm/nonmatchings/cutscene/1DFAA0", func_800B2F2C);

s32 processCutsceneFrame(CutsceneManager *uiManager) {
    s32 slotIndex;
    SceneModel *currentModel;
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
        sprintf((char *)uiManager->debugText, (const char *)D_800BAE00_1E7EB0, uiManager->currentFrame);
        debugEnqueueCallback(uiManager->uiResource->slot_index, 6, &renderTextPalette, &uiManager->textRenderer);
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

    setModelRenderMode(&uiManager->unk10, uiManager->enableTransparency);
    setModelCameraTransform(uiManager->uiResource, 0, 0, -0xA0, xOffset, 0x9F, zScale);
    setModelCameraTransform(uiManager->shadowModel, 0, 0, -0xA0, xOffset, 0x9F, zScale);
    setModelCameraTransform(uiManager->reflectionModel, 0, 0, -0xA0, xOffset, 0x9F, zScale);

    return (uiManager->currentFrame <= uiManager->endFrame) ? 1 : 0;
}

INCLUDE_ASM("asm/nonmatchings/cutscene/1DFAA0", func_800B3360);

void *func_800B3420_1E04D0(void) {
    return &D_800BAEBC_1E7F6C->padding0[4];
}

void *func_800B3430_1E04E0(void) {
    return &D_800BAEBC_1E7F6C->padding0[8];
}

u16 func_800B3440_1E04F0(void) {
    return D_800BAEBC_1E7F6C->unk10;
}

u8 func_800B3450_1E0500(void) {
    return D_800BAEBC_1E7F6C->unk12;
}

u8 getCutsceneSlotCount(void) {
    return D_800BAEBC_1E7F6C->unk13;
}

u16 func_800B3470_1E0520(void) {
    return D_800BAEBC_1E7F6C->unk14;
}

u16 func_800B3480_1E0530(void) {
    return D_800BAEBC_1E7F6C->unk16;
}

s16 func_800B3490_1E0540(void) {
    return D_800BAEBC_1E7F6C->unk18;
}

void func_800B34A0_1E0550(s16 arg0) {
    D_800BAEBC_1E7F6C->unk18 = arg0;
}

StateEntryItem *func_800B34B0_1E0560(s32 arg0) {
    return &D_800BAEBC_1E7F6C->items[arg0];
}

s16 func_800B34D0_1E0580(void) {
    return D_800BAEBC_1E7F6C->unk1A;
}

void func_800B34E0_1E0590(s16 arg0) {
    D_800BAEBC_1E7F6C->unk1A = arg0;
}

u8 func_800B34F0(void) {
    return D_800BAEBC_1E7F6C->unk1C;
}

void func_800B3500_1E05B0(u8 arg0) {
    D_800BAEBC_1E7F6C->unk1C = arg0;
}

s32 func_800B3510_1E05C0(void) {
    func_800084D8_90D8(D_800BAEBC_1E7F6C, D_800BAEB8_1E7F68, 0);
    return 1;
}

s32 func_800B3540_1E05F0(void) {
    func_800084E0_90E0(D_800BAEBC_1E7F6C, D_800BAEB8_1E7F68, 0);
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/cutscene/1DFAA0", func_800B3570);

s32 func_800B36C0(void *arg0) {
    s32 result;
    s32 *src;
    s32 *arg0_int;

    arg0_int = (s32 *)arg0;
    src = (s32 *)D_800BAEBC_1E7F6C;

    result = 1;

    if (src[1] != arg0_int[1]) {
        goto skip_copy;
    }

    if (src[2] != arg0_int[2]) {
        goto skip_copy;
    }

    memcpy(D_800BAEBC_1E7F6C, arg0, 0x78C0);
    goto done;

skip_copy:
    result = 0;

done:
    freeNodeMemory(arg0);
    return result;
}

INCLUDE_ASM("asm/nonmatchings/cutscene/1DFAA0", func_800B3734_1E07E4);

typedef struct {
    u8 _pad0[0x4];
    s32 unk4;
    s32 unk8;
} SaveDataBuffer;

s32 func_800B3790_1E0840(void) {
    SaveDataBuffer *buffer;
    s32 result;
    SaveDataBuffer *compareData;
    StateEntry **ptr;

    buffer = allocateNodeMemory(D_800BAEB8_1E7F68);
    ptr = &D_800BA93C_1E79EC;
    D_800BA95C_1E7A0C = (StateEntry *)buffer;
    *ptr = (StateEntry *)D_800BAEB8_1E7F68;
    func_8003A284_3AE84(0, ptr);

    do {
        result = func_8003A28C_3AE8C();
    } while (result == -1);

    if (result == 0) {
        compareData = (SaveDataBuffer *)D_800BAEBC_1E7F6C;
        if (compareData->unk4 == buffer->unk4 && compareData->unk8 == buffer->unk8) {
            memcpy(D_800BAEBC_1E7F6C, buffer, 0x78C0);
        }
    }

    freeNodeMemory(buffer);
    return result;
}

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

INCLUDE_ASM("asm/nonmatchings/cutscene/1DFAA0", initializeCutsceneSystem);

void func_800B3B40(void) {
    if (D_800BAEBC_1E7F6C != NULL) {
        freeNodeMemory(D_800BAEBC_1E7F6C);
    }
}

u16 func_800B3B68_1E0C18(u8 arg0, u16 arg1, s32 arg2) {
    StateEntry *entry;
    u16 current;
    u16 next;
    u16 nextMasked;
    u16 arg1Masked;
    s32 arg2Copy;
    u16 ffff;

    current = D_800BAEBC_1E7F6C->items[arg0].unk0;
    arg2Copy = arg2;

    entry = getStateEntry(current);
    next = entry->next_index;

    if ((u16)next != 0xFFFF) {
        arg1Masked = arg1;
        ffff = 0xFFFF;
        do {
            entry = getStateEntry(current);
            nextMasked = next;
            if ((u16)entry->unk3C == arg1Masked) {
                if ((arg2Copy & 0xFF) != 0) {
                    return 0xFFFF;
                }
            }

            entry = getStateEntry(nextMasked);
            if (arg1Masked < (u16)entry->unk3C) {
                break;
            }
            current = next;
            entry = getStateEntry(nextMasked);
            next = entry->next_index;
        } while (next != ffff);
    }

    if ((arg2Copy & 0xFF) == 0) {
        entry = getStateEntry(current);
        if ((u16)entry->unk3C != arg1) {
            return 0xFFFF;
        }
    }

    return current;
}

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

s32 func_800B3D24_1E0DD4(u8 arg0, u16 arg1) {
    StateEntry *base;
    u16 searchResult;
    u16 searchMasked;
    u16 newIndex;
    u16 prevNext;
    StateEntry *entry;
    StateEntry *base2;

    if (D_800BAEBC_1E7F6C->unk10 >= 0x1E0) {
        goto ret_ffff;
    }

    searchResult = func_800B3B68_1E0C18(arg0 & 0xFF, arg1, 1);
    searchMasked = searchResult;

    if (searchMasked == 0xFFFF) {
        return 0xFFFF;
    }

    entry = getStateEntry(searchMasked);
    if ((u16)entry->unk3C != arg1) {
        goto do_work;
    }

ret_ffff:
    return 0xFFFF;

do_work:
    newIndex = func_800B384C_1E08FC();
    base = D_800BAEBC_1E7F6C;

    prevNext = *(u16 *)((u8 *)base + (u32)(searchMasked << 6) + 0xF8);
    *(u16 *)((u8 *)base + (u32)(searchMasked << 6) + 0xF8) = newIndex;

    if ((u16)prevNext != 0xFFFF) {
        *(u16 *)((u8 *)base + (u32)((u16)prevNext << 6) + 0xFA) = newIndex;
    }

    base2 = D_800BAEBC_1E7F6C;
    searchMasked = newIndex;
    base2 = (StateEntry *)((u8 *)base2 + (u32)(searchMasked << 6));
    *(u16 *)((u8 *)base2 + 0xFA) = searchResult;
    *(u16 *)((u8 *)base2 + 0xF8) = prevNext;

    func_800B388C_1E093C(searchMasked);

    D_800BAEBC_1E7F6C->unk10++;

    if (searchMasked != 0xFFFF) {
        entry = getStateEntry(searchMasked);
        entry->unk3C = arg1;

        entry = getStateEntry(searchMasked);
        entry->unk3E = 0;

        entry = getStateEntry(searchMasked);
        entry->unk3F = 0;
    }

    return searchMasked;
}

INCLUDE_ASM("asm/nonmatchings/cutscene/1DFAA0", func_800B3E58_1E0F08);

StateEntry *getStateEntry(u16 arg0) {
    return &D_800BAEBC_1E7F6C[arg0 + 3];
}

INCLUDE_ASM("asm/nonmatchings/cutscene/1DFAA0", func_800B3F64_1E1014);

INCLUDE_ASM("asm/nonmatchings/cutscene/1DFAA0", func_800B3FFC_1E10AC);

u16 func_800B41E0_1E1290(void) {
    u16 max_value;
    u16 slot;
    u16 index;
    StateEntry *entry;
    u16 value;

    max_value = 0;
    slot = 0;

    while (slot < getCutsceneSlotCount()) {
        index = func_800B3B68_1E0C18(slot, 0x8000, 1);
        entry = getStateEntry(index);
        value = entry->unk3C;
        if (max_value < value) {
            max_value = value;
        }
        slot++;
    }

    return max_value;
}

u16 func_800B4258_1E1308(u8 arg0) {
    u16 temp;
    StateEntry *entry;

    temp = func_800B3B68_1E0C18(arg0, 0x8000, 1);
    entry = getStateEntry(temp);
    return entry->unk3C;
}

StateEntry *func_800B4288_1E1338(void) {
    return D_800BAEBC_1E7F6C;
}

u8 *func_800B4294_1E1344(void) {
    return &D_800BAEBC_1E7F6C->padding0[D_800BAEB8_1E7F68] - 1;
}

u16 func_800B42B0_1E1360(u16 arg0) {
    u16 var_a1;
    s16 temp_v1;

    if (D_800BAEB4_1E7F64 != 0) {
        temp_v1 = D_800BAEC0_1E7F70;
        if (temp_v1 != -1) {
            var_a1 = temp_v1;
            return var_a1;
        }
    }
    return arg0;
}

u16 func_800B42E8_1E1398(u16 arg0) {
    if (D_800BAEB4_1E7F64 == 0) {
        return arg0;
    }
    if (D_800BAEB2_1E7F62 == -1) {
        return arg0;
    }
    return D_800BAEB2_1E7F62;
}

void func_800B4320_1E13D0(void) {
    s16 temp_v0;
    s16 temp_v0_2;
    u8 masked_arg0;
    u16 masked_arg1;

    if (D_800BAEB4_1E7F64 != 0) {
        temp_v0 = D_800BAEC0_1E7F70;
        if (temp_v0 != -1) {
            temp_v0_2 = D_800BAEB2_1E7F62;
            if (temp_v0_2 != -1) {
                masked_arg0 = temp_v0;
                masked_arg1 = temp_v0_2;
                func_800B4534_1E15E4(masked_arg0, masked_arg1);
            }
        }
    }
    D_800BAEB4_1E7F64 = 0;
}

INCLUDE_ASM("asm/nonmatchings/cutscene/1DFAA0", func_800B4378_1E1428);

void func_800B44A8_1E1558(u8 arg0, u16 arg1) {
    u16 eventId;
    StateEntry *src;
    StateEntry *dst;
    u8 *srcBytes;
    u8 *dstBytes;
    s32 i;
    u8 temp;

    eventId = findEventAtFrame(arg0, arg1);

    // Copy the current entry to D_800BAEC8_1E7F78
    if (eventId != 0xFFFF) {
        src = getStateEntry(eventId);
        dst = &D_800BAEC8_1E7F78[0];
        srcBytes = (u8 *)src;
        dstBytes = (u8 *)dst;

        for (i = 0; i < 0x38; i++) {
            dstBytes[i] = srcBytes[i];
        }

        dst->unk3E = src->unk3E;
        temp = src->unk3F;
        D_800BAEB0_1E7F60 = 1;
        D_800BAEB4_1E7F64 = 0;
        dst->unk3F = temp;
    }
}

void func_800B4534_1E15E4(s32 arg0, s32 arg1) {
    s32 temp_s0;
    StateEntry *dest;
    StateEntry *new_var;
    s32 i;
    u8 masked_arg0 = arg0 & 0xFF;

    new_var = D_800BAEC8_1E7F78;
    temp_s0 = (-((~func_800B34B0_1E0560(masked_arg0)->unk4) != 0)) | 1;

    if (D_800BAEB0_1E7F60 != 0 && getCategorySkipValue(D_800BAF06_1E7FB6) != temp_s0) {
        s32 var_a0 = findEventAtFrame(masked_arg0, arg1);
        if ((var_a0 & 0xFFFF) == 0xFFFF) {
            var_a0 = func_800B3D24_1E0DD4(masked_arg0, arg1);
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

void func_800B462C_1E16DC(u8 arg0, u16 arg1, s32 arg2) {
    u16 result;
    StateEntry *entry;

    result = func_800B3B68_1E0C18(arg0, arg1, 0);

    if (result != 0xFFFF) {
        entry = getStateEntry(result);
        entry->unk3C += arg2;
    }
}

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

extern void func_80069CF8_6A8F8(void);
extern void func_80011924_12524(void);

typedef struct {
    void *start;
    void *end;
    u32 size;
    s8 unkC;
} FadeNode;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    u8 unk12;
    u8 unk13;
    u8 unk14;
} SubStruct;

typedef struct {
    s8 unk0;
    s8 unk1;
    s16 unk2;
    s32 unk4;
    void *unk8;
    SubStruct unkC[6];
    SubStruct unk9C;
    SubStruct unkB4;
    u8 unkCC[0x18];
    s16 unkE4;
} FadeTaskData2;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-non-prototype"
#pragma clang diagnostic ignored "-Wstrict-prototypes"
extern void func_800B4914_1E19C4();
extern void func_800B4ACC_1E1B7C();
#pragma clang diagnostic pop

void func_800B477C_1E182C(void *varg0) {
    FadeTaskData2 *arg0 = (FadeTaskData2 *)varg0;
    FadeNode *node;
    s32 i;

    node = (FadeNode *)&D_800BA960_1E7A10[arg0->unk1];
    arg0->unk8 = dmaRequestAndUpdateStateWithSize(node->start, node->end, node->size);

    for (i = 0; i < 6; i++) {
        arg0->unkC[i].unk0 = 0;
        arg0->unkC[i].unk2 = -0x50 + (i * 0x10);
        arg0->unkC[i].unk4 = arg0->unk8;
        arg0->unkC[i].unk8 = i;
        arg0->unkC[i].unkC = 0x400;
        arg0->unkC[i].unkA = 0x400;
        arg0->unkC[i].unkE = 0;
        arg0->unkC[i].unk10 = 0xFF;
        arg0->unkC[i].unk12 = 0;
        arg0->unkC[i].unk13 = 0;
        arg0->unkC[i].unk14 = 0;
    }

    switch (node->unkC) {
        case 0:
            arg0->unk9C.unk0 = 0;
            arg0->unk9C.unk2 = -0x20;
            arg0->unk9C.unk8 = 6;
            arg0->unk9C.unkC = 0x400;
            arg0->unk9C.unkA = 0x400;
            arg0->unk9C.unkE = 0;
            arg0->unk9C.unk10 = 0xFF;
            arg0->unk9C.unk12 = 0;
            arg0->unk9C.unk13 = 0;
            arg0->unk9C.unk14 = 0;
            arg0->unk9C.unk4 = arg0->unk8;
            break;
        case 1:
            arg0->unk9C.unk0 = 0;
            arg0->unk9C.unk2 = -0x28;
            arg0->unk9C.unk8 = 6;
            arg0->unk9C.unkC = 0x400;
            arg0->unk9C.unkA = 0x400;
            arg0->unk9C.unkE = 0;
            arg0->unk9C.unk10 = 0xFF;
            arg0->unk9C.unk12 = 0;
            arg0->unk9C.unk13 = 0;
            arg0->unk9C.unk14 = 0;
            arg0->unk9C.unk4 = arg0->unk8;

            arg0->unkB4.unk0 = 0;
            arg0->unkB4.unk2 = -0x16;
            arg0->unkB4.unk8 = 7;
            arg0->unkB4.unkC = 0x400;
            arg0->unkB4.unkA = 0x400;
            arg0->unkB4.unkE = 0;
            arg0->unkB4.unk10 = 0xFF;
            arg0->unkB4.unk12 = 0;
            arg0->unkB4.unk13 = 0;
            arg0->unkB4.unk14 = 0;
            arg0->unkB4.unk4 = arg0->unk8;
            break;
    }

    arg0->unkE4 = 0;
    setCleanupCallback((void (*)(void *))func_800B4ACC_1E1B7C);
    setCallback((void (*)(void *))func_800B4914_1E19C4);
}

typedef struct {
    s8 unk0;
    s8 unk1;
    s16 unk2;
    s16 unk4;
    u16 unk6;
    void *unk8;
    u8 unkC[0x90];
    u8 unk9C[0x18];
    u8 unkB4[0x18];
    u8 unkCC[0x18];
    s16 unkE4;
} FadeTaskData;

void func_800B4914_1E19C4(FadeTaskData *arg0) {
    FadeNode *node;
    s32 offset;
    u8 *ptr;
    s32 i;

    node = (FadeNode *)&D_800BA960_1E7A10[arg0->unk1];

    switch (arg0->unk0) {
        case 0:
            arg0->unkE4 += 10;
            if ((s16)arg0->unkE4 >= 0xFF) {
                arg0->unkE4 = 0xFF;
                arg0->unk0 = 1;
            }
            break;
        case 1:
            arg0->unk2--;
            if ((s16)arg0->unk2 < 0) {
                arg0->unk0 = 2;
            }
            break;
        case 2:
            arg0->unkE4 -= 10;
            if ((s16)arg0->unkE4 <= 0) {
                func_80069CF8_6A8F8();
                return;
            }
            break;
    }

    switch (node->unkC) {
        case 0:
            arg0->unk9C[0x14] = (u8)arg0->unkE4;
            debugEnqueueCallback(arg0->unk6, 0, &func_80011924_12524, &arg0->unk9C);
            break;
        case 1:
            arg0->unk9C[0x14] = (u8)arg0->unkE4;
            debugEnqueueCallback(arg0->unk6, 0, &func_80011924_12524, &arg0->unk9C);
            arg0->unkB4[0x14] = (u8)arg0->unkE4;
            debugEnqueueCallback(arg0->unk6, 0, &func_80011924_12524, &arg0->unkB4);
            break;
    }

    i = 0;
    offset = 0xC;
    ptr = (u8 *)arg0;
    do {
        ptr[0x20] = (u8)arg0->unkE4;
        debugEnqueueCallback(arg0->unk6, 0, &func_80011924_12524, (u8 *)arg0 + offset);
        offset += 0x18;
        ptr += 0x18;
        i++;
    } while (i < 6);
}

typedef struct {
    u8 padding[0x8];
    void *unk8;
} func_800B4ACC_1E1B7C_arg;
void func_800B4ACC_1E1B7C(func_800B4ACC_1E1B7C_arg *arg0) {
    freeNodeMemory(arg0->unk8);
}
