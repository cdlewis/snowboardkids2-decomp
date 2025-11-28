#include "common.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_426EF0);
USE_ASSET(_42F1D0);

typedef struct {
    u8 pad[0x4];
    void *unk4;
} func_800B07A0_1DBE80_arg;

extern void *freeNodeMemory(void *);

void func_800B0774_1DBE54(void *);
void func_800B0804_1DBEE4(void *);

INCLUDE_ASM("asm/nonmatchings/1DB7A0", func_800B00C0_1DB7A0);

INCLUDE_ASM("asm/nonmatchings/1DB7A0", func_800B01B8_1DB898);

INCLUDE_ASM("asm/nonmatchings/1DB7A0", func_800B0520_1DBC00);

INCLUDE_ASM("asm/nonmatchings/1DB7A0", func_800B054C_1DBC2C);

INCLUDE_ASM("asm/nonmatchings/1DB7A0", func_800B0610_1DBCF0);

void func_800B0664_1DBD44(func_800B07A0_1DBE80_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/1DB7A0", func_800B0690_1DBD70);

INCLUDE_ASM("asm/nonmatchings/1DB7A0", func_800B070C_1DBDEC);

INCLUDE_ASM("asm/nonmatchings/1DB7A0", func_800B0774_1DBE54);

void func_800B07A0_1DBE80(func_800B07A0_1DBE80_arg *arg0) {
    void *temp;

    getCurrentAllocation();
    temp = dmaRequestAndUpdateStateWithSize(&_426EF0_ROM_START, &_42F1D0_ROM_START, 0xEEE8);
    setCleanupCallback(func_800B0774_1DBE54);
    arg0->unk4 = temp;
    setCallback(func_800B0804_1DBEE4);
}

INCLUDE_ASM("asm/nonmatchings/1DB7A0", func_800B0804_1DBEE4);

void func_800B087C_1DBF5C(func_800B07A0_1DBE80_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/1DB7A0", func_800B08A8_1DBF88);

INCLUDE_ASM("asm/nonmatchings/1DB7A0", func_800B0920_1DC000);

INCLUDE_ASM("asm/nonmatchings/1DB7A0", func_800B0968_1DC048);
