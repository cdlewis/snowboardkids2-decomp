#pragma once

#include "common.h"
#include "data/data_table.h"
#include "graphics/displaylist.h"
#include "math/geometry.h"

typedef struct {
    s16 timer;
} FireworkShowTimerState;

typedef struct {
    /* 0x00 */ DisplayListObject building1;
    /* 0x3C */ DisplayListObject building2;
} StarlightBuildingTaskState;

typedef struct {
    /* 0x00 */ DisplayListObject item;
    /* 0x3C */ DisplayListObject shadow;
    /* 0x78 */ s16 scale;
    /* 0x7A */ u16 rotation;
    /* 0x7C */ u8 positionIndex;
} StarlightItemTaskState;

typedef struct {
    /* 0x00 */ DisplayListObject displayObject;
    /* 0x3C */ Vec3i velocity;
    /* 0x48 */ s16 lifetime;
    /* 0x4A */ u16 rotationX;
    /* 0x4C */ u16 rotationY;
    /* 0x4E */ u8 type;
} StarlightFireworkTaskState;

typedef struct {
    /* 0x00 */ DisplayListObject leftGate;
    /* 0x3C */ DisplayListObject rightGate;
    /* 0x78 */ s32 openAmount;
    /* 0x7C */ u8 barrierIndex;
} StarlightBarrierTaskState;

typedef union {
    s16 timer;
    struct {
        u8 _pad;
        u8 alpha;
    } color;
} StarlightDebugFade;

typedef struct {
    /* 0x00 */ DisplayListObject displayObject;
    /* 0x3C */ DataTable_19E80 *textureTable;
    /* 0x40 */ u8 _pad40[0x4];
    /* 0x44 */ u16 textureUOffset;
    /* 0x46 */ u16 textureVOffset;
    /* 0x48 */ u16 textureUSpeed;
    /* 0x4A */ u16 textureVSpeed;
    /* 0x4C */ u16 textureIndex;
    /* 0x4E */ u8 _pad4E[0x2];
    /* 0x50 */ StarlightDebugFade fade;
    /* 0x52 */ s16 sustainTimer;
} StarlightDebugDisplayTaskState;

void spawnDebugDisplayListTask(s16 arg0);
void spawnStarlightItems(void);
void spawnStarlightBarriers(void);
void initFireworkShowTimer(FireworkShowTimerState *arg0);
void updateFireworkShowTimer(FireworkShowTimerState *arg0);
void initStarlightHighwayBuildingTask(StarlightBuildingTaskState *task);
void initStarlightItemTask(StarlightItemTaskState *);
void cleanupStarlightItemTask(DisplayListObject *);
void updateStarlightItemTask(StarlightItemTaskState *);
