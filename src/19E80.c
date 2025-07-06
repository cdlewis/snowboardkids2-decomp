#include "19E80.h"

void getTableEntryByU16Index(DataTable_19E80 *arg0, u16 arg1, OutputStruct_19E80 *arg2) {
    u16 index_offset;
    TableEntry_19E80 *entry_ptr;
    TableEntry_19E80 *index_table;
    TableEntry_19E80 *entry;

    entry_ptr = arg0->entries;
    index_table = &entry_ptr[arg0->index_table_offset];
    entry_ptr = &entry_ptr[arg1];

    arg2->data_ptr = &arg0->header[entry_ptr->data_offset];
    arg2->index_ptr = &index_table[entry_ptr->index_offset * 2];
    arg2->field1 = entry_ptr->field1;
    arg2->field2 = entry_ptr->field2;
}

void getTableEntryByIndex(DataTable_19E80 *table, u32 entry_index, u8 sub_index, OutputStruct_19E80 *output) {
    TableEntry_19E80 *entry_base;
    TableEntry_19E80 *index_ptr;

    entry_base = table->entries;

    index_ptr = entry_base + table->index_table_offset;
    entry_base += (u16)entry_index;

    output->data_ptr = (void *)table + entry_base->data_offset;
    output->index_ptr = index_ptr + (sub_index << 1);
    output->field1 = entry_base->field1;
    output->field2 = entry_base->field2;
}