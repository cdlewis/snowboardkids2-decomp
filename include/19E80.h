#ifndef _19E80_H_
#define _19E80_H_

#include "common.h"

typedef struct {
    /* 0x00 */ u32 data_offset;
    /* 0x04 */ u16 index_offset;
    /* 0x06 */ u16 field1;
    /* 0x08 */ u16 field2;
    u8 padding[6];
} TableEntry_19E80;

typedef struct {
    /* 0x00 */ u8 header[4];
    /* 0x04 */ u32 index_table_offset;
    /* 0x08 */ TableEntry_19E80 entries[1];
} DataTable_19E80;

typedef struct {
    /* 0x00 */ u8 *data_ptr;
    /* 0x04 */ TableEntry_19E80 *index_ptr;
    /* 0x08 */ u16 field1;
    /* 0x10 */ u16 field2;
} OutputStruct_19E80;

void getTableEntryByU16Index(DataTable_19E80 *, u16, OutputStruct_19E80 *);

void getTableEntryByIndex(DataTable_19E80 *table, u32 entry_index, u8 sub_index, OutputStruct_19E80 *output);

#endif