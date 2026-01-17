#include "10AD0.h"
#include "6E840.h"
#include "common.h"

typedef struct {
    s16 x;
    s16 y;
    void *asset;
    s16 spriteIndex;
    s16 alpha;
    s8 unkC;
    s8 unkD;
    u8 pad[2];
} SpriteElement16;

typedef struct {
    s8 unk0;
    u8 padding[1];
    s16 unk2;
    s32 unk4;
    Node_70B00 unk8;
    Node_70B00 unk1E0;
    Node_70B00 unk3B8;
    Node_70B00 unk590;
    Node_70B00 unk768;
    s16 unk940;
    s16 unk942;
    s16 unk944;
    void *unk948;
    s8 unk94C;
    u8 padding8[0xF];
    void *unk95C;
    void *unk960;
    void *unk964;
    void *unk968;
    s16 unk96C;
    s16 unk96E;
    u8 padding9A[0x4];
    void *unk974;
    s16 unk978;
    s16 unk97A;
    u8 unk97C;
    u8 padding9B[0x3];
    s16 unk980;
    s16 unk982;
    void *unk984;
    void *unk988;
    s16 unk98C;
    s16 unk98E;
    u8 unk990;
    u8 padding9C[0x3];
    void *unk994;
    void *unk998;
    void *unk99C;
    s16 unk9A0;
    u8 padding9D[0x2];
    s16 unk9A4;
    s16 unk9A6;
    u8 padding9E[0x4];
    s16 unk9AC;
    s16 unk9AE;
    s16 unk9B0;
    s16 unk9B2;
    u8 padding9F[0x4];
    void *unk9B8[6];
    u8 padding11[0x41C];
    void *volatile cornerDecorationAsset;
    s32 leftCornerAlpha;
    s32 leftCornerFadeSpeed;
    s32 rightCornerAlpha;
    s32 rightCornerFadeSpeed;
    SpriteElement16 leftTopCornerSprite;
    SpriteElement16 leftBottomCornerSprite;
    SpriteElement16 rightTopCornerSprite;
    SpriteElement16 rightBottomCornerSprite;
    s16 unkE40;
    s16 unkE42;
    ColorData unkE44[3];
} func_80003EE0_4AE0_task_memory;

void initCreditsCornerDecorationSprites(volatile func_80003EE0_4AE0_task_memory *state) {
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
        void *callback = func_80012004_12C04;
        state->leftBottomCornerSprite.alpha = shortVal;
        state->leftTopCornerSprite.alpha = shortVal;
        debugEnqueueCallback(1, 4, callback, &state->leftTopCornerSprite);
        debugEnqueueCallback(1, 4, callback, &state->leftBottomCornerSprite);
    }

    temp_v0 = state->rightCornerAlpha;
    if (temp_v0 != 0) {
        s16 shortVal = temp_v0 >> 16;
        void *callback = func_80012004_12C04;
        state->rightBottomCornerSprite.alpha = shortVal;
        state->rightTopCornerSprite.alpha = shortVal;
        debugEnqueueCallback(1, 4, callback, &state->rightTopCornerSprite);
        debugEnqueueCallback(1, 4, callback, &state->rightBottomCornerSprite);
    }
}
