#pragma once

#include "5520.h"
#include "common.h"
#include "19E80.h"

typedef struct {
    /* 0x00 */ DataTable_19E80 *table;
    u8 _pad[0xC];
    /* 0x10 */ u16 index;
} TableLookupContext;

typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ s16 command;
    /* 0x04 */ u16 spriteFrame;
    /* 0x06 */ u16 duration;
} AnimationEntry;

typedef struct {
    /* 0x00 */ void *entries;
    /* 0x04 */ u16 frameCount;
    /* 0x06 */ u16 initialDelay;
} AnimSetEntry;

typedef struct {
    /* 0x00 */ u32 unk0;
    /* 0x04 */ u16 unk4;
    /* 0x06 */ u16 unk6;
} AnimFrameEntry;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ s16 frameCount;
} AnimationHeader;

typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ AnimationHeader *header;
    /* 0x0C */ AnimationEntry *entries;
    /* 0x10 */ u16 unk10;
    /* 0x12 */ s16 currentSpriteFrame;
    /* 0x14 */ s16 frameIndex;
    /* 0x16 */ s16 frameTimer;
} AnimationState;

typedef struct {
    /* 0x00 */ void *spriteData;
    /* 0x04 */ s16 assetIndex;
    /* 0x06 */ u8 flags;
    /* 0x07 */ u8 initialDelay;
    /* 0x08 */ AnimSetEntry *animSet;
    /* 0x0C */ AnimFrameEntry *frameEntries;
    /* 0x10 */ s16 animIndex;
    /* 0x12 */ s16 currentSpriteFrame;
    /* 0x14 */ s16 frameIndex;
    /* 0x16 */ s16 frameTimer;
    /* 0x18 */ u8 pad18[0x10];
    /* 0x28 */ void *spriteTable;
    /* 0x2C */ u8 pad2C[0x20];
} SpriteAssetState;


typedef struct {
    /* 0x00 */ s8 offsetX;
    /* 0x01 */ s8 offsetY;
    /* 0x02 */ u8 pad2[2];
    /* 0x04 */ u16 textureIndex;
    /* 0x06 */ u8 pad6[2];
} SpriteEntry;

typedef struct {
    /* 0x00 */ u8 pad0[0x0C];
    /* 0x0C */ SpriteEntry *spriteEntries;
    /* 0x10 */ u8 pad10[4];
    /* 0x14 */ s16 currentFrame;
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ void *displayListData;
    /* 0x1C */ s32 positionX;
    /* 0x20 */ s32 positionY;
    /* 0x24 */ s32 positionZ;
    /* 0x28 */ DataTable_19E80 *spriteTable;
    /* 0x2C */ u16 textureIndex;
    /* 0x2E */ u8 alpha;
    /* 0x2F */ u8 flipHorizontal;
    /* 0x30 */ u8 pad30[0x10];
    /* 0x40 */ s32 scaleX;
    /* 0x44 */ s32 scaleY;
    /* 0x48 */ u16 renderMode;
    /* 0x4A */ u16 renderFlags;
} SpriteState;

s32 loadSpriteAsset(SpriteAssetState *state, s16 assetIndex);
void *loadSpriteAssetData(s16 index);
void releaseNodeMemoryRef(void **ptr);
void setSpriteAnimation(void *state, s32 arg1, s32 animIndex, s32 arg3);
s32 updateSpriteAnimation(void *state, s32 arg1);
void renderOpaqueSprite(void *state, s32 slot, s32 posX, s32 posY, s32 posZ, s32 scaleX, s32 scaleY, s16 renderMode, u8 flipH);
void renderSprite(void *state, s32 slot, s32 posX, s32 posY, s32 posZ, s32 scaleX, s32 scaleY, s16 renderMode, u8 flipH, u8 alpha);
s32 getTableEntryValue(TableLookupContext *ctx);