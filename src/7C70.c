#include "common.h"

extern void func_80009F5C_AB5C(s32 *);
extern void func_80069CF8_6A8F8(void);
extern void func_80009E68_AA68(void *, s32);
extern void func_80009F90_AB90(void *, s32, s32, s32);
extern void setCleanupCallback(void *);
extern void setCallback(void *);
extern u8 randA(void);
extern void *scheduleTask(void *, u8, u8, u8);

extern s32 D_8008C190_8CD90[];
extern s32 D_8008C194_8CD94[];
extern s32 D_8008C1F0_8CDF0;

extern void transformVector2(s32 *matrix, s32 *vector, s32 *output);
extern s32 func_8000A030_AC30(void *, s32);
extern void
func_8000A190_AD90(s32 *ptr, u16 arg1, s32 x, s32 y, s32 z, s32 scaleX, s32 scaleY, u8 arg7, u8 arg8, u8 arg9);

typedef struct {
    u8 _pad[0x16];
    u16 unk16;
} func_800071E4_unk10;

typedef struct {
    u8 _pad[0x10];
    func_800071E4_unk10 *unk10;
    u8 _pad2[0x4];
    s32 unk18[3];
    u8 _pad3[0x8];
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    u8 _pad4[0x4];
    s8 unk3C;
    u8 _pad5[0x2];
    s8 unk3F;
    u8 _pad6[0x48];
    s8 unk88;
} func_800071E4_inner;

typedef struct {
    func_800071E4_inner *unk0;
    s32 unk4;
    u8 _pad8[0x48];
    s32 unk50;
    s16 unk54;
    s8 unk56;
} func_80007130_7D30_arg;

typedef struct {
    func_800071E4_inner *unk0;
} func_80007100_7D00_arg;

typedef struct {
    s32 unk0;
    s32 unk4;
} func_80007070_7C70_arg;

void func_800071E4_7DE4(func_80007130_7D30_arg *);
void func_80007360_7F60(func_80007130_7D30_arg *);
void func_80007100_7D00(func_80007100_7D00_arg *);
void func_80007130_7D30(func_80007130_7D30_arg *);
void func_8000714C_7D4C(func_80007130_7D30_arg *);

void func_80007070_7C70(func_80007070_7C70_arg *arg0) {
    s32 i;
    func_80007130_7D30_arg *task;
    s32 temp;

    func_80009E68_AA68(&arg0->unk4, 5);

    for (i = 0; i < 12; i++) {
        task = scheduleTask(func_8000714C_7D4C, 0, 0, 0);
        if (task != NULL) {
            temp = arg0->unk0;
            task->unk50 = i;
            task->unk0 = (func_800071E4_inner *)temp;
        }
    }

    setCleanupCallback(func_80007130_7D30);
    setCallback(func_80007100_7D00);
}

void func_80007100_7D00(func_80007100_7D00_arg *arg0) {
    if (arg0->unk0->unk3C == 1) {
        func_80069CF8_6A8F8();
    }
}

void func_80007130_7D30(func_80007130_7D30_arg *arg0) {
    func_80009F5C_AB5C(&arg0->unk4);
}

void func_8000714C_7D4C(func_80007130_7D30_arg *arg0) {
    s32 *temp_s0;

    temp_s0 = &arg0->unk4;
    func_80009E68_AA68(temp_s0, 5);
    func_80009F90_AB90(temp_s0, 0x10000, 0, -1);
    arg0->unk56 = 0;
    arg0->unk54 = randA() % 15;
    setCleanupCallback(func_80007360_7F60);
    setCallback(func_800071E4_7DE4);
}

void func_800071E4_7DE4(func_80007130_7D30_arg *arg0) {
    s32 matrix[3];
    s32 output[3];
    s32 x, y, z;

    if (arg0->unk0->unk3C == 1) {
        func_80069CF8_6A8F8();
        return;
    }

    if (arg0->unk56 == 0) {
        if (arg0->unk54 == 0) {
            arg0->unk56 = 1;
            return;
        }
        arg0->unk54 = arg0->unk54 - 1;
        return;
    }

    if (arg0->unk50 >= D_8008C1F0_8CDF0) {
        return;
    }

    matrix[0] = D_8008C194_8CD94[arg0->unk50 * 2];
    matrix[1] = 0x16147A;
    matrix[2] = D_8008C190_8CD90[arg0->unk50 * 2];

    transformVector2(matrix, &arg0->unk0->unk18[0], output);

    x = arg0->unk0->unk2C + output[0];
    y = arg0->unk0->unk30 + output[1];
    z = arg0->unk0->unk34 + output[2];

    func_8000A030_AC30(&arg0->unk4, 0x10000);

    if (arg0->unk0->unk88 == 0) {
        return;
    }

    if (arg0->unk0->unk3F == 0) {
        return;
    }

    func_8000A190_AD90(&arg0->unk4, arg0->unk0->unk10->unk16, x, y, z, 0x4000, 0x4000, 0, 0, 0xAA);
}

void func_80007360_7F60(func_80007130_7D30_arg *arg0) {
    func_80009F5C_AB5C(&arg0->unk4);
}
