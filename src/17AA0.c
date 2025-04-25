#include "6E840.h"
#include "common.h"
#include "gamestate.h"

extern s32 D_419440;
extern s32 D_4196E0;
extern s32 D_45A890;
extern s32 D_45B130;
extern u16 D_8008D6B0_8E2B0;
extern u8 D_800A8CC8_A0038;
extern void *func_8006A200_6AE00(void *ptr);
extern void ClocktowerTriggerInit();
extern void func_80000760_1360();
extern void func_8001716C_17D6C();
extern void func_80017248_17E48(void);
extern void func_80017264_17E64(void);
extern void func_80017280_17E80(s32);
extern void func_800172DC_17EDC();
extern void func_8001730C_17F0C();
extern void func_80017328_17F28();
extern void func_80017350_17F50;
extern void func_80018800_19400;
extern void func_80018A90_19690;
extern void func_80018CB0_198B0;
extern void func_80019B70_1A770;
extern void func_80028480_29080;
extern void func_80036250_36E50;
extern void func_80036A3C_3763C;
extern void func_80057564_58164(s32 a0);
extern void func_800697CC_6A3CC(void (*func)(void));
extern void func_800697F4_6A3F4(s32);
extern void func_80069D7C_6A97C(s32 a0);
extern void func_80069DD4_6A9D4(s32 a0, s32 a1);
extern void n_alSynRemovePlayer(void *player);
extern void PhoneTriggerInit;
GameState *func_80069854_6A454(s32 arg0);
GameState *GameStateGet();
s32 func_8003BB5C_3C75C();
s8 *func_800699F4_6A5F4(void *, void *, void *, s32);
void *dmaRequestAndUpdateState(void *, void *, int);
void *func_80035F80_36B80(s32);
void *func_80069E3C_6AA3C(void *, void *);
void func_80000460_1060(s32, void *, void *);
void func_8000056C_116C(s32, s32, GameState *);
void func_80000710_1310();
void func_80027CA0_288A0(void *, s32, s32, s32);
void func_800574A0_580A0(s32);
void func_8006982C_6A42C(void *);
void func_800698EC_6A4EC(s32, s32, s32, void *, s32, s32, s32, s32);
void func_80069CC0_6A8C0(void *);
void func_80069CE8_6A8E8(void *);
void func_80017110_17D10(void);

void func_80016EA0_17AA0(s32 arg0, s32 arg1, s32 arg2) {
    int var_s1;
    GameState *temp_s0;
    volatile u8 padding[0x20];
    temp_s0 = func_80069854_6A454(0x430);
    func_800574A0_580A0(4);
    temp_s0->unk3E8 = 0;
    temp_s0->unk428 = 0;
    temp_s0->unk427 = 0;
    temp_s0->unk429 = 0x63;
    temp_s0->unk425 = 0;
    temp_s0->unk424 = 0;
    temp_s0->unk42E = 0;
    temp_s0->unk3D0 = 0;
    temp_s0->unk3D4 = 0;
    temp_s0->unk3D8 = 0;
    temp_s0->unk42A = 0;
    var_s1 = 0;
    do {
        temp_s0->unk3FE = (s16)(D_8008D6B0_8E2B0 - (((s32)((((short)D_8008D6B0_8E2B0) / 10) << 0x10)) >> 0xF));
        temp_s0->unk403 = 0xFF;
        func_800698EC_6A4EC(0x30, 0xA, 0x14, 0, 0, 0, 0, 0);
        func_80027CA0_288A0(temp_s0, 0, 0xA, 0);
        func_80027CA0_288A0(((void *)((s32)temp_s0)) + 0x1D8, 8, 0xF, 1);
        func_800699F4_6A5F4(&func_80017350_17F50, 0, 0, 0x5A);
        func_800699F4_6A5F4(&func_80018800_19400, 0, 0, 0x5B);
        func_800699F4_6A5F4(&func_80028480_29080, 0, 0, 0x5B);
        var_s1++;
        var_s1--;
        func_800699F4_6A5F4(&func_80019B70_1A770, 0, 0, 0x5C);
        func_800699F4_6A5F4(&func_80017280_17E80, 0, 0, 0x5D);
        func_800699F4_6A5F4(&func_80018A90_19690, 0, 0, 0x62);
        do {
            *func_800699F4_6A5F4(&func_80018CB0_198B0, 0, 0, 0x64) = var_s1;
            var_s1 += 1;
        } while (var_s1 < 6);
        func_800699F4_6A5F4(&PhoneTriggerInit, 0, 0, 0x64);
    } while (0);
    func_800699F4_6A5F4(&ClocktowerTriggerInit, 0, 0, 0x64);
    func_800699F4_6A5F4(&func_80036A3C_3763C, 0, 0, 0x64);
    func_800699F4_6A5F4(&func_80036250_36E50, 0, 0, 0x64);
    temp_s0->unk3E4 = func_80069E3C_6AA3C(&D_419440, &D_4196E0);
    temp_s0->unk3DC = dmaRequestAndUpdateState(&D_45A890, &D_45B130, 0x3108);
    temp_s0->unk3E0 = func_80035F80_36B80(1);
    func_8006FDA0_709A0(NULL, 0xFF, 0);
    func_8006982C_6A42C(&func_80017110_17D10);
}

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
