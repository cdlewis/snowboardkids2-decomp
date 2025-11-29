#include "5E590.h"
#include "common.h"
#include "task_scheduler.h"

typedef void (*FuncPtr)(void *);

typedef struct {
    void *ptr;     // offset 0 within element
    u8 _pad[0x38]; // 0x3C - 4 = 0x38
} Element0x3C;     // Size: 0x3C

typedef struct {
    u8 _pad0[0x58];
    Element0x3C elements[12]; // Array at offset 0x58, total size 12 * 0x3C = 0x2D0
    // Next offset: 0x58 + 0x2D0 = 0x328
    u8 _pad328[0x44C - 0x328];
    s32 unk44C;
    s32 unk450;
    s32 unk454;
    u8 _pad458[0xAE0 - 0x458];
    s32 unkAE0;
    u8 _padAE4[0xB2C - 0xAE4];
    s32 unkB2C;
    u8 _padB30[0xB84 - 0xB30];
    s32 unkB84;
    u8 _padB88[0xBB4 - 0xB88];
    u8 unkBB4;
    u8 _padBB5[0xBB9 - 0xBB5];
    u8 unkBB9;
    u8 unkBBA;
    u8 _padBBB[0xBBE - 0xBBB];
    u8 unkBBE;
    u8 unkBBF;
} func_800BC4AC_arg;

extern FuncPtr D_800BCA44_B1F34[];
extern FuncPtr D_800BCA5C_B1F4C[];

extern void func_800B00D4_9FF84(func_800BC4AC_arg *, s32);
extern void func_800B02AC_A015C(func_800BC4AC_arg *);
extern s32 func_8005D308_5DF08(func_800BC4AC_arg *, s32);
extern void func_8005D180_5DD80(func_800BC4AC_arg *, s32);

INCLUDE_ASM("asm/nonmatchings/B07A0", func_800BB2B0_B07A0);

INCLUDE_ASM("asm/nonmatchings/B07A0", func_800BB66C_B0B5C);

void func_800BB8E0_B0DD0(func_800BC4AC_arg *arg0) {
    D_800BCA44_B1F34[arg0->unkBBE](arg0);
}

void func_800BB910_B0E00(func_800BC4AC_arg *arg0) {
    s32 i;

    for (i = 0; i < 12; i++) {
        arg0->elements[i].ptr = (void *)(loadAssetByIndex_953B0(arg0->unkBB9, arg0->unkBBA) + i * 16 + 0xC0);
    }

    arg0->unkAE0 = 0x100000;
    arg0->unkB2C = 0x100000;
    arg0->unkBB4 = 1;
    arg0->unkB84 = arg0->unkB84 | 0x400000;
}

s32 func_800BB998_B0E88(func_800BC4AC_arg *arg0) {
    s32 pad[3];
    GameState *gameState = getCurrentAllocation();

    if (gameState->unk79 == 0) {
        func_800B00D4_9FF84(arg0, 1);
        return 1;
    }

    arg0->unk44C -= arg0->unk44C / 8;
    arg0->unk454 -= arg0->unk454 / 8;
    arg0->unk450 += -0x8000;
    func_800B02AC_A015C(arg0);

    if (arg0->unkB84 & 0x400000) {
        func_8005D180_5DD80(arg0, 2);
    } else {
        func_8005D180_5DD80(arg0, 0);
    }

    return 0;
}

INCLUDE_ASM("asm/nonmatchings/B07A0", func_800BBA54_B0F44);

s32 func_800BC008_B14F8(func_800BC4AC_arg *arg0) {
    s32 pad[3];

    getCurrentAllocation();

    arg0->unk44C -= arg0->unk44C / 8;
    arg0->unk454 -= arg0->unk454 / 8;
    arg0->unk450 += -0x8000;
    func_800B02AC_A015C(arg0);

    if (arg0->unkB84 & 0x400000) {
        func_8005D180_5DD80(arg0, 2);
    } else {
        func_8005D180_5DD80(arg0, 0);
    }

    return 0;
}

void func_800BC0A8_B1598(func_800BC4AC_arg *arg0) {
    D_800BCA5C_B1F4C[arg0->unkBBE](arg0);
}

INCLUDE_ASM("asm/nonmatchings/B07A0", func_800BC0D8_B15C8);

INCLUDE_ASM("asm/nonmatchings/B07A0", func_800BC3B8_B18A8);

s32 func_800BC4AC_B199C(func_800BC4AC_arg *arg0) {
    s32 pad[3];

    getCurrentAllocation();

    arg0->unk450 += -0x8000;
    func_800B02AC_A015C(arg0);

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
