#include "common.h"
#include "task_scheduler.h"
#include "D_800AFE8C_A71FC_type.h"

INCLUDE_ASM("asm/nonmatchings/1D520", func_8001C920_1D520);

extern void func_8001CD90_1D990(void);

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

INCLUDE_ASM("asm/nonmatchings/1D520", func_8001DEBC_1EABC);

INCLUDE_ASM("asm/nonmatchings/1D520", func_8001DF5C_1EB5C);

INCLUDE_ASM("asm/nonmatchings/1D520", func_8001E104_1ED04);

INCLUDE_ASM("asm/nonmatchings/1D520", func_8001E320_1EF20);

INCLUDE_ASM("asm/nonmatchings/1D520", func_8001E3E8_1EFE8);

INCLUDE_RODATA("asm/nonmatchings/1D520", jtbl_8009E1D8_9EDD8);
