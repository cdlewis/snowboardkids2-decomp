#include "common.h"
#include "5DBC0.h"
#include "9FF70.h"
#include "displaylist.h"
#include "gamestate.h"
#include "task_scheduler.h"
#include "rand.h"
#include "geometry.h"
#include "5AA90.h"
#include "52880.h"

typedef void (*FuncPtr)(void *);

extern s16 identityMatrix[];
extern Mat3x3Padded D_8009A890_9B490;
extern s32 D_8009A8A4_9B4A4;
extern s32 D_8009A8A8_9B4A8;
extern s32 D_8009A8AC_9B4AC;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} UnkA10Entry;

typedef struct {
    u8 pad[0x38];
    s16 unk38[0x10];                    /* 0x38 - 0x57 */
    u8 pad58[0x74 - 0x58];
    u8 unk74[0x20];                     /* 0x74 - 0x93 */
    u8 pad94[0xB0 - 0x94];
    u8 unkB0[0x20];                     /* 0xB0 - 0xCF */
    u8 padD0[0x434 - 0xD0];
    s32 unk434;                         /* 0x434 */
    s32 unk438;                         /* 0x438 */
    s32 unk43C;                         /* 0x43C */
    u8 unk440[0xC];
    s32 unk44C;                         /* 0x44C */
    s32 unk450;                         /* 0x450 */
    s32 unk454;                         /* 0x454 */
    u8 pad458[0x10];
    s32 unk468;                         /* 0x468 */
    u8 pad46C[0x8];
    s32 unk474;                         /* 0x474 */
    u8 pad478[0x950 - 0x478];
    u8 unk950[0x20];                    /* 0x950 - 0x96F */
    u8 unk970[0x14];                    /* 0x970 - 0x983 */
    s32 unk984;                         /* 0x984 */
    s32 unk988;                         /* 0x988 */
    s32 unk98C;                         /* 0x98C */
    u8 unk990[0x20];                    /* 0x990 - 0x9AF */
    u8 unk9B0[0x20];                    /* 0x9B0 - 0x9CF */
    u8 pad9D0[0x9F0 - 0x9D0];
    u8 unk9F0[0x20];                    /* 0x9F0 - 0xA0F */
    UnkA10Entry unkA10[9];              /* 0xA10 - 0xA7B (9 * 12 = 108 = 0x6C) */
    u8 padA7C[0xA8C - 0xA7C];
    u16 unkA8C;                         /* 0xA8C */
    u8 padA8E[0xA9C - 0xA8E];
    u16 unkA9C;                         /* 0xA9C */
    u16 unkA9E;                         /* 0xA9E */
    u8 padAA0[0xB84 - 0xAA0];
    s32 unkB84;                         /* 0xB84 */
    s32 unkB88;                         /* 0xB88 */
    s32 unkB8C;                         /* 0xB8C */
    u8 padB90[0xB94 - 0xB90];
    u16 unkB94;                         /* 0xB94 */
    u8 padB96[0xB9E - 0xB96];
    u16 unkB9E;                         /* 0xB9E */
    u8 padBA0[0xBB8 - 0xBA0];
    u8 unkBB8;                          /* 0xBB8 */
    u8 padBB9[0xBBD - 0xBB9];
    u8 unkBBD;                          /* 0xBBD */
    u8 unkBBE;                          /* 0xBBE */
    u8 unkBBF;                          /* 0xBBF */
    u8 unkBC0;                          /* 0xBC0 */
    u8 unkBC1;                          /* 0xBC1 */
    u8 padBC2[0x7];
    u8 unkBC9;                          /* 0xBC9 */
    u8 unkBCA;                          /* 0xBCA */
    u8 padBCB[1];
    u8 unkBCC;                          /* 0xBCC */
    u8 padBCD[0xBDB - 0xBCD];
    u8 unkBDB;                          /* 0xBDB */
} Arg0Struct;

typedef struct {
    s16 m[3][3];
    s16 pad;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
} LocalMat;

extern FuncPtr D_800BCB5C_B411C[];
extern FuncPtr D_800BCB74_B4134[];
extern s16 D_800BCB7C_B413C[];
extern u16 D_800BCB7E_B413E[];
extern s32 D_800BCBA0_B4160[][3];
extern void func_80041EA4_42AA4(s32 *arg0);

INCLUDE_ASM("asm/nonmatchings/levels/jingle_town_boss", func_800BB2B0_B2870);

INCLUDE_ASM("asm/nonmatchings/levels/jingle_town_boss", func_800BB66C_B2C2C);

void func_800BB86C_B2E2C(Arg0Struct *arg0) {
    D_800BCB5C_B411C[arg0->unkBBE](arg0);
}

s32 func_800BB89C_B2E5C(Arg0Struct *arg0) {
    s32 pad[3];
    GameState *gameState = getCurrentAllocation();

    if (gameState->unk79 == 0) {
        func_800B00D4_9FF84((Player *)arg0, 1);
        return 1;
    }

    arg0->unk44C -= arg0->unk44C / 8;
    arg0->unk454 -= arg0->unk454 / 8;
    arg0->unk450 += -0x8000;
    func_800B02AC_A015C((Player *)arg0);

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
    func_800B02AC_A015C((Player *)arg0);
    return 0;
}

void func_800BBF40_B3500(Arg0Struct *arg0) {
    D_800BCB74_B4134[arg0->unkBBE](arg0);
}

s32 func_800BBF70_B3530(Arg0Struct *arg0) {
    u8 temp_v1;

    temp_v1 = arg0->unkBBF;
    if (temp_v1 == 0) {
        arg0->unkBBF = temp_v1 + 1;
        arg0->unkB8C = 1;
        if (arg0->unk450 > 0) {
            arg0->unk450 = 0;
        }
        if (!(arg0->unkB84 & 0x80000)) {
            u8 temp_v0 = arg0->unkBDB;
            if (temp_v0 != 0) {
                arg0->unkBDB = temp_v0 - 1;
            }
        }
    }

    arg0->unkB8C--;
    if (arg0->unkB8C == 0) {
        arg0->unkB8C = D_800BCB7C_B413C[arg0->unkBC0 * 2];
        arg0->unkB9E = D_800BCB7E_B413E[arg0->unkBC0 * 2];
        arg0->unkBC0++;
    }

    arg0->unkB88 = 0x10;
    arg0->unk44C = 0;
    arg0->unk454 = 0;
    arg0->unk450 -= 0x8000;
    func_800B02AC_A015C((Player *)arg0);

    if (arg0->unkB8C == -1) {
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

    func_800B02AC_A015C((Player *)arg0);

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

s32 func_800BC1C0_B3780(Arg0Struct *arg0) {
    s32 sp10[3];
    s32 sp20[3];
    u8 temp_v0;

    getCurrentAllocation();

    temp_v0 = arg0->unkBBE;
    if (temp_v0 == 0) {
        arg0->unkBBE++;
        transformVector2((s16 *)D_800BCBA0_B4160, arg0->unk38, sp10);
        arg0->unk434 += sp10[0];
        arg0->unk438 += sp10[1];
        arg0->unk43C += sp10[2];
        memcpy(arg0->unk440, &arg0->unk434, 0xC);
        arg0->unkB84 |= 0x200000;
        transformVector(&((s16 *)D_800BCBA0_B4160)[6], arg0->unk38, sp20);
        func_80041EA4_42AA4(sp20);
        transformVector(&((s16 *)D_800BCBA0_B4160)[12], arg0->unk38, sp20);
        func_80041EA4_42AA4(sp20);
        transformVector(&((s16 *)D_800BCBA0_B4160)[18], arg0->unk38, sp20);
        func_80041EA4_42AA4(sp20);
        transformVector(&((s16 *)D_800BCBA0_B4160)[24], arg0->unk38, sp20);
        func_80041EA4_42AA4(sp20);
        transformVector(&((s16 *)D_800BCBA0_B4160)[30], arg0->unk38, sp20);
        func_80041EA4_42AA4(sp20);
        arg0->unk468 = 0x100;
    }

    arg0->unk44C -= arg0->unk44C / 8;
    arg0->unk454 -= arg0->unk454 / 8;
    arg0->unk450 += -0x8000;
    func_800BBE68_B3428(arg0);
    arg0->unkA9E += arg0->unk468;
    if (arg0->unk468 != 0) {
        arg0->unk468 -= 2;
    }
    func_800B02AC_A015C((Player *)arg0);
    return 0;
}

void func_800BC378_B3938(Arg0Struct *arg0) {
    s32 pad[8];
    s32 sp38[3];
    s32 pad2[8];
    void *alloc;
    void *allocPlus30;
    u16 temp;

    alloc = getCurrentAllocation();
    memcpy(&arg0->unk984, &arg0->unk434, 0xC);
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

void func_800BC474_B3A34(Arg0Struct *arg0) {
    s32 pad1[8];
    LocalMat sp30;
    LocalMat sp50;
    s32 sp70[4];
    LocalMat *temp_s0;
    void *temp_s2;
    s32 *ptr;

    func_8006B084_6BC84(arg0->unk990, arg0->unk970, arg0->unk9F0);
    temp_s2 = arg0->unk950;
    func_8006B084_6BC84(arg0->unk9B0, arg0->unk9F0, temp_s2);

    if (arg0->unkB88 & 0x10) {
        temp_s0 = &sp30;
        memcpy(temp_s0, identityMatrix, 0x20);
        temp_s0->m[1][1] = arg0->unkB9E;
        func_8006B084_6BC84(temp_s0, temp_s2, arg0->unk38);
    } else {
        memcpy(arg0->unk38, temp_s2, 0x20);
    }

    createCombinedRotationMatrix(&sp50, arg0->unkA9C, arg0->unkA9E);
    sp50.unk14 = 0;
    sp50.unk1C = 0;

    if (arg0->unkB84 & 0x200000) {
        sp50.unk18 = 0x140000;
    } else {
        sp50.unk18 = 0x3A0000;
    }

    func_8006B084_6BC84(&sp50, arg0->unk38, arg0->unk74);

    ptr = &D_8009A8A4_9B4A4;
    *(s32 *)((u8 *)arg0 + 0x8C) = *(s32 *)((u8 *)arg0 + 0x8C) + arg0->unk474;

    *ptr = 0;
    D_8009A8A8_9B4A8 = 0x140000;
    D_8009A8AC_9B4AC = 0;

    func_8006B084_6BC84(ptr - 5, arg0->unk74, arg0->unkB0);
}

INCLUDE_ASM("asm/nonmatchings/levels/jingle_town_boss", func_800BC5A8_B3B68);

void func_800BCA10_B3FD0(Arg0Struct *arg0) {
    s32 i;
    u8 *temp_s5;
    void *alloc;

    alloc = getCurrentAllocation();
    i = 0;
    temp_s5 = (u8 *)alloc + 0x30;

    do {
        s32 *posPtr;
        u16 temp;

        arg0->unkA10[i].unk0 = arg0->unk984 + D_800BCBA0_B4160[6 + i][0];
        arg0->unkA10[i].unk8 = arg0->unk98C + D_800BCBA0_B4160[6 + i][2];
        posPtr = &arg0->unkA10[i].unk0;
        temp = func_80059E90_5AA90(arg0, temp_s5, arg0->unkB94, posPtr);
        arg0->unkA10[i].unk4 = func_8005CFC0_5DBC0(temp_s5, temp, posPtr, 0x100000);
        i++;
    } while (i < 9);

    arg0->unkBC1 = 1;

    for (i = 0; i < 4; i++) {
        debugEnqueueCallback((u16)i, 1, func_800B9500_A93B0, arg0);
    }
}
