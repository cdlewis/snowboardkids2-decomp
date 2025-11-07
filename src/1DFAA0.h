#pragma once

#include "common.h"

typedef struct {
    u8 padding[0xFF7];
    s8 unkFF7;
} func_800B29F0_1DFAA0_arg;

void resetScriptState(u8 *arg0);

void func_800B29F0_1DFAA0(func_800B29F0_1DFAA0_arg *arg0);