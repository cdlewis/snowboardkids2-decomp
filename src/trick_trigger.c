#include "common.h"
#include "event_trigger.h"
#include "gamestate.h"
#include "task_scheduler.h"

void checkTrickLocationDiscovery(LocationDiscoveryTrigger *);

char TrickLabel[] = "TRICK";

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
    s16 rawYaw;
    s16 normalizedYaw;
    u8 locationId;
    GameState *gameState;
    s32 minAngle;
    s32 maxAngle;

    gameState = (GameState *)getCurrentAllocation();
    if (gameState->unk3F8 > 0x800000) {
        rawYaw = gameState->unk3F4;
        normalizedYaw = rawYaw;
        if (rawYaw >= 0x1001) {
            normalizedYaw -= 0x2000;
        }
        locationId = trigger->locationId;
        minAngle = ((s16 *)D_8008D6C4_8E2C4)[locationId * 2];
        if (normalizedYaw < minAngle) {
            maxAngle = ((s16 *)D_8008D6C4_8E2C4)[locationId * 2 + 1];
            if (maxAngle < normalizedYaw) {
                gameState->locationDiscovered = 1;
                gameState->discoveredLocationId = trigger->locationId;
            }
        }
    }
}
