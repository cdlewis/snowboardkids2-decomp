#include "3E160.h"
#include "6E840.h"
#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "common.h"
#include "task_scheduler.h"

extern void func_8001CD90_1D990(void);
extern u8 D_8008D9B0_8E5B0[8];

void onSaveSlotSelectionConfirm(void);
void onSaveSlotSelectionCancel(void);

typedef struct {
    Node_70B00 unk0;
    Node_70B00 slotModels[4];
    u8 padding0[0x170];
    Node_70B00 *unkAA8;
    Node_70B00 *unkAAC;
    Node_70B00 *unkAB0;
    Node_70B00 *unkAB4;
    Node_70B00 *unkAB8;
    s16 slideOffset;
    u16 unkABE;
    u16 selectionY;         // 0xAC0 - animated Y position (wiggles on selection)
    u16 selectionBaseY;     // 0xAC2 - base Y position
    u16 selectionAnimState; // 0xAC4 - animation counter (0-24) / selection result (1=confirmed)
    u8 padding5[0x2];
    u8 saveSlotIndex;
    u8 unkAC9;
    u8 padding3[0x8];
} allocation_1D520;

typedef struct {
    u8 padding1[0x938];           // 0x000
    EepromSaveData_type slots[4]; // 0x938
    u8 padding2[0x1C];            // 0xAA8
    u16 unkAC4;                   // 0xAC4
    u8 padding3[2];               // 0xAC6
    u8 unkAC8;                    // 0xAC8
    u8 padding4[3];               // 0xAC9
    u8 unkACC;                    // 0xACC
    u8 unkACD;                    // 0xACD
    u8 slotFlags[8];              // 0xACE
} SaveData;

s32 sanitizeSaveSlotData(EepromSaveData_type *saveData);

INCLUDE_ASM("asm/nonmatchings/1D520", func_8001C920_1D520);

void initSaveSlotSelection(void) {
    allocation_1D520 *allocation = (allocation_1D520 *)getCurrentAllocation();
    u8 saveSlot = D_800AFE8C_A71FC->previousSaveSlot;
    allocation->saveSlotIndex = saveSlot;
    setGameStateHandler(func_8001CD90_1D990);
}

INCLUDE_ASM("asm/nonmatchings/1D520", func_8001CD90_1D990);

void cleanupSaveSlotSelectionAndExit(void) {
    allocation_1D520 *allocation;
    s32 i;

    allocation = (allocation_1D520 *)getCurrentAllocation();

    if (func_8006FE10_70A10(0) != 0) {
        return;
    }

    unlinkNode(&allocation->unk0);

    allocation->unkAA8 = freeNodeMemory(allocation->unkAA8);
    allocation->unkAAC = freeNodeMemory(allocation->unkAAC);
    allocation->unkAB0 = freeNodeMemory(allocation->unkAB0);
    allocation->unkAB4 = freeNodeMemory(allocation->unkAB4);
    allocation->unkAB8 = freeNodeMemory(allocation->unkAB8);

    if (allocation->unkAC9 == 0) {
        for (i = 0; i < 3; i++) {
            unlinkNode(&allocation->slotModels[i]);
        }

        for (i = 0; i < 8; i++) {
            EepromSaveData->header_data[i] = D_8008D9B0_8E5B0[i];
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
    func_800697F4_6A3F4(1);
}

void onSaveSlotSelectionCancel(void) {
    func_800697F4_6A3F4(0xFE);
}

void updateSelectionWiggle(void) {
    allocation_1D520 *allocation = (allocation_1D520 *)getCurrentAllocation();
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
            if (allocation->slots[slotIndex].header_data[i] != D_8008D9B0_8E5B0[i]) {
                allocation->unkACD++;
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
                allocation->unkACD++;

            } else {

                funcResult = sanitizeSaveSlotData(&allocation->slots[slotIndex]);
                allocation->unkACD = 0;
                if (!(funcResult & 0xFF)) {
                    allocation->unkACC++;
                    allocation->slotFlags[slotIndex] = 1;

                    if (allocation->unkAC8 >= 3) {
                        allocation->unkAC8 = arg0;
                    }
                } else {
                    allocation->slotFlags[slotIndex] = 0;
                }
            }
        }
    } else {
        allocation->unkACD++;
    }

    temp = allocation->unkACD;
    if (temp != 0) {
        if (temp < 3) {
            allocation->unkAC4 = allocation->unkAC4 - 1;
            return;
        }
        allocation->unkACD = 0;
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
    allocation_1D520 *allocation;
    void *result;
    s32 retryCount;
    u32 computedChecksum;
    u32 expectedChecksum;
    u8 *dataPtr;
    s32 dataSize;

    allocation = (allocation_1D520 *)getCurrentAllocation();
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
    allocation_1D520 *allocation;
    s16 slideOffset;
    u8 slotIndex;

    allocation = (allocation_1D520 *)getCurrentAllocation();
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
