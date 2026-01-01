#include "2DD40.h"
#include "3A1F0.h"
#include "3CD70.h"
#include "413E0.h"
#include "46080.h"
#include "4CD70.h"
#include "56910.h"
#include "6E840.h"
#include "9FF70.h"
#include "common.h"
#include "course.h"
#include "displaylist.h"
#include "gamestate.h"
#include "graphics.h"
#include "levels/lindas_castle.h"
#include "overlay.h"
#include "rand.h"
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

typedef struct {
    u8 unk0[0x18];
    ColorData unk18;
    ColorData unk20;
} VarData;

USE_ASSET(_34CB50);
USE_ASSET(_3FF010);
USE_ASSET(_40E870);
USE_OVERLAY(_9FF70);

extern s32 D_800B0334;
extern s32 D_800B7B7C;
extern s32 gControllerInputs[4];
extern u8 D_800A24A0_A30A0;
extern void func_800404A8_410A8(void);
extern void func_80040588_41188(void);
extern void func_80040608_41208(void);
extern s8 gControllerPollingEnabled;
extern s32 D_80090460_91060[];
extern s32 D_800904A0_910A0[];
extern s32 D_800904E0_910E0[];

void func_80040420_41020(void);
void func_800407B4_413B4(void);
void parseRaceAssetData(void);
void scheduleRaceTasks(void);
void awaitRaceAssetsLoaded(void);
void waitForFadeAndInitPlayers(void);
void awaitBossResultAndFadeOut(void);
void func_800401A0_40DA0(void);
void awaitPlayersAndPlayRaceMusic(void);
void loadPlayerAssets(void);
void func_8003F368_3FF68(void);
void func_8003FD3C_4093C(void);
void func_8003FF78_40B78(void);
void awaitSkillWinAndPromptContinue(void);
void func_8003FD84_40984(void);
void func_8003FF14_40B14(void);
void func_8003FFC0_40BC0(void);
void func_80040304_40F04(void);
void func_8004013C_40D3C(void);
void func_800401E8_40DE8(void);

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

extern void loadRaceGameData(void);
extern ColorData D_80090774_91374;
extern ColorData D_8009077C_9137C;

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

extern void initRaceViewports(void);

extern SessionConfig *D_800AFE8C_A71FC;

extern OverlayEntry Overlays[];

extern u8 D_80090280_90E80[4][4]; // unknown first (4) size
extern u8 D_800902C0_90EC0[];

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
} Unk800902D0_90ED0;
extern Unk800902D0_90ED0 D_800902D0_90ED0[16][6]; // unknown size (16)
extern u8 D_80090450_91050[];
extern u8 D_80090520_91120[];

extern u32 D_800BAA30;

enum GameMode {
    GAME_MODE_STORY = 0,
    GAME_MODE_BATTLE = 1,
    GAME_MODE_DEMO = 2,
    GAME_MODE_INTRO = 3,
};

enum RaceType {
    RACE_TYPE_STANDARD = 0,
    RACE_TYPE_BOSS_JUNGLE = 1,
    RACE_TYPE_BOSS_JINGLE = 2,
    RACE_TYPE_BOSS_ICE = 3,
    RACE_TYPE_SPEED_CROSS = 4,
    RACE_TYPE_SHOT_CROSS = 5,
    RACE_TYPE_X_CROSS = 6,
    RACE_TYPE_UNUSED_7 = 7,
    RACE_TYPE_BATTLE = 8,
    RACE_TYPE_TRAINING = 9,
    RACE_TYPE_DEMO = 10,
    RACE_TYPE_INTRO = 11,
};

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
    func_80056990_57590(0x60, 0x1400);
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
    gameState->unk60 = 0;

    scheduleTask(&D_800B0334, 0, 0, 1);
    scheduleTask(&D_800B7B7C, 0, 0, 0x64);
    scheduleTask(&func_8004E6F8_4F2F8, 0, 0, 0xC8);

    func_800497FC_4A3FC(gameState->memoryPoolId);
    func_80049CA8_4A8A8(gameState->memoryPoolId, gameState->unk5F);
    func_8005011C_50D1C();

    if (gameState->unk7A == 9) {
        scheduleTask(&func_800BB2B0, 0, 0, 0xC8);
    }

    if (gameState->unk7A >= 0xA) {
        func_80040F6C_41B6C(0, 0x28, 0, 0, 0xC, 6);
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
            func_800574A0_580A0(7);
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
                    func_80048860_49460(&state->players[i]);
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
            func_800574A0_580A0(0x20);
        } else {
            func_800574A0_580A0(levelConfig->unk28);
        }

        setGameStateHandler(func_8003F368_3FF68);
    }
}

INCLUDE_ASM("asm/nonmatchings/3E160", func_8003F1F0_3FDF0);

INCLUDE_ASM("asm/nonmatchings/3E160", func_8003F368_3FF68);

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
        func_800574A0_580A0(8);
    } else {
        D_800A24A0_A30A0 = 4;
        func_800574A0_580A0(9);
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
        func_800574A0_580A0(8);
    } else {
        D_800A24A0_A30A0 = 4;
        func_800574A0_580A0(9);
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
        func_80057564_58164(0x3C);
        setGameStateHandler(&func_80040608_41208);
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

            func_800574A0_580A0(8);
            func_8004F194_4FD94(0x78);
            func_8004E6A4_4F2A4(0, 0);
            state->unk7B = 1;
            scheduleTask(&func_8004F820_50420, 1, 0, 0xE6);
            addPlayerGold(0x1388);
            setGameStateHandler(&awaitSkillWinAndPromptContinue);
        } else {
            D_800A24A0_A30A0 = 6;
            func_800574A0_580A0(9);
            state->unk7B = 1;
            setGameStateHandler(&func_8003FD84_40984);
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
        scheduleTask(&func_8004F1D4_4FDD4, 1, 0, 0xE6);
        func_800574A0_580A0(0xA);
        setGameStateHandler(&func_8003FD3C_4093C);
    }
}

void func_8003FD3C_4093C(void) {
    if (gControllerInputs[0] & A_BUTTON) {
        func_8006FDA0_709A0(NULL, 0xFF, 0x10);
        func_80057564_58164(0x3C);
        setGameStateHandler(&func_80040608_41208);
    }
}

void func_8003FD84_40984(void) {
    GameState *gameState = (GameState *)getCurrentAllocation();
    s32 temp = gameState->unk4C - 1;
    gameState->unk4C = temp;

    if (temp == 0) {
        func_8006FDA0_709A0(0, 0xFF, 0x10);
        func_80057564_58164(0x3C);
        setGameStateHandler(func_80040608_41208);
    }
}

void func_8003FDD4_409D4(void) {
    GameState *state;
    s32 score;
    u8 unk5AValue;

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
        func_8004EDCC_4F9CC(1);

        unk5AValue = state->unk5A;
        score = unk5AValue * 300;

        if (unk5AValue == 0x14) {
            D_800A24A0_A30A0 = 5;
            func_800574A0_580A0(8);

            if (state->players->unkBD3 == 0xA) {
                D_800A24A0_A30A0 = 7;
                score += 0x4E20;
            } else {
                score += 0x2710;
            }

            func_8004F194_4FD94(0x78);
            func_8004E6A4_4F2A4(0, 0);
        } else {
            D_800A24A0_A30A0 = 6;
            func_800574A0_580A0(9);
        }

        state->unk7B = 1;
        addPlayerGold(score);
        setGameStateHandler(&func_8003FF14_40B14);
    } else {
        D_800A24A0_A30A0 = 6;
        func_800574A0_580A0(9);
        state->unk7B = 1;
        setGameStateHandler(&func_8003FFC0_40BC0);
    }

    state->unk4C = 0xB4;
}

void func_8003FF14_40B14(void) {
    GameState *gs = (GameState *)getCurrentAllocation();
    gs->unk4C--;

    if (gs->unk4C == 0) {
        gs->unk7C = 1;
        func_800574A0_580A0(0xA);
        scheduleTask(&func_8004D9D0_4E5D0, 1, 0, 0xE6);
        setGameStateHandler(&func_8003FF78_40B78);
    }
}

void func_8003FF78_40B78(void) {
    if (gControllerInputs[0] & 0x8000) {
        func_8006FDA0_709A0(0, 0xFF, 0x10);
        func_80057564_58164(0x3C);
        setGameStateHandler(&func_80040608_41208);
    }
}

void func_8003FFC0_40BC0(void) {
    s32 temp_v1;
    GameState *temp_v0;

    temp_v0 = (GameState *)getCurrentAllocation();
    temp_v1 = temp_v0->unk4C - 1;
    temp_v0->unk4C = temp_v1;
    if (temp_v1 == 0) {
        func_8006FDA0_709A0(NULL, 0xFF, 0x10);
        func_80057564_58164(0x3C);
        setGameStateHandler(&func_80040608_41208);
    }
}

void func_80040010_40C10(void) {
    GameState *state;
    s32 value;
    s32 score;
    void (*handler)(void);

    state = (GameState *)getCurrentAllocation();

    if (state->unk4C == 0) {
        if (state->unk7D == 0) {
            terminateTasksByTypeAndID(0, 1);
            func_8004FF60_50B60(1);

            value = state->players->unkB70;
            score = ((value * 2 + value) * 8 + value) * 2;

            if (value >= 0x12C) {
                D_800A24A0_A30A0 = 5;
                func_800574A0_580A0(8);
                func_8004F194_4FD94(0x78);
                func_8004E6A4_4F2A4(0, 0);

                score += 0x1388;

                if (state->players->unkB70 >= 0x258) {
                    D_800A24A0_A30A0 = 8;
                }
            } else {
                D_800A24A0_A30A0 = 6;
                func_800574A0_580A0(9);
            }

            state->unk7B = 1;
            addPlayerGold(score);
            handler = func_8004013C_40D3C;
        } else {
            D_800A24A0_A30A0 = 6;
            func_800574A0_580A0(9);
            handler = func_800401E8_40DE8;
            state->unk7B = 1;
        }
        setGameStateHandler(handler);
        state->unk4C = 0xB4;
    } else {
        state->unk4C = state->unk4C - 1;
    }
}

void func_8004013C_40D3C(void) {
    s32 temp_v0_2;
    GameState *temp_v0;

    temp_v0 = (GameState *)getCurrentAllocation();
    temp_v0_2 = temp_v0->unk4C - 1;
    temp_v0->unk4C = temp_v0_2;
    if (temp_v0_2 == 0) {
        temp_v0->unk7C = 1;
        func_800574A0_580A0(0xA);
        scheduleTask(&func_8004D9D0_4E5D0, 1, 0, 0xE6);
        setGameStateHandler(&func_800401A0_40DA0);
    }
}

void func_800401A0_40DA0(void) {
    if (gControllerInputs[0] & A_BUTTON) {
        func_8006FDA0_709A0(0, 0xFF, 0x10);
        func_80057564_58164(0x3C);
        setGameStateHandler(func_80040608_41208);
    }
}

void func_800401E8_40DE8(void) {
    GameState *state = (GameState *)getCurrentAllocation();
    s32 temp = state->unk4C - 1;
    state->unk4C = temp;
    if (temp == 0) {
        func_8006FDA0_709A0(0, 0xFF, 0x10);
        func_80057564_58164(0x3C);
        setGameStateHandler(func_80040608_41208);
    }
}

void func_80040238_40E38(void) {
    GameState *allocation;
    s32 counter;

    allocation = (GameState *)getCurrentAllocation();
    counter = allocation->unk4C;

    if (counter == 0) {
        scheduleTask((void *)func_8004D8E4_4E4E4, 1, 0, 0xE6);
        allocation->unk4C = 0xB4;

        if (allocation->players->finishPosition == 0) {
            D_800A24A0_A30A0 = 3;
            func_800574A0_580A0(8);
            func_8004E6A4_4F2A4(0, 0);
            func_8004F194_4FD94(0x78);
            terminateTasksByTypeAndID(0, 1);
        } else {
            D_800A24A0_A30A0 = 4;
            func_800574A0_580A0(9);
            terminateTasksByTypeAndID(0, 1);
        }

        allocation->unk7B = 1;
        setGameStateHandler((void *)func_80040304_40F04);
    } else {
        allocation->unk4C = counter - 1;
    }
}

void func_80040304_40F04(void) {
    GameState *gameState;
    Player *player;

    gameState = (GameState *)getCurrentAllocation();
    gameState->unk4C--;

    if (gameState->unk4C != 0) {
        return;
    }

    player = gameState->players;
    gameState->unk7C = 1;

    switch (player->finishPosition) {
        case 0:
            addPlayerGold(player->unkB6C);
            addPlayerGold(D_80090460_91060[gameState->memoryPoolId]);
            break;

        case 1:
            addPlayerGold(player->unkB6C);
            addPlayerGold(D_800904A0_910A0[gameState->memoryPoolId]);
            break;

        case 2:
            addPlayerGold(player->unkB6C);
            addPlayerGold(D_800904E0_910E0[gameState->memoryPoolId]);
            break;

        case 3:
            player->unkB6C = 0;
            break;

        default:
            break;
    }

    scheduleTask(&func_8004D9D0_4E5D0, 1, 0, 0xE6);
    func_800574A0_580A0(0xA);
    setGameStateHandler(&func_80040420_41020);
}

void func_80040420_41020(void) {
    if (gControllerInputs[0] & A_BUTTON) {
        func_8006FDA0_709A0(0, 0xFF, 0x10);
        func_80057564_58164(0x3C);
        setGameStateHandler(&func_80040608_41208);
    }
}

void func_80040468_41068(void) {
    s32 temp_v1;
    GameState *temp_v0;

    temp_v0 = (GameState *)getCurrentAllocation();
    temp_v1 = temp_v0->unk4C - 1;
    temp_v0->unk4C = temp_v1;
    if (temp_v1 == 0) {
        func_800574A0_580A0(0xA);
        setGameStateHandler(&func_800404A8_410A8);
    }
}

void func_800404A8_410A8(void) {
    s32 i;
    s32 *var_a0;
    GameState *temp_v0;

    temp_v0 = (GameState *)getCurrentAllocation();
    for (i = 0; i < temp_v0->unk5F; i++) {
        if (gControllerInputs[i] & A_BUTTON) {
            func_8006FDA0_709A0(0, 0xFF, 0x10);
            func_80057564_58164(0x3C);
            setGameStateHandler(&func_80040608_41208);
            return;
        }
    }
}

void func_80040528_41128(void) {
    GameState *gs;
    s32 new_var;
    gs = (GameState *)getCurrentAllocation();
    gs->unk4C--;
    if (gs->unk4C == 0) {
        new_var = gs->players->finishPosition == 0;
        D_800A24A0_A30A0 = (new_var) ? 3 : 4;
        func_800574A0_580A0(0xA);
        setGameStateHandler(&func_80040588_41188);
    }
}

void func_80040588_41188(void) {
    s32 i;
    GameState *temp_v0 = (GameState *)getCurrentAllocation();

    for (i = 0; i < (s32)temp_v0->unk5F; i++) {
        if (gControllerInputs[i] & A_BUTTON) {
            func_8006FDA0_709A0(0, 0xFF, 0x10);
            func_80057564_58164(0x3C);
            setGameStateHandler(&func_80040608_41208);
            return;
        }
    }
}

void func_80040608_41208(void) {
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
    func_80057B1C_5871C(0x14);
    terminateSchedulerWithCallback(&func_800407B4_413B4);
    gControllerPollingEnabled = 1;
    resumeMotorStates();
}

void func_800407B4_413B4(void) {
    func_800697F4_6A3F4(D_800A24A0_A30A0);
}
