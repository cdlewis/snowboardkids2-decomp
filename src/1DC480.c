#include "1DC480.h"
#include "6E840.h"
#include "B040.h"
#include "common.h"
#include "race_session.h"

extern s32 func_800B0DC0_1DCF60(s32);
extern s16 func_800B0DD0_1DCF70(s32, s32);
extern u8 *func_800B0E24_1DCFC4(s32, s16);
extern s16 func_800B0E80_1DD020(s32, s16);

typedef union {
    s16 halfword;
    struct {
        u8 high;
        u8 low;
    } bytes;
} HalfwordBytes_1DC480;

typedef union {
    s32 word;
    struct {
        s16 high;
        s16 low;
    } halves;
} WordHalves_1DC480;

typedef struct {
    u8 _pad0[0x2];
    s16 unk2;
    u8 _pad4[0x3B4];
    u8 unk3B8[0x1D8];
    u8 unk590[0x3CC];
    s32 unk95C;
    void *unk960;
    Table_B934 *unk964;
    u8 _pad968[0x4];
    s16 unk96C;
    s16 unk96E;
    u16 *unk970;
    void *unk974;
    u8 unk978;
    u8 unk979;
    HalfwordBytes_1DC480 unk97A;
    u8 unk97C;
    u8 _pad97D[0x3];
    s16 unk980;
    s16 unk982;
    u16 *unk984;
    void *unk988;
    u8 unk98C;
    u8 unk98D;
    HalfwordBytes_1DC480 unk98E;
    u8 unk990;
    u8 _pad991[0x3];
    s32 unk994;
    s32 unk998;
    WordHalves_1DC480 unk99C;
    s16 unk9A0;
    u8 _pad9A2[0x2];
    s16 unk9A4;
    u16 unk9A6;
    u16 unk9A8;
    u8 _pad9AA[0x2];
    s16 unk9AC;
    s16 unk9AE;
    s16 unk9B0;
    s16 unk9B2;
    s16 unk9B4;
} Struct_800B0388;

typedef struct {
    u8 _pad960[0x960];
    void *unk960;
    void *unk964;
    u8 _pad968[0x4];
    s16 unk96C;
    s16 unk96E;
    u8 _pad970[0x4];
    void *unk974;
    s16 unk978;
    s16 unk97A;
    u8 unk97C;
    u8 _pad97D[0x3];
    s16 unk980;
    s16 unk982;
    void *unk984;
    void *unk988;
    s16 unk98C;
    s16 unk98E;
    u8 unk990;
    u8 _pad991[0x3];
    void *unk994;
    void *unk998;
    void *unk99C;
    s16 unk9A0;
    u8 _pad9A2[0x2];
    s16 unk9A4;
    s16 unk9A6;
    u8 _pad9A8[0x4];
    s16 unk9AC;
    s16 unk9AE;
    s16 unk9B0;
    s16 unk9B2;
} func_800B02E0_arg;

void func_800B02E0(void *arg0) {
    func_800B02E0_arg *typedArg = (func_800B02E0_arg *)arg0;
    void *temp;

    typedArg->unk9A0 = -1;
    temp = typedArg->unk960;
    typedArg->unk96C = 0;
    typedArg->unk96E = 0;
    typedArg->unk978 = 0xFF;
    typedArg->unk97A = 0xFF;
    typedArg->unk97C = 0;
    typedArg->unk980 = 0;
    typedArg->unk982 = 0;
    typedArg->unk974 = temp;
    typedArg->unk988 = typedArg->unk960;
    typedArg->unk984 = getTable2DEntry(typedArg->unk964, typedArg->unk9A0, 0);
    typedArg->unk98C = 0xFF;
    typedArg->unk98E = 0xFF;
    typedArg->unk990 = 2;
    typedArg->unk9A4 = 0;
    typedArg->unk9A6 = 0;
    typedArg->unk9AC = 0;
    typedArg->unk99C = NULL;
    typedArg->unk998 = NULL;
    typedArg->unk9B0 = 0;
    typedArg->unk9B2 = 0;
    typedArg->unk9AE = 0;
    typedArg->unk994 = NULL;
}

void func_800B0388_1DC528(Struct_800B0388 *arg0) {
    s16 temp_s0;
    s32 var_s3;
    s16 temp_v1;
    s16 temp_a8;

    if (arg0->unk2 >= arg0->unk9AC) {
        arg0->unk9B0 = 0;
        arg0->unk9B2 = 0;
        arg0->unk9A4 = 0;
        arg0->unk99C.word = 0;
        arg0->unk998 = 0;
        arg0->unk9AE = 0;
        arg0->unk9B4 = 0;
        arg0->unk9A6++;

        if (func_800B0E24_1DCFC4(arg0->unk95C, arg0->unk9A6) != 0) {
            arg0->unk9A0 = arg0->unk9A0 + 1;
        }

        temp_s0 = arg0->unk9A6;
        if (temp_s0 < (func_800B0DC0_1DCF60(arg0->unk95C) - 1)) {
            arg0->unk9AC = func_800B0DD0_1DCF70(arg0->unk95C, (s16)arg0->unk9A6 + 1);
        } else {
            arg0->unk9AC = 0x7530;
        }
    } else if (arg0->unk2 >= (arg0->unk9AC - 2)) {
        arg0->unk994 = arg0->unk994 + 0xFF800000;
        if (arg0->unk994 < 0) {
            arg0->unk994 = 0;
        }
    } else {
        arg0->unk994 += 0x800000;
        if (arg0->unk994 > 0xFF0000) {
            arg0->unk994 = 0xFF0000;
        }
    }

    if (func_800B0E24_1DCFC4(arg0->unk95C, arg0->unk9A6) != 0) {
        temp_v1 = arg0->unk9B4;
        if (temp_v1 == 0) {
            if (arg0->unk9AE == 0) {
                goto loop_start;
            }
            if (arg0->unk9B2 != 0) {
                if (arg0->unk998 != -1) {
                    arg0->unk99C.word += arg0->unk998;
                }
            }
            arg0->unk9AE--;
        } else {
            arg0->unk9B4 = temp_v1 - 1;
        }

        if (arg0->unk9AE == 0) {
        loop_start:
            while (arg0->unk9B0 < func_800B0E80_1DD020(arg0->unk95C, arg0->unk9A6)) {
                u8 *ptr;

                ptr = func_800B0E24_1DCFC4(arg0->unk95C, arg0->unk9A6);
                arg0->unk9AE = ptr[arg0->unk9B0];
                arg0->unk99C.word = arg0->unk9B2 << 20;

                if (arg0->unk9AE & 0x80) {
                    arg0->unk9AE &= 0x7F;
                    arg0->unk998 = -1;
                } else {
                    if (arg0->unk9AE != 0) {
                        arg0->unk998 = 0x100000 / (s16)arg0->unk9AE;
                    } else {
                        arg0->unk998 = -1;
                    }

                    arg0->unk9B2++;
                }

                arg0->unk9B0++;

                if (arg0->unk9AE != 0) {
                    break;
                }
            }
        }

        if (arg0->unk9A4 == 0) {
            arg0->unk970 = getTable2DEntry(arg0->unk964, arg0->unk9A0, 0);
            arg0->unk984 = getTable2DEntry(arg0->unk964, arg0->unk9A0, 0);
            arg0->unk9A8 = getMaxLinePixelWidth(arg0->unk970);
        }

        var_s3 = arg0->unk99C.halves.high;
        temp_a8 = arg0->unk9A8;
        temp_s0 = -(temp_a8 / 2);
        if (temp_a8 < var_s3) {
            var_s3 = temp_a8;
        }

        if (arg0->unk998 == 0) {
            var_s3 = 0;
        }

        setModelCameraTransform(&arg0->unk3B8, temp_s0, 0x58, 0, 0, arg0->unk9A8, 0x10);
        setModelCameraTransform(&arg0->unk590, temp_s0, 0x58, 0, 0, var_s3, 0x10);

        arg0->unk97A.halfword = arg0->unk994 >> 16;
        arg0->unk98E.halfword = arg0->unk994 >> 16;

        func_80035260_35E60(
            arg0->unk974,
            arg0->unk970,
            arg0->unk96C,
            arg0->unk96E,
            arg0->unk979,
            arg0->unk97A.bytes.low,
            arg0->unk97C,
            2,
            0
        );
        func_80035260_35E60(
            arg0->unk988,
            arg0->unk984,
            arg0->unk980,
            arg0->unk982,
            arg0->unk98D,
            arg0->unk98E.bytes.low,
            arg0->unk990,
            3,
            1
        );
    }
    arg0->unk9A4++;
}
