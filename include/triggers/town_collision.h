#include "common.h"
#include "math/geometry.h"

typedef struct {
    u8 padding[0x4C];
    s32 distanceFromOrigin;
} TownController;

s32 checkTownPlayerCollision(s32 posX, s32 posZ, u8 characterIndex);
s32 checkTownLamppostCollision(s32 posX, s32 posY, s16 collisionRadius);
void resolveTownLamppostCollision(Vec3i *position, s16 collisionRadius, s32 lamppostIndex);
s32 checkTownNPCCollision(s32 posX, s32 posY, s32 characterIndex);
void resolveTownNPCCollision(TownController *controller, Vec3i *position, s32 characterIndex);
