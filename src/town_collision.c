#include "common.h"
#include "gamestate.h"
#include "geometry.h"
#include "task_scheduler.h"

// Defines circular collision zones for town objects (lampposts/stations)
// Note: y corresponds to the depth axis (z in 3D space)
typedef struct {
    s16 x;
    s16 y;      // depth/forward-backward position
    s16 radius; // collision boundary radius
} TownLamppost;

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} TownPosition;

// Two lamppost/station bases in the town.
// Positioned between the player and the back wall (y = -83 is forward depth).
// Left station (-56) is a level entry point; right station (56) is decorative/functional.
TownLamppost g_TownLampposts[] = {
    { .x = -56, .y = -83, .radius = 4 },
    { .x = 56,  .y = -83, .radius = 4 },
};

// Checks if the player is colliding with any lamppost/station in the town.
// Returns 1-based lamppost index (1 or 2) if colliding, 0 if no collision.
// posX, posY: player position in 16.16 fixed point
// collisionRadius: player's collision radius
s32 checkTownLamppostCollision(s32 posX, s32 posY, s16 collisionRadius) {
    s32 index;
    s16 *lamppostX;
    s16 *lamppostY;
    s16 *lamppostRadius;
    s32 extraRadius;
    s32 dx;
    s32 dy;
    s32 dist;
    s32 threshold;
    s16 *base;

    getCurrentAllocation();

    index = 0;
    extraRadius = collisionRadius;
    base = (s16 *)g_TownLampposts;
    lamppostRadius = base + 2;
    lamppostY = base + 1;
    lamppostX = base;

    do {
        dx = posX - (*lamppostX << 16);
        dy = posY - (*lamppostY << 16);
        dist = isqrt64((s64)dx * dx + (s64)dy * dy);
        threshold = (*lamppostRadius + extraRadius) << 16;
        if (dist < threshold) {
            return index + 1;
        }
        lamppostRadius += 3;
        lamppostY += 3;
        index += 1;
        lamppostX += 3;
    } while (index < 2);
    return 0;
}

// Resolves collision between player and a lamppost/station in the town.
// Pushes the player to the edge of the collision boundary (bumper effect).
// position: player position (will be modified)
// collisionRadius: player's collision radius
// lamppostIndex: 1-based index of lamppost being collided with
void resolveTownLamppostCollision(TownPosition *position, s16 collisionRadius, s32 lamppostIndex) {
    s16 *lamppostY;
    TownLamppost *lamppost;
    s16 *base;
    s16 angle;
    s32 offset;
    s16 *baseY;
    TownPosition *pos;
    s16 *radiusPtr;
    s32 combinedRadius;
    s32 yOffset;
    s32 zValue;
    s32 pushScale;
    s32 sinComponent;
    s32 cosComponent;
    s32 sinTemp;

    getCurrentAllocation();
    lamppostIndex = lamppostIndex - 1;
    base = (s16 *)g_TownLampposts;
    lamppost = (TownLamppost *)(base + (lamppostIndex * 3));
    offset = lamppostIndex * 3;
    baseY = base;
    yOffset = 1;
    lamppostY = offset + (baseY + yOffset);
    pos = position;
    sinComponent = pos->x;
    base = &lamppost->x;
    angle = func_8006D21C_6DE1C(*base << 16, *lamppostY << 16, sinComponent, position->z);
    radiusPtr = &(g_TownLampposts + lamppostIndex)->radius;
    combinedRadius = *radiusPtr + collisionRadius;
    combinedRadius <<= 16;
    sinTemp = approximateSin(angle);
    combinedRadius = -(combinedRadius >> 8);
    pushScale = combinedRadius;
    sinTemp = sinTemp * pushScale;
    if (sinTemp < 0) {
        sinTemp += 0x1FFF;
    }
    sinComponent = (sinTemp >> 13) << 8;
    cosComponent = approximateCos(angle);
    cosComponent = cosComponent * pushScale;
    pushScale = 0;
    if (cosComponent < pushScale) {
        cosComponent += 0x1FFF;
    }
    sinComponent += lamppost->x << 16;
    zValue = ((cosComponent >> 13) << 8) + (*lamppostY << 16);
    position->x = sinComponent;
    position->z = zValue;
}

// Checks if the player is colliding with an NPC in the town.
// Returns 1-based character index if colliding, 0 if no collision.
// posX, posY: player position in 16.16 fixed point
// characterIndex: which NPC to check against
s32 checkTownNPCCollision(s32 posX, s32 posY, s32 characterIndex) {
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
    u8 padding[0x4C];
    s32 distanceFromOrigin;
} TownController;

// Resolves collision between player and an NPC in the town.
// Pushes the player to the edge of the NPC's collision boundary.
// controller: town controller state (distanceFromOrigin will be updated)
// position: player position (will be modified)
// characterIndex: 1-based index of NPC being collided with
void resolveTownNPCCollision(TownController *controller, TownPosition *position, s32 characterIndex) {
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

// Checks if an NPC is colliding with the player in the town.
// Returns true if NPC is within collision range of player.
// posX, posZ: NPC position in 16.16 fixed point
// characterIndex: which NPC's radius to use for collision check
s32 checkTownPlayerCollision(s32 posX, s32 posZ, u8 characterIndex) {
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
