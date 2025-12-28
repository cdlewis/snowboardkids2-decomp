#include "1DC480.h"
#include "B040.h"
#include "common.h"

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

INCLUDE_ASM("asm/nonmatchings/1DC480", func_800B0388_1DC528);
