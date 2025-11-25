#include "common.h"
#include "90F0.h"

typedef struct {
    u8 _pad[0x4];
    func_80009F5C_AB5C_arg *unk4;
} func_80006398_6F98_arg;

INCLUDE_ASM("asm/nonmatchings/6DD0", func_800061D0_6DD0);

INCLUDE_ASM("asm/nonmatchings/6DD0", func_8000629C_6E9C);

void func_80006398_6F98(func_80006398_6F98_arg *arg0) {
    func_80009F5C_AB5C(&arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/6DD0", func_800063B4_6FB4);

INCLUDE_ASM("asm/nonmatchings/6DD0", func_80006458_7058);

INCLUDE_ASM("asm/nonmatchings/6DD0", func_80006924_7524);

INCLUDE_ASM("asm/nonmatchings/6DD0", func_80006940_7540);

INCLUDE_ASM("asm/nonmatchings/6DD0", func_800069B4_75B4);

INCLUDE_ASM("asm/nonmatchings/6DD0", func_80006A88_7688);
