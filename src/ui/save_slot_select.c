#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "assets.h"
#include "common.h"
#include "core/session_manager.h"
#include "graphics/graphics.h"
#include "race/race_session.h"
#include "story/map_events.h"
#include "system/controller_io.h"
#include "system/task_scheduler.h"
#include "text/font_render.h"

extern void func_8001CD90_1D990(void);
extern void initSaveSlotStatSprites(void *);
extern void initSaveSlotNameText(void *);
extern void initSaveSlotDeleteArrow(void *);
extern void initSaveSlotGoldDisplay(void *);
extern void initSaveSlotConfirmationIndicator(void *);
extern void initSaveSlotDeleteText(void *);

u8 eeprom_save_magic[16] = "SNOW2EEP";

void initSaveSlotSelection(void);
void onSaveSlotSelectionConfirm(void);
void onSaveSlotSelectionCancel(void);

typedef struct {
    u8 pad[0xD0];
    u8 slotIndex;
} SaveSlotStatSpritesReturnType;

typedef struct {
    u8 pad[0x1C];
    u8 slotIndex;
} SaveSlotNameTextReturnType;

typedef struct {
    ViewportNode unk0;
    ViewportNode slotModels[4];
    u8 padding0[0x170];
    void *unkAA8;
    void *unkAAC;
    void *unkAB0;
    void *unkAB4;
    void *unkAB8;
    s16 slideOffset;
    s16 unkABE;
    s16 selectionY;
    u16 selectionBaseY;
    u16 selectionAnimState;
    u16 unkAC6;
    u8 saveSlotIndex;
    u8 hasSaveData;
    u8 paddingACA[0x2];
    u8 unkACC;
    u8 eepromErrorStatus;
    u8 unkACE[3];
    u8 unkAD1;
    u8 unkAD2;
    u8 unkAD3;
    u8 unkAD4;
    u8 unkAD5;
    u8 unkAD6;
} SaveSlotScreenState;

typedef struct {
    u8 padding1[0x938];           // 0x000
    EepromSaveData_type slots[4]; // 0x938
    u8 padding2[0x1C];            // 0xAA8
    u16 unkAC4;                   // 0xAC4
    u8 padding3[2];               // 0xAC6
    u8 saveSlotIndex;             // 0xAC8
    u8 padding4[3];               // 0xAC9
    u8 unkACC;                    // 0xACC
    u8 eepromErrorStatus;         // 0xACD
    u8 slotFlags[8];              // 0xACE
} SaveData;

s32 sanitizeSaveSlotData(EepromSaveData_type *saveData);

void initSaveSlotScreen(void) {
    SaveSlotScreenState *state;
    s32 i;
    s32 xOffset;
    s32 xPos;
    SaveSlotStatSpritesReturnType *statTask;
    SaveSlotNameTextReturnType *nameTask;
    void (*handler)(void);
    s32 probeResult;

    state = (SaveSlotScreenState *)allocateTaskMemory(0xAE0);
    setupTaskSchedulerNodes(0x14, 0x14, 0x14, 0, 0, 0, 0, 0);
    clearMemory((s8 *)state, 0xAE0);

    state->unkAC6 = 0x28;
    state->saveSlotIndex = 3;
    state->slideOffset = -0x140;
    state->unkABE = -0x58;
    state->selectionAnimState = 0;
    state->unkACC = 0;
    state->selectionY = -0xE0;
    state->unkAD2 = 0;
    state->unkAD4 = 0;
    state->unkAD3 = 0;
    state->unkAD5 = 0;
    state->unkAD6 = 0;

    state->unkAA8 = loadCompressedData(&_459310_ROM_START, &font_main_ROM_START, 0x2278);
    state->unkAB4 = loadTextRenderAsset(1);
    state->unkAB8 = loadCompressedData(&_3F6670_ROM_START, &font_race_timer_ROM_START, 0x388);
    state->unkAAC = loadCompressedData(&_41A1D0_ROM_START, &_41AD80_ROM_START, 0x1B48);
    state->unkAB0 = loadCompressedData(&_4547D0_ROM_START, &_458E30_ROM_START, 0x9488);

    initMenuCameraNode((ViewportNode *)state, 8, 0xA, 1);

    i = 0;
    xOffset = (s32)0xFFD00000;
    do {
        initViewportNode(&state->slotModels[i], NULL, (u16)(i + 9), 5, 0);
        setViewportId(&state->slotModels[i], (u16)(i + 0xA));

        if (i & 1) {
            xPos = -state->slideOffset;
        } else {
            xPos = (u16)state->slideOffset;
        }

        setModelCameraTransform(&state->slotModels[i], xPos, (s16)(xOffset >> 16), -0xA0, -0x18, 0xA0, 0x18);
        setViewportFadeValue(&state->slotModels[i], 0, 0);
        setViewportFadeValue(NULL, 0, 0);

        xOffset += 0x380000;
        state->unkACE[i] = 0;
        i++;
    } while (i < 3);

    i = 0;
    do {
        eepromProbeAsync();
        do {
            probeResult = (s32)pollEepromProbeAsync();
        } while (probeResult == -1);

        if (probeResult == 1) {
            state->eepromErrorStatus = 0;
            goto eeprom_done;
        }
        if (probeResult == 2) {
            state->eepromErrorStatus = 0x63;
            goto eeprom_done;
        }
        i++;
        if (probeResult == 0) {
            state->eepromErrorStatus = 0x63;
            goto eeprom_done;
        }
        state->eepromErrorStatus = state->eepromErrorStatus + 1;
    } while (i < 3);

eeprom_done:
    for (i = 0; i < 3; i++) {
        statTask = (SaveSlotStatSpritesReturnType *)scheduleTask(initSaveSlotStatSprites, 0, 0, 0x5A);
        statTask->slotIndex = i;
        if (state->eepromErrorStatus != 0) {
            nameTask = (SaveSlotNameTextReturnType *)scheduleTask(initSaveSlotNameText, 0, 0, 0x5B);
            nameTask->slotIndex = i;
        }
    }

    if (state->eepromErrorStatus != 0) {
        state->unkAC6 = 0;
    } else {
        scheduleTask(initSaveSlotDeleteArrow, 0, 0, 0x5B);
    }

    i = 0;
    state->hasSaveData = 0;
    {
        u8 *saveDataPtr = (u8 *)EepromSaveData;

        do {
            if (*saveDataPtr != 0) {
                state->hasSaveData = 1;
                goto after_loop;
            }
            i++;
            saveDataPtr++;
        } while (i < 8);
    }
after_loop:

    scheduleTask(initSaveSlotGoldDisplay, 0, 0, 0x5B);
    scheduleTask(initSaveSlotConfirmationIndicator, 0, 0, 0x5B);
    scheduleTask(initSaveSlotDeleteText, 0, 0, 0x5B);

    if (state->hasSaveData == 1) {
        statTask = (SaveSlotStatSpritesReturnType *)scheduleTask(initSaveSlotStatSprites, 0, 0, 0x5A);
        statTask->slotIndex = 3;
        state->unkAD1 = 0;
        initViewportNode(&state->slotModels[3], NULL, 0xC, 6, 0);
        setViewportId(&state->slotModels[3], 0xD);
        setModelCameraTransform(&state->slotModels[3], -0x140, -0x30, -0xA0, -0x18, 0xA0, 0x18);
        *(s32 *)EepromSaveData->unknown_0C = D_800AFE8C_A71FC->gold;
        handler = initSaveSlotSelection;
    } else {
        handler = func_8001CD90_1D990;
    }

    setGameStateHandler(handler);
    getFreeNodeCount(0);
}

void initSaveSlotSelection(void) {
    SaveSlotScreenState *allocation = (SaveSlotScreenState *)getCurrentAllocation();
    u8 saveSlot = D_800AFE8C_A71FC->previousSaveSlot;
    allocation->saveSlotIndex = saveSlot;
    setGameStateHandler(func_8001CD90_1D990);
}

INCLUDE_ASM("asm/nonmatchings/ui/save_slot_select", func_8001CD90_1D990);

void cleanupSaveSlotSelectionAndExit(void) {
    SaveSlotScreenState *allocation;
    s32 i;

    allocation = (SaveSlotScreenState *)getCurrentAllocation();

    if (getViewportFadeMode(0) != 0) {
        return;
    }

    unlinkNode(&allocation->unk0);

    allocation->unkAA8 = freeNodeMemory(allocation->unkAA8);
    allocation->unkAAC = freeNodeMemory(allocation->unkAAC);
    allocation->unkAB0 = freeNodeMemory(allocation->unkAB0);
    allocation->unkAB4 = freeNodeMemory(allocation->unkAB4);
    allocation->unkAB8 = freeNodeMemory(allocation->unkAB8);

    if (allocation->hasSaveData == 0) {
        for (i = 0; i < 3; i++) {
            unlinkNode(&allocation->slotModels[i]);
        }

        for (i = 0; i < 8; i++) {
            EepromSaveData->header_data[i] = eeprom_save_magic[i];
        }
    } else {
        for (i = 0; i < 4; i++) {
            unlinkNode(&allocation->slotModels[i]);
        }
    }

    if (allocation->selectionAnimState == 1) {
        terminateSchedulerWithCallback(onSaveSlotSelectionConfirm);
    } else {
        terminateSchedulerWithCallback(onSaveSlotSelectionCancel);
    }
}

void onSaveSlotSelectionConfirm(void) {
    returnToParentScheduler(1);
}

void onSaveSlotSelectionCancel(void) {
    returnToParentScheduler(0xFE);
}

void updateSelectionWiggle(void) {
    SaveSlotScreenState *allocation = (SaveSlotScreenState *)getCurrentAllocation();
    u16 counter = allocation->selectionAnimState;
    u16 temp;

    if (counter < 15) {
        if (counter & 1) {
            allocation->selectionY = allocation->selectionBaseY - 2;
        } else {
            allocation->selectionY = allocation->selectionBaseY + 2;
        }
        counter = allocation->selectionAnimState;
    }

    temp = ((counter + 1) & 0xFFFF) % 25U;
    allocation->selectionAnimState = temp;
    temp &= 0xFFFF;

    if (temp == 15) {
        allocation->selectionY = allocation->selectionBaseY;
    }
}

void onSaveSlotReadComplete(u16 arg0, s32 arg1) {
    SaveData *allocation;
    s32 i;
    s32 sum;
    s32 savedChecksum;
    u16 slotIndex;
    u8 temp;
    s32 funcResult;
    u8 *dataPtr;
    s32 limit;

    allocation = (SaveData *)getCurrentAllocation();

    if (arg1 == 0) {
        i = 0;
        slotIndex = arg0 & 0xFFFF;

        while (i < 8) {
            if (allocation->slots[slotIndex].header_data[i] != eeprom_save_magic[i]) {
                allocation->eepromErrorStatus++;
                break;
            }
            i++;
        }

        if (i == 8) {
            limit = 0x58;
            slotIndex = arg0 & 0xFFFF;
            dataPtr = allocation->slots[slotIndex].header_data;
            savedChecksum = allocation->slots[slotIndex].checksum;
            sum = 0;
            i = 0;
            allocation->slots[slotIndex].checksum = 0;

            while (i < limit) {
                sum += *dataPtr;
                i++;
                dataPtr++;
            }

            slotIndex = arg0 & 0xFFFF;
            if (sum != savedChecksum) {
                allocation->eepromErrorStatus++;

            } else {

                funcResult = sanitizeSaveSlotData(&allocation->slots[slotIndex]);
                allocation->eepromErrorStatus = 0;
                if (!(funcResult & 0xFF)) {
                    allocation->unkACC++;
                    allocation->slotFlags[slotIndex] = 1;

                    if (allocation->saveSlotIndex >= 3) {
                        allocation->saveSlotIndex = arg0;
                    }
                } else {
                    allocation->slotFlags[slotIndex] = 0;
                }
            }
        }
    } else {
        allocation->eepromErrorStatus++;
    }

    temp = allocation->eepromErrorStatus;
    if (temp != 0) {
        if (temp < 3) {
            allocation->unkAC4 = allocation->unkAC4 - 1;
            return;
        }
        allocation->eepromErrorStatus = 0;
        allocation->slotFlags[arg0 & 0xFFFF] = 0;
    }
}

s32 sanitizeSaveSlotData(EepromSaveData_type *saveData) {
    s32 coinValue;
    s32 wasModified;
    s32 i;
    u8 value;

    coinValue = *(s32 *)saveData->unknown_0C;
    wasModified = 0;
    if (coinValue > 0x98967F) {
        *(s32 *)saveData->unknown_0C = 0x98967F;
        wasModified = 1;
        i = 0;
    } else {
        i = 0;
        if (coinValue < 0) {
            *(s32 *)saveData->unknown_0C = 0;
            wasModified = 1;
            i = 0;
        }
    }

    for (i = 0; i < 0x10; i++) {
        value = saveData->save_slot_status[i];
        if (value >= 6) {
            saveData->save_slot_status[i] = 0;
            wasModified = 1;
        }
        value = saveData->save_slot_data[i];
        if (value >= 6) {
            saveData->save_slot_data[i] = 0;
            wasModified = 1;
        }
    }

    if (saveData->save_slot_status[0] == 0) {
        saveData->save_slot_status[0] = 5;
        wasModified = 1;
    }

    for (i = 0; i < 3; i++) {
        value = saveData->save_slot_data[i];
        if (value == 0) {
            saveData->save_slot_data[i] = 5;
            wasModified = 1;
        }
        value = saveData->save_slot_data[i + 4];
        if (value == 0) {
            saveData->save_slot_data[i + 4] = 5;
            wasModified = 1;
        }
    }

    for (i = 0; i < 0x12; i++) {
        value = saveData->character_or_settings[i];
        if (value >= 0x1A) {
            saveData->character_or_settings[i] = 0;
            wasModified = 1;
        }
    }

    for (i = 0; i < 3; i++) {
        value = saveData->character_or_settings[i * 3];
        if (value == 0) {
            saveData->character_or_settings[i * 3] = i + 1;
            wasModified = 1;
        }
    }

    for (i = 0; i < 0x12; i++) {
        value = saveData->character_or_settings[i];
        if (value >= 0x1A) {
            saveData->character_or_settings[i] = 1;
            wasModified = 1;
        }
    }

    for (i = 0; i < 9; i++) {
        value = saveData->u.setting_42[i];
        if (value >= 0x12) {
            saveData->u.setting_42[i] = 0x11;
            wasModified = 1;
        }
    }

    for (i = 0; i < 3; i++) {
        value = saveData->setting_4B[i];
        if (value >= 0x10) {
            saveData->setting_4B[i] = 0xF;
            wasModified = 1;
        }
    }

    if (saveData->setting_4E >= 2) {
        saveData->setting_4E = 0;
        wasModified = 1;
    }

    if (saveData->setting_4F >= 2) {
        saveData->setting_4F = 0;
        wasModified = 1;
    }

    if (saveData->setting_50 >= 2) {
        saveData->setting_50 = 0;
        wasModified = 1;
    }

    if (saveData->unk51 >= 2) {
        saveData->unk51 = 0;
        wasModified = 1;
    }

    return wasModified;
}

void verifySaveSlotChecksum(void) {
    u8 saveBuffer[0x60];
    SaveSlotScreenState *allocation;
    void *result;
    s32 retryCount;
    u32 computedChecksum;
    u32 expectedChecksum;
    u8 *dataPtr;
    s32 dataSize;

    allocation = (SaveSlotScreenState *)getCurrentAllocation();
    allocation->selectionAnimState = 0;

    computedChecksum = 0;
    retryCount = 0;

    while (retryCount < 3) {
        eepromReadAsync(allocation->saveSlotIndex, saveBuffer);

        do {
            result = pollEepromReadAsync();
        } while (result == (void *)-1);

        if (result == 0) {
            break;
        }

        retryCount++;
    }

    if (retryCount < 3) {
        dataSize = 0x58;
        expectedChecksum = *(u32 *)&saveBuffer[8];
        dataPtr = saveBuffer;
        retryCount = 0;
        *(u32 *)&saveBuffer[8] = 0;

        while (retryCount < dataSize) {
            retryCount++;
            computedChecksum += *dataPtr;
            dataPtr++;
        }

        if (computedChecksum == expectedChecksum) {
            allocation->selectionAnimState = 0;
            return;
        }
    }

    allocation->selectionAnimState = 1;
}

void updateSlotSelectionSlide(void) {
    SaveSlotScreenState *allocation;
    s16 slideOffset;
    u8 slotIndex;

    allocation = (SaveSlotScreenState *)getCurrentAllocation();
    slideOffset = allocation->slideOffset;

    if (slideOffset != 0) {
        slotIndex = allocation->saveSlotIndex;
        slideOffset -= 3;
        allocation->slideOffset = slideOffset;

        setModelCameraTransform(
            &allocation->slotModels[3],
            0,
            (s16)((slotIndex * 7 * 8) - 48),
            (s16)-slideOffset,
            -0x18,
            allocation->slideOffset,
            0x18
        );
    }
}
