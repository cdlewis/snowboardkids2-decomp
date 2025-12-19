#ifndef DC90_H
#define DC90_H

#include "common.h"

typedef struct {
    u8 high;
    u8 low;
} DC90Bytes;

typedef union {
    s16 asS16;
    DC90Bytes asBytes;
} DC90S16OrBytes;

typedef struct {
    /* 0x00 */ u8 *unk0;
    /* 0x04 */ void *unk4;
    /* 0x08 */ void *unk8;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ u8 pad12[0x2];
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ void *unk18;
    /* 0x1C */ void *unk1C;
    /* 0x20 */ DC90S16OrBytes unk20;
    /* 0x22 */ DC90S16OrBytes unk22;
    /* 0x24 */ u8 unk24;
    /* 0x25 */ u8 pad25[0x3];
    /* 0x28 */ void *unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s16 unk3C;
    /* 0x3E */ s16 unk3E;
} DC90TaskStruct;

void func_8000D244_DE44(DC90TaskStruct *arg0);

#endif
