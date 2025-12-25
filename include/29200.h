#pragma once

#include "common.h"

typedef struct {
    void *model;
    u8 pad4[0x56];
    s16 unk5A;
    u8 unk5C;
    u8 unk5D;
} Func80028600Arg;

void func_80028600_29200(Func80028600Arg *arg0);
