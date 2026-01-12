#include "common.h"
#include "event_trigger.h"
#include "gamestate.h"
#include "task_scheduler.h"

void checkTrickLocationDiscovery(LocationDiscoveryTrigger *);

u8 TrickLabel[] = "TRICK";

void initTrickDiscoveryTrigger(LocationDiscoveryTrigger *arg0) {
    arg0->locationId = TRICK_EVENT_ID;
    arg0->unk6 = -0x68;
    arg0->unk1 = 0;
    arg0->unk4 = 0;
    arg0->unk8 = 0;
    arg0->locationLabel = &TrickLabel;
    setCallback(&checkTrickLocationDiscovery);
}

void checkTrickLocationDiscovery(LocationDiscoveryTrigger *trigger) {
    s16 rawAngle;
    s16 angle;
    u8 locationId;
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
        locationId = trigger->locationId;
        minAngle = ((s16 *)D_8008D6C4_8E2C4)[locationId * 2];
        if (angle < minAngle) {
            maxAngle = ((s16 *)D_8008D6C4_8E2C4)[locationId * 2 + 1];
            if (maxAngle < angle) {
                state->locationDiscovered = 1;
                state->discoveredLocationId = trigger->locationId;
            }
        }
    }
}
