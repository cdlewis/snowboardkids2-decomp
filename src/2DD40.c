#include "2DD40.h"
#include "297B0.h"
#include "2C8F0.h"
#include "D_800AFE8C_A71FC_type.h"
#include "task_scheduler.h"

extern void func_8000A49C_B09C(s32, s32, s32, s32, void *, s32, s32, s32, s32, s32);

typedef struct Func2E024Arg Func2E024Arg;
struct Func2E024Arg {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 pad0[0x14];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1[0x4];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 pad2[0x1C];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ u8 pad3[0xC];
    /* 0x50 */ s16 unk50;
    /* 0x52 */ u8 pad3b[0xC];
    /* 0x5E */ u8 unk5E;
    /* 0x5F */ u8 pad4[0x3];
    /* 0x62 */ s8 unk62;
    /* 0x63 */ u8 pad5[0x1];
    /* 0x64 */ s32 unk64;
    /* 0x68 */ u8 pad6[0x3C];
    /* 0xA4 */ s32 unkA4;
    /* 0xA8 */ u8 pad7[0xC];
    /* 0xB4 */ s16 unkB4;
    /* 0xB6 */ u8 pad8[0x8];
    /* 0xBE */ s16 unkBE;
    /* 0xC0 */ u8 pad9[0x2];
    /* 0xC2 */ u8 unkC2;
    /* 0xC3 */ u8 pad10[0x3];
    /* 0xC6 */ s8 unkC6;
    /* 0xC7 */ u8 pad11[0x1];
    /* 0xC8 */ void (*unkC8)(Func2E024Arg *);
    /* 0xCC */ s16 unkCC;
    /* 0xCE */ s16 unkCE;
    /* 0xD0 */ u8 pad12[0x5];
    /* 0xD5 */ u8 unkD5;
};

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002D140_2DD40);

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002D46C_2E06C);

void func_8002D668_2E268(Func2E024Arg *arg0) {
    GameState *allocation;
    s32 i;
    Func2E024Arg *ptr;
    s32 val;

    allocation = getCurrentAllocation();

    for (i = 0; i < arg0->unkD5; i++) {
        ptr = (Func2E024Arg *)((u8 *)arg0 + i * 0x64);
        val = ptr->unk5E;

        if (val < 0) {
            goto cont;
        }

        if (val >= 2) {
            if (val >= 4) {
                goto cont;
            }
            goto case23;
        }

        if (ptr->unk62 == 0) {
            goto cont;
        }
        if (i == 0) {
            arg0->unk50 = 0x12;
            arg0->unk5E = 2;
            goto cont;
        }
        arg0->unkB4 = 0x1F;
        arg0->unkC2 = 3;
        goto cont;

    case23:
        ptr->unk62 = 0;
        if (getFreeNodeCount(2) == 0x14) {
            func_8000A49C_B09C(arg0->unk0, 0, 0x29, -1, &arg0->unk40, 0x10000, 0, 2, 0, 0);
            func_8000A49C_B09C(arg0->unk64, 0, 0x24, -1, &arg0->unkA4, 0x10000, 0, 2, 0, 0);
        }

    cont:
        func_8002A2D0_2AED0(ptr);
        ((s32 *)allocation)[0x102 + i] = ptr->unk18;
        ((s32 *)allocation)[0x104 + i] = ptr->unk20;
    }

    if (allocation->unk42A == 0x11) {
        allocation->unk42E = 1;
        arg0->unkCC = 1;
        arg0->unkCE = 1;
        func_8002EBB0_2F7B0(arg0);
        arg0->unkC8 = func_8002D668_2E268;
        setCallback(func_8002BEF4_2CAF4);
    }
}

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002D814_2E414);

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002DA54_2E654);

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002DE44_2EA44);

void func_8002E024_2EC24(Func2E024Arg *arg0) {
    GameState *allocation;
    s32 i;

    allocation = (GameState *)getCurrentAllocation();

    switch (arg0->unkC2) {
        case 0x62:
            arg0->unkBE++;
            if (arg0->unkBE == 0x2D) {
                arg0->unkB4 = 0x10;
                arg0->unkBE = 0;
                arg0->unkC2 = 1;
            }
            break;
        case 1:
            if (arg0->unkC6 != 0) {
                arg0->unkB4 = 0x11;
                arg0->unkC2 = 0x63;
            }
            break;
        case 0x63:
            arg0->unk62 = 0;
            arg0->unkC6 = 0;
            break;
    }

    for (i = 0; i < arg0->unkD5; i++) {
        Func2E024Arg *ptr = (Func2E024Arg *)((u8 *)arg0 + i * 0x64);
        func_8002A2D0_2AED0(ptr);
        ((s32 *)allocation)[0x102 + i] = ptr->unk18;
        ((s32 *)allocation)[0x104 + i] = ptr->unk20;
    }

    if (allocation->unk42A == 0x11) {
        func_8002EBB0_2F7B0(arg0);
        arg0->unkC8 = func_8002E024_2EC24;
        setCallback(func_8002BEF4_2CAF4);
    }
}

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002E170_2ED70);

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002E314_2EF14);

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002E680_2F280);

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002E8B4_2F4B4);

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002EBB0_2F7B0);

s32 func_8002ED30_2F930(void) {
    return D_800AFE8C_A71FC->padding;
}

void func_8002ED40_2F940(s32 a0) {
    s32 new_var = 0x98967F;

    D_800AFE8C_A71FC->padding += a0;

    if (D_800AFE8C_A71FC->padding > new_var) {
        D_800AFE8C_A71FC->padding = new_var;
    }

    if (D_800AFE8C_A71FC->padding < 0) {
        D_800AFE8C_A71FC->padding = 0;
    }
}