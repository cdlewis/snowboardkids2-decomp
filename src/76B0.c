#include "common.h"
#include "task_scheduler.h"

extern u8 identityMatrix[];

extern void func_80006B04_7704(void);

void func_80006C3C_783C(void);

typedef struct {
    u8 unk0[0x4];
    u8 unk4[0x20];
    s16 unk24;
    u8 padding[0x2];
    s16 unk28;
} func_80006AB0_76B0_arg;

void func_80006AB0_76B0(func_80006AB0_76B0_arg *arg0) {
    memcpy(arg0->unk4, identityMatrix, 0x20);
    arg0->unk24 = 0;
    arg0->unk28 = 0;
    setCleanupCallback(func_80006C3C_783C);
    setCallback(func_80006B04_7704);
}

INCLUDE_ASM("asm/nonmatchings/76B0", func_80006B04_7704);

void func_80006C3C_783C(void) {
}
