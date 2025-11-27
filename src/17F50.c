#include "common.h"
#include "gamestate.h"
#include "geometry.h"
#include "task_scheduler.h"

extern s32 approximateSin(s16);
extern s32 approximateCos(s16);
extern s32 isqrt64(s64 val);
extern s16 atan2Fixed(s32, s32);
extern u8 D_800A8CC8_A0038;

typedef struct {
    s16 matrix0[9];
    u8 pad12[0x2];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s16 matrix20[9];
    u8 pad32[0x2];
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    u8 pad40[0x4];
    u16 unk44;
    u8 pad46[0x2];
    s16 unk48;
    u8 pad4A[0x2];
    s32 unk4C;
    s32 unk50;
    s32 unk54;
} Func80018474Arg;

void func_80017384_17F84(void);
void func_800175E0_181E0(void);

void func_80017350_17F50(void) {
    GameState *state = (GameState *)getCurrentAllocation();

    if (state->unk429 == 0) {
        setCallback(&func_80017384_17F84);
    }
}

INCLUDE_ASM("asm/nonmatchings/17F50", func_80017384_17F84);

INCLUDE_ASM("asm/nonmatchings/17F50", func_800175E0_181E0);

INCLUDE_ASM("asm/nonmatchings/17F50", func_80017FE8_18BE8);

void func_800182F4_18EF4(void);

void func_80018148_18D48(Func80018474Arg *arg0) {
    GameState *state;
    Mat3x3Padded localMatrix;
    s32 velocity;
    s32 temp;
    s16 angle;

    state = getCurrentAllocation();

    velocity = arg0->unk54;
    if (velocity < 0) {
        velocity += 0x1F;
    }
    velocity >>= 9;

    temp = approximateSin(arg0->unk44) * velocity;
    if (temp < 0) {
        temp += 0x1FFF;
    }
    arg0->unk34 = arg0->unk34 + ((temp >> 13) << 4);

    temp = approximateCos(arg0->unk44) * velocity;
    if (temp < 0) {
        temp += 0x1FFF;
    }
    arg0->unk3C = arg0->unk3C + ((temp >> 13) << 4);

    angle = atan2Fixed(arg0->unk34, arg0->unk3C);

    createYRotationMatrix((Mat3x3Padded *)&arg0->matrix20, (u16)angle);
    createYRotationMatrix((Mat3x3Padded *)arg0, (u16)(arg0->unk44 - angle));

    func_8006B084_6BC84(arg0, &arg0->matrix20, &localMatrix);

    memcpy(&state->unk3B0, &localMatrix, 0x20);

    state->unk3EC = arg0->unk34;
    state->unk3F0 = arg0->unk3C;

    if (func_8006FE10_70A10(0) == 0) {
        D_800A8CC8_A0038 = state->unk425;
        state->unk427 = state->unk425 + 1;
        setCallback(&func_800182F4_18EF4);
    }
}

void func_8001829C_18E9C(void) {
    GameState *state = (GameState *)getCurrentAllocation();

    if (func_8006FE10_70A10(0) == 0) {
        D_800A8CC8_A0038 = state->unk425;
        state->unk427 = state->unk425 + 1;
        setCallback(&func_800182F4_18EF4);
    }
}

void func_800182F4_18EF4(void) {
}

INCLUDE_ASM("asm/nonmatchings/17F50", func_800182FC_18EFC);

void func_80018474_19074(Func80018474Arg *arg0) {
    GameState *state;
    Mat3x3Padded localMatrix;
    s32 sinVal;
    s32 cosVal;
    s32 x, y;
    s64 sumSquares;
    s32 distance;

    state = getCurrentAllocation();

    sinVal = approximateSin(arg0->unk48);
    arg0->unk34 = arg0->unk34 + (sinVal << 4);

    cosVal = approximateCos(arg0->unk48);
    arg0->unk3C = arg0->unk3C + (cosVal << 4);

    func_8006B084_6BC84(arg0, &arg0->matrix20, &localMatrix);

    state->unk3EC = arg0->unk34;
    state->unk3F0 = arg0->unk3C;

    memcpy(&state->unk3B0, &localMatrix, 0x20);

    state->unk3F4 = arg0->unk48;
    state->unk3F8 = arg0->unk4C;
    state->unk3FC = arg0->unk44 & 0x1FFF;

    x = arg0->unk34;
    y = arg0->unk3C;
    sumSquares = (s64)x * x + (s64)y * y;
    distance = isqrt64(sumSquares);

    if (distance <= 0x880000) {
        setCallback(&func_800175E0_181E0);
    }
}

INCLUDE_ASM("asm/nonmatchings/17F50", func_80018580_19180);

void func_800187DC_193DC(void);

void func_80018798_19398(void) {
    GameState *state = (GameState *)getCurrentAllocation();

    if (func_8006FE10_70A10(0) == 0) {
        state->unk427 = 0xFF;
        setCallbackWithContinue(&func_800187DC_193DC);
    }
}

void func_800187DC_193DC(void) {
}

s32 abs(s32 n) {
    if (n < 0) {
        n = -n;
    }
    return n;
}
