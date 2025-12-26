#include "common.h"

#define PHONE_EVENT_ID 6
#define WALL_NEWSPAPER_EVENT_ID 7
#define CLOCKTOWER_EVENT_ID 8
#define TRICK_EVENT_ID 9

typedef struct {
    s16 minAngle;
    s16 maxAngle;
} TriggerAngleBounds;

extern u8 D_8008D6C4_8E2C4[];
#define TriggerAngleBoundsTable ((TriggerAngleBounds *)D_8008D6C4_8E2C4)

typedef struct {
    s8 eventId;
    s8 unk1;
    char padding[2];
    s16 unk4;
    s16 unk6;
    s16 unk8;
    void *unkC;
} EventTrigger;