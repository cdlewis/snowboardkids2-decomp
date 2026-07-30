#pragma once

#include "common.h"

#define PHONE_EVENT_ID 6
#define WALL_NEWSPAPER_EVENT_ID 7
#define CLOCKTOWER_EVENT_ID 8
#define TRICK_EVENT_ID 9

typedef struct {
    s16 minAngle;
    s16 maxAngle;
} TriggerAngleBounds;

extern u8 storyMapAngleBounds[];
#define TriggerAngleBoundsTable ((TriggerAngleBounds *)storyMapAngleBounds)

typedef struct {
    /* 0x00 */ s8 locationId;
    /* 0x01 */ s8 discoveryState;
    /* 0x02 */ u8 padding[2];
    /* 0x04 */ s16 labelOffsetX;
    /* 0x06 */ s16 labelOffsetY;
    /* 0x08 */ s16 labelOffsetZ;
    /* 0x0A */ u8 padding2[2];
    /* 0x0C */ char *locationLabel;
} LocationDiscoveryTrigger;
