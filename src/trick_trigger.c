#include "common.h"
#include "event_trigger.h"
#include "gamestate.h"
#include "task_scheduler.h"

void checkTrickLocationDiscovery(LocationDiscoveryTrigger *);

u8 TrickLabel[] = "TRICK";

void initTrickDiscoveryTrigger(LocationDiscoveryTrigger *trigger) {
    trigger->locationId = TRICK_EVENT_ID;
    trigger->unk6 = -0x68;
    trigger->unk1 = 0;
    trigger->unk4 = 0;
    trigger->unk8 = 0;
    trigger->locationLabel = &TrickLabel;
    setCallback(&checkTrickLocationDiscovery);
}

void checkTrickLocationDiscovery(LocationDiscoveryTrigger *trigger) {
    s16 rawAngle;
    s16 playerAngle;
    u8 locationId;
    GameState *state;
    s32 minAngle;
    s32 maxAngle;

    state = (GameState *)getCurrentAllocation();
    if (state->unk3F8 > 0x800000) {
        rawAngle = state->unk3F4;
        playerAngle = rawAngle;
        if (rawAngle >= 0x1001) {
            playerAngle -= 0x2000;
        }
        locationId = trigger->locationId;
        minAngle = ((s16 *)D_8008D6C4_8E2C4)[locationId * 2];
        if (playerAngle < minAngle) {
            maxAngle = ((s16 *)D_8008D6C4_8E2C4)[locationId * 2 + 1];
            if (maxAngle < playerAngle) {
                state->locationDiscovered = 1;
                state->discoveredLocationId = trigger->locationId;
            }
        }
    }
}
