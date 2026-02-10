#include "1DC260.h"

void initCreditsCornerDecorationSprites(void *arg0) {
    volatile func_80003EE0_4AE0_task_memory *state = (volatile func_80003EE0_4AE0_task_memory *)arg0;
    void *asset0;
    void *asset1;
    void *asset2;
    void *asset3;
    s16 spriteX;
    s16 topSpriteY;
    s16 alpha;

    asset0 = state->cornerDecorationAsset;
    asset1 = state->cornerDecorationAsset;
    asset2 = state->cornerDecorationAsset;
    asset3 = state->cornerDecorationAsset;
    spriteX = -0x40;
    topSpriteY = -0x10;
    alpha = 0xFF;

    do {
        state->leftBottomCornerSprite.spriteIndex = 1;
        state->rightTopCornerSprite.spriteIndex = 2;
        state->leftCornerAlpha = 0;
        state->leftCornerFadeSpeed = 0;
        state->rightCornerAlpha = 0;
        state->rightCornerFadeSpeed = 0;
        state->leftTopCornerSprite.x = spriteX;
        state->leftTopCornerSprite.y = topSpriteY;
        state->leftTopCornerSprite.spriteIndex = 0;
        state->leftTopCornerSprite.alpha = alpha;
        state->leftTopCornerSprite.unkC = 0;
        state->leftTopCornerSprite.unkD = 0;
        state->leftBottomCornerSprite.x = spriteX;
        state->leftBottomCornerSprite.y = 0;
        state->leftBottomCornerSprite.alpha = alpha;
        state->leftBottomCornerSprite.unkC = 0;
        state->leftBottomCornerSprite.unkD = 0;
        state->rightTopCornerSprite.x = spriteX;
        state->rightTopCornerSprite.y = topSpriteY;
        state->rightTopCornerSprite.alpha = alpha;
        state->rightTopCornerSprite.unkC = 0;
        state->rightTopCornerSprite.unkD = 0;
        state->rightBottomCornerSprite.x = spriteX;
        state->rightBottomCornerSprite.y = 0;
        state->rightBottomCornerSprite.spriteIndex = 3;
        state->leftTopCornerSprite.asset = asset0;
        state->leftBottomCornerSprite.asset = asset1;
        state->rightTopCornerSprite.asset = asset2;
    } while (0);

    state->rightBottomCornerSprite.asset = asset3;
    state->rightBottomCornerSprite.alpha = alpha;
    state->rightBottomCornerSprite.unkC = 0;
    state->rightBottomCornerSprite.unkD = 0;
}

void updateCreditsCornerDecorationSprites(void *arg0) {
    func_80003EE0_4AE0_task_memory *state = (func_80003EE0_4AE0_task_memory *)arg0;
    s16 temp_v1;
    s32 temp_v0;
    s32 value;

    temp_v1 = state->unk2;

    switch (temp_v1) {
        case 0x1A22:
            state->rightCornerFadeSpeed = 0xC0000;
            break;
        case 0x1AD6:
            state->rightCornerFadeSpeed = 0xFFF40000;
            break;
        case 0x1B30:
            state->leftCornerFadeSpeed = 0xC0000;
            break;
    }

    value = state->leftCornerFadeSpeed;
    if (value != 0) {
        temp_v0 = state->leftCornerAlpha + value;
        state->leftCornerAlpha = temp_v0;
        if (temp_v0 > 0xFF0000) {
            state->leftCornerAlpha = 0xFF0000;
        }
        if (state->leftCornerAlpha < 0) {
            state->leftCornerAlpha = 0;
        }
    }

    value = state->rightCornerFadeSpeed;
    if (value != 0) {
        temp_v0 = state->rightCornerAlpha + value;
        state->rightCornerAlpha = temp_v0;
        if (temp_v0 > 0xFF0000) {
            state->rightCornerAlpha = 0xFF0000;
        }
        if (state->rightCornerAlpha < 0) {
            state->rightCornerAlpha = 0;
        }
    }

    temp_v0 = state->leftCornerAlpha;
    if (temp_v0 != 0) {
        s16 shortVal = temp_v0 >> 16;
        void *callback = renderTextSprite;
        state->leftBottomCornerSprite.alpha = shortVal;
        state->leftTopCornerSprite.alpha = shortVal;
        debugEnqueueCallback(1, 4, callback, &state->leftTopCornerSprite);
        debugEnqueueCallback(1, 4, callback, &state->leftBottomCornerSprite);
    }

    temp_v0 = state->rightCornerAlpha;
    if (temp_v0 != 0) {
        s16 shortVal = temp_v0 >> 16;
        void *callback = renderTextSprite;
        state->rightBottomCornerSprite.alpha = shortVal;
        state->rightTopCornerSprite.alpha = shortVal;
        debugEnqueueCallback(1, 4, callback, &state->rightTopCornerSprite);
        debugEnqueueCallback(1, 4, callback, &state->rightBottomCornerSprite);
    }
}
