#pragma once

#include "common.h"
#include "gamestate.h"
#include "graphics/sprite_rdp.h"
#include "text/font_assets.h"

typedef struct {
    u8 padding[0x10];
    void *players;
    u8 padding2[0x4B];
    u8 humanPlayerCount;
    u8 padding3[0x1A];
    u8 raceType;
} RaceHudInitState;

typedef struct {
    s32 unk0;
    u8 pad0[0xB80];
    s32 animFlags;
} PlayerInfo;

typedef struct {
    u8 pad0[0x10];
    PlayerInfo *timeRemaining;
    u8 pad14[0x40];
    s32 raceTimerElapsedTicks;
    u8 pad58[0x1E];
    u8 raceUpdatePaused;
    u8 pad77[0x2];
    u8 activeRaceEffectCount;
    u8 pad7A[0x3];
    u8 timerExpired;
    u8 raceTimerHoldFlag;
} Allocation;

typedef struct {
    u8 pad0[0x54];
    s32 elapsedTicks;
} SkillGameTimerAllocation;

typedef struct {
    s16 x;
    s16 y;
    void *lapIconAsset;
    s16 spriteIndex;
    u8 padA[0x2];
    s16 digitX1;
    u8 padE[0x2];
    s16 unk10;
    u8 pad12[0x2];
    s16 currentLap;
    u8 pad16[0x2];
    s16 digitX2;
    u8 pad1A[0xA];
    s16 digitX3;
    u8 pad26[0x1E];
    Player *player;
    s32 playerIndex;
} LapCounterSinglePlayerState;

typedef struct {
    s16 xPos;
    s16 yOffset;
    void *spriteAsset;
    s16 spriteFrame;
    u8 padA[0x2];
    s32 playerIndex;
    s32 holdFrames;
    s32 animAngle;
} GoalBannerState;

typedef struct {
    u8 pad0[0x4];
    void *unk4;
    u8 pad8[0x8];
    void *unk10;
} Struct_func_8004DCC4;

typedef struct {
    s16 xPos;
    s16 yPos;
    void *spriteAsset;
    u16 spriteIndex;
    u16 unkA;
    u32 playerIndex;
} CenteredSpritePopupState;

typedef struct {
    s16 xPos;
    s16 yPos;
    void *spriteAsset;
    s16 spriteFrame;
    u8 padA[0x2];
    void *digitsTexture;
    char scoreText[8];
    s32 score;
    s16 textX;
    s16 textY;
    s16 textScale;
    u8 pad22[0x2];
    void *textPtr;
    u16 playerIndex;
    u16 holdTimer;
    s16 animAngle;
    s16 useGoldFormat;
} TrickScoreDisplayState;

typedef struct {
    u8 pad0[0x4];
    void *unk4;
    u8 pad8[0x14];
    void *unk1C;
} Struct_func_8004C6F0;

typedef struct {
    u8 pad0[0x10];
    void *players;
    u8 pad14[0x4B];
    u8 numPlayers;
    u8 pad60[0x14];
    u8 totalLaps;
} LapCounterAllocation;

typedef struct {
    s16 x;
    s16 y;
    void *lapIconAsset;
    s16 spriteIndex;
    u8 padA[0x2];
    s16 digitX1;
    s16 digitY1;
    void *digitsAsset;
    u8 pad14[0x2];
    u8 unk16;
    u8 pad17[0x1];
    s16 digitX2;
    s16 digitY2;
    void *unk1C;
    s16 unk20;
    u8 pad22[0x2];
    s16 digitX3;
    s16 digitY3;
    void *unk28;
    s16 totalLaps;
    u8 unk2E;
    u8 pad2F[0x1];
    s16 textX;
    s16 textY;
    s16 unk34;
    u8 pad36[0x2];
    char *lapText;
    char lapTextBuffer[8];
    void *player;
    s32 playerIndex;
} LapCounterState;

typedef struct {
    u8 pad0[0x30];
    TextData unk30;
    u8 unk3C;
    u8 pad3D[0x7];
    Player *player;
    s32 playerIndex;
} LapCounterMultiplayerState;

typedef struct {
    u8 pad0[0x10];
    void *players;
    u8 pad14[0x4A];
    u8 numPlayers;
    u8 raceType;
} InitAllocation;

typedef struct {
    u8 pad0[0xBB9];
    u8 spriteGroupIndex;
} InitPlayerData;

typedef struct {
    s16 x;
    s16 y;
    u8 baseAssetBytes[4];
    s16 initFlags;
    u8 padA[0x2];
    s16 positionOffset;
    u8 padE[0x2];
    void *playerIconAsset;
} RaceProgressIndicatorInitEntry;

typedef struct {
    void *digitsTexture;    // 0x0
    s16 x;                  // 0x4 (singleplayer)
    s16 y;                  // 0x6 (singleplayer)
    s16 iconX;              // 0x8
    s16 iconY;              // 0xA
    void *iconAsset;        // 0xC
    s16 animFrame;          // 0x10
    u8 pad12[0x2];          // 0x12
    s16 textX;              // 0x14 (multiplayer)
    s16 textY;              // 0x16 (multiplayer)
    s16 digitCount;         // 0x18 (multiplayer)
    u8 pad1A[0x2];          // 0x1A
    char *textPtr;          // 0x1C (multiplayer)
    char goldTextBuffer[8]; // 0x20
    Player *player;         // 0x28
    s16 playerIndex;        // 0x2C
    u16 animCounter;        // 0x2E
} GoldDisplayState;

typedef struct {
    void *digitsTexture;
    s16 x;
    s16 y;
    s16 iconX;
    u8 padA[0x6];
    s16 animFrame;
    u8 pad12[0xE];
    char goldTextBuffer[8];
    Player *player;
    u16 playerIndex;
    u16 animCounter;
} PlayerGoldDisplayState;

typedef struct {
    void *playerIconAsset;
    u8 pad4[0x10];
} RaceProgressIndicatorPlayerEntry;

typedef struct {
    u8 pad0[0x4];
    void *baseAsset;
    u8 pad8[0x8];
    RaceProgressIndicatorPlayerEntry playerEntries[4]; // offset 0x10
} RaceProgressIndicatorCleanupState;

typedef struct {
    u8 pad0[0x10];
    void *players;
    u8 pad14[0x4A];
    u8 numPlayers;
    u8 pad5F[0x5];
    u8 playerIndices[4];
} RaceProgressIndicatorAllocation;

typedef struct {
    s16 x;
    s16 y;
    u8 pad4[0x4];
    s16 spriteFrame;
    u8 hasActiveEffect;
    u8 unkB;
    u8 flashCounter;
    s8 flashState;
    u8 padE[0x2];
    s16 positionOffset;
    u8 pad12[0x2];
} RaceProgressIndicatorElement;

typedef struct {
    u8 pad0[0x2];
    s16 baseY;
    u8 pad4[0x8];
    RaceProgressIndicatorElement elements[4];
} RaceProgressIndicatorState;

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
    SpriteRenderArg ammoPanel;
    SpriteRenderArg ammoIcon;
    void *digitAsset;
    Player *player;
} ShotCrossScoreDisplayState;

typedef struct {
    void *digitAsset;
    SpriteRenderArg sprite;
    s16 flashCounter;
    s16 cachedValue;
    s16 layoutMode;
} CrossHudCounterDisplayState;

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

typedef struct {
    /* 0x0 */ s16 padding;
    /* 0x2 */ s16 y;
    /* 0x4 */ void *spriteAsset;
    /* 0x8 */ s16 spriteFrame;
    u8 padA[0x2];
    /* 0xC */ void *digitAsset;
    /* 0x10 */ char scoreText[8];
    /* 0x18 */ s32 score;
    /* 0x1C */ s16 x;
    /* 0x1E */ s16 holdTimer;
    /* 0x20 */ s16 animAngle;
} TrickPointsDisplayState;

typedef struct {
    u8 pad0[0x8];
    s16 iconX;
    u8 padA[0x6];
    s16 animFrame;
    u8 pad12[0x2];
    s16 textX;
    u8 pad16[0x2];
    s16 digitCount;
    u8 pad1A[0x6];
    char goldTextBuffer[8];
    Player *player;
    u16 playerIndex;
    u16 animCounter;
} MultiplayerGoldDisplayState;

typedef struct {
    void *goldIconAsset;
    u8 pad4[0x8];
    void *digitSpriteAsset;
} PlayerGoldDisplayCleanupArg;

typedef struct {
    s16 x;
    s16 y;
    void *spriteAsset;
    s16 spriteIndex;
    u8 padA[0x4];
    u8 unkE;
    u8 padF;
    void *digitAsset;
    s32 alpha;
} TotalGoldDisplayState;

typedef struct {
    s16 x;
    s16 y;
    void *spriteAsset;
    s16 spriteIndex;
    u8 padA[0x4];
    u8 unkE;
    u8 padF;
    void *digitAsset;
    s32 alpha;
    Player *player;
} TotalLapDisplayState;

typedef struct {
    s16 screenX;
    s16 screenY;
    void *spriteAsset;
    s16 spriteFrame;
    u8 lifetime;
    u8 padB;
    s16 playerIndex;
    s16 useSmallSprite;
    u16 animFrame;
    s16 posX;
    s16 posY;
    s16 waveAmplitude;
    s16 velocityX;
    s16 velocityY;
    s16 wavePhase;
    s16 driftTimer;
} VictorySnowflakeState;

typedef struct {
    u8 pad0[0x4];
    void *spriteAsset;
    u8 pad8[0x1C];
    void *backgroundAsset;
} PauseMenuCleanupState;

typedef struct {
    s16 x;
    s16 y;
    void *spriteAsset;
    s16 spriteFrame;
    s16 padA;
} ShotScoreElement;

typedef struct {
    ShotScoreElement elements[10];
    s16 spriteGroupIndex;
    s16 displayedCount;
    s16 animDelayCounter;
} ShotScoreDisplayState;

typedef struct {
    s16 x;
    s16 y;
    void *spriteAsset;
    s16 spriteFrame;
    u8 padA[0x2];
    s16 flashState;
} SuccessMessageDisplayState;

extern const char sTwoDigitFormat[];
extern const char sTwoDigitHighlightFormat[];
extern const char sIntegerFormat[];
extern const char sTimerFormatLow[];
extern const char sTimerFormatNormal[];
extern const char sSpeedCrossTimerBlinkColonFormat[];
extern const char sSpeedCrossTimerBlinkSpaceFormat[];
extern const char sSpeedCrossTimerNormalColonFormat[];
extern const char sSpeedCrossTimerNormalSpaceFormat[];
extern const char sSkillGameResultTimerColonFormat[];
extern const char sSkillGameResultTimerSpaceFormat[];
extern const char sTrickPointsFormat[];
extern const char sTrickPointsHighlightFormat[];

void showGoalBanner(s32 playerIndex);
void showPlacementAnnouncement(u32 playerIndex, s32 placement);
void showTrickScoreDisplay(s32 playerIndex, s32 trickScore);
void initSpeedCrossFinishPositionTask(FinishPositionDisplayState *arg0);
void initShotCrossScoreDisplayTask(ShotCrossScoreDisplayState *arg0);
void spawnShotCrossScoreDisplayTask(void *arg0);
void initGoldAwardDisplayTask(GoldAwardDisplayState *arg0);
void spawnVictorySnowflakes(s16 playerIndex, s16 useSmallSprite);
void spawnTotalLapDisplayTask(Player *player);
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
void updateCenteredSpritePopup(CenteredSpritePopupState *);
void cleanupCenteredSpritePopupTask(CenteredSpritePopupState *);
void updateSpeedCrossFinishPositionDisplay(FinishPositionDisplayState *arg0);
void cleanupSpeedCrossFinishPositionTask(FinishPositionDisplayState *arg0);
void updateTrickScoreSlideOut(TrickScoreDisplayState *state);
void updateTrickScoreHold(TrickScoreDisplayState *state);
void updateTrickScoreSlideIn(TrickScoreDisplayState *state);
void cleanupTrickScoreDisplayTask(TrickScoreDisplayState *state);
void updateGoalBannerSlideIn(GoalBannerState *state);
void updateGoalBannerHold(GoalBannerState *state);
void updateGoalBannerSlideOut(GoalBannerState *state);
void cleanupGoalBannerTask(GoalBannerState *state);
void initPlayerFinishPositionTask(FinishPositionDisplayState *state);
void updatePlayerFinishPositionDisplay(FinishPositionDisplayState *state);
void cleanupPlayerFinishPositionTask(FinishPositionDisplayState *state);
void initPlayerItemDisplayTask(PlayerItemDisplayState *state);
void initPlayerLapCounterTask(LapCounterState *state);
void initPlayerGoldDisplayTask(GoldDisplayState *state);
void initRaceProgressIndicatorTask(RaceProgressIndicatorInitEntry *);
void cleanupPlayerItemDisplayTask(Struct_func_8004C6F0 *arg0);
void updatePlayerItemDisplayMultiplayer(PlayerItemDisplayState *state);
void updatePlayerItemDisplaySinglePlayer(PlayerItemDisplayState *state);
void updatePlayerLapCounterSinglePlayer(LapCounterSinglePlayerState *state);
void cleanupPlayerLapCounterTask(Struct_func_8004DCC4 *arg0);
void updatePlayerLapCounterMultiplayer(LapCounterMultiplayerState *state);
void updatePlayerGoldDisplaySinglePlayer(PlayerGoldDisplayState *state);
void updatePlayerGoldDisplayMultiplayer(MultiplayerGoldDisplayState *state);
void cleanupPlayerGoldDisplayTask(PlayerGoldDisplayCleanupArg *arg0);
void updatePlayerRaceProgressIndicator(RaceProgressIndicatorState *state);
void cleanupRaceProgressIndicatorTask(RaceProgressIndicatorCleanupState *state);
void updateGoldAwardDisplay(GoldAwardDisplayState *arg0);
void cleanupGoldAwardDisplayTask(GoldAwardDisplayState *arg0);
void initTotalGoldDisplayTask(TotalGoldDisplayState *arg0);
void updateTotalGoldDisplay(TotalGoldDisplayState *arg0);
void cleanupTotalGoldDisplayTask(Struct_func_8004DCC4 *);
void updateTotalLapDisplay(TotalLapDisplayState *state);
void cleanupTotalLapDisplayTask(Struct_func_8004DCC4 *);
void cleanupVictorySnowflake(VictorySnowflakeState *state);
void updateVictorySnowflakeWave(VictorySnowflakeState *state);
void renderVictorySnowflake(VictorySnowflakeState *state);
void renderVictorySnowflakeSmall(VictorySnowflakeState *state);
void updateVictorySnowflakeDrift(VictorySnowflakeState *state);
void renderPauseMenuDisplay(PauseMenuDisplayState *);
void cleanupPauseMenuDisplayTask(PauseMenuCleanupState *);
void updateShotScoreDisplay(ShotScoreDisplayState *);
void cleanupShotScoreDisplayTask(ShotScoreDisplayState *);
void updateShotCrossScoreDisplay(ShotCrossScoreDisplayState *arg0);
void cleanupShotCrossScoreDisplayTask(ShotCrossScoreDisplayState *arg0);
void updateShotCrossItemCountDisplay(CrossHudCounterDisplayState *arg0);
void cleanupShotCrossItemCountDisplayTask(CrossHudCounterDisplayState *arg0);
void updateSuccessMessageDisplay(SuccessMessageDisplayState *);
void cleanupSuccessMessageDisplayTask(ShotCrossCountdownTimerState *);
void updateRaceTimerDisplay(RaceTimerState *arg0);
void cleanupRaceTimerDisplay(ShotCrossCountdownTimerState *arg0);
void updateSkillGameResultTimerDisplay(ShotCrossCountdownTimerState *);
void cleanupSkillGameResultTimerDisplay(ShotCrossCountdownTimerState *);
void cleanupTrickPointsDisplayTask(ShotCrossCountdownTimerState *state);
void updateTrickPointsSlideIn(TrickPointsDisplayState *state);
void renderTrickPointsDisplay(TrickPointsDisplayState *state);
void updateTrickPointsSlideOut(TrickPointsDisplayState *state);
void updateTrickPointsHold(TrickPointsDisplayState *state);
void updateShotCrossSkillMeterDisplay(CrossHudCounterDisplayState *);
void cleanupShotCrossSkillMeterDisplayTask(CrossHudCounterDisplayState *);
