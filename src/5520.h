#include "common.h"

typedef struct {
    u8 padding[0x20];
    struct {
        s32 unk0;
        s32 unk4;
        s32 unk8;
        s32 unkC;
    } *unk20;
    u8 padding2[0xC];
    s32 unk30;
} func_80004FF8_5BF8_arg1;

void func_80004FF8_5BF8(u16 arg0, func_80004FF8_5BF8_arg1 *arg1);