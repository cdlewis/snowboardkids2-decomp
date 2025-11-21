#include "90F0.h"

#include "common.h"
#include "task_scheduler.h"

s32 func_800084F0_90F0(void) {
    return 10;
}

INCLUDE_ASM("asm/nonmatchings/90F0", func_800084F8_90F8);

INCLUDE_ASM("asm/nonmatchings/90F0", func_80008514_9114);

INCLUDE_ASM("asm/nonmatchings/90F0", func_80008D18_9918);

INCLUDE_ASM("asm/nonmatchings/90F0", func_80009548_A148);

INCLUDE_ASM("asm/nonmatchings/90F0", func_80009D58_A958);

INCLUDE_ASM("asm/nonmatchings/90F0", func_80009D94_A994);

INCLUDE_ASM("asm/nonmatchings/90F0", func_80009DD0_A9D0);

INCLUDE_ASM("asm/nonmatchings/90F0", func_80009E0C_AA0C);

INCLUDE_ASM("asm/nonmatchings/90F0", func_80009E1C_AA1C);

s32 func_80009E2C_AA2C(void *arg0) {
    return ((u8*)arg0)[6] & 1;
}

INCLUDE_ASM("asm/nonmatchings/90F0", func_80009E38_AA38);

INCLUDE_ASM("asm/nonmatchings/90F0", func_80009E48_AA48);

s32 func_80009E58_AA58(void *arg0) {
    u8 val;
    val = ((u8*)arg0)[6] & 2;
    return val != 0;
}

INCLUDE_ASM("asm/nonmatchings/90F0", func_80009E68_AA68);

INCLUDE_ASM("asm/nonmatchings/90F0", func_80009EF8_AAF8);

void func_80009F5C_AB5C(func_80009F5C_AB5C_arg **arg0) {
    if (arg0 != NULL) {
        *arg0 = (func_80009F5C_AB5C_arg *)freeNodeMemory(*arg0);
    }
}

INCLUDE_ASM("asm/nonmatchings/90F0", func_80009F90_AB90);

INCLUDE_ASM("asm/nonmatchings/90F0", func_8000A030_AC30);

INCLUDE_ASM("asm/nonmatchings/90F0", func_8000A13C_AD3C);

INCLUDE_ASM("asm/nonmatchings/90F0", func_8000A190_AD90);

INCLUDE_ASM("asm/nonmatchings/90F0", func_8000A1E4_ADE4);

INCLUDE_ASM("asm/nonmatchings/90F0", func_8000A410_B010);
