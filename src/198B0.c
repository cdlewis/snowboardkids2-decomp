#include "common.h"
#include "event_trigger.h"
#include "gamestate.h"
#include "task_scheduler.h"

extern u8 D_8008D714_8E314[];
extern u8 D_8008D6C4_8E2C4[];

void func_80018D04_19904(EventTrigger *arg0);

void func_80018CB0_198B0(EventTrigger *arg0) {
    u8 eventId = arg0->unk0;
    arg0->unk1 = 0;
    arg0->unk4 = 0;
    arg0->unk6 = -0x68;
    arg0->unk8 = 0;
    arg0->unkC = &D_8008D714_8E314[eventId * 20];
    setCallback(func_80018D04_19904);
}

void func_80018D04_19904(EventTrigger *trigger) {
    GameState *state = (GameState *)getCurrentAllocation();
    s16 angle;
    u8 triggerId;
    s16 minAngle;
    s16 maxAngle;

    if (state->unk3F8 <= 0x6E0000) {
        return;
    }

    angle = state->unk3F4;
    if (angle >= 0x1001) {
        angle -= 0x2000;
    }

    triggerId = trigger->unk0;
    minAngle = ((s16 *)D_8008D6C4_8E2C4)[triggerId * 2];
    if (angle < minAngle) {
        maxAngle = ((s16 *)D_8008D6C4_8E2C4)[(triggerId * 2) + 1];
        if (maxAngle < angle) {
            if (((u16)(state->unk3FC - 0xC01)) < 0x7FF) {
                state->unk424 = 1;
                state->unk425 = trigger->unk0;
            }
        }
    }
}
