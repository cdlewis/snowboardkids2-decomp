#pragma once

#include "common.h"
#include "level_preview_3d.h"
#include "math/geometry.h"

typedef struct {
    u8 _pad0[0x2C];
    void *textureData;
} TextureDataTaskState;

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ void *spriteData;
    /* 0x08 */ s16 frameIndex;
    /* 0x0A */ s16 scale;
    /* 0x0C */ u8 flags;
    /* 0x0D */ u8 alpha;
    /* 0x0E */ u8 _pad[2];
} CharacterSelectSprite;

typedef struct {
    CharacterSelectSprite sprites[4];
    u8 animTimers[8];
} CharacterSelectSprites;

typedef struct {
    /* 0x00 */ s16 m[9];
    /* 0x12 */ s16 _pad;
    /* 0x14 */ s32 translateX;
    /* 0x18 */ s32 translateY;
    /* 0x1C */ s32 translateZ;
    /* 0x20 */ void *displayList;
    /* 0x24 */ void *textureData1;
    /* 0x28 */ void *textureData2;
    /* 0x2C */ s32 animState;
} CharacterSelectBoardTask;

typedef struct {
    SceneModel *model;
    Transform3D matrix;
    u16 characterIndex;
    s16 animationState;
    u8 displayMode;
    u8 timer;
} CharacterPreviewState;

typedef struct {
    s16 xOffset;
    s16 yOffset;
    void *spriteData;
    s16 frameIndex;
} CharacterSelectIndicatorTask;

void initCharacterSelectTextureDataLoad(TextureDataTaskState *arg0);
void initCharacterSelectSprites(CharacterSelectSprites *arg0);
void initCharacterSelectBoardTask(CharacterSelectBoardTask *arg0);
void initCharacterPreview(CharacterPreviewState *arg0);
void initPlayer3CharacterSelectIndicator(CharacterSelectIndicatorTask *arg0);
void initPlayer2CharacterSelectIndicator(CharacterSelectIndicatorTask *arg0);