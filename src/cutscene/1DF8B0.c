#include "1DF8B0.h"
#include "task_scheduler.h"

void func_800B29F0_1DFAA0(void *);

extern void func_800B993C_1E69EC(void);

void noop_1DF8B(void) {
}

s32 returnZero_1DF8B8(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/cutscene/1DF8B0", func_800B2810_1DF8C0);

s32 returnZero_1DF9DC(void) {
    return 0;
}

void noop_1DF9E4(void) {
}

s32 returnZero_1DF9EC(void) {
    return 0;
}

void func_800B2944_1DF9F4(func_800B2944_1DF9F4_arg0 *arg0, u8 *arg1, s8 arg2) {
    func_800B2944_1DF9F4_asset *temp_v0;
    s32 *ptr;

    ptr = (s32 *)(arg1 + (arg2 * 244 + 0xA8));
    func_800B29F0_1DFAA0(arg1);

    temp_v0 = (func_800B2944_1DF9F4_asset *)scheduleTask(&func_800B993C_1E69EC, 1, 0, 0x64);
    if (temp_v0) {
        temp_v0->unk0 = arg1;
        temp_v0->unk4 = ptr[0x48 / 4];
        temp_v0->unk10 = arg0->unk2;
        temp_v0->unk12 = arg0->unk4;
        temp_v0->unk86 = arg0->unk0;
    } else {
        arg1[0xFF7] = 0;
    }
}
