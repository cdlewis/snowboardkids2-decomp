#include "common.h"
#include "gamestate.h"
#include "task_scheduler.h"

extern u8 D_80093BB0_947B0[];
extern u8 D_80093F7C_94B7C[];

void func_80058690_59290(Player *player) {
    GameState *gameState;
    u8 *table;
    u8 tableIdx;
    u8 val;
    s32 temp;
    s32 temp2;

    gameState = (GameState *)getCurrentAllocation();

    if (gameState->unk7A == 0xB) {
        table = D_80093F7C_94B7C;
        tableIdx = 0;
    } else {
        table = D_80093BB0_947B0 + (player->unkBBB * 54);
        tableIdx = player->unkBB9;
    }

    val = table[tableIdx * 6];

    temp = val * 3;
    temp = temp << 7;
    temp = temp - val;
    temp2 = temp << 3;
    temp2 = temp2 - temp;
    temp = temp2 << 5;
    temp2 = temp2 + temp;
    temp2 = temp2 << 2;
    temp2 = temp2 + val;
    temp2 = temp2 + val;

    player->unkAA0 = (temp2 / 100) + 0xEB333;

    player->unkAC0 = table[tableIdx * 6 + 1] + 0x19;
    player->unkAC1 = table[tableIdx * 6 + 2] + 1;

    temp = table[tableIdx * 6 + 3] << 15;
    player->unkAB0 = (temp / 100) + 0x1000;

    if (player->unkBC7 != 0) {
        player->unkAB0 = 0xC000;
    }

    temp = table[tableIdx * 6 + 4] << 14;
    player->unkAB4 = (temp / 100) + 0x3000;

    temp = table[tableIdx * 6 + 5] << 17;
    player->unkABC = (temp / 100) + 0x28000;
}

s32 func_80058804_59404(s32 arg0, s32 arg1) {
    void *allocation;
    u8 *table;
    s32 value;
    s32 temp;
    s32 temp2;

    allocation = getCurrentAllocation();

    if (*(u8 *)((u8 *)allocation + 0x7A) == 0xB) {
        table = D_80093F7C_94B7C;
        arg0 = 0;
    } else {
        table = D_80093BB0_947B0 + (arg1 * 54);
    }

    value = table[arg0 * 6];

    temp = value * 3;
    temp = temp << 7;
    temp = temp - value;
    temp2 = temp << 3;
    temp2 = temp2 - temp;
    temp = temp2 << 5;
    temp2 = temp2 + temp;
    temp2 = temp2 << 2;
    temp2 = temp2 + value;
    temp2 = temp2 + value;

    return (temp2 / 100) + 0xEB333;
}
