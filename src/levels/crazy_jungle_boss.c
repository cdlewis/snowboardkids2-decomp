#include "5DBC0.h"
#include "9FF70.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
#include "rand.h"
#include "task_scheduler.h"
#include "5AA90.h"
#include "46080.h"

typedef void (*FuncPtr)(void *);

extern FuncPtr D_800BC454_ACC84[];
extern FuncPtr D_800BC460_ACC90[];
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unk7;
} D_800BACC8_AAB78_type;
extern D_800BACC8_AAB78_type D_800BACC8_AAB78[];

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} UnkA10Entry;

typedef struct {
    u8 pad[0x38];
    u8 unk38[0x6C - 0x38];
    u8 unk6C;
    u8 unk6D;
    u8 unk6E;
    u8 pad6F[1];
    u8 unk70;
    u8 unk71;
    u8 unk72;
    u8 pad73[0x434 - 0x73];
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
    u8 pad478[0x970 - 0x478];
    Mat3x3Padded unk970;
    u8 pad990[0xA10 - 0x990];
    UnkA10Entry unkA10[9];
    s32 unkA7C;
    u8 padA80[4];
    s32 unkA84;
    u8 padA88[4];
    u16 unkA8C;
    u8 padA8E[6];
    u16 unkA94;
    u8 padA96[6];
    u16 unkA9C;
    u16 unkA9E;
    u8 padAA0[0xB84 - 0xAA0];
    s32 unkB84;
    s32 unkB88;
    s32 unkB8C;
    s32 unkB90;
    u16 unkB94;
    u8 padding[0x8];
    u16 unkB9E;
    u8 padB96[0xBB7 - 0xBA0];
    u8 unkBB7;
    u8 unkBB8;
    u8 padBB9[0xBBD - 0xBB9];
    u8 unkBBD;
    u8 unkBBE;
    u8 unkBBF;
    u8 unkBC0;
    u8 unkBC1;
    u8 padBC2[0x7];
    u8 unkBC9;
    u8 unkBCA;
    u8 padBCB[1];
    u8 unkBCC;
    u8 padBCD[0xBDB - 0xBCD];
    u8 unkBDB;
} Arg0Struct;

typedef struct {
    s16 unk0;
    s16 unk2;
} D_800BC468_ACC98_type;

extern D_800BC468_ACC98_type D_800BC468_ACC98[];
extern void func_800BC0E8_AC918(Arg0Struct *);
extern s32 D_800BBA7C_AC2AC[][3];
extern s32 D_800BBA84_AC2B4[][3];

INCLUDE_ASM("asm/nonmatchings/levels/crazy_jungle_boss", func_800BB2B0_ABAE0);

INCLUDE_ASM("asm/nonmatchings/levels/crazy_jungle_boss", func_800BB754_ABF84);

void func_800BB9E8_AC218(Arg0Struct *arg0) {
    D_800BC454_ACC84[arg0->unkBBE](arg0);
}

s32 func_800BBA18_AC248(Arg0Struct *arg0) {
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
    func_8005D180_5DD80(arg0, 0);

    return 0;
}

s32 func_800BBAB8_AC2E8(Arg0Struct *arg0) {
    Mat3x3Padded sp10;
    s32 sp30[3];
    GameState *gameState;
    s16 clampedAngle;
    s16 angleDiff;
    u16 currentAngle;
    u16 newAngle;
    s32 temp;
    s32 temp2;

    gameState = getCurrentAllocation();

    if (arg0->padBC2[4] != 0) {
        func_800B00D4_9FF84((Player *)arg0, 2);
        return 1;
    }

    arg0->unkB84 |= 0x40000;

    if (arg0->unkBBF == 0) {
        if ((u32) * (s32 *)((u8 *)gameState + 0x50) < 0x1EU) {
            arg0->unkB8C = ((randA() & 0xFF) >> 2) + 0x5A;
        } else {
            arg0->unkB8C += (randA() & 0xFF) >> 1;
        }
        arg0->unkB90 = 0;
        arg0->unkA8C = 0xFFFF;
        arg0->unkBBF++;
    }

    clampedAngle = (s16)func_8006D21C_6DE1C(arg0->unkA7C, arg0->unkA84, arg0->unk434, arg0->unk43C);
    currentAngle = arg0->unkA94;
    clampedAngle = (clampedAngle - currentAngle) & 0x1FFF;
    if (clampedAngle >= 0x1001) {
        clampedAngle = clampedAngle | 0xE000;
    }
    if (clampedAngle >= 0x39) {
        clampedAngle = 0x38;
    }
    if (clampedAngle < -0x38) {
        clampedAngle = -0x38;
    }
    arg0->unkA94 = currentAngle + clampedAngle;

    if (!(arg0->unkB84 & 1)) {
        createYRotationMatrix(&arg0->unk970, arg0->unkA94);
        func_8006BDBC_6C9BC((func_8005E800_5F400_arg *)arg0->pad990, &arg0->unk970, &sp10);
        transformVector3(&arg0->unk44C, &sp10, sp30);
        sp30[0] = 0;
        transformVector2(sp30, &sp10, &arg0->unk44C);
        transformVector2((s32 *)(gameState->unk48 + 0x144), &sp10, sp30);
        if (sp30[1] > 0) {
            sp30[1] = 0;
        }
        arg0->unk44C += sp30[0];
        arg0->unk450 += sp30[1];
        arg0->unk454 += sp30[2];
    } else {
        temp = arg0->unk44C;
        arg0->unk44C = temp - (temp / 16);
        temp2 = arg0->unk454;
        arg0->unk454 = temp2 - (temp2 / 16);
    }

    if (arg0->unk450 > 0) {
        arg0->unk450 = 0;
    }
    arg0->unk450 += -0x10000;

    func_800B02AC_A015C((Player *)arg0);
    if (func_8005D180_5DD80(arg0, 1) != 0) {
        arg0->unkB90 = 0;
    }

    arg0->unkB90++;
    if (arg0->unkB90 == 4 || arg0->unkB90 == 0xC) {
        func_80056B7C_5777C(&arg0->unk434, 0x49);
    }

    if (arg0->unkB8C == 0) {
        func_8004B794_4C394(arg0);
        if (*((u8 *)gameState + 0x86) != 0) {
            arg0->unkB8C = arg0->unkB8C + (0xF + ((randA() & 0xFF) >> 4));
        } else {
            arg0->unkB8C = arg0->unkB8C + (0xF + ((randA() & 0xFF) >> 2));
        }
    } else {
        arg0->unkB8C--;
    }

    return 0;
}

s32 func_800BBD98_AC5C8(Arg0Struct *arg0) {
    s32 pad[3];
    getCurrentAllocation();

    arg0->unk44C -= arg0->unk44C / 8;
    arg0->unk454 -= arg0->unk454 / 8;
    arg0->unk450 += -0x8000;
    func_800B02AC_A015C((Player *)arg0);
    func_8005D180_5DD80(arg0, 0);

    return 0;
}

void func_800BBE1C_AC64C(Arg0Struct *arg0) {
    D_800BC460_ACC90[arg0->unkBBE](arg0);
}

s32 func_800BBE4C_AC67C(Arg0Struct *arg0) {
    if (arg0->unkBBF == 0) {
        s32 temp = arg0->unk450;
        arg0->unkBBF++;
        arg0->unkB8C = 1;
        if (temp > 0) {
            arg0->unk450 = 0;
        }
        func_80056B7C_5777C(&arg0->unk434, 0x21);
    }

    arg0->unkB8C--;
    if (arg0->unkB8C == 0) {
        arg0->unkB8C = D_800BC468_ACC98[arg0->unkBC0].unk0;
        arg0->unkB9E = D_800BC468_ACC98[arg0->unkBC0].unk2;
        arg0->unkBC0++;
    }

    arg0->unkB88 = 0x10;
    arg0->unk44C = 0;
    arg0->unk454 = 0;
    arg0->unk450 += -0x8000;
    func_800B02AC_A015C((Player *)arg0);

    if (arg0->unkB8C == -1) {
        arg0->unkB88 = 0;
        arg0->unkBBD = 1;
        arg0->unkBBE = 1;
        arg0->unkBBF = 0;
        arg0->unkBC0 = 0;
    }

    return 0;
}

s32 func_800BBF3C_AC76C(Arg0Struct *arg0) {
    if (arg0->unkBBF == 0) {
        arg0->unkA8C = 0xFFFF;
        arg0->unk450 = 0x80000;
        arg0->unkBBF = arg0->unkBBF + 1;
        func_80056B7C_5777C(&arg0->unk434, 0x21);
    }

    arg0->unkB88 = 0x200;
    arg0->unk44C = 0;
    arg0->unk454 = 0;
    arg0->unk450 = arg0->unk450 + -0x8000;
    func_800B02AC_A015C((Player *)arg0);
    func_8005D180_5DD80(arg0, 2);

    if (arg0->unkB84 & 1) {
    } else {
        arg0->unkB88 = 0;
        arg0->unkBBD = 1;
        arg0->unkBBE = 1;
        arg0->unkBBF = 0;
        arg0->unkBC0 = 0;
    }
    return 0;
}

void func_800BBFEC_AC81C(Arg0Struct *arg0) {
    s32 pad[8];
    s32 sp38[3];
    s32 pad2[8];
    void *alloc;
    void *allocPlus30;
    u16 temp;

    alloc = getCurrentAllocation();
    memcpy(&arg0->unk970.unk14, &arg0->unk434, 0xC);
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

INCLUDE_ASM("asm/nonmatchings/levels/crazy_jungle_boss", func_800BC0E8_AC918);

void func_800BC23C_ACA6C(Arg0Struct *arg0) {
    s32 i;
    s32 index;
    s32 pad[36];

    getCurrentAllocation();
    func_800BC0E8_AC918(arg0);

    index = arg0->unkBCC >> 4;

    if (index == 0) {
        for (i = 0; i < 4; i++) {
            func_80064808_65408(i, (enqueueMultiPartDisplayList_arg1 *)&arg0->unk38, arg0->unkBB7);
        }
    } else {
        arg0->unk6C = D_800BACC8_AAB78[index].unk0;
        arg0->unk6D = D_800BACC8_AAB78[index].unk1;
        arg0->unk6E = D_800BACC8_AAB78[index].unk2;
        arg0->unk70 = D_800BACC8_AAB78[index].unk4;
        arg0->unk71 = D_800BACC8_AAB78[index].unk5;
        arg0->unk72 = D_800BACC8_AAB78[index].unk6;

        for (i = 0; i < 4; i++) {
            enqueueMultiPartDisplayList(i, (enqueueMultiPartDisplayList_arg1 *)&arg0->unk38, arg0->unkBB7);
        }
    }
}

#define DATA_OFFSET_ROW 214
#define DATA_OFFSET_COL_0 2
#define DATA_OFFSET_COL_2 0

void func_800BC330_ACB60(Arg0Struct *arg0) {
    s32 i;
    u8 *temp_s5;
    void *alloc;

    alloc = getCurrentAllocation();
    i = 0;
    temp_s5 = (u8 *)alloc + 0x30;

    do {
        s32 *posPtr;
        u16 temp;

        arg0->unkA10[i].unk0 = arg0->unk970.unk14 + D_800BBA7C_AC2AC[DATA_OFFSET_ROW + i][DATA_OFFSET_COL_0];
        arg0->unkA10[i].unk8 = arg0->unk970.unk1C + D_800BBA84_AC2B4[DATA_OFFSET_ROW + i][DATA_OFFSET_COL_2 + 2];
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
