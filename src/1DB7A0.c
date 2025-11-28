#include "common.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_426EF0);
USE_ASSET(_42F1D0);
USE_ASSET(_41A1D0);

extern u8 D_426EF0[];
extern u8 D_42F1D0[];
extern u8 D_41A1D0[];
extern u8 D_41AD80[];

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ void *unk4;
    /* 0x08 */ s16 unk8;
} func_800B07A0_1DBE80_arg;

extern void *freeNodeMemory(void *);

void func_800B0804_1DBEE4(void *);

INCLUDE_ASM("asm/nonmatchings/1DB7A0", func_800B00C0_1DB7A0);

INCLUDE_ASM("asm/nonmatchings/1DB7A0", func_800B01B8_1DB898);

void func_800B0520_1DBC00(func_800B07A0_1DBE80_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/1DB7A0", func_800B054C_1DBC2C);

INCLUDE_ASM("asm/nonmatchings/1DB7A0", func_800B0610_1DBCF0);

void func_800B0664_1DBD44(func_800B07A0_1DBE80_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_800B070C_1DBDEC(void *);
void func_800B0774_1DBE54(func_800B07A0_1DBE80_arg *);

void func_800B0690_1DBD70(func_800B07A0_1DBE80_arg *arg0) {
    void *temp;

    getCurrentAllocation();
    temp = dmaRequestAndUpdateStateWithSize(D_426EF0, D_42F1D0, 0xEEE8);
    setCleanupCallback(func_800B0774_1DBE54);
    arg0->unk0 = -96;
    arg0->unk2 = -80;
    arg0->unk8 = 6;
    arg0->unk4 = temp;
    setCallback(func_800B070C_1DBDEC);
}

INCLUDE_ASM("asm/nonmatchings/1DB7A0", func_800B070C_1DBDEC);

void func_800B0774_1DBE54(func_800B07A0_1DBE80_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

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

void func_800B0920_1DC000(void *);
void func_800B0968_1DC048(func_800B07A0_1DBE80_arg *);

void func_800B08A8_1DBF88(func_800B07A0_1DBE80_arg *arg0) {
    void *temp;

    temp = dmaRequestAndUpdateStateWithSize(D_41A1D0, D_41AD80, 0x1B48);
    setCleanupCallback(func_800B0968_1DC048);
    arg0->unk0 = -0x2C;
    arg0->unk2 = -0x14;
    arg0->unk8 = 0xD;
    arg0->unk4 = temp;
    setCallback(func_800B0920_1DC000);
}

extern void debugEnqueueCallback(u16 index, u8 arg1, void *arg2, void *arg3);
extern void func_8000FED0_10AD0(void);

void func_800B0920_1DC000(void *arg0) {
    GameState *allocation = getCurrentAllocation();

    if (allocation->unk1E6 == 2) {
        debugEnqueueCallback(8, 7, func_8000FED0_10AD0, arg0);
    }
}

void func_800B0968_1DC048(func_800B07A0_1DBE80_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}
