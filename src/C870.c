#include "C870.h"
#include "19E80.h"
#include "common.h"

typedef struct {
    /* 0x000 */ u8 buffer1[0x200];
    /* 0x200 */ u8 buffer2[0x200];
} IndexBuffer;

void func_8000BC70_C870(C870Struct *arg0, DataTable_19E80 *arg1) {
    OutputStruct_19E80 output;

    getTableEntryByU16Index(arg1, 0, &output);
    arg0->unk0 = arg1;
    arg0->unk4 = ((IndexBuffer *)output.index_ptr)->buffer2;
    arg0->unk8 = ((IndexBuffer *)output.index_ptr)->buffer1;
}

void func_8000BCC4_C8C4(C870Struct *arg0) {
    memcpy(arg0->unk4, arg0->unk8, 0x200);
}

INCLUDE_ASM("asm/nonmatchings/C870", func_8000BCEC_C8EC);
