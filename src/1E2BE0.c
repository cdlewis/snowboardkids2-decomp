#include "common.h"
#include "task_scheduler.h"

void func_800B5B30_1E2BE0(void *arg0, u8 arg1) {
    *(u8 *)((u8 *)arg0 + 0x71) = arg1;
}

INCLUDE_ASM("asm/nonmatchings/1E2BE0", func_800B5B38_1E2BE8);

INCLUDE_ASM("asm/nonmatchings/1E2BE0", func_800B5B7C_1E2C2C);

void func_800B5BFC_1E2CAC(void *arg0) {
    freeNodeMemory(arg0);
}

INCLUDE_ASM("asm/nonmatchings/1E2BE0", finalizeAnimationLoop);

INCLUDE_ASM("asm/nonmatchings/1E2BE0", func_800B5CFC_1E2DAC);

INCLUDE_ASM("asm/nonmatchings/1E2BE0", func_800B5E64_1E2F14);

INCLUDE_ASM("asm/nonmatchings/1E2BE0", func_800B5EC4_1E2F74);

void func_800B5F20_1E2FD0(void *arg0, void *arg1) {
    *(s32 *)((u8 *)arg0 + 0x34) = *(s32 *)((u8 *)arg1 + 0x2C);
}

INCLUDE_ASM("asm/nonmatchings/1E2BE0", func_800B5F2C_1E2FDC);

INCLUDE_ASM("asm/nonmatchings/1E2BE0", func_800B5FB0_1E3060);

INCLUDE_ASM("asm/nonmatchings/1E2BE0", func_800B6034_1E30E4);

INCLUDE_ASM("asm/nonmatchings/1E2BE0", func_800B6088_1E3138);

INCLUDE_ASM("asm/nonmatchings/1E2BE0", func_800B60DC_1E318C);

INCLUDE_ASM("asm/nonmatchings/1E2BE0", func_800B6130_1E31E0);

typedef struct {
    u8 _pad[0x68];
    s32 unk68;
    s16 unk6C;
    u8 _pad2[2];
    u8 unk70;
} func_800B6180_1E3230_arg0;

void func_800B6180_1E3230(func_800B6180_1E3230_arg0 *arg0, s32 arg1, s16 arg2) {
    arg0->unk70 = 0;
    arg0->unk68 = arg1;
    arg0->unk6C = arg2;
}

INCLUDE_ASM("asm/nonmatchings/1E2BE0", func_800B6190_1E3240);

INCLUDE_ASM("asm/nonmatchings/1E2BE0", func_800B6544_1E35F4);

INCLUDE_ASM("asm/nonmatchings/1E2BE0", advanceSceneManager);
