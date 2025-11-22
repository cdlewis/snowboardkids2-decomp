#include "5E590.h"
#include "68CF0.h"
#include "common.h"
#include "gamestate.h"
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

extern void *func_80055D7C_5697C(s32);
extern void func_80040870_41470(void);
extern void func_8004106C_41C6C(void);
extern void *func_80035F80_36B80(s32);
extern void *loadAsset_34F7E0(void);
void func_80040E00_41A00(func_80040E00_41A00_arg *);
void func_80040E4C_41A4C(void);
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

typedef struct {
    void *unk0;
    void *unk4;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
} func_80040D80_41980_arg;

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

INCLUDE_ASM("asm/nonmatchings/413E0", func_800409B4_415B4);

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
        arg0->unk4 = func_80035F80_36B80(1);
        arg0->unk0 = loadAsset_34F7E0();
        arg0->unk10 = 0x80;
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

INCLUDE_ASM("asm/nonmatchings/413E0", func_80040E4C_41A4C);

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
            task->unkC = arg4;
            task->unkE = arg5;
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
