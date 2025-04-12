#include "69EF0.h"
#include "6E840.h"
#include "common.h"
#include "gamestate.h"
#include "overlay.h"

USE_OVERLAY(_1DD170)

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
extern s32 D_8008BF98_8CB98;
extern s16 D_8008BF9A_8CB9A;
extern s32 D_8008BF9C_8CB9C;
extern s16 D_8008BF9E_8CB9E;

extern void func_800697F4_6A3F4(s32);
extern void func_800693C4_69FC4(void *, s32);
extern void func_80014480_15080();
extern GameState *GameStateGet();
extern void func_800697CC_6A3CC(void (*func)(void));
extern void n_alSynRemovePlayer(void *player);
extern s16 D_800AB070_A23E0;
extern s16 D_800AFEF0_A7260;
typedef struct
{
    s16 unk0;
    s8 unk2;
    s8 unk3;
    char padding[0xDE];
    s16 unkE2;
} func_80069854_6A454_return;
extern func_80069854_6A454_return *func_80069854_6A454(s32);
extern void func_800698EC_6A4EC(s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_8006BEDC_6CADC(void *, s32, s32, s32, s32, s32, s32);
extern void func_8006F994_70594(void *, s32, s32, s32, s32, s32, s32);
extern void func_8006F9BC_705BC(void *, f32, f32);
extern void func_8006FA0C_7060C(void *, f32, f32, f32, f32);
extern void func_8006FAA4_706A4(void *, s32, s32, s32, s32);
extern void func_8006FD3C_7093C(u16, void *);
extern void func_8006FE28_70A28(void *, s32, s32, s32);
extern void func_8006FEF8_70AF8(void *, s32);
extern void func_80069470_6A070(void *, s32);

void loadOverlay_1DD170();
void func_8000378C_438C();
void func_80003C88_4888();
void func_8006982C_6A42C(void *);
void func_80003D30_4930();
void func_80003CC4_48C4();
extern void *func_800B00C0_9FF70();
void func_80003508_4108();

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

void loadOverlay_1DD170(void) {
    LOAD_OVERLAY(_1DD170)
    func_8006982C_6A42C(&func_80003508_4108);
}

void func_80003508_4108() {
    u8 *new_var3;
    void *s0;
    f32 f20 = 1.0f;
    u8 temp[0x20];

    struct {
        func_80069854_6A454_return *s1;
    } s;
    s.s1 = func_80069854_6A454(0x17E8);
    s.s1->unk2 = 0;
    s.s1->unk0 = 0;
    s.s1->unk3 = 0;
    func_800698EC_6A4EC(0x40, 0x30, 4, 4, 4, 0, 0, 0);

    s0 = ((u8 *)s.s1) + 8;
    func_8006FAA4_706A4(s0, 0, 0, 10, f20);
    func_8006F9BC_705BC(s0, f20, f20);
    func_8006FEF8_70AF8(s0, 1);
    func_8006F994_70594(s0, 0, 0, -160, -120, 159, 119);
    func_8006FA0C_7060C(s0, 40.0f, 1.3333334f, 10.0f, 10000.0f);
    func_8006BEDC_6CADC(temp, 0, 0, 0x01000000, 0, 0, 0);

    new_var3 = ((u8 *)s.s1) + 0x1E0;
    func_8006FD3C_7093C(s.s1->unkE2, temp);
    func_8006FDA0_709A0(NULL, 0, 0);

    s0 = new_var3;
    func_8006FAA4_706A4(s0, 0, 1, 11, 0);
    func_8006F9BC_705BC(s0, f20, f20);

    func_8006FEF8_70AF8(s0, 1);
    func_8006F994_70594(s0, 0, 0, -160, -120, 159, 119);
    func_8006FE28_70A28(s0, 0, 0, 0);
    func_8006FDA0_709A0(s0, 0xFF, 0);
    func_8006FDA0_709A0(s0, 0, 8);

    s.s1 = (func_80069854_6A454_return *)(((u8 *)s.s1) + 0x3B8);
    func_8006FAA4_706A4(s.s1, 0, 3, 8, 0);
    func_8006F9BC_705BC(s.s1, f20, f20);
    func_8006FEF8_70AF8(s.s1, 1);
    func_8006F994_70594(s.s1, 0, 0, -160, -120, 159, 119);
    func_8006FE28_70A28(s.s1, 0, 0, 0);
    func_8006FDA0_709A0(s.s1, 0xFF, 0);
    func_8006FDA0_709A0(s.s1, 0, 8);
    func_8006982C_6A42C(func_8000378C_438C);
}

INCLUDE_ASM("asm/nonmatchings/4050", func_8000378C_438C);

INCLUDE_ASM("asm/nonmatchings/4050", func_80003898_4498);

void func_80003C34_4834(void) {
    GameState *temp_v0;

    temp_v0 = GameStateGet();
    if (temp_v0->unk17E0 != 0) {
        n_alSynRemovePlayer(&temp_v0->audioPlayer5);
        n_alSynRemovePlayer(&temp_v0->audioPlayer3);
        n_alSynRemovePlayer(&temp_v0->audioPlayer1);
        func_800697CC_6A3CC(&func_80003C88_4888);
    }
}

void func_80003C88_4888() {
    func_800697F4_6A3F4(1);
}

void func_80003CA4_48A4() {
    func_8006982C_6A42C(&func_80003CC4_48C4);
}

void func_80003CC4_48C4() {
    if ((D_8008BF98_8CB98 == 5) && (D_8008BF9C_8CB9C == 0)) {
        D_8008BF9C_8CB9C = 1;
    }
    func_80003450_4050(D_8008BF9A_8CB9A, D_8008BF9E_8CB9E);
    func_80069470_6A070(&loadOverlay_1DD170, 0x64);
    func_8006982C_6A42C(&func_80003D30_4930);
}

INCLUDE_ASM("asm/nonmatchings/4050", func_80003D30_4930);

void func_80003E10_4A10(void) {
    func_800693C4_69FC4(&func_80014480_15080, 0xC8);
}
