#include "common.h"

typedef struct {
    u8 padding[0x20];
    struct {
        u8 padding[0x4];
        s32 unk4;
        s32 unk8;
        s32 unkC;
    }* unk20;
    u8 padding2[0xA];
    s32 unk30;
    u8 padding3[0x7];
    u8 unk3B;
    s32 unk3C;
    u8 padding4[0x10];
    u32 unk50;
    u8 padding5[0xC];
    u8 unk60;
} func_80063824_64424_arg;

void func_800635CC_641CC(s32 arg0, func_80063824_64424_arg* arg1);