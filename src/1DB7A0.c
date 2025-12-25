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
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ void *unk4;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
} func_800B00C0_InnerStruct; // size 0x14

typedef struct {
    /* 0x00 */ func_800B00C0_InnerStruct elements[2];
    /* 0x28 */ u8 unk28;
    /* 0x29 */ u8 unk29;
    /* 0x2A */ u8 unk2A;
    /* 0x2B */ u8 unk2B;
    /* 0x2C */ u8 unk2C;
    /* 0x2D */ u8 unk2D;
} func_800B00C0_Arg;

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

void func_800B01B8_1DB898(func_800B00C0_Arg *);
void func_800B0520_1DBC00(func_800B07A0_1DBE80_arg *);

void func_800B00C0_1DB7A0(func_800B00C0_Arg *arg0) {
    Allocation_1DB7A0 *allocation;
    void *dmaResult;
    s32 i;
    s32 yPos;
    s16 scaleConst;
    s16 alphaConst;
    volatile func_800B00C0_InnerStruct *element;

    allocation = getCurrentAllocation();
    dmaResult = loadCompressedData(&_426EF0_ROM_START, &_42F1D0_ROM_START, 0xEEE8);
    setCleanupCallback(func_800B0520_1DBC00);

    i = 0;
    scaleConst = 0x400;
    alphaConst = 0xFF;
    yPos = -0x9C;
    element = (volatile func_800B00C0_InnerStruct *)arg0;
    do {
        s32 temp;
        s16 x;
        s32 unk8_val;

        x = (arg0->unk2B << 6) - 0x60;
        element->unk2 = yPos;
        yPos += 0x20;
        element->unk0 = x;
        unk8_val = arg0->unk2B;
        temp = i + 8;
        i++;
        element->unkA = scaleConst;
        element->unkC = scaleConst;
        element->unk10 = alphaConst;
        element->unkE = 0;
        element->unk13 = 0;
        element->unk12 = 0;
        element->unk4 = dmaResult;
        element->unk8 = temp + unk8_val * 6;
        element++;
    } while (i < 2);

    arg0->unk2C = 0;
    arg0->unk2D = 0;
    {
        u16 alloc_val = allocation->unk1E2;
        arg0->unk28 = 0;
        arg0->unk29 = 0;
        arg0->unk2A = alloc_val;
    }

    setCallback(func_800B01B8_1DB898);
}

INCLUDE_ASM("asm/nonmatchings/1DB7A0", func_800B01B8_1DB898);

void func_800B0520_1DBC00(func_800B07A0_1DBE80_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_800B054C_1DBC2C(func_800B07A0_1DBE80_arg *arg0) {
    void *temp;
    s32 i;
    s16 v0, v2;

    getCurrentAllocation();
    temp = loadCompressedData(&_426EF0_ROM_START, &_426EF0_ROM_END, 0xEEE8);
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
    temp = loadCompressedData(&_426EF0_ROM_START, &_426EF0_ROM_END, 0xEEE8);
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
    temp = loadCompressedData(&_426EF0_ROM_START, &_42F1D0_ROM_START, 0xEEE8);
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

    temp = loadCompressedData(&_41A1D0_ROM_START, &_41A1D0_ROM_END, 0x1B48);
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
