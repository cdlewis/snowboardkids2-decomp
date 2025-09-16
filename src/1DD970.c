#include "1DD970.h"

extern void func_800B5F2C_1E2FDC(s32, s16, s16);
extern void func_800B5FB0_1E3060(s32, s16, s16);
extern void func_800B6034_1E30E4(s32, s32, s16);
extern void func_800B6088_1E3138(s32, s32, s16);
extern void func_800B60DC_1E318C(s32, s32, s16);
extern void func_800B5EC4_1E2F74(s32, s16, s16, s32, s32, s32);

void noop_1DD970(void) {
}

s32 returnZero_1DD978(void) {
    return 0;
}

void func_800B08D0_1DD980(func_800B08D0_1DD980_arg0 *arg0, func_800B08D0_1DD980_arg1 *arg1) {
    func_800B5EC4_1E2F74(arg1->unkC, arg0->unk0, arg0->unk2, arg0->unk4, arg0->unk8, arg0->unkC);
}

void noop_1DD9C0(void) {
}

s32 returnZero_1DD9C8(void) {
    return 0;
}

void func_800B0920_1DD9D0(func_800B0920_1DD9D0_arg0 *arg0, func_800B0920_1DD9D0_arg1 *arg1) {
    func_800B5F2C_1E2FDC(arg1->unkC, arg0->unk0, arg0->unkC);
    func_800B5FB0_1E3060(arg1->unkC, arg0->unk2, arg0->unkC);
    func_800B6034_1E30E4(arg1->unkC, arg0->unk10, arg0->unkC);
    func_800B6088_1E3138(arg1->unkC, arg0->unk4, arg0->unkC);
    func_800B60DC_1E318C(arg1->unkC, arg0->unk8, arg0->unkC);
}

s32 returnZero_1DDA4C(void) {
    return 0;
}

void noop_1DDA54(void) {
}

s32 returnZero_1DDA5C(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/1DD970", func_800B09B4_1DDA64);

void noop_1DDAA4(void) {
}

s32 returnZero_1DDAAC(void) {
    return 0;
}

void func_800B6130_1E31E0(s32, s16, s16);

void func_800B0A04_1DDAB4(func_800B0A04_1DDAB4_arg0 *arg0, func_800B0A04_1DDAB4_arg1 *arg1) {
    func_800B6130_1E31E0(arg1->unkC, arg0->unk0, arg0->unk2);
}

s32 returnZero_1DDADC(void) {
    return 0;
}