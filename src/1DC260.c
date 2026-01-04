#include "10AD0.h"
#include "6E840.h"
#include "common.h"

typedef struct {
    s16 x;
    s16 y;
    void *asset;
    s16 spriteIndex;
    s16 alpha;
    s8 unkC;
    s8 unkD;
    u8 pad[2];
} SpriteElement16;

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
    void *volatile unkDEC;
    s32 unkDF0;
    s32 unkDF4;
    s32 unkDF8;
    s32 unkDFC;
    SpriteElement16 unkE00;
    SpriteElement16 unkE10;
    SpriteElement16 unkE20;
    SpriteElement16 unkE30;
    s16 unkE40;
    s16 unkE42;
    ColorData unkE44[3];
} func_80003EE0_4AE0_task_memory;

void func_800B00C0_1DC260(volatile func_80003EE0_4AE0_task_memory *state) {
    void *asset0;
    void *asset1;
    void *asset2;
    void *asset3;
    s16 negX;
    s16 negY;
    s16 new_var;
    s16 alpha;

    asset0 = state->unkDEC;
    asset1 = state->unkDEC;
    asset2 = state->unkDEC;
    asset3 = state->unkDEC;
    negX = -0x40;
    negY = -0x10;
    alpha = 0xFF;

    do {
        state->unkE10.spriteIndex = 1;
        state->unkE20.spriteIndex = 2;
        state->unkDF0 = 0;
        state->unkDF4 = 0;
        state->unkDF8 = 0;
        state->unkDFC = 0;
        state->unkE00.x = negX;
        state->unkE00.y = negY;
        state->unkE00.spriteIndex = 0;
        state->unkE00.alpha = alpha;
        state->unkE00.unkC = 0;
        new_var = alpha;
        state->unkE00.unkD = 0;
        state->unkE10.x = negX;
        state->unkE10.y = 0;
        state->unkE10.alpha = new_var;
        state->unkE10.unkC = 0;
        state->unkE10.unkD = 0;
        state->unkE20.x = negX;
        state->unkE20.y = negY;
        state->unkE20.alpha = new_var;
        state->unkE20.unkC = 0;
        state->unkE20.unkD = 0;
        state->unkE30.x = negX;
        state->unkE30.y = 0;
        state->unkE30.spriteIndex = 3;
        state->unkE00.asset = asset0;
        state->unkE10.asset = asset1;
        state->unkE20.asset = asset2;
    } while (0);

    state->unkE30.asset = asset3;
    state->unkE30.alpha = new_var;
    state->unkE30.unkC = 0;
    state->unkE30.unkD = 0;
}

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
        state->unkE10.alpha = shortVal;
        state->unkE00.alpha = shortVal;
        debugEnqueueCallback(1, 4, callback, &state->unkE00);
        debugEnqueueCallback(1, 4, callback, &state->unkE10);
    }

    temp_v0 = state->unkDF8;
    if (temp_v0 != 0) {
        s16 shortVal = temp_v0 >> 16;
        void *callback = func_80012004_12C04;
        state->unkE30.alpha = shortVal;
        state->unkE20.alpha = shortVal;
        debugEnqueueCallback(1, 4, callback, &state->unkE20);
        debugEnqueueCallback(1, 4, callback, &state->unkE30);
    }
}
