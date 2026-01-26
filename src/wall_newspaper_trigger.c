#include "common.h"
#include "event_trigger.h"
#include "gamestate.h"
#include "task_scheduler.h"

void checkNewspaperLocationDiscovery(LocationDiscoveryTrigger *);

u8 WallNewspaperLabel[] = "WALL NEWSPAPER";

void initNewspaperDiscoveryTrigger(LocationDiscoveryTrigger *trigger) {
    trigger->locationId = WALL_NEWSPAPER_EVENT_ID;
    trigger->unk6 = -0x68;
    trigger->unk1 = 0;
    trigger->unk4 = 0;
    trigger->unk8 = 0;
    trigger->locationLabel = &WallNewspaperLabel;

    setCallback(&checkNewspaperLocationDiscovery);
}

void checkNewspaperLocationDiscovery(LocationDiscoveryTrigger *trigger) {
    s16 playerYaw;
    s16 normalizedYaw;
    s16 minAngle;
    s16 maxAngle;
    u8 locationId;
    GameState *state;

    state = (GameState *)getCurrentAllocation();
    // Only check if player is above certain Y threshold
    if (state->unk3F8 > 0x760000) {
        playerYaw = state->unk3F4;
        // Normalize angle to range -0x1000 to 0x1000
        normalizedYaw = playerYaw;
        if (playerYaw >= 0x1001) {
            normalizedYaw -= 0x2000;
        }
        // Get angle bounds for this location from the angle bounds table
        locationId = trigger->locationId;
        minAngle = ((s16 *)D_8008D6C4_8E2C4)[locationId * 2];
        if (normalizedYaw < minAngle) {
            maxAngle = ((s16 *)D_8008D6C4_8E2C4)[(locationId * 2) + 1];
            if (normalizedYaw > maxAngle) {
                // Note: temp is computed but the result is not used
                u32 temp = state->unk3FC - 0xC01;
                state->locationDiscovered = 1;
                state->discoveredLocationId = trigger->locationId;
            }
        }
    }
}
