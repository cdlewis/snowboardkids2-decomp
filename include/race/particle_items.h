#pragma once

#include "common.h"
#include "gamestate.h"
#include "math/geometry.h"

typedef struct {
    /* 0x00 */ Transform3D transform;
    /* 0x20 */ void *displayData;
    /* 0x24 */ void *asset1;
    /* 0x28 */ void *asset2;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u8 pad30[0xB];
    /* 0x3B */ u8 alpha;
    /* 0x3C */ u8 pad3C[0x4];
    /* 0x40 */ s16 scale;
    /* 0x42 */ u16 rotation;
    /* 0x44 */ s16 opacity;
    /* 0x46 */ s16 playerIndex;
} SparkleEffectState;

typedef struct {
    /* 0x00 */ u8 _pad0[0x24];
    /* 0x24 */ void *unk24;
    /* 0x28 */ void *player;
    /* 0x2C */ u8 _pad2C[0xE];
    /* 0x3A */ s16 startDelay;
    /* 0x3C */ u8 _pad3C[0x7];
    /* 0x43 */ u8 immediateMode;
} StarEffectTask;

SparkleEffectState *spawnSparkleEffect(void *arg0);
SparkleEffectState *spawnSparkleEffectWithPlayer(void *arg0, s32 arg1);
StarEffectTask *spawnStarEffectImmediate(void *arg0);
void scheduleShieldEffect(void *arg0);
void *createFallingEffect(void *arg0);
void *spawnCrashEffect(void *arg0);
void *spawnPlayerAuraEffect(Player *arg0);
void *spawnGhostEffect(Player *arg0);
void *createLiftEffect(Player *player);
void spawnBurstEffect(Vec3i *position);
