#include "common.h"
#include "gamestate.h"
#include "task_scheduler.h"

extern u8 D_8008D714_8E314[];
extern u8 D_8008D6C4_8E2C4[];

typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[2];
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ u8 padA[2];
    /* 0x0C */ u8 *unkC;
} TaskData_198B0;

void func_80018D04_19904(TaskData_198B0 *arg0);

void func_80018CB0_198B0(TaskData_198B0 *arg0) {
    u8 temp = arg0->unk0;
    arg0->unk1 = 0;
    arg0->unk4 = 0;
    arg0->unk6 = -0x68;
    arg0->unk8 = 0;
    arg0->unkC = &D_8008D714_8E314[temp * 20];
    setCallback(func_80018D04_19904);
}

void func_80018D04_19904(TaskData_198B0 *arg0) {
    GameState *state = (GameState *)getCurrentAllocation();
    s16 val;
    u8 index;
    s16 min;
    s16 max;

    if (state->unk3F8 <= 0x6E0000) {
        return;
    }

    val = state->unk3F4;
    if (val >= 0x1001) {
        val -= 0x2000;
    }

    index = arg0->unk0;
    min = ((s16 *)D_8008D6C4_8E2C4)[index * 2];
    if (val < min) {
        max = ((s16 *)D_8008D6C4_8E2C4)[(index * 2) + 1];
        if (max < val) {
            if (((u16)(state->unk3FC - 0xC01)) < 0x7FF) {
                state->unk424 = 1;
                state->unk425 = arg0->unk0;
            }
        }
    }
}
