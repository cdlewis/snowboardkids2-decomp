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

s32 checkStoryMapCharacterCollision(s32 posX, s32 posY, s32 characterIndex) {
    GameState *state;
    s32 dx;
    s32 dy;
    s32 dist;
    s32 threshold;

    state = (GameState *)getCurrentAllocation();
    dx = posX - state->unk408[characterIndex];
    dy = posY - state->unk410[characterIndex];
    dist = distance_2d(dx, dy);
    threshold = state->unk3FE + state->unk418[characterIndex];
    if (dist < (threshold << 16)) {
        return characterIndex + 1;
    }
    return 0;
}

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} StoryMapPosition;

typedef struct {
    u8 padding[0x4C];
    s32 distanceFromOrigin;
} StoryMapController;

void resolveStoryMapCharacterCollision(StoryMapController *controller, StoryMapPosition *position, s32 characterIndex) {
    GameState *state;
    s16 angle;
    s32 combinedRadius;
    s32 pushScale;
    s32 sinComponent;
    s32 cosComponent;
    s32 offsetX;
    s32 offsetZ;
    s32 characterX;
    s32 characterZ;

    state = (GameState *)getCurrentAllocation();
    characterIndex = characterIndex - 1;
    angle = func_8006D21C_6DE1C(state->unk408[characterIndex], state->unk410[characterIndex], position->x, position->z);
    combinedRadius = state->unk418[characterIndex] + state->unk3FE;
    combinedRadius <<= 16;
    sinComponent = approximateSin(angle);
    pushScale = -(combinedRadius >> 8);
    sinComponent = sinComponent * pushScale;
    if (sinComponent < 0) {
        sinComponent += 0x1FFF;
    }
    offsetX = (sinComponent >> 13) << 8;
    cosComponent = approximateCos(angle);
    cosComponent = cosComponent * pushScale;
    if (cosComponent < 0) {
        cosComponent += 0x1FFF;
    }
    characterX = state->unk408[characterIndex];
    characterZ = state->unk410[characterIndex];
    offsetX += characterX;
    position->x = offsetX;
    offsetZ = ((cosComponent >> 13) << 8) + characterZ;
    position->z = offsetZ;
    controller->distanceFromOrigin = distance_2d(position->x, offsetZ);
}

s32 checkStoryMapPlayerCollision(s32 posX, s32 posZ, u8 characterIndex) {
    GameState *state;
    s32 dx;
    s32 dz;
    s32 dist;
    s32 threshold;

    state = (GameState *)getCurrentAllocation();
    dx = posX - state->unk3EC;
    dz = posZ - state->unk3F0;
    dist = distance_2d(dx, dz);
    threshold = state->unk3FE + state->unk418[characterIndex];
    return dist < (threshold << 16);
}
