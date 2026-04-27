#include "effects/animated_sprite_effect.h"

#include "assets.h"
#include "graphics/displaylist.h"
#include "math/geometry.h"
#include "math/rand.h"
#include "system/task_scheduler.h"

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 posX;
    s32 posY;
    s32 posZ;
    s32 unk18;
    s32 unk1C;
} SpriteEffectPositionNode;

typedef struct {
    /* 0x00 */ void *modelData;
    /* 0x04 */ void *textureData;
    /* 0x08 */ loadAssetMetadata_arg sprite1;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ loadAssetMetadata_arg sprite2;
    /* 0x44 */ u8 padding[0x4];
    /* 0x48 */ s32 velocityX;
    /* 0x4C */ s32 velocityY;
    /* 0x50 */ s32 velocityZ;
    /* 0x54 */ s16 frameCounter;
    /* 0x56 */ s16 textureIndex;
} SpriteEffectTask;

void updateSpriteEffectTask(SpriteEffectTask *task);
void cleanupSpriteEffectTask(SpriteEffectTask *task);

void initSpriteEffectTask(SpriteEffectTask *task) {
    void *textureData;

    task->modelData = loadCompressedData(&_647F90_ROM_START, &_647F90_ROM_END, 0xF18);
    textureData = loadCompressedData(&_49B420_ROM_START, &_49B420_ROM_END, 0x240);
    task->textureData = textureData;
    task->sprite1.assetTemplate = (loadAssetMetadata_arg *)((u8 *)textureData + (task->textureIndex << 6));
    task->sprite1.alpha = (randA() & 0x1F) + 0x70;
    task->frameCounter = 0;
    task->sprite2.assetTemplate = task->sprite1.assetTemplate;
    task->sprite2.alpha = task->sprite1.alpha;
    setCleanupCallback(&cleanupSpriteEffectTask);
    setCallbackWithContinue(&updateSpriteEffectTask);
}

void updateSpriteEffectTask(SpriteEffectTask *task) {
    loadAssetMetadata(&task->sprite1, task->modelData, task->frameCounter);

    task->sprite2.data_ptr = task->sprite1.data_ptr;
    task->sprite2.index_ptr = task->sprite1.index_ptr;
    task->sprite2.unk18 = task->sprite1.unk18;
    task->sprite2.unk19 = task->sprite1.unk19;

    enqueueAlphaSprite(0, &task->sprite1);
    enqueueAlphaSprite(0, &task->sprite2);

    if (task->frameCounter != 0) {
        s32 i;
        SpriteEffectPositionNode *node = (SpriteEffectPositionNode *)task;
        for (i = 0; i < 2; i++) {
            node[i].posX += task->velocityX;
            node[i].posY += task->velocityY;
            node[i].posZ += task->velocityZ;
        }
    }

    task->frameCounter++;
    if (task->frameCounter == 5) {
        terminateCurrentTask();
    }
}

void cleanupSpriteEffectTask(SpriteEffectTask *task) {
    task->textureData = freeNodeMemory(task->textureData);
    task->modelData = freeNodeMemory(task->modelData);
}

void scheduleSpriteEffectTask(void *startPos, void *endPos, Vec3i *velocity, s32 textureIndex) {
    SpriteEffectTask *task = (SpriteEffectTask *)scheduleTask(&initSpriteEffectTask, 0, 0, 0);
    if (task != NULL) {
        memcpy(&task->sprite1.position, startPos, sizeof(Vec3i));
        memcpy(&task->sprite2.position, endPos, sizeof(Vec3i));
        task->textureIndex = textureIndex;
        task->velocityX = velocity->x / 2;
        task->velocityY = velocity->y / 2;
        task->velocityZ = velocity->z / 2;
    }
}

void *loadSpriteEffectModelData(void) {
    return loadCompressedData(&_647F90_ROM_START, &_647F90_ROM_END, 0xF18);
}

void *loadSpriteEffectTextureData(void) {
    return loadCompressedData(&_49B420_ROM_START, &_49B420_ROM_END, 0x240);
}

void *freeSpriteEffectModelData(void *data) {
    return freeNodeMemory(data);
}

void *freeSpriteEffectTextureData(void *data) {
    return freeNodeMemory(data);
}
