#include "20F0.h"
#include "common.h"
#include "gamestate.h"
#include "geometry.h"
#include "task_scheduler.h"

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 padding[0x1B];
    s16 unk24;
} func_80031ABC_326BC_arg;

void func_80002750_3350(s32);
extern void func_80031B30_32730;
extern s32 D_8009A870_9B470;
extern void func_80031C4C_3284C;
void func_80031ABC_326BC(func_80031ABC_326BC_arg *arg0);
func_800021B8_2DB8(s32, s16);
func_8000160C_220C(s32, s32);
func_8000153C_213C(s32, s32 *);

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

void func_80031A0C_3260C(func_80031A0C_3260C_arg *arg0) {
    arg0->unk0 = func_800019B8_25B8(0x3A, &((GameState *)getCurrentAllocation())->audioPlayer2, 0, -1, 0, 0x12);

    memcpy(&arg0->unk4, &D_8009A870_9B470, 0x20);

    arg0->unk18 = 0xFFE70000;
    arg0->unk1C = 0xFFE00000;
    arg0->unk20 = 0;

    createYRotationMatrix((Mat3x3 *)&arg0->unk4, 0x200);

    arg0->unk24 = 0x10;
    arg0->unk26 = 0x10;
    arg0->unk28 = 0;

    setCleanupCallback(&func_80031C4C_3284C);
    setCallback((void (*)(void *)) & func_80031ABC_326BC);
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

INCLUDE_ASM("asm/nonmatchings/31870", func_80032244_32E44);

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

INCLUDE_ASM("asm/nonmatchings/31870", func_80032EDC_33ADC);

INCLUDE_ASM("asm/nonmatchings/31870", func_80032F64_33B64);

INCLUDE_ASM("asm/nonmatchings/31870", func_80032F90_33B90);

INCLUDE_ASM("asm/nonmatchings/31870", func_80033014_33C14);

INCLUDE_ASM("asm/nonmatchings/31870", func_80033088_33C88);

INCLUDE_ASM("asm/nonmatchings/31870", func_800330B4_33CB4);

INCLUDE_ASM("asm/nonmatchings/31870", func_8003316C_33D6C);

INCLUDE_ASM("asm/nonmatchings/31870", func_800331CC_33DCC);
