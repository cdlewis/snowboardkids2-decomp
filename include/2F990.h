#pragma once

#include "common.h"

typedef struct {
    s16 unk00;
    s16 unk02;
    s16 unk04;
    s16 unk06;
    void *unk08;
} SubStruct;

typedef struct {
    void *unk00;
    s16 unk04;
    s16 unk06;
    void *unk08;
    s16 unk0C;
    s16 unk0E;
    SubStruct substruct[3];
    s16 unk34;
    s16 unk36;
    s8 unk38;
    s8 unk39;
    s8 unk3A;
} Struct_80030694;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    s8 unkA;
    s8 unkB;
} func_800308FC_314FC_arg;

typedef struct {
    func_800308FC_314FC_arg items[4];
    s16 unk30;
    s16 unk32;
    void *unk34;
    void *unk38;
    s16 unk3C;
    s16 unk3E;
    s8 unk40;
} func_80030A00_31600_arg;

void func_800304B8_310B8(func_800308FC_314FC_arg *arg0);
void func_80030694_31294(Struct_80030694 *arg0);
void func_800308C4_314C4(Struct_80030694 *arg0);
void func_800308FC_314FC(func_800308FC_314FC_arg *arg0);
void func_80030A00_31600(func_80030A00_31600_arg *arg0);
