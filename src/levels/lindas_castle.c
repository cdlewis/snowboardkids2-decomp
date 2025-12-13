#include "levels/lindas_castle.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
#include "rand.h"
#include "594E0.h"
#include "task_scheduler.h"

extern s32 isPlayerInRangeAndPull(void *a0, s32 a1, Player *a2);
extern void *func_80055E68_56A68(u8);
extern void *func_80055DC4_569C4(u8);
extern void *func_80055DF8_569F8(u8);

typedef struct {
    s32 unk0;
    s32 unk4;
} D_800BBBC0_ABA70_substruct;

typedef struct {
    s32 unk0;
    D_800BBBC0_ABA70_substruct unk4[2];
} D_800BBBC0_ABA70_struct;

extern D_800BBBC0_ABA70_struct D_800BBBC0_ABA70[];
extern s32 D_800BBC1C_ABACC[3];

extern void func_800BB5BC_AB46C(void);

void func_800BB320_AB1D0(func_800BB2B0_arg *arg0);
void func_800BB7D4_AB684(func_800BB2B0_arg *task);

typedef struct {
    u8 pad[0x24];
    void *unk24;
    void *unk28;
} func_800BB9F0_AB8A0_arg;

void func_800BB9F0_AB8A0(func_800BB9F0_AB8A0_arg *arg0);

typedef struct {
    u8 pad[0x5C];
    u8 unk5C;
} Allocation_AB304;

typedef struct {
    u8 pad0[0x20];
    void *unk20;
    void *unk24;
    void *unk28;
    s32 unk2C;
    u8 pad30[0xC];
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s16 unk50;
    s16 unk52;
    u16 unk54;
    s16 unk56;
} TaskArg_AB304;

void func_800BB2B0(func_800BB2B0_arg *arg0) {
    s32 i;

    createYRotationMatrix(&arg0->matrix, arg0->rotationAngle);
    memcpy(&arg0->matrix.unk14, arg0->targetPosition, 0xC);
    arg0->matrix.unk18 += arg0->velocityY;

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void func_800BB320_AB1D0(func_800BB2B0_arg *arg0) {
    GameState *gs;
    s32 pos[3];
    s32 target[3];
    s32 i;
    Player *player;

    gs = getCurrentAllocation();
    memcpy(&pos[0], &arg0->targetPosition[0], 0xC);
    pos[1] += 0x1C0000 + arg0->velocityY;

    for (i = 0; i < gs->numPlayers; i++) {
        player = &gs->players[i];
        if (isPlayerInRangeAndPull(&pos[0], 0x1C0000, player) != 0) {
            target[0] = ((player->worldPosX + player->unkAD4[0] - pos[0]) / 2) + pos[0];
            target[1] = ((player->worldPosY + player->unkAD4[1] - pos[0]) / 2) + pos[1];
            target[2] = ((player->worldPosZ + player->unkAD4[2] - pos[0]) / 2) + pos[2];
            func_800589CC_595CC(&gs->players[i], &target[0]);
        }
    }
}

void func_800BB454_AB304(TaskArg_AB304 *task) {
    Allocation_AB304 *allocation;
    void *temp;
    s16 index;
    s32 temp_value;
    void (*temp_callback)(void);

    allocation = (Allocation_AB304 *)getCurrentAllocation();

    temp = func_80055E68_56A68(allocation->unk5C);
    task->unk20 = (void *)((u32)temp + 0x90);

    task->unk24 = func_80055DC4_569C4(allocation->unk5C);
    task->unk28 = func_80055DF8_569F8(allocation->unk5C);

    index = task->unk50;

    task->unk2C = 0;
    task->unk52 = 1;

    task->unk56 = D_800BBBC0_ABA70[task->unk50].unk0;

    task->unk3C = D_800BBBC0_ABA70[task->unk50].unk4[0].unk0;

    task->unk44 = D_800BBBC0_ABA70[task->unk50].unk4[0].unk4;

    task->unk40 = func_80061A64_62664((u8 *)allocation + 0x30, task->unk56, &task->unk3C);

    index = task->unk50;

    task->unk54 =
        func_8006D21C_6DE1C(D_800BBBC0_ABA70[index].unk4[1].unk0, D_800BBBC0_ABA70[index].unk4[1].unk4, task->unk3C, task->unk44);

    task->unk48 = 0;
    task->unk4C = 0;

    setCleanupCallback(func_800BB9F0_AB8A0);

    if ((randA() & 0xFF) < 0xB3) {
        temp_callback = func_800BB5BC_AB46C;
        task->unk48 = 0;
        temp_value = 0x40000;
    } else {
        temp_value = 0x18000;
        temp_callback = (void (*)(void))func_800BB7D4_AB684;
        task->unk48 = 0;
    }

    task->unk4C = temp_value;
    setCallback(temp_callback);
}

INCLUDE_ASM("asm/nonmatchings/levels/lindas_castle", func_800BB5BC_AB46C);

void func_800BB7D4_AB684(func_800BB2B0_arg *task) {
    GameState *gs;
    s32 rotatedVec[3];
    s16 var_v1;
    GameDataLayout *gameData;

    gs = (GameState*)getCurrentAllocation();

    if (gs->gamePaused == 0) {
        var_v1 = func_8006D21C_6DE1C(
            D_800BBBC0_ABA70[task->unk50].unk4[task->unk52].unk0,
            D_800BBBC0_ABA70[task->unk50].unk4[task->unk52].unk4,
            task->targetPosition[0],
            task->targetPosition[2]
        );
        var_v1 = var_v1 - task->rotationAngle & 0x1FFF;


        if (var_v1 >= 0x1001) {
            var_v1 = var_v1 | 0xE000;
        }

        if (var_v1 >= 0x81) {
            var_v1 = 0x80;
        }

        if (var_v1 < -0x80) {
            var_v1 = -0x80;
        }

        task->rotationAngle = task->rotationAngle + var_v1;

        rotateVectorY(&D_800BBBC0_ABA70[4].unk4[1].unk0, (s16)task->rotationAngle, &rotatedVec);

        gameData = &gs->gameData;
        task->targetPosition[0] += rotatedVec[0];
        task->targetPosition[2] += rotatedVec[2];

        task->unk56 = func_80060A3C_6163C(gameData, task->unk56, &task->targetPosition[0]);
        task->targetPosition[1] = func_80061A64_62664(gameData, task->unk56, &task->targetPosition[0]);

        task->velocityY += task->unk4C;
        task->unk4C = task->unk4C - 0x8000;

        if (task->velocityY == 0) {
            task->unk4C = 0x18000;
        }

        rotatedVec[0] = D_800BBBC0_ABA70[task->unk50].unk4[task->unk52].unk0 - task->targetPosition[0];
        rotatedVec[1] = D_800BBBC0_ABA70[task->unk50].unk4[task->unk52].unk4 - task->targetPosition[2];

        if (rotatedVec[0] + 0xFFFFF <= 0x1FFFFEU && (rotatedVec[2] + 0xFFFFF) <= 0x1FFFFEU) {
            func_80069CF8_6A8F8();
        }

        func_800BB320_AB1D0(task);
    }

    func_800BB2B0(task);
}

void func_800BB9F0_AB8A0(func_800BB9F0_AB8A0_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

typedef struct {
    u8 pad[0x50];
    u16 unk50;
} ScheduledTaskWith50;

typedef struct {
    u16 unk0;
    s16 unk2;
} func_800BBA28_AB8D8_arg;

void func_800BBA54_AB904(func_800BBA28_AB8D8_arg *arg0);

void func_800BBA28_AB8D8(func_800BBA28_AB8D8_arg *arg0) {
    arg0->unk0 = 0;
    arg0->unk2 = 0x14;
    setCallback(func_800BBA54_AB904);
}

void func_800BBA54_AB904(func_800BBA28_AB8D8_arg *arg0) {
    GameState *gameState = getCurrentAllocation();
    ScheduledTaskWith50 *task;
    s16 counter;
    s16 newValue;

    if (gameState->gamePaused) {
        return;
    }

    counter = arg0->unk2;
    if (counter == 0) {
        task = (ScheduledTaskWith50 *)scheduleTask(func_800BB454_AB304, 0, 0, 0x32);
        if (task != NULL) {
            task->unk50 = arg0->unk0;
        }

        arg0->unk0++;
        if ((s16)arg0->unk0 == 4) {
            arg0->unk0 = 0;
        }

        newValue = (randA() & 0xF) + 0x14;
    } else {
        newValue = counter - 1;
    }
    arg0->unk2 = newValue;
}
void func_800BBB1C_AB9CC(s16 *arg0);

void func_800BBAF8_AB9A8(s16 *arg0) {
    *arg0 = 0;
    setCallback(func_800BBB1C_AB9CC);
}

void func_800BBB1C_AB9CC(s16 *arg0) {
    GameState *gameState = getCurrentAllocation();
    s32 i;

    for (i = 0; i < gameState->unk5F; i++) {
        if (gameState->players[i].unkBC5 == *arg0) {
            if ((u16)(gameState->players[i].unkB94 - 0x95) < 0xD) {
                *arg0 = *arg0 + 1;
                func_80058530_59130(0x4D, 6);
            }
        }
    }
}
