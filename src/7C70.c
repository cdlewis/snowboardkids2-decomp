#include "90F0.h"
#include "common.h"
#include "geometry.h"
#include "rand.h"
#include "task_scheduler.h"

#define ORBITAL_SPRITE_COUNT 12
#define ORBITAL_SPRITE_Y_OFFSET 0x16147A

s32 orbitalSpriteOffsetsZ[] = { 0x00100000 };

s32 orbitalSpriteOffsetsX[] = { 0xFFFAE667, 0x00109999, 0x0005CCCC, 0xFFEF3334, 0xFFFA6667, 0xFFF00000,
                                0x00051999, 0xFFF33334, 0x000CCCCC, 0xFFF33334, 0xFFF33334, 0x000CCCCC,
                                0xFFF33334, 0x000CCCCC, 0x000CCCCC, 0x00030000, 0xFFEECCCD, 0xFFFBB334,
                                0xFFEECCCD, 0xFFFC8000, 0x00119999, 0x00038000, 0x00119999 };

s32 maxActiveOrbitalSprites = 12;

typedef struct {
    u8 _pad[0x16];
    u16 assetIndex;
} OrbitalSpriteAssetData;

typedef struct {
    u8 _pad[0x10];
    OrbitalSpriteAssetData *assetData;
    u8 _pad2[0x4];
    s32 rotationMatrix[3];
    u8 _pad3[0x8];
    s32 posX;
    s32 posY;
    s32 posZ;
    u8 _pad4[0x4];
    s8 isDestroyed;
    u8 _pad5[0x2];
    s8 displayEnabled;
    u8 _pad6[0x48];
    s8 isVisible;
} OrbitalSpriteOwner;

typedef struct {
    OrbitalSpriteOwner *owner;
    SpriteAssetState spriteState;
    s32 spriteIndex;
    s16 delayTimer;
    s8 isActive;
} OrbitalSpriteState;

typedef struct {
    OrbitalSpriteOwner *owner;
} OrbitalSpriteRingControllerState;

typedef struct {
    s32 owner;
    SpriteAssetState spriteState;
} OrbitalSpriteRingInitArg;

void initOrbitalSprite(OrbitalSpriteState *);
void updateOrbitalSprite(OrbitalSpriteState *);
void cleanupOrbitalSprite(OrbitalSpriteState *);
void updateOrbitalSpriteRingController(OrbitalSpriteRingControllerState *);
void cleanupOrbitalSpriteRingController(OrbitalSpriteState *);

void initOrbitalSpriteRing(OrbitalSpriteRingInitArg *arg0) {
    s32 i;
    OrbitalSpriteState *task;
    s32 temp;

    loadSpriteAsset(&arg0->spriteState, 5);

    for (i = 0; i < ORBITAL_SPRITE_COUNT; i++) {
        task = scheduleTask(initOrbitalSprite, 0, 0, 0);
        if (task != NULL) {
            temp = arg0->owner;
            task->spriteIndex = i;
            task->owner = (OrbitalSpriteOwner *)temp;
        }
    }

    setCleanupCallback(cleanupOrbitalSpriteRingController);
    setCallback(updateOrbitalSpriteRingController);
}

void updateOrbitalSpriteRingController(OrbitalSpriteRingControllerState *arg0) {
    if (arg0->owner->isDestroyed == 1) {
        terminateCurrentTask();
    }
}

void cleanupOrbitalSpriteRingController(OrbitalSpriteState *arg0) {
    releaseNodeMemoryRef((void **)&arg0->spriteState);
}

void initOrbitalSprite(OrbitalSpriteState *arg0) {
    SpriteAssetState *temp_s0;

    temp_s0 = &arg0->spriteState;
    loadSpriteAsset(temp_s0, 5);
    setSpriteAnimation(temp_s0, 0x10000, 0, -1);
    arg0->isActive = 0;
    arg0->delayTimer = randA() % 15;
    setCleanupCallback(cleanupOrbitalSprite);
    setCallback(updateOrbitalSprite);
}

void updateOrbitalSprite(OrbitalSpriteState *arg0) {
    Vec3i localOffset;
    Vec3i worldOffset;
    s32 x, y, z;

    if (arg0->owner->isDestroyed == 1) {
        terminateCurrentTask();
        return;
    }

    if (arg0->isActive == 0) {
        if (arg0->delayTimer == 0) {
            arg0->isActive = 1;
            return;
        }
        arg0->delayTimer = arg0->delayTimer - 1;
        return;
    }

    if (arg0->spriteIndex >= maxActiveOrbitalSprites) {
        return;
    }

    localOffset.x = orbitalSpriteOffsetsX[arg0->spriteIndex * 2];
    localOffset.y = ORBITAL_SPRITE_Y_OFFSET;
    localOffset.z = orbitalSpriteOffsetsZ[arg0->spriteIndex * 2];

    transformVector2(&localOffset, &arg0->owner->rotationMatrix[0], &worldOffset);

    x = arg0->owner->posX + worldOffset.x;
    y = arg0->owner->posY + worldOffset.y;
    z = arg0->owner->posZ + worldOffset.z;

    updateSpriteAnimation(&arg0->spriteState, 0x10000);

    if (arg0->owner->isVisible == 0) {
        return;
    }

    if (arg0->owner->displayEnabled == 0) {
        return;
    }

    renderSprite(&arg0->spriteState, arg0->owner->assetData->assetIndex, x, y, z, 0x4000, 0x4000, 0, 0, 0xAA);
}

void cleanupOrbitalSprite(OrbitalSpriteState *arg0) {
    releaseNodeMemoryRef((void **)&arg0->spriteState);
}
