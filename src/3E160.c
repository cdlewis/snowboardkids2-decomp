#include "6E840.h"
#include "common.h"
#include "gamestate.h"

void func_80057564_58164(s32);
void func_8006982C_6A42C(void*);
extern s32 D_800AFF10_A7280;
extern void func_80040608_41208();
GameState* GameStateGet();
s32 func_80055D34_56934(u8);
void func_8006983C_6A43C(void*);
GameStateUnk28* func_80069F94_6AB94(void*, void*, s32);
extern s32 D_3FF010;
extern s32 D_3FF910;
extern void func_8003EDF8_3F9F8();
void func_800697F4_6A3F4(u8);
extern u8 D_800A24A0_A30A0;
void func_800574A0_580A0(s32);
extern void func_800404A8_410A8();
void func_800609A0_615A0(s32);
void func_8003EE50_3FA50();
extern void func_80057564_58164(s32);
extern void func_80040608_41208(void);

INCLUDE_RODATA("asm/nonmatchings/3E160", jtbl_8009E4C8_9F0C8);

INCLUDE_RODATA("asm/nonmatchings/3E160", jtbl_8009E5E8_9F1E8);

INCLUDE_ASM("asm/nonmatchings/3E160", func_8003D560_3E160);

INCLUDE_ASM("asm/nonmatchings/3E160", func_8003E4F0_3F0F0);

void func_8003EDA0_3F9A0(void) {
    GameState* temp_v0;

    temp_v0 = GameStateGet();
    temp_v0->unk30 = func_80055D34_56934(temp_v0->unk5C);
    temp_v0->unk28 = func_80069F94_6AB94(&D_3FF010, &D_3FF910, 0x16E0);
    func_8006983C_6A43C(&func_8003EDF8_3F9F8);
}

void func_8003EDF8_3F9F8() {
    GameState* s0 = GameStateGet();

    func_800609A0_615A0((s32)s0 + 0x30);

    s0->unk44 = (u8*)s0->unk28 + s0->unk28->unk0;
    s0->unk48 = (u8*)s0->unk28 + s0->unk28->unk4;

    func_8006983C_6A43C(&func_8003EE50_3FA50);
}

INCLUDE_ASM("asm/nonmatchings/3E160", func_8003EE50_3FA50);

INCLUDE_ASM("asm/nonmatchings/3E160", func_8003EEEC_3FAEC);

INCLUDE_ASM("asm/nonmatchings/3E160", func_8003EFDC_3FBDC);

INCLUDE_ASM("asm/nonmatchings/3E160", func_8003F0AC_3FCAC);

INCLUDE_ASM("asm/nonmatchings/3E160", func_8003F178_3FD78);

INCLUDE_ASM("asm/nonmatchings/3E160", func_8003F1F0_3FDF0);

INCLUDE_ASM("asm/nonmatchings/3E160", func_8003F368_3FF68);

INCLUDE_ASM("asm/nonmatchings/3E160", func_8003FA78_40678);

INCLUDE_ASM("asm/nonmatchings/3E160", func_8003FB00_40700);

void func_8003FB90_40790(void) {
    s32 temp_v1;
    GameState* temp_v0;

    temp_v0 = GameStateGet();
    temp_v1 = temp_v0->unk4C - 1;
    temp_v0->unk4C = temp_v1;
    if (temp_v1 == 0) {
        func_8006FDA0_709A0(NULL, 0xFF, 0x10);
        func_80057564_58164(0x3C);
        func_8006982C_6A42C(&func_80040608_41208);
    }
}

INCLUDE_ASM("asm/nonmatchings/3E160", func_8003FBE0_407E0);

INCLUDE_ASM("asm/nonmatchings/3E160", func_8003FCD8_408D8);

void func_8003FD3C_4093C(void) {
    if (D_800AFF10_A7280 & 0x8000) {
        func_8006FDA0_709A0(NULL, 0xFF, 0x10);
        func_80057564_58164(0x3C);
        func_8006982C_6A42C(&func_80040608_41208);
    }
}

INCLUDE_ASM("asm/nonmatchings/3E160", func_8003FD84_40984);

INCLUDE_ASM("asm/nonmatchings/3E160", func_8003FDD4_409D4);

INCLUDE_ASM("asm/nonmatchings/3E160", func_8003FF14_40B14);

INCLUDE_ASM("asm/nonmatchings/3E160", func_8003FF78_40B78);

void func_8003FFC0_40BC0() {
    s32 temp_v1;
    GameState* temp_v0;

    temp_v0 = GameStateGet();
    temp_v1 = temp_v0->unk4C - 1;
    temp_v0->unk4C = temp_v1;
    if (temp_v1 == 0) {
        func_8006FDA0_709A0(NULL, 0xFF, 0x10);
        func_80057564_58164(0x3C);
        func_8006982C_6A42C(&func_80040608_41208);
    }
}

INCLUDE_ASM("asm/nonmatchings/3E160", func_80040010_40C10);

INCLUDE_ASM("asm/nonmatchings/3E160", func_8004013C_40D3C);

INCLUDE_ASM("asm/nonmatchings/3E160", func_800401A0_40DA0);

void func_800401E8_40DE8(void) {
    GameState* state = GameStateGet();
    s32 temp = state->unk4C - 1;
    state->unk4C = temp;
    if (temp == 0) {
        func_8006FDA0_709A0(0, 0xFF, 0x10);
        func_80057564_58164(0x3C);
        func_8006982C_6A42C(func_80040608_41208);
    }
}

INCLUDE_ASM("asm/nonmatchings/3E160", func_80040238_40E38);

INCLUDE_ASM("asm/nonmatchings/3E160", func_80040304_40F04);

INCLUDE_ASM("asm/nonmatchings/3E160", func_80040420_41020);

void func_80040468_41068(void) {
    s32 temp_v1;
    GameState* temp_v0;

    temp_v0 = GameStateGet();
    temp_v1 = temp_v0->unk4C - 1;
    temp_v0->unk4C = temp_v1;
    if (temp_v1 == 0) {
        func_800574A0_580A0(0xA);
        func_8006982C_6A42C(&func_800404A8_410A8);
    }
}

INCLUDE_ASM("asm/nonmatchings/3E160", func_800404A8_410A8);

INCLUDE_ASM("asm/nonmatchings/3E160", func_80040528_41128);

INCLUDE_ASM("asm/nonmatchings/3E160", func_80040588_41188);

INCLUDE_ASM("asm/nonmatchings/3E160", func_80040608_41208);

void func_800407B4_413B4(void) {
    func_800697F4_6A3F4(D_800A24A0_A30A0);
}
