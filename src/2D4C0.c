#include "297B0.h"
#include "2C8F0.h"
#include "2DD40.h"
#include "common.h"
#include "gamestate.h"
#include "rand.h"
#include "task_scheduler.h"

INCLUDE_ASM("asm/nonmatchings/2D4C0", func_8002C8C0_2D4C0);

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 pad4[0x14];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[0x4];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 pad24[0x2C];
    /* 0x50 */ s16 unk50;
    /* 0x52 */ u8 pad52[0x10];
    /* 0x62 */ s8 unk62;
    /* 0x63 */ u8 pad63[0x65];
    /* 0xC8 */ void *unkC8;
    /* 0xCC */ u8 padCC[0x9];
    /* 0xD5 */ u8 unkD5;
} Func8002CA8CArg;

void func_8002CA8C_2D68C(Func8002CA8CArg *);

void func_8002CA8C_2D68C(Func8002CA8CArg *arg0) {
    GameState *allocation;
    s32 i;
    Func8002CA8CArg *ptr;

    allocation = getCurrentAllocation();

    for (i = 0; i < arg0->unkD5; i++) {
        ptr = (Func8002CA8CArg *)((u8 *)arg0 + i * 0x64);

        if (ptr->unk50 != 0x19) {
            if (ptr->unk50 == 0x1A) {
                ptr->unk62 = 0;
            }
        } else {
            if (ptr->unk62 != 0) {
                ptr->unk50 = 0x1A;
            }
        }

        func_8002A2D0_2AED0(ptr);
        allocation->unk408[i] = ptr->unk18;
        allocation->unk410[i] = ptr->unk20;
    }

    if (allocation->unk42A == 0x11) {
        func_8002EBB0_2F7B0(arg0);
        arg0->unkC8 = func_8002CA8C_2D68C;
        setCallback(func_8002BEF4_2CAF4);
    }
}

INCLUDE_ASM("asm/nonmatchings/2D4C0", func_8002CB88_2D788);

extern void func_8000A49C_B09C(s32, s32, s32, s32, void *, s32, s32, s32, s32, s32);
extern u16 D_8009ADE0_9B9E0;

typedef struct Func8002CD3CArg Func8002CD3CArg;
struct Func8002CD3CArg {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 pad0[0x14];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1[0x4];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 pad2[0x1C];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ u8 pad3[0x8];
    /* 0x50 */ s16 unk50;
    /* 0x52 */ u8 pad3b[0x10];
    /* 0x62 */ s8 unk62;
    /* 0x63 */ u8 pad5[0x1];
    /* 0x64 */ s32 unk64;
    /* 0x68 */ u8 pad6[0x3C];
    /* 0xA4 */ s32 unkA4;
    /* 0xA8 */ u8 pad7[0x20];
    /* 0xC8 */ void (*unkC8)(Func8002CD3CArg *);
    /* 0xCC */ u16 unkCC;
    /* 0xCE */ u16 unkCE;
    /* 0xD0 */ u8 pad12[0x5];
    /* 0xD5 */ u8 unkD5;
};

void func_8002CD3C_2D93C(Func8002CD3CArg *arg0) {
    GameState *allocation;
    s32 i;
    Func8002CD3CArg *ptr;
    s32 temp_a0;

    allocation = getCurrentAllocation();

    for (i = 0; i < arg0->unkD5; i++) {
        ptr = (Func8002CD3CArg *)((u8 *)arg0 + i * 0x64);

        switch (ptr->unk50) {
            case 0x13:
                if (ptr->unk62 != 0) {
                    ptr->unk50 = 0x14;
                }
                break;
            case 0x14:
                ptr->unk62 = 0;
                break;
        }

        func_8002A2D0_2AED0(ptr);
        allocation->unk408[i] = ptr->unk18;
        allocation->unk410[i] = ptr->unk20;
    }

    if (arg0->unkCC != 0) {
        arg0->unkCC--;
        if (arg0->unkCC == 0) {
            temp_a0 = randB() & 0x1F;
            arg0->unkCE = (u8)temp_a0 + 0x18;
            if (D_8009ADE0_9B9E0 & 1) {
                arg0->unk44 = 0x290000;
                func_8000A49C_B09C(arg0->unk0, 0, 8, (s16)(arg0->unkCE - 4), &arg0->unk40, 0x10000, 0, 2, 0, 0);
            } else {
                arg0->unkCE = (u8)temp_a0 + 0x2C;
                arg0->unk44 = 0x290000;
                func_8000A49C_B09C(arg0->unk0, 0, 0x3E, (s16)(arg0->unkCE - 4), &arg0->unk40, 0x10000, 0, 2, 0, 0x400);
                func_8000A49C_B09C(arg0->unk64, 0, 0x3E, (s16)arg0->unkCE, &arg0->unkA4, 0x10000, 0, 2, 0, 0x1F00);
            }
        }
    } else {
        arg0->unkCE--;
        if (arg0->unkCE == 0) {
            arg0->unkCC = (randB() & 0x1F) + 0x28;
        }
    }

    if (allocation->unk42A == 0x11) {
        func_8002EBB0_2F7B0(arg0);
        arg0->unkC8 = func_8002CD3C_2D93C;
        allocation->unk42E = 1;
        arg0->unkCC = (randB() & 0x1F) + 0x28;
        arg0->unkCE = 0;
        setCallback(func_8002BEF4_2CAF4);
    }
}

INCLUDE_ASM("asm/nonmatchings/2D4C0", func_8002CFAC_2DBAC);
