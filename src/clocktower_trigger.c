#include "common.h"
#include "event_trigger.h"
#include "gamestate.h"
#include "task_scheduler.h"

void ClocktowerTriggerCheck(EventTrigger *);
extern u8 D_8008D6C4_8E2C4[];

char ClocktowerLabel[] = "CLOCKTOWER";
s32 D_8008D7FC_8E3FC = 0;

void ClocktowerTriggerInit(EventTrigger *arg0) {
    arg0->eventId = CLOCKTOWER_EVENT_ID;
    arg0->unk6 = -0x68;
    arg0->unk1 = 0;
    arg0->unk4 = 0;
    arg0->unk8 = 0;
    arg0->unkC = &ClocktowerLabel;
    setCallback(&ClocktowerTriggerCheck);
}

void ClocktowerTriggerCheck(EventTrigger *trigger) {
    GameState *state = (GameState *)getCurrentAllocation();
    u32 temp;
    s16 angle;
    u8 triggerId;
    s16 minAngle;
    s16 maxAngle;
    temp = state->unk3F8 + 0xFFB3FFFF;
    if (temp >= 0xBFFFF) {
        return;
    }
    angle = state->unk3F4;
    if (angle >= 0x1001) {
        angle -= 0x2000;
    }
    triggerId = trigger->eventId;
    minAngle = ((s16 *)D_8008D6C4_8E2C4)[triggerId * 2];
    if (angle < minAngle) {
        maxAngle = ((s16 *)D_8008D6C4_8E2C4)[(triggerId * 2) + 1];
        if (maxAngle < angle) {
            if (((u16)(state->unk3FC - 0xC01)) < 0x7FF) {
                state->unk424 = 1;
                state->unk425 = trigger->eventId;
            }
        }
    }
}
