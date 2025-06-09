#include "common.h"
#include "memory_allocator.h"

typedef struct {
    u8 padding[0x24];
    void* unk24;
    void* unk28;
    u8 padding2[0x3B4];
    s32 unk3E0;
    void* unk3E4;
    void* unk3E8;
    void* unk3EC;
    u8 padding3[0x2D];
    void* unk420;
    void* unk424;
} func_80002040_2C40_arg_unk0;

typedef struct
{
    s32 unk0;
    u8 padding[0x48];
} func_80002040_2C40_arg_unk4;

typedef struct {
    u8 padding[0x24];
    void* unk24;
    void* unk28;
} func_80002040_2C40_arg_unk98;

typedef struct {
    func_80002040_2C40_arg_unk0* unk0;
    func_80002040_2C40_arg_unk4* unk4;
    void* unk8;
    /* 0xC */ s16 index;
    u8 paddingA[0x06];
    s16 unk14;
    s16 unk16;
    u8 padding[0x1F];
    s16 unk38;
    s16 unk3A;
    s8 unk3C;
    s8 unk3D;
    s8 unk3E;
    u8 padding2[0x4A];
    s8 unk89;
    s16 unk8A;
    s16 unk8C;
    s16 unk8E;
    s32 unk90;
    s8 unk94;
    s8 unk95;
    func_80002040_2C40_arg_unk98* unk98;
    u8 padding3[0x78];
    s32 unk114;
    s32 unk118;
    void* unk11C;
    void* unk120;
} func_80002040_2C40_arg;

typedef struct {
    void* displayListStart;
    void* displayListEnd;
    void* vertexDataStart;
    void* vertexDataEnd;
    u16 romBSize;
    u16 padding;
    void* unk14;
} Asset;

typedef struct {
    /* 0x00 */ char name[8];
    /* 0x08 */ void* displayListStart;
    /* 0x0C */ void* displayListEnd;
    /* 0x10 */ void* vertexDataStart;
    /* 0x14 */ void* vertexDataEnd;
    /* 0x18 */ s32 size;
    u8 padding3[0x4];
    s8 unk20;
    s8 unk21;
    s16 unk22;
    u8 padding[0x10];
    /* 0x34 */ void* soundSequenceDataStart;
    /* 0x38 */ void* soundSequenceDataEnd;
    /* 0x3C */ s32 soundSequenceDataSize;
    /* 0x40 */ void* initCallback;
    /* 0x44 */ Asset* Assets;
    s8 count;
} AssetGroup;

typedef struct {
    s32 unk0;
    u8 padding[0x9];
    s16 unkE;
    struct {
        u8 padding[0x16];
        u16 unk16;
    }* unk10;
    u8 padding2[0x2B];
    s8 unk3F;
    u8 padding3[0x48];
    s8 unk88;
} func_80002B50_3750_arg;

void* func_800019B8_25B8(void* arg0, void* arg1, s8 arg2, s8 arg3, s8 arg4, s16 arg5);
MemoryAllocatorNode* loadAssetDataByMode(s16 groupIndex, s16 entityIndex, s16 mode);
MemoryAllocatorNode* loadAssetGroupSoundData(func_80002040_2C40_arg*);
MemoryAllocatorNode* loadAssetGroupDisplayList(func_80002040_2C40_arg*);
MemoryAllocatorNode* loadAssetGroupVertexData(func_80002040_2C40_arg*);