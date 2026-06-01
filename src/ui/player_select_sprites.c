#include "ui/player_select_sprites.h"
#include "assets.h"
#include "gamestate.h"
#include "graphics/graphics.h"
#include "graphics/sprite_rdp.h"
#include "system/task_scheduler.h"
#include "ui/player_select.h"

u16 D_800B09A0_1DC080[] = { 0x0000, 0x0001, 0x0002, 0x0001 };
s16 gPlayerCountOptionPositions[] = { 0xFFC0, 0x0020, 0x0000, 0x0020, 0xFF98, 0x0040, 0x0028, 0x0040 };
s16 gPlayerCountIndicatorPositions[] = { 0xFFD7, 0x0024, 0xFFD7, 0x0024, 0xFFC8, 0x0026,
                                         0xFFD8, 0x0024, 0x0000, 0x0000, 0x0000, 0x0000 };

void cleanupPlayerSelectTask(PlayerSelectSpriteTask *);

void initPlayerSelectSprites(PlayerSelectState *state) {
    PlayerCountSelectState *allocation;
    void *spriteData;
    s32 i;
    s32 yPos;
    s16 scale;
    s16 alpha;
    volatile PlayerSelectSprite *sprite;

    allocation = getCurrentAllocation();
    spriteData = loadCompressedData(&playerCountSelectSprites_ROM_START, &playerCountSelectSprites_ROM_END, 0xEEE8);
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
        u16 playerIdx = allocation->playerCount.selectedPlayerIndex;
        state->animState = 0;
        state->animCounter = 0;
        state->playerIndex = playerIdx;
    }

    setCallback(updatePlayerSelectAnim);
}

void updatePlayerSelectAnim(PlayerSelectState *state) {
    PlayerCountSelectState *allocation;
    s32 i;
    volatile PlayerSelectSprite *vsprite;

    allocation = getCurrentAllocation();

    switch (state->unk2C) {
        case 0:
            for (i = 0; i < 2; i++) {
                state->sprites[i].y += 0x10;
            }
            if (state->sprites[0].y == -0x1C) {
                state->unk2C = 1;
            }
            break;

        case 1:
            if (getViewportFadeMode(&allocation->node) == 0) {
                state->unk2C = 2;
            }
            break;

        case 2:
            state->unk2D++;
            if ((state->unk2D & 0xFF) == 3) {
                state->unk2C = 3;
            }
            break;

        case 3:
            if (allocation->playerCount.selectedPlayerIndex != state->playerIndex) {
                state->playerIndex = allocation->playerCount.selectedPlayerIndex;
                state->animState = 0;
                state->animCounter = 0;
            } else {
                state->animCounter = (state->animCounter + 1) & 3;
                if (state->animCounter == 0) {
                    state->animState = (state->animState + 1) & 3;
                }
            }

            i = 0;
            if (allocation->playerCount.selectedPlayerIndex != state->slotIndex) {
                s16 scaleConst = 0x500;
                s16 alphaConst = 0x80;
                s32 divConst = 0x8000;
                vsprite = (volatile PlayerSelectSprite *)state;
                do {
                    s16 scale;
                    u8 slotIdx;
                    s16 frame;
                    s32 yPos;

                    vsprite->scaleY = scaleConst;
                    scale = divConst / (s32)(u16)vsprite->scaleY;
                    vsprite->scaleX = scaleConst;
                    vsprite->alpha = alphaConst;
                    yPos = i * scale;
                    yPos -= 0xC;
                    vsprite->y = yPos - scale / 2;
                    slotIdx = state->slotIndex;
                    frame = i + 8;
                    i++;
                    frame += slotIdx * 6;
                    vsprite->frameIndex = frame;
                    vsprite++;
                } while (i < 2);
            } else {
                s16 scaleConst = 0x400;
                s16 alphaConst = 0xFF;
                s32 divConst = 0x8000;
                u16 *animTable = D_800B09A0_1DC080;
                vsprite = (volatile PlayerSelectSprite *)state;
                do {
                    s16 scale;
                    u8 slotIdx;
                    s32 frame;
                    s32 yPos;

                    vsprite->scaleY = scaleConst;
                    scale = divConst / (s32)(u16)vsprite->scaleY;
                    vsprite->scaleX = scaleConst;
                    vsprite->alpha = alphaConst;
                    yPos = i * scale;
                    yPos -= 0xC;
                    vsprite->y = yPos - scale / 2;
                    slotIdx = state->slotIndex;
                    frame = i + 8;
                    frame += slotIdx * 6;
                    vsprite->frameIndex = frame;
                    i++;
                    vsprite->frameIndex = frame + animTable[state->animState] * 2;
                    vsprite++;
                } while (i < 2);
            }

            if (allocation->menuState == PLAYER_COUNT_MENU_CONFIRM_WAIT) {
                s32 slot;
                slot = state->slotIndex;
                state->unk2C = 4;
                if (slot == allocation->playerCount.bytes.selectedPlayerIndexLo) {
                    PlayerSelectSprite *sprite;
                    s32 frameBase;
                    s32 offset;
                    sprite = &state->sprites[i];
                    frameBase = i + 8;
                    offset = slot * 6;
                    sprite->frameIndex = frameBase + offset;
                    state->animState = 0;
                    state->animCounter = 0;
                }
            }
            break;

        default:
            break;

        case 4:
            if (state->slotIndex == allocation->playerCount.selectedPlayerIndex) {
                i = 0;
                do {
                    if (allocation->frameCounter & 1) {
                        state->sprites[i].unk13 = 0xFF;
                    } else {
                        state->sprites[i].unk13 = 0;
                    }
                    i++;
                } while (i < 2);
            }
            if (allocation->menuState == PLAYER_COUNT_MENU_SELECTING) {
                state->unk2C = 3;
            }
            break;
    }

    i = 0;
    do {
        enqueueCallbackBySlotIndex(8, 0, renderScaledShadedSpriteFrame, &state->sprites[i]);
        i++;
    } while (i < 2);
}

void cleanupPlayerSelectTask(PlayerSelectSpriteTask *arg0) {
    arg0->spriteData = freeNodeMemory(arg0->spriteData);
}

void initPlayerCountSelectSprites(PlayerSelectSpriteTask *arg0) {
    void *spriteData;
    s32 i;
    s16 x, y;

    getCurrentAllocation();
    spriteData = loadCompressedData(&playerCountSelectSprites_ROM_START, &playerCountSelectSprites_ROM_END, 0xEEE8);
    setCleanupCallback(cleanupPlayerCountSelectSprites);

    for (i = 0; i < 4; i++) {
        x = gPlayerCountOptionPositions[i * 2];
        arg0[i].x = x;
        y = gPlayerCountOptionPositions[i * 2 + 1];
        arg0[i].frameIndex = i / 2;
        arg0[i].playerIndex = 0;
        arg0[i].slotIndex = i & 1;
        arg0[i].alpha = 0xFF;
        arg0[i].spriteData = spriteData;
        arg0[i].y = y;
    }

    setCallback(enqueuePlayerSelectSpritesRender);
}

void enqueuePlayerSelectSpritesRender(PlayerSelectSpriteTask *arg0) {
    s32 i;

    for (i = 0; i < 4; i++) {
        enqueueCallbackBySlotIndex(8, 0, renderTextSprite, &arg0[i]);
    }
}

void cleanupPlayerCountSelectSprites(PlayerSelectSpriteTask *arg0) {
    arg0->spriteData = freeNodeMemory(arg0->spriteData);
}

void initPlayerCountHeaderSprite(PlayerSelectSpriteTask *arg0) {
    void *spriteData;

    getCurrentAllocation();
    spriteData = loadCompressedData(&playerCountSelectSprites_ROM_START, &playerCountSelectSprites_ROM_END, 0xEEE8);
    setCleanupCallback(cleanupPlayerCountHeaderSprite);
    arg0->x = -96;
    arg0->y = -80;
    arg0->frameIndex = 6;
    arg0->spriteData = spriteData;
    setCallback(updatePlayerCountHeaderSprite);
}

void updatePlayerCountHeaderSprite(PlayerSelectSpriteTask *arg0) {
    PlayerCountSelectState *allocation = getCurrentAllocation();

    if (allocation->menuState == PLAYER_COUNT_MENU_CANCEL_OK) {
        arg0->x = -80;
        arg0->frameIndex = 7;
    } else {
        arg0->x = -96;
        arg0->frameIndex = 6;
    }
    enqueueCallbackBySlotIndex(8, 0, renderSpriteFrame, arg0);
}

void cleanupPlayerCountHeaderSprite(PlayerSelectSpriteTask *arg0) {
    arg0->spriteData = freeNodeMemory(arg0->spriteData);
}

void initPlayerIndicatorSprite(PlayerSelectSpriteTask *arg0) {
    void *temp;

    getCurrentAllocation();
    temp = loadCompressedData(&playerCountSelectSprites_ROM_START, &playerCountSelectSprites_ROM_END, 0xEEE8);
    setCleanupCallback(cleanupPlayerCountHeaderSprite);
    arg0->spriteData = temp;
    setCallback(updatePlayerIndicatorSprite);
}

void updatePlayerIndicatorSprite(PlayerSelectSpriteTask *arg0) {
    PlayerCountSelectState *allocation = getCurrentAllocation();

    arg0->x = gPlayerCountIndicatorPositions[allocation->playerCount.selectedPlayerIndex * 2];
    arg0->y = gPlayerCountIndicatorPositions[allocation->playerCount.selectedPlayerIndex * 2 + 1];
    arg0->frameIndex = allocation->playerCount.selectedPlayerIndex + 2;

    enqueueCallbackBySlotIndex(8, 7, renderSpriteFrame, arg0);
}

void cleanupPlayerIndicatorSprite(PlayerSelectSpriteTask *arg0) {
    arg0->spriteData = freeNodeMemory(arg0->spriteData);
}

void initCharacterReadyIndicator(PlayerSelectSpriteTask *arg0) {
    void *temp;

    temp = loadCompressedData(&okPromptSprites_ROM_START, &okPromptSprites_ROM_END, 0x1B48);
    setCleanupCallback(cleanupCharacterReadyIndicator);
    arg0->x = -0x2C;
    arg0->y = -0x14;
    arg0->frameIndex = 0xD;
    arg0->spriteData = temp;
    setCallback(updateCharacterReadyIndicator);
}

void updateCharacterReadyIndicator(void *arg0) {
    PlayerCountSelectState *allocation = getCurrentAllocation();

    if (allocation->menuState == PLAYER_COUNT_MENU_CONFIRM_OK) {
        enqueueCallbackBySlotIndex(8, 7, renderSpriteFrame, arg0);
    }
}

void cleanupCharacterReadyIndicator(PlayerSelectSpriteTask *arg0) {
    arg0->spriteData = freeNodeMemory(arg0->spriteData);
}
