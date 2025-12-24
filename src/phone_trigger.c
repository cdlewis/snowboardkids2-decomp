#include "common.h"
#include "event_trigger.h"
#include "gamestate.h"
#include "task_scheduler.h"

void PhoneTriggerCheck(EventTrigger *);
extern u8 D_8008D6C4_8E2C4[];

char PhoneLabel[] = "PHONE";

void PhoneTriggerInit(EventTrigger *arg0) {
    arg0->unk0 = PHONE_EVENT_ID;
    arg0->unk6 = -0x68;
    arg0->unk1 = 0;
    arg0->unk4 = 0;
    arg0->unk8 = 0;
    arg0->unkC = &PhoneLabel;

    setCallback(&PhoneTriggerCheck);
}

void PhoneTriggerCheck(EventTrigger *trigger) {
    s16 rawAngle;
    s16 angle;
    u8 triggerId;
    GameState *state;
    s32 minAngle;
    s32 maxAngle;

    state = (GameState *)getCurrentAllocation();
    if (state->unk3F8 > 0x760000) {
        rawAngle = state->unk3F4;
        angle = rawAngle;
        if (rawAngle >= 0x1001) {
            angle -= 0x2000;
        }
        triggerId = trigger->unk0;
        minAngle = ((s16 *)D_8008D6C4_8E2C4)[triggerId * 2];
        if (angle < minAngle) {
            maxAngle = ((s16 *)D_8008D6C4_8E2C4)[(triggerId * 2) + 1];
            if (angle > maxAngle) {
                u32 temp = state->unk3FC - 0xC01;
                if (temp < 0x7FF) {
                    state->unk424 = 1;
                    state->unk425 = trigger->unk0;
                }
            }
        }
    }
}
