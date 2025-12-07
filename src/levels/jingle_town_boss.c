#include "common.h"
#include "gamestate.h"
#include "task_scheduler.h"

typedef void (*FuncPtr)(void *);

typedef struct {
    u8 pad[0x434];
    s32 unk434;
    s32 unk438;
    s32 unk43C;
    u8 pad440[0xC];
    s32 unk44C;
    s32 unk450;
    s32 unk454;
    u8 pad458[0x10];
    s32 unk468;
    u8 pad46C[0x8];
    s32 unk474;
    u8 pad478[0x984 - 0x478];
    u8 unk984[0xC];
    u8 pad990[0xA8C - 0x990];
    u16 unkA8C;
    u8 padA8E[0xA9C - 0xA8E];
    u16 unkA9C;
    u16 unkA9E;
    u8 padAA0[0xB84 - 0xAA0];
    s32 unkB84;
    s32 unkB88;
    s32 unkB8C;
    u8 padB90[0xB94 - 0xB90];
    u16 unkB94;
    u8 padB96[0xBB8 - 0xB96];
    u8 unkBB8;
    u8 padBB9[0xBBD - 0xBB9];
    u8 unkBBD;
    u8 unkBBE;
    u8 unkBBF;
    u8 unkBC0;
    u8 padBC1[0x8];
    u8 unkBC9;
    u8 unkBCA;
    u8 padBCB[1];
    u8 unkBCC;
    u8 padBCD[0xBDB - 0xBCD];
    u8 unkBDB;
} Arg0Struct;

extern FuncPtr D_800BCB5C_B411C[];
extern FuncPtr D_800BCB74_B4134[];
extern void func_800B00D4_9FF84(Arg0Struct *, s32);
extern void func_800B02AC_A015C(Arg0Struct *);
extern u16 func_80059E90_5AA90(void *arg0, void *arg1, u16 arg2, void *arg3);
extern void func_80060CDC_618DC(void *arg0, u16 arg1, void *arg2, s32 arg3, s32 *arg4);
extern void func_8005C868_5D468(void *arg0);
extern void func_8005CFFC_5DBFC(void *arg0, u16 arg1, void *arg2, void *arg3, void *arg4);
extern u8 randA(void);
extern s32 func_800544B4_550B4(s32, s32, s32);

INCLUDE_ASM("asm/nonmatchings/levels/jingle_town_boss", func_800BB2B0_B2870);

INCLUDE_ASM("asm/nonmatchings/levels/jingle_town_boss", func_800BB66C_B2C2C);

void func_800BB86C_B2E2C(Arg0Struct *arg0) {
    D_800BCB5C_B411C[arg0->unkBBE](arg0);
}

s32 func_800BB89C_B2E5C(Arg0Struct *arg0) {
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

    return 0;
}

INCLUDE_ASM("asm/nonmatchings/levels/jingle_town_boss", func_800BB930_B2EF0);

void func_800BBE68_B3428(Arg0Struct *arg0) {
    s16 val;
    s16 adj;

    val = arg0->unkA9C;
    adj = -val;

    if (adj >= 0x1000) {
        adj -= 0x2000;
    }

    if (adj > 0x80) {
        adj = 0x80;
    }

    if (adj < -0x80) {
        adj = -0x80;
    }

    arg0->unkA9C = val + adj;
}

s32 func_800BBEBC_B347C(Arg0Struct *arg0) {
    s32 pad[3];

    func_800BBE68_B3428(arg0);
    arg0->unkA9E -= 0x100;
    arg0->unk44C -= arg0->unk44C / 8;
    arg0->unk454 -= arg0->unk454 / 8;
    arg0->unk450 += -0x8000;
    func_800B02AC_A015C(arg0);
    return 0;
}

void func_800BBF40_B3500(Arg0Struct *arg0) {
    D_800BCB74_B4134[arg0->unkBBE](arg0);
}

INCLUDE_ASM("asm/nonmatchings/levels/jingle_town_boss", func_800BBF70_B3530);

s32 func_800BC094_B3654(Arg0Struct *arg0) {
    if (arg0->unkBBF == 0) {
        s32 temp_v1 = arg0->unkB84;
        arg0->unkBBF++;
        arg0->unk468 = 0x80000;
        arg0->unkB8C = 4;
        if ((temp_v1 & 0x80000) == 0) {
            if (arg0->unkBDB != 0) {
                arg0->unkBDB--;
            }
        }
    }

    arg0->unkB88 = 0x200;
    arg0->unk450 += -0x8000;
    arg0->unk468 += -0x8000;

    func_800BBE68_B3428(arg0);

    arg0->unk44C = 0;
    arg0->unk454 = 0;
    arg0->unkA9E = (arg0->unkA9E + 0x100) & 0x1FFF;

    if (arg0->unkB8C == 0) {
        u8 rand_val = randA();
        s32 param = (rand_val & 1) | 2;
        func_800544B4_550B4(param, arg0->unkBB8, 0);
        arg0->unkB8C = 4;
    } else {
        arg0->unkB8C--;
    }

    func_800B02AC_A015C(arg0);

    arg0->unk474 += arg0->unk468;

    if (arg0->unk474 == 0) {
        arg0->unkB88 = 0;
        arg0->unkBBD = 1;
        arg0->unkBBE = 1;
        arg0->unkBBF = 0;
        arg0->unkBC0 = 0;
        arg0->unk474 = 0;

        if (arg0->unkBDB == 0) {
            arg0->unkB84 |= 0x100000;
        }
    }

    return 0;
}

INCLUDE_ASM("asm/nonmatchings/levels/jingle_town_boss", func_800BC1C0_B3780);

void func_800BC378_B3938(Arg0Struct *arg0) {
    s32 pad[8];
    s32 sp38[3];
    s32 pad2[8];
    void *alloc;
    void *allocPlus30;
    u16 temp;

    alloc = getCurrentAllocation();
    memcpy(arg0->unk984, &arg0->unk434, 0xC);
    allocPlus30 = (u8 *)alloc + 0x30;
    temp = func_80059E90_5AA90(arg0, allocPlus30, arg0->unkB94, &arg0->unk434);
    arg0->unkB94 = temp;
    func_80060CDC_618DC(allocPlus30, temp, &arg0->unk434, 0x187000, sp38);
    arg0->unk434 = arg0->unk434 + sp38[0];
    arg0->unk43C = arg0->unk43C + sp38[2];
    func_8005C868_5D468(arg0);

    if (arg0->unkB84 & 0x10000) {
        arg0->unkBC9 = 0;
    } else {
        func_8005CFFC_5DBFC(allocPlus30, arg0->unkB94, &arg0->unk434, &arg0->unkBC9, &arg0->unkBCC);
        arg0->unkBCA = arg0->unkBC9 >> 4;
        arg0->unkBC9 = arg0->unkBC9 & 0xF;
    }
}

INCLUDE_ASM("asm/nonmatchings/levels/jingle_town_boss", func_800BC474_B3A34);

INCLUDE_ASM("asm/nonmatchings/levels/jingle_town_boss", func_800BC5A8_B3B68);

INCLUDE_ASM("asm/nonmatchings/levels/jingle_town_boss", func_800BCA10_B3FD0);
