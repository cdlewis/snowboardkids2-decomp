#pragma once

#include "common.h"
#include "gamestate.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    u8 padA[0x2];
} Element_8004E6F8; // size 0xC

typedef struct {
    Element_8004E6F8 elements[3]; // offset 0x0, size 0x24
    void *unk24;                  // offset 0x24
} Struct_func_8004E6F8;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    u8 padA[0x2];
    Player *unkC;
    s32 unk10;
} Struct_func_8004D8E4;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    s16 unkA;
    u8 unkC;
    u8 unkD;
    u8 unkE;
    u8 padF;
    void *unk10;
    s32 unk14;
} Struct_func_8004F1D4;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    u8 padA[0x2];
    void *unkC;
    s16 unk10;
} Struct_func_8004F04C;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    u8 padA[0x6];
    void *unk10;
    s32 unk14;
    s32 unk18;
} Struct_func_8004D9D0;

void func_8004D890_4E490(s32 arg0, void *arg1);
void func_8004D8E4_4E4E4(Struct_func_8004D8E4 *arg0);
void func_8004D9D0_4E5D0(Struct_func_8004D9D0 *arg0);
void func_8004E6A4_4F2A4(s16 arg0, s16 arg1);
void func_8004E6F8_4F2F8(Struct_func_8004E6F8 *arg0);
void func_8004EDCC_4F9CC(s16 arg0);
void func_8004F1D4_4FDD4(Struct_func_8004F1D4 *arg0);
void func_8004F194_4FD94(s16 arg0);
void func_8004F820_50420(Struct_func_8004F04C *arg0);
void func_8004FCF0_508F0(s32 arg0);
void func_8004FF60_50B60(s16 arg0);
void func_8005011C_50D1C(void);
