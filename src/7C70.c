#include "common.h"

extern void func_80009F5C_AB5C(s32 *);
extern void func_80069CF8_6A8F8(void);
extern void func_80009E68_AA68(void *, s32);
extern void func_80009F90_AB90(void *, s32, s32, s32);
extern void setCleanupCallback(void *);
extern void setCallback(void *);
extern u8 randA(void);
extern void *scheduleTask(void *, u8, u8, u8);

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 _pad8[0x48];
    s32 unk50;
    s16 unk54;
    s8 unk56;
} func_80007130_7D30_arg;

typedef struct {
    u8 _pad[0x3C];
    s8 unk3C;
} func_80007100_7D00_inner;

typedef struct {
    func_80007100_7D00_inner *unk0;
} func_80007100_7D00_arg;

typedef struct {
    s32 unk0;
    s32 unk4;
} func_80007070_7C70_arg;

void func_800071E4_7DE4(void *);
void func_80007360_7F60(func_80007130_7D30_arg *);
void func_80007100_7D00(func_80007100_7D00_arg *);
void func_80007130_7D30(func_80007130_7D30_arg *);
void func_8000714C_7D4C(func_80007130_7D30_arg *);

void func_80007070_7C70(func_80007070_7C70_arg *arg0) {
    s32 i;
    func_80007130_7D30_arg *task;
    s32 temp;

    func_80009E68_AA68(&arg0->unk4, 5);

    for (i = 0; i < 12; i++) {
        task = scheduleTask(func_8000714C_7D4C, 0, 0, 0);
        if (task != NULL) {
            temp = arg0->unk0;
            task->unk50 = i;
            task->unk0 = temp;
        }
    }

    setCleanupCallback(func_80007130_7D30);
    setCallback(func_80007100_7D00);
}

void func_80007100_7D00(func_80007100_7D00_arg *arg0) {
    if (arg0->unk0->unk3C == 1) {
        func_80069CF8_6A8F8();
    }
}

void func_80007130_7D30(func_80007130_7D30_arg *arg0) {
    func_80009F5C_AB5C(&arg0->unk4);
}

void func_8000714C_7D4C(func_80007130_7D30_arg *arg0) {
    s32 *temp_s0;

    temp_s0 = &arg0->unk4;
    func_80009E68_AA68(temp_s0, 5);
    func_80009F90_AB90(temp_s0, 0x10000, 0, -1);
    arg0->unk56 = 0;
    arg0->unk54 = randA() % 15;
    setCleanupCallback(func_80007360_7F60);
    setCallback(func_800071E4_7DE4);
}

INCLUDE_ASM("asm/nonmatchings/7C70", func_800071E4_7DE4);

void func_80007360_7F60(func_80007130_7D30_arg *arg0) {
    func_80009F5C_AB5C(&arg0->unk4);
}
