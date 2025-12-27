#pragma once

#include "5EA60.h"
#include "90F0.h"
#include "common.h"
#include "displaylist.h"
#include "memory_allocator.h"

#define MODE_DMA 0
#define MODE_QUEUED_DMA 1
#define MODE_DIRECT_FETCH 2

typedef struct {
    void *displayListStart;
    void *displayListEnd;
    void *vertexDataStart;
    void *vertexDataEnd;
    u16 romBSize;
    u16 padding;
    void *unk14;
} Asset;

typedef struct {
    /* 0x00 */ char name[8];
    /* 0x08 */ void *displayListStart;
    /* 0x0C */ void *displayListEnd;
    /* 0x10 */ void *vertexDataStart;
    /* 0x14 */ void *vertexDataEnd;
    /* 0x18 */ s32 size;
    /* 0x1C */ struct {
        u8 padding[0x10];
    } *unk1C;
    /* 0x20 */ s8 numAssets;
    /* 0x21 */ s8 assetGroupIndex;
    s16 unk22;
    /* 0x24 */ void *asset3Start;
    /* 0x28 */ void *asset3End;
    /* 0x2C */ u32 asset3Size;
    /* 0x30 */ s8 anotherAssetIndex;
    /* 0x31 */ u8 unk31;
    /* 0x34 */ void *soundSequenceDataStart;
    /* 0x38 */ void *soundSequenceDataEnd;
    /* 0x3C */ s32 soundSequenceDataSize;
    /* 0x40 */ void *initCallback;
    /* 0x44 */ Asset *Assets;
    /* 0x48 */ s8 count;
} AssetGroup;

typedef struct {
    u8 padding[0x13];
    s8 animationCount;
} ModelAnimationData;

typedef struct {
    u8 padding[0x89];
    s8 unk89;
    s16 animFrameCount;
    u8 padding3[0x8];
    s8 animationIndex;
    u8 padding2[0x4];
    ModelAnimationData *unk9C;
} func_80001688_2288_arg;

typedef struct {
    s32 unk0;
    u8 padding[0x9];
    s16 unkE;
    struct {
        u8 padding[0x16];
        u16 unk16;
    } *unk10;
    u8 unk14[0x4];
    /* 0x18 */ s16 matrix18[3][3];
    u8 padding2[0x3C - 0x2A];
    s8 isDestroyed;
    s8 actionMode;
    s8 unk3E;
    s8 displayEnabled;
    u8 padding3[0x88 - 0x40];
    s8 unk88;
    u8 padding4[0xF0 - 0x89];
    s16 unkF0[3][3];
    u8 _padUnk102[2];
    s32 unk104;
    s32 unk108;
    s32 unk10C;
} func_80002B50_3750_arg;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    u8 padding[0x14];
    void *unk24;
    void *unk28;
    s32 unk2C;
    u8 padding2[0x3B0];
    s32 unk3E0;
    void *unk3E4;
    void *unk3E8;
    void *unk3EC;
    u8 padding3[0x2D];
    void *unk420;
    void *unk424;
} SceneModel_unk0;

typedef struct {
    u8 padding[0x24];
    void *unk24;
    void *unk28;
} SceneModel_unk98;

typedef struct {
    SceneModel_unk0 *unk0;
    func_8005E800_5F400_arg *unk4;
    void *unk8;
    /* 0xC */ s16 index;
    u8 paddingA[0x06];
    s16 unk14;
    s16 unk16;
    s32 unk18;
    u8 padding[0x10];
    s32 unk2C;
    u8 padding1B[0x8];
    s16 unk38;
    s16 unk3A;
    s8 isDestroyed;
    s8 actionMode;
    s8 unk3E;
    s8 displayEnabled;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s16 unk4C;
    s8 unk4E;
    s8 unk4F;
    u8 padding2[0x38];
    s8 unk88;
    s8 unk89;
    s16 animFrameCount;
    s16 unk8C;
    s16 unk8E;
    s32 unk90;
    s8 animationIndex;
    s8 unk95;
    s8 alpha;
    s8 shadowEnabled;
    SceneModel_unk98 *unk98;
    ModelAnimationData *unk9C;
    u8 unkA0[4];
    SpriteAssetState unkA4;
    s16 unkF0[3][3];
    u8 unkF4[0x8];
    s32 unk10C;
    s32 unk110;
    void *unk114;
    void *unk118;
    void *unk11C;
    void *unk120;
    u8 padding4[0x34];
    s32 height;
    s8 renderEnabled;
} SceneModel;

void *func_800019B8_25B8(s32 arg0, void *arg1, s8 arg2, s8 arg3, s8 arg4, s16 arg5);
void *loadAssetDataByMode(s16 groupIndex, s16 entityIndex, s16 mode);
void *loadAssetGroupSoundData(SceneModel *);
void *loadAssetGroupDisplayList(SceneModel *);
void *loadAssetGroupVertexData(SceneModel *);
void *func_8000198C_258C(s32 arg0, void *arg1);

void func_800021B8_2DB8(SceneModel *arg0, s16 arg1);

void updateModelGeometry(SceneModel *);

void setItemDisplayEnabled(SceneModel *arg0, s8 arg1);
void setItemFlags(SceneModel *arg0, s8 arg1);
void setModelPendingDestroy(SceneModel *arg0);
void clearModelPendingDestroy(SceneModel *arg0);
s8 getAnimationIndex(SceneModel *arg0);
void setModelHeight(SceneModel *arg0, s32 height);

typedef struct {
    u8 transformationMatrix[0x20];
    void *unk20;
    void *asset1;
    void *asset2;
    void *asset3;
    u8 padding2[0xC];
} AssetSlot;

typedef struct {
    AssetSlot *unk00;
    AssetSlot *unk04;
    AssetSlot *unk08;
    s16 unk0C;
    s16 unk0E;
    void *unk10;
    s16 unk14;
    s16 unk16;
    u8 transformationMatrix[0x20];
    s16 unk38;
    s16 unk3A;
    u8 isDestroyed;
    u8 actionMode;
    u8 unk3E;
    u8 displayEnabled;
    u32 unk40;
    s32 unk44;
    u32 unk48;
    s16 unk4C;
    u8 unk4E;
    u8 unk4F;
    u8 padding3[0x38];
    u8 unk88;
    u8 unk89;
    u8 padding4[4];
    s16 unk8E;
    u8 padding5[4];
    s8 animationIndex;
    u8 unk95;
    u8 alpha;
    u8 shadowEnabled;
    AssetSlot *unk98;
    void *unk9C;
    void *unkA0;
    SpriteAssetState unkA4;
    u8 asset2TransformationMatrix[0x20];
    u32 unk110;
    void *unk114;
    void *unk118;
    void *soundData;
    void *unk120;
    void *unk124;
    u8 padding6[0xC];
    void *unk134;
    s16 unk138;
    u8 unk13A;
    u8 unk13B;
    u8 padding7[0x18];
    s16 unk154;
    s16 unk156;
    u32 height;
    u8 renderEnabled;
} GameEntity;

typedef struct {
    Mat3x3Padded unk0;
    union {
        SceneModel *unk20;
        s32 unk20_s32;
        s16 unk20_s16;
    } unk20_u;
} applyTransformToModel_arg1;

void applyTransformToModel(SceneModel *arg0, Mat3x3Padded *arg1);

void initializeGameEntity(void *, s32, void *, s8, s8, s8, s16);

SceneModel *func_80002014_2C14(SceneModel *arg0);
void *func_80002040_2C40(SceneModel *arg0);

s32 clearModelRotation(SceneModel *);

void setModelDisplayEnabled(SceneModel *arg0, s8 arg1);

void setModelVisibility(SceneModel *, s8);

s32 setModelRotation(SceneModel *, s16);

void setAnimationIndex(SceneModel *arg0, s8 arg1);
s8 getModelAnimationCount(SceneModel *arg0);

void disableEntityRendering(GameEntity *arg0);
void enableEntityRendering(SceneModel *arg0);
void enableModelShadow(SceneModel *arg0);
void disableModelShadow(SceneModel *arg0);

void func_800021E8_2DE8(SceneModel *arg0, s16 arg1, s16 arg2, s8 arg3);

void func_80002220_2E20(SceneModel *arg0, s16 arg1, s16 arg2, s8 arg3, s16 arg4);

void func_80002260_2E60(SceneModel *arg0, s16 arg1, s16 arg2, s8 arg3, short arg4, s8 arg5);

void setModelActionMode(SceneModel *model, s8 actionMode);

void func_800016E0_22E0(SceneModel *arg0, s32 arg1);

void func_800016F8_22F8(SceneModel *arg0, s32 arg1);

s32 getWalkAnimationId(s16);

s32 getRunAnimationId(s16);

u8 getModelAlpha(SceneModel *arg0);

void setModelAlpha(SceneModel *arg0, u8 arg1);

void func_80002B50_3750(func_80002B50_3750_arg *arg0, DisplayListObject *arg1);

s32 func_80001904_2504(s16);
