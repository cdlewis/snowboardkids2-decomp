#include "common.h"
#include "displaylist.h"
#include "overlay.h"
#include "rand.h"
#include "task_scheduler.h"

USE_ASSET(_647F90);

extern void *staticSpriteEffectTexture;

void cleanupStaticSpriteEffectTask(void **);

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 posX;
    s32 posY;
    s32 posZ;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
} StaticSpriteEffectPositionNode;

typedef struct {
    void *modelData;
    loadAssetMetadata_arg sprite1;
    u8 padding[4];
    loadAssetMetadata_arg sprite2;
    u8 padding2[4];
    s32 velocityX;
    s32 velocityY;
    s32 velocityZ;
    s16 frameCounter;
} StaticSpriteEffectUpdateData;

void updateStaticSpriteEffectTask(StaticSpriteEffectUpdateData *);

typedef struct {
    void *modelData;
    void *textureData;
    u8 padding[0x16];
    u8 animDuration;
    u8 padding2[0x5];
    void *textureData2;
    u8 padding3[0x16];
    u8 animDuration2;
    u8 padding4[0x11];
    s16 frameCounter;
} StaticSpriteEffectTaskData;

typedef struct {
    u8 padding[0x8];
    u8 startPos[0xC];
    u8 padding2[0x14];
    u8 endPos[0xC];
    u8 padding3[0x10];
    s32 velocityX;
    s32 velocityY;
    s32 velocityZ;
    u8 padding4[0x4];
    s16 frameCounter;
} StaticSpriteEffectTaskMemory;

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} Vec3i_StaticSpriteEffect;

void initStaticSpriteEffectTask(StaticSpriteEffectTaskData *arg0) {
    arg0->modelData = loadCompressedData(&_647F90_ROM_START, &_647F90_ROM_END, 0xF18);
    arg0->textureData = &staticSpriteEffectTexture;
    arg0->animDuration = (randA() & 0x1F) + 0x70;
    arg0->frameCounter = 0;
    arg0->textureData2 = arg0->textureData;
    arg0->animDuration2 = arg0->animDuration;
    setCleanupCallback(&cleanupStaticSpriteEffectTask);
    setCallbackWithContinue(&updateStaticSpriteEffectTask);
}

void updateStaticSpriteEffectTask(StaticSpriteEffectUpdateData *arg0) {
    s32 i;
    loadAssetMetadata_arg *sprite1_ptr = &arg0->sprite1;

    loadAssetMetadata(sprite1_ptr, arg0->modelData, arg0->frameCounter / 4);

    arg0->sprite2.data_ptr = arg0->sprite1.data_ptr;
    arg0->sprite2.index_ptr = arg0->sprite1.index_ptr;
    arg0->sprite2.unk18 = arg0->sprite1.unk18;
    arg0->sprite2.unk19 = arg0->sprite1.unk19;

    func_80067EDC_68ADC(0, sprite1_ptr);
    func_80067EDC_68ADC(0, &arg0->sprite2);

    if (arg0->frameCounter != 0) {
        StaticSpriteEffectPositionNode *node = (StaticSpriteEffectPositionNode *)arg0;
        for (i = 0; i < 2; i++) {
            node[i].posX += arg0->velocityX;
            node[i].posY += arg0->velocityY;
            node[i].posZ += arg0->velocityZ;
        }
    }

    arg0->frameCounter++;
    if (arg0->frameCounter == 20) {
        func_80069CF8_6A8F8();
    }
}

void cleanupStaticSpriteEffectTask(void **arg0) {
    *arg0 = freeNodeMemory(*arg0);
}

void scheduleStaticSpriteEffectTask(void *startPos, void *endPos, Vec3i_StaticSpriteEffect *velocity) {
    StaticSpriteEffectTaskMemory *task =
        (StaticSpriteEffectTaskMemory *)scheduleTask(&initStaticSpriteEffectTask, 0, 0, 0);
    if (task != NULL) {
        memcpy(&task->startPos, startPos, 0xC);
        memcpy(&task->endPos, endPos, 0xC);
        task->frameCounter = 0;
        task->velocityX = velocity->x / 2;
        task->velocityY = velocity->y / 2;
        task->velocityZ = velocity->z / 2;
    }
}
