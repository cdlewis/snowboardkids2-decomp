#include "common.h"
#include "event_trigger.h"
#include "gamestate.h"
#include "task_scheduler.h"

extern u8 D_8008D6C4_8E2C4[];
void TrickTriggerCheck(EventTrigger *);

u8 TrickLabel[] = "TRICK";

void TrickTriggerInit(EventTrigger *arg0) {
    arg0->eventId = TRICK_EVENT_ID;
    arg0->unk6 = -0x68;
    arg0->unk1 = 0;
    arg0->unk4 = 0;
    arg0->unk8 = 0;
    arg0->unkC = &TrickLabel;
    setCallback(&TrickTriggerCheck);
}

void TrickTriggerCheck(EventTrigger *trigger) {
    s16 rawAngle;
    s16 angle;
    u8 triggerId;
    GameState *state;
    s32 minAngle;
    s32 maxAngle;

    state = (GameState *)getCurrentAllocation();
    if (state->unk3F8 > 0x800000) {
        rawAngle = state->unk3F4;
        angle = rawAngle;
        if (rawAngle >= 0x1001) {
            angle -= 0x2000;
        }
        triggerId = trigger->eventId;
        minAngle = ((s16 *)D_8008D6C4_8E2C4)[triggerId * 2];
        if (angle < minAngle) {
            maxAngle = ((s16 *)D_8008D6C4_8E2C4)[triggerId * 2 + 1];
            if (maxAngle < angle) {
                state->unk424 = 1;
                state->unk425 = trigger->eventId;
            }
        }
    }
}
