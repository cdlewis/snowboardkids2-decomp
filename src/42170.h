#pragma once

#include "common.h"
#include "gamestate.h"
#include "geometry.h"

typedef struct {
    u8 pad0[0x14];    /* 0x00 */
    Vec3i position;   /* 0x14 */
    void *displayData;/* 0x20 */
    void *asset1;     /* 0x24 */
    void *asset2;     /* 0x28 */
    s32 unk2C;        /* 0x2C */
    u8 pad30[0xB];    /* 0x30 */
    u8 alpha;         /* 0x3B */
    u8 pad3C[0x4];    /* 0x3C */
    s16 scale;        /* 0x40 */
    u16 rotation;     /* 0x42 */
    s16 opacity;      /* 0x44 */
    s16 playerIndex;  /* 0x46 */
} SparkleEffectState;

typedef struct {
    u8 _pad0[0x24];       /* 0x00 */
    void *unk24;          /* 0x24 */
    void *player;         /* 0x28 */
    u8 _pad2C[0xE];       /* 0x2C */
    s16 frameTimer;       /* 0x3A */
    u8 _pad3C[0x7];       /* 0x3C */
    u8 immediateMode;     /* 0x43 */
} StarEffectTask;

SparkleEffectState *spawnSparkleEffect(void *arg0);
SparkleEffectState *spawnSparkleEffectWithPlayer(void *arg0, s32 arg1);
StarEffectTask *spawnStarEffectImmediate(void *arg0);
void scheduleShieldEffect(void *arg0);
void *createFallingEffect(void *arg0);
void *spawnCrashEffect(void *arg0);
void *func_80043310_43F10(Player *arg0);
void *func_80044050_44C50(Player *arg0);
void *createLiftEffect(Player *player);
void spawnBurstEffect(Vec3i *position);
