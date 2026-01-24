#include "29200.h"
#include "20F0.h"
#include "C570.h"
#include "common.h"
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
    u8 pad423[6];
    u8 unk429;
    u8 pad42A[3];
    u8 unk42D;
    u8 unk42E;
} AllocationData29200;

extern u16 gStoryMapItemValues[];
extern u8 gStoryMapItemTypeTables[][16];
extern s16 gStoryMapItemWaypointCounts[];
extern u16 gStoryMapItemWaypointDataSizes[];
extern s32 gStoryMapItemAnimationTimers[];
extern u16 gStoryMapItemAnimationValues[];
extern void *gStoryMapItemUpdateCallbacks[];
extern u8 identityMatrix[];

static void initStoryMapItemMovement(StoryMapItemTask *);
static void awaitStoryMapItemReady(void);
static void awaitStoryMapItemRespawn(StoryMapItem *arg0);
static void cleanupStoryMapItem(StoryMapItem *arg0);

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

static void awaitStoryMapItemReady(void) {
    AllocationData29200 *alloc = getCurrentAllocation();

    if (alloc->unk429 == 0) {
        setCallback(initStoryMapItemMovement);
    }
}

static void initStoryMapItemMovement(StoryMapItemTask *arg0) {
    GameState *alloc;
    s32 count;
    s32 i;
    s32 index;
    s16 angle;
    u16 temp;
    s32 posZ;

    alloc = getCurrentAllocation();
    memcpy(&arg0->matrix, identityMatrix, 0x20);
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

    setCallback(gStoryMapItemUpdateCallbacks[arg0->itemType]);
}

void collectStoryMapItem(StoryMapItem *arg0) {
    AllocationData29200 *alloc = getCurrentAllocation();

    alloc->unk422 = 1;
    alloc->unk42E = 1;
    arg0->model = destroySceneModel(arg0->model);
    arg0->respawnTimer = (randB() & 0x1F) + 0xF;
    setCallback(awaitStoryMapItemRespawn);
}

static void awaitStoryMapItemRespawn(StoryMapItem *arg0) {
    arg0->respawnTimer--;
    if (arg0->respawnTimer == 0) {
        arg0->respawnTimer = 0;
        setCallback(respawnStoryMapItem);
    }
}

static void cleanupStoryMapItem(StoryMapItem *arg0) {
    arg0->model = destroySceneModel(arg0->model);
}
