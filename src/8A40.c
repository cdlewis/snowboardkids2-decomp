#include "common.h"

extern void *freeNodeMemory(void *);

typedef struct {
    u8 _pad[0x28];
    void *unk28;
    void *unk2C;
} func_800080B8_8CB8_arg;

INCLUDE_ASM("asm/nonmatchings/8A40", func_80007E40_8A40);

INCLUDE_ASM("asm/nonmatchings/8A40", func_80007EC8_8AC8);

void func_800080B8_8CB8(func_800080B8_8CB8_arg *arg0) {
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}

extern void setCleanupCallback(void *);
extern void setCallback(void *);
void func_800081B0_8DB0(void);
extern void func_8000811C_8D1C(void);

void func_800080F0_8CF0(void) {
    setCleanupCallback(func_800081B0_8DB0);
    setCallback(func_8000811C_8D1C);
}

INCLUDE_ASM("asm/nonmatchings/8A40", func_8000811C_8D1C);

void func_800081B0_8DB0(void) {
}

INCLUDE_RODATA("asm/nonmatchings/8A40", jtbl_8009DD88_9E988);

INCLUDE_RODATA("asm/nonmatchings/8A40", jtbl_8009DDA8_9E9A8);
