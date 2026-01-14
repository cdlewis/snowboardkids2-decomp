#include "51060.h"
#include "10AD0.h"
#include "36BE0.h"
#include "5E590.h"
#include "6E840.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "race_session.h"
#include "rand.h"
#include "task_scheduler.h"

typedef struct {
    DataTable_19E80 *assetTable;
    loadAssetMetadata_arg particle;
    u8 padding2[0x10];
    s32 particleType;
    s32 iteration;
    s32 velX;
} SprayEffectTask;

typedef struct {
    s32 padding0;
    loadAssetMetadata_arg particle;
    s32 padding2;
    s32 velX;
    s32 velY;
    s32 velZ;
    s32 padding3;
    s32 iteration;
} SprayEffectUpdateTask;

typedef struct {
    loadAssetMetadata_arg lam;
    u8 padding[0x4];
} AssetWrapper;

typedef struct {
    MemoryAllocatorNode *assetTable;
    AssetWrapper assets[2];
    s32 velX;
    s32 velY;
    s32 velZ;
    s16 frameCounter;
    s16 padding;
    s16 particleType;
} DualSnowSprayUpdateTask;

typedef struct {
    MemoryAllocatorNode *assetTable;
    void *particleSlot;
    Vec3i pos1;
    u8 padding1[0xA];
    s8 alpha1;
    u8 padding2[0x5];
    void *particleSlot2;
    Vec3i pos2;
    u8 padding3[0xA];
    s8 alpha2;
    u8 padding4[0x5];
    s32 velX;
    s32 velY;
    s32 velZ;
    s16 frameCounter;
    s16 slotIndex;
    s16 particleType;
} DualSnowSprayTask;

typedef struct {
    s16 unk0;
    s16 unk2;
    MemoryAllocatorNode *unk4;
    s16 unk8;
    s8 unkA;
    u8 paddingB;
    s32 unkC;
    s32 unk10;
    u16 unk14;
    s16 unk16;
} func_80050F18_51B18_arg;

typedef struct {
    u8 padding[0x164];
    s16 unk164[3];
} func_800516F4_522F4_arg_unk0;

typedef struct {
    func_800516F4_522F4_arg_unk0 *unk0;
    void *unk4;
    loadAssetMetadata_arg unk8;
} func_800516F4_522F4_arg;

typedef struct {
    s32 unk0;
    void *unk4;
} func_80051688_52288_arg;

typedef struct {
    Player *player;
    DataTable_19E80 *assetTable;
    loadAssetMetadata_arg particleLeft;
    u8 padding24[0x4];
    loadAssetMetadata_arg particleRight;
    u8 padding44[0x4];
    Vec3i skiOffsets[2];
    s16 frameCounter;
    s16 particleIndex;
} SkiTrailTask;

typedef struct {
    u8 padding[0xC0];
    MemoryAllocatorNode *unkC0;
} func_8005186C_5246C_arg;

typedef struct {
    Node n;
    s32 unk2C;
    s32 unk30;
} NodeWithPayload;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} SprayEffectSpawnTask;

typedef struct {
    MemoryAllocatorNode *assetTable;
} DualSnowSprayAssetNode;

typedef struct {
    u8 padding[0xC4];
    void *unkC4;
    u8 padding2[0xC];
    u8 unkD4;
    u8 padding3;
    u8 unkD6;
} func_80051BB8_527B8_task;

typedef struct {
    u8 padding[0x12C0];
    void *unk12C0;
} GameStateUnk44_Ext;

typedef struct {
    u8 padding[0x4C];
    Vec3i *unk4C;
    u8 padding2[0x434 - 0x50];
    Vec3i *unk434;
} func_80050C00_51800_Task_unk34;

typedef struct {
    /* 0x00 */ void *assetTable;
    /* 0x04 */ loadAssetMetadata_arg particle;
    /* 0x20 */ u8 padding_20[0x4];
    /* 0x24 */ s16 particleType;
    /* 0x26 */ u16 animFrame;
    /* 0x28 */ s32 velX;
    /* 0x2C */ s32 velY;
    /* 0x30 */ s32 velZ;
    /* 0x34 */ func_80050C00_51800_Task_unk34 *sourceObj;
    /* 0x38 */ s16 positionSelector;
} CharacterTrailParticleTask;

typedef struct {
    u8 padding[0x44C];
    s32 velocity;
    s32 unk450;
    s32 unk454;
} func_80050C80_51880_arg0;

typedef struct {
    u8 padding[0x44];
    void *unk44;
} func_80050C80_51880_allocation;

typedef struct {
    void *unk0;
    void *unk4;
    u8 padding[0x1C];
    s32 unk24;
    s32 unk28;
} func_80050DB0_519B0_arg;

typedef struct {
    loadAssetMetadata_arg unk0;
    u8 _pad2[0x4];
} func_800518AC_524AC_element;

typedef struct {
    func_800518AC_524AC_element elements[6];
    void *unkC0;
    u8 _padC4[0x4];
    u8 unkC8[0xC];
    s8 unkD4;
    u8 unkD5;
    u8 unkD6;
} func_800518AC_524AC_arg;

void loadFirstSprayParticle(SprayEffectTask *);
void cleanupSprayEffect(void **);
void updateSprayEffect(SprayEffectUpdateTask *);
void initDualSnowSprayTask(DualSnowSprayTask *);
void initDualSnowSprayTask_SingleSlot(DualSnowSprayTask *);
void updateDualSnowSprayParticles(DualSnowSprayUpdateTask *);
void cleanupDualSnowSprayAssetNode(DualSnowSprayAssetNode *);
void initCharacterTrailParticleTask(MemoryAllocatorNode **);
void loadCharacterTrailParticleAsset(CharacterTrailParticleTask *);
void updateCharacterTrailParticle(CharacterTrailParticleTask *);
void cleanupCharacterTrailParticleTask(s32 **);
void spawnCharacterTrailParticle(void *);
void spawnPlayerCharacterTrailParticle(Player *, s32);
void func_80050DB0_519B0(func_80050DB0_519B0_arg *);
void func_80050E08_51A08(func_80050DB0_519B0_arg *);
void func_80050EA0_51AA0(void **);
void func_80050F64_51B64(func_80050F18_51B18_arg *);
void func_80050FE0_51BE0(func_80050F18_51B18_arg *);
void updateDualSnowSprayParticles_SingleSlot(DualSnowSprayUpdateTask *);
void cleanupDualSnowSprayTask(DualSnowSprayTask *);
void func_80051760_52360(func_800516F4_522F4_arg *);
void func_80051800_52400(func_800516F4_522F4_arg *);
void func_800518AC_524AC(func_800518AC_524AC_arg *);
void func_80051B8C_5278C(func_8005186C_5246C_arg *);

extern loadAssetMetadata_arg D_80090EC0_91AC0;
extern loadAssetMetadata_arg D_80090F00_91B00;
extern void *D_80090F40_91B40;
extern void *D_80090F4C_91B4C;
extern u8 D_80090E70_91A70[];
extern s16 D_80090E80_91A80[];
extern s16 D_80090E98_91A98[];
extern s16 D_80090EB0_91AB0[];
extern u16 D_8009ADE0_9B9E0;

void initSprayEffectTask(void **node) {
    *node = load_3ECE40();
    setCleanupCallback(&cleanupSprayEffect);
    setCallbackWithContinue(&loadFirstSprayParticle);
}

void loadFirstSprayParticle(SprayEffectTask *arg0) {
    GameState *gs = (GameState *)getCurrentAllocation();
    loadAssetMetadata(&arg0->particle, arg0->assetTable, arg0->particleType);
    arg0->particle.unk1A = 0xE0;
    arg0->particle.unk0 = &gs->unk44->unkFC0->asset;
    arg0->iteration = 0;
    setCallbackWithContinue(&updateSprayEffect);
}

void updateSprayEffect(SprayEffectUpdateTask *arg0) {
    GameState *gs;
    s32 i;
    gs = (GameState *)getCurrentAllocation();
    i = 0;

    if (gs->gamePaused == 0) {
        GameStateUnk44 *base;

        base = gs->unk44;
        arg0->particle.unk0 = &base->unkFC0[arg0->iteration].asset;

        arg0->iteration = arg0->iteration + 1;
        if (arg0->iteration == 4) {
            func_80069CF8_6A8F8();
            return;
        }
        arg0->particle.unk1A = arg0->particle.unk1A - 0x30;
        arg0->particle.position.x += arg0->velX;
        arg0->particle.position.y += arg0->velY;
        arg0->particle.position.z += arg0->velZ;
    }

    for (i = 0; i < 4; i++) {
        func_800677C0_683C0(i, &arg0->particle);
    }
}

void cleanupSprayEffect(void **arg0) {
    *arg0 = freeNodeMemory(*arg0);
}

void spawnSprayEffect(Vec3i *arg0, Vec3i *arg1, s32 arg2) {
    NodeWithPayload *task = (NodeWithPayload *)scheduleTask(&initSprayEffectTask, 2, 0, 0xDC);
    if (task != NULL) {
        memcpy((void *)&task->n.freeNext, arg0, sizeof(Vec3i));
        task->unk30 = arg2;
        task->n.cleanupCallback = (void *)(arg1->x / 2);
        task->n.payload = (void *)(arg1->y / 2);
        task->unk2C = (arg1->z / 2);
    }
}

void initDualSnowSprayTask(DualSnowSprayTask *arg0) {
    GameState *gs = (GameState *)getCurrentAllocation();
    arg0->assetTable = load_3ECE40();
    arg0->particleSlot = &gs->unk44->unk1080[arg0->slotIndex];
    arg0->alpha1 = (u8)((randA() & 0x1F) + 0x70);
    arg0->frameCounter = 0;
    arg0->particleSlot2 = arg0->particleSlot;
    arg0->alpha2 = (u8)arg0->alpha1;
    setCleanupCallback(&cleanupDualSnowSprayAssetNode);
    setCallbackWithContinue(&updateDualSnowSprayParticles);
}

void updateDualSnowSprayParticles(DualSnowSprayUpdateTask *arg0) {
    GameState *gs;
    s32 i;

    gs = (GameState *)getCurrentAllocation();
    loadAssetMetadata(&arg0->assets[0].lam, arg0->assetTable, arg0->particleType + arg0->frameCounter);

    arg0->assets[1].lam.data_ptr = arg0->assets[0].lam.data_ptr;
    arg0->assets[1].lam.index_ptr = arg0->assets[0].lam.index_ptr;
    arg0->assets[1].lam.unk18 = arg0->assets[0].lam.unk18;
    arg0->assets[1].lam.unk19 = arg0->assets[0].lam.unk19;

    for (i = 0; i < 4; i++) {
        func_80067EDC_68ADC(i, &arg0->assets[0].lam);
        func_80067EDC_68ADC(i, &arg0->assets[1].lam);
    }

    if (gs->gamePaused == 0) {
        if (arg0->frameCounter != 0) {
            for (i = 0; i < 2; i++) {
                arg0->assets[i].lam.position.x += arg0->velX;
                arg0->assets[i].lam.position.y += arg0->velY;
                arg0->assets[i].lam.position.z += arg0->velZ;
            }
        }

        arg0->frameCounter++;
        if (arg0->frameCounter == 5) {
            func_80069CF8_6A8F8();
        }
    }
}

void cleanupDualSnowSprayAssetNode(DualSnowSprayAssetNode *arg0) {
    arg0->assetTable = freeNodeMemory(arg0->assetTable);
}

void spawnDualSnowSprayEffect(Vec3i *pos1, Vec3i *pos2, Vec3i *velocity, s16 slotIndex, s32 characterId) {
    DualSnowSprayTask *task;
    s32 velX;
    s32 velY;
    s32 velZ;
    u32 signX;
    u32 signY;
    u32 signZ;
    u8 particleType;

    if (D_80090E70_91A70[characterId] == 0xFF) {
        return;
    }

    task = (DualSnowSprayTask *)scheduleTask(&initDualSnowSprayTask, 2, 0, 0xDD);
    if (task == NULL) {
        return;
    }

    memcpy(&task->pos1, pos1, 0xC);
    memcpy(&task->pos2, pos2, 0xC);

    particleType = D_80090E70_91A70[characterId];
    task->slotIndex = slotIndex;
    task->particleType = particleType;

    velX = velocity->x;
    signX = (u32)velX >> 31;
    velX += signX;
    velX >>= 1;
    task->velX = velX;

    velY = velocity->y;
    signY = (u32)velY >> 31;
    velY += signY;
    velY >>= 1;
    task->velY = velY;

    velZ = velocity->z;
    signZ = (u32)velZ >> 31;
    velZ += signZ;
    velZ >>= 1;
    task->velZ = velZ;
}

void initCharacterTrailParticleTask(MemoryAllocatorNode **node) {
    *node = load_3ECE40();
    setCleanupCallback(&cleanupCharacterTrailParticleTask);
    setCallbackWithContinue(&loadCharacterTrailParticleAsset);
}

void loadCharacterTrailParticleAsset(CharacterTrailParticleTask *arg0) {
    s32 temp;
    s32 shift9;
    int new_var2;
    s32 new_var;
    getCurrentAllocation();
    if (arg0->positionSelector >= 0) {
        memcpy(&arg0->particle.position, &arg0->sourceObj->unk4C, sizeof(Vec3i));
    } else {
        memcpy(&arg0->particle.position, &arg0->sourceObj->unk434, sizeof(Vec3i));
        arg0->particle.position.y += 0x80000;
    }
    temp = (randA() & 0xFF) - 0x80;
    shift9 = (new_var = temp << 9);
    arg0->particle.position.x += ((temp << 11) + shift9) << 1;
    new_var2 = (randA() & 0xFF) - 0x80;
    temp = new_var2;
    shift9 = temp << 9;
    arg0->animFrame = 0;
    arg0->particle.position.z += ((temp << 11) + shift9) << 1;
    loadAssetMetadata(&arg0->particle, arg0->assetTable, arg0->particleType);
    setCallbackWithContinue(updateCharacterTrailParticle);
}

void updateCharacterTrailParticle(CharacterTrailParticleTask *arg0) {
    GameState *gs;
    s32 i;
    s16 temp;

    gs = (GameState *)getCurrentAllocation();

    if (gs->gamePaused == 0) {
        temp = arg0->animFrame;

        if (temp % 3 == 0) {
            loadAssetMetadata((&arg0->particle), arg0->assetTable, arg0->particleType + (temp >> 2));
        }

        arg0->animFrame++;
        if ((s16)arg0->animFrame >= 0xF) {
            func_80069CF8_6A8F8();
        }

        arg0->particle.position.x += arg0->velX;
        arg0->particle.position.y += arg0->velY;
        arg0->particle.position.z += arg0->velZ;
    }

    i = 0;
    if (arg0->particle.unk1A == 0xFF) {
        do {
            enqueueTexturedBillboardSprite(i, (TexturedBillboardSprite *)&arg0->particle);
            i++;
        } while (i < 4);
    } else {
        do {
            func_800677C0_683C0(i, (&arg0->particle));
            i++;
        } while (i < 4);
    }
}

void cleanupCharacterTrailParticleTask(s32 **arg0) {
    *arg0 = freeNodeMemory(*arg0);
}

void spawnCharacterTrailParticle(void *arg0) {
    GameState *allocation;
    CharacterTrailParticleTask *task;

    allocation = (GameState *)getCurrentAllocation();
    task = (CharacterTrailParticleTask *)scheduleTask(&initCharacterTrailParticleTask, 2, 0, 0xEA);
    if (task != NULL) {
        task->particleType = 0x35;
        task->sourceObj = arg0;
        task->particle.unk1A = 0xFF;
        task->velX = 0;
        task->velY = 0;
        task->velZ = 0;
        task->positionSelector = 0;
        task->particle.unk0 = (loadAssetMetadata_arg *)&allocation->unk44->unkFC0;
    }
}

void spawnPlayerCharacterTrailParticle(Player *arg0, s32 arg1) {
    func_80050C80_51880_allocation *allocation;
    CharacterTrailParticleTask *task;
    u8 temp;

    allocation = (func_80050C80_51880_allocation *)getCurrentAllocation();
    temp = D_80090E70_91A70[arg1];

    if (temp == 0xFF) {
        return;
    }

    if (D_8009ADE0_9B9E0 & 1) {
        return;
    }

    task = (CharacterTrailParticleTask *)scheduleTask(&initCharacterTrailParticleTask, 2, 0, 0xEA);

    if (task != NULL) {
        u8 temp2;
        task->sourceObj = (func_80050C00_51800_Task_unk34 *)arg0;
        temp2 = D_80090E70_91A70[arg1];
        task->particle.unk1A = 0x80;
        task->particleType = temp2;
        task->velX = arg0->velocity.x / 2;
        task->velY = arg0->velocity.y / 2;
        task->velZ = arg0->velocity.z / 2;
        task->positionSelector = -1;
        task->particle.unk0 = (void *)((u32)allocation->unk44 + 0x1440);
    }
}

void func_80050D70_51970(MemoryAllocatorNode **node) {
    *node = load_3ECE40();
    setCleanupCallback(&func_80050EA0_51AA0);
    setCallbackWithContinue(&func_80050DB0_519B0);
}

void func_80050DB0_519B0(func_80050DB0_519B0_arg *arg0) {
    GameState *allocation;
    void *temp;

    allocation = getCurrentAllocation();
    temp = &((GameStateUnk44_Ext *)allocation->unk44)->unk12C0;

    arg0->unk24 = 0x40;
    arg0->unk4 = temp;
    arg0->unk28 = 0;

    loadAssetMetadata((loadAssetMetadata_arg *)&arg0->unk4, arg0->unk0, arg0->unk24);

    setCallbackWithContinue(&func_80050E08_51A08);
}

void func_80050E08_51A08(func_80050DB0_519B0_arg *arg0) {
    GameState *alloc;
    s32 i;

    alloc = getCurrentAllocation();
    if (alloc->gamePaused == 0) {
        if ((arg0->unk28 & 1) == 0) {
            loadAssetMetadata((loadAssetMetadata_arg *)&arg0->unk4, arg0->unk0, arg0->unk24 + (arg0->unk28 >> 1));
        }
        arg0->unk28++;
        if (arg0->unk28 >= 0xA) {
            func_80069CF8_6A8F8();
        }
    }

    for (i = 0; i < 4; i++) {
        enqueueTexturedBillboardSprite(i, (TexturedBillboardSprite *)&arg0->unk4);
    }
}

void func_80050EA0_51AA0(void **arg0) {
    *arg0 = freeNodeMemory(*arg0);
}

void spawnImpactStar(Vec3i *arg0) {
    Node *task;

    task = scheduleTask(&func_80050D70_51970, 2, 0, 0xFA);
    if (task != NULL) {
        memcpy(&task->freeNext, arg0, sizeof(Vec3i));
    }
}

void func_80050F18_51B18(func_80050F18_51B18_arg *arg0) {
    arg0->unk4 = load_3ECE40();
    arg0->unk8 = 0x45;
    arg0->unk10 = 0;
    setCleanupCallback(&func_80050FE0_51BE0);
    setCallbackWithContinue(&func_80050F64_51B64);
}

void func_80050F64_51B64(func_80050F18_51B18_arg *arg0) {
    arg0->unk8 = (arg0->unk10 >> 1) + 0x45;
    arg0->unk10 = arg0->unk10 + 1;

    if (arg0->unk10 == 0x10) {
        func_80069CF8_6A8F8();
    }

    if (arg0->unk16 == 0) {
        debugEnqueueCallback(arg0->unk14, 1, func_80010240_10E40, arg0);
    } else {
        debugEnqueueCallback(arg0->unk14, 1, func_80010924_11524, arg0);
    }
}

void func_80050FE0_51BE0(func_80050F18_51B18_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_8005100C_51C0C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_80050F18_51B18_arg *task;

    task = (func_80050F18_51B18_arg *)scheduleTask(&func_80050F18_51B18, 2, 0, 0xE6);
    if (task != NULL) {
        task->unk0 = arg0;
        task->unk2 = arg1;
        if (arg2 != 0) {
            task->unkA = 0x11;
        } else {
            task->unkA = 0x10;
        }
        task->unk14 = arg3;
        task->unk16 = arg4;
    }
}

void initDualSnowSprayTask_SingleSlot(DualSnowSprayTask *arg0) {
    GameState *gs = (GameState *)getCurrentAllocation();
    arg0->assetTable = load_3ECE40();
    arg0->particleSlot = &gs->unk44->unk1340;
    arg0->alpha1 = (u8)((randA() & 0x1F) + 0x70);
    arg0->frameCounter = 0;
    arg0->particleSlot2 = arg0->particleSlot;
    arg0->alpha2 = (u8)arg0->alpha1;
    setCleanupCallback(&cleanupDualSnowSprayTask);
    setCallbackWithContinue(&updateDualSnowSprayParticles_SingleSlot);
}

void updateDualSnowSprayParticles_SingleSlot(DualSnowSprayUpdateTask *arg0) {
    GameState *gs;
    s32 i;

    gs = (GameState *)getCurrentAllocation();
    loadAssetMetadata(&arg0->assets[0].lam, arg0->assetTable, (arg0->frameCounter / 4) + 8);

    arg0->assets[1].lam.data_ptr = arg0->assets[0].lam.data_ptr;
    arg0->assets[1].lam.index_ptr = arg0->assets[0].lam.index_ptr;
    arg0->assets[1].lam.unk18 = arg0->assets[0].lam.unk18;
    arg0->assets[1].lam.unk19 = arg0->assets[0].lam.unk19;

    for (i = 0; i < 4; i++) {
        func_80067EDC_68ADC(i, &arg0->assets[0].lam);
        func_80067EDC_68ADC(i, &arg0->assets[1].lam);
    }

    if (gs->gamePaused == 0) {
        if (arg0->frameCounter != 0) {
            for (i = 0; i < 2; i++) {
                arg0->assets[i].lam.position.x += arg0->velX;
                arg0->assets[i].lam.position.y += arg0->velY;
                arg0->assets[i].lam.position.z += arg0->velZ;
            }
        }

        arg0->frameCounter++;
        if (arg0->frameCounter == 0x14) {
            func_80069CF8_6A8F8();
        }
    }
}

void cleanupDualSnowSprayTask(DualSnowSprayTask *arg0) {
    arg0->assetTable = freeNodeMemory(arg0->assetTable);
}

void spawnDualSnowSprayEffect_SingleSlot(Vec3i *pos1, Vec3i *pos2, Vec3i *velocity, s32 particleType) {
    DualSnowSprayTask *task = (DualSnowSprayTask *)scheduleTask(&initDualSnowSprayTask_SingleSlot, 2, 0, 0xDD);
    if (task != NULL) {
        memcpy(&task->pos1, pos1, 0xC);
        memcpy(&task->pos2, pos2, 0xC);
        task->particleType = particleType;
        task->velX = (s32)(velocity->x / 2);
        task->velY = (s32)(velocity->y / 2);
        task->velZ = (s32)(velocity->z / 2);
    }
}

void func_8005152C_5212C(SkiTrailTask *arg0);
void func_80051688_52288(func_80051688_52288_arg *arg0);

void func_80051348_51F48(SkiTrailTask *task) {
    GameState *gs;
    s32 i;
    s16 *transformSource;
    s32 outputOffset;
    volatile SkiTrailTask *outputPtr;
    void *particleAsset;

    gs = (GameState *)getCurrentAllocation();
    task->assetTable = load_3ECE40();
    particleAsset = (void *)((u8 *)gs->unk44 + 0x13C0);
    task->particleLeft.unk1A = 0xFF;
    task->particleLeft.unk0 = particleAsset;
    task->particleRight.unk1A = task->particleLeft.unk1A;
    task->particleRight.unk0 = task->particleLeft.unk0;

    i = 0;

    if (task->player->unkB84 & 2) {
        outputPtr = task;
        outputOffset = 0x48;
        transformSource = D_80090E80_91A80;
        do {
            transformVector(transformSource, (s16 *)&task->player->unk3F8, (void *)((u8 *)task + outputOffset));
            outputPtr->skiOffsets[0].x -= task->player->worldPos.x;
            outputPtr->skiOffsets[0].y -= task->player->worldPos.y;
            outputOffset += 0xC;
            transformSource += 6;
            i++;
            outputPtr->skiOffsets[0].z -= task->player->worldPos.z;
            outputPtr = (volatile SkiTrailTask *)((u8 *)outputPtr + 0xC);
        } while (i < 2);
    } else {
        outputPtr = task;
        outputOffset = 0x48;
        transformSource = D_80090E98_91A98;
        do {
            transformVector(transformSource, (s16 *)&task->player->unk3F8, (void *)((u8 *)task + outputOffset));
            outputPtr->skiOffsets[0].x -= task->player->worldPos.x;
            outputPtr->skiOffsets[0].y -= task->player->worldPos.y;
            outputOffset += 0xC;
            transformSource += 6;
            i++;
            outputPtr->skiOffsets[0].z -= task->player->worldPos.z;
            outputPtr = (volatile SkiTrailTask *)((u8 *)outputPtr + 0xC);
        } while (i < 2);
    }

    i = 0;
    task->frameCounter = 0;
    task->particleIndex = -1;

    do {
        if ((task->player->tricksPerformedMask >> i) & 1) {
            task->particleIndex++;
        }
        i++;
    } while (i < 8);

    task->particleIndex += 0x15;
    setCleanupCallback(func_80051688_52288);
    setCallbackWithContinue(func_8005152C_5212C);
}

void func_8005152C_5212C(SkiTrailTask *task) {
    GameState *gs;
    s32 i;

    gs = (GameState *)getCurrentAllocation();
    loadAssetMetadataByIndex(
        (loadAssetMetadataByIndex_arg *)&task->particleLeft,
        task->assetTable,
        task->frameCounter + 0x61,
        task->particleIndex
    );

    task->particleRight.data_ptr = task->particleLeft.data_ptr;
    task->particleRight.index_ptr = task->particleLeft.index_ptr;
    task->particleRight.unk18 = task->particleLeft.unk18;
    task->particleRight.unk19 = task->particleLeft.unk19;

    task->particleLeft.position.x = task->skiOffsets[0].x + task->player->worldPos.x;
    task->particleLeft.position.y = task->skiOffsets[0].y + task->player->worldPos.y;
    task->particleLeft.position.z = task->skiOffsets[0].z + task->player->worldPos.z;
    task->particleRight.position.x = task->skiOffsets[1].x + task->player->worldPos.x;
    task->particleRight.position.y = task->skiOffsets[1].y + task->player->worldPos.y;
    task->particleRight.position.z = task->skiOffsets[1].z + task->player->worldPos.z;

    for (i = 0; i < 4; i++) {
        func_80067EDC_68ADC(i, &task->particleLeft);
        func_80067EDC_68ADC(i, &task->particleRight);
    }

    if (gs->gamePaused == 0) {
        task->particleLeft.unk1A -= 0x14;
        task->particleRight.unk1A -= 0x14;
        task->frameCounter++;
        if (task->frameCounter == 8) {
            func_80069CF8_6A8F8();
        }
    }
}

void func_80051688_52288(func_80051688_52288_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_800516B4_522B4(Player *player) {
    SkiTrailTask *task = (SkiTrailTask *)scheduleTask(&func_80051348_51F48, 2, 0, 0xDD);
    if (task != NULL) {
        task->player = player;
    }
}

void func_800516F4_522F4(func_800516F4_522F4_arg *arg0) {
    getCurrentAllocation();
    arg0->unk4 = load_3ECE40();
    arg0->unk8.unk0 = &D_80090EC0_91AC0;
    arg0->unk8.unk1A = 0xFF;
    loadAssetMetadata(&arg0->unk8, arg0->unk4, 0x6A);
    setCleanupCallback(&func_80051800_52400);
    setCallbackWithContinue(&func_80051760_52360);
}

void func_80051760_52360(func_800516F4_522F4_arg *arg0) {
    s32 i;
    GameState *gs;

    gs = (GameState *)getCurrentAllocation();
    transformVector((s16 *)&D_80090EB0_91AB0, (s16 *)&arg0->unk0->unk164, &arg0->unk8.position);

    for (i = 0; i < 4; i++) {
        func_80067EDC_68ADC(i, &arg0->unk8);
    }

    if (gs->gamePaused == 0) {
        arg0->unk8.unk1A -= 0x10;
        if (arg0->unk8.unk1A < 0x40) {
            func_80069CF8_6A8F8();
        }
    }
}

void func_80051800_52400(func_800516F4_522F4_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_8005182C_5242C(void *arg0) {
    void *result = scheduleTask(&func_800516F4_522F4, 2, 0, 0xDD);
    if (result != NULL) {
        *(void **)result = arg0;
    }
}

void func_8005186C_5246C(func_8005186C_5246C_arg *arg0) {
    arg0->unkC0 = load_3ECE40();
    setCleanupCallback(&func_80051B8C_5278C);
    setCallbackWithContinue(&func_800518AC_524AC);
}

void func_800518AC_524AC(func_800518AC_524AC_arg *arg0) {
    s32 i;

    for (i = 0; i < 6; i++) {
        loadAssetMetadata(&arg0->elements[i].unk0, arg0->unkC0, arg0->unkD4);
        arg0->elements[i].unk0.unk0 = &D_80090F00_91B00;

        if (arg0->unkD6 == 0) {
            memcpy(&arg0->unkC8, &D_80090F40_91B40, 0xC);
            arg0->elements[i].unk0.unk1A = 0x90;
        } else {
            memcpy(&arg0->unkC8, &D_80090F4C_91B4C, 0xC);
            arg0->elements[i].unk0.unk1A = 0xF0;
        }
    }

    arg0->unkD5 = 0;
    setCallbackWithContinue(&func_80051978_52578);
}

INCLUDE_ASM("asm/nonmatchings/51060", func_80051978_52578);

void func_80051B8C_5278C(func_8005186C_5246C_arg *arg0) {
    arg0->unkC0 = freeNodeMemory(arg0->unkC0);
}

void func_80051BB8_527B8(void *arg0) {
    func_80051BB8_527B8_task *task = (func_80051BB8_527B8_task *)scheduleTask(&func_8005186C_5246C, 2, 0, 0xE7);
    if (task != NULL) {
        task->unkC4 = arg0;
        task->unkD4 = 0x12;
        task->unkD6 = 0;
    }
}

void func_80051C08_52808(void *arg0, s32 arg1) {
    s32 temp_s0;
    func_80051BB8_527B8_task *task;

    temp_s0 = D_80090E70_91A70[arg1];
    if (temp_s0 == 0xFF) {
        temp_s0 = 0xD;
    }

    task = (func_80051BB8_527B8_task *)scheduleTask(&func_8005186C_5246C, 2, 0, 0xE7);
    if (task != NULL) {
        task->unkC4 = arg0;
        task->unkD4 = temp_s0;
        task->unkD6 = 1;
    }
}
