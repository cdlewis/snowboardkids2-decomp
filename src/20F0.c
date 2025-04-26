#include "common.h"

extern OSThread* __osActiveQueue_0;
s32 func_800018F4_24F4(void*);
s32 func_80001904_2504(s16);
typedef struct
{
    u8 padding[0xC];
    s16 unkC;
} func_80002B8C_378C_arg;

INCLUDE_ASM("asm/nonmatchings/20F0", func_800014F0_20F0);

INCLUDE_ASM("asm/nonmatchings/20F0", func_800014FC_20FC);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80001504_2104);

INCLUDE_ASM("asm/nonmatchings/20F0", func_8000150C_210C);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80001518_2118);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80001524_2124);

INCLUDE_ASM("asm/nonmatchings/20F0", func_8000152C_212C);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80001534_2134);

INCLUDE_ASM("asm/nonmatchings/20F0", func_8000153C_213C);

OSThread* __osGetActiveQueue(void) {
    return __osActiveQueue_0;
}

INCLUDE_ASM("asm/nonmatchings/20F0", func_8000156C_216C);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80001574_2174);

INCLUDE_ASM("asm/nonmatchings/20F0", func_800015A0_21A0);

INCLUDE_ASM("asm/nonmatchings/20F0", func_800015CC_21CC);

INCLUDE_ASM("asm/nonmatchings/20F0", func_800015D4_21D4);

INCLUDE_ASM("asm/nonmatchings/20F0", func_800015DC_21DC);

INCLUDE_ASM("asm/nonmatchings/20F0", func_800015E4_21E4);

INCLUDE_ASM("asm/nonmatchings/20F0", func_800015EC_21EC);

INCLUDE_ASM("asm/nonmatchings/20F0", func_800015F4_21F4);

INCLUDE_ASM("asm/nonmatchings/20F0", func_800015FC_21FC);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80001604_2204);

INCLUDE_ASM("asm/nonmatchings/20F0", func_8000160C_220C);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80001624_2224);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80001630_2230);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80001638_2238);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80001640_2240);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80001650_2250);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80001660_2260);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80001668_2268);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80001688_2288);

INCLUDE_ASM("asm/nonmatchings/20F0", func_800016E0_22E0);

INCLUDE_ASM("asm/nonmatchings/20F0", func_800016F8_22F8);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80001714_2314);

INCLUDE_ASM("asm/nonmatchings/20F0", func_800017F4_23F4);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80001818_2418);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80001868_2468);

INCLUDE_ASM("asm/nonmatchings/20F0", func_800018AC_24AC);

INCLUDE_ASM("asm/nonmatchings/20F0", func_800018F4_24F4);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80001904_2504);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80001934_2534);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80001970_2570);

INCLUDE_ASM("asm/nonmatchings/20F0", func_8000198C_258C);

INCLUDE_ASM("asm/nonmatchings/20F0", func_800019B8_25B8);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80001A6C_266C);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80002014_2C14);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80002040_2C40);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80002170_2D70);

INCLUDE_ASM("asm/nonmatchings/20F0", func_800021B8_2DB8);

INCLUDE_ASM("asm/nonmatchings/20F0", func_800021E8_2DE8);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80002220_2E20);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80002260_2E60);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80002468_3068);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80002484_3084);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80002750_3350);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80002B50_3750);

s32 func_80002B8C_378C(func_80002B8C_378C_arg* arg0) {
    s32 new_var;
    if (func_80001904_2504(arg0->unkC) == 0) {
        return 0;
    }
    new_var = func_800018F4_24F4(arg0);
    if (new_var != 0) {
        return 0;
    }
    new_var = 0x90;
    return new_var;
}

INCLUDE_ASM("asm/nonmatchings/20F0", func_80002BD0_37D0);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80002C14_3814);
