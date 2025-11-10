#include "1DF310.h"
#include "1DFAA0.h"
#include "task_scheduler.h"

extern u8 D_800BA5C0_1E7670[];

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

void func_800B228C_1DF33C(func_800B228C_1DF33C_arg *arg0, Node_70B00 *arg1) {
    u8 temp_a1 = D_800BA5C0_1E7670[arg0->unk2];
    func_8006FE28_70A28(arg1->unk0.next, temp_a1, temp_a1, temp_a1);
    func_8006FDA0_709A0(arg1->unk0.next, 0, arg0->unk1);
}

s32 returnZero_1DF398(void) {
    return 0;
}

void noop_1DF3A0(void) {
}

s32 returnZero_1DF3A8(void) {
    return 0;
}

void func_800B2300_1DF3B0(func_800B2300_1DF3B0_arg *arg0, Node_70B00 *arg1) {
    u8 temp_a1;

    temp_a1 = D_800BA5C0_1E7670[arg0->unk2];
    func_8006FE28_70A28(arg1->unk0.next, temp_a1, temp_a1, temp_a1);
    func_8006FDA0_709A0(arg1->unk0.next, 0xFF, arg0->unk1);
}

s32 returnZero_1DF40C(void) {
    return 0;
}

void noop_1DF414(void) {
}

s32 returnZero_1DF41C(void) {
    return 0;
}

void func_800B2374_1DF424(func_800B2A24_1DFAD4_arg_item *arg0, func_800B2A24_1DFAD4_arg *arg1, s8 arg2) {
    func_800B2A24_1DFAD4_arg_item *temp_v0;

    temp_v0 = func_800B2A78_1DFB28(arg1, arg2);
    temp_v0->unk0.half.lower = arg0->unk0.half.lower;
    temp_v0->unkA = 0xFF;
    memcpy(&temp_v0->unk0.half.upper, &arg0->unk0.half.upper, 4);
    memcpy(&temp_v0->unk4.half.upper, &arg0->unk4.half.upper, 4);
    func_800B2A24_1DFAD4(arg1, arg2);
}

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

void func_800B2700_1DF7B0(func_800B2A24_1DFAD4_arg *arg0, s8 arg1) {
    func_800B2A24_1DFAD4_arg_item *temp_a0;
    s32 temp_v1;
    s32 temp_a0_val;

    temp_a0 = func_800B2A78_1DFB28(arg0, arg1);
    temp_v1 = arg0->unk1220 + temp_a0->unk4.full;
    arg0->unk1220 = temp_v1;

    if (temp_a0->unk4.full > 0) {
        if (temp_a0->unk0.full < temp_v1) {
            arg0->unk1220 = temp_a0->unk0.full;
            func_800B2A50_1DFB00(arg0, arg1);
        }
    } else {
        if (temp_v1 < temp_a0->unk0.full) {
            arg0->unk1220 = temp_a0->unk0.full;
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
