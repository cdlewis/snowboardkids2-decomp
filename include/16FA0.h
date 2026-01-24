#pragma once

#include "20F0.h"
#include "common.h"
#include "main.h"

typedef struct {
    u8 padding[0x2];
    s16 unk2;
    u8 padding2[0x34];
    void *assetData;
} TitleLogoTask;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    u8 unkA;
    u8 unkB;
    u8 unkC;
    u8 unkD;
    u8 unkE;
} Struct16728;

typedef struct {
    SceneModel *sceneModel;
    Transform3D transform;
    s32 yVelocity;
    u16 *animSequencePtr;
    u16 currentAnim;
    u8 animSequenceIndex;
    u8 characterIndex;
    u8 _pad30;
    u8 animTimer;
    u8 flyAwayState;
} TitleCharacterState;

void loadTitleLogoAsset(TitleLogoTask *arg0);
void func_80016488_17088(void);
void func_80016728_17328(Struct16728 *arg0);
void initTitleEffectModel(ModelEntity *arg0);
void initTitleCharacterModel(TitleCharacterState *arg0);