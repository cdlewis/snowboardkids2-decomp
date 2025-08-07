#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_

#include "common.h"

typedef struct {
    u8 pading[0x1D8];
} GameStateUnk4;

typedef struct {
    s32 unk0;
    s32 unk4;
} GameStateUnk28;

typedef struct {
    u8 padding[0x434];
    s32 worldPosX;
    s32 unk438;
    s32 worldPosZ;
    u8 padding2[0x744];
    s32 unkB84;
    u8 padding3[0xC];
    u16 unkB94;
    u8 padding4[0x2E];
    u8 unkBC4;
    u8 padding5[0x23];
} Player;

typedef struct {
    /* 0x0 */ ALPlayer *audioPlayer0;
    GameStateUnk4 *unk4;
    GameStateUnk4 *unk8;
    GameStateUnk4 *unkC;
    Player *players;
    void *unk14;
    void *unk18;
    void *unk1C;
    void *unk20;
    u8 PAD_2[0x4];
    GameStateUnk28 *unk28;
    u8 PAD_3[0x4];
    // Start Embedded GameData
    u16 *gameDataStart;
    u16 *gameDataSection1Data;
    u16 *gameDataSection2Data;
    u16 *gameDataSection3Data;
    u16 gameDataFinalValue;
    // End Embedded Gamedata
    u8 *unk44;
    u8 *unk48;
    s32 unk4C;
    u8 PAD_5[0xC];
    u8 memoryPoolId;
    u8 unk5D;
    u8 numPlayers;
    u8 unk5F;
    u8 PAD_6[0x16];
    u8 gamePaused;
    u8 unk77;
    u8 unk78;
    u8 unk79;
    u8 unk7A;
    s8 unk7B;
    s8 unk7C;
    u8 PAD_7[0x5D];
    u16 unkDA;
    u8 PAD_8[0xF9];
    /* 0x1D8 */ ALPlayer *audioPlayer2;
    u8 PAD_9[0x4];
    /* 0x1E0 */ ALPlayer *audioPlayer3;
    u8 PAD_A[0x1CA];
    /* 0x3B0 */ ALPlayer *audioPlayer4;
    u8 PAD_B[0x4];
    /* 0x3B8 */ ALPlayer *audioPlayer5;
    u8 PAD_C[0x14];
    s32 unk3D0;
    s32 unk3D4;
    s32 unk3D8;
    void *unk3DC;
    void *unk3E0;
    void *unk3E4;
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
    s32 *unk588;
    s32 *unk58C;
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
    u8 PAD_15[0x1B2];
    s16 unk77C;
    u8 PAD_16[0x1D];
    u8 unk79B;
    u8 PAD_17[0x1044];
    s8 unk17E0;
} GameState;

#endif