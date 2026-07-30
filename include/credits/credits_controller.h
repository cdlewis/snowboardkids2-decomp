#pragma once

#include "math/geometry.h"
#include "ui/level_preview_3d.h"

typedef struct {
    /* 0x00 */ s16 modelId;
    /* 0x02 */ s16 animationIndex;
    /* 0x04 */ u8 reserved04;
    /* 0x05 */ s8 actionMode;
    /* 0x06 */ u16 initialYaw;
    /* 0x08 */ u16 reserved08;
    /* 0x0A */ s16 scale;
    /* 0x0C */ s32 verticalOffset;
} CreditsCharacterConfig;

typedef union {
    s16 selection;
    struct {
        u8 padding;
        s8 index;
    } parts;
} CreditsAssetPair;

typedef struct {
    /* 0x00 */ SceneModel *model;
    /* 0x04 */ s16 animationPhase;
    /* 0x06 */ s16 configIndex;
    /* 0x08 */ CreditsAssetPair assetPair;
    /* 0x0A */ s16 modelCleanedUp;
} CreditsCharacter;

typedef struct {
    /* 0x00 */ s32 depth;
    /* 0x04 */ s16 reserved04;
    /* 0x06 */ s16 pitch;
    /* 0x08 */ s16 reserved08;
    /* 0x0A */ s16 yaw;
} CreditsCameraConfig;

typedef struct {
    /* 0x00 */ u16 configCount;
    /* 0x02 */ u16 reserved02;
} CreditsCharacterConfigHeader;

extern Vec3i D_8008C120_8CD20[];
