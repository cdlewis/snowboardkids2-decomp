#include "4CD70.h"
#include "10AD0.h"
#include "19E80.h"
#include "2DD40.h"
#include "56910.h"
#include "5E590.h"
#include "68CF0.h"
#include "6DE50.h"
#include "6E840.h"
#include "D_800AFE8C_A71FC_type.h"
#include "common.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
#include "race_session.h"
#include "rand.h"
#include "rom_loader.h"
#include "task_scheduler.h"

extern s32 gFrameCounter;

#define SECONDS_TO_TICKS(s) ((s) * 30)

#define SCHEDULE_AND_SET(func, offset, value)        \
    do {                                             \
        void *task = scheduleTask(func, 0, 1, 0xE6); \
        if (task)                                    \
            ((s32 *)task)[offset] = (value);         \
    } while (0)

#define SCHEDULE_AND_SET_SHORT(func, offset, value)  \
    do {                                             \
        void *task = scheduleTask(func, 0, 1, 0xE6); \
        if (task)                                    \
            ((s16 *)task)[offset] = (value);         \
    } while (0)

USE_ASSET(_3F3940);
USE_ASSET(_3F3D10);
USE_ASSET(_3F6670);
USE_ASSET(_3F6950);
USE_ASSET(_3F6BB0);
USE_ASSET(_3F3EF0);
USE_ASSET(_3F58E0);
USE_ASSET(_3F6420);
USE_ASSET(_3F65C0);

typedef struct {
    u8 padding[0x10];
    void *unk10;
    u8 padding2[0x4B];
    u8 unk5F;
    u8 padding3[0x1A];
    u8 unk7A;
} func_8005011C_50D1C_alloc;

typedef struct {
    s32 unk0;
    u8 pad0[0xB80];
    s32 unkB84;
} PlayerInfo;

typedef struct {
    u8 pad0[0x10];
    PlayerInfo *timeRemaining;
    u8 pad14[0x40];
    s32 unk54;
    u8 pad58[0x1E];
    u8 unk76;
    u8 pad77[0x2];
    u8 unk79;
    u8 pad7A[0x3];
    u8 timerExpired;
    u8 unk7E;
} Allocation;

// defined later to keep the rodata gods happy
const char sTimerFormatLow[];
const char sTimerFormatNormal[];

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
    u8 padC[0xC];
    void *unkC;
    s16 timeRemaining;
} Struct_func_8004EEB4_4FAB4;

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
    void *unk0;
    s16 unk4;
    s16 unk6;
    void *unk8;
    s16 unkC;
    u8 padE[0x2];
    s16 unk10;
    s16 unk12;
    s16 unk14;
} Struct_func_8004FF28;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    u8 padA[0x2];
    s16 unkC;
    s16 unkE;
    void *unk10;
    s16 unk14;
} Struct_func_8004FFB8;

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

void updateCenteredSpritePopup(CenteredSpritePopupState *);
void cleanupCenteredSpritePopupTask(CenteredSpritePopupState *);
void updateSpeedCrossFinishPositionDisplay(FinishPositionDisplayState *arg0);
void cleanupSpeedCrossFinishPositionTask(FinishPositionDisplayState *arg0);
void updateTrickScoreSlideOut(TrickScoreDisplayState *state);
void updateTrickScoreHold(TrickScoreDisplayState *state);
void updateTrickScoreSlideIn(TrickScoreDisplayState *state);
void cleanupTrickScoreDisplayTask(Struct_func_8004F04C *state);
void updateGoalBannerSlideIn(GoalBannerState *state);
void updateGoalBannerHold(GoalBannerState *state);
void updateGoalBannerSlideOut(GoalBannerState *state);
void cleanupGoalBannerTask(GoalBannerState *state);
void func_8004EEB4_4FAB4(Struct_func_8004EEB4_4FAB4 *arg0);
void initPlayerFinishPositionTask(FinishPositionDisplayState *state);
void updatePlayerFinishPositionDisplay(FinishPositionDisplayState *state);
void cleanupPlayerFinishPositionTask(FinishPositionDisplayState *state);
void initPlayerItemDisplayTask(PlayerItemDisplayState *state);
void initPlayerLapCounterTask(LapCounterState *state);
typedef struct GoldDisplayState_s GoldDisplayState;
void initPlayerGoldDisplayTask(GoldDisplayState *state);
void func_8004CDC0_4D9C0(void);
void cleanupPlayerItemDisplayTask(Struct_func_8004C6F0 *arg0);
void updatePlayerItemDisplayMultiplayer(PlayerItemDisplayState *state);
void updatePlayerItemDisplaySinglePlayer(PlayerItemDisplayState *state);
void updatePlayerLapCounterSinglePlayer(LapCounterSinglePlayerState *state);
void cleanupPlayerLapCounterTask(Struct_func_8004DCC4 *arg0);
void updatePlayerLapCounterMultiplayer(LapCounterMultiplayerState *state);

static const char D_8009E880_9F480[] = "%5d";
extern char D_8009E89C_9F49C[];
extern char D_8009E8A0_9F4A0[];
extern char D_8009E8A8_9F4A8[];
extern char D_8009E928_9F528[];
extern char D_8009E868_9F468[];

extern s32 gFirstPlaceGoldReward[];
extern s32 gSecondPlaceGoldReward[];
extern s32 gThirdPlaceGoldReward[];

void initPlayerFinishPositionTask(FinishPositionDisplayState *arg0) {
    GameState *state;
    s32 index;
    s32 numPlayers;
    void *romStart;
    void *romEnd;
    s32 size;

    state = (GameState *)getCurrentAllocation();
    index = arg0->playerIndex;
    arg0->player = (Player *)((u8 *)state->players + index * 0xBE8);

    numPlayers = state->unk5F;
    if (numPlayers < 3) {
        if (numPlayers != 0) {
            arg0->x = -0x88;
            arg0->y = 0x40;
            if (state->unk5F == 2) {
                arg0->y = 0x10;
            }
            romStart = &_3F3940_ROM_START;
            romEnd = &_3F3D10_ROM_START;
            size = 0x888;
            goto dma_and_callbacks;
        }
    }

    romStart = &_3F3D10_ROM_START;
    romEnd = &_3F3EF0_ROM_START;
    size = 0x288;
    arg0->x = -0x44;
    arg0->y = 0x20;

dma_and_callbacks:
    arg0->asset = loadCompressedData(romStart, romEnd, size);
    setCleanupCallback(cleanupPlayerFinishPositionTask);
    setCallback(updatePlayerFinishPositionDisplay);
}

void updatePlayerFinishPositionDisplay(FinishPositionDisplayState *state) {
    state->spriteIndex = state->player->finishPosition;
    debugEnqueueCallback((u16)(state->playerIndex + 8), 0, func_8000FED0_10AD0, state);
}

void cleanupPlayerFinishPositionTask(FinishPositionDisplayState *state) {
    state->asset = freeNodeMemory(state->asset);
}

void initPlayerItemDisplayTask(PlayerItemDisplayState *state) {
    GameState *gameState;
    s32 playerMode;

    gameState = (GameState *)getCurrentAllocation();
    state->player = &gameState->players[state->playerIndex];
    playerMode = gameState->unk5F;

    if (playerMode >= 3) {
        goto else_branch;
    }
    if (playerMode == 0) {
        goto else_branch;
    }

    if (playerMode == 1) {
        state->primaryItemX = -0x20;
        state->primaryItemY = -0x60;
        state->secondaryItemX = 0;
        state->secondaryItemY = -0x60;
    } else {
        state->primaryItemX = -0x88;
        state->primaryItemY = -0x30;
        state->secondaryItemX = -0x68;
        state->secondaryItemY = -0x30;
    }
    state->secondaryItemAsset = state->primaryItemAsset =
        loadCompressedData(&_3F3EF0_ROM_START, &_3F3EF0_ROM_END, 0x2608);
    state->itemCountX = state->primaryItemX + 0x18;
    state->itemCountY = state->primaryItemY + 0x10;
    state->digitAsset = loadCompressedData(&_3F6950_ROM_START, &_3F6950_ROM_END, 0x508);
    goto callbacks;

else_branch:
    state->primaryItemX = -0x10;
    state->primaryItemY = -0x30;
    state->secondaryItemX = 0;
    state->secondaryItemY = -0x30;
    state->secondaryItemAsset = state->primaryItemAsset =
        loadCompressedData(&_3F58E0_ROM_START, &_3F58E0_ROM_END, 0xB08);
    state->unk28 = 0;
    state->charDisplayPtr = &state->charDisplayValue;
    state->charDisplayX = state->primaryItemX + 8;
    state->charDisplayY = state->primaryItemY + 8;
    state->digitAsset = loadCompressedData(&_3F3EF0_ROM_START, &_3F3EF0_ROM_END, 0x2608);
    state->charDisplayFlag = 0;

callbacks:
    if (gameState->unk5F < 3) {
        setCallbackWithContinue(updatePlayerItemDisplaySinglePlayer);
    } else {
        setCallbackWithContinue(updatePlayerItemDisplayMultiplayer);
    }
    setCleanupCallback(cleanupPlayerItemDisplayTask);
}

void updatePlayerItemDisplaySinglePlayer(PlayerItemDisplayState *state) {
    Player *player;
    Player *playerRef;
    u8 tempValue;
    void *callback;

    player = state->player;
    tempValue = player->unkBD3;
    if (tempValue != 0) {
        state->itemCountValue = tempValue;
        debugEnqueueCallback((state->playerIndex + 8) & 0xFFFF, 0, func_8000FED0_10AD0, &state->itemCountX);
    }

    callback = func_8000FED0_10AD0;
    tempValue = state->player->unkBD2;
    state->primaryItemIndex = tempValue;
    debugEnqueueCallback((state->playerIndex + 8) & 0xFFFF, 0, callback, state);

    player = state->player;
    if ((player->unkBD8 & 1) != 0) {
        func_8005100C_51C0C((s32)state->primaryItemX - 8, (s32)state->primaryItemY - 8, 0, state->playerIndex + 8, 0);
        playerRef = state->player;
        tempValue = playerRef->unkBD8;
        playerRef->unkBD8 = tempValue & 0xFE;
    }

    tempValue = state->player->unkBD4;
    state->secondaryItemIndex = tempValue + 7;
    debugEnqueueCallback((state->playerIndex + 8) & 0xFFFF, 0, callback, &state->secondaryItemX);

    player = state->player;
    if ((player->unkBD8 & 2) != 0) {
        func_8005100C_51C0C(
            (s32)state->secondaryItemX - 8,
            (s32)state->secondaryItemY - 8,
            1,
            state->playerIndex + 8,
            0
        );
        playerRef = state->player;
        tempValue = playerRef->unkBD8;
        playerRef->unkBD8 = tempValue & 0xFD;
    }
}

void updatePlayerItemDisplayMultiplayer(PlayerItemDisplayState *state) {
    Player *player;
    u8 tempValue;
    Player *playerRef;
    void *callback;

    player = state->player;
    tempValue = player->unkBD3;
    if (tempValue != 0) {
        state->charDisplayValue = tempValue + 0x30;
        debugEnqueueCallback((state->playerIndex + 8) & 0xFFFF, 0, renderTextPalette, &state->charDisplayX);
    }

    callback = func_8000FED0_10AD0;
    tempValue = state->player->unkBD2;
    state->primaryItemIndex = tempValue;
    debugEnqueueCallback((state->playerIndex + 8) & 0xFFFF, 0, callback, state);

    player = state->player;
    if ((player->unkBD8 & 1) != 0) {
        func_8005100C_51C0C((s32)state->primaryItemX - 4, (s32)state->primaryItemY - 4, 0, state->playerIndex + 8, 1);
        playerRef = state->player;
        tempValue = playerRef->unkBD8;
        playerRef->unkBD8 = tempValue & 0xFE;
    }

    tempValue = state->player->unkBD4;
    state->secondaryItemIndex = tempValue + 7;
    debugEnqueueCallback((state->playerIndex + 8) & 0xFFFF, 0, callback, &state->secondaryItemX);

    player = state->player;
    if ((player->unkBD8 & 2) != 0) {
        func_8005100C_51C0C(
            (s32)state->secondaryItemX - 4,
            (s32)state->secondaryItemY - 4,
            1,
            state->playerIndex + 8,
            1
        );
        playerRef = state->player;
        tempValue = playerRef->unkBD8;
        playerRef->unkBD8 = tempValue & 0xFD;
    }
}

void cleanupPlayerItemDisplayTask(Struct_func_8004C6F0 *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk1C = freeNodeMemory(arg0->unk1C);
}

void initPlayerLapCounterTask(LapCounterState *state) {
    LapCounterAllocation *allocation;
    char *textBuffer;
    s16 temp;

    allocation = (LapCounterAllocation *)getCurrentAllocation();
    state->player = (void *)(((u8 *)allocation->players) + (state->playerIndex * 0xBE8));

    if (allocation->numPlayers == 1) {
        state->x = -0x88;
        state->y = -0x60;
        state->lapIconAsset = loadCompressedData(&_3F6420_ROM_START, &_3F6420_ROM_END, 0x168);
        state->spriteIndex = 0;
        state->digitX1 = ((u16)state->x) + 0x1C;
        state->digitY1 = state->y;
        state->digitsAsset = loadCompressedData(&_3F6950_ROM_START, &_3F6950_ROM_END, 0x508);
        state->digitX2 = ((u16)state->digitX1) + 8;
        state->unk16 = 1;
        state->unk20 = 1;
        state->digitY2 = state->y;
        state->unk1C = state->lapIconAsset;
        temp = state->digitX2;
        state->digitY3 = state->y;
        state->unk28 = state->digitsAsset;
        state->digitX3 = ((u16)temp) + 8;
        state->totalLaps = allocation->totalLaps + 1;
        state->unk2E = 3;
    } else {
        state->y = -0x30;
        if (allocation->numPlayers == 2) {
            state->x = -0x18;
            state->textX = 0;
            state->textY = state->y;
        } else {
            if (state->playerIndex < 2) {
                state->x = -0x44;
            } else {
                state->x = 0x2C;
            }
            state->textX = state->x;
            state->textY = ((u16)state->y) + 8;
        }
        textBuffer = state->lapTextBuffer;
        state->lapIconAsset = loadCompressedData(&_3F65C0_ROM_START, &_3F65C0_ROM_END, 0x98);
        state->spriteIndex = 0;
        state->digitsAsset = 0;
        sprintf(textBuffer, D_8009E868_9F468, 1, allocation->totalLaps + 1);
        state->unk34 = 1;
        state->lapText = textBuffer;
    }

    setCleanupCallback(cleanupPlayerLapCounterTask);
    if (allocation->numPlayers == 1) {
        setCallbackWithContinue(updatePlayerLapCounterSinglePlayer);
    } else {
        setCallbackWithContinue(updatePlayerLapCounterMultiplayer);
    }
}

void updatePlayerLapCounterSinglePlayer(LapCounterSinglePlayerState *state) {
    debugEnqueueCallback((u16)(state->playerIndex + 8), 0, func_8000FED0_10AD0, state);
    state->currentLap = state->player->unkBC5 + 1;
    debugEnqueueCallback((u16)(state->playerIndex + 8), 0, func_80010240_10E40, &state->digitX1);
    debugEnqueueCallback((u16)(state->playerIndex + 8), 0, func_8000FED0_10AD0, &state->digitX2);
    debugEnqueueCallback((u16)(state->playerIndex + 8), 0, func_80010240_10E40, &state->digitX3);
}

void updatePlayerLapCounterMultiplayer(LapCounterMultiplayerState *state) {
    debugEnqueueCallback((u16)(state->playerIndex + 8), 0, func_8000FED0_10AD0, state);
    state->unk3C = state->player->unkBC5 + 0x31;
    debugEnqueueCallback((u16)(state->playerIndex + 8), 0, renderTextPalette, &state->unk30);
}

void cleanupPlayerLapCounterTask(Struct_func_8004DCC4 *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk10 = freeNodeMemory(arg0->unk10);
}

// Callback struct types (needed for forward declarations)
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

// Forward declarations for callbacks
void updatePlayerGoldDisplaySinglePlayer(PlayerGoldDisplayState *state);
void updatePlayerGoldDisplayMultiplayer(MultiplayerGoldDisplayState *state);
void cleanupPlayerGoldDisplayTask(PlayerGoldDisplayCleanupArg *arg0);

struct GoldDisplayState_s {
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
};

void initPlayerGoldDisplayTask(GoldDisplayState *state) {
    GameState *gameState;
    void *digitsRomStart;
    s32 numPlayers;

    gameState = getCurrentAllocation();
    state->player = (Player *)((u8 *)gameState->players + state->playerIndex * 0xBE8);

    numPlayers = gameState->unk5F;
    if (numPlayers >= 3) {
        state->textX = 0x12;
        goto multiplayer;
    }
    digitsRomStart = &_3F6950_ROM_START;
    if (numPlayers == 0) {
        goto multiplayer_setup;
    }

    // Singleplayer path
    state->x = 0x50;
    if (gameState->unk5F == 1) {
        state->y = 0x50;
    } else {
        if (state->playerIndex == 0) {
            state->y = -0x30;
        } else {
            state->y = 0x20;
        }
    }
    state->digitsTexture = loadCompressedData(digitsRomStart, &_3F6BB0_ROM_START, 0x508);
    state->iconX = state->x + 0x28;
    state->iconY = state->y;
    state->iconAsset = loadCompressedData(&_3F6670_ROM_START, &_3F6950_ROM_START, 0x388);
    goto common;

multiplayer_setup:
    state->textX = 0x12;
multiplayer:
    state->textY = 0x28;
    state->digitsTexture = 0;
    state->digitCount = 1;
    state->textPtr = state->goldTextBuffer;
    state->iconX = state->textX + 0x28;
    state->iconY = state->textY;
    state->iconAsset = loadCompressedData(&_3F6670_ROM_START, &_3F6950_ROM_START, 0x388);

common:
    state->animCounter = 0;
    setCleanupCallback(cleanupPlayerGoldDisplayTask);

    if (gameState->unk5F < 3) {
        setCallbackWithContinue(updatePlayerGoldDisplaySinglePlayer);
    } else {
        setCallbackWithContinue(updatePlayerGoldDisplayMultiplayer);
    }
}

extern char sGoldFormatShort[];
extern char sGoldFormatLong[];

void updatePlayerGoldDisplaySinglePlayer(PlayerGoldDisplayState *state) {
    s32 gold = state->player->unkB6C;

    if (gold < 100) {
        sprintf(state->goldTextBuffer, sGoldFormatShort, gold);
    } else {
        sprintf(state->goldTextBuffer, sGoldFormatLong, gold);
    }

    drawNumericString(state->goldTextBuffer, state->x, state->y, 0xFF, state->digitsTexture, state->playerIndex + 8, 0);

    state->animCounter++;
    if ((s16)state->animCounter >= 12) {
        state->animCounter = 0;
    }

    state->animFrame = (s16)state->animCounter >> 1;

    debugEnqueueCallback((u16)(state->playerIndex + 8), 0, func_8000FED0_10AD0, &state->iconX);
}

void updatePlayerGoldDisplayMultiplayer(MultiplayerGoldDisplayState *state) {
    s32 gold = state->player->unkB6C;

    if (gold < 100) {
        state->digitCount = 1;
    } else {
        state->digitCount = 2;
    }

    sprintf(state->goldTextBuffer, D_8009E880_9F480, state->player->unkB6C);

    debugEnqueueCallback((u16)(state->playerIndex + 8), 0, renderTextPalette, &state->textX);

    state->animCounter++;
    if ((s16)state->animCounter >= 12) {
        state->animCounter = 0;
    }

    state->animFrame = (s16)state->animCounter >> 1;

    debugEnqueueCallback((u16)(state->playerIndex + 8), 0, func_800105B0_111B0, &state->iconX);
}

void cleanupPlayerGoldDisplayTask(PlayerGoldDisplayCleanupArg *arg0) {
    arg0->goldIconAsset = freeNodeMemory(arg0->goldIconAsset);
    arg0->digitSpriteAsset = freeNodeMemory(arg0->digitSpriteAsset);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004CDC0_4D9C0);

typedef struct {
    u8 pad0[0x10];
    void *players;
    u8 pad14[0x4A];
    u8 numPlayers;
    u8 pad5F[0x5];
    u8 playerIndices[4];
} RaceProgressIndicatorAllocation;

typedef struct {
    u8 pad0[0xB88];
    s32 playerStateFlags;
    u8 padB8C[0xC];
    s16 raceProgress;
    u8 padB9A[0x35];
    u8 activeEffectCount;
} RaceProgressPlayerData;

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

void updatePlayerRaceProgressIndicator(RaceProgressIndicatorState *state) {
    RaceProgressIndicatorAllocation *allocation;
    s32 i;
    u8 playerIndex;
    RaceProgressPlayerData *playerData;
    RaceProgressIndicatorElement *elem;
    s32 targetPosition;
    s32 delta;
    s8 flashState;
    s16 currentPosition;
    s32 playerCount;
    u8 pad[0x8];

    allocation = getCurrentAllocation();

    playerCount = allocation->numPlayers;
    i = 0;
    if (playerCount > 0) {
        do {
            playerIndex = allocation->playerIndices[i];
            playerData = (RaceProgressPlayerData *)((u8 *)allocation->players + playerIndex * 0xBE8);

            targetPosition = (0x2000 - playerData->raceProgress) * 0x8C;
            elem = &state->elements[playerIndex];

            if (targetPosition < 0) {
                targetPosition += 0x1FFF;
            }

            currentPosition = elem->positionOffset;
            delta = (targetPosition >> 13) - currentPosition;

            if (delta < -4) {
                delta = -4;
            }
            if (delta >= 5) {
                delta = 4;
            }

            elem->positionOffset = currentPosition + delta;
            flashState = elem->flashState;

            switch (flashState) {
                case 0:
                    if (playerData->playerStateFlags & 0x10) {
                        elem->flashState = flashState + 1;
                        case 1:
                            elem->flashCounter++;
                            if ((s8)elem->flashCounter == 2) {
                                elem->flashState = elem->flashState + 1;
                            }
                    }
                    break;
                case 2:
                    if (!(playerData->playerStateFlags & 0x10)) {
                        elem->flashState = flashState + 1;
                        case 3:
                            elem->flashCounter--;
                            if ((elem->flashCounter << 24) == 0) {
                                elem->flashState = 0;
                            }
                    }
                    break;
            }

            elem->y = (u16)elem->positionOffset + state->baseY - 4;
            elem->spriteFrame = (s8)elem->flashCounter;

            if (playerData->activeEffectCount != 0) {
                elem->hasActiveEffect = 1;
            } else {
                elem->hasActiveEffect = 0;
            }

            debugEnqueueCallback(0xC, 0, func_80010240_10E40, elem);
            i++;
            playerCount = allocation->numPlayers;
        } while (i < playerCount);
    }

    debugEnqueueCallback(0xC, 0, func_8000FED0_10AD0, state);
}

typedef struct {
    void *playerIconAsset;
    u8 pad4[0x10];
} RaceProgressIndicatorPlayerEntry; // size 0x14

typedef struct {
    u8 pad0[0x4];
    void *baseAsset;
    u8 pad8[0x8];
    RaceProgressIndicatorPlayerEntry playerEntries[4]; // offset 0x10
} RaceProgressIndicatorCleanupState;

void cleanupRaceProgressIndicatorTask(RaceProgressIndicatorCleanupState *state) {
    s32 i;

    for (i = 0; i < 4; i++) {
        state->playerEntries[i].playerIconAsset = freeNodeMemory(state->playerEntries[i].playerIconAsset);
    }
    state->baseAsset = freeNodeMemory(state->baseAsset);
}

void initGoalBannerTask(GoalBannerState *state) {
    state->spriteAsset = loadAsset_34CB50();
    state->spriteFrame = 0xD;
    func_80058530_59130(0x11B, 6);
    state->holdFrames = 0x2D;
    state->animAngle = 0;
    state->yOffset = -8;
    setCleanupCallback(cleanupGoalBannerTask);
    setCallback(updateGoalBannerSlideIn);
}

void updateGoalBannerSlideIn(GoalBannerState *state) {
    s32 sinVal;
    s32 angle;

    angle = state->animAngle + 0x80;
    state->animAngle = angle;
    sinVal = approximateSin((s16)angle);
    state->xPos = (0x2000 - sinVal) / 20 - 0x38;
    if (state->animAngle == 0x800) {
        setCallback(updateGoalBannerHold);
    }
    debugEnqueueCallback((u16)(state->playerIndex + 8), 6, func_8000FED0_10AD0, state);
}

void updateGoalBannerHold(GoalBannerState *state) {
    state->holdFrames--;
    if (state->holdFrames == 0) {
        setCallback(updateGoalBannerSlideOut);
    }
    debugEnqueueCallback((u16)(state->playerIndex + 8), 6, func_8000FED0_10AD0, state);
}

void updateGoalBannerSlideOut(GoalBannerState *state) {
    s32 sinVal;
    s32 angle;

    angle = state->animAngle + 0x80;
    state->animAngle = angle;
    sinVal = approximateSin((s16)angle);
    state->xPos = -((0x2000 - sinVal) / 20) - 0x38;
    if (state->animAngle == 0x1000) {
        func_80069CF8_6A8F8();
    }
    debugEnqueueCallback((u16)(state->playerIndex + 8), 6, func_8000FED0_10AD0, state);
}

void cleanupGoalBannerTask(GoalBannerState *state) {
    state->spriteAsset = freeNodeMemory(state->spriteAsset);
}

void showGoalBanner(s32 playerIndex) {
    GoalBannerState *task;

    task = scheduleTask(initGoalBannerTask, 0, 0, 0xE6);
    if (task != NULL) {
        task->playerIndex = playerIndex;
    }
}

void initCenteredSpritePopupTask(CenteredSpritePopupState *state) {
    state->spriteAsset = loadAsset_34CB50();
    setCleanupCallback(cleanupCenteredSpritePopupTask);
    setCallback(updateCenteredSpritePopup);
}

void updateCenteredSpritePopup(CenteredSpritePopupState *state) {
    OutputStruct_19E80 output;

    getTableEntryByU16Index(state->spriteAsset, state->spriteIndex, &output);
    state->xPos = -output.field1 / 2;
    state->yPos = -output.field2 / 2;
    debugEnqueueCallback((u16)(state->playerIndex + 8), 6, func_8000FED0_10AD0, state);
}

void cleanupCenteredSpritePopupTask(CenteredSpritePopupState *state) {
    state->spriteAsset = freeNodeMemory(state->spriteAsset);
}

extern u8 D_80090E60_91A60[];

void showPlacementAnnouncement(u32 playerIndex, s32 placement) {
    CenteredSpritePopupState *task;

    task = scheduleTask(initCenteredSpritePopupTask, 0, 1, 0xE6);
    if (task != NULL) {
        task->playerIndex = playerIndex;
        task->spriteIndex = D_80090E60_91A60[placement];

        switch (placement) {
            case 0:
                func_80058530_59130(0x118, 6);
                break;
            case 1:
                func_80058530_59130(0x119, 6);
                break;
            case 2:
                func_80058530_59130(0x11A, 6);
                break;
            case 3:
                func_80058530_59130(0x134, 6);
                break;
        }
    }
}

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E884_9F484);

extern char D_8009E884_9F484[];

static const char D_8009E88C_9F48C[] = "%4dG";

void initTrickScoreDisplayTask(TrickScoreDisplayState *state) {
    GameState *gameState = (GameState *)getCurrentAllocation();

    if (gameState->unk5F == 1) {
        state->useGoldFormat = 0;
    } else {
        state->useGoldFormat = 1;
    }
    state->spriteAsset = loadAsset_34CB50();
    if (state->useGoldFormat == 0) {
        state->yPos = -0x20;
        state->spriteFrame = 0xF;
        state->digitsTexture = loadCompressedData(&_3F6950_ROM_START, &_3F6950_ROM_END, 0x508);
        sprintf(state->scoreText, D_8009E884_9F484, state->score);
    } else {
        state->yPos = -0x18;
        state->spriteFrame = 0x10;
        state->digitsTexture = 0;
        state->textScale = 4;
        state->textPtr = state->scoreText;
        state->textY = state->yPos;
        sprintf(state->scoreText, D_8009E88C_9F48C, state->score);
    }
    state->animAngle = 0;
    state->holdTimer = 0x1E;
    setCleanupCallback(cleanupTrickScoreDisplayTask);
    setCallback(updateTrickScoreSlideIn);
}

void renderTrickScoreDisplay(TrickScoreDisplayState *state) {
    debugEnqueueCallback(state->playerIndex + 8, 6, func_8000FED0_10AD0, state);

    if (state->useGoldFormat == 0) {
        drawNumericString(
            state->scoreText,
            state->xPos + 0x38,
            state->yPos,
            0xFF,
            state->digitsTexture,
            state->playerIndex + 8,
            6
        );
    } else {
        state->textX = state->xPos + 0x38;
        debugEnqueueCallback(state->playerIndex + 8, 6, renderTextPalette, &state->textX);
    }
}

void updateTrickScoreSlideIn(TrickScoreDisplayState *state) {
    s32 sinVal;
    s16 angle;

    angle = state->animAngle + 0x80;
    state->animAngle = angle;
    sinVal = approximateSin(angle);
    state->xPos = (0x2000 - sinVal) / 20 - 0x30;
    if (state->animAngle == 0x800) {
        setCallback(updateTrickScoreHold);
    }
    renderTrickScoreDisplay(state);
}

void updateTrickScoreHold(TrickScoreDisplayState *state) {
    state->holdTimer--;
    if ((s16)state->holdTimer == 0) {
        setCallback(updateTrickScoreSlideOut);
    }
    renderTrickScoreDisplay(state);
}

void updateTrickScoreSlideOut(TrickScoreDisplayState *state) {
    s32 sinVal;
    s16 angle;

    angle = state->animAngle + 0x80;
    state->animAngle = angle;
    sinVal = approximateSin(angle);
    state->xPos = -((0x2000 - sinVal) / 20) - 0x30;
    if (state->animAngle == 0x1000) {
        func_80069CF8_6A8F8();
    }
    renderTrickScoreDisplay(state);
}

void cleanupTrickScoreDisplayTask(Struct_func_8004F04C *state) {
    state->unk4 = freeNodeMemory(state->unk4);
    state->unkC = freeNodeMemory(state->unkC);
}

typedef struct {
    u8 pad0[0x18];
    s32 score;
    u8 pad1C[0xC];
    s16 playerIndex;
} TrickScoreDisplaySpawnState;

void showTrickScoreDisplay(s32 playerIndex, s32 trickScore) {
    TrickScoreDisplaySpawnState *task;

    task = scheduleTask(initTrickScoreDisplayTask, 0, 0, 0xE6);
    if (task != NULL) {
        task->playerIndex = (s16)playerIndex;
        task->score = trickScore;
    }
}

void initSpeedCrossFinishPositionTask(FinishPositionDisplayState *arg0) {
    GameState *state = (GameState *)getCurrentAllocation();

    arg0->player = state->players;
    arg0->asset = loadCompressedData(&_3F3940_ROM_START, &_3F3940_ROM_END, 0x888);
    arg0->x = -0x48;
    arg0->y = -0x38;
    setCleanupCallback(&cleanupSpeedCrossFinishPositionTask);
    setCallback(&updateSpeedCrossFinishPositionDisplay);
}

void updateSpeedCrossFinishPositionDisplay(FinishPositionDisplayState *arg0) {
    arg0->spriteIndex = arg0->player->finishPosition;
    debugEnqueueCallback(8, 6, &func_8000FED0_10AD0, arg0);
}

void cleanupSpeedCrossFinishPositionTask(FinishPositionDisplayState *arg0) {
    arg0->asset = freeNodeMemory(arg0->asset);
}

typedef struct {
    u8 pad0[0xA];
    s16 unkA;
    u8 unkC;
    u8 unkD;
    u8 unkE;
} HudElementState;

void initHudElementState(HudElementState *arg0) {
    arg0->unkC = 0;
    arg0->unkD = 0;
    arg0->unkE = 0;
    arg0->unkA = 0xFF;
}

void updateGoldAwardDisplay(GoldAwardDisplayState *arg0);
void cleanupGoldAwardDisplayTask(GoldAwardDisplayState *arg0);

void initGoldAwardDisplayTask(GoldAwardDisplayState *arg0) {
    GameState *gameState = (GameState *)getCurrentAllocation();
    s32 totalGold;
    u8 shotCrossScore;
    s32 meterValue;

    arg0->alpha = 0;
    arg0->spriteAsset = loadAsset_34CB50();
    initHudElementState((HudElementState *)arg0);
    arg0->spriteIndex = 0x14;
    arg0->digitAsset = loadCompressedData(&_3F6950_ROM_START, &_3F6BB0_ROM_START, 0x508);

    switch (gameState->unk7A) {
        case 5:
            shotCrossScore = gameState->unk5A;
            arg0->x = 0x10;
            arg0->y = -0x48;
            arg0->goldAmount = shotCrossScore * 0x12C;
            break;
        case 6:
            meterValue = gameState->players->unkB70;
            arg0->x = 0x10;
            arg0->y = -0x48;
            arg0->goldAmount = meterValue * 0x32;
            break;
        default:
            switch (gameState->players->finishPosition) {
                case 0:
                    arg0->goldAmount = gFirstPlaceGoldReward[gameState->memoryPoolId];
                    break;
                case 1:
                    arg0->goldAmount = gSecondPlaceGoldReward[gameState->memoryPoolId];
                    break;
                case 2:
                    arg0->goldAmount = gThirdPlaceGoldReward[gameState->memoryPoolId];
                    break;
                default:
                    arg0->goldAmount = 0;
                    break;
            }
            arg0->x = 0x10;
            arg0->y = -0x3C;
            totalGold = arg0->goldAmount + gameState->players->unkB6C;
            arg0->goldAmount = totalGold;
            if (totalGold > 0x1869F) {
                arg0->goldAmount = 0x1869F;
            }
            break;
    }

    setCleanupCallback(cleanupGoldAwardDisplayTask);
    setCallback(updateGoldAwardDisplay);
}

extern char D_8009E894_9F494[];

typedef struct TotalGoldDisplayState TotalGoldDisplayState;
extern void initTotalGoldDisplayTask(TotalGoldDisplayState *);

void updateGoldAwardDisplay(GoldAwardDisplayState *arg0) {
    char buf[24];
    s32 strLen;
    char *ptr;
    s32 alphaVal;
    s32 yPos;
    s32 xPos;
    GameState *gameState;
    s32 gameMode;

    gameState = (GameState *)getCurrentAllocation();
    alphaVal = arg0->alpha;

    if (alphaVal != 0xFF) {
        alphaVal = arg0->alpha = alphaVal + 0x10;
        if (alphaVal >= 0x100) {
            arg0->alpha = 0xFF;
            gameMode = gameState->unk7A;
            if (gameMode < 7) {
                if (gameMode < 5) {
                    scheduleTask(initTotalGoldDisplayTask, 1, 0, 0xE6);
                } else {
                    scheduleTask(func_8004F1D4_4FDD4, 1, 0, 0xE6);
                }
            } else {
                scheduleTask(initTotalGoldDisplayTask, 1, 0, 0xE6);
            }
        }
    }

    arg0->padA[4] = (u8)arg0->alpha;
    debugEnqueueCallback(8, 6, func_80012518_13118, arg0);

    sprintf(buf, D_8009E894_9F494, arg0->goldAmount);

    strLen = 0;
    ptr = &buf[1];
    if (*ptr != 0) {
        do {
            ptr++;
            strLen++;
        } while (*ptr != 0);
    }

    yPos = arg0->y;
    xPos = arg0->x;
    drawNumericString(
        buf,
        (s16)(xPos + ((0x48 - (strLen << 3)) >> 1)),
        (s16)(yPos + 0x14),
        (s16)arg0->alpha,
        arg0->digitAsset,
        8,
        6
    );
}

void cleanupGoldAwardDisplayTask(GoldAwardDisplayState *arg0) {
    arg0->spriteAsset = freeNodeMemory(arg0->spriteAsset);
    arg0->digitAsset = freeNodeMemory(arg0->digitAsset);
}

struct TotalGoldDisplayState {
    s16 x;
    s16 y;
    void *spriteAsset;
    s16 spriteIndex;
    u8 padA[0x4];
    u8 unkE;
    u8 padF;
    void *digitAsset;
    s32 alpha;
};

void updateTotalGoldDisplay(TotalGoldDisplayState *arg0);
void cleanupTotalGoldDisplayTask(Struct_func_8004DCC4 *);

void initTotalGoldDisplayTask(TotalGoldDisplayState *arg0) {
    GameState *allocation = (GameState *)getCurrentAllocation();

    arg0->alpha = 0;
    arg0->spriteAsset = loadAsset_34CB50();
    initHudElementState((HudElementState *)arg0);
    arg0->spriteIndex = 0x15;
    arg0->digitAsset = loadCompressedData(&_3F6950_ROM_START, &_3F6950_ROM_END, 0x508);

    switch (allocation->unk7A) {
        case 5:
            arg0->x = 0x10;
            arg0->y = 8;
            break;
        case 6:
            arg0->x = 0x10;
            arg0->y = 8;
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        default:
            arg0->x = 0x10;
            arg0->y = -0x10;
            break;
    }

    setCleanupCallback(cleanupTotalGoldDisplayTask);
    setCallback(updateTotalGoldDisplay);
}

void updateTotalGoldDisplay(TotalGoldDisplayState *arg0) {
    char buf[24];
    s32 strLen;
    char *ptr;
    s32 alphaVal;
    s32 yPos;
    s32 xPos;

    alphaVal = arg0->alpha;
    if (alphaVal != 0xFF) {
        alphaVal += 0x10;
        arg0->alpha = alphaVal;
        if (alphaVal >= 0x100) {
            arg0->alpha = 0xFF;
        }
    }

    arg0->unkE = (u8)arg0->alpha;
    debugEnqueueCallback(8, 6, func_80012518_13118, arg0);

    sprintf(buf, D_8009E894_9F494, getPlayerGold());

    strLen = 0;
    ptr = &buf[1];
    if (*ptr != 0) {
        do {
            ptr++;
            strLen++;
        } while (*ptr != 0);
    }

    yPos = arg0->y;
    xPos = arg0->x;
    drawNumericString(
        buf,
        (s16)(xPos + ((0x48 - (strLen << 3)) >> 1)),
        (s16)(yPos + 0x14),
        (s16)arg0->alpha,
        arg0->digitAsset,
        8,
        6
    );
}

void cleanupTotalGoldDisplayTask(Struct_func_8004DCC4 *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk10 = freeNodeMemory(arg0->unk10);
}

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

void updateTotalLapDisplay(TotalLapDisplayState *state);
void cleanupTotalLapDisplayTask(Struct_func_8004DCC4 *);

void initTotalLapDisplayTask(TotalLapDisplayState *state) {
    D_800AFE8C_A71FC_type *global;

    getCurrentAllocation();
    state->alpha = 0;
    state->spriteAsset = loadAsset_34CB50();
    initHudElementState((HudElementState *)state);
    global = D_800AFE8C_A71FC;
    state->spriteIndex = 0x16;
    state->x = 0;

    if (global->unk9[state->player->unkBB8 + 0x11] < 10) {
        state->x = -4;
    }

    state->y = 0x10;
    state->digitAsset = loadCompressedData(&_3F6950_ROM_START, &_3F6950_ROM_END, 0x508);
    setCleanupCallback(cleanupTotalLapDisplayTask);
    setCallback(updateTotalLapDisplay);
}

void updateTotalLapDisplay(TotalLapDisplayState *state) {
    char buffer[16];
    s32 alphaValue;
    u8 lapCount;
    Player *player;

    alphaValue = state->alpha;
    if (alphaValue != 0xFF) {
        alphaValue += 0x10;
        state->alpha = alphaValue;
        if (alphaValue >= 0x100) {
            state->alpha = 0xFF;
        }
    }

    player = state->player;
    if (player->finishPosition == 0 && (gFrameCounter & 1)) {
        lapCount = D_800AFE8C_A71FC->unk9[player->unkBB8 + 0x11];
        sprintf(buffer, D_8009E89C_9F49C, lapCount);
    } else {
        lapCount = D_800AFE8C_A71FC->unk9[state->player->unkBB8 + 0x11];
        sprintf(buffer, D_8009E8A0_9F4A0, lapCount);
    }

    state->unkE = (u8)state->alpha;

    debugEnqueueCallback(state->player->unkBB8 + 8, 6, func_80012518_13118, state);

    drawNumericString(
        buffer,
        (s16)((u16)state->x - 0x18),
        state->y,
        state->alpha,
        state->digitAsset,
        (s16)(state->player->unkBB8 + 8),
        6
    );
}

void cleanupTotalLapDisplayTask(Struct_func_8004DCC4 *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk10 = freeNodeMemory(arg0->unk10);
}

void spawnTotalLapDisplayTask(Player *player) {
    Node *task;

    task = scheduleTask(initTotalLapDisplayTask, 1, 0, 0xE6);
    if (task != NULL) {
        task->unk18 = player;
    }
}

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

void cleanupVictorySnowflake(VictorySnowflakeState *state);
void updateVictorySnowflakeWave(VictorySnowflakeState *state);
void renderVictorySnowflake(VictorySnowflakeState *state);
void renderVictorySnowflakeSmall(VictorySnowflakeState *state);
void updateVictorySnowflakeDrift(VictorySnowflakeState *state);

void initVictorySnowflake(VictorySnowflakeState *state) {
    u8 movementType;

    state->spriteAsset = loadAsset_34CB50();
    movementType = randA();

    switch (movementType & 1) {
        case 0:
            state->posX = ((randA() & 0xFF) - 0x80) << 4;
            state->posY = -0x780;
            state->waveAmplitude = randA() & 0xF;
            state->wavePhase = (randA() & 0xFF) << 4;
            if (randA() & 1) {
                state->velocityY = (randA() & 7) | 0x40;
            } else {
                state->velocityY = (randA() & 7) | 0x20;
            }
            setCallback(updateVictorySnowflakeWave);
            break;
        case 1:
            state->posX = ((randA() & 0xFF) - 0x80) << 4;
            state->posY = -0x780;
            if (randA() & 1) {
                state->velocityY = (randA() & 7) + 0x3C;
            } else {
                state->velocityY = (randA() & 7) + 0x22;
            }
            state->wavePhase = (randA() & 1) - 1;
            state->velocityX = (-((randA() & 0xFF) << 3)) & 0xF;
            state->driftTimer = randA() & 0xF;
            __asm__ volatile("" ::: "memory");
            setCallback(updateVictorySnowflakeDrift);
            break;
    }

    state->lifetime = (randA() & 0xFF) % 5 + 12;
    setCleanupCallback(cleanupVictorySnowflake);
}

void updateVictorySnowflakeDrift(VictorySnowflakeState *state) {
    if (state->wavePhase >= 0) {
        if (state->driftTimer != 0) {
            __asm__ volatile("" ::: "memory");
            state->velocityX += 3;
            state->driftTimer--;
        } else {
            state->velocityX -= 3;
            if ((state->velocityX << 16) == 0) {
                state->driftTimer = (randA() & 0x1F) + 0x10;
                state->wavePhase = -1;
            }
        }
    } else {
        if (state->driftTimer != 0) {
            __asm__ volatile("" ::: "memory");
            state->velocityX -= 3;
            state->driftTimer--;
        } else {
            state->velocityX += 3;
            if ((state->velocityX << 16) == 0) {
                state->driftTimer = (randA() & 0x1F) + 0x10;
                state->wavePhase = 1;
            }
        }
    }

    state->posX += state->velocityX;
    state->posY += state->velocityY;

    if (state->posY >= 0x6E1) {
        func_80069CF8_6A8F8();
    }

    if (state->useSmallSprite != 0) {
        renderVictorySnowflakeSmall(state);
    } else {
        renderVictorySnowflake(state);
    }
}

void updateVictorySnowflakeWave(VictorySnowflakeState *state) {
    s32 sinVal;

    state->posY = state->posY + state->velocityY;
    state->wavePhase = (state->wavePhase + 0x20) & 0x1FFF;

    sinVal = approximateSin(state->wavePhase);

    state->posX = state->posX + (sinVal * (state->waveAmplitude + 0x30)) / 8192;

    if (state->wavePhase == 0 || state->wavePhase == 0x1000) {
        state->waveAmplitude = randA() & 0xF;
    }

    if (state->posY >= 0x6E1) {
        func_80069CF8_6A8F8();
    }

    if (state->useSmallSprite != 0) {
        renderVictorySnowflakeSmall(state);
    } else {
        renderVictorySnowflake(state);
    }
}

void renderVictorySnowflake(VictorySnowflakeState *state) {
    GameState *allocation = (GameState *)getCurrentAllocation();
    s32 animIndex;
    u8 poolId;

    animIndex = (state->animFrame + 1) & 3;
    state->animFrame = animIndex;

    poolId = allocation->memoryPoolId;
    if (poolId == 3) {
        goto set_26;
    }
    if (poolId != 8) {
        state->spriteFrame = animIndex + 0x17;
    } else {
    set_26:
        state->spriteFrame = animIndex + 0x26;
    }

    state->screenX = state->posX >> 4;
    state->screenY = state->posY >> 4;
    debugEnqueueCallback((u16)(state->playerIndex + 8), 0, func_80010240_10E40, state);
}

void renderVictorySnowflakeSmall(VictorySnowflakeState *state) {
    GameState *allocation = (GameState *)getCurrentAllocation();
    s32 animIndex;
    u8 poolId;

    animIndex = (state->animFrame + 1) & 3;
    state->animFrame = animIndex;

    poolId = allocation->memoryPoolId;
    if (poolId == 3) {
        goto set_26;
    }
    if (poolId != 8) {
        state->spriteFrame = animIndex + 0x17;
    } else {
    set_26:
        state->spriteFrame = animIndex + 0x26;
    }

    state->screenX = state->posX >> 5;
    state->screenY = state->posY >> 5;
    debugEnqueueCallback((u16)(state->playerIndex + 8), 0, func_80010924_11524, state);
}

void cleanupVictorySnowflake(VictorySnowflakeState *state) {
    state->spriteAsset = freeNodeMemory(state->spriteAsset);
}

typedef struct {
    s16 playerIndex;
    s16 useSmallSprite;
} VictorySnowflakeSpawnArgs;

typedef struct {
    u8 pad0[0xC];
    s16 playerIndex;
    s16 useSmallSprite;
} VictorySnowflakeTaskState;

extern u16 D_8009ADE0_9B9E0;

void conditionalSpawnVictorySnowflake(VictorySnowflakeSpawnArgs *args) {
    VictorySnowflakeTaskState *task;

    if (D_8009ADE0_9B9E0 & 1) {
        task = scheduleTask(initVictorySnowflake, 2, 0, 0xE6);
        if (task != NULL) {
            task->playerIndex = args->playerIndex;
            task->useSmallSprite = args->useSmallSprite;
        }
    }
}

void spawnVictorySnowflakes(s16 playerIndex, s16 useSmallSprite) {
    VictorySnowflakeSpawnArgs *task;

    task = scheduleTask(&conditionalSpawnVictorySnowflake, 1, 0, 0xE5);
    if (task != NULL) {
        task->playerIndex = playerIndex;
        task->useSmallSprite = useSmallSprite;
    }
}

typedef struct {
    u8 pad0[0x4];
    void *spriteAsset;
    u8 pad8[0x1C];
    void *backgroundAsset;
} PauseMenuCleanupState;

void renderPauseMenuDisplay(PauseMenuDisplayState *);
void cleanupPauseMenuDisplayTask(PauseMenuCleanupState *);

void initPauseMenuDisplayTask(PauseMenuDisplayState *state) {
    s32 i;
    s16 xPos;
    s32 yPos;

    getCurrentAllocation();
    state->backgroundAsset = loadAsset_34F7E0();
    state->elements[0].spriteAsset = loadAsset_34CB50();

    i = 0;
    xPos = -0x1C;
    yPos = -0xC;
    do {
        state->elements[i].x = xPos;
        state->elements[i].y = yPos;
        state->elements[i].spriteAsset = state->elements[0].spriteAsset;
        yPos += 8;
        i++;
    } while (i < 3);

    state->elements[0].spriteIndex = 0x1B;
    state->elements[1].spriteIndex = 0x1C;
    state->elements[2].spriteIndex = 0x1D;

    setCleanupCallback(cleanupPauseMenuDisplayTask);
    setCallback(renderPauseMenuDisplay);
}

void renderPauseMenuDisplay(PauseMenuDisplayState *state) {
    GameState *gameState;
    s32 i;

    gameState = (GameState *)getCurrentAllocation();
    i = 0;
    if (gameState->gamePaused == 1) {
        do {
            if (gameState->pauseMenuSelection == i) {
                state->elements[i].padA[0] = 0x12;
            } else {
                state->elements[i].padA[0] = 0x11;
            }
            debugEnqueueCallback(0xC, 6, func_80010240_10E40, &state->elements[i]);
            i++;
        } while (i < 3);
        func_8006D4B8_6E0B8(state->backgroundAsset, -0x20, -8, 4, 1, 0, 0x80, 0, 0, 0xFF, 0x80, 0xC, 6);
    }
}

void cleanupPauseMenuDisplayTask(PauseMenuCleanupState *state) {
    state->spriteAsset = freeNodeMemory(state->spriteAsset);
    state->backgroundAsset = freeNodeMemory(state->backgroundAsset);
}

typedef struct {
    u8 pad0[0x10];
    void *gameState;
} ShotScoreDisplayAllocation;

typedef struct {
    u8 pad0[0x17C3];
    u8 itemCount;
} ShotScoreData;

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

void updateShotScoreDisplay(ShotScoreDisplayState *);
void cleanupShotScoreDisplayTask(ShotScoreDisplayState *);

void initShotScoreDisplayTask(ShotScoreDisplayState *arg0) {
    ShotScoreDisplayAllocation *allocation = (ShotScoreDisplayAllocation *)getCurrentAllocation();
    s32 i;
    u8 val;
    void *p;

    val = ((ShotScoreData *)allocation->gameState)->itemCount;
    arg0->animDelayCounter = 0;
    arg0->displayedCount = val;
    arg0->elements[0].spriteAsset = loadAssetByIndex_95470(arg0->spriteGroupIndex);

    for (i = 0; i < 10; i++) {
        p = arg0->elements[0].spriteAsset;
        arg0->elements[i].spriteFrame = 0;
        arg0->elements[i].y = 0x58;
        arg0->elements[i].spriteAsset = p;
    }

    setCleanupCallback(cleanupShotScoreDisplayTask);
    setCallback(updateShotScoreDisplay);
}

void updateShotScoreDisplay(ShotScoreDisplayState *arg0) {
    ShotScoreDisplayAllocation *allocation;
    s32 i;
    s16 xPos;
    s16 currentVal;

    allocation = (ShotScoreDisplayAllocation *)getCurrentAllocation();
    currentVal = arg0->displayedCount;
    i = ((ShotScoreData *)allocation->gameState)->itemCount - currentVal;

    if (i != 0) {
        xPos = 0x40;
        if (i < 0) {
            arg0->displayedCount = currentVal - 1;
            arg0->animDelayCounter = 0;
        } else {
            if (arg0->animDelayCounter == 0) {
                arg0->displayedCount = currentVal + 1;
                arg0->animDelayCounter = 4;
            } else {
                arg0->animDelayCounter--;
            }
        }
        xPos = 0x40;
    }

    xPos = 0x40;

    for (i = 0; i < 10; i++) {
        if (i >= arg0->displayedCount) {
            arg0->elements[i].spriteFrame = 2;
        } else {
            arg0->elements[i].spriteFrame = 0;
        }

        arg0->elements[i].x = xPos;
        xPos -= 0x10;

        debugEnqueueCallback(8, 0, func_8000FED0_10AD0, &arg0->elements[i]);
    }
}

void cleanupShotScoreDisplayTask(ShotScoreDisplayState *arg0) {
    arg0->elements[0].spriteAsset = freeNodeMemory(arg0->elements[0].spriteAsset);
}

void updateShotCrossScoreDisplay(ShotCrossScoreDisplayState *arg0);
void cleanupShotCrossScoreDisplayTask(ShotCrossScoreDisplayState *arg0);

void initShotCrossScoreDisplayTask(ShotCrossScoreDisplayState *arg0) {
    getCurrentAllocation();
    arg0->spriteAsset = loadAsset_34F9A0();
    arg0->spriteIndex = 2;
    arg0->spriteX = -0x88;
    arg0->spriteY = -0x60;
    arg0->spriteCount = 3;
    arg0->hudX = -0x84;
    arg0->hudY = -0x54;
    arg0->spriteAssetCopy = arg0->spriteAsset;
    arg0->digitAsset = loadCompressedData(&_3F6950_ROM_START, &_3F6950_ROM_END, 0x508);
    setCleanupCallback(cleanupShotCrossScoreDisplayTask);
    setCallback(updateShotCrossScoreDisplay);
}

void updateShotCrossScoreDisplay(ShotCrossScoreDisplayState *arg0) {
    char buf[16];

    sprintf(buf, D_8009E8A8_9F4A8, arg0->player->unkBD3);
    drawNumericString(buf, -0x70, -0x54, 0xFF, arg0->digitAsset, arg0->player->unkBB8 + 8, 0);
    debugEnqueueCallback(8, 0, func_8000FED0_10AD0, arg0);
    debugEnqueueCallback(8, 0, func_8000FED0_10AD0, &arg0->hudX);
}

void cleanupShotCrossScoreDisplayTask(ShotCrossScoreDisplayState *arg0) {
    arg0->spriteAsset = freeNodeMemory(arg0->spriteAsset);
    arg0->digitAsset = freeNodeMemory(arg0->digitAsset);
}

void spawnShotCrossScoreDisplayTask(void *arg0) {
    ShotCrossScoreDisplayState *task;

    task = scheduleTask(initShotCrossScoreDisplayTask, 0, 1, 0xE6);
    if (task != NULL) {
        task->player = arg0;
    }
}

void func_8004ECA4_4F8A4(Struct_func_8004FF28 *arg0);
void func_8004ED94_4F994(Struct_func_8004FF28 *arg0);

void func_8004EC08_4F808(Struct_func_8004FF28 *arg0) {
    GameState *allocation = (GameState *)getCurrentAllocation();

    arg0->unk12 = allocation->unk5A;
    arg0->unk8 = loadAsset_34F9A0();
    arg0->unkC = 1;
    if (arg0->unk14 == 0) {
        arg0->unk4 = 0x68;
        arg0->unk6 = -0x60;
    } else {
        arg0->unk4 = -0x48;
        arg0->unk6 = -0x38;
    }
    arg0->unk10 = 0;
    arg0->unk0 = loadCompressedData(&_3F6950_ROM_START, &_3F6950_ROM_END, 0x508);
    setCleanupCallback(func_8004ED94_4F994);
    setCallback(func_8004ECA4_4F8A4);
}

void func_8004ECA4_4F8A4(Struct_func_8004FF28 *arg0) {
    char buffer[0x10];
    GameState *allocation;

    allocation = getCurrentAllocation();
    debugEnqueueCallback(8, 0, func_8000FED0_10AD0, &arg0->unk4);

    if (arg0->unk12 != allocation->unk5A) {
        arg0->unk10 = 9;
        arg0->unk12 = allocation->unk5A;
    }

    if (arg0->unk10 & 1) {
        sprintf(buffer, D_8009E89C_9F49C, allocation->unk5A);
    } else {
        sprintf(buffer, D_8009E8A0_9F4A0, allocation->unk5A);
    }

    if (arg0->unk10 != 0) {
        arg0->unk10--;
    }

    drawNumericString(buffer, arg0->unk4 + 0x10, arg0->unk6 + 0x10, 0xFF, arg0->unk0, 8, 1);
}

void func_8004ED94_4F994(Struct_func_8004FF28 *arg0) {
    arg0->unk8 = freeNodeMemory(arg0->unk8);
    arg0->unk0 = freeNodeMemory(arg0->unk0);
}

void func_8004EDCC_4F9CC(s16 arg0) {
    Struct_func_8004FF28 *task;

    if (arg0 == 0) {
        task = scheduleTask(func_8004EC08_4F808, 0, 1, 0xE6);
    } else {
        task = scheduleTask(func_8004EC08_4F808, 1, 1, 0xE6);
    }
    if (task != NULL) {
        task->unk14 = arg0;
    }
}

void func_8004F04C_4FC4C(Struct_func_8004F04C *);

void func_8004EE24_4FA24(Struct_func_8004F04C *arg0) {
    GameState *allocation = (GameState *)getCurrentAllocation();

    if (allocation->unk7A == 6) {
        arg0->unk10 = 0xA8C;
    } else {
        arg0->unk10 = 0x1194;
    }
    arg0->unkC = loadCompressedData(&_3F6950_ROM_START, &_3F6950_ROM_END, 0x508);
    arg0->unk4 = loadAsset_34CB50();
    arg0->unk8 = 0x23;
    arg0->unk0 = 0x68;
    arg0->unk2 = 0x48;
    setCleanupCallback(func_8004F04C_4FC4C);
    setCallback(func_8004EEB4_4FAB4);
}

void func_8004EEB4_4FAB4(Struct_func_8004EEB4_4FAB4 *arg0) {
    char buffer[16];
    Allocation *allocation;
    s32 timeValue;
    s32 minutes;
    s32 seconds;
    s32 remainingTicks;
    s32 temp;

    allocation = getCurrentAllocation();

    if (allocation->unk79 == 0 && allocation->unk76 == 0) {
        PlayerInfo *player = allocation->timeRemaining;
        if ((player->unkB84 & 0x80000) == 0) {
            if (arg0->timeRemaining != 0) {
                arg0->timeRemaining--;
                if (arg0->timeRemaining == 0) {
                    allocation->timerExpired = 1;
                }
            }
        }
    }

    // Maths assumes 30 Hz timer
    minutes = arg0->timeRemaining / 1800;
    temp = arg0->timeRemaining - minutes * 1800;
    seconds = temp / 30;
    temp = temp - seconds * 30;
    remainingTicks = temp * 100 / 30;

    if (arg0->timeRemaining < SECONDS_TO_TICKS(30)) {
        sprintf(buffer, sTimerFormatLow, minutes, seconds, remainingTicks);
    } else {
        sprintf(buffer, sTimerFormatNormal, minutes, seconds, remainingTicks);
    }

    debugEnqueueCallback(8, 0, func_8000FED0_10AD0, arg0);

    drawNumericString(buffer, 0x48, 0x50, 0xFF, arg0->unkC, 8, 0);
}

void func_8004F04C_4FC4C(Struct_func_8004F04C *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unkC = freeNodeMemory(arg0->unkC);
}

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    u8 padA[0x2];
    s16 unkC;
} Struct_func_8004F084;

void func_8004F104_4FD04(Struct_func_8004F084 *);
void func_8004F168_4FD68(Struct_func_8004F04C *);

void func_8004F084_4FC84(Struct_func_8004F084 *arg0) {
    if (arg0->unkC == 0) {
        arg0->unk4 = loadAsset_34CB50();
        arg0->unk8 = 0x1E;
        arg0->unk0 = -0x48;
        arg0->unk2 = -0x10;
        arg0->unkC = 1;
        setCleanupCallback(func_8004F168_4FD68);
        setCallback(func_8004F104_4FD04);
    } else {
        arg0->unkC = arg0->unkC - 1;
    }
}

void func_8004F104_4FD04(Struct_func_8004F084 *arg0) {
    if (arg0->unkC == 0) {
        arg0->unkC = 1;
        if ((u16)arg0->unk8 != 0x22) {
            arg0->unk8 = arg0->unk8 + 1;
        }
    } else {
        arg0->unkC = arg0->unkC - 1;
    }
    debugEnqueueCallback(8, 0, func_8000FED0_10AD0, arg0);
}

void func_8004F168_4FD68(Struct_func_8004F04C *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

typedef struct {
    u8 _pad[0xC];
    s16 unkC;
} func_8004F194_task;

void func_8004F194_4FD94(s16 arg0) {
    func_8004F194_task *task = (func_8004F194_task *)scheduleTask(&func_8004F084_4FC84, 1, 1, 0xE6);
    if (task != NULL) {
        task->unkC = arg0;
    }
}

void func_8004F27C_4FE7C(Struct_func_8004F1D4 *);
void func_8004F3EC_4FFEC(Struct_func_8004DCC4 *);

void func_8004F1D4_4FDD4(Struct_func_8004F1D4 *arg0) {
    GameState *allocation = (GameState *)getCurrentAllocation();

    arg0->unk14 = 0;
    arg0->unk4 = loadAsset_34CB50();
    initHudElementState((HudElementState *)arg0);
    arg0->unk8 = 0x24;
    arg0->unk10 = loadCompressedData(&_3F6950_ROM_START, &_3F6950_ROM_END, 0x508);
    if (allocation->unk7A == 4) {
        arg0->unk0 = 0xC;
        arg0->unk2 = -0x3C;
    } else {
        arg0->unk0 = 0xC;
        arg0->unk2 = -0x20;
    }
    setCleanupCallback(func_8004F3EC_4FFEC);
    setCallback(func_8004F27C_4FE7C);
}

typedef struct {
    s16 unk14_hi;
    s16 unk16;
} Struct_func_8004F27C_unk14;

void func_8004F27C_4FE7C(Struct_func_8004F1D4 *arg0) {
    char buf[16];
    s32 var;
    char *ptr;
    GameState *allocation;
    s32 y;
    s32 x;
    s16 z;

    allocation = getCurrentAllocation();

    if (arg0->unk14 != 0xFF) {
        arg0->unk14 = arg0->unk14 + 0x10;
        if (arg0->unk14 >= 0x100) {
            arg0->unk14 = 0xFF;
            scheduleTask(initTotalGoldDisplayTask, 1, 0, 0xE6);
        }
    }

    arg0->unkE = (u8)arg0->unk14;
    debugEnqueueCallback(8, 6, func_80012518_13118, arg0);

    var = 0;
    if (allocation->unk7A == 4) {
        var = 0x1388;
    } else if (allocation->unk7A == 6) {
        if (allocation->players->unkB70 >= 0x12C) {
            var = 0x1388;
        }
    } else if (allocation->unk5A == 0x14) {
        var = 0x2710;
        if (allocation->players->unkBD3 == 0xA) {
            var = 0x4E20;
        }
    }

    sprintf(buf, D_8009E894_9F494, var);

    var = 0;
    ptr = &buf[1];
    if (*ptr != 0) {
        do {
            ptr++;
            var++;
        } while (*ptr != 0);
    }

    y = arg0->unk2;
    x = arg0->unk0;
    z = ((Struct_func_8004F27C_unk14 *)&arg0->unk14)->unk16;
    drawNumericString(buf, (s16)(x + (0x50 - var * 8) / 2), (s16)(y + 0x14), z, arg0->unk10, 8, 6);
}

void func_8004F3EC_4FFEC(Struct_func_8004DCC4 *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk10 = freeNodeMemory(arg0->unk10);
}

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    u8 padA[0x2];
    void *unkC;
    s32 unk10;
    s16 unk14;
} Struct_func_8004F424;

void func_8004F4A8_500A8(Struct_func_8004F424 *arg0);
void func_8004F69C_5029C(Struct_func_8004F04C *arg0);

extern char D_8009E8D4_9F4D4[];
extern char D_8009E8E0_9F4E0[];
extern char D_8009E8EC_9F4EC[];
extern char D_8009E8F8_9F4F8[];

void func_8004F424_50024(Struct_func_8004F424 *arg0) {
    arg0->unk10 = 0x4293C;
    arg0->unkC = loadCompressedData(&_3F6950_ROM_START, &_3F6950_ROM_END, 0x508);
    arg0->unk4 = loadAsset_34CB50();
    arg0->unk8 = 0x23;
    arg0->unk0 = 0x68;
    arg0->unk2 = 0x48;
    arg0->unk14 = 0;
    setCleanupCallback(func_8004F69C_5029C);
    setCallback(func_8004F4A8_500A8);
}

void func_8004F4A8_500A8(Struct_func_8004F424 *arg0) {
    char sp20[0x10];
    Allocation *alloc;
    s32 minutes;
    s32 seconds;

    alloc = (Allocation *)getCurrentAllocation();

    if (alloc->unk79 != 0) {
        goto check_time_flag;
    }
    if (alloc->unk76 != 0) {
        goto check_time_flag;
    }
    if (alloc->timeRemaining->unkB84 & 0x80000) {
        goto set_7E;
    }
    if (arg0->unk10 == 0x433C8) {
        goto check_time_flag;
    }
    arg0->unk10++;
    if (arg0->unk10 != 0x433C8) {
        goto check_time_flag;
    }
    alloc->timerExpired = 1;
    func_80058530_59130(0x46, 6);

check_time_flag:
    if (!(alloc->timeRemaining->unkB84 & 0x80000)) {
        goto after_7E;
    }
set_7E:
    if (arg0->unk10 > 0x4309E) {
        goto after_7E;
    }
    alloc->unk7E = 1;

after_7E:
    alloc->unk54 = arg0->unk10;

    minutes = arg0->unk10 / 32400;
    seconds = (arg0->unk10 % 32400) / 540;

    arg0->unk14++;
    if (arg0->unk14 == 0x28) {
        arg0->unk14 = 0;
    }

    if (arg0->unk10 > 0x431AB) {
        if (arg0->unk14 < 0x14) {
            sprintf(sp20, D_8009E8D4_9F4D4, minutes, seconds);
        } else {
            sprintf(sp20, D_8009E8E0_9F4E0, minutes, seconds);
        }
    } else {
        if (arg0->unk14 < 0x14) {
            sprintf(sp20, D_8009E8EC_9F4EC, minutes, seconds);
        } else {
            sprintf(sp20, D_8009E8F8_9F4F8, minutes, seconds);
        }
    }

    debugEnqueueCallback(8, 0, func_8000FED0_10AD0, arg0);
    drawNumericString(sp20, 0x68, 0x50, 0xFF, arg0->unkC, 8, 0);
}

void func_8004F69C_5029C(Struct_func_8004F04C *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unkC = freeNodeMemory(arg0->unkC);
}

void func_8004F7F4_503F4(Struct_func_8004F04C *arg0);

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    u8 padA[0x2];
    Player *unkC;
    s32 unk10;
} Struct_func_8004F6D4;

void func_8004F760_50360(Struct_func_8004F6D4 *arg0);

typedef struct {
    u8 pad0[0x10];
    void *unk10;
} AllocationStruct_8004F6D4;

void func_8004F6D4_502D4(Struct_func_8004F6D4 *arg0) {
    AllocationStruct_8004F6D4 *alloc = getCurrentAllocation();
    s32 index = arg0->unk10;
    void *base = alloc->unk10;

    arg0->unk0 = -0x10;
    arg0->unk2 = -0x60;
    arg0->unkC = (Player *)((u8 *)base + index * 3048);
    arg0->unk4 = loadCompressedData(&_3F3EF0_ROM_START, &_3F3EF0_ROM_END, 0x2608);
    setCallbackWithContinue(func_8004F760_50360);
    setCleanupCallback(func_8004F7F4_503F4);
}

void func_8004F760_50360(Struct_func_8004F6D4 *arg0) {
    arg0->unk8 = arg0->unkC->unkBD4 + 7;
    debugEnqueueCallback((u16)(arg0->unk10 + 8), 0, func_8000FED0_10AD0, arg0);

    if (arg0->unkC->unkBD8 & 2) {
        func_8005100C_51C0C(arg0->unk0 - 8, arg0->unk2 - 8, 1, arg0->unk10 + 8, 0);
        arg0->unkC->unkBD8 &= ~2;
    }
}

void func_8004F7F4_503F4(Struct_func_8004F04C *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_8004F898_50498(Struct_func_8004F04C *);
void func_8004F9E8_505E8(Struct_func_8004F04C *);

extern char D_8009E904_9F504[];
extern char D_8009E914_9F514[];

typedef struct {
    u8 pad[0x54];
    s32 unk54;
} AllocationStruct_8004F898;

void func_8004F820_50420(Struct_func_8004F04C *arg0) {
    arg0->unkC = loadCompressedData(&_3F6950_ROM_START, &_3F6950_ROM_END, 0x508);
    arg0->unk4 = loadAsset_34CB50();
    arg0->unk8 = 0x23;
    arg0->unk0 = -0x4C;
    arg0->unk2 = -0x30;
    arg0->unk10 = 0;
    setCleanupCallback(func_8004F9E8_505E8);
    setCallback(func_8004F898_50498);
}

void func_8004F898_50498(Struct_func_8004F04C *arg0) {
    char buf[16];
    AllocationStruct_8004F898 *allocation;
    s32 time;
    s32 minutes;
    s32 seconds;
    s32 frames;
    s16 temp;
    char *formatStr;

    allocation = (AllocationStruct_8004F898 *)getCurrentAllocation();
    time = allocation->unk54;
    minutes = time / 32400;
    seconds = (time % 32400) / 540;
    frames = ((time % 32400) % 540) / 9;

    temp = (u16)arg0->unk10 + 1;
    arg0->unk10 = temp;
    if (temp == 0x28) {
        arg0->unk10 = 0;
    }

    if (arg0->unk10 < 0x14) {
        formatStr = D_8009E904_9F504;
        sprintf(buf, formatStr, minutes, seconds, frames);
    } else {
        formatStr = D_8009E914_9F514;
        sprintf(buf, formatStr, minutes, seconds, frames);
    }

    debugEnqueueCallback(8, 0, func_8000FED0_10AD0, arg0);
    drawNumericString(buf, -0x54, -0x28, 0xFF, arg0->unkC, 8, 0);
}

void func_8004F9E8_505E8(Struct_func_8004F04C *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unkC = freeNodeMemory(arg0->unkC);
}

extern char D_8009E924_9F524[];

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    u8 padA[0x2];
    void *unkC;
    char unk10[8];
    s32 unk18;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
} Struct_func_8004FA20;

void func_8004FCB8_508B8(Struct_func_8004F04C *arg0);
void func_8004FB64_50764(Struct_func_8004FA20 *arg0);
void func_8004FAB4_506B4(void *);
void func_8004FC34_50834(Struct_func_8004FA20 *arg0);

void func_8004FA20_50620(Struct_func_8004FA20 *arg0) {
    getCurrentAllocation();
    arg0->unk4 = loadAsset_3505F0();
    arg0->unk2 = -0x20;
    arg0->unk8 = 2;
    arg0->unkC = loadCompressedData(&_3F6950_ROM_START, &_3F6950_ROM_END, 0x508);
    sprintf(arg0->unk10, D_8009E924_9F524, arg0->unk18);
    arg0->unk20 = 0;
    arg0->unk1E = 0x1E;
    setCleanupCallback(func_8004FCB8_508B8);
    setCallback(func_8004FB64_50764);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004FAB4_506B4);

void func_8004FBE8_507E8(Struct_func_8004FA20 *arg0);

void func_8004FB64_50764(Struct_func_8004FA20 *arg0) {
    s32 sinVal;
    s16 angle;

    angle = arg0->unk20 + 0x80;
    arg0->unk20 = angle;
    sinVal = approximateSin(angle);
    arg0->unk1C = (0x2000 - sinVal) / 20;
    if (arg0->unk20 == 0x800) {
        setCallback(func_8004FBE8_507E8);
    }
    func_8004FAB4_506B4(arg0);
}

void func_8004FBE8_507E8(Struct_func_8004FA20 *arg0) {
    arg0->unk1E--;
    if (arg0->unk1E == 0) {
        setCallback(func_8004FC34_50834);
    }
    func_8004FAB4_506B4(arg0);
}

void func_8004FC34_50834(Struct_func_8004FA20 *arg0) {
    s32 sinVal;
    s16 angle;

    angle = arg0->unk20 + 0x80;
    arg0->unk20 = angle;
    sinVal = approximateSin(angle);
    arg0->unk1C = -((0x2000 - sinVal) / 20);
    if (arg0->unk20 == 0x1000) {
        func_80069CF8_6A8F8();
    }
    func_8004FAB4_506B4(arg0);
}

void func_8004FCB8_508B8(Struct_func_8004F04C *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unkC = freeNodeMemory(arg0->unkC);
}

void func_8004FCF0_508F0(s32 arg0) {
    Struct_func_8004FA20 *task = (Struct_func_8004FA20 *)scheduleTask(func_8004FA20_50620, 0, 0, 0xE6);
    if (task != NULL) {
        task->unk18 = arg0;
    }
}

void func_8004FDD0_509D0(Struct_func_8004FF28 *);
void func_8004FF28_50B28(Struct_func_8004FF28 *);

void func_8004FD30_50930(Struct_func_8004FF28 *arg0) {
    GameState *allocation = (GameState *)getCurrentAllocation();

    arg0->unk12 = allocation->players->unkB70;
    arg0->unk8 = loadAsset_3505F0();
    arg0->unkC = 3;
    if (arg0->unk14 == 0) {
        arg0->unk4 = -0x10;
        arg0->unk6 = -0x60;
    } else {
        arg0->unk4 = -0x48;
        arg0->unk6 = -0x30;
    }
    arg0->unk10 = 0;
    arg0->unk0 = loadCompressedData(&_3F6950_ROM_START, &_3F6950_ROM_END, 0x508);
    setCleanupCallback(func_8004FF28_50B28);
    setCallback(func_8004FDD0_509D0);
}

void func_8004FDD0_509D0(Struct_func_8004FF28 *arg0) {
    GameState *allocation;
    s32 strLen;
    char buf[16];
    char *ptr;
    s16 temp;
    s32 x;

    allocation = (GameState *)getCurrentAllocation();
    debugEnqueueCallback(8, 0, func_8000FED0_10AD0, &arg0->unk4);

    if (arg0->unk12 != allocation->players->unkB70) {
        arg0->unk10 = 9;
        arg0->unk12 = allocation->players->unkB70;
    }

    if (arg0->unk10 & 1) {
        sprintf(buf, D_8009E8A8_9F4A8, allocation->players->unkB70);
        strLen = 0;
        ptr = buf;
        if (*ptr != 0) {
            do {
                ptr++;
                strLen++;
            } while (*ptr != 0);
        }
    } else {
        sprintf(buf, D_8009E928_9F528, allocation->players->unkB70);
        strLen = 0;
        ptr = buf;
        if (*ptr != 0) {
            do {
                ptr++;
                strLen++;
            } while (*ptr != 0);
        }
        strLen--;
    }

    temp = arg0->unk10;
    if (temp != 0) {
        arg0->unk10 = temp - 1;
    }

    x = arg0->unk4 + 0x10;
    drawNumericString(buf, x - (strLen << 2), arg0->unk6 + 8, 0xFF, arg0->unk0, 8, 1);
}

void func_8004FF28_50B28(Struct_func_8004FF28 *arg0) {
    arg0->unk8 = freeNodeMemory(arg0->unk8);
    arg0->unk0 = freeNodeMemory(arg0->unk0);
}

void func_8004FF60_50B60(s16 arg0) {
    Struct_func_8004FF28 *task;

    if (arg0 == 0) {
        task = scheduleTask(func_8004FD30_50930, 0, 1, 0xE6);
    } else {
        task = scheduleTask(func_8004FD30_50930, 1, 1, 0xE6);
    }
    if (task != NULL) {
        task->unk14 = arg0;
    }
}

void func_80050098_50C98(Struct_func_8004FFB8 *);
void func_800500F0_50CF0(Struct_func_8004F04C *);

void func_8004FFB8_50BB8(Struct_func_8004FFB8 *arg0) {
    GameState *allocation = (GameState *)getCurrentAllocation();

    switch (allocation->unk7A) {
        case 4:
            arg0->unk4 = loadAsset_350140();
            arg0->unk8 = 0;
            arg0->unk14 = 1;
            break;
        case 5:
            arg0->unk4 = loadAsset_34F9A0();
            arg0->unk8 = 4;
            arg0->unk14 = 5;
            break;
        case 6:
            arg0->unk4 = loadAsset_3505F0();
            arg0->unk8 = 0;
            arg0->unk14 = 1;
            break;
    }
    arg0->unk10 = arg0->unk4;
    arg0->unk0 = -0x88;
    arg0->unk2 = 0x40;
    arg0->unkC = -0x88;
    arg0->unkE = 0x40;
    setCallbackWithContinue(func_80050098_50C98);
    setCleanupCallback(func_800500F0_50CF0);
}

void func_80050098_50C98(Struct_func_8004FFB8 *arg0) {
    debugEnqueueCallback(8, 0, func_8000FED0_10AD0, &arg0->unkC);
    debugEnqueueCallback(8, 0, func_8000FED0_10AD0, arg0);
}

void func_800500F0_50CF0(Struct_func_8004F04C *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E894_9F494);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E89C_9F49C);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E8A0_9F4A0);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E8A8_9F4A8);

const char sTimerFormatLow[] = "\x01%2.2d'%2.2d\"%2.2d";

const char sTimerFormatNormal[] = "\x03%2.2d'%2.2d\"%2.2d";

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E8D4_9F4D4);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E8E0_9F4E0);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E8EC_9F4EC);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E8F8_9F4F8);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E904_9F504);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E914_9F514);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E924_9F524);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E928_9F528);

void func_8005011C_50D1C(void) {
    func_8005011C_50D1C_alloc *allocation;
    s32 i;
    s32 pad[2];
    s32 temp;

    allocation = (func_8005011C_50D1C_alloc *)getCurrentAllocation();

    temp = allocation->unk5F;
    if (temp <= 0) {
        return;
    }

    i = 0;
    do {
        switch (allocation->unk7A) {
            case 0:
            case 8:
            case 9:
            case 10:
                SCHEDULE_AND_SET(initPlayerFinishPositionTask, 4, i);
                SCHEDULE_AND_SET(initPlayerItemDisplayTask, 14, i);
                SCHEDULE_AND_SET(initPlayerLapCounterTask, 18, i);
                SCHEDULE_AND_SET_SHORT(initPlayerGoldDisplayTask, 22, i);
                scheduleTask(func_8004CDC0_4D9C0, 0, 1, 0xE6);
                break;

            case 1:
                SCHEDULE_AND_SET(initPlayerFinishPositionTask, 4, i);
                SCHEDULE_AND_SET(initPlayerItemDisplayTask, 14, i);
                SCHEDULE_AND_SET(initPlayerLapCounterTask, 18, i);
                scheduleTask(func_8004CDC0_4D9C0, 0, 1, 0xE6);
                break;

            case 2:
                SCHEDULE_AND_SET(initPlayerItemDisplayTask, 14, i);
                SCHEDULE_AND_SET(initPlayerLapCounterTask, 18, i);
                SCHEDULE_AND_SET_SHORT(initShotScoreDisplayTask, 60, 0xA);
                scheduleTask(func_8004CDC0_4D9C0, 0, 1, 0xE6);
                break;

            case 3:
                SCHEDULE_AND_SET(initPlayerItemDisplayTask, 14, i);
                SCHEDULE_AND_SET(initPlayerLapCounterTask, 18, i);
                SCHEDULE_AND_SET_SHORT(initShotScoreDisplayTask, 60, 0xB);
                scheduleTask(func_8004CDC0_4D9C0, 0, 1, 0xE6);
                break;

            case 4:
                scheduleTask(func_8004F424_50024, 0, 1, 0xF0);
                SCHEDULE_AND_SET(func_8004F6D4_502D4, 4, i);
                scheduleTask(func_8004FFB8_50BB8, 0, 1, 0xF0);
                break;

            case 5:
                spawnShotCrossScoreDisplayTask(allocation->unk10);
                func_8004EDCC_4F9CC(0);
                scheduleTask(func_8004EE24_4FA24, 0, 1, 0xF0);
                scheduleTask(func_8004FFB8_50BB8, 0, 1, 0xF0);
                break;

            case 6:
                func_8004FF60_50B60(0);
                scheduleTask(func_8004FFB8_50BB8, 0, 1, 0xF0);
                scheduleTask(func_8004EE24_4FA24, 0, 1, 0xF0);
                break;
        }

        i++;
    } while (i < allocation->unk5F);
}