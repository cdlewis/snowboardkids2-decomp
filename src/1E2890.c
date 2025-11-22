#include "common.h"

extern u8 D_800BAF08_1E7FB8;
extern void setCleanupCallback(void *);
extern void setCallback(void *);
extern void func_800B58D0_1E2980(void);

void func_800B5984_1E2A34(void);

void func_800B57E0_1E2890(void) {
    D_800BAF08_1E7FB8 = 1;
}

INCLUDE_ASM("asm/nonmatchings/1E2890", func_800B57F0_1E28A0);

void func_800B5898_1E2948(void) {
    setCleanupCallback(func_800B5984_1E2A34);
    D_800BAF08_1E7FB8 = 0;
    setCallback(func_800B58D0_1E2980);
}

INCLUDE_ASM("asm/nonmatchings/1E2890", func_800B58D0_1E2980);

void func_800B5984_1E2A34(void) {
}

INCLUDE_ASM("asm/nonmatchings/1E2890", func_800B598C_1E2A3C);

INCLUDE_ASM("asm/nonmatchings/1E2890", func_800B5A34_1E2AE4);

INCLUDE_ASM("asm/nonmatchings/1E2890", func_800B5A6C_1E2B1C);

void func_800B5B20_1E2BD0(void) {
}
