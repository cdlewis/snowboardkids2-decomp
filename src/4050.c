#include "69EF0.h"
#include "common.h"
#include "gamestate.h"

/*
s8 D_8008BF90_8CB90 = 0;
s8 D_8008BF92_8CB92 = 0;
s16 D_8008BF94_8CB94 = 0;
s8 D_8008BF98_8CB98 = 0;
s8 D_8008BF9A_8CB9A = 0;
s8 D_8008BF9C_8CB9C = 0;
s8 D_8008BF9E_8CB9E = 2;
*/

// not actually extern, see above;
extern s16 D_8008BF90_8CB90;
extern s16 D_8008BF92_8CB92;
extern s16 D_8008BF94_8CB94;

extern void func_800697F4_6A3F4(s32);
extern void func_80003CC4_48C4;
extern void func_800693C4_69FC4(void*, s32);
extern void func_80014480_15080();
extern GameState* GameStateGet();
extern void func_800697CC_6A3CC(void (*func)(void));
extern void n_alSynRemovePlayer(void* player);
extern s16 D_800AB070_A23E0;
extern s16 D_800AFEF0_A7260;

void func_80003C88_4888();

void func_80003450_4050(s16 arg0, s16 arg1) {
    D_800AB070_A23E0 = arg0;
    D_800AFEF0_A7260 = arg1;
}

s32 __udiv_w_sdiv(void) {
    return 0;
}

void func_8000346C_406C(s16 arg0, s16 arg1, s16 arg2) {
    D_8008BF90_8CB90 = arg0;
    D_8008BF92_8CB92 = arg1;
    D_8008BF94_8CB94 = arg2;
}

INCLUDE_ASM("asm/nonmatchings/4050", func_80003488_4088);

INCLUDE_ASM("asm/nonmatchings/4050", func_80003508_4108);

INCLUDE_ASM("asm/nonmatchings/4050", func_8000378C_438C);

INCLUDE_ASM("asm/nonmatchings/4050", func_80003898_4498);

void func_80003C34_4834(void) {
    GameState* temp_v0;

    temp_v0 = GameStateGet();
    if (temp_v0->unk17E0 != 0) {
        n_alSynRemovePlayer(&temp_v0->audioPlayerC);
        n_alSynRemovePlayer(&temp_v0->audioPlayerB);
        n_alSynRemovePlayer(&temp_v0->audioPlayerA);
        func_800697CC_6A3CC(&func_80003C88_4888);
    }
}

void func_80003C88_4888() {
    func_800697F4_6A3F4(1);
}

void func_80003CA4_48A4() {
    func_8006982C_6A42C(&func_80003CC4_48C4);
}

INCLUDE_ASM("asm/nonmatchings/4050", func_80003CC4_48C4);

INCLUDE_ASM("asm/nonmatchings/4050", func_80003D30_4930);

void func_80003E10_4A10(void) {
    func_800693C4_69FC4(&func_80014480_15080, 0xC8);
}
