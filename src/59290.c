#include "common.h"

extern u8 D_80093BB0_947B0[];
extern u8 D_80093F7C_94B7C[];
extern void *getCurrentAllocation(void);

INCLUDE_ASM("asm/nonmatchings/59290", func_80058690_59290);

s32 func_80058804_59404(s32 arg0, s32 arg1) {
    void *allocation;
    u8 *table;
    s32 value;
    s32 temp;
    s32 temp2;

    allocation = getCurrentAllocation();

    if (*(u8 *)((u8 *)allocation + 0x7A) == 0xB) {
        table = D_80093F7C_94B7C;
        arg0 = 0;
    } else {
        table = D_80093BB0_947B0 + (arg1 * 54);
    }

    value = table[arg0 * 6];

    temp = value * 3;
    temp = temp << 7;
    temp = temp - value;
    temp2 = temp << 3;
    temp2 = temp2 - temp;
    temp = temp2 << 5;
    temp2 = temp2 + temp;
    temp2 = temp2 << 2;
    temp2 = temp2 + value;
    temp2 = temp2 + value;

    return (temp2 / 100) + 0xEB333;
}
