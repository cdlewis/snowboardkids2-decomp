#include "common.h"
#include "task_scheduler.h"

typedef struct {
    u8 _pad[0x20];
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s16 unk58;
    s16 unk5A;
    s16 unk5C;
    s16 unk5E;
    s16 unk60;
    s16 unk62;
    s32 unk64;
    s32 unk68;
    s16 unk6C;
    s16 unk6E;
    u8 unk70;
    u8 unk71;
} func_800B5E64_1E2F14_arg0;

void func_800B5B30_1E2BE0(func_800B5E64_1E2F14_arg0 *arg0, u8 arg1) {
    arg0->unk71 = arg1;
}

void func_800B5B7C_1E2C2C(func_800B5E64_1E2F14_arg0 *, u16);

void *func_800B5B38_1E2BE8(u16 arg0) {
    func_800B5E64_1E2F14_arg0 *temp = allocateNodeMemory(0x74);
    func_800B5B7C_1E2C2C(temp, arg0);
    return temp;
}

void func_800B5B7C_1E2C2C(func_800B5E64_1E2F14_arg0 *arg0, u16 arg1) {
    arg0->unk20 = 0;
    arg0->unk22 = 0;
    arg0->unk24 = 0;
    arg0->unk26 = 0;
    arg0->unk28 = 0;
    arg0->unk2A = 0;
    arg0->unk2C = 0;
    arg0->unk2E = 0;
    arg0->unk30 = 0;
    arg0->unk32 = 0;
    arg0->unk34 = 0;
    arg0->unk38 = 0;
    arg0->unk3C = 0;
    arg0->unk40 = 0;
    arg0->unk44 = 0;
    arg0->unk48 = 0;
    arg0->unk4C = 0;
    arg0->unk50 = 0;
    arg0->unk54 = 0;
    arg0->unk58 = 0;
    arg0->unk5A = 0;
    arg0->unk5C = 0;
    arg0->unk5E = 0;
    arg0->unk60 = 0;
    arg0->unk62 = 0;
    arg0->unk64 = 0;
    arg0->unk68 = 0;
    arg0->unk6C = 0;
    arg0->unk6E = arg1;
    arg0->unk70 = 0;
    arg0->unk71 = 0;
}

void func_800B5BFC_1E2CAC(void *arg0) {
    freeNodeMemory(arg0);
}

INCLUDE_ASM("asm/nonmatchings/1E2BE0", finalizeAnimationLoop);

INCLUDE_ASM("asm/nonmatchings/1E2BE0", func_800B5CFC_1E2DAC);

void func_800B5E64_1E2F14(func_800B5E64_1E2F14_arg0 *arg0, s16 arg1, s16 arg2, s32 arg3, s32 arg4) {
    arg0->unk20 = arg1;
    arg0->unk22 = arg2;
    arg0->unk24 = arg1;
    arg0->unk26 = arg2;
    arg0->unk28 = 0;
    arg0->unk2A = 0;
    arg0->unk2C = 0;
    arg0->unk2E = 0;
    arg0->unk30 = 0;
    arg0->unk32 = 0;
    arg0->unk38 = arg3;
    arg0->unk44 = arg3;
    arg0->unk4C = 0;
    arg0->unk50 = 0;
    arg0->unk54 = 0;
    arg0->unk5A = 0;
    arg0->unk5C = 0;
    arg0->unk60 = 0;
    arg0->unk62 = 0;
    arg0->unk70 = 0;
    arg0->unk3C = arg4;
    arg0->unk48 = arg4;
}

void func_800B5EC4_1E2F74(func_800B5E64_1E2F14_arg0 *arg0, s16 arg1, s16 arg2, s32 arg3, s32 arg4, s32 arg5) {
    func_800B5E64_1E2F14(arg0, arg1, arg2, arg3, arg4);
    arg0->unk34 = arg5;
    arg0->unk40 = arg5;
    arg0->unk4C = 0;
    arg0->unk58 = 0;
    arg0->unk5E = 0;
}

void func_800B5F20_1E2FD0(void *arg0, void *arg1) {
    *(s32 *)((u8 *)arg0 + 0x34) = *(s32 *)((u8 *)arg1 + 0x2C);
}

INCLUDE_ASM("asm/nonmatchings/1E2BE0", func_800B5F2C_1E2FDC);

INCLUDE_ASM("asm/nonmatchings/1E2BE0", func_800B5FB0_1E3060);

void func_800B6034_1E30E4(func_800B5E64_1E2F14_arg0 *arg0, s32 arg1, s16 arg2) {
    s32 quotient;

    quotient = (arg1 - arg0->unk34) / arg2;

    arg0->unk70 = 0;
    arg0->unk40 = arg1;
    arg0->unk58 = arg2;
    arg0->unk5E = arg2;
    arg0->unk4C = quotient;
}

void func_800B6088_1E3138(func_800B5E64_1E2F14_arg0 *arg0, s32 arg1, s16 arg2) {
    s32 quotient;

    quotient = (arg1 - arg0->unk38) / arg2;

    arg0->unk70 = 0;
    arg0->unk44 = arg1;
    arg0->unk5A = arg2;
    arg0->unk60 = arg2;
    arg0->unk50 = quotient;
}

void func_800B60DC_1E318C(func_800B5E64_1E2F14_arg0 *arg0, s32 arg1, s16 arg2) {
    s32 quotient;

    quotient = (arg1 - arg0->unk3C) / arg2;

    arg0->unk70 = 0;
    arg0->unk48 = arg1;
    arg0->unk5C = arg2;
    arg0->unk62 = arg2;
    arg0->unk54 = quotient;
}

void func_800B6130_1E31E0(func_800B5E64_1E2F14_arg0 *arg0, s16 arg1, s16 arg2) {
    if ((arg1 == 0) || (arg2 == 0)) {
        arg0->unk70 = 0;
        arg0->unk2A = 0;
        arg0->unk2E = 0;
        arg0->unk32 = 0;
    } else {
        arg0->unk70 = 1;
        arg0->unk2A = arg1;
        arg0->unk2E = arg2;
        arg0->unk32 = arg2;
    }
}

typedef struct {
    u8 _pad[0x68];
    s32 unk68;
    s16 unk6C;
    s16 unk6E;
    u8 unk70;
    u8 unk71;
} func_800B6180_1E3230_arg0;

void func_800B6180_1E3230(func_800B6180_1E3230_arg0 *arg0, s32 arg1, s16 arg2) {
    arg0->unk70 = 0;
    arg0->unk68 = arg1;
    arg0->unk6C = arg2;
}

INCLUDE_ASM("asm/nonmatchings/1E2BE0", func_800B6190_1E3240);

INCLUDE_ASM("asm/nonmatchings/1E2BE0", func_800B6544_1E35F4);

INCLUDE_ASM("asm/nonmatchings/1E2BE0", advanceSceneManager);
