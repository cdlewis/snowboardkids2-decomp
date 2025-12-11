#pragma once

#include "common.h"
#include "38310.h"
#include "displaylist.h"
#include "gamestate.h"

typedef struct {
    u8 padding[0x14];
} D_8008D7FC_8E3FC_item;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    D_8008D7FC_8E3FC_item *unk8;
} func_80019CD0_1A8D0_arg;

typedef struct {
    u8 pad0[0x18];  /* 0x00 */
    s32 unk18;      /* 0x18 */
    u8 pad1C[0x4];  /* 0x1C */
    void *unk20;    /* 0x20 */
    void *unk24;    /* 0x24 */
    void *unk28;    /* 0x28 */
    s32 unk2C;      /* 0x2C */
    u8 pad30[0x30]; /* 0x30 */
    void *unk60;    /* 0x60 */
    void *unk64;    /* 0x64 */
    s32 unk68;      /* 0x68 */
    u8 pad6C[0xC];  /* 0x6C */
    Player *unk78;  /* 0x78 */
    s32 unk7C;      /* 0x7C */
    s16 unk80;      /* 0x80 */
    s16 unk82;      /* 0x82 */
} Func43374State;

void func_80014480_15080(void);
void func_8001452C_1512C(void);
void func_80033200_33E00(void);
void initRace(void);
void func_80021BD0_227D0(void);
void func_80022130_22D30(void);
void func_800226F0_232F0(void);
void func_80018EB0_19AB0(void);
void func_80019CD0_1A8D0(func_80019CD0_1A8D0_arg *arg0);
void func_8001C920_1D520(void);
void func_80003EE0_4AE0(void);
void func_8001E590_1F190(void);
void func_8001E5C0_1F1C0(void);
void func_80035260_35E60(s32 arg0, s32 arg1, s16 arg2, s16 arg3, u8 arg4, u8 arg5, u8 arg6, u8 arg7, u8 arg8);
void func_8002529C_25E9C(void *arg0);
void func_80028AEC_296EC(void *arg0);
void func_8002AE80_2BA80(void *arg0);
void func_8002B248_2BE48(void *arg0);
s32 func_8002A390_2AF90(void *arg0);
void func_80066444_67044(s32 arg0, func_80066444_67044_arg1 *arg1);
void func_8006BEDC_6CADC(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
void func_80057ABC_586BC(s32 arg0, s32 arg1);
void func_800B4B30_1E1BE0(s16 arg0, s16 arg1, s16 arg2, s16 arg3);
void func_80037E78_38A78(Entry1 *arg0);
void func_80041518_42118(void *arg0);
void func_800497FC_4A3FC(s32 poolId);
s32 func_80037FE0_38BE0(u8 arg0);
u8 func_80038000_38C00(u8 arg0);
u8 func_80038058_38C58(u8 arg0);
s32 func_80038070_38C70(void);
void func_80038090_38C90(s16 arg0);
s16 func_80069810_6A410(void);
void loadOverlay_1BBA0(void);
void func_8005100C_51C0C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_8003BD60_3C960(char *arg0, s16 arg1, s16 arg2, s16 arg3, void *arg4, s16 arg5, s16 arg6);
void func_800585C8_591C8(s32 arg0);
void func_80058B30_59730(Player *arg0);
void func_8003B47C_3C07C(void *arg0);
void func_80044538_45138(s16 arg0);
void func_80045434_46034(s16 arg0);
void func_80041418_42018(void);
Func43374State *func_800436B4_442B4(Player *arg0);
void func_80015A18_16618(void);
void func_8000D7F8_E3F8(void);
void func_80021D20_22920(void);
void func_80037270_37E70(void);
void func_8003B1F4_3BDF4(s32 arg0, void *arg1);
void *func_8003B28C_3BE8C(void);

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    u8 unkA;
    u8 pad;
} func_80032330_32F30_element;

typedef struct {
    func_80032330_32F30_element elements[7];
    s16 unk54;
    s16 unk56;
    void *unk58;
    s16 unk5C;
    u8 unk5E;
    u8 pad;
    char unk60[8];
} func_80032330_32F30_arg;

void func_80032330_32F30(func_80032330_32F30_arg *arg0);
s32 abs(s32 n);
void func_80058A3C_5963C(Player *arg0);
