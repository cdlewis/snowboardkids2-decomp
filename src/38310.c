#include "common.h"

extern void *freeNodeMemory(void *);

typedef struct {
    u8 pad[0x8];
    void *unk8;
    u8 pad2[0xC];
    void *unk18;
} func_80037874_38474_arg;

typedef struct {
    u8 pad0[0x4];
    void *unk4;
    u8 pad[0xD0];
    void *unkD8;
} func_80037BC4_387C4_arg;

INCLUDE_ASM("asm/nonmatchings/38310", func_80037710_38310);

INCLUDE_ASM("asm/nonmatchings/38310", func_800377FC_383FC);

void func_80037874_38474(func_80037874_38474_arg *arg0) {
    arg0->unk8 = freeNodeMemory(arg0->unk8);
    arg0->unk18 = freeNodeMemory(arg0->unk18);
}

INCLUDE_ASM("asm/nonmatchings/38310", func_800378AC_384AC);

INCLUDE_ASM("asm/nonmatchings/38310", func_80037A64_38664);

void func_80037BC4_387C4(func_80037BC4_387C4_arg *arg0) {
    arg0->unkD8 = freeNodeMemory(arg0->unkD8);
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/38310", func_80037BFC_387FC);

INCLUDE_ASM("asm/nonmatchings/38310", func_80037D18_38918);

INCLUDE_ASM("asm/nonmatchings/38310", func_80037E40_38A40);

INCLUDE_ASM("asm/nonmatchings/38310", func_80037E78_38A78);

INCLUDE_ASM("asm/nonmatchings/38310", func_80037F14_38B14);

INCLUDE_ASM("asm/nonmatchings/38310", func_80037FB0_38BB0);
