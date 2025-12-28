#pragma once

#include "1DFAA0.h"
#include "common.h"

typedef struct {
    DisplayListObject unk0;
    DisplayListObject unk3C;
    void **unk78;
    s32 unk7C;
    u16 unk80;
    s16 unk82;
    s16 unk84;
} RocketEffectUpdateData;

typedef struct {
    DisplayListObject unk0;
    u8 padding[0x7C - 0x3C];
    s32 unk7C;
} RocketEffectFallingData;

void updateRocketEffectFalling(RocketEffectFallingData *arg0);
void spawnRocketEffect(SceneModel *, s16);
void spawnRocketEffectEx(SceneModel *sceneModel, s16 displayDuration, u8 nodeType, u8 identifierFlag, u8 priority);
void updateRocketEffect(RocketEffectUpdateData *arg0);
