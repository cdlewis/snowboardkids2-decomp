#include "5EA60.h"
#include "common.h"
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
    s32 unk0;
    u8 padding[0x9];
    s16 unkE;
    struct {
        u8 padding[0x16];
        u16 unk16;
    } *unk10;
    u8 padding2[0x2B];
    s8 unk3F;
    u8 padding3[0x48];
    s8 unk88;
} func_80002B50_3750_arg;

typedef struct {
    u8 padding[0x24];
    void *unk24;
    void *unk28;
    u8 padding2[0x3B4];
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
    u8 padding[0x1B];
    s16 unk38;
    s16 unk3A;
    s8 unk3C;
    s8 unk3D;
    s8 unk3E;
    s8 unk3F;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s16 unk4C;
    s8 unk4E;
    u8 padding2[0x39];
    s8 unk88;
    s8 unk89;
    s16 unk8A;
    s16 unk8C;
    s16 unk8E;
    s32 unk90;
    s8 unk94;
    s8 unk95;
    SceneModel_unk98 *unk98;
    u8 padding3[0x78];
    void *unk114;
    void *unk118;
    void *unk11C;
    void *unk120;
} SceneModel;

void *func_800019B8_25B8(s32 arg0, void *arg1, s8 arg2, s8 arg3, s8 arg4, s16 arg5);
void *loadAssetDataByMode(s16 groupIndex, s16 entityIndex, s16 mode);
void *loadAssetGroupSoundData(SceneModel *);
void *loadAssetGroupDisplayList(SceneModel *);
void *loadAssetGroupVertexData(SceneModel *);
void *func_8000198C_258C(s32 arg0, void *arg1);

void func_800021B8_2DB8(SceneModel *arg0, s16 arg1);

void updateModelGeometry(SceneModel *);

void func_8000160C_220C(SceneModel *arg0, s8 arg1);

typedef struct {
    u8 padding[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    SceneModel *unk20;
} applyTransformToModel_arg1;

void applyTransformToModel(SceneModel *arg0, applyTransformToModel_arg1 *arg1);

SceneModel *func_80002014_2C14(SceneModel *arg0);

void clearModelRotation(SceneModel *);

typedef struct {
    u8 padding[0xC];
    s16 unkC;
    u8 padding2[0x8];
    s32 unk18;
    u8 padding3[0x20];
    s8 unk3C;
    s8 unk3D;
    s8 unk3E;
    s8 unk3F;
} func_80001604_2204_arg;

void func_80001604_2204(func_80001604_2204_arg *arg0, s8 arg1);

void setModelVisibility(SceneModel *, s8);

s32 setModelRotation(SceneModel *, s16);
