#include "15690.h"
#include "common.h"
#include "gamestate.h"
#include "geometry.h"
#include "task_scheduler.h"

extern u8 D_800A8CC8_A0038;
extern s16 identityMatrix[9];
extern void func_800182FC_18EFC(void);
extern void func_80018580_19180(void);

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
    s16 unk46;
    s16 unk48;
    u8 pad4A[0x2];
    s32 unk4C;
    s16 unk50;
    s32 unk54;
    u8 pad58[0x2];
    u8 unk5A;
    u8 unk5B;
} Func80018474Arg;

void func_80017384_17F84(Func80018474Arg *arg0);
void func_800175E0_181E0(void);

void func_80017350_17F50(void) {
    GameState *state = (GameState *)getCurrentAllocation();

    if (state->unk429 == 0) {
        setCallback(&func_80017384_17F84);
    }
}

void func_80017384_17F84(Func80018474Arg *arg0) {
    GameState *state;
    s32 temp;
    s32 x, y;
    u8 mode;

    state = getCurrentAllocation();
    state->unk400 = 4;

    if (D_800A8CC8_A0038 != 0) {
        state->unk425 = D_800A8CC8_A0038 - 1;
        setCallbackWithContinue(&func_800182FC_18EFC);
        return;
    }

    memcpy(&arg0->matrix20, identityMatrix, 0x20);
    memcpy(&arg0->matrix0, identityMatrix, 0x20);

    mode = func_8001523C_15E3C();

    if (mode == 3) {
        arg0->unk34 = 0xFFE60000;
        arg0->unk3C = 0xFF9C0000;
        arg0->unk48 = atan2Fixed(arg0->unk34, arg0->unk3C);

        x = arg0->unk34;
        y = arg0->unk3C;
        arg0->unk4C = isqrt64((s64)x * x + (s64)y * y);

        arg0->unk46 = 0;
        arg0->unk44 = 0;
        state->unk400 = 0;
    } else {
        mode = func_8001523C_15E3C();
        if (mode == 1) {
            arg0->unk48 = 0x14B0;
            arg0->unk4C = 0x650000;
            arg0->unk46 = 0x1800;
            arg0->unk44 = 0x1800;
        } else {
            arg0->unk48 = 0x0B50;
            arg0->unk4C = 0x650000;
            arg0->unk46 = 0x0800;
            arg0->unk44 = 0x0800;
        }

        temp = approximateSin(arg0->unk48);
        temp = temp * -(arg0->unk4C >> 8);
        if (temp < 0) {
            temp += 0x1FFF;
        }
        arg0->unk34 = (temp >> 13) << 8;

        temp = approximateCos(arg0->unk48);
        temp = temp * -(arg0->unk4C >> 8);
        if (temp < 0) {
            temp += 0x1FFF;
        }
        arg0->unk3C = (temp >> 13) << 8;
    }

    state->unk3EC = arg0->unk34;
    state->unk3F0 = arg0->unk3C;
    state->unk3F4 = arg0->unk48;
    state->unk3F8 = arg0->unk4C;
    state->unk3FC = arg0->unk44 & 0x1FFF;

    arg0->unk50 = 0;
    arg0->unk5B = 8;
    arg0->unk5A = 8;

    createYRotationMatrix((Mat3x3Padded *)&arg0->matrix0, arg0->unk44);
    createYRotationMatrix((Mat3x3Padded *)&arg0->matrix20, arg0->unk48);

    mode = func_8001523C_15E3C();
    if (mode == 3) {
        setCallback(&func_800175E0_181E0);
    } else {
        setCallback(&func_80018580_19180);
    }
}

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
