#include "1DB7A0.h"
#include "10AD0.h"
#include "rom_loader.h"
#include "task_scheduler.h"

USE_ASSET(_426EF0);
USE_ASSET(_42F1D0);
USE_ASSET(_41A1D0);
USE_ASSET(_426EF0);

typedef struct {
    /* 0x000 */ u8 pad0[0x1E2];
    /* 0x1E2 */ u16 unk1E2;
} Allocation_1DB7A0;

extern s16 D_800B09B8_1DC098[];
extern s16 D_800B09BA_1DC09A[];
extern s16 D_800B09A8_1DC088[];

void func_800B0920_1DC000(void *);
void func_800B0968_1DC048(PlayerSelectSpriteTask *);
void func_800B0804_1DBEE4(PlayerSelectSpriteTask *);
void func_800B0774_1DBE54(PlayerSelectSpriteTask *);
void updatePlayerCountHeaderSprite(PlayerSelectSpriteTask *);
void updatePlayerSelectAnim(PlayerSelectState *);
void cleanupPlayerSelectTask(PlayerSelectSpriteTask *);

void initPlayerSelectSprites(PlayerSelectState *state) {
    Allocation_1DB7A0 *allocation;
    void *spriteData;
    s32 i;
    s32 yPos;
    s16 scale;
    s16 alpha;
    volatile PlayerSelectSprite *sprite;

    allocation = getCurrentAllocation();
    spriteData = loadCompressedData(&_426EF0_ROM_START, &_42F1D0_ROM_START, 0xEEE8);
    setCleanupCallback(cleanupPlayerSelectTask);

    i = 0;
    scale = 0x400;
    alpha = 0xFF;
    yPos = -0x9C;
    sprite = (volatile PlayerSelectSprite *)state;
    do {
        s32 baseFrame;
        s16 x;
        s32 slotIndex;

        x = (state->slotIndex << 6) - 0x60;
        sprite->y = yPos;
        yPos += 0x20;
        sprite->x = x;
        slotIndex = state->slotIndex;
        baseFrame = i + 8;
        i++;
        sprite->scaleX = scale;
        sprite->scaleY = scale;
        sprite->alpha = alpha;
        sprite->flags = 0;
        sprite->unk13 = 0;
        sprite->unk12 = 0;
        sprite->spriteData = spriteData;
        sprite->frameIndex = baseFrame + slotIndex * 6;
        sprite++;
    } while (i < 2);

    state->unk2C = 0;
    state->unk2D = 0;
    {
        u16 playerIdx = allocation->unk1E2;
        state->animState = 0;
        state->animCounter = 0;
        state->playerIndex = playerIdx;
    }

    setCallback(updatePlayerSelectAnim);
}

INCLUDE_ASM("asm/nonmatchings/1DB7A0", updatePlayerSelectAnim);

void cleanupPlayerSelectTask(PlayerSelectSpriteTask *arg0) {
    arg0->spriteData = freeNodeMemory(arg0->spriteData);
}

void func_800B054C_1DBC2C(PlayerSelectSpriteTask *arg0) {
    void *temp;
    s32 i;
    s16 v0, v2;

    getCurrentAllocation();
    temp = loadCompressedData(&_426EF0_ROM_START, &_426EF0_ROM_END, 0xEEE8);
    setCleanupCallback(cleanupPlayerCountSelectSprites);

    for (i = 0; i < 4; i++) {
        v0 = D_800B09A8_1DC088[i * 2];
        arg0[i].x = v0;
        v2 = D_800B09A8_1DC088[i * 2 + 1];
        arg0[i].frameIndex = i / 2;
        arg0[i].playerIndex = 0;
        arg0[i].slotIndex = i & 1;
        arg0[i].alpha = 0xFF;
        arg0[i].spriteData = temp;
        arg0[i].y = v2;
    }

    setCallback(enqueuePlayerSelectSpritesRender);
}

void enqueuePlayerSelectSpritesRender(PlayerSelectSpriteTask *arg0) {
    s32 i;

    for (i = 0; i < 4; i++) {
        debugEnqueueCallback(8, 0, func_80012004_12C04, &arg0[i]);
    }
}

void cleanupPlayerCountSelectSprites(PlayerSelectSpriteTask *arg0) {
    arg0->spriteData = freeNodeMemory(arg0->spriteData);
}

void initPlayerCountHeaderSprite(PlayerSelectSpriteTask *arg0) {
    void *spriteData;

    getCurrentAllocation();
    spriteData = loadCompressedData(&_426EF0_ROM_START, &_426EF0_ROM_END, 0xEEE8);
    setCleanupCallback(func_800B0774_1DBE54);
    arg0->x = -96;
    arg0->y = -80;
    arg0->frameIndex = 6;
    arg0->spriteData = spriteData;
    setCallback(updatePlayerCountHeaderSprite);
}

void updatePlayerCountHeaderSprite(PlayerSelectSpriteTask *arg0) {
    GameState *allocation = getCurrentAllocation();

    if (allocation->unk1E6 == 10) {
        arg0->x = -80;
        arg0->frameIndex = 7;
    } else {
        arg0->x = -96;
        arg0->frameIndex = 6;
    }
    debugEnqueueCallback(8, 0, func_8000FED0_10AD0, arg0);
}

void func_800B0774_1DBE54(PlayerSelectSpriteTask *arg0) {
    arg0->spriteData = freeNodeMemory(arg0->spriteData);
}

void func_800B07A0_1DBE80(PlayerSelectSpriteTask *arg0) {
    void *temp;

    getCurrentAllocation();
    temp = loadCompressedData(&_426EF0_ROM_START, &_42F1D0_ROM_START, 0xEEE8);
    setCleanupCallback(func_800B0774_1DBE54);
    arg0->spriteData = temp;
    setCallback(func_800B0804_1DBEE4);
}

void func_800B0804_1DBEE4(PlayerSelectSpriteTask *arg0) {
    s32 pad[2];
    Allocation_1DB7A0 *allocation = getCurrentAllocation();

    arg0->x = D_800B09B8_1DC098[allocation->unk1E2 * 2];
    arg0->y = D_800B09BA_1DC09A[allocation->unk1E2 * 2];
    arg0->frameIndex = allocation->unk1E2 + 2;

    debugEnqueueCallback(8, 7, func_8000FED0_10AD0, arg0);
}

void func_800B087C_1DBF5C(PlayerSelectSpriteTask *arg0) {
    arg0->spriteData = freeNodeMemory(arg0->spriteData);
}

void func_800B08A8_1DBF88(PlayerSelectSpriteTask *arg0) {
    void *temp;

    temp = loadCompressedData(&_41A1D0_ROM_START, &_41A1D0_ROM_END, 0x1B48);
    setCleanupCallback(func_800B0968_1DC048);
    arg0->x = -0x2C;
    arg0->y = -0x14;
    arg0->frameIndex = 0xD;
    arg0->spriteData = temp;
    setCallback(func_800B0920_1DC000);
}

void func_800B0920_1DC000(void *arg0) {
    GameState *allocation = getCurrentAllocation();

    if (allocation->unk1E6 == 2) {
        debugEnqueueCallback(8, 7, func_8000FED0_10AD0, arg0);
    }
}

void func_800B0968_1DC048(PlayerSelectSpriteTask *arg0) {
    arg0->spriteData = freeNodeMemory(arg0->spriteData);
}
