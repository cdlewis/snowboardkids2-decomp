#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_

#include "6E840.h"
#include "common.h"
#include "displaylist.h"

typedef struct {
    loadAssetMetadata_arg asset;
    u8 padding[0x24];
} GameStateUnk44_Item;

typedef struct {
    u8 data[0x180];
} GameStateUnk44Unk2C0;

typedef struct {
    u8 padding[0x2C0];
    GameStateUnk44Unk2C0 unk2C0[8];
    u8 padding2[0x100];
    GameStateUnk44_Item unkFC0[3];
    GameStateUnk44_Item unk1080[3];
    u8 padding3[0x1FF];
    s32 unk1340;
    u8 padding4[0x3C];
    s32 unk1380;
} GameStateUnk44;

typedef struct {
    s32 unk0;
    s32 unk4;
} GameStateUnk28;

typedef struct {
    u8 padding[0x434];
    s32 worldPosX;
    s32 unk438;
    s32 worldPosZ;
    u8 padding2[0x72C];
    s32 unkB6C;
    s32 unkB70;
    u8 padding6[0x10];
    s32 unkB84;
    u8 padding3[0xC];
    u16 unkB94;
    u8 padding4[0x2E];
    u8 unkBC4;
    u8 unkBC5;
    u8 padding5[0x22];
} Player;

typedef struct {
    /* 0x0 */ Node_70B00 *audioPlayer0;
    Node_70B00 *unk4;
    Node_70B00 *unk8;
    Node_70B00 *unkC;
    Player *players;
    void *unk14;
    void *unk18;
    void *unk1C;
    void *unk20;
    u8 PAD_2[0x4];
    GameStateUnk28 *unk28;
    u8 PAD_3[0x4];
    /* 0x30 */ GameDataLayout gameData;
    GameStateUnk44 *unk44;
    u8 *unk48;
    s32 unk4C;
    u8 PAD_5[0xB];
    u8 unk5B;
    u8 memoryPoolId;
    u8 unk5D;
    u8 numPlayers;
    u8 unk5F;
    u8 unk60;
    u8 PAD_6[0x13];
    u8 unk74;
    u8 PAD_6B[0x1];
    u8 gamePaused;
    u8 unk77;
    u8 unk78;
    u8 unk79;
    u8 unk7A;
    s8 unk7B;
    s8 unk7C;
    u8 unk7D;
    u8 unk7E;
    u8 PAD_7[0x4];
    /* 0x83 */ u8 unk83;
    /* 0x84 */ u8 unk84;
    u8 PAD_7B[0x55];
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
    u8 PAD_13[0x8];
    u8 unk59A[8];
    u8 PAD_13B[0x11];
    u8 unk5B3;
    u8 PAD_14[0x4];
    u8 unk5B8[9];
    u8 PAD_14B[0x4];
    u8 unk5C5;
    u8 unk5C6;
    u8 unk5C7;
    s8 unk5C8;
    u8 unk5C9;
    u8 unk5CA[0];
    u8 PAD_15[0xE];
    u8 unk5D8;
    u8 PAD_15B[0x1A3];
    s16 unk77C;
    u8 PAD_16[0x6];
    u8 unk784[4];
    u8 PAD_16B[0x10];
    u8 unk798;
    u8 PAD_16C[2];
    u8 unk79B;
    u8 unk79C;
    u8 PAD_17[4];
    s8 unk7A1;
    u8 PAD_18[0x103E];
    s8 unk17E0;
} GameState;

#endif
