#include "common.h"

typedef struct {
    s32 unk0;
    s32 unk4;
} GameStateUnk28;

typedef struct {
    ALPlayer* audioPlayer0;
    u8 padding0[4];
    ALPlayer* audioPlayer1;  // 0x8
    u8 padding1[0x1A];
    GameStateUnk28* unk28;
    u8 padding2[0x4];
    s32 unk30;
    u8 padding3[0x10];
    u8* unk44;
    u8* unk48;
    s32 unk4C;
    u8 padding4[0xC];
    u8 unk5C;
    u8 paddingN[0x7D];
    u16 unkDA;
    u8 padding5[0xF9];
    ALPlayer* audioPlayer2;  // 0x1D8
    u8 padding6[0x4];
    ALPlayer* audioPlayer3;  // 0x1E0
    u8 padding7[0x1CA];
    ALPlayer* audioPlayer4;  // 0x3B0
    u8 padding8[0x4];
    ALPlayer* audioPlayer5;  // 0x3B8
    u8 padding9[0x20];
    void* unk3DC;
    void* unk3E0;
    void* unk3E4;
    u8 padding10[0x0C];
    s16 unk3F4;
    s32 unk3F8;
    u16 unk3FC;
    u8 padding11[0x26];
    u8 unk424;
    s8 unk425;
    u8 padding12[1];
    u8 unk427;
    u8 padding13[1];
    u8 unk429;
    u8 padding14[4];
    s8 unk42E;
    u8 padding15[0x158];
    s32 unk588;
    s32 unk58C;
    u16 unk590;
    u8 padding16[0x21];
    u8 unk5B3;
    u8 padding18[0x11];
    u8 unk5C5;
    u8 unk5C6;
    u8 unk5C7;
    u8 padding19[0x1218];
    s8 unk17E0;
} GameState;

GameState* GameStateGet();