#include "19E80.h"
#include "common.h"

extern s32 D_800A2D60_A3960;
extern s32 D_800A2D64_A3964;

void func_800680F0_68CF0(void* arg0) {
    func_80019280_19E80_return sp10;

    func_80019280_19E80(arg0, 0, &sp10);
    D_800A2D60_A3960 = (s32)sp10.unk0;
    D_800A2D64_A3964 = sp10.unk4;
}

INCLUDE_ASM("asm/nonmatchings/68CF0", func_80068128_68D28);

INCLUDE_ASM("asm/nonmatchings/68CF0", func_800688D4_694D4);

INCLUDE_ASM("asm/nonmatchings/68CF0", func_800689C0_695C0);
