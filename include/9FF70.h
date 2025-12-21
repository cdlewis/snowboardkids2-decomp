#pragma once

#include "common.h"
#include "gamestate.h"

typedef struct {
    u8 _pad0[0xB84];
    s32 unkB84;
    u8 _padB88[0x12]; // 0xB88 to 0xB9A
    s16 unkB9A;
    u8 _padB9C[0x8]; // 0xB9C to 0xBA4
    s16 unkBA4;
    s16 unkBA6;
    u8 _padBA8[0x15]; // 0xBA8 to 0xBBD
    u8 unkBBD;
    u8 unkBBE;
    u8 unkBBF;
    u8 unkBC0;
} func_800B00D4_arg;

void func_800B00C0_9FF70(Player *arg0, u8 arg1);
void func_800B00D4_9FF84(Player *arg0, s32 arg1);
void func_800B02AC_A015C(Player *arg0);
void func_800B9500_A93B0(void *arg0);
void func_800B99E0(void *arg0);
void func_800B9AE0(void *arg0);
