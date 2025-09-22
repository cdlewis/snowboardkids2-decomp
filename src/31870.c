#include "20F0.h"
#include "6E840.h"
#include "common.h"
#include "gamestate.h"
#include "geometry.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_419C60);
USE_ASSET(_41A1D0);

typedef struct {
    u8 padding[0x7A4];
    u8 unk7A4;
} func_8003316C_33D6C_alloc;

typedef struct {
    func_80002040_2C40_arg *unk0;
    func_8000153C_213C_arg1 *unk4;
    u8 padding[0x1B];
    s16 unk24;
} func_80031ABC_326BC_arg;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
} func_80032244_32E44_arg;

typedef struct {
    s16 unk0;
    u16 unk2;
    void *unk4;
    s16 unk8;
    s16 unkA;
    s8 unkC;
    s8 unkD;
    s16 unkE;
} func_800330B4_33CB4_arg;

typedef struct {
    void *unk0;
    s32 unk4;
    u8 padding[0x10];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s16 unk24;
    s16 unk26;
    s8 unk28;
} func_80031A0C_3260C_arg;

typedef struct {
    u8 padding[0x8];
    void *unk8;
} func_80033088_33C88_arg;

typedef struct {
    s16 unk0;
    s32 unk4;
} func_80033014_33C14_arg;

typedef struct {
    s16 unk0;
    s16 unk2;
    s32 *unk4;
    s32 unk8;
    s16 unkC;
    s16 unkE;
    s8 unk10;
} func_80032F90_33B90_arg;

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 padding[0x80];
    u8 unk84;
} func_80032EDC_33ADC_arg;

typedef struct {
    u8 padding[0x79C];
    u8 unk79C;
} func_80032EDC_33ADC_asset;

extern void func_80012004_12C04(void);
extern void func_80035408_36008(void);
extern s32 func_80035F80_36B80(s32);
extern void func_80069CF8_6A8F8(void);
extern void func_800136E0_142E0(void);

void func_80031C4C_3284C(void);
void func_80031ABC_326BC(func_80031ABC_326BC_arg *arg0);
void func_80031B30_32730(void);
void func_800322BC_32EBC(void);
void func_80032304_32F04(void);
void func_8003316C_33D6C(func_800330B4_33CB4_arg *arg0);
void func_800331CC_33DCC(func_800330B4_33CB4_arg *arg0);
void func_80033014_33C14(func_80033014_33C14_arg *arg0);
void func_80033088_33C88(func_80033088_33C88_arg *arg0);

extern s32 D_8008F18C_8FD8C;
extern s32 D_8008F200_8FE00[];
extern u16 D_8008F20A_8FE0A[];
extern u16 D_8008F20C_8FE0C;
extern s32 identityMatrix;

INCLUDE_ASM("asm/nonmatchings/31870", func_80030C70_31870);

INCLUDE_ASM("asm/nonmatchings/31870", func_80030E54_31A54);

INCLUDE_ASM("asm/nonmatchings/31870", func_80030EAC_31AAC);

INCLUDE_ASM("asm/nonmatchings/31870", func_80030F6C_31B6C);

INCLUDE_ASM("asm/nonmatchings/31870", func_80030FBC_31BBC);

INCLUDE_ASM("asm/nonmatchings/31870", func_80031100_31D00);

INCLUDE_ASM("asm/nonmatchings/31870", func_80031138_31D38);

INCLUDE_ASM("asm/nonmatchings/31870", func_80031248_31E48);

INCLUDE_ASM("asm/nonmatchings/31870", func_80031288_31E88);

INCLUDE_ASM("asm/nonmatchings/31870", func_800313A4_31FA4);

INCLUDE_ASM("asm/nonmatchings/31870", func_800313DC_31FDC);

INCLUDE_ASM("asm/nonmatchings/31870", func_80031458_32058);

INCLUDE_ASM("asm/nonmatchings/31870", func_80031510_32110);

INCLUDE_ASM("asm/nonmatchings/31870", func_800315C0_321C0);

INCLUDE_ASM("asm/nonmatchings/31870", func_8003165C_3225C);

INCLUDE_ASM("asm/nonmatchings/31870", func_800316AC_322AC);

INCLUDE_ASM("asm/nonmatchings/31870", func_80031758_32358);

INCLUDE_ASM("asm/nonmatchings/31870", func_800317D4_323D4);

INCLUDE_ASM("asm/nonmatchings/31870", func_80031818_32418);

INCLUDE_ASM("asm/nonmatchings/31870", func_80031944_32544);

INCLUDE_ASM("asm/nonmatchings/31870", func_800319C8_325C8);

void func_80031A0C_3260C(func_80031A0C_3260C_arg *arg0) {
    arg0->unk0 = func_800019B8_25B8(0x3A, &((GameState *)getCurrentAllocation())->audioPlayer2, 0, -1, 0, 0x12);

    memcpy(&arg0->unk4, &identityMatrix, 0x20);

    arg0->unk18 = 0xFFE70000;
    arg0->unk1C = 0xFFE00000;
    arg0->unk20 = 0;

    createYRotationMatrix((s16(*)[3]) & arg0->unk4, 0x200);

    arg0->unk24 = 0x10;
    arg0->unk26 = 0x10;
    arg0->unk28 = 0;

    setCleanupCallback(&func_80031C4C_3284C);
    setCallback(&func_80031ABC_326BC);
}

void func_80031ABC_326BC(func_80031ABC_326BC_arg *arg0) {
    GameState *temp_s1;

    temp_s1 = (GameState *)getCurrentAllocation();
    func_8000153C_213C(arg0->unk0, &arg0->unk4);
    func_8000160C_220C(arg0->unk0, 1);
    func_800021B8_2DB8(arg0->unk0, arg0->unk24);
    func_80002750_3350(arg0->unk0);
    if (temp_s1->unk79B != 0) {
        setCallback(&func_80031B30_32730);
    }
}

INCLUDE_ASM("asm/nonmatchings/31870", func_80031B30_32730);

INCLUDE_ASM("asm/nonmatchings/31870", func_80031C4C_3284C);

INCLUDE_ASM("asm/nonmatchings/31870", func_80031C68_32868);

INCLUDE_ASM("asm/nonmatchings/31870", func_80031CC0_328C0);

INCLUDE_ASM("asm/nonmatchings/31870", func_80031CE8_328E8);

INCLUDE_ASM("asm/nonmatchings/31870", func_80031D14_32914);

INCLUDE_ASM("asm/nonmatchings/31870", func_80031D40_32940);

INCLUDE_ASM("asm/nonmatchings/31870", func_80031DE4_329E4);

INCLUDE_ASM("asm/nonmatchings/31870", func_80031F68_32B68);

INCLUDE_ASM("asm/nonmatchings/31870", func_80031F94_32B94);

INCLUDE_ASM("asm/nonmatchings/31870", func_80032020_32C20);

INCLUDE_ASM("asm/nonmatchings/31870", func_800320B8_32CB8);

INCLUDE_ASM("asm/nonmatchings/31870", func_800320E4_32CE4);

INCLUDE_ASM("asm/nonmatchings/31870", func_80032170_32D70);

INCLUDE_ASM("asm/nonmatchings/31870", func_80032218_32E18);

void func_80032244_32E44(func_80032244_32E44_arg *arg0) {
    void *asset = dmaRequestAndUpdateStateWithSize(&_41A1D0_ROM_START, &_41A1D0_ROM_END, 0x1B48);
    setCleanupCallback(&func_80032304_32F04);
    arg0->unk0 = -0x2C;
    arg0->unk2 = -0x14;
    arg0->unk8 = 0xD;
    arg0->unk4 = asset;
    setCallback(&func_800322BC_32EBC);
}

INCLUDE_ASM("asm/nonmatchings/31870", func_800322BC_32EBC);

INCLUDE_ASM("asm/nonmatchings/31870", func_80032304_32F04);

INCLUDE_ASM("asm/nonmatchings/31870", func_80032330_32F30);

INCLUDE_ASM("asm/nonmatchings/31870", func_800323FC_32FFC);

INCLUDE_ASM("asm/nonmatchings/31870", func_80032504_33104);

INCLUDE_ASM("asm/nonmatchings/31870", func_8003253C_3313C);

INCLUDE_ASM("asm/nonmatchings/31870", func_80032628_33228);

INCLUDE_ASM("asm/nonmatchings/31870", func_80032708_33308);

INCLUDE_ASM("asm/nonmatchings/31870", func_800329A8_335A8);

INCLUDE_ASM("asm/nonmatchings/31870", func_80032B0C_3370C);

INCLUDE_ASM("asm/nonmatchings/31870", func_80032BCC_337CC);

INCLUDE_ASM("asm/nonmatchings/31870", func_80032CB4_338B4);

INCLUDE_ASM("asm/nonmatchings/31870", func_80032DBC_339BC);

INCLUDE_ASM("asm/nonmatchings/31870", func_80032DE8_339E8);

INCLUDE_ASM("asm/nonmatchings/31870", func_80032EA4_33AA4);

void func_80032EDC_33ADC(func_80032EDC_33ADC_arg *arg0) {
    func_80032EDC_33ADC_asset *temp_v0 = (func_80032EDC_33ADC_asset *)getCurrentAllocation();

    arg0->unk84++;
    if (temp_v0->unk79C == 1) {
        arg0->unk2 = arg0->unk2 - 0x14;
    } else {
        arg0->unk2 = arg0->unk2 + 0x14;
    }

    debugEnqueueCallback(8, 0, &func_800136E0_142E0, arg0);

    if (arg0->unk84 == 4) {
        func_80069CF8_6A8F8();
    }
}

void func_80032F64_33B64(func_80032244_32E44_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_80032F90_33B90(func_80032F90_33B90_arg *arg0) {
    s32 temp_s1 = func_80035F80_36B80(1);
    setCleanupCallback(&func_80033088_33C88);
    arg0->unk2 = -0x60;
    arg0->unk4 = &D_8008F18C_8FD8C;
    arg0->unkC = 0xFF;
    arg0->unkE = 0xFF;
    arg0->unk8 = temp_s1;
    arg0->unk0 = D_8008F20C_8FE0C;
    arg0->unk10 = 5;
    setCallback(&func_80033014_33C14);
}

void func_80033014_33C14(func_80033014_33C14_arg *arg0) {
    func_8003316C_33D6C_alloc *asset = (func_8003316C_33D6C_alloc *)getCurrentAllocation();
    if (asset->unk7A4 != 0) {
        arg0->unk0 = D_8008F20A_8FE0A[asset->unk7A4];
        arg0->unk4 = D_8008F200_8FE00[asset->unk7A4];
        debugEnqueueCallback(9, 7, &func_80035408_36008, arg0);
    }
}

void func_80033088_33C88(func_80033088_33C88_arg *arg0) {
    arg0->unk8 = freeNodeMemory(arg0->unk8);
}

void func_800330B4_33CB4(func_800330B4_33CB4_arg *arg0) {
    s32 i;
    void *asset = dmaRequestAndUpdateStateWithSize(&_419C60_ROM_START, &_419C60_ROM_END, 0x1548);
    setCleanupCallback(&func_800331CC_33DCC);

    for (i = 0; i < 4; i++) {
        func_800330B4_33CB4_arg *j = &arg0[i];

        if (i % 2 != 0) {
            j->unk0 = -0x80;
        } else {
            j->unk0 = 0;
        }

        j->unk2 = (s16)(((i / 2) * 0x10) - 0x66);
        j->unk4 = asset;
        j->unk8 = i;
        j->unkA = 0xFF;
        j->unkD = 0;
        j->unkC = 1;
    }

    setCallback(&func_8003316C_33D6C);
}

void func_8003316C_33D6C(func_800330B4_33CB4_arg *arg0) {
    s32 i;

    if (((func_8003316C_33D6C_alloc *)getCurrentAllocation())->unk7A4 != NULL) {
        for (i = 0; i < 4; i++) {
            debugEnqueueCallback(9, 1, &func_80012004_12C04, &arg0[i]);
        }
    }
}

void func_800331CC_33DCC(func_800330B4_33CB4_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}
