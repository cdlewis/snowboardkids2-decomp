#pragma once

#include "common.h"
#include "cutscene/cutscene_manager.h"
#include "cutscene/sys_effect.h"
#include "graphics/sprite_table.h"

typedef struct {
    /* 0x00 */ SceneModel *parentModel;
    /* 0x04 */ s16 layer;
    /* 0x06 */ s16 animId;
    /* 0x08 */ s16 duration;
    /* 0x0A */ u8 opacity;
    /* 0x0B */ u8 state;
    /* 0x0C */ s32 targetScale;
    /* 0x10 */ s32 offsetX;
    /* 0x14 */ s32 offsetY;
    /* 0x18 */ s32 offsetZ;
    /* 0x1C */ u16 rotation;
    /* 0x1E */ u16 useParentPos;
    /* 0x20 */ SpriteAssetState spriteState;
    /* 0x6C */ s32 effectScratch0;
    /* 0x70 */ s32 effectScratch1;
} SpriteEffectTaskState;

typedef struct {
    s32 array_offset;
    s32 count_offset;
} Table_B934;

s32 spawnSpriteEffectInternal(
    SceneModel *arg0,
    s16 arg1,
    s16 arg2,
    s16 arg3,
    cutsceneEffectDisp_exec_arg *arg4,
    s32 arg5,
    s8 arg6,
    u8 arg7,
    u8 arg8,
    s16 arg9,
    s16 arg10
);
void *loadDmaAsset(s16 arg0);
void *getTable2DEntry(Table_B934 *table, s32 rowIndex, s32 colIndex);
s16 getTable2DRowCount(Table_B934 *table, s32 rowIndex, s32 colIndex);
s16 getMaxLinePixelWidth(u16 *encodedText);
void spawnSpriteEffectEx(SceneModel *, s16, s16, s16, void *, s32, s8, u8, u8, s16);
