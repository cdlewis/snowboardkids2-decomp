#include "common.h"

#define PHONE_EVENT_ID 6
#define WALL_NEWSPAPER_EVENT_ID 7
#define CLOCKTOWER_EVENT_ID 8

typedef struct {
    s8 unk0;
    s8 unk1;
    char padding[2];
    s16 unk4;
    s16 unk6;
    s16 unk8;
    void *unkC;
} EventTrigger;