#include "69EF0.h"

#include "gamestate.h"

s32 func_8006A258_6AE58(s32, s32, void*);

typedef struct {
    u8 padding[0xE];
    u8 unk0E;
    u8 padding2[0x2];
    u8 unk11;
    u8 padding3[0x4];
    u32 unk18;
    void* unk1C;
    s32 unk20;
    s32 unk24;
} D_8009A864_9B464_type;
extern D_8009A864_9B464_type* D_8009A864_9B464;

typedef struct {
    char padding[0x10];
    void* unk10;
    s32 unk14;
    u8 unk18;
    char padding5[0x4];
    u32 unk20;
    void* unk24;
    GameState* GameState;  // 0x28
    char padding3[0x2];
    s32 unk30;
    char padding4[0x4];
    s16 unk38[9];
    s16 unk4A;
} D_8009A860_9B460_type;
extern D_8009A860_9B460_type* D_8009A860_9B460;

extern void* func_8003B8F0_3C4F0(void);
extern s32 func_8006A51C_6B11C(void* ptr);
extern s32 func_8006A52C_6B12C(void* ptr);
extern void func_8006A3FC_6AFFC();

INCLUDE_ASM("asm/nonmatchings/69EF0", func_800692F0_69EF0);

INCLUDE_ASM("asm/nonmatchings/69EF0", func_800693C4_69FC4);

INCLUDE_ASM("asm/nonmatchings/69EF0", func_80069470_6A070);

INCLUDE_ASM("asm/nonmatchings/69EF0", func_80069530_6A130);

void func_800697CC_6A3CC(s32 arg0) {
    if (D_8009A860_9B460 != NULL) {
        D_8009A860_9B460->unk18 = 2;
        D_8009A860_9B460->unk14 = arg0;
    }
}

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

void* func_80069E3C_6AA3C(void) {
    void* s0;

    if (D_8009A864_9B464 == 0) {
        s0 = func_8003B8F0_3C4F0();
        if (func_8006A51C_6B11C(s0) != 0) {
            if (D_8009A860_9B460->unk18 != 3) {
                D_8009A860_9B460->unk20 = func_8006A52C_6B12C(s0);
                D_8009A860_9B460->unk24 = s0;
                D_8009A860_9B460->unk18 = 3;
            } else if (D_8009A860_9B460->unk20 < func_8006A52C_6B12C(s0)) {
                D_8009A860_9B460->unk20 = func_8006A52C_6B12C(s0);
                D_8009A860_9B460->unk24 = s0;
            }
        }
    } else {
        s0 = func_8003B8F0_3C4F0();
        if (func_8006A51C_6B11C(s0) != 0) {
            if ((((u8)(D_8009A864_9B464->unk0E - 3)) >= 2)) {
                D_8009A864_9B464->unk18 = func_8006A52C_6B12C(s0);
                D_8009A864_9B464->unk1C = s0;
                D_8009A864_9B464->unk0E = 3;
            } else {
                if (D_8009A864_9B464->unk18 < func_8006A52C_6B12C(s0)) {
                    D_8009A864_9B464->unk18 = func_8006A52C_6B12C(s0);
                    D_8009A864_9B464->unk1C = s0;
                }
            }
        }
        D_8009A864_9B464->unk11 = 1;
    }

    return s0;
}

INCLUDE_ASM("asm/nonmatchings/69EF0", func_80069F94_6AB94);

INCLUDE_ASM("asm/nonmatchings/69EF0", func_8006A0EC_6ACEC);

s32 func_8006A1C0_6ADC0(s32 arg0) {
    s32 sp10;
    s32 temp_a0;

    temp_a0 = func_8006A258_6AE58(0, arg0, &sp10);
    if (D_8009A864_9B464 != NULL) {
        D_8009A864_9B464->unk11 = 1;
    }
    return temp_a0;
}

void func_8006A200_6AE00(void) {
    func_8006A3FC_6AFFC();
}
