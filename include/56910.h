#include "common.h"
#include "displaylist.h"
#include "geometry.h"
#include "graphics.h"

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
    s32 shortcutPosX;
    s32 shortcutPosZ;
    u16 yawOffset;
    u8 padding[2];
    Vec3i spawnPos;        /* 0x0C - Shortcut spawn/warp position */
    ColorData lightColors; /* 0x18 - Viewport light colors */
    ColorData fogColors;   /* 0x20 - Fog colors */
    s16 musicTrack;        /* 0x28 - Music track for this level */
    u8 padding2[0x2];
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