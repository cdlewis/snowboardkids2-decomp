#ifndef _19E80_H_
#define _19E80_H_

#include "common.h"

typedef struct {
    u32 data_offset;
    u16 index_offset;
    u16 field1;
    u16 field2;
    u8 padding[6];
} TableEntry_19E80;

typedef struct {
    u8 header[4];
    u32 index_table_offset;
    TableEntry_19E80 entries[1];
} DataTable_19E80;

typedef struct {
    /* 0x00 */ u8 *data_ptr;
    /* 0x04 */ TableEntry_19E80 *index_ptr;
    /* 0x08 */ u16 field1;
    /* 0x10 */ u16 field2;
} OutputStruct_19E80;

void func_80019280_19E80(DataTable_19E80 *, u16, OutputStruct_19E80 *);

void func_800192CC_19ECC(DataTable_19E80 *table, u32 entry_index, u8 sub_index, OutputStruct_19E80 *output);

#endif