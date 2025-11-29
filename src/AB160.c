#include "common.h"
#include "displaylist.h"
#include "geometry.h"
#include "task_scheduler.h"

extern u8 randA(void);
extern void func_800BB454_AB304(void);
extern void func_80058530_59130(s32, s32);

typedef struct {
    u8 pad[0x24];
    void *unk24;
    void *unk28;
} func_800BB9F0_AB8A0_arg;

typedef struct {
    /* 0x00 */ Mat3x3Padded matrix;
    /* 0x20 */ u8 _pad20[0x1C];
    /* 0x3C */ s32 targetPosition[3];
    /* 0x48 */ s32 velocityY;
    /* 0x4C */ u8 _pad4C[0x8];
    /* 0x54 */ u16 rotationAngle;
} func_800BB2B0_arg;

void func_800BB2B0(func_800BB2B0_arg *arg0) {
    s32 i;

    createYRotationMatrix(&arg0->matrix, arg0->rotationAngle);
    memcpy(&arg0->matrix.unk14, arg0->targetPosition, 0xC);
    arg0->matrix.unk18 += arg0->velocityY;

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

INCLUDE_ASM("asm/nonmatchings/AB160", func_800BB320_AB1D0);

INCLUDE_ASM("asm/nonmatchings/AB160", func_800BB454_AB304);

INCLUDE_ASM("asm/nonmatchings/AB160", func_800BB5BC_AB46C);

INCLUDE_ASM("asm/nonmatchings/AB160", func_800BB7D4_AB684);

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
