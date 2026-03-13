#include "common.h"
#include "graphics/displaylist.h"
#include "graphics/graphics.h"
#include "math/geometry.h"

typedef struct {
    void *start;
    void *end;
    s32 size;
} Asset_56910;

typedef struct {
    void *start;
    void *end;
} AssetWithoutSize;

typedef struct {
    /* 0x0 */ s32 shortcutPosX;
    /* 0x4 */ s32 shortcutPosZ;
    /* 0x8 */ u16 yawOffset;
    /* 0xA */ u8 padding[2];
    /* 0xC */ Vec3i spawnPos;
    /* 0x18 */ ColorData lightColors;
    /* 0x20 */ ColorData fogColors;
    /* 0x28 */ s16 musicTrack;
    /* 0x2A */ u8 padding2[0x2];
} LevelConfig;

typedef struct {
    u8 padding[0x40];
    DisplayLists finalLapDisplayLists;
    u8 padding2[0x50 - 0x10];
    DisplayLists sceneryDisplayLists1;
    DisplayLists sceneryDisplayLists2;
    DisplayLists sceneryDisplayLists3;
    DisplayLists sceneryDisplayLists4;
} LevelDisplayLists;

LevelDisplayLists *getSkyDisplayLists3ByIndex(s32);

void *loadCourseDataByIndex(s32 index);
void *loadSpriteAssetByIndex(s32 index);
void *loadCompressedSegment2AssetByIndex(s32 index);
void *loadUncompressedAssetByIndex(s32 index);
LevelConfig *getLevelConfig(s32 index);
s32 getSkyDisplayLists1ByIndex(s32 arg0);
s32 getSkyDisplayLists2ByIndex(s32 arg0);

void *loadAsset_B7E70(void);
void *loadAsset_216290(void);
void *loadAsset_34CB50(void);
void *loadAsset_34F9A0(void);
void *loadAsset_350140(void);
void *loadAsset_3505F0(void);
void *loadAsset_34F7E0(void);