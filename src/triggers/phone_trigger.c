#include "common.h"
#include "gamestate.h"
#include "system/task_scheduler.h"
#include "triggers/event_trigger.h"

void checkPhoneLocationDiscovery(LocationDiscoveryTrigger *);

char PhoneLabel[] = "PHONE";

void initPhoneDiscoveryTrigger(LocationDiscoveryTrigger *trigger) {
    trigger->locationId = PHONE_EVENT_ID;
    trigger->unk6 = -0x68;
    trigger->unk1 = 0;
    trigger->unk4 = 0;
    trigger->unk8 = 0;
    trigger->locationLabel = PhoneLabel;

    setCallback(&checkPhoneLocationDiscovery);
}

void checkPhoneLocationDiscovery(LocationDiscoveryTrigger *trigger) {
    s16 playerYaw;
    s16 normalizedYaw;
    s16 minAngle;
    s16 maxAngle;
    u8 locationId;
    GameState *gameState;

    gameState = getCurrentAllocation();
    // Only check if player is above certain Y threshold
    if (gameState->unk3F8 > 0x760000) {
        playerYaw = gameState->unk3F4;
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
                // Check if player's X position is within discovery range
                if ((u16)(gameState->unk3FC - 0xC01) < 0x7FF) {
                    gameState->locationDiscovered = 1;
                    gameState->discoveredLocationId = trigger->locationId;
                }
            }
        }
    }
}
