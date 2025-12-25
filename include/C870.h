#pragma once

#include "19E80.h"

typedef struct {
    DataTable_19E80 *unk0;
    void *unk4;
    void *unk8;
} C870Struct;

void func_8000BC70_C870(C870Struct *arg0, DataTable_19E80 *arg1);
void func_8000BCEC_C8EC(C870Struct *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
