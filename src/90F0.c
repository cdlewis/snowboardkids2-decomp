#include "90F0.h"

#include "common.h"
#include "task_scheduler.h"

extern s16 D_8008C930_8D530[][10];

s32 func_800084F0_90F0(void) {
    return 10;
}

s16 func_800084F8_90F8(s32 arg0) {
    return D_8008C930_8D530[arg0][0];
}

INCLUDE_ASM("asm/nonmatchings/90F0", func_80008514_9114);

INCLUDE_ASM("asm/nonmatchings/90F0", func_80008D18_9918);

INCLUDE_ASM("asm/nonmatchings/90F0", func_80009548_A148);

INCLUDE_ASM("asm/nonmatchings/90F0", func_80009D58_A958);

INCLUDE_ASM("asm/nonmatchings/90F0", func_80009D94_A994);

INCLUDE_ASM("asm/nonmatchings/90F0", func_80009DD0_A9D0);

void func_80009E0C_AA0C(void *arg0) {
    ((u8*)arg0)[6] |= 1;
}

void func_80009E1C_AA1C(void *arg0) {
    ((u8*)arg0)[6] &= 0xFE;
}

s32 func_80009E2C_AA2C(void *arg0) {
    return ((u8*)arg0)[6] & 1;
}

void func_80009E38_AA38(void *arg0) {
    ((u8*)arg0)[6] |= 2;
}

void func_80009E48_AA48(void *arg0) {
    ((u8*)arg0)[6] &= 0xFD;
}

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

u16 func_8000A410_B010(func_8000A410_B010_arg *arg0) {
    OutputStruct_19E80 output;
    getTableEntryByU16Index(arg0->table, arg0->index, &output);
    return output.field1;
}
