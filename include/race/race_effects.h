#pragma once

#include "common.h"
#include "gamestate.h"

typedef struct {
    s16 x;
    s16 y;
    void *spriteAsset;
    s16 spriteIndex;
    u8 padA[0x2];
} PauseMenuOptionElement; // size 0xC

typedef struct {
    PauseMenuOptionElement elements[3]; // offset 0x0, size 0x24
    void *backgroundAsset;              // offset 0x24
} PauseMenuDisplayState;

typedef struct {
    s16 x;
    s16 y;
    void *asset;
    s16 spriteIndex;
    u8 padA[0x2];
    Player *player;
    s32 playerIndex;
} FinishPositionDisplayState;

typedef struct {
    s16 x;
    s16 y;
    void *spriteAsset;
    s16 spriteFrame;
    s16 padA;
    u8 unkC;
    u8 unkD;
    u8 alpha;
    u8 padF;
    void *digitAsset;
    s32 alphaValue;
} BonusGoldDisplayState;

typedef struct {
    s16 x;
    s16 y;
    void *spriteAsset;
    s16 spriteIndex;
    u8 padA[0x2];
    void *digitAsset;
    s16 timeRemaining;
} ShotCrossCountdownTimerState;

typedef struct {
    u8 pad0[0xC];
    void *digitAsset;
    s16 timeRemaining;
} ShotCrossCountdownTimerUpdateState;

typedef struct {
    s16 x;
    s16 y;
    void *spriteAsset;
    s16 spriteIndex;
    u8 padA[0x6];
    void *digitAsset;
    s32 alpha;
    s32 goldAmount;
} GoldAwardDisplayState;

typedef struct {
    s16 primaryItemX;
    s16 primaryItemY;
    void *primaryItemAsset;
    s16 primaryItemIndex;
    u8 padA[0x2];
    s16 secondaryItemX;
    s16 secondaryItemY;
    void *secondaryItemAsset;
    s16 secondaryItemIndex;
    u8 pad16[0x2];
    s16 itemCountX;
    s16 itemCountY;
    void *digitAsset;
    s16 itemCountValue;
    u8 pad22[0x2];
    s16 charDisplayX;
    s16 charDisplayY;
    s16 unk28;
    u8 pad2A[0x2];
    void *charDisplayPtr;
    u8 charDisplayValue;
    u8 charDisplayFlag;
    u8 pad32[0x2];
    Player *player;
    s32 playerIndex;
} PlayerItemDisplayState;

typedef struct {
    s16 spriteX;
    s16 spriteY;
    void *spriteAsset;
    s16 spriteIndex;
    u8 padA[0x2];
    s16 hudX;
    s16 hudY;
    void *spriteAssetCopy;
    s16 spriteCount;
    u8 pad16[0x2];
    void *digitAsset;
    Player *player;
} ShotCrossScoreDisplayState;

typedef struct {
    void *digitAsset;
    s16 x;
    s16 y;
    void *spriteAsset;
    s16 spriteIndex;
    u8 padE[0x2];
    s16 flashCounter;
    s16 cachedItemCount;
    s16 displayMode;
} ShotCrossItemCountDisplayState;

typedef struct {
    s16 x;
    s16 y;
    void *spriteAsset;
    s16 spriteIndex;
    u8 padA[0x2];
    void *digitAsset;
    s32 elapsedTicks;
    s16 blinkCounter;
} RaceTimerState;

typedef struct {
    s16 itemX;
    s16 itemY;
    void *spriteAsset;
    s16 itemIndex;
    u8 padA[0x2];
    Player *player;
    s32 playerIndex;
} SecondaryItemDisplayState;

typedef struct {
    s16 mainX;
    s16 mainY;
    void *mainSpriteAsset;
    s16 mainSpriteFrame;
    u8 padA[0x2];
    s16 bgX;
    s16 bgY;
    void *bgSpriteAsset;
    s16 bgSpriteFrame;
} CrossRaceBadgeState;

void showGoalBanner(s32 playerIndex);
void showTrickScoreDisplay(s32 playerIndex, s32 trickScore);
void initSpeedCrossFinishPositionTask(FinishPositionDisplayState *arg0);
void initShotCrossScoreDisplayTask(ShotCrossScoreDisplayState *arg0);
void spawnShotCrossScoreDisplayTask(void *arg0);
void initGoldAwardDisplayTask(GoldAwardDisplayState *arg0);
void spawnVictorySnowflakes(s16 playerIndex, s16 useSmallSprite);
void initPauseMenuDisplayTask(PauseMenuDisplayState *arg0);
void spawnShotCrossItemCountDisplayTask(s16 arg0);
void initBonusGoldDisplayTask(BonusGoldDisplayState *arg0);
void updateBonusGoldDisplay(BonusGoldDisplayState *arg0);
void cleanupBonusGoldDisplayTask(BonusGoldDisplayState *arg0);
void spawnSuccessMessageDisplayTask(s16 delayFrames);
void initSkillGameResultTimerDisplay(ShotCrossCountdownTimerState *arg0);
void initShotCrossCountdownTimerTask(ShotCrossCountdownTimerState *arg0);
void updateShotCrossCountdownTimer(ShotCrossCountdownTimerUpdateState *arg0);
void cleanupShotCrossCountdownTimerTask(ShotCrossCountdownTimerState *arg0);
void showTrickPointsDisplay(s32 arg0);
void spawnShotCrossSkillMeterDisplayTask(s16 arg0);
void initCrossRaceBadgeTask(CrossRaceBadgeState *arg0);
void updateCrossRaceBadgeDisplay(CrossRaceBadgeState *arg0);
void cleanupCrossRaceBadgeTask(void *arg0);
void initRaceHudTasks(void);
void initSecondaryItemDisplayTask(SecondaryItemDisplayState *arg0);
void updateSecondaryItemDisplay(SecondaryItemDisplayState *arg0);
void cleanupSecondaryItemDisplayTask(SecondaryItemDisplayState *arg0);
