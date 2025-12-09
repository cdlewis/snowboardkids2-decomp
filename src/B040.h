#pragma once

#include "common.h"

typedef struct {
    s32 array_offset;
    s32 count_offset;
} Table_B934;

s32 func_8000A510_B110(
    s32 arg0,
    s16 arg1,
    s16 arg2,
    s16 arg3,
    void *arg4,
    s32 arg5,
    volatile unsigned int arg6,
    u8 arg7,
    u8 arg8,
    s16 arg9,
    s16 argA
);
void *func_8000B6B8_C2B8(s16 arg0);
void *func_8000B714_C314(Table_B934 *arg0, s32 arg1, s32 arg2);
