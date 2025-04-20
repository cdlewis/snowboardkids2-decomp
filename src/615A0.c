#include "6E840.h"
#include "common.h"
#include "gamestate.h"
#include "gbi.h"
#include "overlay.h"

USE_OVERLAY(_9FDF0)

void func_8006982C_6A42C(void*);
s32* func_80069854_6A454(s32);
s32 dmaRequestAndUpdateState(void*, void*, s32);
extern s32 D_215D70;
extern s32 D_216290;
extern void func_80068060_68C60();
extern void func_80066474_67074();
void func_800697CC_6A3CC(void*);
s32 func_80069810_6A410();
extern void func_800680C4_68CC4();
extern void func_80065DD8_669D8();
void func_800680F0_68CF0(ALPlayer*);
void func_8006318C_63D8C(void*);
extern s32* gRegionAllocPtr;
s32 func_80070140_70D40(void*);
extern void func_800659E4_665E4;
void func_80063A94_64694(void*);
void func_800648EC_654EC();
s32 func_80070140_70D40(void*);
extern void func_800638C0_644C0();
extern void func_8006395C_6455C();
extern void func_80065150_65D50();
extern void func_800653E0_65FE0();
extern void func_80065670_66270();

typedef struct {
    u8 padding[0x20];
    s32* unk20;
    u8 padding3[0x9];
    s32 unk30;
    u8 padding2[0x3];
    s8 unk37;
} func_80065900_66500_arg1;

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 padding[0x18];
    struct {
        u8 padding[0xC];
        void* unkC;
    }* unk20;
} func_80063728_64328_arg;

typedef struct {
    u8 padding[0x10];
    s32 unk10;
    s32 unk14;
    s8 unk18;
    u8 unk19;
} func_80067F5C_68B5C_arg;

typedef struct {
    s32 sp10;
    s32 sp14;
    u16 sp18;
    u16 sp1A;
} func_80067F5C_68B5C_sp10;

void func_800192CC_19ECC(s32, s32, s32, func_80067F5C_68B5C_sp10*);

typedef struct {
    u8 padding[0x148];
    u8 unk148;
    u8 unk149;
    u8 unk14A;
    u8 padding2[0xD];
    u8 unk158;
    u8 unk159;
    u8 unk15A;
} D_800AB068_A23D8_arg;

extern D_800AB068_A23D8_arg* D_800AB068_A23D8;

typedef struct {
    u8 padding[0x20];
    struct {
        u8 padding[0x8];
        s32 unk8;
    }* unk20;
} func_80064E34_65A34_arg;

typedef struct {
    u8 padding[0x20];
    struct {
        u8 padding[0x8];
        s32 unk8;
    }* unk20;
} func_800636C8_642C8_arg;

typedef struct {
    u8 padding[0x20];
    struct {
        u8 padding[0x4];
        s32 unk4;
    }* unk20;
    u8 padding2[0x17];
    u8 unk3B;
} func_80063824_64424_arg;

typedef struct {
    u8 padding[0x14];
    s32 unk14;
    u8 padding2[0x8];
    struct {
        u8 padding[0xC];
        s32 unkC;
    }* unk20;
} func_8006411C_64D1C_arg;

typedef struct {
    u8 padding[0x30];
    s32 unk30;
} func_80065DA8_669A8_arg;

typedef struct {
    s16 unk0;
    u8 _pad[0x22];
} func_80062C98_63898_arg;

void func_80062CF0_638F0();

typedef struct {
    u8 padding[0x20];
    struct {
        s32 padding;
        s32 unk4;
        u8 padding2[0x4];
        s32 unkC;
    }* unk20;
} func_800630A4_63CA4_arg;

typedef struct {
    u8 padding[0x1C];
    s32 unk1C;
    u8 padding2[0x10];
    s32 unk30;
} func_80066444_67044_arg1;

void func_800609A0_615A0(u32* arg0) {
    u16 temp;
    u32 addr;
    int new_var;

    addr = arg0[0];
    temp = *((u16*)addr);
    new_var = temp * 6;
    addr += 2;
    arg0[1] = addr;

    addr += new_var;
    temp = *((u16*)addr);
    addr += 2;
    arg0[2] = addr;

    addr += temp * 8;
    temp = *((u16*)addr);
    addr += 2;
    arg0[3] = addr;

    *((u16*)(((u8*)arg0) + 0x10)) = temp;
}

INCLUDE_ASM("asm/nonmatchings/615A0", func_800609E8_615E8);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80060A3C_6163C);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80060CDC_618DC);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80061A64_62664);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80061D6C_6296C);

INCLUDE_ASM("asm/nonmatchings/615A0", func_800620D0_62CD0);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80062254_62E54);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80062274_62E74);

INCLUDE_ASM("asm/nonmatchings/615A0", func_800625A4_631A4);

INCLUDE_ASM("asm/nonmatchings/615A0", func_800626C4_632C4);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80062918_63518);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80062B1C_6371C);

s32 func_80062C98_63898(func_80062C98_63898_arg** arg0, u16 index) {
    s16 temp_v0;
    s16 var_a1 = index;

    temp_v0 = arg0[3][index].unk0;

    if (temp_v0 >= 0) {
        var_a1 = temp_v0;
    }
    return var_a1 & 0xFFFF;
}

INCLUDE_ASM("asm/nonmatchings/615A0", func_80062CD0_638D0);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80062CF0_638F0);

void func_8006300C_63C0C(func_80063824_64424_arg* arg0) {
    s32* temp_v1;

    func_80062CF0_638F0();
    temp_v1 = gRegionAllocPtr;
    temp_v1[0] = 0xDE000000;
    gRegionAllocPtr = temp_v1 + 2;
    temp_v1[1] = (s32)arg0->unk20->unk4;
}

INCLUDE_ASM("asm/nonmatchings/615A0", func_80063058_63C58);

void func_800630A4_63CA4(func_800630A4_63CA4_arg* arg0) {
    s32* temp_v1;

    func_80062CF0_638F0();
    temp_v1 = gRegionAllocPtr;
    temp_v1[0] = 0xDE000000;
    gRegionAllocPtr = temp_v1 + 2;
    temp_v1[1] = (s32)arg0->unk20->unkC;
}

INCLUDE_ASM("asm/nonmatchings/615A0", func_800630F0_63CF0);

INCLUDE_ASM("asm/nonmatchings/615A0", func_8006318C_63D8C);

void func_800634E8_640E8(func_800630A4_63CA4_arg* arg0) {
    s32* temp_v1;

    func_8006318C_63D8C(arg0);
    temp_v1 = gRegionAllocPtr;
    temp_v1[0] = 0xDE000000;
    gRegionAllocPtr = temp_v1 + 2;
    temp_v1[1] = (s32)arg0->unk20->unk4;
}

INCLUDE_ASM("asm/nonmatchings/615A0", func_80063534_64134);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80063580_64180);

INCLUDE_ASM("asm/nonmatchings/615A0", func_800635CC_641CC);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80063668_64268);

void func_800636C8_642C8(func_800636C8_642C8_arg* arg0) {
    s32* temp_v1;

    if (func_80070140_70D40((void*)(s32)arg0 + 0x14) == 0) {
        func_8006318C_63D8C(arg0);
        temp_v1 = gRegionAllocPtr;
        temp_v1[0] = (s32)gsSPDisplayList(0);
        gRegionAllocPtr = temp_v1 + 2;
        temp_v1[1] = (s32)arg0->unk20->unk8;
    }
}

void func_80063728_64328(func_80063728_64328_arg* arg0) {
    s32* temp_v1;

    if (func_80070140_70D40((void*)((s32)arg0 + 0x14)) == NULL) {
        func_8006318C_63D8C(arg0);

        temp_v1 = gRegionAllocPtr;
        temp_v1[0] = (s32)gsSPDisplayList(0);

        gRegionAllocPtr = (void*)temp_v1 + 8;
        temp_v1[1] = (s32)arg0->unk20->unkC;
    }
}

INCLUDE_ASM("asm/nonmatchings/615A0", func_80063788_64388);

void buildDisplayListSegment(func_80063824_64424_arg* arg0) {
    s32* temp_a0;
    s32 v1;

    if (func_80070140_70D40((void*)((s32)arg0) + 0x14) == 0) {
        func_8006318C_63D8C(arg0);
        temp_a0 = gRegionAllocPtr;
        gRegionAllocPtr = temp_a0 + 2;

        temp_a0[0x0] = (s32)gsDPPipeSync();
        gRegionAllocPtr = temp_a0 + 0x4;
        temp_a0[0x1] = (s32)gsDPNoOp();

        temp_a0[0x2] = (s32)gsDPSetEnvColor(0, 0, 0, 0);

        v1 = arg0->unk3B;

        temp_a0[0x4] = (s32)gsSPDisplayList(0x0);
        v1 = v1 | (~0xFF);
        temp_a0[0x3] = v1;
        gRegionAllocPtr = temp_a0 + 0x6;
        temp_a0[0x5] = (s32)arg0->unk20->unk4;
    }
}

INCLUDE_ASM("asm/nonmatchings/615A0", func_800638C0_644C0);

INCLUDE_ASM("asm/nonmatchings/615A0", func_8006395C_6455C);

typedef struct {
    u8 padding[0x20];
    struct {
        u8 : 0x4;
        s32 unk4;
        s32 unk8;
        s32 unkC;
    }* unk20;
    u8 padding2[0xC];
    s32 unk30;
} func_800639F8_645F8_arg;

void func_800639F8_645F8(s32 arg0, func_800639F8_645F8_arg* arg1) {
    arg1->unk30 = 0;
    if (arg1->unk20->unk4 != 0) {
        debugEnqueueCallback(arg0 & 0xFFFF, 1, &buildDisplayListSegment, arg1);
    }
    if (arg1->unk20->unk8 != 0) {
        debugEnqueueCallback(arg0 & 0xFFFF, 3, &func_800638C0_644C0, arg1);
    }
    if (arg1->unk20->unkC != 0) {
        debugEnqueueCallback(arg0 & 0xFFFF, 5, &func_8006395C_6455C, arg1);
    }
}

INCLUDE_ASM("asm/nonmatchings/615A0", func_80063A94_64694);

INCLUDE_ASM("asm/nonmatchings/615A0", func_8006405C_64C5C);

INCLUDE_ASM("asm/nonmatchings/615A0", func_800640BC_64CBC);

void func_8006411C_64D1C(func_8006411C_64D1C_arg* arg0) {
    s32* temp_v1;

    if (func_80070140_70D40((void*)(s32)arg0 + 0x14) == 0) {
        func_80063A94_64694(arg0);
        temp_v1 = gRegionAllocPtr;
        temp_v1[0] = (s32)gsSPDisplayList(0x0);
        gRegionAllocPtr = temp_v1 + 2;
        temp_v1[1] = (s32)arg0->unk20->unkC;
    }
}

INCLUDE_ASM("asm/nonmatchings/615A0", func_8006417C_64D7C);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80064218_64E18);

INCLUDE_ASM("asm/nonmatchings/615A0", func_800643AC_64FAC);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80064628_65228);

INCLUDE_ASM("asm/nonmatchings/615A0", func_800646C8_652C8);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80064768_65368);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80064808_65408);

INCLUDE_ASM("asm/nonmatchings/615A0", func_800648EC_654EC);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80064CF4_658F4);

void func_80064E34_65A34(func_80064E34_65A34_arg* arg0) {
    s32* temp_a1;
    func_800648EC_654EC();

    temp_a1 = gRegionAllocPtr;
    temp_a1[0x0] = (s32)gsSPDisplayList(0x0);

    gRegionAllocPtr = temp_a1 + 0x2;
    temp_a1[0x1] = arg0->unk20->unk8;

    gRegionAllocPtr = temp_a1 + 0x4;
    temp_a1[0x2] = (s32)gsMoveWd(0xA, 0, 0);
    temp_a1[0x3] = D_800AB068_A23D8->unk148 << 0x18 | D_800AB068_A23D8->unk149 << 0x10 | D_800AB068_A23D8->unk14A << 8;

    gRegionAllocPtr = temp_a1 + 0x6;
    temp_a1[0x4] = (s32)gsMoveWd(0xA, 0x4, 0);
    temp_a1[0x5] = D_800AB068_A23D8->unk148 << 0x18 | D_800AB068_A23D8->unk149 << 0x10 | D_800AB068_A23D8->unk14A << 8;

    gRegionAllocPtr = temp_a1 + 0x8;
    temp_a1[0x6] = (s32)gsMoveWd(0xA, 0x18, 0);
    temp_a1[0x7] = D_800AB068_A23D8->unk158 << 0x18 | D_800AB068_A23D8->unk159 << 0x10 | D_800AB068_A23D8->unk15A << 8;

    gRegionAllocPtr = temp_a1 + 0xA;
    temp_a1[0x8] = (s32)gsMoveWd(0xA, 0x1C, 0);
    temp_a1[0x9] = D_800AB068_A23D8->unk158 << 0x18 | D_800AB068_A23D8->unk159 << 0x10 | D_800AB068_A23D8->unk15A << 8;
}

INCLUDE_ASM("asm/nonmatchings/615A0", func_80064F74_65B74);

INCLUDE_ASM("asm/nonmatchings/615A0", func_800650B4_65CB4);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80065150_65D50);

INCLUDE_ASM("asm/nonmatchings/615A0", func_800653E0_65FE0);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80065670_66270);

void func_80065900_66500(s32 arg0, func_80065900_66500_arg1* arg1, s32 arg2) {
    func_80065900_66500_arg1* new_var;
    s32 var_a1;
    s32 var_s0;
    s32* temp_v1;
    func_80065900_66500_arg1* var_a0;
    volatile u8 padding[0x1];
    var_a1 = 0;
    var_s0 = 0;
    arg1->unk37 = arg2;
    if (arg2 > 0) {
        var_a0 = arg1;
        do {
            temp_v1 = var_a0->unk20;
            (new_var = var_a0)->unk30 = 0;
            if (temp_v1[1] != 0) {
                var_s0 |= 1;
            }
            if (temp_v1[2] != 0) {
                var_s0 |= 2;
            }
            if (temp_v1[0x3] != 0) {
                var_s0 |= 4;
            }
            var_a1 += 1;
            var_a0 = (func_80065900_66500_arg1*)(((s32)var_a0) + 0x3C);
        } while (var_a1 < arg2);
    }
    if (var_s0 & 1) {
        debugEnqueueCallback(arg0 & 0xFFFF, 1, &func_80065150_65D50, arg1);
    }
    new_var = arg1;
    if (var_s0 & 2) {
        debugEnqueueCallback((arg0 & 0xFFFF) ^ 0, 3, &func_800653E0_65FE0, new_var);
    }
    if (var_s0 & 4) {
        debugEnqueueCallback(arg0 & 0xFFFF, 5, &func_80065670_66270, arg1);
    }
}

INCLUDE_ASM("asm/nonmatchings/615A0", func_800659E4_665E4);

void func_80065DA8_669A8(s32 arg0, func_80065DA8_669A8_arg* arg1) {
    arg1->unk30 = 0;
    debugEnqueueCallback(arg0 & 0xFFFF, 0, &func_800659E4_665E4, arg1);
}

INCLUDE_ASM("asm/nonmatchings/615A0", func_80065DD8_669D8);

void func_80066444_67044(s32 arg0, func_80066444_67044_arg1* arg1) {
    arg1->unk1C = 0;
    debugEnqueueCallback(arg0 & 0xFFFF, 4, &func_80065DD8_669D8, arg1);
}

INCLUDE_ASM("asm/nonmatchings/615A0", func_80066474_67074);

void func_80066AC0_676C0(s32 arg0, func_80066444_67044_arg1* arg1) {
    arg1->unk30 = 0;
    debugEnqueueCallback(arg0 & 0xFFFF, 4, &func_80066474_67074, arg1);
}

INCLUDE_ASM("asm/nonmatchings/615A0", func_80066AF0_676F0);

INCLUDE_ASM("asm/nonmatchings/615A0", func_800670A4_67CA4);

INCLUDE_ASM("asm/nonmatchings/615A0", func_800670D4_67CD4);

INCLUDE_ASM("asm/nonmatchings/615A0", func_800677C0_683C0);

INCLUDE_ASM("asm/nonmatchings/615A0", func_800677F0_683F0);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80067EDC_68ADC);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80067F0C_68B0C);

void func_80067F5C_68B5C(func_80067F5C_68B5C_arg* arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_80067F5C_68B5C_sp10 sp10;

    func_800192CC_19ECC(arg1, arg2 & 0xFFFF, arg3 & 0xFF, &sp10);
    arg0->unk10 = sp10.sp10;
    arg0->unk14 = sp10.sp14;
    arg0->unk18 = sp10.sp18;
    arg0->unk19 = sp10.sp1A;
}

void func_80067FB0_68BB0(void) {
    s32* temp_s0;

    temp_s0 = func_80069854_6A454(4);
    LOAD_OVERLAY(_9FDF0)
    *temp_s0 = dmaRequestAndUpdateState(&D_215D70, &D_216290, 0x918);
    func_8006982C_6A42C(&func_80068060_68C60);
}

void func_80068060_68C60(void) {
    func_800680F0_68CF0(GameStateGet()->audioPlayer0);
    func_800697CC_6A3CC(&func_800680C4_68CC4);
}

void func_80068090_68C90(void) {
    if ((func_80069810_6A410() << 0x10) != 0) {
        func_800697CC_6A3CC(&func_800680C4_68CC4);
    }
}

INCLUDE_ASM("asm/nonmatchings/615A0", func_800680C4_68CC4);
