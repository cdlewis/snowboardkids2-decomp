#include "common.h"
#include "gamestate.h"
#include "geometry.h"
#include "task_scheduler.h"

typedef struct {
    s16 x;
    s16 y;
    s16 radius;
} StoryMapHotspot;

extern StoryMapHotspot storyMapHotspots[];

s32 checkStoryMapHotspotCollision(s32 posX, s32 posY, s16 collisionRadius) {
    s32 index;
    s16 *hotspotX;
    s16 *hotspotY;
    s16 *hotspotRadius;
    s32 extraRadius;
    s32 dx;
    s32 dy;
    s32 dist;
    s32 threshold;
    s16 *base;

    getCurrentAllocation();

    index = 0;
    extraRadius = collisionRadius;
    base = (s16 *)storyMapHotspots;
    hotspotRadius = base + 2;
    hotspotY = base + 1;
    hotspotX = base;

    do {
        dx = posX - (*hotspotX << 16);
        dy = posY - (*hotspotY << 16);
        dist = isqrt64((s64)dx * dx + (s64)dy * dy);
        threshold = (*hotspotRadius + extraRadius) << 16;
        if (dist < threshold) {
            return index + 1;
        }
        hotspotRadius += 3;
        hotspotY += 3;
        index += 1;
        hotspotX += 3;
    } while (index < 2);
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/1A110", func_8001960C_1A20C);

s32 func_8001974C_1A34C(s32 arg0, s32 arg1, s32 arg2) {
    GameState *state;
    s32 dx;
    s32 dy;
    s32 dist;
    s32 threshold;

    state = (GameState *)getCurrentAllocation();
    dx = arg0 - state->unk408[arg2];
    dy = arg1 - state->unk410[arg2];
    dist = distance_2d(dx, dy);
    threshold = state->unk3FE + state->unk418[arg2];
    if (dist < (threshold << 16)) {
        return arg2 + 1;
    }
    return 0;
}

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} func_800197D8_1A3D8_arg1;

typedef struct {
    u8 padding[0x4C];
    s32 unk4C;
} func_800197D8_1A3D8_arg0;

void func_800197D8_1A3D8(func_800197D8_1A3D8_arg0 *arg0, func_800197D8_1A3D8_arg1 *arg1, s32 arg2) {
    GameState *state;
    s16 angle;
    s32 rot;
    s32 scale;
    s32 sinVal;
    s32 cosVal;
    s32 newX;
    s32 newY;
    s32 baseX;
    s32 baseY;

    state = (GameState *)getCurrentAllocation();
    arg2 = arg2 - 1;
    angle = func_8006D21C_6DE1C(state->unk408[arg2], state->unk410[arg2], arg1->unk0, arg1->unk8);
    rot = state->unk418[arg2] + state->unk3FE;
    rot <<= 16;
    sinVal = approximateSin(angle);
    scale = -(rot >> 8);
    sinVal = sinVal * scale;
    if (sinVal < 0) {
        sinVal += 0x1FFF;
    }
    newX = (sinVal >> 13) << 8;
    cosVal = approximateCos(angle);
    cosVal = cosVal * scale;
    if (cosVal < 0) {
        cosVal += 0x1FFF;
    }
    baseX = state->unk408[arg2];
    baseY = state->unk410[arg2];
    newX += baseX;
    arg1->unk0 = newX;
    newY = ((cosVal >> 13) << 8) + baseY;
    arg1->unk8 = newY;
    arg0->unk4C = distance_2d(arg1->unk0, newY);
}

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
