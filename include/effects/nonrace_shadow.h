#pragma once

#include "math/geometry.h"

typedef struct {
    u8 pad0[0x14];
    s32 position[3];
} ModelPositionData;

typedef struct {
    ModelPositionData *modelData;
    u8 pad04[0x14];
    s16 rotationMatrix[3][3];
    u8 pad2A[2];
    Vec3i position;
    u8 pad38[0x17];
    s8 shadowRadius;
    u8 pad50[0x30];
    Vtx *shadowVertices;
    Mtx *shadowMatrix;
    u8 pad88[0xE];
    u8 alpha;
} ShadowEntity;

extern Gfx g_NonRaceShadowDL[];

void renderNonRaceShadow(ShadowEntity *entity);