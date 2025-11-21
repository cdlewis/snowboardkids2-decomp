#include "common.h"
#include "task_scheduler.h"
#include "D_800AFE8C_A71FC_type.h"
#include "6E840.h"

INCLUDE_ASM("asm/nonmatchings/1D520", func_8001C920_1D520);

extern void func_8001CD90_1D990(void);
extern void func_8003B1F4_3BDF4(s32 arg0, void *arg1);
extern void *func_8003B28C_3BE8C(void);

void func_8001CD58_1D958(void) {
    void *allocation = getCurrentAllocation();
    u8 value = ((u8 *)D_800AFE8C_A71FC)[0x1E];
    ((u8 *)allocation)[0xAC8] = value;
    setGameStateHandler(func_8001CD90_1D990);
}

INCLUDE_ASM("asm/nonmatchings/1D520", func_8001CD90_1D990);

INCLUDE_ASM("asm/nonmatchings/1D520", func_8001DD54_1E954);

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

INCLUDE_ASM("asm/nonmatchings/1D520", func_8001DF5C_1EB5C);

INCLUDE_ASM("asm/nonmatchings/1D520", func_8001E104_1ED04);

void func_8001E320_1EF20(void) {
    u8 sp10[0x60];
    void *allocation;
    void *result;
    s32 retryCount;
    u32 checksum;
    u32 expectedChecksum;
    u8 *ptr;
    s32 limit;

    allocation = getCurrentAllocation();
    *(u16 *)((u8 *)allocation + 0xAC4) = 0;

    checksum = 0;
    retryCount = 0;

    while (retryCount < 3) {
        func_8003B1F4_3BDF4(*(u8 *)((u8 *)allocation + 0xAC8), sp10);

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
            *(u16 *)((u8 *)allocation + 0xAC4) = 0;
            return;
        }
    }

    *(u16 *)((u8 *)allocation + 0xAC4) = 1;
}

INCLUDE_ASM("asm/nonmatchings/1D520", func_8001E3E8_1EFE8);

INCLUDE_RODATA("asm/nonmatchings/1D520", jtbl_8009E1D8_9EDD8);
