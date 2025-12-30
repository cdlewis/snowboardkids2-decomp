#include "D6F0.h"

#include "displaylist.h"
#include "overlay.h"
#include "rand.h"
#include "task_scheduler.h"

USE_ASSET(_647F90);
USE_ASSET(_49B420);

typedef struct {
    void *modelData;
    void *textureData;
    s32 currentTexture;
    u8 padding[0x16];
    s8 animDuration;
    s32 currentTexture2;
    s32 unk28;
    u8 padding2[0x16];
    s8 animDuration2;
    u8 padding3[0x10];
    s16 frameCounter;
    s16 textureIndex;
} SpriteEffectTaskData;

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
    SpriteEffectPositionNode *unk0;
    void *unk4;
    /* 0x8 */ loadAssetMetadata_arg sprite1;
    s32 unk24;
    loadAssetMetadata_arg sprite2;
    u8 padding3[0x4];
    s32 velocityX;
    s32 velocityY;
    s32 velocityZ;
    s16 frameCounter;
    s16 unk56;
} SpriteEffectUpdateData;

typedef struct {
    void *modelData;
    void *textureData;
} SpriteEffectCleanupData;

typedef struct {
    u8 padding[0xC];
    s32 unkC;
    u8 padding2[0x1C];
    s32 unk2C;
    u8 padding3[0x18];
    s32 velocityX;
    s32 velocityY;
    s32 velocityZ;
    s16 frameCounter;
    s16 textureIndex;
} SpriteEffectTaskMemory;

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} Vec3i_SpriteEffect;

void updateSpriteEffectTask(SpriteEffectUpdateData *task);
void cleanupSpriteEffectTask(SpriteEffectCleanupData *);

void initSpriteEffectTask(SpriteEffectTaskData *task) {
    void *textureData;

    task->modelData = loadCompressedData(&_647F90_ROM_START, &_647F90_ROM_END, 0xF18);
    textureData = loadCompressedData(&_49B420_ROM_START, &_49B420_ROM_END, 0x240);
    task->textureData = textureData;
    task->currentTexture = (s32)(textureData + (task->textureIndex << 6));
    task->animDuration = (u8)((randA() & 0x1F) + 0x70);
    task->frameCounter = 0;
    task->unk28 = task->currentTexture;
    task->animDuration2 = (u8)task->animDuration;
    setCleanupCallback(&cleanupSpriteEffectTask);
    setCallbackWithContinue(&updateSpriteEffectTask);
}

void updateSpriteEffectTask(SpriteEffectUpdateData *task) {
    loadAssetMetadata(&task->sprite1, task->unk0, task->frameCounter);

    task->sprite2.data_ptr = task->sprite1.data_ptr;
    task->sprite2.index_ptr = task->sprite1.index_ptr;
    task->sprite2.unk18 = task->sprite1.unk18;
    task->sprite2.unk19 = task->sprite1.unk19;

    func_80067EDC_68ADC(0, &task->sprite1);
    func_80067EDC_68ADC(0, &task->sprite2);

    if (task->frameCounter != 0) {
        s32 i;
        SpriteEffectPositionNode *node = (SpriteEffectPositionNode *)&task->unk0;
        for (i = 0; i < 2; i++) {
            node[i].posX += task->velocityX;
            node[i].posY += task->velocityY;
            node[i].posZ += task->velocityZ;
        }
    }

    task->frameCounter++;
    if (task->frameCounter == 5) {
        func_80069CF8_6A8F8();
    }
}

void cleanupSpriteEffectTask(SpriteEffectCleanupData *task) {
    task->textureData = freeNodeMemory(task->textureData);
    task->modelData = freeNodeMemory(task->modelData);
}

void scheduleSpriteEffectTask(void *startPos, void *endPos, Vec3i_SpriteEffect *velocity, s32 textureIndex) {
    SpriteEffectTaskMemory *task = (SpriteEffectTaskMemory *)scheduleTask(&initSpriteEffectTask, 0, 0, 0);
    if (task != NULL) {
        memcpy(&task->unkC, startPos, 0xC);
        memcpy(&task->unk2C, endPos, 0xC);
        task->textureIndex = textureIndex;
        task->velocityX = (s32)(velocity->x / 2);
        task->velocityY = (s32)(velocity->y / 2);
        task->velocityZ = (s32)(velocity->z / 2);
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
