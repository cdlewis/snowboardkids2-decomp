#pragma once

#include "common.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    void *unk8;
    s16 unkC;
    s16 unkE;
    u8 unk10;
    u8 pad11[3];
    s16 unk14;
    s16 unk16;
    void *unk18;
    s16 unk1C;
    s16 unk1E;
    u8 unk20;
    u8 unk21;
    u8 pad22[2];
    s16 unk24;
    s16 unk26;
    void *unk28;
    s16 unk2C;
    s16 unk2E;
    u8 unk30;
    u8 unk31;
} func_80037710_38310_arg;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    s16 unkA;
    u8 unkC;
    u8 unkD;
    u8 padE[2];
} Entry1;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    void *unk8;
    s16 unkC;
    s16 unkE;
    u8 unk10;
    u8 pad11[3];
} Entry2;

typedef struct {
    Entry1 entries1[4];
    Entry2 entries2[4];
    void *unk90;
} func_80037BFC_387FC_arg;

void func_80037710_38310(func_80037710_38310_arg *arg0);
void func_800378AC_384AC(void);
void func_80037BFC_387FC(func_80037BFC_387FC_arg *arg0);
