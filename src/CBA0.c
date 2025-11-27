#include "6E840.h"
#include "common.h"

INCLUDE_ASM("asm/nonmatchings/CBA0", func_8000BFA0_CBA0);

INCLUDE_ASM("asm/nonmatchings/CBA0", func_8000BFD0_CBD0);

INCLUDE_ASM("asm/nonmatchings/CBA0", func_8000C208_CE08);

INCLUDE_ASM("asm/nonmatchings/CBA0", func_8000C238_CE38);

INCLUDE_ASM("asm/nonmatchings/CBA0", func_8000C268_CE68);

INCLUDE_ASM("asm/nonmatchings/CBA0", func_8000C278_CE78);

INCLUDE_ASM("asm/nonmatchings/CBA0", func_8000C28C_CE8C);

typedef struct {
    u8 pad[0x204];
    s32 unk204;
} Func8000C2ACArg;

void func_8000C2AC_CEAC(Func8000C2ACArg *arg0, s32 arg1) {
    arg0->unk204 = arg1;
}

typedef struct {
    u8 pad[0x218];
    s8 unk218;
} Func8000C2B4Arg;

void func_8000C2B4_CEB4(Func8000C2B4Arg *arg0, s32 arg1) {
    arg0->unk218 = arg1;
}

INCLUDE_ASM("asm/nonmatchings/CBA0", func_8000C2BC_CEBC);

INCLUDE_ASM("asm/nonmatchings/CBA0", func_8000C2CC_CECC);

INCLUDE_ASM("asm/nonmatchings/CBA0", func_8000C334_CF34);

INCLUDE_ASM("asm/nonmatchings/CBA0", func_8000C440_D040);

void n_alSeqpDelete(Node_70B00 *arg0) {
    unlinkNode(arg0);
}

INCLUDE_ASM("asm/nonmatchings/CBA0", func_8000C5AC_D1AC);

INCLUDE_ASM("asm/nonmatchings/CBA0", func_8000C7A4_D3A4);

extern void *freeNodeMemory(void *);

typedef struct {
    u8 pad0[0x28];
    void *unk28;
    void *unk2C;
    void *unk30;
    u8 pad34[0x30];
    void *unk64;
    void *unk68;
    void *unk6C;
} Func8000CA80Arg;

void func_8000CA80_D680(Func8000CA80Arg *arg0) {
    arg0->unk30 = freeNodeMemory(arg0->unk30);
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk6C = freeNodeMemory(arg0->unk6C);
    arg0->unk68 = freeNodeMemory(arg0->unk68);
    arg0->unk64 = freeNodeMemory(arg0->unk64);
}
