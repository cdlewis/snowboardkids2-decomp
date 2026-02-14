#include "common.h"
#include "event_trigger.h"
#include "gamestate.h"
#include "task_scheduler.h"

extern char D_8008D714_8E314[][20];

void checkGenericLocationDiscovery(LocationDiscoveryTrigger *);

void initGenericDiscoveryTrigger(LocationDiscoveryTrigger *trigger) {
    u8 eventId = trigger->locationId;
    trigger->unk1 = 0;
    trigger->unk4 = 0;
    trigger->unk6 = -0x68;
    trigger->unk8 = 0;
    trigger->locationLabel = &D_8008D714_8E314[eventId];
    setCallback(checkGenericLocationDiscovery);
}

void checkGenericLocationDiscovery(LocationDiscoveryTrigger *trigger) {
    GameState *gameState = (GameState *)getCurrentAllocation();
    s16 orbitAngle;
    s16 normalizedYaw;
    u8 locationId;
    s16 minAngle;
    s16 maxAngle;

    // Only check if player is above certain Y threshold
    if (gameState->unk3F8 <= 0x6E0000) {
        return;
    }

    orbitAngle = gameState->unk3F4;
    // Normalize angle to range -0x1000 to 0x1000
    normalizedYaw = orbitAngle;
    if (orbitAngle >= 0x1001) {
        normalizedYaw -= 0x2000;
    }

    locationId = trigger->locationId;
    // Get angle bounds for this location from the angle bounds table (TriggerAngleBounds)
    minAngle = ((s16 *)D_8008D6C4_8E2C4)[locationId * 2];
    if (normalizedYaw < minAngle) {
        maxAngle = ((s16 *)D_8008D6C4_8E2C4)[(locationId * 2) + 1];
        if (maxAngle < normalizedYaw) {
            if (((u16)(gameState->unk3FC - 0xC01)) < 0x7FF) {
                gameState->locationDiscovered = 1;
                gameState->discoveredLocationId = trigger->locationId;
            }
        }
    }
}
