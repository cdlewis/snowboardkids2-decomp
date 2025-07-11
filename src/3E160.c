#include "56910.h"
#include "6E840.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "task_scheduler.h"

extern s32 D_34CB50;
extern s32 D_34F7E0;
extern s32 D_3FF010;
extern s32 D_3FF910;
extern s32 D_800AFF10_A7280;
extern u8 D_800A24A0_A30A0;
extern void func_8003EDF8_3F9F8();
extern void func_8003EEEC_3FAEC();
extern void func_8003FB90_40790(void);
extern void func_800401A0_40DA0();
extern void func_800404A8_410A8();
extern void func_80040588_41188;
extern void func_80040608_41208();
extern void func_8004D9D0_4E5D0();
extern void func_8004F1D4_4FDD4();
extern void func_800574A0_580A0(s32);
extern void func_80057564_58164(s32);
extern s32 func_80055D10_56910(u8);

s32 func_80055E7C_56A7C();
s32 func_80055EA4_56AA4();
void func_8003EE50_3FA50();
void func_8003F368_3FF68();
void func_8003FD3C_4093C();
void func_800574A0_580A0(s32);
void func_80057564_58164(s32);
void func_800697F4_6A3F4(u8);
void func_8006983C_6A43C(void *);
void func_800B99E0(void *);

INCLUDE_RODATA("asm/nonmatchings/3E160", jtbl_8009E4C8_9F0C8);

INCLUDE_RODATA("asm/nonmatchings/3E160", jtbl_8009E5E8_9F1E8);

INCLUDE_ASM("asm/nonmatchings/3E160", func_8003D560_3E160);

INCLUDE_ASM("asm/nonmatchings/3E160", func_8003E4F0_3F0F0);

void func_8003EDA0_3F9A0(void) {
    GameState *temp_v0;

    temp_v0 = (GameState *)getCurrentAllocation();
    temp_v0->gameDataStart = func_80055D34_56934(temp_v0->memoryPoolId);
    temp_v0->unk28 = dmaRequestAndUpdateStateWithSize(&D_3FF010, &D_3FF910, 0x16E0);
    func_8006983C_6A43C(&func_8003EDF8_3F9F8);
}

void func_8003EDF8_3F9F8() {
    GameState *gs = (GameState *)getCurrentAllocation();

    parseGameDataLayout(&gs->gameDataStart);

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
    gs->unk18 = dmaRequestAndUpdateStateWithSize(&D_34CB50, &D_34F7E0, 0x5E28);
    gs->unk1C = func_80055E7C_56A7C();
    gs->unk20 = func_80055EA4_56AA4();
    func_8006983C_6A43C(&func_8003EEEC_3FAEC);
}

INCLUDE_ASM("asm/nonmatchings/3E160", func_8003EEEC_3FAEC);

INCLUDE_ASM("asm/nonmatchings/3E160", func_8003EFDC_3FBDC);

INCLUDE_ASM("asm/nonmatchings/3E160", func_8003F0AC_3FCAC);

void func_8003F178_3FD78(void) {
    GameState *state = (GameState *)getCurrentAllocation();

    if ((state->unk79 == 0) || (state->unk7A == 0xB)) {
        s32 ret = func_80055D10_56910(state->memoryPoolId);

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

INCLUDE_ASM("asm/nonmatchings/3E160", func_8003FA78_40678);

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

INCLUDE_ASM("asm/nonmatchings/3E160", func_8003FBE0_407E0);

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
    if (D_800AFF10_A7280 & 0x8000) {
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

INCLUDE_ASM("asm/nonmatchings/3E160", func_8003FDD4_409D4);

void func_800574A0_580A0(s32);
void setGameStateHandler(void *);
extern void func_8003FF78_40B78;

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

INCLUDE_ASM("asm/nonmatchings/3E160", func_8003FF78_40B78);

void func_8003FFC0_40BC0() {
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

INCLUDE_ASM("asm/nonmatchings/3E160", func_80040010_40C10);

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

void func_800401A0_40DA0() {
    if (D_800AFF10_A7280 & 0x8000) {
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

INCLUDE_ASM("asm/nonmatchings/3E160", func_80040238_40E38);

INCLUDE_ASM("asm/nonmatchings/3E160", func_80040304_40F04);

void func_80040420_41020(void) {
    if (D_800AFF10_A7280 & 0x8000) {
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

INCLUDE_ASM("asm/nonmatchings/3E160", func_800404A8_410A8);

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

INCLUDE_ASM("asm/nonmatchings/3E160", func_80040588_41188);

INCLUDE_ASM("asm/nonmatchings/3E160", func_80040608_41208);

void func_800407B4_413B4(void) {
    func_800697F4_6A3F4(D_800A24A0_A30A0);
}
