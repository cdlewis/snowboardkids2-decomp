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
    ALPlayer* audioPlayer0;
    u8 PAD_0[4];
    ALPlayer* audioPlayer1;  // 0x8
    u8 PAD_1[0x2];
    GameStateUnk10* unk10;
    u8 PAD_2[0x12];
    GameStateUnk28* unk28;
    u8 PAD_3[0x4];
    s32 unk30;
    u8 PAD_4[0x10];
    u8* unk44;
    u8* unk48;
    s32 unk4C;
    u8 PAD_5[0xC];
    u8 unk5C;
    u8 PAD_6[0x1E];
    s8 unk7B;
    s8 unk7C;
    u8 PAD_7[0x5D];
    u16 unkDA;
    u8 PAD_8[0xF9];
    ALPlayer* audioPlayer2;  // 0x1D8
    u8 PAD_9[0x4];
    ALPlayer* audioPlayer3;  // 0x1E0
    u8 PAD_A[0x1CA];
    ALPlayer* audioPlayer4;  // 0x3B0
    u8 PAD_B[0x4];
    ALPlayer* audioPlayer5;  // 0x3B8
    u8 PAD_C[0x20];
    void* unk3DC;
    void* unk3E0;
    void* unk3E4;
    u8 PAD_D[0x0C];
    s16 unk3F4;
    s32 unk3F8;
    u16 unk3FC;
    u8 PAD_E[0x26];
    u8 unk424;
    s8 unk425;
    u8 PAD_F[1];
    u8 unk427;
    u8 PAD_10[1];
    u8 unk429;
    u8 PAD_11[4];
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