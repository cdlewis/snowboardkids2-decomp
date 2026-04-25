#pragma once

#include "cutscene/cutscene_manager.h"
#include "common.h"
#include "ui/level_preview_3d.h"

typedef struct {
    DisplayListObject body;
    DisplayListObject flame;
    SceneModel *sceneModel;
    s32 fallVelocity;
    u16 yAngle;
    s16 scale;
    s16 displayDuration;
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
