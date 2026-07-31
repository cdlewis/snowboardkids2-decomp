#include "ui/player_select_sprites.h"
#include "assets.h"
#include "graphics/graphics.h"
#include "graphics/sprite_rdp.h"
#include "system/task_scheduler.h"
#include "ui/player_select.h"

#define PLAYER_SELECT_SELECTED_PORTRAIT_SCALE 0x400
#define PLAYER_SELECT_UNSELECTED_PORTRAIT_SCALE 0x500
#define PLAYER_SELECT_SELECTED_SHADE 0xFF
#define PLAYER_SELECT_UNSELECTED_SHADE 0x80
#define PLAYER_SELECT_PORTRAIT_LAYOUT_UNIT 0x8000
#define PLAYER_SELECT_PORTRAIT_Y_OFFSET 0xC

typedef enum {
    PLAYER_SELECT_PHASE_ENTRANCE_SLIDE = 0,
    PLAYER_SELECT_PHASE_WAIT_FOR_FADE = 1,
    PLAYER_SELECT_PHASE_DELAY = 2,
    PLAYER_SELECT_PHASE_ACTIVE = 3,
    PLAYER_SELECT_PHASE_CONFIRM_BLINK = 4,
} PlayerSelectPhase;

u16 gPlayerCountSelectedOptionAnimFrameOffsets[] = { 0x0000, 0x0001, 0x0002, 0x0001 };
s16 gPlayerCountOptionPositions[] = { 0xFFC0, 0x0020, 0x0000, 0x0020, 0xFF98, 0x0040, 0x0028, 0x0040 };
s16 gPlayerCountIndicatorPositions[] = { 0xFFD7, 0x0024, 0xFFD7, 0x0024, 0xFFC8, 0x0026,
                                         0xFFD8, 0x0024, 0x0000, 0x0000, 0x0000, 0x0000 };

void initPlayerSelectSprites(PlayerSelectState *state) {
    PlayerCountSelectState *allocation;
    void *spriteData;
    s32 i;
    s32 yPos;
    s16 scale;
    s16 alpha;
    volatile ScaledSpriteArg *sprite;

    allocation = getCurrentAllocation();
    spriteData = loadCompressedData(&playerCountSelectSprites_ROM_START, &playerCountSelectSprites_ROM_END, 0xEEE8);
    setCleanupCallback(cleanupPlayerSelectTask);

    i = 0;
    scale = PLAYER_SELECT_SELECTED_PORTRAIT_SCALE;
    alpha = PLAYER_SELECT_SELECTED_SHADE;
    yPos = -0x9C;
    sprite = state->portraits;
    do {
        s32 baseFrame;
        s16 x;
        s32 slotIndex;

        x = (state->playerCountOptionIndex << 6) - 0x60;
        sprite->y = yPos;
        yPos += 0x20;
        sprite->x = x;
        slotIndex = state->playerCountOptionIndex;
        baseFrame = i + 8;
        i++;
        sprite->renderWidth = scale;
        sprite->renderHeight = scale;
        sprite->shade.shadeWithPadding = alpha;
        sprite->padding0E = 0;
        sprite->overridePaletteCount = 0;
        sprite->tileMode = 0;
        sprite->spriteData = spriteData;
        sprite->frameIndex = baseFrame + slotIndex * 6;
        sprite++;
    } while (i < 2);

    state->phase = PLAYER_SELECT_PHASE_ENTRANCE_SLIDE;
    state->activationDelayTimer = 0;
    {
        u16 selectedOptionIndex = allocation->playerCount.selectedOptionIndex;
        state->selectedAnimFrame = 0;
        state->selectedAnimFrameTimer = 0;
        state->lastSelectedOptionIndex = selectedOptionIndex;
    }

    setCallback(updatePlayerSelectAnim);
}

void updatePlayerSelectAnim(PlayerSelectState *state) {
    PlayerCountSelectState *allocation;
    s32 i;
    volatile ScaledSpriteArg *portrait;

    allocation = getCurrentAllocation();

    switch (state->phase) {
        case PLAYER_SELECT_PHASE_ENTRANCE_SLIDE:
            for (i = 0; i < 2; i++) {
                state->portraits[i].y += 0x10;
            }
            if (state->portraits[0].y == -0x1C) {
                state->phase = PLAYER_SELECT_PHASE_WAIT_FOR_FADE;
            }
            break;

        case PLAYER_SELECT_PHASE_WAIT_FOR_FADE:
            if (getViewportFadeMode(&allocation->viewport) == 0) {
                state->phase = PLAYER_SELECT_PHASE_DELAY;
            }
            break;

        case PLAYER_SELECT_PHASE_DELAY:
            state->activationDelayTimer++;
            if ((state->activationDelayTimer & 0xFF) == 3) {
                state->phase = PLAYER_SELECT_PHASE_ACTIVE;
            }
            break;

        case PLAYER_SELECT_PHASE_ACTIVE:
            if (allocation->playerCount.selectedOptionIndex != state->lastSelectedOptionIndex) {
                state->lastSelectedOptionIndex = allocation->playerCount.selectedOptionIndex;
                state->selectedAnimFrame = 0;
                state->selectedAnimFrameTimer = 0;
            } else {
                state->selectedAnimFrameTimer = (state->selectedAnimFrameTimer + 1) & 3;
                if (state->selectedAnimFrameTimer == 0) {
                    state->selectedAnimFrame = (state->selectedAnimFrame + 1) & 3;
                }
            }

            i = 0;
            if (allocation->playerCount.selectedOptionIndex != state->playerCountOptionIndex) {
                s16 scaleConst = PLAYER_SELECT_UNSELECTED_PORTRAIT_SCALE;
                s16 alphaConst = PLAYER_SELECT_UNSELECTED_SHADE;
                s32 divConst = PLAYER_SELECT_PORTRAIT_LAYOUT_UNIT;
                portrait = state->portraits;
                do {
                    s16 scale;
                    u8 slotIdx;
                    s16 frame;
                    s32 yPos;

                    portrait->renderHeight = scaleConst;
                    scale = divConst / (s32)(u16)portrait->renderHeight;
                    portrait->renderWidth = scaleConst;
                    portrait->shade.shadeWithPadding = alphaConst;
                    yPos = i * scale;
                    yPos -= PLAYER_SELECT_PORTRAIT_Y_OFFSET;
                    portrait->y = yPos - scale / 2;
                    slotIdx = state->playerCountOptionIndex;
                    frame = i + 8;
                    i++;
                    frame += slotIdx * 6;
                    portrait->frameIndex = frame;
                    portrait++;
                } while (i < 2);
            } else {
                s16 scaleConst = PLAYER_SELECT_SELECTED_PORTRAIT_SCALE;
                s16 alphaConst = PLAYER_SELECT_SELECTED_SHADE;
                s32 divConst = PLAYER_SELECT_PORTRAIT_LAYOUT_UNIT;
                u16 *animTable = gPlayerCountSelectedOptionAnimFrameOffsets;
                portrait = state->portraits;
                do {
                    s16 scale;
                    u8 slotIdx;
                    s32 frame;
                    s32 yPos;

                    portrait->renderHeight = scaleConst;
                    scale = divConst / (s32)(u16)portrait->renderHeight;
                    portrait->renderWidth = scaleConst;
                    portrait->shade.shadeWithPadding = alphaConst;
                    yPos = i * scale;
                    yPos -= PLAYER_SELECT_PORTRAIT_Y_OFFSET;
                    portrait->y = yPos - scale / 2;
                    slotIdx = state->playerCountOptionIndex;
                    frame = i + 8;
                    frame += slotIdx * 6;
                    portrait->frameIndex = frame;
                    i++;
                    portrait->frameIndex = frame + animTable[state->selectedAnimFrame] * 2;
                    portrait++;
                } while (i < 2);
            }

            if (allocation->menuState == PLAYER_COUNT_MENU_CONFIRM_WAIT) {
                s32 slot;
                slot = state->playerCountOptionIndex;
                state->phase = PLAYER_SELECT_PHASE_CONFIRM_BLINK;
                if (slot == allocation->playerCount.bytes.selectedOptionIndex) {
                    ScaledSpriteArg *portrait;
                    s32 frameBase;
                    s32 offset;
                    portrait = &state->portraits[i];
                    frameBase = i + 8;
                    offset = slot * 6;
                    portrait->frameIndex = frameBase + offset;
                    state->selectedAnimFrame = 0;
                    state->selectedAnimFrameTimer = 0;
                }
            }
            break;

        default:
            break;

        case PLAYER_SELECT_PHASE_CONFIRM_BLINK:
            if (state->playerCountOptionIndex == allocation->playerCount.selectedOptionIndex) {
                i = 0;
                do {
                    if (allocation->frameCounter & 1) {
                        state->portraits[i].overridePaletteCount = 0xFF;
                    } else {
                        state->portraits[i].overridePaletteCount = 0;
                    }
                    i++;
                } while (i < 2);
            }
            if (allocation->menuState == PLAYER_COUNT_MENU_SELECTING) {
                state->phase = PLAYER_SELECT_PHASE_ACTIVE;
            }
            break;
    }

    i = 0;
    do {
        enqueueCallbackBySlotIndex(8, 0, renderScaledShadedSpriteFrame, &state->portraits[i]);
        i++;
    } while (i < 2);
}

void cleanupPlayerSelectTask(PlayerSelectState *state) {
    state->portraits[0].spriteData = freeNodeMemory(state->portraits[0].spriteData);
}

void initPlayerCountSelectSprites(TextRenderArg *sprites) {
    void *spriteData;
    s32 i;
    s16 x, y;

    getCurrentAllocation();
    spriteData = loadCompressedData(&playerCountSelectSprites_ROM_START, &playerCountSelectSprites_ROM_END, 0xEEE8);
    setCleanupCallback(cleanupPlayerCountSelectSprites);

    for (i = 0; i < 4; i++) {
        x = gPlayerCountOptionPositions[i * 2];
        sprites[i].x = x;
        y = gPlayerCountOptionPositions[i * 2 + 1];
        sprites[i].frameIndex = i / 2;
        sprites[i].overridePaletteCount = 0;
        sprites[i].tileMode = i & 1;
        sprites[i].color.paletteAndAlpha = 0xFF;
        sprites[i].spriteData = spriteData;
        sprites[i].y = y;
    }

    setCallback(enqueuePlayerSelectSpritesRender);
}

void enqueuePlayerSelectSpritesRender(TextRenderArg *sprites) {
    s32 i;

    for (i = 0; i < 4; i++) {
        enqueueCallbackBySlotIndex(8, 0, renderTextSprite, &sprites[i]);
    }
}

void cleanupPlayerCountSelectSprites(TextRenderArg *sprites) {
    sprites[0].spriteData = freeNodeMemory(sprites[0].spriteData);
}

void initPlayerCountHeaderSprite(SpriteRenderArg *sprite) {
    void *spriteData;

    getCurrentAllocation();
    spriteData = loadCompressedData(&playerCountSelectSprites_ROM_START, &playerCountSelectSprites_ROM_END, 0xEEE8);
    setCleanupCallback(cleanupPlayerCountHeaderSprite);
    sprite->x = -96;
    sprite->y = -80;
    sprite->frameIndex = 6;
    sprite->spriteData = spriteData;
    setCallback(updatePlayerCountHeaderSprite);
}

void updatePlayerCountHeaderSprite(SpriteRenderArg *sprite) {
    PlayerCountSelectState *allocation = getCurrentAllocation();

    if (allocation->menuState == PLAYER_COUNT_MENU_CANCEL_OK) {
        sprite->x = -80;
        sprite->frameIndex = 7;
    } else {
        sprite->x = -96;
        sprite->frameIndex = 6;
    }
    enqueueCallbackBySlotIndex(8, 0, renderSpriteFrame, sprite);
}

void cleanupPlayerCountHeaderSprite(SpriteRenderArg *sprite) {
    sprite->spriteData = freeNodeMemory(sprite->spriteData);
}

void initPlayerIndicatorSprite(SpriteRenderArg *sprite) {
    void *temp;

    getCurrentAllocation();
    temp = loadCompressedData(&playerCountSelectSprites_ROM_START, &playerCountSelectSprites_ROM_END, 0xEEE8);
    setCleanupCallback(cleanupPlayerCountHeaderSprite);
    sprite->spriteData = temp;
    setCallback(updatePlayerIndicatorSprite);
}

void updatePlayerIndicatorSprite(SpriteRenderArg *sprite) {
    PlayerCountSelectState *allocation = getCurrentAllocation();

    sprite->x = gPlayerCountIndicatorPositions[allocation->playerCount.selectedOptionIndex * 2];
    sprite->y = gPlayerCountIndicatorPositions[allocation->playerCount.selectedOptionIndex * 2 + 1];
    sprite->frameIndex = allocation->playerCount.selectedOptionIndex + 2;

    enqueueCallbackBySlotIndex(8, 7, renderSpriteFrame, sprite);
}

void cleanupPlayerIndicatorSprite(SpriteRenderArg *sprite) {
    sprite->spriteData = freeNodeMemory(sprite->spriteData);
}

void initCharacterReadyIndicator(SpriteRenderArg *sprite) {
    void *temp;

    temp = loadCompressedData(&okPromptSprites_ROM_START, &okPromptSprites_ROM_END, 0x1B48);
    setCleanupCallback(cleanupCharacterReadyIndicator);
    sprite->x = -0x2C;
    sprite->y = -0x14;
    sprite->frameIndex = 0xD;
    sprite->spriteData = temp;
    setCallback(updateCharacterReadyIndicator);
}

void updateCharacterReadyIndicator(SpriteRenderArg *sprite) {
    PlayerCountSelectState *allocation = getCurrentAllocation();

    if (allocation->menuState == PLAYER_COUNT_MENU_CONFIRM_OK) {
        enqueueCallbackBySlotIndex(8, 7, renderSpriteFrame, sprite);
    }
}

void cleanupCharacterReadyIndicator(SpriteRenderArg *sprite) {
    sprite->spriteData = freeNodeMemory(sprite->spriteData);
}
