#pragma once

#include "19E80.h"
#include "common.h"
#include "geometry.h"

typedef struct {
    s32 unk0;
    /* 0x4 */ Vec3s32 position;
} DisplayListObject_unk10;

typedef struct {
    u32 flags;
    Gfx *opaqueDisplayList;
    Gfx *transparentDisplayList;
    Gfx *overlayDisplayList;
} DisplayLists;

typedef struct {
    DisplayLists unk0;
    DisplayListObject_unk10 unk10;
    DisplayLists *unk20;
    void *unk24;
    void *unk28;
    s32 unk2C;
    s32 unk30;
    u8 padding3[0x3];
    u8 unk37;
    u8 padding4[0x3];
    u8 unk3B;
} DisplayListObject;

void enqueueDisplayListObject(s32 arg0, DisplayListObject *arg1);

typedef struct {
    u16 *dataStart;
    u16 *section1Data;
    u16 *section2Data;
    u16 *section3Data;
    u16 finalValue;
} GameDataLayout;

void parseGameDataLayout(GameDataLayout *gameData);

void initializeOverlaySystem(void);

typedef struct loadAssetMetadata_arg {
    struct loadAssetMetadata_arg *unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    /* 0x10 */ u8 *data_ptr;
    /* 0x14 */ TableEntry_19E80 *index_ptr;
    s8 unk18;
    s8 unk19;
    u8 unk1A;
} loadAssetMetadata_arg;

void loadAssetMetadata(loadAssetMetadata_arg *, void *, s32);

void enqueueDisplayListWithFrustumCull(s32, DisplayListObject *);

void buildDisplayListSegment(DisplayListObject *);

void func_800638C0_644C0(DisplayListObject *);

void func_80063580_64180(s32);

void func_80063534_64134(s32);

void func_800634E8_640E8(DisplayListObject *arg0);

void func_800630F0_63CF0(s32, void *);

void func_8006300C_63C0C(DisplayListObject *arg0);

void func_80063058_63C58(DisplayListObject *arg0);

void func_800630A4_63CA4(DisplayListObject *arg0);

typedef struct {
    s32 unk0;
    Vec3s32 unk4;
    u8 padding[0xC];
    s32 unk1C;
    u8 padding2[0x4];
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s16 unk30;
    s16 unk36;
} func_80066444_67044_arg1;

void func_80066444_67044(s32 arg0, func_80066444_67044_arg1 *arg1);
void func_800677C0_683C0(s32 arg0, loadAssetMetadata_arg *arg1);

void func_800670A4_67CA4(u16 arg0, func_80066444_67044_arg1 *arg1);

typedef struct {
    u8 padding0[4];
    s32 unk4;
    s32 unk8;
    s32 unkC;
    /* 0x10 */ u8 *data_ptr;
    /* 0x14 */ TableEntry_19E80 *index_ptr;
    s8 unk18;
    u8 unk19;
    u8 unk1A;
} loadAssetMetadataByIndex_arg;

void loadAssetMetadataByIndex(
    loadAssetMetadataByIndex_arg *arg0,
    DataTable_19E80 *table,
    s32 entry_index,
    s32 sub_index
);

void func_80067EDC_68ADC(s32, loadAssetMetadata_arg *);

void func_8006395C_6455C(DisplayListObject *arg0);

void func_800639F8_645F8(s32 arg0, DisplayListObject *arg1);

void func_80065DA8_669A8(s32 arg0, DisplayListObject *arg1);

u16 func_800625A4_631A4(void *arg0, void *arg1);

u16 func_80060A3C_6163C(void *arg0, u16 arg1, void *arg2);

void func_80060CDC_618DC(void *arg0, u16 arg1, void *arg2, s32 arg3, s32 *arg4);

s32 func_80061A64_62664(void *arg0, u16 arg1, void *arg2);

s32 func_80061D6C_6296C(void *arg0, u16 arg1, void *arg2, s32 arg3);

s32 func_80062274_62E74(void *arg0, u16 arg1);

void func_80062918_63518(void *arg0, u16 arg1, void *arg2, void *arg3, void *arg4);

void func_800650B4_65CB4(u16 arg0, DisplayListObject *arg1);

u16 func_80062B1C_6371C(void *arg0, u16 arg1, void *arg2, void *arg3);

typedef struct {
    s16 unk0;
    u8 _pad[0x8];
    s16 unkA;
    u8 _pad2[0x18];
} func_80062C98_63898_arg;

s16 func_80062254_62E54(GameDataLayout *arg0, u16 arg1);
