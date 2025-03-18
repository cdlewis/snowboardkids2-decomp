#include "common.h"

typedef struct {
    char padding[0x3DC];
    void* unk3DC;
    void* unk3E0;
    void* unk3E4;
    char padding5[0xC];
    s16 unk3F4;
    s32 unk3F8;
    u16 unk3FC;
    char padding2[0x26];
    u8 unk424;
    s8 unk425;
    char padding4[1];
    u8 unk427;
    char paddding3[1];
    s8 unk429;
    char padding3[4];
    s8 unk42E;
} GameState;