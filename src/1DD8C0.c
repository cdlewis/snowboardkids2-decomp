#include "1DD8C0.h"

void func_80057514_58114(s16, u16, u16);
void func_80057564_58164(s16);
void func_80057550_58150(u16, u16);

void noop_1DD8C0(void) {
}

s32 returnZero_1DD8C8(void) {
    return 0;
}

void func_800B0820_1DD8D0(func_800B0820_1DD8D0_arg0 *arg0) {
    func_80057514_58114(arg0->unk0, arg0->unk4, arg0->unk2);
}

void noop_1DD8F8(void) {
}

s32 returnZero_1DD900(void) {
    return 0;
}

void func_800B0858_1DD908(s16 *arg0) {
    if (*arg0 < 4) {
        func_80057564_58164(4);
    } else {
        func_80057564_58164(*arg0);
    }
}

void noop_1DD934(void) {
}

s32 returnZero_1DD93C(void) {
    return 0;
}

void func_800B0894_1DD944(func_800B0820_1DD8D0_arg0 *arg0) {
    func_80057550_58150(arg0->unk2, arg0->unk0);
}
