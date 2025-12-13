#pragma once

#include "common.h"

typedef struct {
    u8 pad0[0x14]; /* 0x00 */
    u8 unk14[0xC]; /* 0x14 */
    void *unk20;   /* 0x20 */
    void *unk24;   /* 0x24 */
    void *unk28;   /* 0x28 */
    s32 unk2C;     /* 0x2C */
    u8 pad30[0xB]; /* 0x30 */
    u8 unk3B;      /* 0x3B */
    u8 pad3C[0x4]; /* 0x3C */
    s16 unk40;     /* 0x40 */
    u16 unk42;     /* 0x42 */
    s16 unk44;     /* 0x44 */
    s16 unk46;     /* 0x46 */
} Func420E8State;

Func420E8State *func_80042340_42F40(void *arg0);
void func_800419E4_425E4(void *arg0);
