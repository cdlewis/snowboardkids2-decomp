#include "10AD0.h"
#include "common.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_426EF0);
USE_ASSET(_42F1D0);
USE_ASSET(_41A1D0);
USE_ASSET(_426EF0);

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ void *unk4;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 unkD;
    /* 0x0E */ char pad[2];
} func_800B07A0_1DBE80_arg; // size 0x10

typedef struct {
    /* 0x000 */ u8 pad0[0x1E2];
    /* 0x1E2 */ u16 unk1E2;
} Allocation_1DB7A0;

extern s16 D_800B09B8_1DC098[];
extern s16 D_800B09BA_1DC09A[];
extern s16 D_800B09A8_1DC088[];

void func_800B0920_1DC000(void *);
void func_800B0968_1DC048(func_800B07A0_1DBE80_arg *);

void func_800B0804_1DBEE4(func_800B07A0_1DBE80_arg *);
void func_800B0610_1DBCF0(func_800B07A0_1DBE80_arg *);
void func_800B0664_1DBD44(func_800B07A0_1DBE80_arg *);

INCLUDE_ASM("asm/nonmatchings/1DB7A0", func_800B00C0_1DB7A0);

INCLUDE_ASM("asm/nonmatchings/1DB7A0", func_800B01B8_1DB898);

void func_800B0520_1DBC00(func_800B07A0_1DBE80_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_800B054C_1DBC2C(func_800B07A0_1DBE80_arg *arg0) {
    void *temp;
    s32 i;
    s16 v0, v2;

    getCurrentAllocation();
    temp = dmaRequestAndUpdateStateWithSize(&_426EF0_ROM_START, &_426EF0_ROM_END, 0xEEE8);
    setCleanupCallback(func_800B0664_1DBD44);

    for (i = 0; i < 4; i++) {
        v0 = D_800B09A8_1DC088[i * 2];
        arg0[i].unk0 = v0;
        v2 = D_800B09A8_1DC088[i * 2 + 1];
        arg0[i].unk8 = i / 2;
        arg0[i].unkD = 0;
        arg0[i].unkC = i & 1;
        arg0[i].unkA = 0xFF;
        arg0[i].unk4 = temp;
        arg0[i].unk2 = v2;
    }

    setCallback(func_800B0610_1DBCF0);
}

void func_800B0610_1DBCF0(func_800B07A0_1DBE80_arg *arg0) {
    s32 i;

    for (i = 0; i < 4; i++) {
        debugEnqueueCallback(8, 0, func_80012004_12C04, &arg0[i]);
    }
}

void func_800B0664_1DBD44(func_800B07A0_1DBE80_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_800B070C_1DBDEC(func_800B07A0_1DBE80_arg *);
void func_800B0774_1DBE54(func_800B07A0_1DBE80_arg *);

void func_800B0690_1DBD70(func_800B07A0_1DBE80_arg *arg0) {
    void *temp;

    getCurrentAllocation();
    temp = dmaRequestAndUpdateStateWithSize(&_426EF0_ROM_START, &_426EF0_ROM_END, 0xEEE8);
    setCleanupCallback(func_800B0774_1DBE54);
    arg0->unk0 = -96;
    arg0->unk2 = -80;
    arg0->unk8 = 6;
    arg0->unk4 = temp;
    setCallback(func_800B070C_1DBDEC);
}

void func_800B070C_1DBDEC(func_800B07A0_1DBE80_arg *arg0) {
    GameState *allocation = getCurrentAllocation();

    if (allocation->unk1E6 == 10) {
        arg0->unk0 = -80;
        arg0->unk8 = 7;
    } else {
        arg0->unk0 = -96;
        arg0->unk8 = 6;
    }
    debugEnqueueCallback(8, 0, func_8000FED0_10AD0, arg0);
}

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

void func_800B0804_1DBEE4(func_800B07A0_1DBE80_arg *arg0) {
    s32 pad[2];
    Allocation_1DB7A0 *allocation = getCurrentAllocation();

    arg0->unk0 = D_800B09B8_1DC098[allocation->unk1E2 * 2];
    arg0->unk2 = D_800B09BA_1DC09A[allocation->unk1E2 * 2];
    arg0->unk8 = allocation->unk1E2 + 2;

    debugEnqueueCallback(8, 7, func_8000FED0_10AD0, arg0);
}

void func_800B087C_1DBF5C(func_800B07A0_1DBE80_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_800B08A8_1DBF88(func_800B07A0_1DBE80_arg *arg0) {
    void *temp;

    temp = dmaRequestAndUpdateStateWithSize(&_41A1D0_ROM_START, &_41A1D0_ROM_END, 0x1B48);
    setCleanupCallback(func_800B0968_1DC048);
    arg0->unk0 = -0x2C;
    arg0->unk2 = -0x14;
    arg0->unk8 = 0xD;
    arg0->unk4 = temp;
    setCallback(func_800B0920_1DC000);
}

void func_800B0920_1DC000(void *arg0) {
    GameState *allocation = getCurrentAllocation();

    if (allocation->unk1E6 == 2) {
        debugEnqueueCallback(8, 7, func_8000FED0_10AD0, arg0);
    }
}

void func_800B0968_1DC048(func_800B07A0_1DBE80_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}
