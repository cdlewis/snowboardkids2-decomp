#include "3E160.h"
#include "6E840.h"
#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "common.h"
#include "task_scheduler.h"

extern void func_8001CD90_1D990(void);
extern u8 D_8008D9B0_8E5B0[8];

void func_8001DE84_1EA84(void);
void func_8001DEA0_1EAA0(void);

typedef struct {
    Node_70B00 unk0;
    Node_70B00 unk1D8[4];
    u8 padding0[0x170];
    Node_70B00 *unkAA8;
    Node_70B00 *unkAAC;
    Node_70B00 *unkAB0;
    Node_70B00 *unkAB4;
    Node_70B00 *unkAB8;
    s16 unkABC;
    u8 padding2[0x6];
    u16 unkAC4;
    u8 padding5[0x2];
    u8 unkAC8;
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

extern s32 func_8001E104_1ED04(EepromSaveData_type *arg0);

INCLUDE_ASM("asm/nonmatchings/1D520", func_8001C920_1D520);

void func_8001CD58_1D958(void) {
    allocation_1D520 *allocation = (allocation_1D520 *)getCurrentAllocation();
    u8 saveSlot = D_800AFE8C_A71FC->unk9[0x15];
    allocation->unkAC8 = saveSlot;
    setGameStateHandler(func_8001CD90_1D990);
}

INCLUDE_ASM("asm/nonmatchings/1D520", func_8001CD90_1D990);

void func_8001DD54_1E954(void) {
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
            unlinkNode(&allocation->unk1D8[i]);
        }

        for (i = 0; i < 8; i++) {
            EepromSaveData->header_data[i] = D_8008D9B0_8E5B0[i];
        }
    } else {
        for (i = 0; i < 4; i++) {
            unlinkNode(&allocation->unk1D8[i]);
        }
    }

    if (allocation->unkAC4 == 1) {
        terminateSchedulerWithCallback(func_8001DE84_1EA84);
    } else {
        terminateSchedulerWithCallback(func_8001DEA0_1EAA0);
    }
}

void func_8001DE84_1EA84(void) {
    func_800697F4_6A3F4(1);
}

void func_8001DEA0_1EAA0(void) {
    func_800697F4_6A3F4(0xFE);
}

void func_8001DEBC_1EABC(void) {
    u16 *allocation = (u16 *)getCurrentAllocation();
    u16 counter = allocation[0xAC4 / 2];
    u16 temp;

    if (counter < 15) {
        if (counter & 1) {
            allocation[0xAC0 / 2] = allocation[0xAC2 / 2] - 2;
        } else {
            allocation[0xAC0 / 2] = allocation[0xAC2 / 2] + 2;
        }
        counter = allocation[0xAC4 / 2];
    }

    temp = ((counter + 1) & 0xFFFF) % 25U;
    allocation[0xAC4 / 2] = temp;
    temp &= 0xFFFF;

    if (temp == 15) {
        allocation[0xAC0 / 2] = allocation[0xAC2 / 2];
    }
}

void func_8001DF5C_1EB5C(u16 arg0, s32 arg1) {
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

                funcResult = func_8001E104_1ED04(&allocation->slots[slotIndex]);
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

s32 func_8001E104_1ED04(EepromSaveData_type *arg0) {
    s32 temp_a1;
    s32 changed;
    s32 i;
    u8 temp;

    temp_a1 = *(s32 *)arg0->unknown_0C;
    changed = 0;
    if (temp_a1 > 0x98967F) {
        *(s32 *)arg0->unknown_0C = 0x98967F;
        changed = 1;
        i = 0;
    } else {
        i = 0;
        if (temp_a1 < 0) {
            *(s32 *)arg0->unknown_0C = 0;
            changed = 1;
            i = 0;
        }
    }

    for (i = 0; i < 0x10; i++) {
        temp = arg0->save_slot_status[i];
        if (temp >= 6) {
            arg0->save_slot_status[i] = 0;
            changed = 1;
        }
        temp = arg0->save_slot_data[i];
        if (temp >= 6) {
            arg0->save_slot_data[i] = 0;
            changed = 1;
        }
    }

    if (arg0->save_slot_status[0] == 0) {
        arg0->save_slot_status[0] = 5;
        changed = 1;
    }

    for (i = 0; i < 3; i++) {
        temp = arg0->save_slot_data[i];
        if (temp == 0) {
            arg0->save_slot_data[i] = 5;
            changed = 1;
        }
        temp = arg0->save_slot_data[i + 4];
        if (temp == 0) {
            arg0->save_slot_data[i + 4] = 5;
            changed = 1;
        }
    }

    for (i = 0; i < 0x12; i++) {
        temp = arg0->character_or_settings[i];
        if (temp >= 0x1A) {
            arg0->character_or_settings[i] = 0;
            changed = 1;
        }
    }

    for (i = 0; i < 3; i++) {
        temp = arg0->character_or_settings[i * 3];
        if (temp == 0) {
            arg0->character_or_settings[i * 3] = i + 1;
            changed = 1;
        }
    }

    for (i = 0; i < 0x12; i++) {
        temp = arg0->character_or_settings[i];
        if (temp >= 0x1A) {
            arg0->character_or_settings[i] = 1;
            changed = 1;
        }
    }

    for (i = 0; i < 9; i++) {
        temp = arg0->u.setting_42[i];
        if (temp >= 0x12) {
            arg0->u.setting_42[i] = 0x11;
            changed = 1;
        }
    }

    for (i = 0; i < 3; i++) {
        temp = arg0->setting_4B[i];
        if (temp >= 0x10) {
            arg0->setting_4B[i] = 0xF;
            changed = 1;
        }
    }

    if (arg0->setting_4E >= 2) {
        arg0->setting_4E = 0;
        changed = 1;
    }

    if (arg0->setting_4F >= 2) {
        arg0->setting_4F = 0;
        changed = 1;
    }

    if (arg0->setting_50 >= 2) {
        arg0->setting_50 = 0;
        changed = 1;
    }

    if (arg0->unk51 >= 2) {
        arg0->unk51 = 0;
        changed = 1;
    }

    return changed;
}

void func_8001E320_1EF20(void) {
    u8 sp10[0x60];
    allocation_1D520 *allocation;
    void *result;
    s32 retryCount;
    u32 checksum;
    u32 expectedChecksum;
    u8 *ptr;
    s32 limit;

    allocation = (allocation_1D520 *)getCurrentAllocation();
    allocation->unkAC4 = 0;

    checksum = 0;
    retryCount = 0;

    while (retryCount < 3) {
        func_8003B1F4_3BDF4(allocation->unkAC8, sp10);

        do {
            result = func_8003B28C_3BE8C();
        } while (result == (void *)-1);

        if (result == 0) {
            break;
        }

        retryCount++;
    }

    if (retryCount < 3) {
        limit = 0x58;
        expectedChecksum = *(u32 *)&sp10[8];
        ptr = sp10;
        retryCount = 0;
        *(u32 *)&sp10[8] = 0;

        while (retryCount < limit) {
            retryCount++;
            checksum += *ptr;
            ptr++;
        }

        if (checksum == expectedChecksum) {
            allocation->unkAC4 = 0;
            return;
        }
    }

    allocation->unkAC4 = 1;
}

void func_8001E3E8_1EFE8(void) {
    allocation_1D520 *allocation;
    s16 counter;
    u8 playerIndex;

    allocation = (allocation_1D520 *)getCurrentAllocation();
    counter = allocation->unkABC;

    if (counter != 0) {
        playerIndex = allocation->unkAC8;
        counter -= 3;
        allocation->unkABC = counter;

        setModelCameraTransform(
            &allocation->unk1D8[3],
            0,
            (s16)((playerIndex * 7 * 8) - 48),
            (s16)-counter,
            -0x18,
            allocation->unkABC,
            0x18
        );
    }
}
