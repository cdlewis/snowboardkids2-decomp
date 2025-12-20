#include "10AD0.h"
#include "6E840.h"
#include "common.h"

typedef struct {
    s8 unk0;
    u8 padding[1];
    s16 unk2;
    s32 unk4;
    Node_70B00 unk8;
    Node_70B00 unk1E0;
    Node_70B00 unk3B8;
    Node_70B00 unk590;
    Node_70B00 unk768;
    s16 unk940;
    s16 unk942;
    s16 unk944;
    void *unk948;
    s8 unk94C;
    u8 padding8[0xF];
    void *unk95C;
    void *unk960;
    void *unk964;
    void *unk968;
    s16 unk96C;
    s16 unk96E;
    u8 padding9A[0x4];
    void *unk974;
    s16 unk978;
    s16 unk97A;
    u8 unk97C;
    u8 padding9B[0x3];
    s16 unk980;
    s16 unk982;
    void *unk984;
    void *unk988;
    s16 unk98C;
    s16 unk98E;
    u8 unk990;
    u8 padding9C[0x3];
    void *unk994;
    void *unk998;
    void *unk99C;
    s16 unk9A0;
    u8 padding9D[0x2];
    s16 unk9A4;
    s16 unk9A6;
    u8 padding9E[0x4];
    s16 unk9AC;
    s16 unk9AE;
    s16 unk9B0;
    s16 unk9B2;
    u8 padding9F[0x4];
    void *unk9B8[6];
    u8 padding11[0x41C];
    void *unkDEC;
    s32 unkDF0;
    s32 unkDF4;
    s32 unkDF8;
    s32 unkDFC;
    u8 unkE00[0xA];
    s16 unkE0A;
    u8 paddingE0C[0x4];
    u8 unkE10[0xA];
    s16 unkE1A;
    u8 paddingE1C[0x4];
    u8 unkE20[0xA];
    s16 unkE2A;
    u8 paddingE2C[0x4];
    u8 unkE30[0xA];
    s16 unkE3A;
    u8 paddingE3C[0x4];
    s16 unkE40;
    s16 unkE42;
    ColorData unkE44[3];
} func_80003EE0_4AE0_task_memory;

INCLUDE_ASM("asm/nonmatchings/1DC260", func_800B00C0_1DC260);

void func_800B016C(void *arg0) {
    func_80003EE0_4AE0_task_memory *state = (func_80003EE0_4AE0_task_memory *)arg0;
    s16 temp_v1;
    s32 temp_v0;
    s32 value;

    temp_v1 = state->unk2;

    switch (temp_v1) {
        case 0x1A22:
            state->unkDFC = 0xC0000;
            break;
        case 0x1AD6:
            state->unkDFC = 0xFFF40000;
            break;
        case 0x1B30:
            state->unkDF4 = 0xC0000;
            break;
    }

    value = state->unkDF4;
    if (value != 0) {
        temp_v0 = state->unkDF0 + value;
        state->unkDF0 = temp_v0;
        if (temp_v0 > 0xFF0000) {
            state->unkDF0 = 0xFF0000;
        }
        if (state->unkDF0 < 0) {
            state->unkDF0 = 0;
        }
    }

    value = state->unkDFC;
    if (value != 0) {
        temp_v0 = state->unkDF8 + value;
        state->unkDF8 = temp_v0;
        if (temp_v0 > 0xFF0000) {
            state->unkDF8 = 0xFF0000;
        }
        if (state->unkDF8 < 0) {
            state->unkDF8 = 0;
        }
    }

    temp_v0 = state->unkDF0;
    if (temp_v0 != 0) {
        s16 shortVal = temp_v0 >> 16;
        void *callback = func_80012004_12C04;
        state->unkE1A = shortVal;
        state->unkE0A = shortVal;
        debugEnqueueCallback(1, 4, callback, state->unkE00);
        debugEnqueueCallback(1, 4, callback, state->unkE10);
    }

    temp_v0 = state->unkDF8;
    if (temp_v0 != 0) {
        s16 shortVal = temp_v0 >> 16;
        void *callback = func_80012004_12C04;
        state->unkE3A = shortVal;
        state->unkE2A = shortVal;
        debugEnqueueCallback(1, 4, callback, state->unkE20);
        debugEnqueueCallback(1, 4, callback, state->unkE30);
    }
}
