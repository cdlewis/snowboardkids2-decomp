#include "common.h"
#include "event_trigger.h"
#include "gamestate.h"
#include "task_scheduler.h"

void ClocktowerTriggerCheck(EventTrigger *);

char ClocktowerLabel[] = "CLOCKTOWER";
s32 D_8008D7FC_8E3FC = 0;

void ClocktowerTriggerInit(EventTrigger *trigger) {
    trigger->eventId = CLOCKTOWER_EVENT_ID;
    trigger->unk6 = -0x68;
    trigger->unk1 = 0;
    trigger->unk4 = 0;
    trigger->unk8 = 0;
    trigger->unkC = &ClocktowerLabel;
    setCallback(&ClocktowerTriggerCheck);
}

void ClocktowerTriggerCheck(EventTrigger *trigger) {
    GameState *state = (GameState *)getCurrentAllocation();
    u32 positionCheck;
    s16 rawAngle;
    s16 playerAngle;
    u8 eventId;
    s16 minAngle;
    s16 maxAngle;

    positionCheck = state->unk3F8 + 0xFFB3FFFF;
    if (positionCheck >= 0xBFFFF) {
        return;
    }

    rawAngle = state->unk3F4;
    playerAngle = rawAngle;
    if (rawAngle >= 0x1001) {
        playerAngle -= 0x2000;
    }

    eventId = trigger->eventId;
    minAngle = ((s16 *)D_8008D6C4_8E2C4)[eventId * 2];
    if (playerAngle < minAngle) {
        maxAngle = ((s16 *)D_8008D6C4_8E2C4)[(eventId * 2) + 1];
        if (maxAngle < playerAngle) {
            if (((u16)(state->unk3FC - 0xC01)) < 0x7FF) {
                state->unk424 = 1;
                state->unk425 = trigger->eventId;
            }
        }
    }
}
