#include "19E80.h"
#include "common.h"

typedef struct {
    DataTable_19E80 *unk0;
    void *unk4;
    void *unk8;
} C870Struct;

void func_8000BC70_C870(C870Struct *arg0, DataTable_19E80 *arg1) {
    OutputStruct_19E80 output;

    getTableEntryByU16Index(arg1, 0, &output);
    arg0->unk0 = arg1;
    arg0->unk4 = (u8 *)output.index_ptr + 0x200;
    arg0->unk8 = output.index_ptr;
}

void func_8000BCC4_C8C4(C870Struct *arg0) {
    memcpy(arg0->unk4, arg0->unk8, 0x200);
}

INCLUDE_ASM("asm/nonmatchings/C870", func_8000BCEC_C8EC);
