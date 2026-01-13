#include "race_session.h"
#include "2DD40.h"
#include "3CD70.h"
#include "413E0.h"
#include "46080.h"
#include "4CD70.h"
#include "56910.h"
#include "6E840.h"
#include "9FF70.h"
#include "common.h"
#include "controller_io.h"
#include "course.h"
#include "displaylist.h"
#include "gamestate.h"
#include "graphics.h"
#include "levels/lindas_castle.h"
#include "levels/starlight_highway.h"
#include "rand.h"
#include "rom_loader.h"
#include "task_scheduler.h"

#define SET_PLAYER_CAMERA_PERSPECTIVE(gs, idx, aspect_val)                     \
    do {                                                                       \
        fov = 70.0f;                                                           \
        aspect = (aspect_val);                                                 \
        near = 20.0f;                                                          \
        far = 10000.0f;                                                        \
        if ((gs)->memoryPoolId != 0xB) {                                       \
            func_8006FA0C_7060C(&(gs)->unk8[idx], fov, aspect, near, 3000.0f); \
        } else {                                                               \
            func_8006FA0C_7060C(&(gs)->unk8[idx], fov, aspect, near, 2000.0f); \
        }                                                                      \
        func_8006FA0C_7060C(&(gs)->unkC[idx], fov, aspect, near, far);         \
    } while (0)

USE_ASSET(_34CB50);
USE_ASSET(_3FF010);
USE_ASSET(_40E870);
USE_OVERLAY(_9FF70);

typedef struct {
    u8 unk0[0x18];
    ColorData unk18;
    ColorData unk20;
} VarData;

typedef struct {
    Node_70B00 *audioPlayer0;
    Node_70B00 *unk4;
    Node_70B00 *unk8;
    Node_70B00 *unkC;
    u8 padding[0x5C - 0x10];
    u8 memoryPoolId;
    u8 numPlayers;
    u8 padding2[0x7A - 0x5E];
    u8 unk7A;
} GameState_temp;

typedef struct {
    u8 unk0[4];
    u8 gameMode;
    u8 demoIndex;
    u8 unk6;
    u8 currentLevel;
    u8 numPlayers;
    u8 characterIDs[4];
    u8 boardTypes[4];
    u8 costumeIDs[4];
    u8 colorSlots[4];
    u8 lapCount;
    u8 pad1A[5];
    u8 battleTimeLimit;
    u8 battleScoreLimit;
    u8 pad21[3];
    u8 isExpertMode;
} SessionConfig;

typedef struct {
    u8 padding_0[0xBB8];
    u8 racerIndex;
    /* 0xBB9 */ u8 characterID;
    u8 boardType;
    u8 costumeID;
    u8 colorSlot;
    u8 unkBBD;
    u8 padding_BBE[0xBC7 - 0xBBE];
    u8 isCPU;
    u8 padding_BC8[0xBD9 - 0xBC8];
    u8 bossID;
    u8 cpuEnabled;
    u8 padding_BDB[0xBDD - 0xBDB];
    u8 cpuDifficulty;
    u8 padding_BDC[0xBE3 - 0xBDE];
    u8 demoControlType;
    u8 demoControllerIndex;
    u8 padding_BE5[0xBE8 - 0xBE5];
} RacerConfig;

typedef struct {
    void *unk0;
    void *unk4;
    void *unk8;
    void *unkC;
    RacerConfig *racers;
    void *unk14;
    u8 pad18[64];
    u16 unk58;
    u8 unk5A;
    u8 frameDelay;
    /* 0x5C */ u8 currentLevel;
    /* 0x5D */ u8 humanPlayerCount;
    /* 0x5E */ u8 totalRacers;
    u8 activePlayerCount;
    u8 pad60[4];
    u8 racerOrder[16];
    u8 lapCount;
    u8 unk75;
    u8 pad76[3];
    u8 fadeState;
    u8 raceType;
    u8 disableItems;
    u8 pad7C[3];
    u8 demoMode;
    u8 pad80[3];
    u8 battleTimeLimit;
    u8 battleScoreLimit;
    u8 pad85[1];
    u8 isExpertMode;
} RaceState;

typedef struct {
    void *romStart;
    void *romEnd;
    void *ramStart;
    void *icacheStart;
    void *icacheEnd;
    void *dcacheStart;
    void *dcacheEnd;
    void *bssStart;
    void *bssEnd;
} OverlayEntry;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
} Unk800902D0_90ED0;

// Data
extern s32 gThirdPlaceGoldReward[];
extern s32 gFirstPlaceGoldReward[];
extern s32 gSecondPlaceGoldReward[];
extern u8 D_80090450_91050[];
extern Unk800902D0_90ED0 D_800902D0_90ED0[16][6]; // unknown size (16)
extern u8 D_800902C0_90EC0[];
extern u8 D_80090280_90E80[4][4]; // unknown first (4) size
extern OverlayEntry Overlays[];
extern ColorData D_8009077C_9137C;
extern ColorData D_80090774_91374;
extern u8 D_80090520_91120[];

// Bss
extern s8 gControllerPollingEnabled;
extern u8 D_800A24A0_A30A0;
extern s32 gControllerInputs[4];
extern SessionConfig *D_800AFE8C_A71FC;

void initRaceViewports(void);
void func_80040420_41020(void);
void onGameSessionTerminated(void);
void parseRaceAssetData(void);
void scheduleRaceTasks(void);
void awaitRaceAssetsLoaded(void);
void waitForFadeAndInitPlayers(void);
void awaitBossResultAndFadeOut(void);
void awaitMeterWinContinuePress(void);
void awaitPlayersAndPlayRaceMusic(void);
void loadPlayerAssets(void);
void func_8003F1F0_3FDF0(void);
void func_8003F368_3FF68(void);
void handleBossRaceResult(void);
void handleBossDefeatResult(void);
void handleSkillGameResult(void);
void handleShotCrossGameResult(void);
void handleMeterGameResult(void);
void handleSpeedCrossGameResult(void);
void awaitBattleEndAndPromptContinue(void);
void handleExpertRaceResult(void);
void awaitSkillWinContinuePress(void);
void awaitShotCrossWinContinuePress(void);
void awaitSkillWinAndPromptContinue(void);
void awaitSkillLossAndFadeOut(void);
void awaitShotCrossWinAndPromptContinue(void);
void awaitShotCrossLossAndFadeOut(void);
void awaitSpeedCrossAwardGold(void);
void awaitSpeedCrossContinuePress(void);
void awaitMeterWinAndPromptContinue(void);
void awaitMeterLossAndFadeOut(void);
void awaitBattleContinuePress(void);
void awaitExpertRaceContinuePress(void);
void cleanupGameSession(void);
void loadRaceGameData(void);

void initRace(void) {
    RaceState *raceState;
    s32 i;
    s32 unused_mode;
    s32 j;
    s32 unused_offset;
    u8 characterCount[6];
    s32 unused_pad2[2];
    u8 availableCharacters[6];
    s32 unused_pad[2];

    LOAD_OVERLAY(_9FF70);

    func_800698CC_6A4CC(0x37);

    raceState = allocateTaskMemory(sizeof(RaceState));
    for (i = sizeof(RaceState) - 1; i >= 0; i--) {
        ((u8 *)raceState)[i] = 0;
    }

    D_800A24A0_A30A0 = 2;
    raceState->unk75 = 0x7F;
    raceState->fadeState = 2;
    raceState->battleTimeLimit = 0;
    raceState->battleScoreLimit = 0;
    raceState->unk58 = 0x7FFF;

    switch (D_800AFE8C_A71FC->gameMode) {
        case GAME_MODE_STORY:
            raceState->currentLevel = D_800AFE8C_A71FC->currentLevel;
            raceState->lapCount = D_800AFE8C_A71FC->lapCount - 1;
            raceState->isExpertMode = D_800AFE8C_A71FC->isExpertMode;

            switch (raceState->currentLevel) {
                case LINDAS_CASTLE:
                case WENDYS_HOUSE:
                case CRAZY_JUNGLE:
                default:
                    raceState->humanPlayerCount = D_800AFE8C_A71FC->numPlayers;
                    raceState->activePlayerCount = D_800AFE8C_A71FC->numPlayers;
                    raceState->totalRacers = 4;
                    if (raceState->isExpertMode != 0) {
                        raceState->battleTimeLimit = 1;
                    }
                    break;
                case CRAZY_JUNGLE_BOSS:
                    raceState->raceType = RACE_TYPE_BOSS_JUNGLE;
                    raceState->humanPlayerCount = 1;
                    raceState->activePlayerCount = 1;
                    raceState->totalRacers = 2;
                    raceState->lapCount = 0;
                    break;
                case JINGLE_TOWN_BOSS:
                    raceState->raceType = RACE_TYPE_BOSS_JINGLE;
                    raceState->humanPlayerCount = 1;
                    raceState->activePlayerCount = 1;
                    raceState->totalRacers = 2;
                    raceState->lapCount = 0;
                    break;
                case ICE_LAND_BOSS:
                    raceState->raceType = RACE_TYPE_BOSS_ICE;
                    raceState->humanPlayerCount = 1;
                    raceState->activePlayerCount = 1;
                    raceState->totalRacers = 2;
                    raceState->lapCount = 0;
                    break;
                case SNOWBOARD_STREET_SPEED_CROSS:
                    raceState->raceType = RACE_TYPE_SPEED_CROSS;
                    raceState->humanPlayerCount = 1;
                    raceState->activePlayerCount = 1;
                    raceState->totalRacers = 1;
                    raceState->lapCount = 0;
                    break;
                case SNOWBOARD_STREET_SHOT_CROSS:
                    raceState->raceType = RACE_TYPE_SHOT_CROSS;
                    raceState->humanPlayerCount = 1;
                    raceState->activePlayerCount = 1;
                    raceState->totalRacers = 1;
                    raceState->lapCount = 0;
                    break;
                case X_CROSS:
                    raceState->raceType = RACE_TYPE_X_CROSS;
                    raceState->humanPlayerCount = 1;
                    raceState->activePlayerCount = 1;
                    raceState->totalRacers = 1;
                    raceState->lapCount = 0;
                    break;
                case TRAINING:
                    raceState->raceType = RACE_TYPE_TRAINING;
                    raceState->humanPlayerCount = 1;
                    raceState->activePlayerCount = 1;
                    raceState->totalRacers = 2;
                    raceState->lapCount = 1;
                    raceState->disableItems = 1;
                    break;
            }
            break;
        case GAME_MODE_BATTLE:
            raceState->currentLevel = D_800AFE8C_A71FC->currentLevel;
            raceState->lapCount = D_800AFE8C_A71FC->lapCount - 1;
            raceState->raceType = RACE_TYPE_BATTLE;
            raceState->humanPlayerCount = D_800AFE8C_A71FC->numPlayers;
            raceState->activePlayerCount = D_800AFE8C_A71FC->numPlayers;
            raceState->totalRacers = 4;
            raceState->disableItems = 1;
            raceState->battleTimeLimit = D_800AFE8C_A71FC->battleTimeLimit;
            raceState->battleScoreLimit = D_800AFE8C_A71FC->battleScoreLimit;
            break;

        case GAME_MODE_DEMO:
            initRand();
            raceState->raceType = RACE_TYPE_DEMO;
            raceState->demoMode = D_800AFE8C_A71FC->demoIndex;

            switch (raceState->demoMode) {
                case 0:
                    raceState->humanPlayerCount = 1;
                    raceState->activePlayerCount = 1;
                    raceState->currentLevel = SUNNY_MOUNTAIN;
                    raceState->lapCount = 2;
                    raceState->totalRacers = 4;
                    break;
                case 1:
                    raceState->currentLevel = TURTLE_ISLAND;
                    raceState->lapCount = 2;
                    raceState->humanPlayerCount = 2;
                    raceState->activePlayerCount = 2;
                    raceState->totalRacers = 4;
                    break;
                case 2:
                    raceState->currentLevel = LINDAS_CASTLE;
                    raceState->lapCount = 2;
                    raceState->humanPlayerCount = 4;
                    raceState->activePlayerCount = 4;
                    raceState->totalRacers = 4;
                    break;
            }
            break;
        case GAME_MODE_INTRO:
            initRand();
            raceState->demoMode = 0;
            raceState->raceType = RACE_TYPE_INTRO;
            raceState->currentLevel = JINGLE_TOWN;
            raceState->lapCount = 2;
            raceState->activePlayerCount = 4;
            raceState->totalRacers = 4;

            j = raceState->demoMode;
            switch (raceState->demoMode) {
                case 0:
                case 1:
                    raceState->humanPlayerCount = 1;
                    raceState->unk14 = loadCompressedData(&_40E870_ROM_START, &_40E870_ROM_END, 0xB8E0);
                    break;
            }
            break;
    }

    if (Overlays[raceState->currentLevel].romStart != NULL) {
        dmaLoadAndInvalidate(
            Overlays[raceState->currentLevel].romStart,
            Overlays[raceState->currentLevel].romEnd,
            Overlays[raceState->currentLevel].ramStart,
            Overlays[raceState->currentLevel].icacheStart,
            Overlays[raceState->currentLevel].icacheEnd,
            Overlays[raceState->currentLevel].dcacheStart,
            Overlays[raceState->currentLevel].dcacheEnd,
            Overlays[raceState->currentLevel].bssStart,
            Overlays[raceState->currentLevel].bssEnd
        );
    }

    raceState->unk0 = allocateNodeMemory(0x1D8);
    raceState->unk4 = allocateNodeMemory(
        ((raceState->humanPlayerCount * 16 - raceState->humanPlayerCount) * 4 - raceState->humanPlayerCount) * 8
    );
    raceState->unk8 = allocateNodeMemory(
        ((raceState->humanPlayerCount * 16 - raceState->humanPlayerCount) * 4 - raceState->humanPlayerCount) * 8
    );
    raceState->unkC = allocateNodeMemory(
        ((raceState->humanPlayerCount * 16 - raceState->humanPlayerCount) * 4 - raceState->humanPlayerCount) * 8
    );
    raceState->racers = allocateNodeMemory(
        ((raceState->totalRacers * 2 + raceState->totalRacers) * 128 -
         (raceState->totalRacers * 2 + raceState->totalRacers)) *
        8
    );

    for (i = 0; i < raceState->totalRacers; i++) {
        u8 *racerBytes = (u8 *)&raceState->racers[i];
        for (j = sizeof(RacerConfig) - 1; j >= 0; j--) {
            racerBytes[j] = 0;
        }

        raceState->racers[i].racerIndex = i;
        raceState->racerOrder[i] = i;
    }

    for (i = 11; i >= 0; i--) {
        characterCount[i] = 0;
    }

    if (raceState->raceType < RACE_TYPE_TRAINING) {
        for (i = 0; i < raceState->activePlayerCount; i++) {
            raceState->racers[i].characterID = D_800AFE8C_A71FC->characterIDs[i];
            raceState->racers[i].boardType = D_800AFE8C_A71FC->colorSlots[i];
            raceState->racers[i].costumeID = D_800AFE8C_A71FC->boardTypes[i];
            raceState->racers[i].colorSlot = D_800AFE8C_A71FC->costumeIDs[i];
        }
    }

    for (i = raceState->activePlayerCount; i < raceState->totalRacers; i++) {
        raceState->racers[i].characterID = D_80090280_90E80[raceState->currentLevel][i];
        raceState->racers[i].boardType = D_800902C0_90EC0[raceState->currentLevel];
        raceState->racers[i].isCPU = 1;
        raceState->racers[i].cpuEnabled = 1;
    }

    switch (D_800AFE8C_A71FC->gameMode) {
        case GAME_MODE_STORY:
            switch (raceState->currentLevel) {
                case CRAZY_JUNGLE_BOSS:
                    raceState->racers[1].bossID = 1;
                    raceState->racers[1].characterID = 9;
                    break;
                case JINGLE_TOWN_BOSS:
                    raceState->racers[1].bossID = 2;
                    raceState->racers[1].characterID = 10;
                    break;
                case ICE_LAND_BOSS:
                    raceState->racers[1].bossID = 3;
                    raceState->racers[1].characterID = 11;
                    break;
                case TRAINING:
                    raceState->racers[0].characterID = 0;
                    raceState->racers[0].boardType = 0;
                    raceState->racers[0].costumeID = 0;
                    raceState->racers[0].colorSlot = 0;
                    break;
            }
            break;
        case GAME_MODE_DEMO:
            switch (raceState->demoMode) {
                case 0:
                    raceState->racers[0].characterID = 0;
                    raceState->racers[0].boardType = 0;
                    raceState->racers[0].costumeID = 0;
                    raceState->racers[0].colorSlot = 0;
                    raceState->racers[0].demoControlType = 1;
                    raceState->racers[0].demoControllerIndex = 0;
                    break;
                case 1:
                    for (i = 0; i < 2; i++) {
                        raceState->racers[i].characterID = i;
                        raceState->racers[i].boardType = 1;
                        raceState->racers[i].costumeID = 0;
                        raceState->racers[i].colorSlot = i;
                        raceState->racers[i].demoControlType = 1;
                        raceState->racers[i].demoControllerIndex = i + 1;
                    }
                    break;
                case 2:
                    for (i = 0; i < 4; i++) {
                        raceState->racers[i].characterID = i;
                        raceState->racers[i].boardType = 1;
                        raceState->racers[i].costumeID = 0;
                        raceState->racers[i].colorSlot = i;
                        raceState->racers[i].demoControlType = 1;
                        raceState->racers[i].demoControllerIndex = i + 3;
                    }
            }
            break;
        case GAME_MODE_INTRO:
            raceState->racers[0].characterID = 0;
            raceState->racers[1].characterID = 3;
            raceState->racers[2].characterID = 5;
            raceState->racers[3].characterID = 2;

            for (i = 0; i < 4; i++) {
                raceState->racers[i].boardType = 0;
                raceState->racers[i].costumeID = 0;
                raceState->racers[i].colorSlot = i;
                raceState->racers[i].demoControllerIndex = i + 7;
                if (raceState->demoMode == 3) {
                    raceState->racers[i].demoControlType = 2;
                } else {
                    raceState->racers[i].demoControlType = 1;
                }
            }
            break;
        case GAME_MODE_BATTLE:
            break;
    }

    for (i = 0; i < raceState->totalRacers; i++) {
        characterCount[raceState->racers[i].characterID]++;
    }

    for (i = raceState->activePlayerCount; i < raceState->totalRacers; i++) {
        s32 j;
        u8 count;

        if (characterCount[raceState->racers[i].characterID] >= 2) {
            characterCount[raceState->racers[i].characterID]--;
            count = 0;
            for (j = 0; j < 6; j++) {
                if (characterCount[j] == 0) {
                    availableCharacters[count] = j;
                    count++;
                }
            }

            raceState->racers[i].characterID = availableCharacters[randA() % count];
            characterCount[raceState->racers[i].characterID]++;
        }
    }

    for (i = raceState->activePlayerCount; i < raceState->totalRacers; i++) {
        if (raceState->racers[i].characterID < 6) {
            raceState->racers[i].costumeID =
                D_800902D0_90ED0[raceState->currentLevel][raceState->racers[i].characterID].unk0;
            raceState->racers[i].colorSlot =
                D_800902D0_90ED0[raceState->currentLevel][raceState->racers[i].characterID].unk1;

            if (raceState->activePlayerCount == 1) {
                raceState->racers[i].cpuDifficulty =
                    D_800902D0_90ED0[raceState->currentLevel][raceState->racers[i].characterID].unk2;
            } else {
                raceState->racers[i].cpuDifficulty =
                    D_800902D0_90ED0[raceState->currentLevel][raceState->racers[i].characterID].unk3;
            }

            if (raceState->isExpertMode != 0) {
                raceState->racers[i].costumeID = 2;
                if (i == 1) {
                    raceState->racers[1].costumeID = D_80090450_91050[raceState->currentLevel];
                }
            }
        } else {
            raceState->racers[i].costumeID = 13;
            raceState->racers[i].colorSlot = 5;
            raceState->racers[i].cpuDifficulty = 7;

            if (raceState->raceType == RACE_TYPE_BATTLE) {
                raceState->racers[i].cpuDifficulty = 5;
            }

            if (raceState->racers[i].characterID != 6) {
                raceState->racers[i].costumeID = 9;
                if (raceState->raceType == RACE_TYPE_BATTLE) {
                    raceState->racers[i].cpuDifficulty = 5;
                } else {
                    raceState->racers[i].cpuDifficulty = 0;
                    raceState->racers[i].costumeID = 1;
                }
            }
        }

        if (raceState->isExpertMode != 0) {
            raceState->racers[i].cpuDifficulty = 7;
        }
    }

    if (raceState->raceType == RACE_TYPE_BATTLE) {
        u8 maxCostumeLevel = 0;
        for (i = 0; i < raceState->activePlayerCount; i++) {
            if (maxCostumeLevel < D_80090520_91120[raceState->racers[i].costumeID]) {
                maxCostumeLevel = D_80090520_91120[raceState->racers[i].costumeID];
            }
        }

        for (i = raceState->activePlayerCount; i < raceState->totalRacers; i++) {
            if (raceState->racers[i].costumeID >= 9) {
                if (maxCostumeLevel == 4) {
                    raceState->racers[i].costumeID = 16;
                }
                if (maxCostumeLevel == 5) {
                    raceState->racers[i].costumeID = 17;
                }
                continue;
            }

            if (maxCostumeLevel <= 2) {
                raceState->racers[i].costumeID = maxCostumeLevel + (raceState->racers[i].costumeID / 3) * 3;
                continue;
            }

            if (maxCostumeLevel == 3) {
                raceState->racers[i].costumeID = 9;
            }
            if (maxCostumeLevel == 4) {
                raceState->racers[i].costumeID = 16;
            }
            if (maxCostumeLevel == 5) {
                raceState->racers[i].costumeID = 17;
            }
        }
    }

    raceState->frameDelay = 0x28;
    switch (raceState->raceType) {
        default:
            setupTaskSchedulerNodes(0x50, 0x1E, 0x50, 0x28, 0xA, 0xA, 0xA, 0xA);
            break;
        case 2:
            setupTaskSchedulerNodes(0x50, 0x1E, 0x50, 0x28, 0xA, 0x1E, 0, 0);
            break;
        case 1:
        case 3:
            setupTaskSchedulerNodes(0x50, 0x1E, 0x50, 0x3C, 0xA, 0x14, 0, 0);
            raceState->frameDelay = 0x3C;
            break;
        case 5:
            setupTaskSchedulerNodes(0x50, 0x1E, 0x50, 0x28, 0x1E, 0, 0, 0);
            break;
    }

    gControllerPollingEnabled = 1;
    func_8006FDA0_709A0(0, 0xFF, 0);
    func_8006983C_6A43C((void *)initRaceViewports);
}

void initRaceViewports(void) {
    GameState_temp *gs;
    VarData *levelConfig;
    s32 i;
    f32 new_var;
    s32 j;
    s32 playerIdx;
    f32 fov;
    f32 aspect;
    f32 near;
    f32 far;

    gs = (GameState_temp *)getCurrentAllocation();
    levelConfig = (VarData *)func_80055D10_56910(gs->memoryPoolId);
    setAudioDistanceLimits(0x60, 0x1400);
    func_8006FAA4_706A4(gs->audioPlayer0, 0, 0xC, 0x1E, 0);
    setModelCameraTransform(gs->audioPlayer0, 0, 0, -0xA0, -0x78, 0xA0, 0x78);

    for (i = 0; i < gs->numPlayers; i++) {
        func_8006FAA4_706A4(&gs->unkC[i], 0, (u16)(i + 4), 5, 1);
        func_8006FAA4_706A4(&gs->unk8[i], &gs->unkC[i], (u16)i, 0xA, 1);
        func_8006FAA4_706A4(&gs->unk4[i], &gs->unk8[i], (u16)(i + 8), 0x14, 0);
        func_8006FEF8_70AF8(&gs->unk8[i], (u16)(i + 0x64));
        func_8006FEF8_70AF8(&gs->unkC[i], (u16)(i + 0x64));

        if (gs->unk7A == 0xB) {
            func_8006FC70_70870(i + 0x64, 1, &D_80090774_91374, &D_8009077C_9137C);
        } else {
            func_8006FC70_70870(i + 0x64, 1, &levelConfig->unk18, &levelConfig->unk20);
        }

        if (gs->memoryPoolId != 0xB) {
            func_8006FE48_70A48(
                i + 0x64,
                0x3E3,
                0x3E7,
                levelConfig->unk20.r2,
                levelConfig->unk20.g2,
                levelConfig->unk20.b2
            );
        } else {
            func_8006FE48_70A48(
                i + 0x64,
                0x384,
                0x3E7,
                levelConfig->unk20.r2,
                levelConfig->unk20.g2,
                levelConfig->unk20.b2
            );
        }
    }

    switch (gs->numPlayers) {
        case 1:
            setModelCameraTransform(gs->unkC, 0, 0, -0xA0, -0x78, 0xA0, 0x78);
            setModelCameraTransform(gs->unk8, 0, 0, -0xA0, -0x78, 0xA0, 0x78);
            setModelCameraTransform(gs->unk4, 0, 0, -0xA0, -0x78, 0xA0, 0x78);

            if (gs->memoryPoolId != 0xB) {
                near = 70.0f;
                aspect = 4.0f / 3.0f;
                fov = 20.0f;
                func_8006FA0C_7060C(gs->unk8, near, aspect, fov, 3800.0f);
            } else {
                near = 70.0f;
                aspect = 4.0f / 3.0f;
                fov = 20.0f;
                func_8006FA0C_7060C(gs->unk8, near, aspect, fov, 2000.0f);
            }
            func_8006FA0C_7060C(gs->unkC, near, aspect, fov, 10000.0f);
            break;

        case 2:
            osViExtendVStart(1);

            setModelCameraTransform(&gs->unkC[0], 0, -0x35, -0xA0, -0x34, 0xA0, 0x34);
            setModelCameraTransform(&gs->unkC[1], 0, 0x35, -0xA0, -0x34, 0xA0, 0x34);

            for (playerIdx = 0, i = 0; i < 2; i++, playerIdx++) {
                setModelCameraTransform(&gs->unk8[playerIdx], 0, 0, -0xA0, -0x34, 0xA0, 0x34);
                setModelCameraTransform(&gs->unk4[playerIdx], 0, 0, -0xA0, -0x34, 0xA0, 0x34);
                func_8006F9BC_705BC(&gs->unk8[playerIdx], 1.0f, 0.5f);
                func_8006F9BC_705BC(&gs->unkC[playerIdx], 1.0f, 0.5f);

                // SET_PLAYER_CAMERA_PERSPECTIVE should probably work here but it doesn't because
                // the copy of far to new_var is essential here but breaks the 3 and 4 cases.
                do {
                    fov = 70.0f;
                    aspect = (4.0f / 3.0f) * 2.0f;
                    near = 20.0f;
                    far = 10000.0f;
                    new_var = far;
                    if (gs->memoryPoolId != 0xB) {
                        func_8006FA0C_7060C(&gs->unk8[playerIdx], fov, aspect, near, 3000.0f);
                    } else {
                        func_8006FA0C_7060C(&gs->unk8[playerIdx], fov, aspect, near, 2000.0f);
                    }
                    func_8006FA0C_7060C(&gs->unkC[playerIdx], fov, aspect, near, new_var);
                } while (0);
            }

            break;

        case 3:
            osViExtendVStart(1);

            setModelCameraTransform(&gs->unkC[0], -0x49, -0x35, -0x48, -0x34, 0x48, 0x34);
            setModelCameraTransform(&gs->unkC[1], -0x49, 0x35, -0x48, -0x34, 0x48, 0x34);
            setModelCameraTransform(&gs->unkC[2], 0x49, -0x35, -0x48, -0x34, 0x48, 0x34);

            for (playerIdx = 0, i = 0; i < 3; i++, playerIdx++) {
                setModelCameraTransform(&gs->unk8[playerIdx], 0, 0, -0x48, -0x34, 0x48, 0x34);
                new_var = 0.5f;
                setModelCameraTransform(&gs->unk4[playerIdx], 0, 0, -0x48, -0x34, 0x48, 0x34);
                func_8006F9BC_705BC(&gs->unk8[playerIdx], 0.5f, 0.5f);
                func_8006F9BC_705BC(&gs->unkC[playerIdx], 0.5f, new_var);

                SET_PLAYER_CAMERA_PERSPECTIVE(gs, playerIdx, (4.0f / 3.0f));
            }

            break;

        case 4:
            osViExtendVStart(1);

            setModelCameraTransform(&gs->unkC[0], -0x49, -0x35, -0x48, -0x34, 0x48, 0x34);
            setModelCameraTransform(&gs->unkC[1], -0x49, 0x35, -0x48, -0x34, 0x48, 0x34);
            setModelCameraTransform(&gs->unkC[2], 0x49, -0x35, -0x48, -0x34, 0x48, 0x34);
            setModelCameraTransform(&gs->unkC[3], 0x49, 0x35, -0x48, -0x34, 0x48, 0x34);

            for (playerIdx = 0, i = 0; i < 4; i++, playerIdx++) {
                setModelCameraTransform(&gs->unk8[playerIdx], 0, 0, -0x48, -0x34, 0x48, 0x34);
                setModelCameraTransform(&gs->unk4[playerIdx], 0, 0, -0x48, -0x34, 0x48, 0x34);
                func_8006F9BC_705BC(&gs->unk8[playerIdx], 0.5f, 0.5f);
                func_8006F9BC_705BC(&gs->unkC[playerIdx], 0.5f, 0.5f);

                SET_PLAYER_CAMERA_PERSPECTIVE(gs, playerIdx, (4.0f / 3.0f));
            }

            break;
    }

    func_8006983C_6A43C(&loadRaceGameData);
}

void loadRaceGameData(void) {
    GameState *gameState;

    gameState = (GameState *)getCurrentAllocation();
    gameState->gameData.dataStart = func_80055D34_56934(gameState->memoryPoolId);
    gameState->unk28 = loadCompressedData(&_3FF010_ROM_START, &_3FF010_ROM_END, 0x16E0);
    func_8006983C_6A43C(&parseRaceAssetData);
}

void parseRaceAssetData(void) {
    GameState *gs = (GameState *)getCurrentAllocation();

    parseGameDataLayout(&gs->gameData);

    gs->unk44 = (GameStateUnk44 *)((u8 *)gs->unk28 + gs->unk28->assetTableOffset);
    gs->unk48 = (u8 *)gs->unk28 + gs->unk28->transformDataOffset;

    func_8006983C_6A43C(&loadPlayerAssets);
}

void loadPlayerAssets(void) {
    GameState *gs = (GameState *)getCurrentAllocation();
    s32 i = 0;
    s32 offset;
    int new_var;

    new_var = 0;
    if (gs->numPlayers <= new_var) {
    } else {
        offset = 0;
        do {
            func_800B99E0((void *)(((u8 *)gs->players) + offset));
            i++;
            offset += 0xBE8;
        } while (i < gs->numPlayers);
    }

    gs->unk18 = loadCompressedData(&_34CB50_ROM_START, &_34CB50_ROM_END, 0x5E28);
    gs->unk1C = loadAsset_B7E70();
    gs->unk20 = loadAsset_216290();

    func_8006983C_6A43C(&scheduleRaceTasks);
}

void scheduleRaceTasks(void) {
    GameState *gameState;

    gameState = (GameState *)getCurrentAllocation();
    gameState->pendingPlayerRenderTasks = 0;

    scheduleTask(&func_800B0334_A01E4, 0, 0, 1);
    scheduleTask(&func_800B7B7C_A7A2C, 0, 0, 0x64);
    scheduleTask(&initPauseMenuDisplayTask, 0, 0, 0xC8);

    scheduleLevelEnvironmentTasks(gameState->memoryPoolId);
    scheduleCourseTasks(gameState->memoryPoolId, gameState->unk5F);
    func_8005011C_50D1C();

    if (gameState->unk7A == 9) {
        scheduleTask(&initStarlightHighwayBuildingTask, 0, 0, 0xC8);
    }

    if (gameState->unk7A >= 0xA) {
        spawnPushStartPrompt(0, 0x28, 0, 0, 0xC, 6);
    }

    setGameStateHandler(&awaitRaceAssetsLoaded);
}

void awaitRaceAssetsLoaded(void) {
    GameState *state;

    state = getCurrentAllocation();

    if (getPendingDmaCount() == 0) {
        if (state->unk7A < 9) {
            gControllerPollingEnabled = 0;
        }

        if ((state->unk5F == 1) && (state->unk7A < 10)) {
            spawnOrbitCameraTask();
            state->unk79 = 2;
            playMusicTrack(7);
        } else {
            state->unk79 = 1;
        }

        state->unk4C = 0x10;

        if (state->unk7A != 0xB) {
            func_8006FDA0_709A0(0, 0, 0x10);
        } else {
            state->unk4C = 0;
        }

        func_8006FEBC_70ABC(0);
        setGameStateHandler(waitForFadeAndInitPlayers);
    }
}

void waitForFadeAndInitPlayers(void) {
    s32 fadeDelay;
    s32 i;
    GameState *state;

    state = (GameState *)getCurrentAllocation();
    fadeDelay = state->unk4C;
    if (state->unk4C == 0) {
        if (state->unk79 == 1) {
            state->unk79 = 0;

            if (state->unk7A < 0xA) {
                for (i = 0; i < state->unk5F; i++) {
                    schedulePlayerHaloTask(&state->players[i]);
                    state->unk79++;
                }
            }

            if (state->unk7A == 0xB) {
                state->unk79 = 1;
            }

            setGameStateHandler(&awaitPlayersAndPlayRaceMusic);
        }
    } else {
        state->unk4C = fadeDelay - 1;
    }
}

void awaitPlayersAndPlayRaceMusic(void) {
    GameState *state = (GameState *)getCurrentAllocation();

    if ((state->unk79 == 0) || (state->unk7A == 0xB)) {
        D_80090F90_91B90_item *levelConfig = func_80055D10_56910(state->memoryPoolId);

        if (state->unk7A == 0xB) {
            playMusicTrack(0x20);
        } else {
            playMusicTrack(levelConfig->unk28);
        }

        setGameStateHandler(func_8003F368_3FF68);
    }
}

INCLUDE_ASM("asm/nonmatchings/race_session", func_8003F1F0_3FDF0);

typedef struct {
    u16 frame;
    u16 unk2;
} IntroFrameData;

typedef struct {
    u16 frame;
    s16 value;
} IntroFrameData2;

extern IntroFrameData D_80090780_91380[];
extern IntroFrameData2 D_8009079C_9139C[];
extern void spawnScriptedCameraTask(s16);

void func_8003F368_3FF68(void) {
    GameState *gs;
    s32 i;
    s32 inputMask;
    s32 aPressed;
    void (*handler)(void);
    s32 count;
    s16 temp;
    s32 unk7FTemp;
    s32 playerFlags;
    volatile u8 padding[0x1C];

    gs = (GameState *)getCurrentAllocation();

    if (gs->unk7A == 0xA)
        goto handleA;
    if (gs->unk7A == 0xB)
        goto handleB;

    inputMask = 0;
    for (i = 0; i < gs->unk5F; i++) {
        inputMask |= gControllerInputs[i];
    }

    if (gs->gamePaused != 0) {
        switch (gs->gamePaused) {
            case 1:
                if (inputMask & 0x20400) {
                    if ((gs->pauseMenuSelection & 0xFF) != 2) {
                        gs->pauseMenuSelection++;
                    } else {
                        gs->pauseMenuSelection = 0;
                    }
                    func_800585C8_591C8(0x2B);
                }
                if (inputMask & 0x10800) {
                    if (gs->pauseMenuSelection == 0) {
                        gs->pauseMenuSelection = 2;
                    } else {
                        gs->pauseMenuSelection--;
                    }
                    func_800585C8_591C8(0x2B);
                }
                aPressed = inputMask & 0x1000;
                if (aPressed) {
                    switch (gs->pauseMenuSelection) {
                        case 0:
                            gs->gamePaused = 0;
                            break;
                        case 1:
                            D_800A24A0_A30A0 = 1;
                        case 2:
                            func_8006FDA0_709A0(NULL, 0xFF, 0x10);
                            setMusicFadeOut(0x3C);
                            setGameStateHandler(cleanupGameSession);
                            func_800585C8_591C8(0x2E);
                            return;
                        case 3:
                            gs->gamePaused = 3;
                            break;
                        case 4:
                            gs->gamePaused = 4;
                            break;
                        case 5:
                            gs->gamePaused = 5;
                            break;
                        case 6:
                            gs->gamePaused = 6;
                            break;
                        case 7:
                            gs->gamePaused = 7;
                            break;
                        case 8:
                            gs->gamePaused = 8;
                            break;
                        case 9:
                            gs->gamePaused = 9;
                            break;
                        case 10:
                            gs->gamePaused = 10;
                            break;
                        case 11:
                            gs->gamePaused = 11;
                            break;
                    }
                }
                break;
            default:
                if (gControllerInputs[0] & 0x1000) {
                    gs->gamePaused = 0;
                }
                break;
            case 12:
                if (gs->unk78 == -1) {
                    gs->gamePaused = 0;
                    gs->unk78 = 0;
                }
                break;
        }
    } else {
        gs->unk50++;
        switch (gs->unk7A) {
            case 0:
                count = 0;
                for (i = 0; i < gs->unk5F; i++) {
                    playerFlags = gs->players[i].unkB84 & 0x80000;
                    count += playerFlags != 0;
                }
                if (gs->unk5F == count) {
                    gs->unk4C = 0x3C;
                    handler = handleSpeedCrossGameResult;
                    setGameStateHandler(handler);
                    return;
                }
                break;
            case 1:
                if (gs->players->unkB84 & 0x80000) {
                    setMusicFadeOut(0x3C);
                    gs->unk4C = 0x1E;
                    handler = handleBossRaceResult;
                    setGameStateHandler(handler);
                    return;
                }
                break;
            case 2:
            case 3:
                if (gs->players->unkB84 & 0x80000) {
                    setMusicFadeOut(0x3C);
                    gs->unk4C = 0x1E;
                    handler = handleBossDefeatResult;
                    setGameStateHandler(handler);
                    return;
                }
                break;
            case 8:
                count = 0;
                for (i = 0; i < gs->unk5F; i++) {
                    playerFlags = gs->players[i].unkB84 & 0x80000;
                    count += playerFlags != 0;
                }
                if (gs->unk5F == count) {
                    gs->unk4C = 0x1E;
                    setMusicFadeOut(0x3C);
                    handler = awaitBattleEndAndPromptContinue;
                    setGameStateHandler(handler);
                    return;
                }
                break;
            case 9:
                count = 0;
                for (i = 0; i < gs->unk5F; i++) {
                    playerFlags = gs->players[i].unkB84 & 0x80000;
                    count += playerFlags != 0;
                }
                if (gs->unk5F == count) {
                    gs->unk4C = 0x1E;
                    setMusicFadeOut(0x3C);
                    handler = handleExpertRaceResult;
                    setGameStateHandler(handler);
                    return;
                }
                break;
            case 4:
                if (gs->players->unkB84 & 0x80000) {
                    if (gs->unk7D != 0) {
                        gs->unk7B = 1;
                    }
                    gs->unk4C = 0x3C;
                    setMusicFadeOut(0x3C);
                    handler = handleSkillGameResult;
                    setGameStateHandler(handler);
                    return;
                }
                break;
            case 5:
                if (gs->players->unkB84 & 0x80000) {
                    if (gs->unk7D != 0) {
                        gs->unk7B = 1;
                    }
                    gs->unk4C = 0x3C;
                    setMusicFadeOut(0x3C);
                    handler = handleShotCrossGameResult;
                    setGameStateHandler(handler);
                    return;
                }
                break;
            case 6:
                if (gs->players->unkB84 & 0x80000) {
                    if (gs->unk7D != 0) {
                        gs->unk7B = 1;
                    }
                    gs->unk4C = 0x3C;
                    setMusicFadeOut(0x3C);
                    handler = handleMeterGameResult;
                    setGameStateHandler(handler);
                    return;
                }
                break;
        }
        aPressed = inputMask & 0x1000;
        if (aPressed) {
            gs->pauseMenuSelection = 0;
            gs->gamePaused = (gs->gamePaused == 0);
            func_800585C8_591C8(0x2B);
            resumeMotorStates();
        }
        if (gs->unk78 == 1) {
            gs->gamePaused = 0xC;
            gs->pauseMenuSelection = 0;
        }
    }
    return;

handleA:
    gs->unk50++;
    if (gs->unk50 >= 0x349) {
        D_800A24A0_A30A0 = 1;
        func_8006FDA0_709A0(NULL, 0xFF, 0x10);
        setMusicFadeOut(0x3C);
        setGameStateHandler(cleanupGameSession);
        return;
    }
    if (gControllerInputs[0] & 0x1000) {
        D_800A24A0_A30A0 = 2;
        func_8006FDA0_709A0(NULL, 0xFF, 0x10);
        setMusicFadeOut(0x3C);
        setGameStateHandler(cleanupGameSession);
    }
    return;

handleB:
    if (gs->gamePaused == 0) {
        if (gs->unk50 == 0) {
            func_8006FDA0_709A0(NULL, 0, 8);
        }
        gs->unk50++;
    }
    unk7FTemp = gs->unk7F;
    gs->unk79 = 0;
    if (unk7FTemp < 0) {
        return;
    }
    if (unk7FTemp >= 2) {
        return;
    }
    if (D_80090780_91380[gs->unk82].frame == gs->unk50) {
        func_8003F1F0_3FDF0();
        gs->unk82++;
    }
    if (D_8009079C_9139C[gs->unk85].frame == gs->unk50) {
        terminateTasksByTypeAndID(0, 2);
        temp = D_8009079C_9139C[gs->unk85].value;
        if (temp >= 0) {
            spawnScriptedCameraTask(temp);
        }
        gs->unk85++;
    }
    switch (gs->unk61) {
        case 0:
            if (gs->unk50 >= 0x39E) {
                D_800A24A0_A30A0 = 2;
                func_8006FE28_70A28(NULL, 0xFF, 0xFF, 0xFF);
                func_8006FDA0_709A0(NULL, 0xFF, 0x10);
                setMusicFadeOut(0x20);
                setGameStateHandler(cleanupGameSession);
                return;
            }
            if (gControllerInputs[0] & 0x1000) {
                D_800A24A0_A30A0 = 2;
                func_8006FE28_70A28(NULL, 0, 0, 0);
                func_8006FDA0_709A0(NULL, 0xFF, 0x10);
                setMusicFadeOut(0x20);
                gs->unk61++;
            }
            break;
        case 1:
            if (func_8006FE10_70A10(NULL) == 0) {
                setGameStateHandler(cleanupGameSession);
            }
            break;
    }
}

void handleBossRaceResult(void) {
    GameState *gs;
    Player *player;

    gs = (GameState *)getCurrentAllocation();
    gs->unk4C--;

    if (gs->unk4C != 0) {
        return;
    }

    player = gs->players;

    if (player->finishPosition == 0) {
        D_800A24A0_A30A0 = 3;
        playMusicTrack(8);
    } else {
        D_800A24A0_A30A0 = 4;
        playMusicTrack(9);
    }

    gs->unk4C = 0x96;
    gs->unk7B = 1;

    setGameStateHandler(awaitBossResultAndFadeOut);
}

void handleBossDefeatResult(void) {
    GameState *state = (GameState *)getCurrentAllocation();

    state->unk4C--;
    if (state->unk4C != 0) {
        return;
    }

    if (state->players[1].unkB84 & 0x100000) {
        D_800A24A0_A30A0 = 3;
        playMusicTrack(8);
    } else {
        D_800A24A0_A30A0 = 4;
        playMusicTrack(9);
    }

    state->unk4C = 0x96;
    state->unk7B = 1;

    setGameStateHandler(awaitBossResultAndFadeOut);
}

void awaitBossResultAndFadeOut(void) {
    s32 delayTimer;
    GameState *state;

    state = (GameState *)getCurrentAllocation();
    delayTimer = state->unk4C - 1;
    state->unk4C = delayTimer;
    if (delayTimer == 0) {
        func_8006FDA0_709A0(NULL, 0xFF, 0x10);
        setMusicFadeOut(0x3C);
        setGameStateHandler(&cleanupGameSession);
    }
}

void handleSkillGameResult(void) {
    GameState *state;
    s32 delayTimer;

    state = (GameState *)getCurrentAllocation();
    delayTimer = state->unk4C;

    if (delayTimer == 0) {
        state->unk4C = 0xB4;

        if (state->unk7D == 0) {
            terminateTasksByTypeAndID(0, 1);
            D_800A24A0_A30A0 = 5;

            if (state->unk7E != 0) {
                D_800A24A0_A30A0 = 9;
            }

            playMusicTrack(8);
            spawnSuccessMessageDisplayTask(0x78);
            spawnVictorySnowflakes(0, 0);
            state->unk7B = 1;
            scheduleTask(&func_8004F820_50420, 1, 0, 0xE6);
            addPlayerGold(0x1388);
            setGameStateHandler(&awaitSkillWinAndPromptContinue);
        } else {
            D_800A24A0_A30A0 = 6;
            playMusicTrack(9);
            state->unk7B = 1;
            setGameStateHandler(&awaitSkillLossAndFadeOut);
        }
    } else {
        state->unk4C = delayTimer - 1;
    }
}

void awaitSkillWinAndPromptContinue(void) {
    s32 delayTimer;
    GameState *state;

    state = (GameState *)getCurrentAllocation();
    delayTimer = state->unk4C - 1;
    state->unk4C = delayTimer;
    if (delayTimer == 0) {
        state->unk7C = 1;
        scheduleTask(initBonusGoldDisplayTask, 1, 0, 0xE6);
        playMusicTrack(0xA);
        setGameStateHandler(&awaitSkillWinContinuePress);
    }
}

void awaitSkillWinContinuePress(void) {
    if (gControllerInputs[0] & A_BUTTON) {
        func_8006FDA0_709A0(NULL, 0xFF, 0x10);
        setMusicFadeOut(0x3C);
        setGameStateHandler(&cleanupGameSession);
    }
}

void awaitSkillLossAndFadeOut(void) {
    GameState *state = (GameState *)getCurrentAllocation();
    s32 delayTimer = state->unk4C - 1;
    state->unk4C = delayTimer;

    if (delayTimer == 0) {
        func_8006FDA0_709A0(0, 0xFF, 0x10);
        setMusicFadeOut(0x3C);
        setGameStateHandler(cleanupGameSession);
    }
}

void handleShotCrossGameResult(void) {
    GameState *state;
    s32 score;
    u8 itemsCollected;

    state = (GameState *)getCurrentAllocation();

    if (state->unk4C != 0) {
        state->unk4C = state->unk4C - 1;
        return;
    }

    if (getFreeNodeCount(4) != 0x1E) {
        return;
    }

    if (state->unk7D == 0) {
        terminateTasksByTypeAndID(0, 1);
        spawnShotCrossItemCountDisplayTask(1);

        itemsCollected = state->unk5A;
        score = itemsCollected * 300;

        if (itemsCollected == 0x14) {
            D_800A24A0_A30A0 = 5;
            playMusicTrack(8);

            if (state->players->unkBD3 == 0xA) {
                D_800A24A0_A30A0 = 7;
                score += 0x4E20;
            } else {
                score += 0x2710;
            }

            spawnSuccessMessageDisplayTask(0x78);
            spawnVictorySnowflakes(0, 0);
        } else {
            D_800A24A0_A30A0 = 6;
            playMusicTrack(9);
        }

        state->unk7B = 1;
        addPlayerGold(score);
        setGameStateHandler(&awaitShotCrossWinAndPromptContinue);
    } else {
        D_800A24A0_A30A0 = 6;
        playMusicTrack(9);
        state->unk7B = 1;
        setGameStateHandler(&awaitShotCrossLossAndFadeOut);
    }

    state->unk4C = 0xB4;
}

void awaitShotCrossWinAndPromptContinue(void) {
    GameState *gs = (GameState *)getCurrentAllocation();
    gs->unk4C--;

    if (gs->unk4C == 0) {
        gs->unk7C = 1;
        playMusicTrack(0xA);
        scheduleTask(&initGoldAwardDisplayTask, 1, 0, 0xE6);
        setGameStateHandler(&awaitShotCrossWinContinuePress);
    }
}

void awaitShotCrossWinContinuePress(void) {
    if (gControllerInputs[0] & A_BUTTON) {
        func_8006FDA0_709A0(0, 0xFF, 0x10);
        setMusicFadeOut(0x3C);
        setGameStateHandler(&cleanupGameSession);
    }
}

void awaitShotCrossLossAndFadeOut(void) {
    GameState *state = (GameState *)getCurrentAllocation();
    s32 delayTimer = state->unk4C - 1;
    state->unk4C = delayTimer;

    if (delayTimer == 0) {
        func_8006FDA0_709A0(NULL, 0xFF, 0x10);
        setMusicFadeOut(0x3C);
        setGameStateHandler(&cleanupGameSession);
    }
}

void handleMeterGameResult(void) {
    GameState *state;
    s32 meterValue;
    s32 goldReward;
    void (*handler)(void);

    state = (GameState *)getCurrentAllocation();

    if (state->unk4C == 0) {
        if (state->unk7D == 0) {
            terminateTasksByTypeAndID(0, 1);
            spawnShotCrossSkillMeterDisplayTask(1);

            meterValue = state->players->unkB70;
            goldReward = ((meterValue * 2 + meterValue) * 8 + meterValue) * 2;

            if (meterValue >= 0x12C) {
                D_800A24A0_A30A0 = 5;
                playMusicTrack(8);
                spawnSuccessMessageDisplayTask(0x78);
                spawnVictorySnowflakes(0, 0);

                goldReward += 0x1388;

                if (state->players->unkB70 >= 0x258) {
                    D_800A24A0_A30A0 = 8;
                }
            } else {
                D_800A24A0_A30A0 = 6;
                playMusicTrack(9);
            }

            state->unk7B = 1;
            addPlayerGold(goldReward);
            handler = awaitMeterWinAndPromptContinue;
        } else {
            D_800A24A0_A30A0 = 6;
            playMusicTrack(9);
            handler = awaitMeterLossAndFadeOut;
            state->unk7B = 1;
        }
        setGameStateHandler(handler);
        state->unk4C = 0xB4;
    } else {
        state->unk4C = state->unk4C - 1;
    }
}

void awaitMeterWinAndPromptContinue(void) {
    s32 delayTimer;
    GameState *state;

    state = (GameState *)getCurrentAllocation();
    delayTimer = state->unk4C - 1;
    state->unk4C = delayTimer;
    if (delayTimer == 0) {
        state->unk7C = 1;
        playMusicTrack(0xA);
        scheduleTask(&initGoldAwardDisplayTask, 1, 0, 0xE6);
        setGameStateHandler(&awaitMeterWinContinuePress);
    }
}

void awaitMeterWinContinuePress(void) {
    if (gControllerInputs[0] & A_BUTTON) {
        func_8006FDA0_709A0(0, 0xFF, 0x10);
        setMusicFadeOut(0x3C);
        setGameStateHandler(cleanupGameSession);
    }
}

void awaitMeterLossAndFadeOut(void) {
    GameState *state = (GameState *)getCurrentAllocation();
    s32 delayTimer = state->unk4C - 1;
    state->unk4C = delayTimer;
    if (delayTimer == 0) {
        func_8006FDA0_709A0(0, 0xFF, 0x10);
        setMusicFadeOut(0x3C);
        setGameStateHandler(cleanupGameSession);
    }
}

void handleSpeedCrossGameResult(void) {
    GameState *state;
    s32 delayTimer;

    state = (GameState *)getCurrentAllocation();
    delayTimer = state->unk4C;

    if (delayTimer == 0) {
        scheduleTask((void *)initSpeedCrossFinishPositionTask, 1, 0, 0xE6);
        state->unk4C = 0xB4;

        if (state->players->finishPosition == 0) {
            D_800A24A0_A30A0 = 3;
            playMusicTrack(8);
            spawnVictorySnowflakes(0, 0);
            spawnSuccessMessageDisplayTask(0x78);
            terminateTasksByTypeAndID(0, 1);
        } else {
            D_800A24A0_A30A0 = 4;
            playMusicTrack(9);
            terminateTasksByTypeAndID(0, 1);
        }

        state->unk7B = 1;
        setGameStateHandler((void *)awaitSpeedCrossAwardGold);
    } else {
        state->unk4C = delayTimer - 1;
    }
}

void awaitSpeedCrossAwardGold(void) {
    GameState *state;
    Player *player;

    state = (GameState *)getCurrentAllocation();
    state->unk4C--;

    if (state->unk4C != 0) {
        return;
    }

    player = state->players;
    state->unk7C = 1;

    switch (player->finishPosition) {
        case 0:
            addPlayerGold(player->unkB6C);
            addPlayerGold(gFirstPlaceGoldReward[state->memoryPoolId]);
            break;

        case 1:
            addPlayerGold(player->unkB6C);
            addPlayerGold(gSecondPlaceGoldReward[state->memoryPoolId]);
            break;

        case 2:
            addPlayerGold(player->unkB6C);
            addPlayerGold(gThirdPlaceGoldReward[state->memoryPoolId]);
            break;

        case 3:
            player->unkB6C = 0;
            break;

        default:
            break;
    }

    scheduleTask(&initGoldAwardDisplayTask, 1, 0, 0xE6);
    playMusicTrack(0xA);
    setGameStateHandler(&awaitSpeedCrossContinuePress);
}

void awaitSpeedCrossContinuePress(void) {
    if (gControllerInputs[0] & A_BUTTON) {
        func_8006FDA0_709A0(0, 0xFF, 0x10);
        setMusicFadeOut(0x3C);
        setGameStateHandler(&cleanupGameSession);
    }
}

void awaitBattleEndAndPromptContinue(void) {
    s32 delayTimer;
    GameState *state;

    state = (GameState *)getCurrentAllocation();
    delayTimer = state->unk4C - 1;
    state->unk4C = delayTimer;
    if (delayTimer == 0) {
        playMusicTrack(0xA);
        setGameStateHandler(&awaitBattleContinuePress);
    }
}

void awaitBattleContinuePress(void) {
    s32 i;
    GameState *state;

    state = (GameState *)getCurrentAllocation();
    for (i = 0; i < state->unk5F; i++) {
        if (gControllerInputs[i] & A_BUTTON) {
            func_8006FDA0_709A0(0, 0xFF, 0x10);
            setMusicFadeOut(0x3C);
            setGameStateHandler(&cleanupGameSession);
            return;
        }
    }
}

void handleExpertRaceResult(void) {
    GameState *gs;
    s32 playerWon;

    gs = (GameState *)getCurrentAllocation();
    gs->unk4C--;

    if (gs->unk4C == 0) {
        playerWon = gs->players->finishPosition == 0;
        D_800A24A0_A30A0 = playerWon ? 3 : 4;
        playMusicTrack(0xA);
        setGameStateHandler(&awaitExpertRaceContinuePress);
    }
}

void awaitExpertRaceContinuePress(void) {
    s32 i;
    GameState *state = (GameState *)getCurrentAllocation();

    for (i = 0; i < (s32)state->unk5F; i++) {
        if (gControllerInputs[i] & A_BUTTON) {
            func_8006FDA0_709A0(0, 0xFF, 0x10);
            setMusicFadeOut(0x3C);
            setGameStateHandler(&cleanupGameSession);
            return;
        }
    }
}

void cleanupGameSession(void) {
    GameState *gameState;
    s32 i;
    s32 playerOffset;

    gameState = getCurrentAllocation();

    if (func_8006FE10_70A10(0) != 0) {
        return;
    }

    func_8006FE94_70A94(0, 0, 0, 0);
    terminateAllTasks();

    for (i = 0; i < gameState->unk5D; i++) {
        unlinkNode(gameState->unk4 + i);
    }

    for (i = 0; i < gameState->unk5D; i++) {
        unlinkNode(gameState->unk8 + i);
    }

    for (i = 0; i < gameState->unk5D; i++) {
        unlinkNode(gameState->unkC + i);
    }

    for (i = 0; i < gameState->numPlayers; i++) {
        func_800B9AE0(gameState->players + i);
    }

    unlinkNode(gameState->audioPlayer0);

    freeNodeMemory(gameState->players);
    freeNodeMemory(gameState->audioPlayer0);
    freeNodeMemory(gameState->unk4);
    freeNodeMemory(gameState->unk8);
    freeNodeMemory(gameState->unkC);
    freeNodeMemory(gameState->gameData.dataStart);
    freeNodeMemory(gameState->unk18);
    freeNodeMemory(gameState->unk1C);
    freeNodeMemory(gameState->unk20);
    freeNodeMemory(gameState->unk28);
    freeNodeMemory(gameState->unk14);

    osViExtendVStart(0);
    stopAllSoundEffectsAndClearQueues(0x14);
    terminateSchedulerWithCallback(&onGameSessionTerminated);
    gControllerPollingEnabled = 1;
    resumeMotorStates();
}

void onGameSessionTerminated(void) {
    func_800697F4_6A3F4(D_800A24A0_A30A0);
}
