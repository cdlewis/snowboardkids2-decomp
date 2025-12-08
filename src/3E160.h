#pragma once

#include "common.h"

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
void func_8001E5C0_1F1C0(void);
void func_80035260_35E60(s32 arg0, s32 arg1, s16 arg2, s16 arg3, u8 arg4, u8 arg5, u8 arg6, u8 arg7, u8 arg8);
void func_8002529C_25E9C(void *arg0);
void func_80028AEC_296EC(void *arg0);
void func_8002AE80_2BA80(void *arg0);
void func_8002B248_2BE48(void *arg0);
s32 func_8002A390_2AF90(void *arg0);
void func_80066444_67044(s32 arg0, void *arg1);
void func_8006BEDC_6CADC(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
void func_80057ABC_586BC(s32 arg0, s32 arg1);
void func_800B4B30_1E1BE0(s16 arg0, s16 arg1, s16 arg2, s16 arg3);
void func_80041518_42118(void *arg0);
void func_800497FC_4A3FC(s32 poolId);
s32 func_80037FE0_38BE0(u8 arg0);
u8 func_80038000_38C00(u8 arg0);
u8 func_80038058_38C58(u8 arg0);
s32 func_80038070_38C70(void);
s16 func_80069810_6A410(void);
void loadOverlay_1BBA0(void);
void func_8005100C_51C0C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_8003BD60_3C960(char *arg0, s16 arg1, s16 arg2, s16 arg3, void *arg4, s16 arg5, s16 arg6);
