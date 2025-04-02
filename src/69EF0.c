#include "69EF0.h"

#include "gamestate.h"

typedef struct {
    char padding[0x20];
    s32 unk20;
    s32 unk24;
} D_8009A864_9B464_type;

typedef struct {
    char padding[0x10];
    void* unk10;
    char padding1[0x14];
    void* GameState;  // 0x28
    char padding3[0x2];
    s32 unk30;
    char padding4[0x4];
    s16 unk38[9];
    s16 unk4A;
} D_8009A860_9B460_type;

extern D_8009A860_9B460_type* D_8009A860_9B460;
extern D_8009A864_9B464_type* D_8009A864_9B464;

extern void func_8006A3FC_6AFFC();

INCLUDE_ASM("asm/nonmatchings/69EF0", func_800692F0_69EF0);

INCLUDE_ASM("asm/nonmatchings/69EF0", func_800693C4_69FC4);

INCLUDE_ASM("asm/nonmatchings/69EF0", func_80069470_6A070);

INCLUDE_ASM("asm/nonmatchings/69EF0", func_80069530_6A130);

INCLUDE_ASM("asm/nonmatchings/69EF0", func_800697CC_6A3CC);

INCLUDE_ASM("asm/nonmatchings/69EF0", func_800697F4_6A3F4);

s16 func_80069810_6A410(void) {
    s16 temp_v0;

    temp_v0 = D_8009A860_9B460->unk4A;
    D_8009A860_9B460->unk4A = 0;

    return temp_v0;
}

void func_8006982C_6A42C(void* arg0) {
    D_8009A860_9B460->unk10 = arg0;
}

INCLUDE_ASM("asm/nonmatchings/69EF0", func_8006983C_6A43C);

INCLUDE_ASM("asm/nonmatchings/69EF0", func_80069854_6A454);

GameState* GameStateGet() {
    return D_8009A860_9B460->GameState;
}

INCLUDE_ASM("asm/nonmatchings/69EF0", func_800698CC_6A4CC);

INCLUDE_ASM("asm/nonmatchings/69EF0", func_800698DC_6A4DC);

INCLUDE_ASM("asm/nonmatchings/69EF0", func_800698EC_6A4EC);

INCLUDE_ASM("asm/nonmatchings/69EF0", func_800699F4_6A5F4);

s16 func_80069AEC_6A6EC(s32 arg0) {
    return D_8009A860_9B460->unk38[arg0];
}

INCLUDE_ASM("asm/nonmatchings/69EF0", func_80069B04_6A704);

void func_80069CC0_6A8C0(s32 arg0) {
    D_8009A864_9B464->unk20 = arg0;
}

INCLUDE_ASM("asm/nonmatchings/69EF0", func_80069CD0_6A8D0);

void func_80069CE8_6A8E8(s32 arg0) {
    D_8009A864_9B464->unk24 = arg0;
}

INCLUDE_ASM("asm/nonmatchings/69EF0", func_80069CF8_6A8F8);

s32 func_80069D20_6A920(void) {
    return D_8009A860_9B460->unk30 != 0;
}

INCLUDE_ASM("asm/nonmatchings/69EF0", func_80069D34_6A934);

INCLUDE_ASM("asm/nonmatchings/69EF0", func_80069D7C_6A97C);

INCLUDE_ASM("asm/nonmatchings/69EF0", func_80069DD4_6A9D4);

INCLUDE_ASM("asm/nonmatchings/69EF0", func_80069E3C_6AA3C);

INCLUDE_ASM("asm/nonmatchings/69EF0", func_80069F94_6AB94);

INCLUDE_ASM("asm/nonmatchings/69EF0", func_8006A0EC_6ACEC);

INCLUDE_ASM("asm/nonmatchings/69EF0", func_8006A1C0_6ADC0);

void func_8006A200_6AE00(void) {
    func_8006A3FC_6AFFC();
}
