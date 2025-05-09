#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_

#include "common.h"

typedef struct {
    s32 unk0;
    s32 unk4;
} GameStateUnk28;

typedef struct {
    u8 padding[0x176C];
    s32 unk176C;
} GameStateUnk10;

typedef struct {
    /* 0x0 */ ALPlayer* audioPlayer0;
    u8 PAD_0[4];
    /* 0x8 */ ALPlayer* audioPlayer1;
    u8 PAD_1[0x2];
    GameStateUnk10* unk10;
    u8 PAD_NEW_2[0x4];
    void* unk18;
    s32 unk1C;
    s32 unk20;
    u8 PAD_2[0x4];
    GameStateUnk28* unk28;
    u8 PAD_3[0x4];
    s32 unk30;
    u8 PAD_4[0x10];
    u8* unk44;
    u8* unk48;
    s32 unk4C;
    u8 PAD_5[0xC];
    u8 unk5C;
    u8 unk5D;
    u8 unk5E;
    u8 PAD_6[0x1A];
    u8 unk79;
    u8 unk7A;
    s8 unk7B;
    s8 unk7C;
    u8 PAD_7[0x5D];
    u16 unkDA;
    u8 PAD_8[0xF9];
    /* 0x1D8 */ ALPlayer* audioPlayer2;
    u8 PAD_9[0x4];
    /* 0x1E0 */ ALPlayer* audioPlayer3;
    u8 PAD_A[0x1CA];
    /* 0x3B0 */ ALPlayer* audioPlayer4;
    u8 PAD_B[0x4];
    /* 0x3B8 */ ALPlayer* audioPlayer5;
    u8 PAD_C[0x14];
    s32 unk3D0;
    s32 unk3D4;
    s32 unk3D8;
    void* unk3DC;
    void* unk3E0;
    void* unk3E4;
    s16 unk3E8;
    u8 PAD_D[0xA];
    s16 unk3F4;
    s32 unk3F8;
    u16 unk3FC;
    u16 unk3FE;
    u8 PAD_E[0x3];
    u8 unk403;
    u8 PAD_0A[0x20];
    u8 unk424;
    s8 unk425;
    u8 unk426;
    u8 unk427;
    u8 unk428;
    u8 unk429;
    u8 unk42A;
    u8 PAD_11[3];
    s8 unk42E;
    u8 PAD_12[0x158];
    s32 unk588;
    s32 unk58C;
    u16 unk590;
    u8 PAD_13[0x21];
    u8 unk5B3;
    u8 PAD_14[0x11];
    u8 unk5C5;
    u8 unk5C6;
    u8 unk5C7;
    s8 unk5C8;
    u8 unk5C9;
    u8 unk5CA[0];
    u8 PAD_15[0x1216];
    s8 unk17E0;
} GameState;

GameState* GameStateGet();

#endif