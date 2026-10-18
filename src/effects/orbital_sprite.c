#include "effects/orbital_sprite.h"
#include "common.h"
#include "graphics/graphics.h"
#include "graphics/sprite_table.h"
#include "math/geometry.h"
#include "math/rand.h"
#include "system/task_scheduler.h"
#include "ui/level_preview_3d.h"

#define ORBITAL_SPRITE_COUNT 12
#define ORBITAL_SPRITE_Y_OFFSET 0x16147A

s32 orbitalSpriteOffsetsZ[] = { 0x00100000 };

s32 orbitalSpriteOffsetsX[] = { 0xFFFAE667, 0x00109999, 0x0005CCCC, 0xFFEF3334, 0xFFFA6667, 0xFFF00000,
                                0x00051999, 0xFFF33334, 0x000CCCCC, 0xFFF33334, 0xFFF33334, 0x000CCCCC,
                                0xFFF33334, 0x000CCCCC, 0x000CCCCC, 0x00030000, 0xFFEECCCD, 0xFFFBB334,
                                0xFFEECCCD, 0xFFFC8000, 0x00119999, 0x00038000, 0x00119999 };

s32 maxActiveOrbitalSprites = 12;

typedef struct {
    SceneModel *model;
    SpriteAssetState spriteState;
    s32 spriteIndex;
    s16 delayTimer;
    s8 isActive;
} OrbitalSpriteState;

typedef struct {
    SceneModel *model;
    SpriteAssetState spriteState;
} OrbitalSpriteRingControllerState;

void initOrbitalSprite(OrbitalSpriteState *);
void updateOrbitalSprite(OrbitalSpriteState *);
void cleanupOrbitalSprite(OrbitalSpriteState *);
void updateOrbitalSpriteRingController(OrbitalSpriteRingControllerState *);
void cleanupOrbitalSpriteRingController(OrbitalSpriteRingControllerState *);

void initOrbitalSpriteRing(OrbitalSpriteRingInitArg *arg0) {
    s32 i;
    OrbitalSpriteState *task;
    s32 modelAddress;

    loadSpriteAsset(&arg0->spriteState, 5);

    for (i = 0; i < ORBITAL_SPRITE_COUNT; i++) {
        task = scheduleTask(initOrbitalSprite, 0, 0, 0);
        if (task != NULL) {
            modelAddress = arg0->modelAddress;
            task->spriteIndex = i;
            task->model = (SceneModel *)modelAddress;
        }
    }

    setCleanupCallback(cleanupOrbitalSpriteRingController);
    setCallback(updateOrbitalSpriteRingController);
}

void updateOrbitalSpriteRingController(OrbitalSpriteRingControllerState *arg0) {
    if (arg0->model->isDestroyed == 1) {
        terminateCurrentTask();
    }
}

void cleanupOrbitalSpriteRingController(OrbitalSpriteRingControllerState *arg0) {
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

    if (arg0->model->isDestroyed == 1) {
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

    transformVector2(&localOffset, arg0->model->transform.m, &worldOffset);

    x = arg0->model->transform.translation.x + worldOffset.x;
    y = arg0->model->transform.translation.y + worldOffset.y;
    z = arg0->model->transform.translation.z + worldOffset.z;

    updateSpriteAnimation(&arg0->spriteState, 0x10000);

    if (arg0->model->visibilityEnabled == 0) {
        return;
    }

    if (arg0->model->displayEnabled == 0) {
        return;
    }

    renderSprite(&arg0->spriteState, arg0->model->viewport->callbackSlotIndex, x, y, z, 0x4000, 0x4000, 0, 0, 0xAA);
}

void cleanupOrbitalSprite(OrbitalSpriteState *arg0) {
    releaseNodeMemoryRef((void **)&arg0->spriteState);
}
