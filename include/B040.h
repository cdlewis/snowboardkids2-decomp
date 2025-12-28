#pragma once

#include "1DFAA0.h"
#include "common.h"
#include "cutscene/sys_effect.h"

typedef struct {
    SceneModel *unk0;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    u8 unkA;
    u8 unkB;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u16 unk1C;
    u16 unk1E;
    u8 pad20[0x6C - 0x20];
    s32 unk6C;
    s32 unk70;
} ScheduledTask;

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
s16 getTable2DRowCount(Table_B934 *table, s32 rowIndex);
s16 getMaxLinePixelWidth(u16 *encodedText);
void spawnSpriteEffectEx(SceneModel *, s16, s16, s16, void *, s32, s8, u8, u8, s16);
void spawnSpriteEffectEx(
    SceneModel *arg0,
    s16 arg1,
    s16 arg2,
    s16 arg3,
    void *arg4,
    s32 arg5,
    s8 arg6,
    u8 arg7,
    u8 arg8,
    s16 arg9
);