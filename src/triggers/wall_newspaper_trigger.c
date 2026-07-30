#include "common.h"
#include "gamestate.h"
#include "system/task_scheduler.h"
#include "triggers/event_trigger.h"

char WallNewspaperLabel[] = "WALL NEWSPAPER";

void checkNewspaperLocationDiscovery(LocationDiscoveryTrigger *);

void initNewspaperDiscoveryTrigger(LocationDiscoveryTrigger *trigger) {
    trigger->locationId = WALL_NEWSPAPER_EVENT_ID;
    trigger->labelOffsetY = -0x68;
    trigger->discoveryState = 0;
    trigger->labelOffsetX = 0;
    trigger->labelOffsetZ = 0;
    trigger->locationLabel = WallNewspaperLabel;

    setCallback(&checkNewspaperLocationDiscovery);
}

void checkNewspaperLocationDiscovery(LocationDiscoveryTrigger *trigger) {
    s16 cameraOrbitAngle;
    s16 normalizedOrbitAngle;
    s16 maxDiscoveryAngle;
    s16 minDiscoveryAngle;
    u8 locationId;
    GameState *gameState;

    gameState = getCurrentAllocation();
    // Only check when the camera is far enough from the center of the story map.
    if (gameState->storyMapCameraOrbitRadius > 0x760000) {
        cameraOrbitAngle = gameState->storyMapCameraOrbitAngle;
        // Normalize the camera angle to the signed -0x1000 to 0x1000 range.
        normalizedOrbitAngle = cameraOrbitAngle;
        if (cameraOrbitAngle >= 0x1001) {
            normalizedOrbitAngle -= 0x2000;
        }
        locationId = trigger->locationId;
        maxDiscoveryAngle = storyMapAngleBounds[locationId * 2];
        if (normalizedOrbitAngle < maxDiscoveryAngle) {
            minDiscoveryAngle = storyMapAngleBounds[(locationId * 2) + 1];
            if (normalizedOrbitAngle > minDiscoveryAngle) {
                gameState->locationDiscovered = 1;
                gameState->discoveredLocationId = trigger->locationId;
            }
        }
    }
}
