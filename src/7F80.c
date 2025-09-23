#include "20F0.h"
#include "6E840.h"
#include "common.h"
#include "geometry.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_49B500);
USE_ASSET(_646CD0);

typedef struct {
    s32 unk0;
    u8 padding[0x17C];
} func_800078C4_84C4_arg_unk44;

typedef struct {
    struct {
        u8 padding[0xC];
        s16 unkC;
        u8 padding2[0x8];
        s32 unk18;
        u8 padding3[0x20];
        s8 unk3C;
        s8 unk3D;
    } *unk0;
    void *unk4;

    u8 padding[0xC];
    s32 unk14;

    u8 padding2[0x1C];

    s32 unk34;
    s32 *unk38;
    s32 unk3C;

    s32 unk40;
    func_800078C4_84C4_arg_unk44 *unk44;
} func_800078C4_84C4_arg;

void func_80007958_8558(void);
void func_80007ABC_86BC(func_800078C4_84C4_arg *);
void func_800073E0_7FE0(void);
void func_80007558_8158(void);
extern void func_80007560_8160(void);
extern u8 identityMatrix[];

typedef struct {
    s32 unk0;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
} func_80007380_7F80_arg;
void func_80007380_7F80(func_80007380_7F80_arg *arg0) {
    setCleanupCallback(&func_80007558_8158);
    arg0->unk8 = 0;
    arg0->unk6 = 0;
    arg0->unk4 = 0;
    arg0->unkE = 0;
    arg0->unkC = 0;
    arg0->unkA = 0;
    arg0->unk14 = 0;
    arg0->unk12 = 0;
    arg0->unk10 = 0;
    setCallback(&func_800073E0_7FE0);
}

INCLUDE_ASM("asm/nonmatchings/7F80", func_800073E0_7FE0);

void func_80007558_8158(void) {
}

INCLUDE_ASM("asm/nonmatchings/7F80", func_80007560_8160);

void func_800078C4_84C4(func_800078C4_84C4_arg *arg0) {
    setCleanupCallback(&func_80007ABC_86BC);
    arg0->unk4 = dmaRequestAndUpdateStateWithSize(&_646CD0_ROM_START, &_646CD0_ROM_END, 0xE8);
    arg0->unk44 = dmaRequestAndUpdateStateWithSize(&_49B500_ROM_START, &_49B500_ROM_END, 0xC00);
    if (arg0->unk0->unkC == 0x3E) {
        arg0->unk40 = 0;
    } else {
        arg0->unk40 = 1;
    }
    setCallback(&func_80007958_8558);
}

INCLUDE_ASM("asm/nonmatchings/7F80", func_80007958_8558);

void func_8000799C_859C(func_800078C4_84C4_arg *arg0) {
    u8 sp10[0x20];
    s32 temp_t0;
    s8 temp_v0;
    void *temp_s0;

    memcpy(&sp10, &identityMatrix, 0x20);

    if (arg0->unk0->unk3C == 1) {
        func_80069CF8_6A8F8();
        return;
    }

    temp_v0 = arg0->unk0->unk3D;
    if (arg0->unk0->unk3D == 0) {
        arg0->unk3C = 0;
    } else if (temp_v0 < 0 || arg0->unk0->unk3D >= 3) {
        arg0->unk3C = 0;
    } else {
        do {
            if (arg0->unk3C == 0) {
                memcpy(&arg0->unk14, &arg0->unk0->unk18, 0x20);
                scaleMatrix(&arg0->unk14, 0x1000, 0x1000, 0x1000);
            }
        } while (0);
        func_80001604_2204(arg0->unk0, 0);
    }

    if (arg0->unk0->unk3D == 1) {
        temp_t0 = arg0->unk3C;
        if (arg0->unk3C < 7) {
            arg0->unk34 = 0;
            arg0->unk38 = &arg0->unk44[temp_t0].unk0;
            debugEnqueueCallback(0, 1, &func_80007560_8160, arg0);
            arg0->unk3C++;
        }
    }
}

void func_80007ABC_86BC(func_800078C4_84C4_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk44 = freeNodeMemory(arg0->unk44);
}
