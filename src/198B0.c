#include "common.h"
#include "event_trigger.h"
#include "gamestate.h"
#include "task_scheduler.h"

extern u8 D_8008D714_8E314[];

void GenericTriggerCheck(EventTrigger *);

void GenericTriggerInit(EventTrigger *arg0) {
    u8 eventId = arg0->eventId;
    arg0->unk1 = 0;
    arg0->unk4 = 0;
    arg0->unk6 = -0x68;
    arg0->unk8 = 0;
    arg0->unkC = &D_8008D714_8E314[eventId * 20];
    setCallback(GenericTriggerCheck);
}

void GenericTriggerCheck(EventTrigger *trigger) {
    GameState *state = (GameState *)getCurrentAllocation();
    s16 rawAngle;
    s16 playerAngle;
    u8 eventId;
    s16 minAngle;
    s16 maxAngle;

    if (state->unk3F8 <= 0x6E0000) {
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
