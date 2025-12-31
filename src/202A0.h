#pragma once

#include "common.h"
#include "geometry.h"

typedef struct {
    s32 posX;               // 0x00
    s32 posY;               // 0x04
    s32 posZ;               // 0x08
    s32 targetX;            // 0x0C
    s32 targetY;            // 0x10
    s32 targetZ;            // 0x14
    u8 gameData[0x14];      // 0x18 - embedded GameDataLayout
    void *sceneModel;       // 0x2C
    Transform3D transform;  // 0x30
    u16 currentWaypoint;    // 0x50
    u16 startWaypoint;      // 0x52
    s16 animationPhase;     // 0x54
    u16 currentRotation;    // 0x56
    u8 _pad58[0x2];         // 0x58
    u16 targetRotation;     // 0x5A
    s32 cameraHorzOffset;   // 0x5C
    s32 cameraDistance;     // 0x60
    s32 heightOffset;       // 0x64
    s32 altHeightOffset;    // 0x68
    u8 _pad6C[0x6];         // 0x6C
    u16 frameTimer;         // 0x72
    s16 extraRotation;      // 0x74
    u8 turnDirection;       // 0x76
} LevelPreviewCharacterState;

void initLevelPreviewCharacter(LevelPreviewCharacterState *arg0);
