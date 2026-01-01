#pragma once

#include "38310.h"
#include "42170.h"
#include "46080.h"
#include "5AA90.h"
#include "90F0.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "memory_allocator.h"

typedef struct {
    u8 padding[0x14];
} D_8008D7FC_8E3FC_item;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    D_8008D7FC_8E3FC_item *unk8;
} func_80019CD0_1A8D0_arg;

typedef struct {
    u8 pad0[0x18];  /* 0x00 */
    s32 unk18;      /* 0x18 */
    u8 pad1C[0x4];  /* 0x1C */
    void *unk20;    /* 0x20 */
    void *unk24;    /* 0x24 */
    void *unk28;    /* 0x28 */
    s32 unk2C;      /* 0x2C */
    u8 pad30[0xC];  /* 0x30 */
    u8 unk3C[0x20]; /* 0x3C */
    void *unk5C;    /* 0x5C */
    void *unk60;    /* 0x60 */
    void *unk64;    /* 0x64 */
    s32 unk68;      /* 0x68 */
    u8 pad6C[0xC];  /* 0x6C */
    Player *unk78;  /* 0x78 */
    s32 unk7C;      /* 0x7C */
    u16 unk80;      /* 0x80 */
    s16 unk82;      /* 0x82 */
} Func43374State;

void func_80014480_15080(void);
void func_8001452C_1512C(void);
void initControllerMotors(void);
void initRace(void);
void initLevelSelectBasicState(void);
void initPlayerCountSelectState(void);
void func_800226F0_232F0(void);
void initStoryMapState(void);
void storyMapLocationTextTask(func_80019CD0_1A8D0_arg *arg0);
void func_8001C920_1D520(void);
void func_80003EE0_4AE0(void);
void initLevelSelectWithDetail(void);
void initLevelSelectBasic(void);
void func_80035260_35E60(void *arg0, void *arg1, s16 arg2, s16 arg3, u8 arg4, u8 arg5, u8 arg6, u8 arg7, u8 arg8);
void initCharSelectIcons(void *arg0);
void collectStoryMapItem(void *arg0);
void func_8002AE80_2BA80(void *arg0);
void func_8002B248_2BE48(void *arg0);
s32 tryStoryMapNpcInteraction(void *arg0);
void func_80066444_67044(s32 arg0, func_80066444_67044_arg1 *arg1);
void func_8006BEDC_6CADC(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
void func_80057ABC_586BC(s32 arg0, s32 arg1);
void func_800B4B30_1E1BE0(s16 arg0, s16 arg1, s16 arg2, s16 arg3);
void func_80037E78_38A78(Entry1 *arg0);
void func_80041518_42118(void *arg0);
s32 func_80037FE0_38BE0(u8 arg0);
u8 func_80038000_38C00(u8 arg0);
u8 func_80038058_38C58(u8 arg0);
s32 func_80038070_38C70(void);
void func_80038090_38C90(s16 arg0);
s16 func_80069810_6A410(void);
void loadOverlay_1BBA0(void);
void func_8005100C_51C0C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_800516B4_522B4(Player *player);
void func_8003BD60_3C960(char *arg0, s16 arg1, s16 arg2, s16 arg3, void *arg4, s16 arg5, s16 arg6);
void func_800585C8_591C8(s32 arg0);
void func_80058B30_59730(Player *arg0);
s32 func_80058BB0_597B0(Player *arg0);
void func_8003B47C_3C07C(void *arg0);
void func_80044538_45138(s16 arg0);
void func_80045434_46034(s16 arg0);
void func_80041418_42018(void);
Func43374State *func_800436B4_442B4(Player *arg0);
void initTitleScreen(void);
void initLogoScreen(void);
void initVersusMode(void);
void initOptionsMenu(void);
void func_8003B1F4_3BDF4(s32 arg0, void *arg1);
void *func_8003B28C_3BE8C(void);

typedef struct {
    s16 x;
    s16 y;
    void *asset;
    s16 digitValue;
    u8 colorStyle;
    u8 pad;
} BoardShopGoldDigitState;

typedef struct {
    BoardShopGoldDigitState digits[7];
    s16 iconX;
    s16 iconY;
    void *iconAsset;
    s16 unk5C;
    u8 unk5E;
    u8 pad;
    char goldString[8];
} BoardShopGoldDisplayState;

void initBoardShopGoldDisplay(BoardShopGoldDisplayState *arg0);
s32 abs(s32 n);
void func_80058A10_59610(Player *arg0);
void func_80058A3C_5963C(Player *arg0);
void func_80058A68_59668(Player *arg0);
void func_80058AC0_596C0(Player *arg0);
void func_80059C24_5A824(Player *arg0);
void func_80059D30_5A930(Player *arg0);
Player *func_8005B548_5C148(Vec3i *arg0, s32 arg1, s32 arg2);
s16 func_8005CE98_5DA98(Player *arg0);
void func_8004A9A8_4B5A8(void *arg0, s32 arg1, void *arg2);
void func_80058AEC_596EC(Player *arg0, void *arg1);
void func_80058A94_59694(Player *arg0);
void func_80059A48_5A648(Player *arg0, s32 arg1);
void func_80059A88_5A688(Player *arg0, s32 arg1);
s32 func_80059AC4_5A6C4(Player *arg0);
u8 getItemStat1(u8 itemIndex);
u8 getItemStat2(s32 itemIndex);
u8 getItemStat3(s32 itemIndex);
void *func_8005DC60_5E860(s32 index);
void *loadAssetByIndex_5E990(s32 index);
MemoryAllocatorNode *func_8005DE04_5EA04(s16 groupIndex, s16 entityIndex);
MemoryAllocatorNode *func_8005DE30_5EA30(s16 groupIndex, s16 entityIndex);
s32 loadSpriteAsset(SpriteAssetState *arg0, s16 arg1);
