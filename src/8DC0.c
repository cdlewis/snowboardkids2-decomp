#include "90F0.h"
#include "common.h"
#include "geometry.h"
#include "task_scheduler.h"

extern void func_80009E68_AA68(void *, s32);
extern void func_80009F90_AB90(void *, s32, s32, s32);
extern u8 identityMatrix[];
extern s32 approximateSin(s16);
extern s32 approximateCos(s16);

typedef struct {
    u8 _pad0[0x3C];
    s8 unk3C;
    s8 unk3D;
    u8 _pad3E[0xB2];
    Mat3x3Padded unkF0;
} InnerStruct8DEC;

typedef struct {
    InnerStruct8DEC *unk0;
    u16 unk4;
} func_800081EC_8DEC_arg;

void func_800081EC_8DEC(func_800081EC_8DEC_arg *arg0);
void func_80008308_8F08(void);
void func_80008370_8F70(void);

typedef struct {
    u8 unk0[0x4];
    func_80009F5C_AB5C_arg *unk4;
    u8 unk8[0x48];
    u8 unk50;
} Arg8F10;

void func_800084A8_90A8(Arg8F10 *arg0);

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

void func_80008310_8F10(Arg8F10 *arg0) {
    arg0->unk50 = 0;
    func_80009E68_AA68(&arg0->unk4, 9);
    func_80009F90_AB90(&arg0->unk4, 0x10000, 0, -1);
    setCleanupCallback(func_800084A8_90A8);
    setCallback(func_80008370_8F70);
}

INCLUDE_ASM("asm/nonmatchings/8DC0", func_80008370_8F70);

void func_800084A8_90A8(Arg8F10 *arg0) {
    func_80009F5C_AB5C(&arg0->unk4);
}
