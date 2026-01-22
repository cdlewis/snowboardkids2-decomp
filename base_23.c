#include "common.h"
#include "D_800AFE8C_A71FC_type.h"
#include "gamestate.h"
#include "task_scheduler.h"
#include "10AD0.h"

extern struct {
    u16 unk0;
    u16 unk2;
} D_8008DE54_8EA54[];

extern struct {
    u16 x;
    u16 y;
} D_8008DE64_8EA64[];

extern struct {
    u16 x;
    u16 y;
} D_8008DE9C_8EA9C[];

extern u16 D_8008DE7A_8EA7A[];

typedef struct {
    s16 x;
    s16 y;
    void *asset;
    s16 spriteIndex;
    s16 alpha;
    u8 unkC;
    u8 blinkState;
    u8 paddingE[2];
} CharacterNameSprite;

void func_800269C8_275C8(CharacterNameSprite *sprite) {
    D_800AFE8C_A71FC_type *global;
    u8 *allocHW;
    u8 *allocB;
    s32 i;
    s32 numPlayers;
    u16 xPos;
    u16 yPos;
    s32 charIndex;
    u16 *d54;
    u16 *d54Y;
    s32 one;
    u16 *d64;
    u16 *d64Y;
    u16 state;

    allocHW = (u8 *)getCurrentAllocation();
    global = D_800AFE8C_A71FC;
    i = 0;

    if ((s32)global->numPlayers > 0) {
        d54 = (u16 *)D_8008DE54_8EA54;
        d54Y = d54 + 1;
        one = 1;
        d64 = (u16 *)D_8008DE64_8EA64;
        d64Y = d64 + 1;
        allocB = allocHW;

        do {
            state = *(u16 *)(allocHW + 0x1898);

            if ((state & 0xFFFF) != 4) {
                if ((u32)(state - 3) < 8) {
                    numPlayers = D_800AFE8C_A71FC->numPlayers;
                    xPos = d64[numPlayers * 2];
                    yPos = d64Y[numPlayers * 2];
                    sprite->spriteIndex = allocB[0x18A8] + 0x1D;
                } else {
                    numPlayers = global->numPlayers;
                    xPos = d54[numPlayers * 2];
                    charIndex = ((u8 *)global + i)[0xD];
                    yPos = d54Y[numPlayers * 2];

                    if ((u32)(charIndex & 0xFF) >= 9U) {
                        if (numPlayers == one) {
                            sprite->spriteIndex = charIndex + 0x30;
                            xPos += 0x18;
                        } else {
                            sprite->spriteIndex = charIndex + 0x23;
                            xPos = D_8008DE9C_8EA9C[numPlayers].x - D_8008DE7A_8EA7A[charIndex];
                        }
                    } else {
                        s32 spriteIdx = 0x1D;
                        if (numPlayers == one) {
                            spriteIdx = 0x36;
                        }
                        sprite->spriteIndex = spriteIdx + allocB[0x18A8];
                    }
                }
                sprite->x = xPos;
                sprite->y = yPos;

                if ((u32)(*(u16 *)(allocHW + 0x1898) - 5) < 2 && (*(u16 *)(allocHW + 0x18A0) & 1)) {
                    sprite->blinkState = 0xFF;
                } else {
                    sprite->blinkState = 0;
                }
                debugEnqueueCallback((i + 0xC) & 0xFFFF, 0, func_80012004_12C04, sprite);
            }

            global = D_800AFE8C_A71FC;
            sprite++;
            allocHW += 2;
            i++;
            allocB++;
        } while (i < (s32)global->numPlayers);
    }
}
