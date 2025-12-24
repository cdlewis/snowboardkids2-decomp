#include "common.h"

typedef struct {
    /* 0x0 */ s16 unk0;
    /* 0x2 */ s16 unk2;
    /* 0x4 */ s32 dataOffset;
} TableEntry;

typedef struct {
    /* 0x0 */ s32 entriesOffset;
    /* 0x4 */ s32 countOffset;
} TableHeader;

s32 func_800B0DC0_1DCF60(TableHeader *table) {
    return *(s32 *)(table->countOffset + (s32)table);
}

s16 func_800B0DD0_1DCF70(TableHeader *table, s32 index) {
    TableEntry *entry;
    if (index < func_800B0DC0_1DCF60(table)) {
        entry = (TableEntry *)(table->entriesOffset + (s32)table + index * sizeof(TableEntry));
        return entry->unk0;
    }
    return -1;
}

void *func_800B0E24_1DCFC4(TableHeader *table, s32 index) {
    TableEntry *entry;
    s32 dataOffset;
    if (index < func_800B0DC0_1DCF60(table)) {
        entry = (TableEntry *)(table->entriesOffset + (s32)table + index * sizeof(TableEntry));
        dataOffset = entry->dataOffset;
        if (dataOffset != -1) {
            return (void *)(dataOffset + (s32)table);
        }
    }
    return 0;
}

s16 func_800B0E80_1DD020(TableHeader *table, s32 index) {
    TableEntry *entry;
    if (index < func_800B0DC0_1DCF60(table)) {
        entry = (TableEntry *)(table->entriesOffset + (s32)table + index * sizeof(TableEntry));
        return entry->unk2;
    }
    return -1;
}
