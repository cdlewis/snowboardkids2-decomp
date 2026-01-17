#include "common.h"
#include "event_trigger.h"
#include "gamestate.h"
#include "task_scheduler.h"

void checkPhoneLocationDiscovery(LocationDiscoveryTrigger *);

char PhoneLabel[] = "PHONE";

void initPhoneDiscoveryTrigger(LocationDiscoveryTrigger *trigger) {
    trigger->locationId = PHONE_EVENT_ID;
    trigger->unk6 = -0x68;
    trigger->unk1 = 0;
    trigger->unk4 = 0;
    trigger->unk8 = 0;
    trigger->locationLabel = &PhoneLabel;

    setCallback(&checkPhoneLocationDiscovery);
}

void checkPhoneLocationDiscovery(LocationDiscoveryTrigger *trigger) {
    s16 rawAngle;
    s16 playerAngle;
    u8 locationId;
    GameState *state;
    s32 minAngle;
    s32 maxAngle;

    state = (GameState *)getCurrentAllocation();
    // Only check if player is above certain Y threshold
    if (state->unk3F8 > 0x760000) {
        rawAngle = state->unk3F4;
        // Normalize angle to range -0x1000 to 0x1000
        playerAngle = rawAngle;
        if (rawAngle >= 0x1001) {
            playerAngle -= 0x2000;
        }
        locationId = trigger->locationId;
        // Get min/max angle bounds for this location from the angle bounds table
        minAngle = ((s16 *)D_8008D6C4_8E2C4)[locationId * 2];
        if (playerAngle < minAngle) {
            maxAngle = ((s16 *)D_8008D6C4_8E2C4)[(locationId * 2) + 1];
            if (playerAngle > maxAngle) {
                // Check if player's X position is within discovery range
                u32 xDist = state->unk3FC - 0xC01;
                if (xDist < 0x7FF) {
                    state->locationDiscovered = 1;
                    state->discoveredLocationId = trigger->locationId;
                }
            }
        }
    }
}
