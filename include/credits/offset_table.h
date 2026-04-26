#pragma once

#include "common.h"

typedef struct {
    /* 0x0 */ s16 value0;
    /* 0x2 */ s16 value2;
    /* 0x4 */ s32 dataOffset;
} OffsetTableEntry;

typedef struct {
    /* 0x0 */ s32 entriesOffset;
    /* 0x4 */ s32 countOffset;
} OffsetTable;

s32 getOffsetTableEntryCount(OffsetTable *table);
s16 getOffsetTableEntryValue0(OffsetTable *table, s32 index);
void *getOffsetTableEntryData(OffsetTable *table, s32 index);
s16 getOffsetTableEntryValue2(OffsetTable *table, s32 index);
