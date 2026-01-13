#pragma once

#include "1DFAA0.h"
#include "common.h"
#include "geometry.h"

typedef struct {
    s32 posX;              // 0x00
    s32 posY;              // 0x04
    s32 posZ;              // 0x08
    s32 targetX;           // 0x0C
    s32 targetY;           // 0x10
    s32 targetZ;           // 0x14
    u8 gameData[0x14];     // 0x18 - embedded GameDataLayout
    void *sceneModel;      // 0x2C
    Transform3D transform; // 0x30
    u16 currentWaypoint;   // 0x50
    u16 startWaypoint;     // 0x52
    s16 animationPhase;    // 0x54
    u16 currentRotation;   // 0x56
    u8 _pad58[0x2];        // 0x58
    u16 targetRotation;    // 0x5A
    s32 cameraHorzOffset;  // 0x5C
    s32 cameraDistance;    // 0x60
    s32 heightOffset;      // 0x64
    s32 altHeightOffset;   // 0x68
    u8 _pad6C[0x6];        // 0x6C
    u16 frameTimer;        // 0x72
    s16 extraRotation;     // 0x74
    u8 turnDirection;      // 0x76
} LevelPreviewCharacterState;

typedef struct {
    u8 _pad0[0x2C];
    void *imageAsset;
    s16 x;
    s16 y;
    void *spriteSheetAsset;
    u16 frameIndex;
} UnlockNotificationState;

typedef struct {
    u8 _pad0[0xF8];
    void *portraitAsset;
} LevelPreviewPortraitState;

typedef struct {
    SceneModel *model;
    Transform3D transform;
    s16 animationIndex;
    s16 unk22;
} MenuCharacterModelState;

typedef struct {
    s16 x;
    s16 y;
    void *spriteAsset;
    s16 frameIndex;
} ConfirmationIndicatorState;

typedef struct {
    u8 high;
    u8 low;
} ColorBytes;

typedef union {
    s16 asS16;
    ColorBytes asBytes;
} ColorValue;

typedef struct {
    s16 x;
    s16 y;
    void *textString;
    void *textRenderAsset;
    ColorValue color1;
    ColorValue color2;
    u8 priority;
} CharacterDescriptionTextState;

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ void *spriteAsset;
    /* 0x08 */ s16 frameIndex;
    /* 0x0A */ s16 alpha;
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 unkD;
    /* 0x0E */ u8 _padE[2];
} PrizeSpriteEntry;

typedef struct {
    /* 0x00 */ PrizeSpriteEntry spriteEntries[2];
    /* 0x20 */ u8 _pad20[0xC];
    /* 0x2C */ s16 titleX;
    /* 0x2E */ s16 titleY;
    /* 0x30 */ u16 *titleText;
    /* 0x34 */ void *textRenderAsset;
    /* 0x38 */ s16 titleColor1;
    /* 0x3A */ s16 titleColor2;
    /* 0x3C */ u8 titlePriority;
    /* 0x3D */ u8 _pad3D[3];
    /* 0x40 */ s16 counterX;
    /* 0x42 */ s16 counterY;
    /* 0x44 */ void *counterValuePtr;
    /* 0x48 */ void *counterRenderAsset;
    /* 0x4C */ s16 counterColor1;
    /* 0x4E */ s16 counterColor2;
    /* 0x50 */ u8 counterPriority;
    /* 0x51 */ u8 _pad51[3];
    /* 0x54 */ void *backgroundAsset;
    /* 0x58 */ s16 prizeCount;
    /* 0x5A */ u16 previousCount;
    /* 0x5C */ u8 _pad5C[4];
    /* 0x60 */ u8 animationTimer;
} PrizeDisplayState;

typedef struct {
    u16 rotationX;
    u16 rotationY;
    u8 _pad4[0x28];
    void *effectAsset;
} MenuBackgroundEffectState;

void func_80020B44_21744(void);
void initLevelPreviewCharacter(LevelPreviewCharacterState *arg0);
void initUnlockNotification(UnlockNotificationState *state);
void initLevelPreviewPortraits(LevelPreviewPortraitState *state);
void func_80020708_21308(void);
void cleanupLevelPreviewPortraits(LevelPreviewPortraitState *state);
void initMenuCharacterModel(MenuCharacterModelState *state);
void setupLevelPreviewCamera(LevelPreviewCharacterState *arg0);
void updateLevelPreviewCamera(LevelPreviewCharacterState *state);
void func_80020924_21524(void);
void renderConfirmationIndicator(void *arg0);
void cleanupConfirmationIndicator(ConfirmationIndicatorState *state);
void updatePrizeDisplay(PrizeDisplayState *state);
void cleanupPrizeDisplay(PrizeDisplayState *state);
void renderUnlockNotification(UnlockNotificationState *state);
void initUnlockNotificationSprite(UnlockNotificationState *state);
void cleanupUnlockNotification(UnlockNotificationState *state);
void cleanupMenuCharacterModel(MenuCharacterModelState *state);
void setupMenuCharacterModel(MenuCharacterModelState *state);
void updateMenuCharacterModel(MenuCharacterModelState *state);
void handleMenuCharacterAnimationEnd(MenuCharacterModelState *state);
void setMenuCharacterAnimation(u8 animationType, MenuCharacterModelState *state);
void cleanupMenuBackgroundEffect(MenuBackgroundEffectState *state);
void setupMenuBackgroundEffect(MenuBackgroundEffectState *state);
void updateMenuBackgroundEffect(MenuBackgroundEffectState *state);
void renderCharacterDescriptionText(CharacterDescriptionTextState *state);
void cleanupCharacterDescriptionText(CharacterDescriptionTextState *state);
void initConfirmationIndicator(ConfirmationIndicatorState *state);
void initPrizeDisplay(PrizeDisplayState *arg0);
void initMenuBackgroundEffect(MenuBackgroundEffectState *state);
void initCharacterDescriptionText(CharacterDescriptionTextState *state);
void initLevelSelectTransition(void);
void cleanupLevelPreviewCharacter(LevelPreviewCharacterState *state);
