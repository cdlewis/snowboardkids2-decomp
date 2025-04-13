#include "6E840.h"
#include "common.h"
#include "gamestate.h"
#include "overlay.h"

USE_OVERLAY(_9FDF0)

void func_8006982C_6A42C(void*);
s32* func_80069854_6A454(s32);
s32 func_80069F94_6AB94(void*, void*, s32);
extern s32 D_215D70;
extern s32 D_216290;
extern void func_80068060_68C60();
extern void func_80066474_67074();
void func_800697CC_6A3CC(void*);
s32 func_80069810_6A410();
extern void func_800680C4_68CC4();
extern void func_80065DD8_669D8();
void func_800680F0_68CF0(ALPlayer*);

typedef struct {
    u8 padding[0x4];
    s32 unk4;
} func_80063824_64424_arg_unk20;

typedef struct {
    u8 padding[0x20];
    func_80063824_64424_arg_unk20* unk20;
    u8 padding2[0x17];
    u8 unk3B;
} func_80063824_64424_arg;

void func_8006318C_63D8C(void*);
s32 func_80070140_70D40(void*);

typedef struct {
    u8 padding[0xC];
    s32 unkC;
} func_8006411C_64D1C_arg_unk20;

typedef struct {
    u8 padding[0x14];
    s32 unk14;
    u8 padding2[0x8];
    func_8006411C_64D1C_arg_unk20* unk20;
} func_8006411C_64D1C_arg;

void func_80063A94_64694(void*);
extern s32* gRegionAllocPtr;

INCLUDE_ASM("asm/nonmatchings/615A0", func_800609A0_615A0);

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

INCLUDE_ASM("asm/nonmatchings/615A0", func_80062C98_63898);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80062CD0_638D0);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80062CF0_638F0);

INCLUDE_ASM("asm/nonmatchings/615A0", func_8006300C_63C0C);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80063058_63C58);

INCLUDE_ASM("asm/nonmatchings/615A0", func_800630A4_63CA4);

INCLUDE_ASM("asm/nonmatchings/615A0", func_800630F0_63CF0);

INCLUDE_ASM("asm/nonmatchings/615A0", func_8006318C_63D8C);

INCLUDE_ASM("asm/nonmatchings/615A0", func_800634E8_640E8);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80063534_64134);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80063580_64180);

INCLUDE_ASM("asm/nonmatchings/615A0", func_800635CC_641CC);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80063668_64268);

INCLUDE_ASM("asm/nonmatchings/615A0", func_800636C8_642C8);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80063728_64328);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80063788_64388);

void buildDisplayListSegment(func_80063824_64424_arg* arg0) {
    s32* temp_a0;
    s32 v1;

    if (func_80070140_70D40((void*)((s32)arg0) + 0x14) == 0) {
        func_8006318C_63D8C(arg0);
        temp_a0 = gRegionAllocPtr;
        gRegionAllocPtr = temp_a0 + 2;

        // G_NOOP
        temp_a0[0x0] = 0xE7000000;
        gRegionAllocPtr = temp_a0 + 0x4;
        temp_a0[0x1] = 0;

        // G_SETOTHERMODE_H
        temp_a0[0x2] = 0xFB000000;

        v1 = arg0->unk3B;

        // G_DL
        temp_a0[0x4] = 0xDE000000;
        v1 = v1 | (~0xFF);
        temp_a0[0x3] = v1;
        gRegionAllocPtr = temp_a0 + 0x6;
        temp_a0[0x5] = (s32)arg0->unk20->unk4;
    }
}

INCLUDE_ASM("asm/nonmatchings/615A0", func_800638C0_644C0);

INCLUDE_ASM("asm/nonmatchings/615A0", func_8006395C_6455C);

INCLUDE_ASM("asm/nonmatchings/615A0", func_800639F8_645F8);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80063A94_64694);

INCLUDE_ASM("asm/nonmatchings/615A0", func_8006405C_64C5C);

INCLUDE_ASM("asm/nonmatchings/615A0", func_800640BC_64CBC);

void func_8006411C_64D1C(func_8006411C_64D1C_arg* arg0) {
    s32* temp_v1;

    if (func_80070140_70D40((void*)(s32)arg0 + 0x14) == 0) {
        func_80063A94_64694(arg0);
        temp_v1 = gRegionAllocPtr;
        temp_v1[0] = 0xDE000000;
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

INCLUDE_ASM("asm/nonmatchings/615A0", func_80064E34_65A34);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80064F74_65B74);

INCLUDE_ASM("asm/nonmatchings/615A0", func_800650B4_65CB4);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80065150_65D50);

INCLUDE_ASM("asm/nonmatchings/615A0", func_800653E0_65FE0);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80065670_66270);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80065900_66500);

INCLUDE_ASM("asm/nonmatchings/615A0", func_800659E4_665E4);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80065DA8_669A8);

INCLUDE_ASM("asm/nonmatchings/615A0", func_80065DD8_669D8);

void func_80066444_67044(s32 arg0, debugEnqueueCallback_arg3* arg1) {
    arg1->unk1C = 0;
    debugEnqueueCallback(arg0 & 0xFFFF, 4, &func_80065DD8_669D8, arg1);
}

INCLUDE_ASM("asm/nonmatchings/615A0", func_80066474_67074);

void func_80066AC0_676C0(s32 arg0, debugEnqueueCallback_arg3* arg1) {
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

INCLUDE_ASM("asm/nonmatchings/615A0", func_80067F5C_68B5C);

void func_80067FB0_68BB0(void) {
    s32* temp_s0;

    temp_s0 = func_80069854_6A454(4);
    LOAD_OVERLAY(_9FDF0)
    *temp_s0 = func_80069F94_6AB94(&D_215D70, &D_216290, 0x918);
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
