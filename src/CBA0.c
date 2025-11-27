#include "6E840.h"
#include "common.h"

INCLUDE_ASM("asm/nonmatchings/CBA0", func_8000BFA0_CBA0);

INCLUDE_ASM("asm/nonmatchings/CBA0", func_8000BFD0_CBD0);

extern void func_8000BFD0_CBD0(void *);

typedef struct {
    Node_70B00 *unk0;
} Func8000C208Arg;

void func_8000C208_CE08(Func8000C208Arg *arg0) {
    debugEnqueueCallback(arg0->unk0->slot_index, 7, func_8000BFD0_CBD0, arg0);
}

extern void func_8000BFA0_CBA0(void *);

void func_8000C238_CE38(Func8000C208Arg *arg0) {
    debugEnqueueCallback(arg0->unk0->slot_index, 0, func_8000BFA0_CBA0, arg0);
}

typedef struct {
    u8 pad[0x210];
    s32 unk210;
} Func8000C268Arg;

void func_8000C268_CE68(Func8000C268Arg *arg0) {
    arg0->unk210 |= 1;
}

void func_8000C278_CE78(Func8000C268Arg *arg0) {
    arg0->unk210 &= ~1;
}

typedef struct {
    u8 pad[0x1E4];
    u8 unk1E4[0xC];
} Func8000C28CArg;

void func_8000C28C_CE8C(Func8000C28CArg *arg0, void *arg1) {
    memcpy(arg0->unk1E4, arg1, 0xC);
}

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

typedef struct {
    u8 pad[0x214];
    u8 unk214;
    u8 unk215;
    u8 unk216;
} Func8000C2BCArg;

void func_8000C2BC_CEBC(Func8000C2BCArg *arg0, u8 arg1, u8 arg2, u8 arg3) {
    arg0->unk214 = arg1;
    arg0->unk215 = arg2;
    arg0->unk216 = arg3;
}

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
