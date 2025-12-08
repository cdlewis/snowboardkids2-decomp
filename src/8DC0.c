#include "90F0.h"
#include "common.h"
#include "geometry.h"
#include "task_scheduler.h"

extern s32 func_8000A030_AC30(void *, s32);
extern u8 identityMatrix[];

typedef struct {
    u8 padding[0x16];
    u16 unk16;
} InnerInner8F70;

typedef struct {
    u8 _pad0[0xC];
    s16 unkC;
    u8 _pad0E[0x2];
    InnerInner8F70 *unk10;
    u8 _pad14[0x18];
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    u8 _pad38[0x4];
    s8 unk3C;
    s8 unk3D;
    s8 unk3E;
    s8 unk3F;
    u8 _pad40[0x48];
    s8 unk88;
    u8 _pad89[0x67];
    Mat3x3Padded unkF0;
} InnerStruct8DEC;

typedef struct {
    InnerStruct8DEC *unk0;
    u16 unk4;
} func_800081EC_8DEC_arg;

void func_800081EC_8DEC(func_800081EC_8DEC_arg *arg0);
void func_80008308_8F08(void);

typedef struct {
    InnerStruct8DEC *unk0;
    func_80009F5C_AB5C_arg *unk4;
    u8 unk8[0x48];
    u8 unk50;
} Arg8F70;

void func_80008370_8F70(Arg8F70 *arg0);
void func_800084A8_90A8(Arg8F70 *arg0);

void func_800081C0_8DC0(void) {
    setCleanupCallback(func_80008308_8F08);
    setCallback(func_800081EC_8DEC);
}

void func_800081EC_8DEC(func_800081EC_8DEC_arg *arg0) {
    Mat3x3Padded local_buffer;
    s32 sinVal;
    s32 cosVal;
    s32 scaledCos;
    s32 scaledSin;

    memcpy(&local_buffer, identityMatrix, 0x20);

    if (arg0->unk0->unk3C == 1) {
        func_80069CF8_6A8F8();
        return;
    }

    memcpy(&arg0->unk0->unkF0, identityMatrix, 0x20);

    if (arg0->unk0->unk3D == 0) {
        return;
    }
    if (arg0->unk0->unk3D != 1) {
        return;
    }

    arg0->unk4 += 0x333;
    sinVal = approximateSin(arg0->unk4);
    cosVal = approximateCos(arg0->unk4);

    scaledCos = ((cosVal * 7 * 8 + cosVal) * 4 - cosVal) >> 12;
    scaledSin = ((sinVal * 7 * 8 + sinVal) * 4 - sinVal) >> 12;

    createRotationMatrixXZ(&arg0->unk0->unkF0, -scaledCos, scaledSin);

    arg0->unk0->unkF0.unk18 += 0x33333;
}

void func_80008308_8F08(void) {
}

void func_80008310_8F10(Arg8F70 *arg0) {
    arg0->unk50 = 0;
    func_80009E68_AA68(&arg0->unk4, 9);
    func_80009F90_AB90(&arg0->unk4, 0x10000, 0, -1);
    setCleanupCallback(func_800084A8_90A8);
    setCallback(func_80008370_8F70);
}

void func_80008370_8F70(Arg8F70 *arg0) {
    s8 unused[2] = { 1, -1 };
    InnerStruct8DEC *inner;
    s32 s3, s4, s2;

    inner = arg0->unk0;

    if (inner->unk3C == 1) {
        func_80069CF8_6A8F8();
        return;
    }

    switch (inner->unk3D) {
        case 0:
            arg0->unk50 = 0;
            break;
        case 1:
            func_80009F90_AB90(&arg0->unk4, 0x10000, 0, -1);
            break;
        case 2:
            func_80009F90_AB90(&arg0->unk4, 0x10000, 1, -1);
            break;
        case 3:
            arg0->unk50 = 1;
            break;
        default:
            arg0->unk50 = 0;
            break;
    }

    s3 = arg0->unk0->unk2C;
    s4 = arg0->unk0->unk30;
    s2 = arg0->unk0->unk34;

    func_8000A030_AC30(&arg0->unk4, 0x10000);

    inner = arg0->unk0;
    if (inner->unk88 != 0) {
        if (inner->unk3F != 0) {
            func_8000A13C_AD3C(&arg0->unk4, inner->unk10->unk16, s3, s4, s2, 0x13333, 0x13333, 0, arg0->unk50);
        }
    }
}

void func_800084A8_90A8(Arg8F70 *arg0) {
    func_80009F5C_AB5C(&arg0->unk4);
}
