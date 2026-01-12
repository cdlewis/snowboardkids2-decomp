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
    GameState *state = (GameState *)getCurrentAllocation();
    u32 positionCheck;
    s16 rawAngle;
    s16 playerAngle;
    u8 locationId;
    s16 minAngle;
    s16 maxAngle;

    positionCheck = state->unk3F8 + 0xFFB3FFFF;
    if (positionCheck >= 0xBFFFF) {
        return;
    }

    rawAngle = state->unk3F4;
    playerAngle = rawAngle;
    if (rawAngle >= 0x1001) {
        playerAngle -= 0x2000;
    }

    locationId = trigger->locationId;
    minAngle = ((s16 *)D_8008D6C4_8E2C4)[locationId * 2];
    if (playerAngle < minAngle) {
        maxAngle = ((s16 *)D_8008D6C4_8E2C4)[(locationId * 2) + 1];
        if (maxAngle < playerAngle) {
            if (((u16)(state->unk3FC - 0xC01)) < 0x7FF) {
                state->locationDiscovered = 1;
                state->discoveredLocationId = trigger->locationId;
            }
        }
    }
}
