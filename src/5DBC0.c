#include "common.h"
#include "gamestate.h"

extern s32 func_80061A64_62664(void *, u16, void *);
extern s32 func_80061D6C_6296C(void *, u16, void *, s32);

s32 func_8005CFC0_5DBC0(void *arg0, u16 arg1, void *arg2, s32 arg3) {
    return func_80061A64_62664(arg0, arg1, arg2);
}

s32 func_8005CFDC_5DBDC(void *arg0, u16 arg1, void *arg2, s32 arg3, s32 arg4) {
    return func_80061D6C_6296C(arg0, arg1, arg2, arg4);
}

INCLUDE_ASM("asm/nonmatchings/5DBC0", func_8005CFFC_5DBFC);

extern s32 func_80062274_62E74(void *, u16);

s32 func_8005D020_5DC20(void *arg0, u16 arg1) {
    return func_80062274_62E74(arg0, arg1);
}

INCLUDE_ASM("asm/nonmatchings/5DBC0", func_8005D03C_5DC3C);

INCLUDE_ASM("asm/nonmatchings/5DBC0", func_8005D180_5DD80);

INCLUDE_ASM("asm/nonmatchings/5DBC0", func_8005D308_5DF08);

INCLUDE_ASM("asm/nonmatchings/5DBC0", func_8005D48C_5E08C);

INCLUDE_ASM("asm/nonmatchings/5DBC0", func_8005D61C_5E21C);

void func_8005D804_5E404(Player *arg0, u8 arg1, u8 arg2) {
    arg0->unkBE6 = arg1;
    arg0->unkBE7 = arg2;
}

void func_8005D810_5E410(void) {
}

INCLUDE_ASM("asm/nonmatchings/5DBC0", func_8005D818_5E418);

INCLUDE_ASM("asm/nonmatchings/5DBC0", func_8005D8C8_5E4C8);
