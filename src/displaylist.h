#include "common.h"

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 padding[0x18];
    struct
    {
        s32 unk0;
        s32 unk4;
        s32 unk8;
        s32 unkC;
    }* unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    u8 padding3[0x7];
    u8 unk3B;
    s32 unk3C;
    u8 padding4[0x10];
    u32 unk50;
    u8 padding5[0xC];
    u8 unk60;
} DisplayListObject;

void enqueueDisplayListObject(s32 arg0, DisplayListObject* arg1);

typedef struct {
    u16* dataStart;
    u16* section1Data;
    u16* section2Data;
    u16* section3Data;
    u16 finalValue;
} GameDataLayout;

void parseGameDataLayout(GameDataLayout* gameData);

void initializeOverlaySystem();

typedef struct {
    u8 padding[0x10];
    s32 unk10;
    s32 unk14;
    s8 unk18;
    s8 unk19;
    s8 unk1A;
} loadAssetMetadata_arg;
void loadAssetMetadata(loadAssetMetadata_arg*, void*, s32);

void enqueueDisplayListWithFrustumCull(s32, DisplayListObject*);

void buildDisplayListSegment(DisplayListObject*);