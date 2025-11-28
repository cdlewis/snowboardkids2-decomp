#include "common.h"
#include "task_scheduler.h"

typedef struct {
    u8 _pad0[0x24];
    void *unk24;
    void *unk28;
} B4240Struct;

INCLUDE_ASM("asm/nonmatchings/B4240", func_800BB2B0_B4240);

INCLUDE_ASM("asm/nonmatchings/B4240", func_800BB320_B42B0);

INCLUDE_ASM("asm/nonmatchings/B4240", func_800BB454_B43E4);

INCLUDE_ASM("asm/nonmatchings/B4240", func_800BB598_B4528);

INCLUDE_ASM("asm/nonmatchings/B4240", func_800BB63C_B45CC);

INCLUDE_ASM("asm/nonmatchings/B4240", func_800BB89C_B482C);

void func_800BBAFC_B4A8C(B4240Struct *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

INCLUDE_ASM("asm/nonmatchings/B4240", func_800BBB34);
