#include "1DF180.h"

extern s32 gControllerInputs[4];

extern void func_800B4C80_1E1D30(void);
extern void func_800B4CB0_1E1D60(void);
extern void func_800B4B30_1E1BE0(s16, s16, s16, s16);
extern void func_800B57F0_1E28A0(s16, s16, s16, s16, s32, s32);
extern void func_800B4BDC_1E1C8C(s16, s16, s16, s32);
extern void func_800B598C_1E2A3C(s16, s16, s16, s16, s32, s32);
extern void func_80057ABC_586BC(s16, s16);

void func_800B20D0_1DF180(void) {
    s32 input = gControllerInputs[0x3];
    if (input & A_BUTTON) {
        func_800B4C80_1E1D30();
        return;
    }

    if (input & B_BUTTON) {
        func_800B4CB0_1E1D60();
    }
}

void noop_1DF1C8(void) {
}

s32 returnZero_1DF1D0(void) {
    return 0;
}

void func_800B2128_1DF1D8(func_800B2128_1DF1D8_arg *arg0) {
    s16 temp_a3;

    temp_a3 = arg0->unk4;
    if (temp_a3 == 1) {
        func_800B4B30_1E1BE0(arg0->unk0, arg0->unk2, arg0->unkA, arg0->unk8);
        return;
    }

    func_800B57F0_1E28A0(arg0->unk0, arg0->unk2, arg0->unkA, temp_a3, (s32)arg0->unk8, (s32)arg0->unk6);
}

void noop_1DF23C(void) {
}

s32 returnZero_1DF244(void) {
    return 0;
}

void func_800B219C_1DF24C(func_800B2128_1DF1D8_arg *arg0, func_800B219C_1DF24C_arg1_item *arg1, s8 arg2) {
    s32 *new_var;
    s32 temp_t0;
    new_var = &arg1[arg2].unkF0;
    temp_t0 = *new_var;

    if (arg0->unk4 == 1) {
        func_800B4BDC_1E1C8C(arg0->unk0, arg0->unk2, arg0->unk8, temp_t0);
        return;
    }
    func_800B598C_1E2A3C(arg0->unk0, arg0->unk2, arg0->unk4, arg0->unk8, (s32)arg0->unk6, temp_t0);
}

void noop_1DF2D0(void) {
}

s32 returnZero_1DF2D8(void) {
    return 0;
}

void func_800B2230_1DF2E0(func_800B2128_1DF1D8_arg *arg0) {
    func_80057ABC_586BC(arg0->unk2, arg0->unk0);
}
