#include "51060.h"
#include "52880.h"
#include "5AA90.h"
#include "5DBC0.h"
#include "5E590.h"
#include "9FF70.h"
#include "A9A40.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "graphics.h"
#include "rand.h"
#include "task_scheduler.h"

typedef void (*FuncPtr)(void *);

extern s32 D_800BCA50_B1F40;
extern s32 gControllerInputs[];

typedef struct {
    /* 0x0 */ void *ptr;
    u8 _pad[0x38];
} Element0x3C; // Size: 0x3C

typedef struct {
    u8 _pad0[0x38];
    u8 unk38[0x6C - 0x38];
    u8 unk6C;
    u8 unk6D;
    u8 unk6E;
    u8 _pad6F[1];
    u8 unk70;
    u8 unk71;
    u8 unk72;
    u8 _pad73[0x328 - 0x73];
    // Element0x3C elements[12] overlaps at offset 0x58
    u8 _pad328[0x434 - 0x328];
    s32 unk434;
    s32 unk438;
    s32 unk43C;
    u8 _pad440[0x44C - 0x440];
    s32 unk44C;
    s32 unk450;
    s32 unk454;
    u8 _pad458[0x984 - 0x458];
    u8 unk984[0xC];
    u8 _pad990[0xAE0 - 0x990];
    s32 unkAE0;
    u8 _padAE4[0xB2C - 0xAE4];
    s32 unkB2C;
    u8 _padB30[0xB84 - 0xB30];
    s32 unkB84;
    s32 unkB88;
    u8 _padB8C[0xB94 - 0xB8C];
    u16 unkB94;
    u8 _padB96[0xBB4 - 0xB96];
    u8 unkBB4;
    u8 _padBB5[0xBB7 - 0xBB5];
    u8 unkBB7;
    u8 _padBB8[0xBB9 - 0xBB8];
    u8 unkBB9;
    u8 unkBBA;
    u8 _padBBB[0xBBD - 0xBBB];
    u8 unkBBD;
    u8 unkBBE;
    u8 unkBBF;
    u8 unkBC0;
    u8 _padBC1[0xBC9 - 0xBC1];
    u8 unkBC9;
    u8 unkBCA;
    u8 _padBCB[1];
    u8 unkBCC;
    u8 _padBCD[0xBDB - 0xBCD];
    u8 unkBDB;
} func_800BC4AC_arg;

extern FuncPtr D_800BCA44_B1F34[];
extern FuncPtr D_800BCA5C_B1F4C[];

extern u16 D_8009ADE0_9B9E0;

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

void func_800BC61C_B1B0C(Player *);
void func_8004C10C_4CD0C(Player *);

INCLUDE_ASM("asm/nonmatchings/levels/ice_land_boss", func_800BB2B0_B07A0);

INCLUDE_ASM("asm/nonmatchings/levels/ice_land_boss", func_800BB66C_B0B5C);

void func_800BB8E0_B0DD0(func_800BC4AC_arg *arg0) {
    D_800BCA44_B1F34[arg0->unkBBE](arg0);
}

void func_800BB910_B0E00(Player *arg0) {
    s32 i;
    Element0x3C *elements = (Element0x3C *)arg0;

    for (i = 0; i < 12; i++) {
        *(void **)((u8 *)elements + i * 0x3C + 0x58) =
            (void *)(loadAssetByIndex_953B0(arg0->unkBB9, arg0->unkBBA) + i * 16 + 0xC0);
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
        func_800B00D4_9FF84((Player *)arg0, 1);
        return 1;
    }

    arg0->unk44C -= arg0->unk44C / 8;
    arg0->unk454 -= arg0->unk454 / 8;
    arg0->unk450 += -0x8000;
    func_800B02AC_A015C((Player *)arg0);

    if (arg0->unkB84 & 0x400000) {
        func_8005D180_5DD80(arg0, 2);
    } else {
        func_8005D180_5DD80(arg0, 0);
    }

    return 0;
}

s32 func_800BBA54_B0F44(Player *arg0) {
    Mat3x3Padded sp10;
    s32 sp30[3];
    s32 sp40[3];
    GameState *gameState;
    s16 angleDiff;
    Player *player;
    s32 v1, v0;

    gameState = (GameState *)getCurrentAllocation();

    if (arg0->unkB84 & 0x100000) {
        func_800B00C0_9FF70(arg0, 3);
        return 1;
    }

    if (arg0->unkB84 & 0x80000) {
        func_800B00D4_9FF84(arg0, 2);
        return 1;
    }

    if (gControllerInputs[1] & L_TRIG) {
        func_800BB910_B0E00(arg0);
    }

    if (arg0->unkBBF == 0) {
        arg0->unkBBF++;

        if (gameState->unk50 < 0x1EU) {
            arg0->unkB8C = ((randA() & 0xFF) >> 2) + 0x5A;
        } else if (arg0->unkB84 & 0x400000) {
            arg0->unkB8C = randA() & 0xF;
        } else {
            arg0->unkB8C = (randA() & 0xFF) >> 1;
        }
        arg0->unkB90 = 0;
    }

    arg0->unkB84 = arg0->unkB84 | 0x40000;
    func_800B9B90_A9A40(arg0);

    angleDiff = func_8006D21C_6DE1C(arg0->unkA7C, arg0->unkA84, arg0->worldPosX, arg0->worldPosZ) - arg0->unkA94;
    angleDiff = angleDiff & 0x1FFF;

    if (angleDiff >= 0x1001) {
        angleDiff = angleDiff | 0xE000;
    }

    if (angleDiff >= 0x39) {
        angleDiff = 0x38;
    }

    if (angleDiff < -0x38) {
        angleDiff = -0x38;
    }

    arg0->unkA94 = arg0->unkA94 + angleDiff;

    if (!(arg0->unkB84 & 0x1)) {
        createYRotationMatrix(&arg0->unk970, arg0->unkA94);
        func_8006BDBC_6C9BC((func_8005E800_5F400_arg *)&arg0->unk990, &arg0->unk970, &sp10);
        transformVector3(&arg0->unk44C, &sp10, sp30);
        sp30[0] = 0;
        transformVector2(sp30, &sp10, &arg0->unk44C);
        transformVector2(&D_800BCA50_B1F40, &sp10, sp30);

        if (sp30[1] > 0) {
            sp30[1] = 0;
        }

        arg0->unk44C += sp30[0];
        arg0->unk450 += sp30[1];
        arg0->unk454 += sp30[2];
    } else {
        arg0->unk44C -= arg0->unk44C / 16;
        arg0->unk454 -= arg0->unk454 / 16;
    }

    if (arg0->unk450 > 0) {
        arg0->unk450 = 0;
    }

    arg0->unk450 -= 0x10000;
    func_800B02AC_A015C(arg0);
    func_800BC61C_B1B0C(arg0);

    transformVectorRelative(&gameState->players->worldPosX, arg0->unk164, sp40);

    angleDiff = atan2Fixed(-sp40[0], -sp40[2]) & 0x1FFF;

    if (angleDiff >= 0x1000) {
        angleDiff = angleDiff | 0xE000;
    }

    if (angleDiff >= 0x81) {
        angleDiff = 0x80;
    }

    if (angleDiff < -0x80) {
        angleDiff = -0x80;
    }

    arg0->unkA9E = arg0->unkA9E + angleDiff;

    if (arg0->unkB84 & 0x400000) {
        if (func_8005D180_5DD80(arg0, 3) != 0) {
            arg0->unkB90 = 0;
        }

        arg0->unkB90++;

        if (arg0->unkB90 == 4) {
            arg0->unkB90 = 0;
            func_80056B7C_5777C(&arg0->worldPosX, 0x4B);
        }

        if (arg0->unkB8C == 0) {
            func_8004C10C_4CD0C(arg0);

            if (gameState->unk86 != 0) {
                arg0->unkB8C = ((u32)(randA() & 0xFF) >> 1) + 0x1E;
            } else {
                arg0->unkB8C = (randA() & 0xFF) + 0x3C;
            }
        } else {
            arg0->unkB8C = arg0->unkB8C - 1;
        }
    } else {
        if (func_8005D180_5DD80(arg0, 1) != 0) {
            arg0->unkB90 = 0;
        }

        arg0->unkB90 = arg0->unkB90 + 1;

        if ((arg0->unkB90 == 4) || (arg0->unkB90 == 0xC)) {
            func_80056B7C_5777C(&arg0->worldPosX, 0x4A);
        }

        player = gameState->players;
        if (player->unkB88 != 0) {
            return 0;
        }

        if (arg0->unkB8C == 0) {
            if (arg0->unkBC0 != 0) {
                if (arg0->unkBBF == 2) {
                    arg0->unkBC0 = arg0->unkBC0 - 1;
                    if (func_80054C8C_5588C(arg0->unkBB8) != 0) {
                        func_8005182C_5242C(arg0);
                    }

                    if (arg0->unkBC0 == 0) {
                        if (gameState->unk86 != 0) {
                            arg0->unkB8C = (randA() & 0xF) + 8;
                        } else {
                            arg0->unkB8C = ((u32)(randA() & 0xFF) >> 1) + 0x1E;
                        }
                    } else {
                        arg0->unkB8C = 0xA;
                    }
                } else {
                    arg0->unkBC0 = arg0->unkBC0 - 1;
                    if (func_800553D4_55FD4(arg0->unkBB8) != 0) {
                        func_8005182C_5242C(arg0);
                    }

                    if (arg0->unkBC0 == 0) {
                        arg0->unkB8C = ((u32)(randA() & 0xFF) >> 1) + 0x1E;
                    } else {
                        arg0->unkB8C = 0xA;
                    }
                }
            } else {
                // unkBC0 is zero - calculate distance
                if ((u32)distance_3d(
                        arg0->worldPosX - player->worldPosX,
                        arg0->worldPosY - player->worldPosY,
                        arg0->worldPosZ - player->worldPosZ
                    ) <= 0xDFFFFF) {
                    // Close range
                    if (func_80054C8C_5588C(arg0->unkBB8) != 0) {
                        func_8005182C_5242C(arg0);
                    }

                    arg0->unkBC0 = randA() & 0x3;

                    if (arg0->unkBC0 != 0) {
                        arg0->unkBBF = 2;
                        arg0->unkB8C = 0xA;
                    } else {
                        if (gameState->unk86 != 0) {
                            arg0->unkB8C = (randA() & 0xF) + 8;
                        } else {
                            arg0->unkB8C = ((u32)(randA() & 0xFF) >> 1) + 0x3C;
                        }
                    }
                } else {
                    // Far range
                    if (func_800553D4_55FD4(arg0->unkBB8) != 0) {
                        func_8005182C_5242C(arg0);
                    }

                    if (gameState->unk86 != 0) {
                        arg0->unkB8C = (randA() & 0xF) + 8;
                    } else {
                        arg0->unkB8C = ((u32)(randA() & 0xFF) >> 1) + 0x3C;
                    }
                }
            }
        } else {
            arg0->unkB8C = arg0->unkB8C - 1;
        }
    }

    return 0;
}

s32 func_800BC008_B14F8(func_800BC4AC_arg *arg0) {
    s32 pad[3];

    getCurrentAllocation();

    arg0->unk44C -= arg0->unk44C / 8;
    arg0->unk454 -= arg0->unk454 / 8;
    arg0->unk450 += -0x8000;
    func_800B02AC_A015C((Player *)arg0);

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

INCLUDE_ASM("asm/nonmatchings/levels/ice_land_boss", func_800BC0D8_B15C8);

s32 func_800BC3B8_B18A8(Player *arg0) {
    u8 temp;

    temp = arg0->unkBBF;
    if (temp == 0) {
        arg0->unkBBF = temp + 1;
        arg0->unk450 = 0x80000;
        if (!(arg0->unkB84 & 0x80000)) {
            if (arg0->unkBDB != 0) {
                arg0->unkBDB = arg0->unkBDB - 1;
            }
        }
    }

    arg0->unkB88 = 0x200;
    arg0->unk44C = 0;
    arg0->unk454 = 0;
    arg0->unk450 = arg0->unk450 + (-0x8000);

    func_800B02AC_A015C(arg0);
    func_8005D180_5DD80(arg0, 5);

    if ((D_8009ADE0_9B9E0 & 0xF) == 0) {
        func_8004C10C_4CD0C(arg0);
    }

    if (arg0->unk450 < 0) {
        if (!(arg0->unkB84 & 0x1)) {
            temp = arg0->unkBDB;
            arg0->unkB88 = 0;
            arg0->unkBBD = 1;
            arg0->unkBBE = 1;
            arg0->unkBBF = 0;
            arg0->unkBC0 = 0;

            if (temp == 0) {
                arg0->unkB84 = arg0->unkB84 | 0x100000;
            }

            return 0;
        }
    }

    return 0;
}

s32 func_800BC4AC_B199C(func_800BC4AC_arg *arg0) {
    s32 pad[3];

    getCurrentAllocation();

    arg0->unk450 += -0x8000;
    func_800B02AC_A015C((Player *)arg0);

    if (arg0->unkBBF == 0) {
        if (func_8005D308_5DF08(arg0, 6) != 0) {
            arg0->unkBBF = arg0->unkBBF + 1;
        }
        return 0;
    }

    func_8005D180_5DD80(arg0, 7);

    return 0;
}

void func_800BC520_B1A10(func_800BC4AC_arg *arg0) {
    s32 pad[8];
    s32 sp38[3];
    s32 pad2[8];
    GameState *alloc;
    GameDataLayout *allocPlus30;
    u16 temp;

    alloc = getCurrentAllocation();
    memcpy(arg0->unk984, &arg0->unk434, 0xC);
    allocPlus30 = &alloc->gameData;
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

INCLUDE_ASM("asm/nonmatchings/levels/ice_land_boss", func_800BC61C_B1B0C);

void func_800BC7A8_B1C98(Player *arg0) {
    s32 pad[36];
    s32 i;
    s32 index;

    getCurrentAllocation();
    func_800BC61C_B1B0C(arg0);

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

INCLUDE_ASM("asm/nonmatchings/levels/ice_land_boss", func_800BC89C_B1D8C);
