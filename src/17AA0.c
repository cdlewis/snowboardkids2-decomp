#include "common.h"
#include "gamestate.h"
#include "6E840.h"

GameState *GameStateGet();
void func_80000460_1060(s32, void *, void *);
void func_8000056C_116C(s32, s32, GameState *);
void func_80069CC0_6A8C0(void *);
void func_80069CE8_6A8E8(void *);
void func_80000710_1310();
void func_80069CC0_6A8C0(void *);
s32 func_8003BB5C_3C75C();
void func_8006982C_6A42C(void *);
extern void func_8001716C_17D6C();
extern void func_800697F4_6A3F4(s32);
extern void func_8001730C_17F0C();
extern void func_800172DC_17EDC();
extern void func_80017328_17F28();
extern void func_80000760_1360();
extern void func_80069DD4_6A9D4(s32 a0, s32 a1);
extern void func_80069D7C_6A97C(s32 a0);
extern void func_80057564_58164(s32 a0);
extern void n_alSynRemovePlayer(void *player);
extern void *func_8006A200_6AE00(void *ptr);
extern void func_800697CC_6A3CC(void (*func)(void));
extern void func_80017248_17E48(void);
extern void func_80017264_17E64(void);
extern u8 D_800A8CC8_A0038;

INCLUDE_ASM("asm/nonmatchings/17AA0", func_80016EA0_17AA0);

void func_80017110_17D10(void) {
    GameState *gameState = GameStateGet();

    if (gameState->unk429 != 0) {
        gameState->unk429--;
    } else {
        if (func_8003BB5C_3C75C() == 0) {
            func_8006FDA0_709A0(NULL, 0, 0x10);
            func_8006982C_6A42C(func_8001716C_17D6C);
        }
    }
}

void func_8001716C_17D6C(void) {
    GameState *gs = GameStateGet();

    u8 val = gs->unk42E;
    if (val != 0) {
        if (val == 2) {
            func_80069DD4_6A9D4(2, 1);
        } else {
            func_80069D7C_6A97C(2);
        }
        gs->unk42E = 0;
    }

    if (gs->unk427 != 0) {
        func_80057564_58164(10);
        n_alSynRemovePlayer(gs);
        n_alSynRemovePlayer((void *)((u8 *)gs + 0x1D8));

        gs->unk3E4 = func_8006A200_6AE00((void *)gs->unk3E4);
        gs->unk3DC = func_8006A200_6AE00((void *)gs->unk3DC);
        gs->unk3E0 = func_8006A200_6AE00((void *)gs->unk3E0);

        if (gs->unk427 == 0xFF) {
            func_800697CC_6A3CC(func_80017264_17E64);
        } else {
            u8 tmp = gs->unk425 + 1;
            D_800A8CC8_A0038 = tmp;
            func_800697CC_6A3CC(func_80017248_17E48);
        }
    }
}

void func_80017248_17E48(void) {
    func_800697F4_6A3F4(1);
}

void func_80017264_17E64(void) {
    func_800697F4_6A3F4(0xFF);
}

void func_80017280_17E80(s32 arg0) {
    char *sp10[5];
    char *sp28;

    func_8000056C_116C(arg0, 0, GameStateGet());
    func_80069CE8_6A8E8(&func_80017328_17F28);
    func_80000460_1060(arg0, &sp10, &sp28);
    func_80069CC0_6A8C0(&func_800172DC_17EDC);
}

void func_800172DC_17EDC(void) {
    GameStateGet()->unk429 = 2;
    func_80069CC0_6A8C0(&func_8001730C_17F0C);
}

void func_8001730C_17F0C(void) {
    func_80000760_1360();
}

void func_80017328_17F28(void) {
    func_80000710_1310();
}
