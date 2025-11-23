#pragma once

#include "19E80.h"
#include "common.h"

typedef struct {
    s32 X;
    s32 Y;
    s32 Z;
} objectPosition;

typedef struct {
    s32 unk0;
    /* 0x4 */ objectPosition position;
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

typedef struct {
    u8 padding[0x1C];
    s32 unk1C;
    u8 padding2[0x10];
    s32 unk30;
} func_80066444_67044_arg1;
void func_800677C0_683C0(s32 arg0, loadAssetMetadata_arg *arg1);

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
