#include "common.h"

extern void *freeNodeMemory(void *);

typedef struct {
    u8 pad[0x24];
    void *unk24;
    void *unk28;
} func_800BB9F0_AB8A0_arg;

INCLUDE_ASM("asm/nonmatchings/AB160", func_800BB2B0);

INCLUDE_ASM("asm/nonmatchings/AB160", func_800BB320_AB1D0);

INCLUDE_ASM("asm/nonmatchings/AB160", func_800BB454_AB304);

INCLUDE_ASM("asm/nonmatchings/AB160", func_800BB5BC_AB46C);

INCLUDE_ASM("asm/nonmatchings/AB160", func_800BB7D4_AB684);

void func_800BB9F0_AB8A0(func_800BB9F0_AB8A0_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

INCLUDE_ASM("asm/nonmatchings/AB160", func_800BBA28_AB8D8);

INCLUDE_ASM("asm/nonmatchings/AB160", func_800BBA54_AB904);

INCLUDE_ASM("asm/nonmatchings/AB160", func_800BBAF8_AB9A8);

INCLUDE_ASM("asm/nonmatchings/AB160", func_800BBB1C_AB9CC);
