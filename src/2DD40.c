#include "2DD40.h"
#include "D_800AFE8C_A71FC_type.h"
#include "task_scheduler.h"

extern void func_8002A2D0_2AED0(void *);
extern void func_8002EBB0_2F7B0(void *);
extern void func_8002BEF4_2CAF4(void *);

typedef struct Func2E024Arg Func2E024Arg;
struct Func2E024Arg {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1[0x4];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 pad2[0x3E];
    /* 0x62 */ u8 unk62;
    /* 0x63 */ u8 pad3[0x51];
    /* 0xB4 */ s16 unkB4;
    /* 0xB6 */ u8 pad4[0x8];
    /* 0xBE */ s16 unkBE;
    /* 0xC0 */ u8 pad5[0x2];
    /* 0xC2 */ u8 unkC2;
    /* 0xC3 */ u8 pad6[0x3];
    /* 0xC6 */ s8 unkC6;
    /* 0xC7 */ u8 pad7[0x1];
    /* 0xC8 */ void (*unkC8)(Func2E024Arg *);
    /* 0xCC */ u8 pad8[0x9];
    /* 0xD5 */ u8 unkD5;
};

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002D140_2DD40);

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002D46C_2E06C);

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002D668_2E268);

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
            if ((s16)arg0->unkBE == 0x2D) {
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