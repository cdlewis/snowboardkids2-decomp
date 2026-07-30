#include "common.h"
#include "gamestate.h"
#include "system/task_scheduler.h"
#include "triggers/event_trigger.h"

extern char storyMapLocationNames[][20];

void checkGenericLocationDiscovery(LocationDiscoveryTrigger *);

void initGenericDiscoveryTrigger(LocationDiscoveryTrigger *trigger) {
    u8 eventId = trigger->locationId;
    trigger->discoveryState = 0;
    trigger->labelOffsetX = 0;
    trigger->labelOffsetY = -0x68;
    trigger->labelOffsetZ = 0;
    trigger->locationLabel = storyMapLocationNames[eventId];
    setCallback(checkGenericLocationDiscovery);
}

void checkGenericLocationDiscovery(LocationDiscoveryTrigger *trigger) {
    s16 playerYaw;
    s16 normalizedYaw;
    s16 minAngle;
    s16 maxAngle;
    u8 locationId;
    GameState *gameState;

    gameState = getCurrentAllocation();
    // Only check if player is above certain Y threshold
    if (gameState->storyMapCameraOrbitRadius > 0x6E0000) {
        playerYaw = gameState->storyMapCameraOrbitAngle;
        // Normalize angle to range -0x1000 to 0x1000
        normalizedYaw = playerYaw;
        if (playerYaw >= 0x1001) {
            normalizedYaw -= 0x2000;
        }
        // Get angle bounds for this location from the angle bounds table
        locationId = trigger->locationId;
        minAngle = ((s16 *)storyMapAngleBounds)[locationId * 2];
        if (normalizedYaw < minAngle) {
            maxAngle = ((s16 *)storyMapAngleBounds)[(locationId * 2) + 1];
            if (normalizedYaw > maxAngle) {
                // Additional secondary-angle gate unique to the generic trigger
                if ((u16)(gameState->storyMapCameraViewAngle - 0xC01) < 0x7FF) {
                    gameState->locationDiscovered = 1;
                    gameState->discoveredLocationId = trigger->locationId;
                }
            }
        }
    }
}
