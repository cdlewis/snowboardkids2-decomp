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
    s16 rawAngle;
    s16 playerAngle;
    u8 locationId;
    GameState *state;
    s32 minAngle;
    s32 maxAngle;

    state = (GameState *)getCurrentAllocation();
    if (state->unk3F8 > 0x760000) {
        rawAngle = state->unk3F4;
        playerAngle = rawAngle;
        if (rawAngle >= 0x1001) {
            playerAngle -= 0x2000;
        }
        locationId = trigger->locationId;
        minAngle = ((s16 *)D_8008D6C4_8E2C4)[locationId * 2];
        if (playerAngle < minAngle) {
            maxAngle = ((s16 *)D_8008D6C4_8E2C4)[(locationId * 2) + 1];
            if (playerAngle > maxAngle) {
                u32 temp = state->unk3FC - 0xC01;
                state->locationDiscovered = 1;
                state->discoveredLocationId = trigger->locationId;
            }
        }
    }
}
