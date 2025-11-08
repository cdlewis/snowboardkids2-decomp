#include "1DF310.h"
#include "1DFAA0.h"
#include "task_scheduler.h"

typedef struct {
    func_800B29F0_1DFAA0_arg *unk0;
    u8 padding[0xA];
    s16 unkE;
} func_800B25A0_1DF650_task;

extern void func_8000D244_DE44(void);

void noop_1DF310(void) {
}

s32 returnZero_1DF318(void) {
    return 0;
}

void func_800B2270_1DF320(u8 *arg0, func_800B2270_1DF320_arg *arg1) {
    arg1->unkFF5 = (u8)*arg0;
}

void noop_1DF32C(void) {
}

s32 returnZero_1DF334(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/1DF310", func_800B228C_1DF33C);

s32 returnZero_1DF398(void) {
    return 0;
}

void noop_1DF3A0(void) {
}

s32 returnZero_1DF3A8(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/1DF310", func_800B2300_1DF3B0);

s32 returnZero_1DF40C(void) {
    return 0;
}

void noop_1DF414(void) {
}

s32 returnZero_1DF41C(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/1DF310", func_800B2374_1DF424);

INCLUDE_ASM("asm/nonmatchings/1DF310", func_800B2404_1DF4B4);

s32 returnZero_1DF590(void) {
    return 0;
}

void noop_1DF598(void) {
}

s32 returnZero_1DF5A0(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/1DF310", func_800B24F8_1DF5A8);

void noop_1DF5C4(void) {
}

s32 returnZero_1DF5CC(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/1DF310", func_800B2524_1DF5D4);

INCLUDE_ASM("asm/nonmatchings/1DF310", func_800B2550_1DF600);

void noop_1DF640(void) {
}

s32 returnZero_1DF648(void) {
    return 0;
}

void func_800B25A0_1DF650(u16 *arg0, func_800B29F0_1DFAA0_arg *arg1) {
    func_800B25A0_1DF650_task *temp_v0 = scheduleTask(&func_8000D244_DE44, 1, 0, 0x64);
    if (temp_v0 != NULL) {
        func_800B29F0_1DFAA0(arg1);
        temp_v0->unk0 = arg1;
        temp_v0->unkE = (u16)*arg0;
    }
}

void noop_1DF6BC(void) {
}

s32 returnZero_1DF6C4(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/1DF310", func_800B261C_1DF6CC);

func_800B2700_1DF7B0(func_800B2A24_1DFAD4_arg *arg0, s8 arg1) {
    func_800B2A24_1DFAD4_arg_item *temp_a0;
    s32 temp_v1;
    s32 temp_a0_val;

    temp_a0 = func_800B2A78_1DFB28(arg0, arg1);
    temp_v1 = arg0->unk1220 + temp_a0->unk4;
    arg0->unk1220 = temp_v1;

    if (temp_a0->unk4 > 0) {
        if (temp_a0->unk0 < temp_v1) {
            arg0->unk1220 = temp_a0->unk0;
            func_800B2A50_1DFB00(arg0, arg1);
        }
    } else {
        if (temp_v1 < temp_a0->unk0) {
            arg0->unk1220 = temp_a0->unk0;
            func_800B2A50_1DFB00(arg0, arg1);
        }
    }
}

s32 returnZero_1DF834(void) {
    return 0;
}

void noop_1DF83C(void) {
}

s32 returnZero_1DF844(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/1DF310", func_800B279C_1DF84C);

s32 returnZero_1DF89C(void) {
    return 0;
}
