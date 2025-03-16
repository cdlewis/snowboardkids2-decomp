#include "common.h"
#include "gamestate.h"

GameState *GameStateGet();
void func_80000460_1060(s32, void *, void *);
void func_8000056C_116C(s32, s32, GameState *);
void func_80069CC0_6A8C0(void *);
void func_80069CE8_6A8E8(void *);
void func_80000710_1310();
void func_80069CC0_6A8C0(void *);

extern void func_8001730C_17F0C();
extern void func_800172DC_17EDC();
extern void func_80017328_17F28();
extern void func_80000760_1360();

INCLUDE_ASM("asm/nonmatchings/17AA0", func_80016EA0_17AA0);

INCLUDE_ASM("asm/nonmatchings/17AA0", func_80017110_17D10);

INCLUDE_ASM("asm/nonmatchings/17AA0", func_8001716C_17D6C);

INCLUDE_ASM("asm/nonmatchings/17AA0", func_80017248_17E48);

INCLUDE_ASM("asm/nonmatchings/17AA0", func_80017264_17E64);

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
