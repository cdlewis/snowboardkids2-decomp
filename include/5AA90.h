#pragma once

#include "common.h"
#include "gamestate.h"
#include "geometry.h"

typedef struct ListNode_5AA90 {
    /* 0x00 */ struct ListNode_5AA90 *next;
    /* 0x04 */ Vec3i *posPtr;
    /* 0x08 */ Vec3i localPos;
    /* 0x14 */ s32 radius;
    /* 0x18 */ u8 id;
} ListNode_5AA90;

void func_8005C838_5D438(ListNode_5AA90 *arg0);
void *func_8005B24C_5BE4C(Vec3i *arg0, s32 arg1, s32 arg2);
s32 func_8005A9A8_5B5A8(Player *arg0);
s32 func_8005B400_5C000(Player *arg0, Vec3i *arg1, s32 arg2);
s32 func_8005B9E4_5C5E4(Vec3i *arg0, s32 arg1, s32 arg2, s16 arg3);
void func_8005BCB8_5C8B8(void *arg0, s32 arg1, s32 arg2);
s32 func_8005C250_5CE50(Vec3i *arg0, s32 arg1, s32 arg2);
void func_8005C868_5D468(void *arg0);
s32 isPlayerInRangeAndPull(Vec3i *arg0, s32 arg1, Player *arg2);
void func_8005B730_5C330(Vec3i *arg0, s32 arg1, s32 arg2, s16 arg3);
s32 func_80059E90_5AA90(void *arg0, void *arg1, u16 arg2, void *arg3);
void *func_8005C454_5D054(Vec3i *arg0, s32 arg1, s32 arg2, Vec3i *arg3);
s16 func_8005BF50_5CB50(Vec3i *arg0, s16 arg1, s16 arg2, s32 arg3, s32 arg4);
