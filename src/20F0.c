#include "common.h"

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    u8 padding[0x3C];
} D_80089A6C_8A66C_type;
extern D_80089A6C_8A66C_type D_80089A6C_8A66C[];

extern OSThread* __osActiveQueue_0;
s32 func_800018F4_24F4(void*);
s32 func_80001904_2504(s16);
typedef struct
{
    u8 padding[0xC];
    s16 unkC;
} func_80002B8C_378C_arg;

void func_800635CC_641CC(u16);

typedef struct {
    u8 padding[0x10];
    struct {
        u8 padding[0x16];
        u16 unk16;
    }* unk10;
    u8 padding2[0x2B];
    s8 unk3F;
    u8 padding3[0x48];
    s8 unk88;
} func_80002B50_3750_arg;

s32 func_800018F4_24F4(void*);
s32 func_80001904_2504(s16);

typedef struct {
    u8 padding[0xC];
    s16 unkC;
} func_80002BD0_37D0_arg;

typedef struct {
    u8 padding[0x3C];
    s8 unk3C;
} func_800014F0_20F0_arg;

void func_800014F0_20F0(func_800014F0_20F0_arg* arg0) {
    arg0->unk3C = 1;
}

typedef struct {
    u8 padding[0x158];
    s32 unk158;
} func_800014FC_20FC_arg;

void func_800014FC_20FC(func_800014FC_20FC_arg* arg0, s32 arg1) {
    arg0->unk158 = arg1;
}

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

typedef struct {
    s8 unk0;
    u8 padding[0x4B];
} D_80089A8C_8A68C_type;
extern D_80089A8C_8A68C_type D_80089A8C_8A68C[0];

s8 func_800015A0_21A0(s16 arg0) {
    return D_80089A8C_8A68C[arg0].unk0;
}

typedef struct {
    u8 padding[0x3D];
    s8 unk3D;
} func_800015CC_21CC_arg;

void func_800015CC_21CC(func_800015CC_21CC_arg* arg0, s8 arg1) {
    arg0->unk3D = arg1;
}

s8 func_800015D4_21D4(func_800015CC_21CC_arg* arg0) {
    return arg0->unk3D;
}

s32 func_800015DC_21DC(void) {
    return 0x98;
}

s32 func_800015E4_21E4(void) {
    return 0x9B;
}

s32 func_800015EC_21EC(void) {
    return 6;
}

s32 func_800015F4_21F4(void) {
    return 6;
}

typedef struct {
    u8 padding[0x88];
    s8 unk88;
} func_800015FC_21FC_arg;

void func_800015FC_21FC(func_800015FC_21FC_arg* arg0, s8 arg1) {
    arg0->unk88 = arg1;
}

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

typedef struct {
    u8 padding[0x110];
    s32 unk110;
} func_800016E0_22E0_arg;

void func_800016E0_22E0(func_800016E0_22E0_arg* arg0, s32 arg1) {
    arg0->unk110 = (s32)(arg0->unk110 | (1 << arg1));
}

void func_800016F8_22F8(func_800016E0_22E0_arg* arg0, s32 arg1) {
    arg0->unk110 = (s32)(arg0->unk110 & ~(1 << arg1));
}

INCLUDE_ASM("asm/nonmatchings/20F0", func_80001714_2314);

s32 func_800017F4_23F4(func_80002B50_3750_arg* arg0) {
    s32 new_var;
    int new_var2 = 0;
    s32 var_v0 = 0;

    if ((arg0 != 0) && ((new_var = arg0->unk88) != new_var2)) {
        var_v0 = arg0->unk3F != 0;
    }

    new_var = var_v0;
    return new_var;
}

INCLUDE_ASM("asm/nonmatchings/20F0", func_80001818_2418);

void func_80069E3C_6AA3C(s32, s32);

typedef struct {
    u8 padding[0xC];
    s16 unkC;
} func_80001868_2468_arg;

void func_80001868_2468(func_80001868_2468_arg* arg0) {
    D_80089A6C_8A66C_type* temp_v1 = &D_80089A6C_8A66C[arg0->unkC];
    func_80069E3C_6AA3C(temp_v1->unk8, temp_v1->unkC);
}

INCLUDE_ASM("asm/nonmatchings/20F0", func_800018AC_24AC);

INCLUDE_ASM("asm/nonmatchings/20F0", func_800018F4_24F4);

typedef struct {
    s32 unk0;
    u8 padding[0x48];
} D_80089A74_8A674_type;
extern D_80089A74_8A674_type D_80089A74_8A674[];

s32 func_80001904_2504(s16 arg0) {
    return D_80089A74_8A674[arg0].unk0 == 0;
}

extern D_80089A6C_8A66C_type D_8008BD2C_8C92C;

D_80089A6C_8A66C_type* func_80001934_2534(s32 arg0) {
    if (arg0 != -1) {
        return &D_80089A6C_8A66C[arg0];
    }

    return &D_8008BD2C_8C92C;
}

typedef struct {
    u8 padding[0x14];
    s16 unk14;
    s16 unk16;
    u8 padding2[0x20];
    s16 unk38;
    s16 unk3A;
    u8 padding5[0x52];
    s16 unk8E;
} func_80001970_2570_arg;

void func_80001970_2570(func_80001970_2570_arg* arg0) {
    arg0->unk14 = -1;
    arg0->unk16 = -1;
    arg0->unk3A = -1;
    arg0->unk38 = -1;
    arg0->unk8E = -1;
}

INCLUDE_ASM("asm/nonmatchings/20F0", func_8000198C_258C);

INCLUDE_ASM("asm/nonmatchings/20F0", func_800019B8_25B8);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80001A6C_266C);

s32 func_80002040_2C40();
s32 func_8006A200_6AE00(s32);

void func_80002014_2C14(s32 arg0) {
    func_80002040_2C40();
    func_8006A200_6AE00(arg0);
}

INCLUDE_ASM("asm/nonmatchings/20F0", func_80002040_2C40);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80002170_2D70);

INCLUDE_ASM("asm/nonmatchings/20F0", func_800021B8_2DB8);

INCLUDE_ASM("asm/nonmatchings/20F0", func_800021E8_2DE8);

void func_80002260_2E60(s32, s16, s16, s8, s32, s32);

void func_80002220_2E20(s32 arg0, s16 arg1, s16 arg2, s8 arg3, s16 arg4) {
    func_80002260_2E60(arg0, arg1, arg2, arg3, (s32)arg4, 1);
}

INCLUDE_ASM("asm/nonmatchings/20F0", func_80002260_2E60);

void func_80002484_3084(s32, s32);

void func_80002468_3068(s32 arg0) {
    func_80002484_3084(arg0, -1);
}

INCLUDE_ASM("asm/nonmatchings/20F0", func_80002484_3084);

INCLUDE_ASM("asm/nonmatchings/20F0", func_80002750_3350);

void func_80002B50_3750(func_80002B50_3750_arg* arg0) {
    if ((arg0 != NULL) && (arg0->unk88 != 0) && (arg0->unk3F != 0)) {
        func_800635CC_641CC(arg0->unk10->unk16);
    }
}

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

s32 func_80002BD0_37D0(func_80002BD0_37D0_arg* arg0) {
    u32 new_var = arg0->unkC;
    if (func_80001904_2504(new_var) == 0) {
        return 8;
    }

    if (func_800018F4_24F4(arg0) != 0) {
        new_var = 8;
        return new_var;
    }

    return 0x90;
}

INCLUDE_ASM("asm/nonmatchings/20F0", func_80002C14_3814);
