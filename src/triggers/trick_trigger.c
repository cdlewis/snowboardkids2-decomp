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
    s16 playerYaw;
    s16 normalizedYaw;
    u8 locationId;
    GameState *gameState;
    s16 minAngle;
    s16 maxAngle;

    gameState = getCurrentAllocation();
    if (gameState->unk3F8 > 0x800000) {
        playerYaw = gameState->unk3F4;
        normalizedYaw = playerYaw;
        if (playerYaw >= 0x1001) {
            normalizedYaw -= 0x2000;
        }
        locationId = trigger->locationId;
        minAngle = ((s16 *)storyMapAngleBounds)[locationId * 2];
        if (normalizedYaw < minAngle) {
            maxAngle = ((s16 *)storyMapAngleBounds)[locationId * 2 + 1];
            if (maxAngle < normalizedYaw) {
                gameState->locationDiscovered = 1;
                gameState->discoveredLocationId = trigger->locationId;
            }
        }
    }
}
