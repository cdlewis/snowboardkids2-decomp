#include "46080.h"
#include "59290.h"
#include "5AA90.h"
#include "5DBC0.h"
#include "9FF70.h"
#include "A9A40.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
#include "rand.h"
#include "task_scheduler.h"

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
    s16 unk38[6];
    u8 pad44[0x6C - 0x44];
    u8 unk6C;
    u8 unk6D;
    u8 unk6E;
    u8 pad6F[1];
    u8 unk70;
    u8 unk71;
    u8 unk72;
    u8 pad73[0xEC - 0x73];
    s16 unkEC[6];
    u8 padF8[0x1A0 - 0xF8];
    s16 unk1A0[6];
    u8 pad1AC[0x344 - 0x1AC];
    s16 unk344[6];
    u8 pad350[0x380 - 0x350];
    s16 unk380[6];
    u8 pad38C[0x3BC - 0x38C];
    s16 unk3BC[6];
    u8 pad3C8[0x434 - 0x3C8];
    Vec3i unk434;
    s32 unk440;
    s32 unk444;
    s32 unk448;
    Vec3i unk44C;
    u8 pad458[0x10];
    s32 unk468;
    u8 pad46C[0x8];
    s32 unk474;
    u8 pad478[0x970 - 0x478];
    Transform3D unk970;
    Transform3D unk990;
    Transform3D unk9B0;
    u8 pad9D0[0xA10 - 0x9D0];
    UnkA10Entry unkA10[9];
    s32 unkA7C;
    u8 padA80[4];
    s32 unkA84;
    u8 padA88[4];
    u16 unkA8C;
    u16 unkA8E;
    u16 unkA90;
    u16 unkA92;
    u16 unkA94;
    u8 padA96[6];
    u16 unkA9C;
    u16 unkA9E;
    u8 padAA0[4];
    s32 unkAA4;
    s32 unkAA8;
    u8 padAAC[0xAC2 - 0xAAC];
    s16 unkAC2;
    u8 padAC4[0xAD4 - 0xAC4];
    Vec3i unkAD4;
    u8 padAE0[0x4];
    s32 unkAE4;
    s32 unkAE8;
    s32 unkAEC;
    s32 unkAF0;
    s32 unkAF4;
    s32 unkAF8;
    s32 unkAFC;
    s32 unkB00;
    s32 unkB04;
    s32 unkB08;
    s32 unkB0C;
    s32 unkB10;
    s32 unkB14;
    s32 unkB18;
    s32 unkB1C;
    s32 unkB20;
    s32 unkB24;
    s32 unkB28;
    u8 padB2C[0xB50 - 0xB2C];
    ListNode_5AA90 unkB50;
    u8 padB69[0xB84 - 0xB6C];
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
    u8 padBC2[0x2];
    u8 finishPosition;
    u8 padBC5[0x4];
    u8 unkBC9;
    u8 unkBCA;
    u8 padBCB[1];
    u8 unkBCC;
    u8 padBCD[0x2];
    u8 unkBCF;
    u8 padBD0[0xBDB - 0xBD0];
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

typedef s32 (*StateFunc)(void *);
extern StateFunc D_800BC440_ACC70[];

void func_800BB2B0_ABAE0(Arg0Struct *arg0) {
    Transform3D sp10;
    Transform3D sp30;
    volatile u8 pad[8];
    GameState *alloc;
    Arg0Struct *player;
    s32 dist;
    s32 diff;
    s32 i;
    s32 count;
    s32 loopBound;

    alloc = getCurrentAllocation();
    func_800B9B90_A9A40((Player *)arg0);

    arg0->unk44C.x = arg0->unk434.x - arg0->unk440;
    arg0->unk44C.y = arg0->unk434.y - arg0->unk444;
    arg0->unk44C.z = arg0->unk434.z - arg0->unk448;
    memcpy(&arg0->unk440, &arg0->unk434, 0xC);

    if (arg0->unkB94 < 0x16) {
        arg0->unkAA4 = 0x180000;
    } else {
        player = (Arg0Struct *)alloc->players;
        dist = distance_3d(arg0->unk434.x - player->unk434.x, arg0->unk434.y - player->unk434.y, arg0->unk434.z - player->unk434.z);

        if (alloc->unk86 != 0) {
            if ((arg0->finishPosition == 0) & (dist > 0xE00000)) {
                arg0->unkAA4 = getCharacterBoardStatParam0(0, 8) + 0x40000;
            } else {
                arg0->unkAA4 = 0x180000;
            }
        } else {
            if ((arg0->finishPosition == 0) & (dist > 0xE00000)) {
                arg0->unkAA4 = getCharacterBoardStatParam0(0, 6) + -0x8000;
            } else {
                arg0->unkAA4 = getCharacterBoardStatParam0(0, 6) + 0x18000;
            }
        }
    }

    if (arg0->unkAA4 > 0x180000) {
        arg0->unkAA4 = 0x180000;
    }

    count = arg0->unkBCF;
    i = 0;
    if (count > 0) {
        loopBound = count;
        do {
            arg0->unkAA4 = arg0->unkAA4 - (arg0->unkAA4 >> 2);
            i++;
        } while (i < loopBound);
    }

    diff = arg0->unkAA4 - arg0->unkAA8;
    if (diff >= 0x101) {
        diff = 0x100;
    }
    if (diff < -0x80) {
        diff = -0x80;
    }
    arg0->unkAA8 = arg0->unkAA8 + diff;
    arg0->unkB84 &= 0xFFFBFFFF;

    if (arg0->unkBBD != 3) {
        if (arg0->unkAC2 != 0) {
            if (arg0->unkAC2 != 0x3D) {
                if (arg0->unkAC2 == 0x3E) {
                    arg0->unkBBD = 2;
                    arg0->unkBBE = 0;
                    arg0->unkBBF = 0;
                    arg0->unkBC0 = 0;
                }
            } else {
                arg0->unkBBD = 2;
                arg0->unkBBE = 1;
                arg0->unkBBF = 0;
                arg0->unkBC0 = 0;
            }
        }
    }
    arg0->unkAC2 = 0;

    do {
    } while (D_800BC440_ACC70[arg0->unkBBD](arg0) != 0);

    createZRotationMatrix(&arg0->unk9B0, arg0->unkA92);
    createCombinedRotationMatrix(&arg0->unk990, arg0->unkA8E, arg0->unkA90);
    createYRotationMatrix(&arg0->unk970, arg0->unkA94);

    func_8006B084_6BC84(&arg0->unk9B0, &arg0->unk990, &sp10);
    func_8006B084_6BC84(&sp10, &arg0->unk970, &sp30);

    sp30.translation.x -= arg0->unk970.translation.x;
    sp30.translation.y -= arg0->unk970.translation.y;
    sp30.translation.z -= arg0->unk970.translation.z;

    transformVector((s16 *)(alloc->unk48 + 0xF0), (s16 *)&sp30, &arg0->unkAD4);
    memcpy(&arg0->unkB50.localPos, &arg0->unkAD4, sizeof(Vec3i));
    func_8005C838_5D438(&arg0->unkB50);
    func_800BC0E8_AC918(arg0);

    transformVector((s16 *)(alloc->unk48 + 0xFC), arg0->unk38, &arg0->unkAE4);
    arg0->unkAE4 -= arg0->unk970.translation.x;
    arg0->unkAE8 -= arg0->unk970.translation.y;
    arg0->unkAEC -= arg0->unk970.translation.z;

    transformVector((s16 *)(alloc->unk48 + 0x108), arg0->unk344, &arg0->unkAF0);
    arg0->unkAF0 -= arg0->unk970.translation.x;
    arg0->unkAF4 -= arg0->unk970.translation.y;
    arg0->unkAF8 -= arg0->unk970.translation.z;

    transformVector((s16 *)(alloc->unk48 + 0x114), arg0->unk380, &arg0->unkAFC);
    arg0->unkAFC -= arg0->unk970.translation.x;
    arg0->unkB00 -= arg0->unk970.translation.y;
    arg0->unkB04 -= arg0->unk970.translation.z;

    transformVector((s16 *)(alloc->unk48 + 0x120), arg0->unk3BC, &arg0->unkB08);
    arg0->unkB08 -= arg0->unk970.translation.x;
    arg0->unkB0C -= arg0->unk970.translation.y;
    arg0->unkB10 -= arg0->unk970.translation.z;

    transformVector((s16 *)(alloc->unk48 + 0x12C), arg0->unkEC, &arg0->unkB14);
    arg0->unkB14 -= arg0->unk970.translation.x;
    arg0->unkB18 -= arg0->unk970.translation.y;
    arg0->unkB1C -= arg0->unk970.translation.z;

    transformVector((s16 *)(alloc->unk48 + 0x138), arg0->unk1A0, &arg0->unkB20);
    arg0->unkB20 -= arg0->unk970.translation.x;
    arg0->unkB24 -= arg0->unk970.translation.y;
    arg0->unkB28 -= arg0->unk970.translation.z;
}

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

    arg0->unk44C.x -= arg0->unk44C.x / 8;
    arg0->unk44C.z -= arg0->unk44C.z / 8;
    arg0->unk44C.y += -0x8000;
    func_800B02AC_A015C((Player *)arg0);
    func_8005D180_5DD80(arg0, 0);

    return 0;
}

s32 func_800BBAB8_AC2E8(Arg0Struct *arg0) {
    Transform3D sp10;
    Vec3i sp30;
    GameState *gameState;
    s16 clampedAngle;
    s16 angleDiff;
    u16 currentAngle;
    u16 newAngle;
    s32 temp;
    s32 temp2;

    gameState = getCurrentAllocation();

    if (arg0->padBC5[1] != 0) {
        func_800B00D4_9FF84((Player *)arg0, 2);
        return 1;
    }

    arg0->unkB84 |= 0x40000;

    if (arg0->unkBBF == 0) {
        if ((u32)gameState->unk50 < 0x1EU) {
            arg0->unkB8C = ((randA() & 0xFF) >> 2) + 0x5A;
        } else {
            arg0->unkB8C += (randA() & 0xFF) >> 1;
        }
        arg0->unkB90 = 0;
        arg0->unkA8C = 0xFFFF;
        arg0->unkBBF++;
    }

    clampedAngle = (s16)func_8006D21C_6DE1C(arg0->unkA7C, arg0->unkA84, arg0->unk434.x, arg0->unk434.z);
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
        func_8006BDBC_6C9BC((func_8005E800_5F400_arg *)&arg0->unk990, &arg0->unk970, &sp10);
        transformVector3(&arg0->unk44C, &sp10, &sp30);
        sp30.x = 0;
        transformVector2(&sp30, &sp10, &arg0->unk44C);
        transformVector2((s32 *)(gameState->unk48 + 0x144), &sp10, &sp30);
        if (sp30.y > 0) {
            sp30.y = 0;
        }
        arg0->unk44C.x += sp30.x;
        arg0->unk44C.y += sp30.y;
        arg0->unk44C.z += sp30.z;
    } else {
        temp = arg0->unk44C.x;
        arg0->unk44C.x = temp - (temp / 16);
        temp2 = arg0->unk44C.z;
        arg0->unk44C.z = temp2 - (temp2 / 16);
    }

    if (arg0->unk44C.y > 0) {
        arg0->unk44C.y = 0;
    }
    arg0->unk44C.y += -0x10000;

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
        if (gameState->unk86 != 0) {
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

    arg0->unk44C.x -= arg0->unk44C.x / 8;
    arg0->unk44C.z -= arg0->unk44C.z / 8;
    arg0->unk44C.y += -0x8000;
    func_800B02AC_A015C((Player *)arg0);
    func_8005D180_5DD80(arg0, 0);

    return 0;
}

void func_800BBE1C_AC64C(Arg0Struct *arg0) {
    D_800BC460_ACC90[arg0->unkBBE](arg0);
}

s32 func_800BBE4C_AC67C(Arg0Struct *arg0) {
    if (arg0->unkBBF == 0) {
        s32 temp = arg0->unk44C.y;
        arg0->unkBBF++;
        arg0->unkB8C = 1;
        if (temp > 0) {
            arg0->unk44C.y = 0;
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
    arg0->unk44C.x = 0;
    arg0->unk44C.z = 0;
    arg0->unk44C.y += -0x8000;
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
        arg0->unk44C.y = 0x80000;
        arg0->unkBBF = arg0->unkBBF + 1;
        func_80056B7C_5777C(&arg0->unk434, 0x21);
    }

    arg0->unkB88 = 0x200;
    arg0->unk44C.x = 0;
    arg0->unk44C.z = 0;
    arg0->unk44C.y = arg0->unk44C.y + -0x8000;
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
    Vec3i sp38;
    s32 pad2[8];
    GameState *alloc;
    void *allocPlus30;
    u16 temp;

    alloc = getCurrentAllocation();
    memcpy(&arg0->unk970.translation.x, &arg0->unk434, 0xC);
    allocPlus30 = &alloc->gameData;
    temp = func_80059E90_5AA90(arg0, allocPlus30, arg0->unkB94, &arg0->unk434);
    arg0->unkB94 = temp;
    func_80060CDC_618DC(allocPlus30, temp, &arg0->unk434, 0x187000, &sp38);
    arg0->unk434.x = arg0->unk434.x + sp38.x;
    arg0->unk434.z = arg0->unk434.z + sp38.z;
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
    GameState *alloc;

    alloc = getCurrentAllocation();
    i = 0;
    temp_s5 = (u8 *)&alloc->gameData;

    do {
        s32 *posPtr;
        u16 temp;

        arg0->unkA10[i].unk0 = arg0->unk970.translation.x + D_800BBA7C_AC2AC[DATA_OFFSET_ROW + i][DATA_OFFSET_COL_0];
        arg0->unkA10[i].unk8 = arg0->unk970.translation.z + D_800BBA84_AC2B4[DATA_OFFSET_ROW + i][DATA_OFFSET_COL_2 + 2];
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
