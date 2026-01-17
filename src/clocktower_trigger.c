#include "common.h"
#include "event_trigger.h"
#include "gamestate.h"
#include "task_scheduler.h"

void checkClocktowerLocationDiscovery(LocationDiscoveryTrigger *);

char ClocktowerLabel[] = "CLOCKTOWER";
s32 D_8008D7FC_8E3FC = 0;

void initClocktowerDiscoveryTrigger(LocationDiscoveryTrigger *trigger) {
    trigger->locationId = CLOCKTOWER_EVENT_ID;
    trigger->unk6 = -0x68;
    trigger->unk1 = 0;
    trigger->unk4 = 0;
    trigger->unk8 = 0;
    trigger->locationLabel = &ClocktowerLabel;
    setCallback(&checkClocktowerLocationDiscovery);
}

void checkClocktowerLocationDiscovery(LocationDiscoveryTrigger *trigger) {
    GameState *gameState = (GameState *)getCurrentAllocation();
    u32 positionCheck;
    s16 yaw;
    s16 normalizedYaw;
    u8 locationId;
    s16 minAngle;
    s16 maxAngle;

    // Only check if player is at specific Y position range
    // (gameState->unk3F8 should be between 0x4C0000 and 0x580000)
    positionCheck = gameState->unk3F8 + 0xFFB3FFFF;
    if (positionCheck >= 0xBFFFF) {
        return;
    }

    yaw = gameState->unk3F4;
    // Normalize angle to range -0x1000 to 0x1000
    normalizedYaw = yaw;
    if (yaw >= 0x1001) {
        normalizedYaw -= 0x2000;
    }

    locationId = trigger->locationId;
    minAngle = ((s16 *)D_8008D6C4_8E2C4)[locationId * 2];
    if (normalizedYaw < minAngle) {
        maxAngle = ((s16 *)D_8008D6C4_8E2C4)[(locationId * 2) + 1];
        if (maxAngle < normalizedYaw) {
            // Check if player's X position is within discovery range
            if (((u16)(gameState->unk3FC - 0xC01)) < 0x7FF) {
                gameState->locationDiscovered = 1;
                gameState->discoveredLocationId = trigger->locationId;
            }
        }
    }
}
