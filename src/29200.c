#include "29200.h"
#include "20F0.h"
#include "C570.h"
#include "common.h"
#include "rand.h"
#include "task_scheduler.h"

typedef struct {
    /* 0x00 */ void *model;
    /* 0x04 */ Transform3D matrix;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s16 *itemData;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ u8 pad30[0x4];
    /* 0x34 */ u16 unk34;
    /* 0x36 */ u8 pad36[0x2];
    /* 0x38 */ u8 unk38;
    /* 0x39 */ u8 unk39;
    /* 0x3A */ u8 unk3A;
    /* 0x3B */ u8 pad3B[0x15];
    /* 0x50 */ u16 unk50;
    /* 0x52 */ u16 unk52;
    /* 0x54 */ u8 pad54[0x4];
    /* 0x58 */ u16 unk58;
    /* 0x5A */ u16 unk5A;
    /* 0x5C */ u8 pad5C;
    /* 0x5D */ u8 itemType;
    /* 0x5E */ u8 pad5E[0x4];
    /* 0x62 */ u8 unk62;
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

void func_800288D4_294D4(StoryMapItemTask *);
void awaitStoryMapItemReady(void);
void awaitStoryMapItemRespawn(StoryMapItem *arg0);
void cleanupStoryMapItem(StoryMapItem *arg0);

extern u16 D_8008E19C_8ED9C[];
extern u8 D_8008E030_8EC30[][16];
extern s16 D_8008E0C0_8ECC0[];
extern s16 D_8008E0C0_8ECC0[];
extern u16 D_8008E0E4_8ECE4[];
extern s32 D_8008E0F0_8ECF0[];
extern u16 D_8008E134_8ED34[];
extern void *D_8008E158_8ED58[];
extern u8 identityMatrix[];

void initStoryMapItem(StoryMapItem *arg0) {
    AllocationData29200 *alloc;
    u8 temp;
    u32 itemType;
    s16 arg5;

    alloc = getCurrentAllocation();
    alloc->unk418 = D_8008E19C_8ED9C[arg0->eventTypeIndex];
    temp = D_8008E030_8EC30[arg0->eventTypeIndex][randB() & 0xF];
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
        temp = D_8008E030_8EC30[arg0->eventTypeIndex][randB() & 0xF];
        arg0->itemType = temp;
    } while (D_8008E0C0_8ECC0[temp] == 4);

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
        setCallback(func_800288D4_294D4);
    }
}

void func_800288D4_294D4(StoryMapItemTask *arg0) {
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

    count = D_8008E0C0_8ECC0[arg0->itemType];
    arg0->itemData = alloc->unk3E4;

    for (i = 0; i < count; i++) {
        arg0->itemData += D_8008E0E4_8ECE4[i] * 2 + 1;
    }

    arg0->unk39 = arg0->itemData[0];
    index = randB();
    arg0->unk38 = index & 1;

    if (index & 1) {
        arg0->unk3A = 0;
        index = 1;
    } else {
        index = arg0->unk39 - 2;
        arg0->unk3A = arg0->unk39 - 1;
    }

    arg0->matrix.translation.x = arg0->itemData[arg0->unk3A * 2 + 1] << 16;
    arg0->matrix.translation.z = arg0->itemData[arg0->unk3A * 2 + 2] << 16;

    angle = func_8006D21C_6DE1C(
        arg0->itemData[(index & 0xFF) * 2 + 1] << 16,
        arg0->itemData[(index & 0xFF) * 2 + 2] << 16,
        arg0->matrix.translation.x,
        arg0->matrix.translation.z
    );

    arg0->unk2C = angle;
    arg0->unk2E = angle;
    arg0->unk34 = arg0->unk2C;

    arg0->unk24 = D_8008E0F0_8ECF0[arg0->itemType];

    temp = D_8008E134_8ED34[arg0->itemType];
    arg0->unk50 = temp;
    arg0->unk5A = 0;
    arg0->unk62 = 0;
    arg0->unk52 = temp;
    arg0->unk58 = arg0->unk50;

    alloc->unk408[0] = arg0->matrix.translation.x;
    posZ = arg0->matrix.translation.z;
    alloc->unk422 = 0;
    alloc->unk410[0] = posZ;

    setCallback(D_8008E158_8ED58[arg0->itemType]);
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
