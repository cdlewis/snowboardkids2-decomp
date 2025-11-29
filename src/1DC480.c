#include "common.h"

extern void *func_8000B714_C314(void *, s32, s32);

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

void func_800B02E0(func_800B02E0_arg *arg0) {
    void *temp;

    arg0->unk9A0 = -1;
    temp = arg0->unk960;
    arg0->unk96C = 0;
    arg0->unk96E = 0;
    arg0->unk978 = 0xFF;
    arg0->unk97A = 0xFF;
    arg0->unk97C = 0;
    arg0->unk980 = 0;
    arg0->unk982 = 0;
    arg0->unk974 = temp;
    arg0->unk988 = arg0->unk960;
    arg0->unk984 = func_8000B714_C314(arg0->unk964, arg0->unk9A0, 0);
    arg0->unk98C = 0xFF;
    arg0->unk98E = 0xFF;
    arg0->unk990 = 2;
    arg0->unk9A4 = 0;
    arg0->unk9A6 = 0;
    arg0->unk9AC = 0;
    arg0->unk99C = NULL;
    arg0->unk998 = NULL;
    arg0->unk9B0 = 0;
    arg0->unk9B2 = 0;
    arg0->unk9AE = 0;
    arg0->unk994 = NULL;
}

INCLUDE_ASM("asm/nonmatchings/1DC480", func_800B0388_1DC528);
