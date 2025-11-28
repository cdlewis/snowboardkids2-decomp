#include "2DD40.h"
#include "3A1F0.h"
#include "413E0.h"
#include "46080.h"
#include "56910.h"
#include "6E840.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "graphics.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_34CB50);
USE_ASSET(_3FF010);

extern s32 D_800B0334;
extern s32 D_800B7B7C;
extern s32 gControllerInputs[4];
extern u8 D_800A24A0_A30A0;
extern void func_800404A8_410A8(void);
extern void func_80040588_41188(void);
extern void func_80040608_41208(void);
extern void func_8004D9D0_4E5D0(void);
extern void func_8004F1D4_4FDD4(void);
extern void func_8004E6F8_4F2F8(void);
extern void func_800BB2B0(void);
extern void func_8004D8E4_4E4E4(void);
extern void func_800574A0_580A0(s32);
extern void func_8004E6A4_4F2A4(s32, s32);
extern void func_8004F194_4FD94(s16);
extern void terminateTasksByTypeAndID(s32, s32);
extern void func_8002ED40_2F940(s32);
extern void func_8004FF60_50B60(s32);
extern void func_8004F820_50420(void);
extern void func_8004EDCC_4F9CC(s32);
extern s32 getFreeNodeCount(s32);
extern s8 D_800AFCE2_A7052;
extern s32 D_80090460_91060[];
extern s32 D_800904A0_910A0[];
extern s32 D_800904E0_910E0[];

void func_80040420_41020(void);
void func_800B9AE0(Player *);
void func_800407B4_413B4(void);
void func_8003EDF8_3F9F8(void);
void func_8003EEEC_3FAEC(void);
void func_8003EFDC_3FBDC(void);
void func_8003F0AC_3FCAC(void);
void func_8003FB90_40790(void);
void func_800401A0_40DA0(void);
void func_8003F178_3FD78(void);
void func_8005011C_50D1C(void);
void func_8003EE50_3FA50(void);
void func_8003F368_3FF68(void);
void func_8003FD3C_4093C(void);
void func_8003FF78_40B78(void);
void func_8003FCD8_408D8(void);
void func_8003FD84_40984(void);
void func_8003FF14_40B14(void);
void func_8003FFC0_40BC0(void);
void func_800B99E0(void *);
void func_80040304_40F04(void);
void func_8003D0C8_3DCC8(void);
void func_8006FEBC_70ABC(s32);
void func_8004013C_40D3C(void);
void func_800401E8_40DE8(void);

INCLUDE_ASM("asm/nonmatchings/3E160", func_8003D560_3E160);

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
