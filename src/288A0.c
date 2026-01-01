#include "288A0.h"
#include "29200.h"
#include "2C8F0.h"
#include "3E160.h"
#include "6E840.h"
#include "rand.h"
#include "task_scheduler.h"

extern ColorData gMenuLightColor;
extern ColorData gMenuAmbientColor;

extern u8 D_8008DF00_8EB00[][10];
extern u8 D_8008DF3C_8EB3C[][8];
extern u16 D_8009ADE0_9B9E0;

typedef struct {
    u8 pad0[0x9];
    u8 unk9;
} GameConfigLocal_288A0;

extern GameConfigLocal_288A0 *D_800AFE8C_A71FC;

void nullRandomEventCallback(void);

typedef struct {
    u8 pad0[0x41C];
    s32 hasRandomEvent;
    u8 regularEventType;
    u8 pad421[0xB];
    u8 rareEventType;
} StoryMapEventState;

typedef struct {
    u8 pad0[0x5C];
    u8 eventTypeIndex;
} RegularEventTask;

typedef struct {
    u8 pad0[0xD4];
    u8 eventTypeIndex;
} RareEventTask;

void initMenuCameraNode(Node_70B00 *node, s32 slotIndex, s32 priority, s32 isSecondary) {
    s8 lightSettings[0x20];
    s32 secondaryFlag;

    secondaryFlag = isSecondary & 0xFF;
    if (secondaryFlag == 0) {
        func_8006FAA4_706A4(node, 0, slotIndex & 0xFF, priority & 0xFF, 1);
    } else {
        func_8006FAA4_706A4(node, 0, slotIndex & 0xFF, priority & 0xFF, 0);
    }

    func_8006F9BC_705BC(node, 1.0f, 1.0f);
    func_8006FEF8_70AF8(node, ((slotIndex & 0xFF) + 1));
    setModelCameraTransform(node, 0, 0, -0x98, -0x70, 0x97, 0x6F);
    func_8006FA0C_7060C(node, 50.0f, 1.3333334f, 10.0f, 4000.0f);
    func_8006FE48_70A48(node->id, 0x3D4, 0x3E6, 0x64, 0x64, 0x64);
    func_8006BEDC_6CADC(&lightSettings, 0, 0, 0x200000, 0, 0, 0);
    func_8006FD3C_7093C(node->id, &lightSettings);
    func_8006FE28_70A28(node, 0, 0, 0);
    func_8006FDA0_709A0(node, 0, 0);
    func_8006FC70_70870(node->id, 1, &gMenuLightColor, &gMenuAmbientColor);
}

INCLUDE_ASM("asm/nonmatchings/288A0", func_80027E04_28A04);

INCLUDE_ASM("asm/nonmatchings/288A0", func_80028074_28C74);

void initStoryMapRandomEvent(u8 *eventTypeOut) {
    StoryMapEventState *state;
    u8 eventTypeIndex;
    u8 randomIndex;
    RegularEventTask *regularTask;
    RareEventTask *rareTask;

    state = getCurrentAllocation();
    state->hasRandomEvent = 0;
    state->regularEventType = 0xFF;

    if ((randA() & 7) == 7) {
        randomIndex = randB();
        *eventTypeOut = D_8008DF00_8EB00[D_800AFE8C_A71FC->unk9][randomIndex % 10] - 1;
        rareTask = scheduleTask(func_8002BCF0_2C8F0, 0, 0, 0x5B);
        rareTask->eventTypeIndex = *eventTypeOut;
        state->rareEventType = *eventTypeOut;
        goto check_event;
    }

    eventTypeIndex = D_8008DF3C_8EB3C[D_800AFE8C_A71FC->unk9][D_8009ADE0_9B9E0 & 7];
    if (eventTypeIndex != 0) {
        regularTask = scheduleTask(initStoryMapItem, 0, 0, 0x5B);
        regularTask->eventTypeIndex = eventTypeIndex - 1;
        state->regularEventType = eventTypeIndex - 1;
    }

check_event:
    if (eventTypeIndex != 0) {
        state->hasRandomEvent = 1;
        setCallback(nullRandomEventCallback);
    } else {
        func_80069CF8_6A8F8();
    }
}

void nullRandomEventCallback(void) {
}
