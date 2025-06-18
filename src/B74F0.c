#include "common.h"

INCLUDE_ASM("asm/nonmatchings/B74F0", func_800BB2B0_B74F0);

INCLUDE_ASM("asm/nonmatchings/B74F0", func_800BB3B8_B75F8);

INCLUDE_ASM("asm/nonmatchings/B74F0", func_800BB468_B76A8);

INCLUDE_ASM("asm/nonmatchings/B74F0", func_800BB658_B7898);

INCLUDE_ASM("asm/nonmatchings/B74F0", func_800BB71C_B795C);

INCLUDE_ASM("asm/nonmatchings/B74F0", func_800BB7D0_B7A10);

INCLUDE_ASM("asm/nonmatchings/B74F0", func_800BB808_B7A48);

INCLUDE_ASM("asm/nonmatchings/B74F0", func_800BB8B8_B7AF8);

typedef struct {
    u8 padding[0x24];
    s32 unk24;
    s32 unk28;
} func_800BBA60_B7CA0_arg;
void func_800BBA60_B7CA0(func_800BBA60_B7CA0_arg* arg0) {
    arg0->unk24 = freeGameStateMemory(arg0->unk24);
    arg0->unk28 = freeGameStateMemory(arg0->unk28);
}

INCLUDE_ASM("asm/nonmatchings/B74F0", func_800BBA98);
