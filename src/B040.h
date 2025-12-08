#pragma once

#include "common.h"

typedef struct {
    s32 array_offset;
    s32 count_offset;
} Table_B934;

void func_8000A510_B110(s32, s16, s16, s16, void *, s32, s8, u8, u8, s16);
void *func_8000B6B8_C2B8(s16 arg0);
void *func_8000B714_C314(Table_B934 *arg0, s32 arg1, s32 arg2);
