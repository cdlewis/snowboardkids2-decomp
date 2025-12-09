#include "36B80.h"
#include "3E160.h"
#include "56910.h"
#include "5E590.h"
#include "68CF0.h"
#include "6DE50.h"
#include "common.h"
#include "gamestate.h"
#include "geometry.h"
#include "task_scheduler.h"

typedef struct {
    void *unk0;
    void *unk4;
} func_800407E0_413E0_arg;

typedef struct {
    u8 padding[0x8];
    s16 unk8;
    s16 padding2;
    s16 unkC;
    s16 unkE;
} func_80040E00_41A00_arg;

extern s32 gFrameCounter;
extern s32 D_800907F8_913F8;
extern s32 D_800907EC_913EC[];
extern s16 identityMatrix[];

extern void func_80040870_41470(void);
extern void func_8004106C_41C6C(void);
void func_80040B4C_4174C(void);
void func_80040E00_41A00(func_80040E00_41A00_arg *);
void func_80040F34_41B34(func_800407E0_413E0_arg *);
void func_800413E0_41FE0(func_800407E0_413E0_arg *arg0);

typedef struct {
    u8 _pad[0x24];
    void *unk24;
    u8 _pad2[0x10];
    s32 unk38;
} func_8004083C_4143C_arg;

typedef struct {
    u8 _pad[0xBDE];
    u8 unkBDE;
} func_8004083C_4143C_struct24;

typedef struct {
    void *unk0;
} func_80040948_41548_arg;

typedef struct {
    u8 _pad[0x24];
    void *unk24;
    void *unk28;
} func_80040D48_41948_arg;

void func_80040D48_41948(func_80040D48_41948_arg *);

typedef struct {
    u8 high;
    u8 low;
} Bytes;

typedef union {
    s16 asS16;
    Bytes asBytes;
} S16OrBytes;

typedef union {
    s16 asS16;
    Bytes asBytes;
} S16OrBytesC;

typedef union {
    s16 asS16;
    Bytes asBytes;
} S16OrBytesE;

typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    S16OrBytesC unkC;
    S16OrBytesE unkE;
    S16OrBytes unk10;
    s16 unk12;
} func_80040D80_41980_arg;

void func_80040E4C_41A4C(func_80040D80_41980_arg *);

void func_8004083C_4143C(func_8004083C_4143C_arg *arg0);
void func_80040948_41548(func_80040948_41548_arg *arg0);

void func_800407E0_413E0(func_800407E0_413E0_arg *arg0) {
    GameState *temp_s0 = (GameState *)getCurrentAllocation();
    arg0->unk0 = load_3ECE40();
    arg0->unk4 = (u8 *)&temp_s0->unk44->unk1380;
    setCleanupCallback(&func_80040948_41548);
    setCallbackWithContinue(&func_8004083C_4143C);
}

void func_8004083C_4143C(func_8004083C_4143C_arg *arg0) {
    func_8004083C_4143C_struct24 *temp;

    temp = (func_8004083C_4143C_struct24 *)arg0->unk24;
    if (temp->unkBDE != 0) {
        arg0->unk38 = 6;
        setCallback(func_80040870_41470);
    }
}

INCLUDE_ASM("asm/nonmatchings/413E0", func_80040870_41470);

void func_80040948_41548(func_80040948_41548_arg *arg0) {
    arg0->unk0 = freeNodeMemory(arg0->unk0);
}

void func_80040974_41574(void *arg0) {
    Node *temp_v0 = scheduleTask(&func_800407E0_413E0, 0U, 0U, 0xF0U);
    if (temp_v0 != NULL) {
        temp_v0->cleanupCallback = arg0;
    }
}

void func_800409B4_415B4(void *arg0) {
    s32 sp10[8];
    s32 sp30[4]; // Extra padding for stack alignment
    s32 *temp_s0;
    void *temp_s2;
    D_80090F90_91B90_item *temp_s3;
    u16 angle;

    temp_s2 = getCurrentAllocation();
    temp_s3 = func_80055D10_56910(((u8 *)temp_s2)[0x5C]);
    ((s32 *)arg0)[0x20 / 4] = (s32)((u8 *)func_80055E68_56A68(((u8 *)temp_s2)[0x5C]) + 0x50);
    ((s32 *)arg0)[0x24 / 4] = (s32)func_80055DC4_569C4(((u8 *)temp_s2)[0x5C]);
    ((s32 *)arg0)[0x28 / 4] = (s32)func_80055DF8_569F8(((u8 *)temp_s2)[0x5C]);
    ((s32 *)arg0)[0x2C / 4] = 0;
    angle = func_800625A4_631A4((u8 *)temp_s2 + 0x30, sp30);
    createYRotationMatrix(arg0, (angle + temp_s3->unk8) & 0xFFFF);
    rotateVectorY(&D_800907EC_913EC, (s16)(angle + temp_s3->unk8), (u8 *)arg0 + 0x14);
    ((s32 *)arg0)[0x14 / 4] = ((s32 *)arg0)[0x14 / 4] + temp_s3->unk0;
    ((s32 *)arg0)[0x1C / 4] = ((s32 *)arg0)[0x1C / 4] + temp_s3->unk4;
    ((s32 *)arg0)[0x18 / 4] = sp30[1]; // sp34
    ((s32 *)arg0)[0x5C / 4] = (s32)((u8 *)func_80055E68_56A68(((u8 *)temp_s2)[0x5C]) + 0x60);
    temp_s0 = sp10;
    ((void **)arg0)[0x60 / 4] = (void *)((s32 *)arg0)[0x24 / 4];
    ((void **)arg0)[0x64 / 4] = (void *)((s32 *)arg0)[0x28 / 4];
    ((s32 *)arg0)[0x68 / 4] = ((s32 *)arg0)[0x2C / 4];
    memcpy(temp_s0, identityMatrix, 0x20);
    temp_s0[6] = 0x180000;
    func_8006B084_6BC84(temp_s0, arg0, (u8 *)arg0 + 0x3C);
    ((void **)arg0)[0x9C / 4] = (void *)((s32 *)arg0)[0x24 / 4];
    ((void **)arg0)[0xA0 / 4] = (void *)((s32 *)arg0)[0x28 / 4];
    ((s32 *)arg0)[0xA4 / 4] = ((s32 *)arg0)[0x2C / 4];
    temp_s0[6] = 0x160000;
    temp_s0[7] = 0xA3333;
    func_8006B084_6BC84(temp_s0, arg0, (u8 *)arg0 + 0x78);
    ((s16 *)arg0)[0xB4 / 2] = 0;
    ((s16 *)arg0)[0xB6 / 2] = 0;
    setCleanupCallback(func_80040D48_41948);
    setCallback(func_80040B4C_4174C);
}

INCLUDE_ASM("asm/nonmatchings/413E0", func_80040B4C_4174C);

void func_80040D48_41948(func_80040D48_41948_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void func_80040D80_41980(func_80040D80_41980_arg *arg0) {
    if (arg0->unkA == 0) {
        arg0->unk0 = NULL;
        arg0->unk4 = NULL;
        setCallback(&func_80040E00_41A00);
    } else {
        arg0->unk4 = (s32)func_80035F80_36B80(1);
        arg0->unk0 = (s32)loadAsset_34F7E0();
        arg0->unk10.asS16 = 0x80;
        arg0->unk12 = -8;
        setCallback(&func_80040E4C_41A4C);
    }
    setCleanupCallback(&func_80040F34_41B34);
}

const char pushStartButtonText[] = "PUSH START BUTTON";

void func_80040E00_41A00(func_80040E00_41A00_arg *arg0) {
    if (gFrameCounter & 8) {
        enqueueTextRender(-0x44, arg0->unk8, 0, (u8 *)&pushStartButtonText, (s32)arg0->unkC, (s32)arg0->unkE);
    }
}

void func_80040E4C_41A4C(func_80040D80_41980_arg *arg0) {
    func_80035260_35E60(
        arg0->unk4,
        (s32)&D_800907F8_913F8,
        -0x68,
        arg0->unk8,
        0xFF,
        0xFF,
        0,
        arg0->unkC.asBytes.low,
        arg0->unkE.asBytes.low
    );

    arg0->unk10.asS16 += arg0->unk12;

    if (arg0->unk10.asS16 == 0x20) {
        arg0->unk12 = 8;
    }

    if (arg0->unk10.asS16 == 0x80) {
        arg0->unk12 = -8;
    }

    func_8006D4B8_6E0B8(
        arg0->unk0,
        -0x68,
        arg0->unk8,
        0xD,
        2,
        0,
        arg0->unk10.asBytes.low,
        0xFF,
        0,
        0,
        0xC0,
        arg0->unkC.asBytes.low,
        arg0->unkE.asBytes.low
    );
}

void func_80040F34_41B34(func_800407E0_413E0_arg *arg0) {
    arg0->unk0 = freeNodeMemory(arg0->unk0);
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

extern u8 gConnectedControllerMask;

void func_80040F6C_41B6C(s32 arg0, s16 arg1, u8 arg2, u8 arg3, s16 arg4, s16 arg5) {
    func_80040D80_41980_arg *task;

    if (((s16)arg0 == 0) || !(gConnectedControllerMask & 1)) {
        task = (func_80040D80_41980_arg *)scheduleTask(&func_80040D80_41980, arg2, arg3, 0xC8);
        if (task != NULL) {
            if (gConnectedControllerMask & 1) {
                task->unkA = 0;
            } else {
                task->unkA = 1;
            }
            task->unk8 = arg1;
            task->unkC.asS16 = arg4;
            task->unkE.asS16 = arg5;
        }
    }
}

typedef struct {
    void *unk0;
    void *unk4;
    u8 _pad[0x12];
    s16 unk1A;
} func_80041010_41C10_arg;

void func_80041010_41C10(func_80041010_41C10_arg *arg0) {
    s16 temp;

    arg0->unk0 = func_80055D7C_5697C(0xB);
    temp = arg0->unk1A;
    arg0->unk4 = allocateNodeMemory((temp * 7) * 8);
    setCleanupCallback(&func_800413E0_41FE0);
    setCallbackWithContinue(&func_8004106C_41C6C);
}

INCLUDE_ASM("asm/nonmatchings/413E0", func_8004106C_41C6C);

INCLUDE_ASM("asm/nonmatchings/413E0", func_8004119C_41D9C);

void func_800413E0_41FE0(func_800407E0_413E0_arg *arg0) {
    arg0->unk0 = freeNodeMemory(arg0->unk0);
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/413E0", func_80041418_42018);

typedef struct {
    u8 _pad[0x8];
    void *freeNext;
    u8 _pad2[0xC];
    s16 unk18;
    s16 unk1A;
    u8 unk1C;
} func_80041518_42118_Node;

void func_80041518_42118(void *arg0) {
    func_80041518_42118_Node *task;

    task = (func_80041518_42118_Node *)scheduleTask(&func_80041010_41C10, 0, 0, 0xF0);

    if (task != NULL) {
        task->unk18 = 0;
        task->freeNext = arg0;
        task->unk1A = 0x64;
        task->unk1C = 0;
    }
}
