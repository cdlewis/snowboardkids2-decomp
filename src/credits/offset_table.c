#include "credits/offset_table.h"

s32 getOffsetTableEntryCount(OffsetTable *table) {
    return *(s32 *)(table->countOffset + (s32)table);
}

s16 getOffsetTableEntryValue0(OffsetTable *table, s32 index) {
    OffsetTableEntry *entry;
    if (index < getOffsetTableEntryCount(table)) {
        entry = (OffsetTableEntry *)(table->entriesOffset + (s32)table + index * sizeof(OffsetTableEntry));
        return entry->value0;
    }
    return -1;
}

void *getOffsetTableEntryData(OffsetTable *table, s32 index) {
    OffsetTableEntry *entry;
    s32 dataOffset;
    if (index < getOffsetTableEntryCount(table)) {
        entry = (OffsetTableEntry *)(table->entriesOffset + (s32)table + index * sizeof(OffsetTableEntry));
        dataOffset = entry->dataOffset;
        if (dataOffset != -1) {
            return (void *)(dataOffset + (s32)table);
        }
    }
    return 0;
}

s16 getOffsetTableEntryValue2(OffsetTable *table, s32 index) {
    OffsetTableEntry *entry;
    if (index < getOffsetTableEntryCount(table)) {
        entry = (OffsetTableEntry *)(table->entriesOffset + (s32)table + index * sizeof(OffsetTableEntry));
        return entry->value2;
    }
    return -1;
}
