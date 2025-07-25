#include "common.h"
#include "event_trigger.h"
#include "gamestate.h"
#include "task_scheduler.h"

void ClocktowerTriggerCheck(void *);
extern u8 D_8008D6C4_8E2C4[];

char ClocktowerLabel[] = "CLOCKTOWER";
s32 D_8008D7FC_8E3FC = 0;

void ClocktowerTriggerInit(EventTrigger *arg0) {
    arg0->unk0 = CLOCKTOWER_EVENT_ID;
    arg0->unk6 = -0x68;
    arg0->unk1 = 0;
    arg0->unk4 = 0;
    arg0->unk8 = 0;
    arg0->unkC = &ClocktowerLabel;
    setCallback(&ClocktowerTriggerCheck);
}

void ClocktowerTriggerCheck(void *arg0) {
    GameState *state = (GameState *)getCurrentAllocation();
    u32 temp;
    s16 val;
    u8 index;
    s16 min;
    s16 max;
    temp = state->unk3F8 + 0xFFB3FFFF;
    if (temp >= 0xBFFFF) {
        return;
    }
    val = (*state).unk3F4;
    if (val >= 0x1001) {
        val -= 0x2000;
    }
    index = *((u8 *)arg0);
    min = ((s16 *)D_8008D6C4_8E2C4)[index * 2];
    if (val < min) {
        max = ((s16 *)D_8008D6C4_8E2C4)[(index * 2) + 1];
        if (max < val) {
            if (((u16)(state->unk3FC - 0xC01)) < 0x7FF) {
                state->unk424 = 1;
                state->unk425 = *((u8 *)arg0);
            }
        }
    }
}
