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
#include "displaylist.h"
#include "gamestate.h"
#include "graphics.h"
#include "levels/lindas_castle.h"
#include "overlay.h"
#include "rand.h"
#include "task_scheduler.h"

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
extern void func_8004EDCC_4F9CC(s32);
extern s8 D_800AFCE2_A7052;
extern s32 D_80090460_91060[];
extern s32 D_800904A0_910A0[];
extern s32 D_800904E0_910E0[];

void func_80040420_41020(void);
void func_800407B4_413B4(void);
void func_8003EDF8_3F9F8(void);
void func_8003EEEC_3FAEC(void);
void func_8003EFDC_3FBDC(void);
void func_8003F0AC_3FCAC(void);
void func_8003FB90_40790(void);
void func_800401A0_40DA0(void);
void func_8003F178_3FD78(void);
void func_8003EE50_3FA50(void);
void func_8003F368_3FF68(void);
void func_8003FD3C_4093C(void);
void func_8003FF78_40B78(void);
void func_8003FCD8_408D8(void);
void func_8003FD84_40984(void);
void func_8003FF14_40B14(void);
void func_8003FFC0_40BC0(void);
void func_80040304_40F04(void);
void func_8004013C_40D3C(void);
void func_800401E8_40DE8(void);

typedef struct {
    u8 unk0[4];
    u8 gameMode;
    u8 unk5;
    u8 unk6;
    u8 currentLevel;
    u8 unk8;
    u8 unk9[4];
    u8 unkD[4];
    u8 unk11[4];
    u8 unk15[4];
    u8 unk19;
    u8 pad1A[5];
    u8 unk1F;
    u8 unk20;
    u8 pad21[3];
    u8 unk24;
} GameConfig;

typedef struct {
    u8 padding_0[0xBB8];
    u8 unkBB8;
    /* 0xBB9 */ u8 charcterID;
    u8 unkBBA;
    u8 unkBBB;
    u8 unkBBC;
    u8 unkBBD;
    u8 padding_BBE[0xBC7 - 0xBBE];
    u8 unkBC7;
    u8 padding_BC8[0xBD9 - 0xBC8];
    u8 unkBD9;
    u8 unkBDA;
    u8 padding_BDB[0xBDD - 0xBDB];
    u8 unkBDD;
    u8 padding_BDC[0xBE3 - 0xBDE];
    u8 unkBE3;
    u8 unkBE4;
    u8 padding_BE5[0xBE8 - 0xBE5];
} GameStateSmall_unk10;

typedef struct {
    void *unk0;
    void *unk4;
    void *unk8;
    void *unkC;
    GameStateSmall_unk10 *unk10;
    void *unk14;
    u8 pad18[64];
    u16 unk58;
    u8 unk5A;
    u8 unk5B;
    /* 0x5C */ u8 currentLevel;
    /* 0x5D */ u8 humanPlayerCount;
    /* 0x5E */ u8 totalRacers;
    u8 unk5F;
    u8 pad60[4];
    u8 unk64[16];
    u8 unk74;
    u8 unk75;
    u8 pad76[3];
    u8 unk79;
    u8 unk7A;
    u8 unk7B;
    u8 pad7C[3];
    u8 unk7F;
    u8 pad80[3];
    u8 unk83;
    u8 unk84;
    u8 pad85[1];
    u8 unk86;
} GameStateSmall;

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

extern void func_8003E4F0_3F0F0(void);

extern u8 D_800A24A0_A30A0;
extern GameConfig *D_800AFE8C_A71FC;
extern s8 D_800AFCE2_A7052;
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
    MODE_STORY = 0,
    MODE_BATTLE = 1,
    MODE_UNKNOWN = 2,
    MODE_INTRO = 3,
};

enum Course {
    SUNNY_MOUNTAIN = 0,
    TURTLE_ISLAND = 1,
    JINGLE_TOWN = 2,
    JINGLE_TOWN_BOSS = 3,
    WENDYS_HOUSE = 4,
    LINDAS_CASTLE = 5,
    CRAZY_JUNGLE = 6,
    CRAZY_JUNGLE_BOSS = 7,
    STARLIGHT_HIGHWAY = 8,
    HAUNTED_HOUSE = 9,
    ICE_LAND = 10,
    ICE_LAND_BOSS = 11,
    SNOWBOARD_STREET_SPEED_CROSS = 12,
    SNOWBOARD_STREET_SHOT_CROSS = 13,
    X_CROSS = 14,
    TRAINING = 15,
};

void initRace(void) {
    GameStateSmall *gs;
    s32 i;
    s32 mode;
    s32 temp;
    s32 offset;
    u8 charCount[6];
    s32 pad2[2];
    u8 charList[6];
    s32 pad[2];

    LOAD_OVERLAY(_9FF70);

    func_800698CC_6A4CC(0x37);

    // Allocate gs and zero out memory
    gs = allocateTaskMemory(0x88);
    for (i = sizeof(GameStateSmall) - 1; i >= 0; i--) {
        ((u8 *)gs)[i] = 0;
    }

    D_800A24A0_A30A0 = 2;
    gs->unk75 = 0x7F;
    gs->unk79 = 2;
    gs->unk83 = 0;
    gs->unk84 = 0;
    gs->unk58 = 0x7FFF;

    switch (D_800AFE8C_A71FC->gameMode) {
        case MODE_STORY:
            gs->currentLevel = D_800AFE8C_A71FC->currentLevel;
            gs->unk74 = D_800AFE8C_A71FC->unk19 - 1;
            gs->unk86 = D_800AFE8C_A71FC->unk24;

            switch (gs->currentLevel) {
                case LINDAS_CASTLE:
                case WENDYS_HOUSE:
                case CRAZY_JUNGLE:
                default:
                    gs->humanPlayerCount = D_800AFE8C_A71FC->unk8;
                    gs->unk5F = D_800AFE8C_A71FC->unk8;
                    gs->totalRacers = 4;
                    if (gs->unk86 != 0) {
                        gs->unk83 = 1;
                    }
                    break;
                case CRAZY_JUNGLE_BOSS:
                    gs->unk7A = 1;
                    gs->humanPlayerCount = 1;
                    gs->unk5F = 1;
                    gs->totalRacers = 2;
                    gs->unk74 = 0;
                    break;
                case JINGLE_TOWN_BOSS:
                    gs->unk7A = 2;
                    gs->humanPlayerCount = 1;
                    gs->unk5F = 1;
                    gs->totalRacers = 2;
                    gs->unk74 = 0;
                    break;
                case ICE_LAND_BOSS:
                    gs->unk7A = 3;
                    gs->humanPlayerCount = 1;
                    gs->unk5F = 1;
                    gs->totalRacers = 2;
                    gs->unk74 = 0;
                    break;
                case SNOWBOARD_STREET_SPEED_CROSS:
                    gs->unk7A = 4;
                    gs->humanPlayerCount = 1;
                    gs->unk5F = 1;
                    gs->totalRacers = 1;
                    gs->unk74 = 0;
                    break;
                case SNOWBOARD_STREET_SHOT_CROSS:
                    gs->unk7A = 5;
                    gs->humanPlayerCount = 1;
                    gs->unk5F = 1;
                    gs->totalRacers = 1;
                    gs->unk74 = 0;
                    break;
                case X_CROSS:
                    gs->unk7A = 6;
                    gs->humanPlayerCount = 1;
                    gs->unk5F = 1;
                    gs->totalRacers = 1;
                    gs->unk74 = 0;
                    break;
                case TRAINING:
                    gs->unk7A = 9;
                    gs->humanPlayerCount = 1;
                    gs->unk5F = 1;
                    gs->totalRacers = 2;
                    gs->unk74 = 1;
                    gs->unk7B = 1;
                    break;
            }
            break;
        case MODE_BATTLE:
            gs->currentLevel = D_800AFE8C_A71FC->currentLevel;
            gs->unk74 = D_800AFE8C_A71FC->unk19 - 1;
            gs->unk7A = 8;
            gs->humanPlayerCount = D_800AFE8C_A71FC->unk8;
            gs->unk5F = D_800AFE8C_A71FC->unk8;
            gs->totalRacers = 4;
            gs->unk7B = 1;
            gs->unk83 = D_800AFE8C_A71FC->unk1F;
            gs->unk84 = D_800AFE8C_A71FC->unk20;
            break;

        case MODE_UNKNOWN:
            initRand();
            gs->unk7A = 10;
            gs->unk7F = D_800AFE8C_A71FC->unk5;

            switch (gs->unk7F) {
                case 0:
                    gs->humanPlayerCount = 1;
                    gs->unk5F = 1;
                    gs->currentLevel = SUNNY_MOUNTAIN;
                    gs->unk74 = 2;
                    gs->totalRacers = 4;
                    break;
                case 1:
                    gs->currentLevel = TURTLE_ISLAND;
                    gs->unk74 = 2;
                    gs->humanPlayerCount = 2;
                    gs->unk5F = 2;
                    gs->totalRacers = 4;
                    break;
                case 2:
                    gs->currentLevel = LINDAS_CASTLE;
                    gs->unk74 = 2;
                    gs->humanPlayerCount = 4;
                    gs->unk5F = 4;
                    gs->totalRacers = 4;
                    break;
            }
            break;
        case MODE_INTRO:
            initRand();
            gs->unk7F = 0;
            gs->unk7A = 11;
            gs->currentLevel = 2;
            gs->unk74 = 2;
            gs->unk5F = 4;
            gs->totalRacers = 4;

            temp = gs->unk7F;
            switch (gs->unk7F) {
                case 0:
                case 1:
                    gs->humanPlayerCount = 1;
                    gs->unk14 = dmaRequestAndUpdateStateWithSize(&_40E870_ROM_START, &_40E870_ROM_END, 0xB8E0);
                    break;
            }
            break;
    }

    if (Overlays[gs->currentLevel].romStart != NULL) {
        dmaLoadAndInvalidate(
            Overlays[gs->currentLevel].romStart,
            Overlays[gs->currentLevel].romEnd,
            Overlays[gs->currentLevel].ramStart,
            Overlays[gs->currentLevel].icacheStart,
            Overlays[gs->currentLevel].icacheEnd,
            Overlays[gs->currentLevel].dcacheStart,
            Overlays[gs->currentLevel].dcacheEnd,
            Overlays[gs->currentLevel].bssStart,
            Overlays[gs->currentLevel].bssEnd
        );
    }

    gs->unk0 = allocateNodeMemory(0x1D8);
    gs->unk4 = allocateNodeMemory(((gs->humanPlayerCount * 16 - gs->humanPlayerCount) * 4 - gs->humanPlayerCount) * 8);
    gs->unk8 = allocateNodeMemory(((gs->humanPlayerCount * 16 - gs->humanPlayerCount) * 4 - gs->humanPlayerCount) * 8);
    gs->unkC = allocateNodeMemory(((gs->humanPlayerCount * 16 - gs->humanPlayerCount) * 4 - gs->humanPlayerCount) * 8);
    gs->unk10 = allocateNodeMemory(
        ((gs->totalRacers * 2 + gs->totalRacers) * 128 - (gs->totalRacers * 2 + gs->totalRacers)) * 8
    );

    for (i = 0; i < gs->totalRacers; i++) {
        // zero out gs->unk10[i]
        u8 *bytes = (u8 *)&gs->unk10[i];
        for (temp = sizeof(GameStateSmall_unk10) - 1; temp >= 0; temp--) {
            bytes[temp] = 0;
        }

        gs->unk10[i].unkBB8 = i;
        gs->unk64[i] = i;
    }

    for (i = 11; i >= 0; i--) {
        charCount[i] = 0;
    }

    if (gs->unk7A < 9) {
        for (i = 0; i < gs->unk5F; i++) {
            gs->unk10[i].charcterID = D_800AFE8C_A71FC->unk9[i];
            gs->unk10[i].unkBBA = D_800AFE8C_A71FC->unk15[i];
            gs->unk10[i].unkBBB = D_800AFE8C_A71FC->unkD[i];
            gs->unk10[i].unkBBC = D_800AFE8C_A71FC->unk11[i];
        }
    }

    for (i = gs->unk5F; i < gs->totalRacers; i++) {
        gs->unk10[i].charcterID = D_80090280_90E80[gs->currentLevel][i];
        gs->unk10[i].unkBBA = D_800902C0_90EC0[gs->currentLevel];
        gs->unk10[i].unkBC7 = 1;
        gs->unk10[i].unkBDA = 1;
    }

    switch (D_800AFE8C_A71FC->gameMode) {
        case MODE_STORY:
            switch (gs->currentLevel) {
                case CRAZY_JUNGLE_BOSS:
                    gs->unk10[1].unkBD9 = 1;
                    gs->unk10[1].charcterID = 9;
                    break;
                case JINGLE_TOWN_BOSS:
                    gs->unk10[1].unkBD9 = 2;
                    gs->unk10[1].charcterID = 10;
                    break;
                case ICE_LAND_BOSS:
                    gs->unk10[1].unkBD9 = 3;
                    gs->unk10[1].charcterID = 11;
                    break;
                case TRAINING:
                    gs->unk10[0].charcterID = 0;
                    gs->unk10[0].unkBBA = 0;
                    gs->unk10[0].unkBBB = 0;
                    gs->unk10[0].unkBBC = 0;
                    break;
            }
            break;
        case MODE_UNKNOWN:
            switch (gs->unk7F) {
                case 0:
                    gs->unk10[0].charcterID = 0;
                    gs->unk10[0].unkBBA = 0;
                    gs->unk10[0].unkBBB = 0;
                    gs->unk10[0].unkBBC = 0;
                    gs->unk10[0].unkBE3 = 1;
                    gs->unk10[0].unkBE4 = 0;
                    break;
                case 1:
                    for (i = 0; i < 2; i++) {
                        gs->unk10[i].charcterID = i;
                        gs->unk10[i].unkBBA = 1;
                        gs->unk10[i].unkBBB = 0;
                        gs->unk10[i].unkBBC = i;
                        gs->unk10[i].unkBE3 = 1;
                        gs->unk10[i].unkBE4 = i + 1;
                    }
                    break;
                case 2:
                    for (i = 0; i < 4; i++) {
                        gs->unk10[i].charcterID = i;
                        gs->unk10[i].unkBBA = 1;
                        gs->unk10[i].unkBBB = 0;
                        gs->unk10[i].unkBBC = i;
                        gs->unk10[i].unkBE3 = 1;
                        gs->unk10[i].unkBE4 = i + 3;
                    }
            }
            break;
        case MODE_INTRO:
            gs->unk10[0].charcterID = 0;
            gs->unk10[1].charcterID = 3;
            gs->unk10[2].charcterID = 5;
            gs->unk10[3].charcterID = 2;

            for (i = 0; i < 4; i++) {
                gs->unk10[i].unkBBA = 0;
                gs->unk10[i].unkBBB = 0;
                gs->unk10[i].unkBBC = i;
                gs->unk10[i].unkBE4 = i + 7;
                if (gs->unk7F == 3) {
                    gs->unk10[i].unkBE3 = 2;
                } else {
                    gs->unk10[i].unkBE3 = 1;
                }
            }
            break;
        case MODE_BATTLE:
            break;
    }

    for (i = 0; i < gs->totalRacers; i++) {
        charCount[gs->unk10[i].charcterID]++;
    }

    for (i = gs->unk5F; i < gs->totalRacers; i++) {
        s32 j;
        u8 count;

        if (charCount[gs->unk10[i].charcterID] >= 2) {
            charCount[gs->unk10[i].charcterID]--;
            count = 0;
            for (j = 0; j < 6; j++) {
                if (charCount[j] == 0) {
                    charList[count] = j;
                    count++;
                }
            }

            gs->unk10[i].charcterID = charList[(u8)randA() % count];
            charCount[gs->unk10[i].charcterID]++;
        }
    }

    for (i = gs->unk5F; i < gs->totalRacers; i++) {
        if (gs->unk10[i].charcterID < 6) {
            gs->unk10[i].unkBBB = D_800902D0_90ED0[gs->currentLevel][gs->unk10[i].charcterID].unk0;
            gs->unk10[i].unkBBC = D_800902D0_90ED0[gs->currentLevel][gs->unk10[i].charcterID].unk1;

            if (gs->unk5F == 1) {
                gs->unk10[i].unkBDD = D_800902D0_90ED0[gs->currentLevel][gs->unk10[i].charcterID].unk2;
            } else {
                gs->unk10[i].unkBDD = D_800902D0_90ED0[gs->currentLevel][gs->unk10[i].charcterID].unk3;
            }

            if (gs->unk86 != 0) {
                gs->unk10[i].unkBBB = 2;
                if (i == 1) {
                    gs->unk10[1].unkBBB = D_80090450_91050[gs->currentLevel];
                }
            }
        } else {
            gs->unk10[i].unkBBB = 13;
            gs->unk10[i].unkBBC = 5;
            gs->unk10[i].unkBDD = 7;

            if (gs->unk7A == 8) {
                gs->unk10[i].unkBDD = 5;
            }

            if (gs->unk10[i].charcterID != 6) {
                gs->unk10[i].unkBBB = 9;
                if (gs->unk7A == 8) {
                    gs->unk10[i].unkBDD = 5;
                } else {
                    gs->unk10[i].unkBDD = 0;
                    gs->unk10[i].unkBBB = 1;
                }
            }
        }

        if (gs->unk86 != 0) {
            gs->unk10[i].unkBDD = 7;
        }
    }

    if (gs->unk7A == 8) {
        u8 maxLevel = 0;
        for (i = 0; i < gs->unk5F; i++) {
            if (maxLevel < D_80090520_91120[gs->unk10[i].unkBBB]) {
                maxLevel = D_80090520_91120[gs->unk10[i].unkBBB];
            }
        }

        for (i = gs->unk5F; i < gs->totalRacers; i++) {
            if (gs->unk10[i].unkBBB >= 9) {
                if (maxLevel == 4) {
                    gs->unk10[i].unkBBB = 16;
                }
                if (maxLevel == 5) {
                    gs->unk10[i].unkBBB = 17;
                }
                continue;
            }

            if (maxLevel <= 2) {
                gs->unk10[i].unkBBB = maxLevel + (gs->unk10[i].unkBBB / 3) * 3;
                continue;
            }

            if (maxLevel == 3) {
                gs->unk10[i].unkBBB = 9;
            }
            if (maxLevel == 4) {
                gs->unk10[i].unkBBB = 16;
            }
            if (maxLevel == 5) {
                gs->unk10[i].unkBBB = 17;
            }
        }
    }

    gs->unk5B = 0x28;
    switch (gs->unk7A) {
        default:
            setupTaskSchedulerNodes(0x50, 0x1E, 0x50, 0x28, 0xA, 0xA, 0xA, 0xA);
            break;
        case 2:
            setupTaskSchedulerNodes(0x50, 0x1E, 0x50, 0x28, 0xA, 0x1E, 0, 0);
            break;
        case 1:
        case 3:
            setupTaskSchedulerNodes(0x50, 0x1E, 0x50, 0x3C, 0xA, 0x14, 0, 0);
            gs->unk5B = 0x3C;
            break;
        case 5:
            setupTaskSchedulerNodes(0x50, 0x1E, 0x50, 0x28, 0x1E, 0, 0, 0);
            break;
    }

    D_800AFCE2_A7052 = 1;
    func_8006FDA0_709A0(0, 0xFF, 0);
    func_8006983C_6A43C((void *)func_8003E4F0_3F0F0);
}

INCLUDE_ASM("asm/nonmatchings/3E160", func_8003E4F0_3F0F0);

void func_8003EDA0_3F9A0(void) {
    GameState *temp_v0;

    temp_v0 = (GameState *)getCurrentAllocation();
    temp_v0->gameData.dataStart = func_80055D34_56934(temp_v0->memoryPoolId);
    temp_v0->unk28 = dmaRequestAndUpdateStateWithSize(&_3FF010_ROM_START, &_3FF010_ROM_END, 0x16E0);
    func_8006983C_6A43C(&func_8003EDF8_3F9F8);
}

void func_8003EDF8_3F9F8(void) {
    GameState *gs = (GameState *)getCurrentAllocation();

    parseGameDataLayout(&gs->gameData);

    gs->unk44 = (u8 *)gs->unk28 + gs->unk28->unk0;
    gs->unk48 = (u8 *)gs->unk28 + gs->unk28->unk4;

    func_8006983C_6A43C(&func_8003EE50_3FA50);
}

void func_8003EE50_3FA50(void) {
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
    gs->unk18 = dmaRequestAndUpdateStateWithSize(&_34CB50_ROM_START, &_34CB50_ROM_END, 0x5E28);
    gs->unk1C = loadAsset_B7E70();
    gs->unk20 = loadAsset_216290();
    func_8006983C_6A43C(&func_8003EEEC_3FAEC);
}

void func_8003EEEC_3FAEC(void) {
    GameState *temp_v0;

    temp_v0 = (GameState *)getCurrentAllocation();
    temp_v0->unk60 = 0;

    scheduleTask(&D_800B0334, 0, 0, 1);
    scheduleTask(&D_800B7B7C, 0, 0, 0x64);
    scheduleTask(&func_8004E6F8_4F2F8, 0, 0, 0xC8);

    func_800497FC_4A3FC(temp_v0->memoryPoolId);
    func_80049CA8_4A8A8(temp_v0->memoryPoolId, temp_v0->unk5F);
    func_8005011C_50D1C();

    if (temp_v0->unk7A == 9) {
        scheduleTask(&func_800BB2B0, 0, 0, 0xC8);
    }

    if (temp_v0->unk7A >= 0xA) {
        func_80040F6C_41B6C(0, 0x28, 0, 0, 0xC, 6);
    }

    setGameStateHandler(&func_8003EFDC_3FBDC);
}

void func_8003EFDC_3FBDC(void) {
    GameState *state;

    state = getCurrentAllocation();

    if (func_8003BB5C_3C75C() == 0) {
        if (state->unk7A < 9) {
            D_800AFCE2_A7052 = 0;
        }

        if ((state->unk5F == 1) && (state->unk7A < 10)) {
            func_8003D0C8_3DCC8();
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
        setGameStateHandler(func_8003F0AC_3FCAC);
    }
}

void func_8003F0AC_3FCAC(void) {
    s32 temp_a0;
    s32 i;
    GameState *temp_v0;

    temp_v0 = (GameState *)getCurrentAllocation();
    temp_a0 = temp_v0->unk4C;
    if (temp_v0->unk4C == 0) {
        if (temp_v0->unk79 == 1) {
            temp_v0->unk79 = 0;

            if (temp_v0->unk7A < 0xA) {
                for (i = 0; i < temp_v0->unk5F; i++) {
                    func_80048860_49460(&temp_v0->players[i]);
                    temp_v0->unk79++;
                }
            }

            if (temp_v0->unk7A == 0xB) {
                temp_v0->unk79 = 1;
            }

            setGameStateHandler(&func_8003F178_3FD78);
        }
    } else {
        temp_v0->unk4C = temp_a0 - 1;
    }
}

void func_8003F178_3FD78(void) {
    GameState *state = (GameState *)getCurrentAllocation();

    if ((state->unk79 == 0) || (state->unk7A == 0xB)) {
        void *ret = func_80055D10_56910(state->memoryPoolId);

        if (state->unk7A == 0xB) {
            func_800574A0_580A0(0x20);
        } else {
            func_800574A0_580A0(*(s16 *)(ret + 0x28));
        }

        setGameStateHandler(func_8003F368_3FF68);
    }
}

INCLUDE_ASM("asm/nonmatchings/3E160", func_8003F1F0_3FDF0);

INCLUDE_ASM("asm/nonmatchings/3E160", func_8003F368_3FF68);

void func_8003FA78_40678(void) {
    GameState *gs;
    Player *player;

    gs = (GameState *)getCurrentAllocation();
    gs->unk4C--;

    if (gs->unk4C != 0) {
        return;
    }

    player = (Player *)gs->players;

    if (player->unkBC4 == 0) {
        D_800A24A0_A30A0 = 3;
        func_800574A0_580A0(8);
    } else {
        D_800A24A0_A30A0 = 4;
        func_800574A0_580A0(9);
    }

    gs->unk4C = 0x96;
    gs->unk7B = 1;

    setGameStateHandler(func_8003FB90_40790);
}

void func_8003FB00_40700(void) {
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

    setGameStateHandler(func_8003FB90_40790);
}

void func_8003FB90_40790(void) {
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

void func_8003FBE0_407E0(void) {
    GameState *state;
    s32 counter;

    state = (GameState *)getCurrentAllocation();
    counter = state->unk4C;

    if (counter == 0) {
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
            func_8002ED40_2F940(0x1388);
            setGameStateHandler(&func_8003FCD8_408D8);
        } else {
            D_800A24A0_A30A0 = 6;
            func_800574A0_580A0(9);
            state->unk7B = 1;
            setGameStateHandler(&func_8003FD84_40984);
        }
    } else {
        state->unk4C = counter - 1;
    }
}

void func_8003FCD8_408D8(void) {
    s32 temp_v0_2;
    GameState *temp_v0;

    temp_v0 = (GameState *)getCurrentAllocation();
    temp_v0_2 = temp_v0->unk4C - 1;
    temp_v0->unk4C = temp_v0_2;
    if (temp_v0_2 == 0) {
        temp_v0->unk7C = 1;
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
        func_8002ED40_2F940(score);
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
            func_8002ED40_2F940(score);
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

        if (((u8 *)allocation->players)[0xBC4] == 0) {
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

    switch (player->unkBC4) {
        case 0:
            func_8002ED40_2F940(player->unkB6C);
            func_8002ED40_2F940(D_80090460_91060[gameState->memoryPoolId]);
            break;

        case 1:
            func_8002ED40_2F940(player->unkB6C);
            func_8002ED40_2F940(D_800904A0_910A0[gameState->memoryPoolId]);
            break;

        case 2:
            func_8002ED40_2F940(player->unkB6C);
            func_8002ED40_2F940(D_800904E0_910E0[gameState->memoryPoolId]);
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
        new_var = gs->players->unkBC4 == 0;
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
    D_800AFCE2_A7052 = 1;
    func_8003AF6C_3BB6C();
}

void func_800407B4_413B4(void) {
    func_800697F4_6A3F4(D_800A24A0_A30A0);
}
