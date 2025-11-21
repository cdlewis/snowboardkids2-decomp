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
void func_8004083C_4143C(void);

typedef struct {
    void *unk0;
} func_80040948_41548_arg;

void func_80040948_41548(func_80040948_41548_arg *arg0);

void func_800407E0_413E0(func_800407E0_413E0_arg *arg0) {
    GameState *temp_s0 = (GameState *)getCurrentAllocation();
    arg0->unk0 = load_3ECE40();
    arg0->unk4 = (u8 *)&temp_s0->unk44->unk1380;
    setCleanupCallback(&func_80040948_41548);
    setCallbackWithContinue(&func_8004083C_4143C);
}

INCLUDE_ASM("asm/nonmatchings/413E0", func_8004083C_4143C);

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

INCLUDE_ASM("asm/nonmatchings/413E0", func_80040D48_41948);

INCLUDE_ASM("asm/nonmatchings/413E0", func_80040D80_41980);

const char pushStartButtonText[] = "PUSH START BUTTON";

void func_80040E00_41A00(func_80040E00_41A00_arg *arg0) {
    if (gFrameCounter & 8) {
        enqueueTextRender(-0x44, arg0->unk8, 0, (u8 *)&pushStartButtonText, (s32)arg0->unkC, (s32)arg0->unkE);
    }
}

INCLUDE_ASM("asm/nonmatchings/413E0", func_80040E4C_41A4C);

INCLUDE_ASM("asm/nonmatchings/413E0", func_80040F34_41B34);

INCLUDE_ASM("asm/nonmatchings/413E0", func_80040F6C_41B6C);

INCLUDE_ASM("asm/nonmatchings/413E0", func_80041010_41C10);

INCLUDE_ASM("asm/nonmatchings/413E0", func_8004106C_41C6C);

INCLUDE_ASM("asm/nonmatchings/413E0", func_8004119C_41D9C);

void func_800413E0_41FE0(func_800407E0_413E0_arg *arg0) {
    arg0->unk0 = freeNodeMemory(arg0->unk0);
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/413E0", func_80041418_42018);

INCLUDE_ASM("asm/nonmatchings/413E0", func_80041518_42118);
