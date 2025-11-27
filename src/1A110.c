#include "common.h"
#include "gamestate.h"
#include "task_scheduler.h"

extern s32 distance_2d(s32, s32);

INCLUDE_ASM("asm/nonmatchings/1A110", func_80019510_1A110);

INCLUDE_ASM("asm/nonmatchings/1A110", func_8001960C_1A20C);

INCLUDE_ASM("asm/nonmatchings/1A110", func_8001974C_1A34C);

INCLUDE_ASM("asm/nonmatchings/1A110", func_800197D8_1A3D8);

s32 func_800198F0_1A4F0(s32 arg0, s32 arg1, u8 arg2) {
    GameState *state;
    s32 dx;
    s32 dy;
    s32 dist;
    s32 threshold;

    state = (GameState *)getCurrentAllocation();
    dx = arg0 - state->unk3EC;
    dy = arg1 - state->unk3F0;
    dist = distance_2d(dx, dy);
    threshold = state->unk3FE + state->unk418[arg2];
    return dist < (threshold << 16);
}
