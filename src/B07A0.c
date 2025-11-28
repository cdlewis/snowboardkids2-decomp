#include "common.h"
#include "task_scheduler.h"

typedef void (*FuncPtr)(void *);

typedef struct {
    u8 _pad0[0x450];
    s32 unk450;
    u8 _pad454[0x76A];
    u8 unkBBE;
    u8 unkBBF;
} func_800BC4AC_arg;

extern FuncPtr D_800BCA44_B1F34[];
extern FuncPtr D_800BCA5C_B1F4C[];

extern void func_800B02AC(func_800BC4AC_arg *);
extern s32 func_8005D308_5DF08(func_800BC4AC_arg *, s32);
extern void func_8005D180_5DD80(func_800BC4AC_arg *, s32);

INCLUDE_ASM("asm/nonmatchings/B07A0", func_800BB2B0_B07A0);

INCLUDE_ASM("asm/nonmatchings/B07A0", func_800BB66C_B0B5C);

void func_800BB8E0_B0DD0(func_800BC4AC_arg *arg0) {
    D_800BCA44_B1F34[arg0->unkBBE](arg0);
}

INCLUDE_ASM("asm/nonmatchings/B07A0", func_800BB910_B0E00);

INCLUDE_ASM("asm/nonmatchings/B07A0", func_800BB998_B0E88);

INCLUDE_ASM("asm/nonmatchings/B07A0", func_800BBA54_B0F44);

INCLUDE_ASM("asm/nonmatchings/B07A0", func_800BC008_B14F8);

void func_800BC0A8_B1598(func_800BC4AC_arg *arg0) {
    D_800BCA5C_B1F4C[arg0->unkBBE](arg0);
}

INCLUDE_ASM("asm/nonmatchings/B07A0", func_800BC0D8_B15C8);

INCLUDE_ASM("asm/nonmatchings/B07A0", func_800BC3B8_B18A8);

s32 func_800BC4AC_B199C(func_800BC4AC_arg *arg0) {
    s32 pad[3];

    getCurrentAllocation();

    arg0->unk450 += -0x8000;
    func_800B02AC(arg0);

    if (arg0->unkBBF == 0) {
        if (func_8005D308_5DF08(arg0, 6) != 0) {
            arg0->unkBBF = arg0->unkBBF + 1;
        }
        return 0;
    }

    func_8005D180_5DD80(arg0, 7);

    return 0;
}

INCLUDE_ASM("asm/nonmatchings/B07A0", func_800BC520_B1A10);

INCLUDE_ASM("asm/nonmatchings/B07A0", func_800BC61C_B1B0C);

INCLUDE_ASM("asm/nonmatchings/B07A0", func_800BC7A8_B1C98);

INCLUDE_ASM("asm/nonmatchings/B07A0", func_800BC89C_B1D8C);
