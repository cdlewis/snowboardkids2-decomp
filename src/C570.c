#include "common.h"

extern void *freeNodeMemory(void *);

typedef struct {
    u8 _pad[0x24];
    void *unk24;
    void *unk28;
} func_8000BBA8_C7A8_arg;

INCLUDE_ASM("asm/nonmatchings/C570", func_8000B970_C570);

INCLUDE_ASM("asm/nonmatchings/C570", func_8000BA08_C608);

INCLUDE_ASM("asm/nonmatchings/C570", func_8000BB48_C748);

void func_8000BBA8_C7A8(func_8000BBA8_C7A8_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

INCLUDE_ASM("asm/nonmatchings/C570", func_8000BBE0_C7E0);

INCLUDE_ASM("asm/nonmatchings/C570", func_8000BC10_C810);
