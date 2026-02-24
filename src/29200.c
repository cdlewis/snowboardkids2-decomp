#include "29200.h"
#include "20F0.h"
#include "C570.h"
#include "common.h"
#include "geometry.h"
#include "rand.h"
#include "task_scheduler.h"

typedef struct {
    /* 0x00 */ void *model;
    /* 0x04 */ Transform3D matrix;
    /* 0x24 */ s32 moveTimer;
    /* 0x28 */ s16 *waypointData;
    /* 0x2C */ s16 rotationAngle;
    /* 0x2E */ s16 targetRotation;
    /* 0x30 */ u8 pad30[0x4];
    /* 0x34 */ u16 currentRotation;
    /* 0x36 */ u8 pad36[0x2];
    /* 0x38 */ u8 moveDirection;
    /* 0x39 */ u8 waypointCount;
    /* 0x3A */ u8 currentWaypoint;
    /* 0x3B */ u8 pad3B[0x15];
    /* 0x50 */ u16 animValue;
    /* 0x52 */ u16 animValue2;
    /* 0x54 */ u8 pad54[0x4];
    /* 0x58 */ u16 animValue3;
    /* 0x5A */ u16 animCounter;
    /* 0x5C */ u8 pad5C;
    /* 0x5D */ u8 itemType;
    /* 0x5E */ u8 pad5E[0x4];
    /* 0x62 */ u8 stateCounter;
} StoryMapItemTask;

typedef struct {
    u8 pad0[0x418];
    s16 unk418;
    u8 pad41A[7];
    u8 unk421;
    u8 unk422;
    u8 pad423[0x6];
    u8 unk429;
    u8 pad42A[0x3];
    u8 unk42D;
    u8 unk42E;
} AllocationData29200;

void initStoryMapItemMovement(StoryMapItemTask *);
void awaitStoryMapItemReady(void);
void awaitStoryMapItemRespawn(StoryMapItem *arg0);
void cleanupStoryMapItem(StoryMapItem *arg0);

extern void initStoryMapNpcJump(void);
extern void initStoryMapNpcWave(void);
extern void initStoryMapNpcFloatEffect(void);
extern void initStoryMapNpcLookAround(void);
extern void initStoryMapNpcThinkEffect(void);
extern void initStoryMapNpcIdle(void);
extern void initStoryMapNpcIdleNoAnim(void);
extern void initStoryMapNpcIdleNoAnim2(void);
extern void initStoryMapNpcIdleWithEffect(void);
extern void initStoryMapNpcIdleWithEffect2(void);
extern void initStoryMapNpcNod(void);
extern void initStoryMapNpcIdleRandomDelay(void);
extern void initStoryMapNpcIdleRandomDelayNoAnim(void);
extern void initStoryMapNpcIdleRandomDelayNoAnim2(void);
extern void initStoryMapNpcIdleRandomDelayNoAnim3(void);
extern void initStoryMapNpcTalk(void);
extern void initStoryMapNpcStretch(void);

u8 gStoryMapItemTypeTables[][16] = {
    { 0x00, 0x0D, 0x06, 0x02, 0x00, 0x0D, 0x06, 0x02, 0x00, 0x0D, 0x06, 0x02, 0x00, 0x0D, 0x06, 0x02 },
    { 0x03, 0x06, 0x09, 0x0D, 0x03, 0x06, 0x09, 0x0D, 0x03, 0x06, 0x09, 0x0D, 0x03, 0x06, 0x09, 0x0D },
    { 0x02, 0x0E, 0x0F, 0x07, 0x02, 0x0E, 0x0F, 0x07, 0x02, 0x0E, 0x0F, 0x07, 0x02, 0x0E, 0x0F, 0x07 },
    { 0x08, 0x0E, 0x04, 0x07, 0x08, 0x0E, 0x04, 0x07, 0x08, 0x0E, 0x04, 0x07, 0x08, 0x0E, 0x04, 0x07 },
    { 0x01, 0x05, 0x0A, 0x0B, 0x01, 0x05, 0x0A, 0x0B, 0x01, 0x05, 0x0A, 0x0B, 0x01, 0x05, 0x0A, 0x0B },
    { 0x09, 0x0A, 0x0E, 0x0F, 0x09, 0x0A, 0x0E, 0x0F, 0x09, 0x0A, 0x0E, 0x0F, 0x09, 0x0A, 0x0E, 0x0F },
    { 0x0C, 0x02, 0x03, 0x04, 0x0C, 0x02, 0x03, 0x04, 0x0C, 0x02, 0x03, 0x04, 0x0C, 0x02, 0x03, 0x04 },
    { 0x0D, 0x0E, 0x0F, 0x0D, 0x0E, 0x0F, 0x0D, 0x0E, 0x0F, 0x0D, 0x0E, 0x0F, 0x0D, 0x0E, 0x0F, 0x0D },
    { 0x0D, 0x0E, 0x0F, 0x0D, 0x0E, 0x0F, 0x0D, 0x0E, 0x0F, 0x0D, 0x0E, 0x0F, 0x0D, 0x0E, 0x0F, 0x0D },
};

s16 gStoryMapItemWaypointCounts[] = {
    0x0000, 0x0001, 0x0003, 0x0001, 0x0002, 0x0001, 0x0000, 0x0001, 0x0002,
    0x0004, 0x0004, 0x0000, 0x0001, 0x0002, 0x0001, 0x0000, 0x0002, 0x0000,
};

u16 gStoryMapItemWaypointDataSizes[] = {
    0x001A, 0x0028, 0x0020, 0x0020, 0x0022, 0x0000,
};

s32 gStoryMapItemAnimationTimers[] = {
    0x00018000, 0x00010000, 0x00012000, 0x00010000, 0x00011000, 0x00010000, 0x00010000, 0x00019000, 0x00010000,
    0x00010000, 0x00010000, 0x0001F000, 0x0001C000, 0x0002A000, 0x00023000, 0x0002A000, 0x00012000,
};

u16 gStoryMapItemAnimationValues[] = {
    0x0004, 0x0002, 0x0002, 0x0002, 0x0002, 0x001D, 0x0002, 0x0004, 0x0002,
    0x0002, 0x0002, 0x0016, 0x0012, 0x0009, 0x0009, 0x0009, 0x0002, 0x0000,
};

s32 gStoryMapItemUpdateCallbacks[] = {
    (s32)initStoryMapNpcJump,
    (s32)initStoryMapNpcWave,
    (s32)initStoryMapNpcFloatEffect,
    (s32)initStoryMapNpcLookAround,
    (s32)initStoryMapNpcThinkEffect,
    (s32)initStoryMapNpcIdle,
    (s32)initStoryMapNpcIdleNoAnim,
    (s32)initStoryMapNpcIdleNoAnim2,
    (s32)initStoryMapNpcIdleWithEffect,
    (s32)initStoryMapNpcIdleWithEffect2,
    (s32)initStoryMapNpcNod,
    (s32)initStoryMapNpcIdleRandomDelay,
    (s32)initStoryMapNpcIdleRandomDelayNoAnim,
    (s32)initStoryMapNpcIdleRandomDelayNoAnim2,
    (s32)initStoryMapNpcIdleRandomDelayNoAnim3,
    (s32)initStoryMapNpcTalk,
    (s32)initStoryMapNpcStretch,
};

u16 gStoryMapItemValues[] = {
    0x000F, 0x000F, 0x0013, 0x0013, 0x0013, 0x000F, 0x000F, 0x000F, 0x0013, 0x0000, 0x0002, 0x0304, 0x0506, 0x0907,
    0x0001, 0x0304, 0x0506, 0x0809, 0x0001, 0x0204, 0x0506, 0x0907, 0x0001, 0x0203, 0x0506, 0x0807, 0x0001, 0x0203,
    0x0406, 0x0907, 0x0001, 0x0203, 0x0405, 0x0807, 0x000D, 0x0602, 0x000D, 0x0602, 0x000D, 0x0602, 0x000D, 0x0602,
    0x0306, 0x090D, 0x0306, 0x090D, 0x0306, 0x090D, 0x0306, 0x090D, 0x020E, 0x0F07, 0x020E, 0x0F07, 0x020E, 0x0F07,
    0x020E, 0x0F07, 0x080E, 0x0407, 0x080E, 0x0407, 0x080E, 0x0407, 0x080E, 0x0407, 0x0105, 0x0A0B, 0x0105, 0x0A0B,
    0x0105, 0x0A0B, 0x0105, 0x0A0B, 0x090A, 0x0E0F, 0x090A, 0x0E0F, 0x090A, 0x0E0F, 0x090A, 0x0E0F, 0x0C02, 0x0304,
    0x0C02, 0x0304, 0x0C02, 0x0304, 0x0C02, 0x0304, 0x0D0E, 0x0F0D, 0x0E0F, 0x0D0E, 0x0F0D, 0x0E0F, 0x0D0E, 0x0F0D,
    0x0D0E, 0x0F0D, 0x0E0F, 0x0D0E, 0x0F0D, 0x0E0F, 0x0D0E, 0x0F0D, 0x000F, 0x000F, 0x0013, 0x0013, 0x0013, 0x000F,
    0x000F, 0x000F, 0x0013, 0x0000, 0x0000, 0x005C, 0x005C, 0x0060, 0x005B, 0x005C, 0x005E, 0x0074, 0x0000, 0x0074,
    0x0074, 0x0074, 0x0074, 0x0076, 0x008A, 0x008A, 0x0000, 0x0090, 0x008A, 0x008A, 0x008D, 0x00AA, 0x00A2, 0x00AA,
    0x0000, 0x00A2, 0x00A2, 0x00A5, 0x00BB, 0x00BB, 0x00BB, 0x00BB, 0x0000, 0x00BB, 0x00BE, 0x00D0, 0x00D1, 0x00D1,
    0x00D1, 0x00D0, 0x0000, 0x00D4, 0x0000, 0x005C, 0x005C, 0x0060, 0x005B, 0x005C, 0x0074, 0x0000, 0x0074, 0x0074,
    0x0074, 0x0074, 0x008A, 0x008A, 0x0000, 0x0090, 0x008A, 0x008A, 0x00A7, 0x00A2, 0x00A7, 0x0000, 0x00A2, 0x00A2,
    0x00BB, 0x00BB, 0x00BB, 0x00BB, 0x0000, 0x00BB, 0x00D0, 0x00D1, 0x00D1, 0x00D1, 0x00D0, 0x0000, 0x010B, 0x010B,
    0x010B, 0x010B, 0x010B, 0x010B,
};

u16 D_8008E32C_8EF2C[] = { 0x000D, 0x000E, 0x000F, 0xFFFF };
u16 D_8008E334_8EF34[] = { 0x000A, 0x000B, 0x000C, 0xFFFF };
u16 D_8008E33C_8EF3C[] = { 0x0014, 0x0015, 0x0016, 0xFFFF };
u16 D_8008E344_8EF44[] = { 0x0016, 0x0017, 0x0018, 0xFFFF };
u16 D_8008E34C_8EF4C[] = { 0x0011, 0x0012, 0x0013, 0xFFFF };

s32 D_8008E354_8EF54[] = { 0x0000FFFF,
                           0x001B001B,
                           0xFFFF0000,
                           0x00000000,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E344_8EF44,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E334_8EF34,
                           (s32)D_8008E32C_8EF2C,
                           0x00000000,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E334_8EF34,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           0x00000000,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E334_8EF34,
                           (s32)D_8008E34C_8EF4C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E34C_8EF4C,
                           0x00000000,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E334_8EF34,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           0x00000000,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E334_8EF34,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           0x00000000,
                           (s32)D_8008E334_8EF34,
                           0x00000000,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E344_8EF44,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           0x00000000,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           0x00000000,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E33C_8EF3C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E33C_8EF3C,
                           0x00000000,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           0x00000000,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           0x00000000,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           0x00000000,
                           0x00000000,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E354_8EF54,
                           0x00000000,
                           0x00000000,
                           0x00000000,
                           0x00000000,
                           0x00000000,
                           0x00000000,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E354_8EF54,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E354_8EF54,
                           (s32)D_8008E354_8EF54,
                           (s32)D_8008E354_8EF54,
                           (s32)D_8008E354_8EF54,
                           (s32)D_8008E32C_8EF2C,
                           (s32)D_8008E354_8EF54,
                           0x0000005C,
                           0x005E0000,
                           0x0000005E,
                           0x00000000,
                           0x00790074,
                           0x00000077,
                           0x00000000,
                           0x00740000,
                           0x00000000,
                           0x008D008E,
                           0x0000008D,
                           0x008A0000,
                           0x00AB00A2,
                           0x00A50000,
                           0x000000A5,
                           0x00A20000,
                           0x00C000BB,
                           0x000000BF,
                           0x000000BE,
                           0x00000000,
                           0x00D60000,
                           0x00D400D5,
                           0x000000D4,
                           0x00D00000 };

void initStoryMapItem(StoryMapItem *arg0) {
    AllocationData29200 *alloc;
    u8 temp;
    u32 itemType;
    s16 arg5;

    alloc = getCurrentAllocation();
    alloc->unk418 = gStoryMapItemValues[arg0->eventTypeIndex];
    temp = gStoryMapItemTypeTables[arg0->eventTypeIndex][randB() & 0xF];
    arg0->itemType = temp;
    alloc->unk42D = temp;
    itemType = arg0->itemType;
    arg5 = 5;

    if (itemType == 5 || itemType == 0xB) {
        if (itemType == 5) {
            arg0->model = createSceneModelEx(arg0->eventTypeIndex + 0x50, alloc, 0, -1, 0, itemType);
            alloc->unk421 = 0;
        } else {
            arg0->model = createSceneModelEx(arg0->eventTypeIndex + 0x50, alloc, 0, 0, 0, arg5);
            alloc->unk421 = 1;
        }
    } else if (itemType < 0xBU || itemType == 0x10) {
        arg0->model = createSceneModel(arg0->eventTypeIndex + 0x50, alloc);
        alloc->unk421 = 0;
    } else {
        arg0->model = createSceneModelEx(arg0->eventTypeIndex + 0x50, alloc, 0, 0, 0, -1);
        alloc->unk421 = 1;
    }

    setCleanupCallback(cleanupStoryMapItem);
    setCallback(awaitStoryMapItemReady);
}

void respawnStoryMapItem(StoryMapItem *arg0) {
    AllocationData29200 *alloc;
    u8 temp;
    u32 itemType;
    s16 arg5;

    alloc = getCurrentAllocation();

    do {
        temp = gStoryMapItemTypeTables[arg0->eventTypeIndex][randB() & 0xF];
        arg0->itemType = temp;
    } while (gStoryMapItemWaypointCounts[temp] == 4);

    alloc->unk42D = temp;
    itemType = arg0->itemType;
    arg5 = 5;

    if (itemType == 5 || itemType == 0xB) {
        if (itemType == 5) {
            arg0->model = createSceneModelEx(arg0->eventTypeIndex + 0x50, alloc, 0, -1, 0, itemType);
            alloc->unk421 = 0;
        } else {
            arg0->model = createSceneModelEx(arg0->eventTypeIndex + 0x50, alloc, 0, 0, 0, arg5);
            alloc->unk421 = 1;
        }
    } else if (itemType < 0xBU || itemType == 0x10) {
        arg0->model = createSceneModel(arg0->eventTypeIndex + 0x50, alloc);
        alloc->unk421 = 0;
    } else {
        arg0->model = createSceneModelEx(arg0->eventTypeIndex + 0x50, alloc, 0, 0, 0, -1);
        alloc->unk421 = 1;
    }

    setCallback(awaitStoryMapItemReady);
}

void awaitStoryMapItemReady(void) {
    AllocationData29200 *alloc = getCurrentAllocation();

    if (alloc->unk429 == 0) {
        setCallback(initStoryMapItemMovement);
    }
}

void initStoryMapItemMovement(StoryMapItemTask *arg0) {
    GameState *alloc;
    s32 count;
    s32 i;
    s32 index;
    s16 angle;
    u16 temp;
    s32 posZ;

    alloc = getCurrentAllocation();
    memcpy(&arg0->matrix, &identityMatrix, 0x20);
    alloc->unk42E = 0;

    if (arg0->itemType == 5 || arg0->itemType == 0xB) {
        setItemDisplayEnabled(arg0->model, 1);
    } else {
        setItemDisplayEnabled(arg0->model, 0);
    }

    if (arg0->itemType == 0xD) {
        spawnRocketEffectEx(arg0->model, 0x7FFF, 2, 0, 0x5A);
    }

    count = gStoryMapItemWaypointCounts[arg0->itemType];
    arg0->waypointData = alloc->unk3E4;

    for (i = 0; i < count; i++) {
        arg0->waypointData += gStoryMapItemWaypointDataSizes[i] * 2 + 1;
    }

    arg0->waypointCount = arg0->waypointData[0];
    index = randB();
    arg0->moveDirection = index & 1;

    if (index & 1) {
        arg0->currentWaypoint = 0;
        index = 1;
    } else {
        index = arg0->waypointCount - 2;
        arg0->currentWaypoint = arg0->waypointCount - 1;
    }

    arg0->matrix.translation.x = arg0->waypointData[arg0->currentWaypoint * 2 + 1] << 16;
    arg0->matrix.translation.z = arg0->waypointData[arg0->currentWaypoint * 2 + 2] << 16;

    angle = computeAngleToPosition(
        arg0->waypointData[(index & 0xFF) * 2 + 1] << 16,
        arg0->waypointData[(index & 0xFF) * 2 + 2] << 16,
        arg0->matrix.translation.x,
        arg0->matrix.translation.z
    );

    arg0->rotationAngle = angle;
    arg0->targetRotation = angle;
    arg0->currentRotation = arg0->rotationAngle;

    arg0->moveTimer = gStoryMapItemAnimationTimers[arg0->itemType];

    temp = gStoryMapItemAnimationValues[arg0->itemType];
    arg0->animValue = temp;
    arg0->animCounter = 0;
    arg0->stateCounter = 0;
    arg0->animValue2 = temp;
    arg0->animValue3 = arg0->animValue;

    alloc->npcPosX[0] = arg0->matrix.translation.x;
    posZ = arg0->matrix.translation.z;
    alloc->unk422 = 0;
    alloc->npcPosZ[0] = posZ;

    setCallback((void *)gStoryMapItemUpdateCallbacks[arg0->itemType]);
}

void collectStoryMapItem(StoryMapItem *arg0) {
    AllocationData29200 *alloc = getCurrentAllocation();

    alloc->unk422 = 1;
    alloc->unk42E = 1;
    arg0->model = destroySceneModel(arg0->model);
    arg0->respawnTimer = (randB() & 0x1F) + 0xF;
    setCallback(awaitStoryMapItemRespawn);
}

void awaitStoryMapItemRespawn(StoryMapItem *arg0) {
    arg0->respawnTimer--;
    if (arg0->respawnTimer == 0) {
        arg0->respawnTimer = 0;
        setCallback(respawnStoryMapItem);
    }
}

void cleanupStoryMapItem(StoryMapItem *arg0) {
    arg0->model = destroySceneModel(arg0->model);
}

s16 D_8008E568_8F168[] = {
    0x0051, 0x006D, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00F5, 0x00E3, 0x0000,
};

s16 D_8008E57C_8F17C[] = {
    0x0000, 0x0000, 0x0083, 0x0099, 0x0000, 0x00C9, 0x0000, 0x00F4, 0x00E1, 0x0000,
};
