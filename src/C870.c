#include "common.h"

typedef struct {
    s32 unk0;
    void *unk4;
    void *unk8;
} C870Struct;

INCLUDE_ASM("asm/nonmatchings/C870", func_8000BC70_C870);

void func_8000BCC4_C8C4(C870Struct *arg0) {
    memcpy(arg0->unk4, arg0->unk8, 0x200);
}

INCLUDE_ASM("asm/nonmatchings/C870", func_8000BCEC_C8EC);
