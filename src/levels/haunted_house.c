#include "levels/haunted_house.h"
#include "race_session.h"
#include "42170.h"
#include "52880.h"
#include "56910.h"
#include "5AA90.h"
#include "5DBC0.h"
#include "5E590.h"
#include "6E840.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
#include "rand.h"
#include "task_scheduler.h"

typedef struct {
    /* 0x00 */ void *assetData;
    /* 0x04 */ void *assetMetadata;
    /* 0x08 */ s8 renderPositionBuffer[0x16];
    /* 0x1E */ u8 alpha;
    /* 0x1F */ s8 pad1F[0x5];
    /* 0x24 */ s32 posX;
    /* 0x28 */ s32 posY;
    /* 0x2C */ s32 posZ;
    /* 0x30 */ Vec3i velocity;
    /* 0x3C */ s16 entityType;
    /* 0x3E */ s16 collisionHeight;
    /* 0x40 */ s16 animTimer;
    /* 0x42 */ s16 lifetime;
    /* 0x44 */ s16 animFrameIndex;
    /* 0x46 */ u8 fadeDirection;
} AnimatedGhostEntity;

typedef struct {
    u8 pad[0x30];
    void *unk30;
} AllocationUnk30;

typedef struct {
    u8 pad[0x10];
    Player *players;
    u8 pad14[0x1C];
    void *gameData;
    u8 pad34[0x2A];
    u8 memoryPoolId;
    u8 pad5F[0x17];
    u8 gamePaused;
} Allocation;

typedef struct {
    u8 pad[0x24];
    s32 unk24;
} Task;

typedef struct {
    u8 _pad[0x24];
    void *unk24;
    void *unk28;
} func_800BBC2C_AF91C_arg;

typedef struct {
    void *unk0;
    void *unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 pad[0xA];
    u8 unk1E;
    u8 pad2[0x5];
    s32 unk24;
    s16 unk28;
} func_800BBC64_AF954_arg;

typedef struct {
    s8 frameDuration;
    s8 assetIndex;
} AnimationData;

typedef struct {
    void *matrixData;
    void *textureTable;
    u8 textureIndices[8];
} GhostRenderState;

typedef struct {
    u8 _pad[0x24];
    s16 unk24;
} func_800BC0FC_Task;

typedef struct {
    void *unk0;
    void *unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 pad14[0x10];
    s16 unk24;
    s16 unk26;
} func_800BBF4C_AFC3C_arg;

extern s32 D_8009A8A4_9B4A4;
extern void *g_GhostDefaultAssetMetadata;
extern Vec3i g_GhostBaseDirection;
extern AnimationData D_800BC830_B0520[];
extern void *D_800BC8C8_B05B8;
extern s8 D_800BC908_B05F8[12];
extern void *D_800BC920_B0610;
extern void *D_800BC960_B0650;
extern s32 D_800BC9A0_B0690[];
extern s16 D_800BC9DC_B06CC[];
extern s16 D_800BC9E8_B06D8[];
extern s16 D_800BC9F4_B06E4[];
extern Vec3i D_800BCA00_B06F0[];
extern s32 D_800BC914_B0604;
extern s32 D_800BC918_B0608;
extern s32 D_800BC91C_B060C;
extern Vec3i D_800BCA00_B06F0[];
extern Gfx D_8009A780_9B380[];
extern Gfx D_800BCA60_B0750[];
extern s32 D_800A8B14_9FE84;
extern s16 gGraphicsMode;
extern Gfx *gRegionAllocPtr;

void func_800BB388_AF078(AnimatedGhostEntity *);
void cleanupAnimatedGhost(void **);
void fadeInGhost(AnimatedGhostEntity *);
void oscillateGhostFade(AnimatedGhostEntity *);
void fadeOutGhost(AnimatedGhostEntity *);
void func_800BB778_AF468(void);
void func_800BB9A4_AF694(func_800BB8E8_AF5D8_arg *);
void func_800BBC2C_AF91C(func_800BBC2C_AF91C_arg *);
void func_800BBC64_AF954(func_800BBC64_AF954_arg *);
void func_800BBEAC_AFB9C(s16 *);
void func_800BBCE8_AF9D8(void **);
void func_800BBD14_AFA04(func_800BBC64_AF954_arg *);
void func_800BC184_AFE74(GhostManager *);
void func_800BC220_AFF10(u8 *ghostSlots);
void func_800BC340_B0030(GhostManager *);
void func_800BC750_B0440(s16 *);
void updateGhostAnimation(AnimatedGhostEntity *);
void func_800BC378_B0068(GhostRenderState *);
extern void func_800BC0D0_AFDC0(void **);
void func_800BBFC8_AFCB8(func_800BBF4C_AFC3C_arg *);

void updateGhostAnimation(AnimatedGhostEntity *ghost) {
    s32 viewport;

    ghost->animTimer--;

    if (ghost->animTimer == 0) {
        loadAssetMetadata((loadAssetMetadata_arg *)&ghost->assetMetadata, ghost->assetData, D_800BC830_B0520[ghost->animFrameIndex].assetIndex);

        ghost->animTimer = D_800BC830_B0520[ghost->animFrameIndex].frameDuration;
        ghost->animFrameIndex++;

        if (D_800BC830_B0520[ghost->animFrameIndex].frameDuration == 0) {
            ghost->animFrameIndex = 0;
        }
    }

    memcpy(&ghost->renderPositionBuffer, &ghost->posX, 0xC);

    for (viewport = 0; viewport < 4; viewport++) {
        func_800677C0_683C0(viewport, (loadAssetMetadata_arg *)&ghost->assetMetadata);
    }
}

void func_800BB388_AF078(AnimatedGhostEntity *ghost) {
    Vec3i direction;
    Vec3i *directionPtr;
    s32 randomSpeed;
    s32 rotationAngle;

    directionPtr = &direction;
    memcpy(directionPtr, &g_GhostBaseDirection, sizeof(Vec3i));

    getCurrentAllocation();

    ghost->assetData = load_3ECE40();
    ghost->assetMetadata = &g_GhostDefaultAssetMetadata;
    ghost->alpha = 0;

    randomSpeed = randA() & 0xFF;
    randomSpeed = randomSpeed << 10;
    direction.z = randomSpeed + 0x20000;

    rotationAngle = randA() & 0xFF;
    rotationAngle = rotationAngle << 5;

    rotateVectorY(directionPtr, rotationAngle, &ghost->velocity);

    if (ghost->entityType == 0) {
        ghost->collisionHeight = 0x1A;
    } else {
        ghost->collisionHeight = 0x58;
    }

    ghost->animTimer = 1;
    ghost->animFrameIndex = 0;
    ghost->lifetime = 0x3C;

    setCleanupCallback(cleanupAnimatedGhost);
    setCallbackWithContinue(fadeInGhost);
}

void cleanupAnimatedGhost(void **arg0) {
    *arg0 = freeNodeMemory(*arg0);
}

s32 updateGhostPositionAndCheckEnd(AnimatedGhostEntity *ghost) {
    Vec3i surfaceNormal;
    Allocation *allocation;
    void *collisionContext;
    s32 *posPtr;
    s32 shouldEnd;
    u16 newHeight;

    shouldEnd = 0;
    allocation = (Allocation *)getCurrentAllocation();

    if (allocation->gamePaused == 0) {
        collisionContext = &allocation->gameData;

        ghost->posX = ghost->posX + ghost->velocity.x;
        ghost->posZ = ghost->posZ + ghost->velocity.z;

        posPtr = &ghost->posX;

        newHeight = func_80060A3C_6163C(collisionContext, ghost->collisionHeight, posPtr);
        ghost->collisionHeight = newHeight;

        func_80060CDC_618DC(collisionContext, newHeight, posPtr, 0x80000, &surfaceNormal);

        ghost->posY = func_8005CFC0_5DBC0(collisionContext, (u16)ghost->collisionHeight, posPtr, 0x100000) + 0x180000;

        if (ghost->entityType == 0) {
            shouldEnd = (ghost->collisionHeight != 0x1A);
        } else {
            u16 height = ghost->collisionHeight;
            if ((u16)(height - 0x58) >= 2) {
                shouldEnd = 1;
            }
        }

        if (surfaceNormal.x != 0) {
            shouldEnd = 1;
        } else if (surfaceNormal.z != 0) {
            shouldEnd = 1;
        }

        if (ghost->lifetime == 0) {
            shouldEnd = 1;
        } else {
            ghost->lifetime = ghost->lifetime - 1;
        }
    }

    return shouldEnd;
}

void fadeInGhost(AnimatedGhostEntity *ghost) {
    ghost->alpha += 0x10;

    if (ghost->alpha == 0xE0) {
        ghost->fadeDirection = 1;
        setCallback(oscillateGhostFade);
    }

    if (updateGhostPositionAndCheckEnd(ghost)) {
        setCallback(fadeOutGhost);
    }

    updateGhostAnimation(ghost);
}

void oscillateGhostFade(AnimatedGhostEntity *ghost) {
    u8 unused;
    Player *nearbyPlayer;

    if (ghost->fadeDirection != 0) {
        ghost->alpha -= 0x10;
        if (ghost->alpha < 0x41) {
            ghost->fadeDirection = 0;
        }
    } else {
        ghost->alpha += 0x10;
        if (ghost->alpha >= 0xE0) {
            ghost->fadeDirection = 1;
        }
    }

    if (updateGhostPositionAndCheckEnd(ghost) != 0) {
        setCallback(fadeOutGhost);
    }

    nearbyPlayer = func_8005B548_5C148(&ghost->posX, -1, 0x100000);

    if (nearbyPlayer != NULL) {
        if (nearbyPlayer->unkBCF < 3) {
            spawnStarEffectImmediate(nearbyPlayer);
        }

        setCallback(fadeOutGhost);
    }

    updateGhostAnimation(ghost);
}

void fadeOutGhost(AnimatedGhostEntity *ghost) {
    ghost->alpha -= 0x10;

    if (ghost->alpha == 0) {
        func_80069CF8_6A8F8();
    } else {
        updateGhostPositionAndCheckEnd(ghost);
        updateGhostAnimation(ghost);
    }
}

void initGhostSpawnerTask(GhostSpawnerTask *arg0) {
    arg0->ghostType = 0;
    arg0->spawnTimer = 0x14;
    setCallback(func_800BB778_AF468);
}

INCLUDE_ASM("asm/nonmatchings/levels/haunted_house", func_800BB778_AF468);

void func_800BB8E8_AF5D8(func_800BB8E8_AF5D8_arg *arg0) {
    func_80055E68_56A68_result *temp_v0_3;
    u16 temp_a1;

    arg0->unk24 = func_80055DC4_569C4(9);
    arg0->unk28 = func_80055DF8_569F8(9);
    arg0->unk2C = 0;

    temp_v0_3 = func_80055E68_56A68(9);
    arg0->unk20 = &temp_v0_3->unk90;

    arg0->unk4E = 0xF;
    arg0->unk3C = 0xFD71ABE1;
    arg0->unk40 = 0x1E37049C;
    arg0->unk44 = 0xF61F3676;
    arg0->unk4C = 0;
    arg0->unk48 = 0;
    arg0->unk4A = 0x400;
    arg0->unk50 = 0x1E;

    temp_a1 = arg0->unk4A + 0x2A0;
    createYRotationMatrix((Transform3D *)arg0, temp_a1 & 0xFFFF);
    setCleanupCallback(func_800BBC2C_AF91C);
    setCallback(func_800BB9A4_AF694);
}

void func_800BB9A4_AF694(func_800BB8E8_AF5D8_arg *arg0) {
    Allocation *allocation;
    s32 i;
    s32 offset;
    Vec3i *pos;
    Player *player;

    allocation = (Allocation *)getCurrentAllocation();

    if (allocation->gamePaused == 0) {
        switch (arg0->unk48) {
            case 0:
                arg0->unk4E--;
                if (arg0->unk4E != 0) {
                    break;
                }
                arg0->unk48++;
                /* fallthrough */
            case 1:
                arg0->unk50--;
                arg0->unk4A -= 0x10;
                if ((s16)arg0->unk4A != -0x400) {
                    break;
                }
                arg0->unk4E = 0xF;
                arg0->unk48++;
                /* fallthrough */
            case 2:
                arg0->unk4E--;
                if (arg0->unk4E != 0) {
                    break;
                }
                arg0->unk48++;
                /* fallthrough */
            case 3:
                arg0->unk50--;
                arg0->unk4A += 0x10;
                if ((s16)arg0->unk4A != 0x400) {
                    break;
                }
                arg0->unk48 = 0;
                arg0->unk4E = 0xF;
                break;
        }

        arg0->unk4C += 0x40;

        i = 0;
        if (allocation->memoryPoolId > i) {
            do {
                isPlayerInRangeAndPull(&arg0->unk14, 0x200000, allocation->players + i);
                i += 1;
            } while (i < allocation->memoryPoolId);
        }

        createYRotationMatrix((Transform3D *)arg0, (arg0->unk4A + 0x2A0) & 0xFFFF);

        if (arg0->unk50 == 0) {
            arg0->unk50 = (randA() & 0xF) + 0xF;

            for (i = 0; i < allocation->memoryPoolId; i++) {
                player = allocation->players + i;
                if ((u16)(player->unkB94 - 0x38) < 5) {
                    break;
                }
            }

            if (i < allocation->memoryPoolId) {
                pos = &arg0->unk14;
                func_80055C80_56880(0, (s16)(arg0->unk4A + 0x2A0), pos);
                func_80055C80_56880(1, (s16)(arg0->unk4A + 0x2A0), pos);
            }
        }
    }

    memcpy(&arg0->unk14, &arg0->unk3C, 0xC);
    arg0->unk14.y = arg0->unk14.y + approximateSin(arg0->unk4C) * 0x1C0;

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void func_800BBC2C_AF91C(func_800BBC2C_AF91C_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void func_800BBC64_AF954(func_800BBC64_AF954_arg *arg0) {
    s32 temp_v1;

    arg0->unk0 = func_80055D7C_5697C(9);
    arg0->unk4 = &D_800BC8C8_B05B8;
    arg0->unk1E = 0xFF;
    memcpy(&arg0->unk8, &D_800BC908_B05F8, 12);
    temp_v1 = arg0->unkC + arg0->unk24;
    arg0->unk28 = 0xB4;
    arg0->unkC = temp_v1;
    setCleanupCallback(func_800BBCE8_AF9D8);
    setCallbackWithContinue(func_800BBD14_AFA04);
}

void func_800BBCE8_AF9D8(void **arg0) {
    *arg0 = freeNodeMemory(*arg0);
}

void func_800BBD14_AFA04(func_800BBC64_AF954_arg *arg0) {
    Allocation *allocation;
    s32 i;
    s32 *ptr8;
    s32 *ptrC;
    s32 *ptr10;

    allocation = getCurrentAllocation();

    if (allocation->gamePaused == 0) {
        arg0->unk28 -= 1;

        if (arg0->unk28 == 0) {
            func_80069CF8_6A8F8();
            return;
        }

        ptr8 = &arg0->unk8;
        *ptr8 += (D_800BC914_B0604 - *ptr8) / arg0->unk28;

        ptrC = &arg0->unkC;
        *ptrC += (D_800BC918_B0608 - (*ptrC + arg0->unk24)) / arg0->unk28;

        ptr10 = &arg0->unk10;
        *ptr10 += (D_800BC91C_B060C - *ptr10) / arg0->unk28;
    }

    loadAssetMetadata((loadAssetMetadata_arg *)&arg0->unk4, arg0->unk0, 5);

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)&arg0->unk4);
    }
}

void func_800BBE84_AFB74(s16 *arg0) {
    *arg0 = 0xB4;
    setCallback(func_800BBEAC_AFB9C);
}

void func_800BBEAC_AFB9C(s16 *arg0) {
    Allocation *allocation;
    Task *task;
    u8 rand;
    s32 value;

    allocation = (Allocation *)getCurrentAllocation();
    if (allocation->gamePaused != 0) {
        return;
    }

    *arg0 -= 1;
    if (*arg0 != 0) {
        return;
    }

    task = (Task *)scheduleTask(func_800BBC64_AF954, 0, 0, 0xC8);
    if (task != NULL) {
        rand = randA() & 0xFF;
        value = (rand * 5) << 13;
        task->unk24 = value;
    }

    rand = randA() & 0x1F;
    *arg0 = rand + 0xB4;
}

void func_800BBF4C_AFC3C(func_800BBF4C_AFC3C_arg *arg0) {
    s16 index;

    arg0->unk0 = func_80055D7C_5697C(9);
    arg0->unk4 = &D_800BC920_B0610;

    index = arg0->unk24;
    if (D_800BC9DC_B06CC[index] == 7) {
        arg0->unk4 = &D_800BC960_B0650;
    }

    setCleanupCallback(func_800BC0D0_AFDC0);
    setCallback(func_800BBFC8_AFCB8);
}

void func_800BBFC8_AFCB8(func_800BBF4C_AFC3C_arg *arg0) {
    Allocation *allocation;
    s32 i;
    s16 index;
    s32 sinResult;
    s16 multiplier;
    s32 *src;

    allocation = (Allocation *)getCurrentAllocation();

    if (allocation->gamePaused == 0) {
        index = arg0->unk24;
        arg0->unk26 += D_800BC9F4_B06E4[index];
    }

    index = arg0->unk24;
    src = D_800BC9A0_B0690;
    memcpy(&arg0->unk8, &src[index * 3], 0xC);

    sinResult = approximateSin(arg0->unk26);

    index = arg0->unk24;
    multiplier = D_800BC9E8_B06D8[index];
    arg0->unkC += sinResult * multiplier;

    index = arg0->unk24;
    loadAssetMetadata((loadAssetMetadata_arg *)&arg0->unk4, arg0->unk0, D_800BC9DC_B06CC[index]);

    func_8005BCB8_5C8B8(&arg0->unk8, 0x140000, 0x300000);

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)&arg0->unk4);
    }
}

void func_800BC0D0_AFDC0(void **arg0) {
    *arg0 = freeNodeMemory(*arg0);
}

void func_800BC0FC(s16 arg0) {
    func_800BC0FC_Task *task = (func_800BC0FC_Task *)scheduleTask(func_800BBF4C_AFC3C, 0, 0, 0xC8);
    if (task != NULL) {
        task->unk24 = arg0;
    }
}

void func_800BC13C_AFE2C(GhostManager *ghostManager) {
    ghostManager->unk4 = func_80055D7C_5697C(9);
    ghostManager->ghostSlotData = 0;
    setCleanupCallback(func_800BC340_B0030);
    setCallback(func_800BC184_AFE74);
}

void func_800BC184_AFE74(GhostManager *ghostManager) {
    s32 ghostIndex;
    s32 *tempPosition;
    Vec3i *spawnPositions;

    ghostManager->ghostSlotData = allocateNodeMemory(0x200);

    ghostIndex = 0;
    tempPosition = &D_8009A8A4_9B4A4;
    spawnPositions = D_800BCA00_B06F0;

    while (ghostIndex < 8) {
        memcpy(tempPosition, spawnPositions, 0xC);
        func_8006BFB8_6CBB8(tempPosition - 5, (void *)((u8 *)ghostManager->ghostSlotData + (ghostIndex << 6)));
        spawnPositions++;
        ghostIndex++;
    }

    setCallback(func_800BC220_AFF10);
}

void func_800BC220_AFF10(u8 *ghostSlots) {
    GameState *gameState;
    s32 i;
    s32 maxRaceState;
    s32 activeGhostCount;
    s32 playerCount;
    Player *player;
    s32 numPlayersToCheck;
    s32 raceState;
    s32 pad[4];

    (void)pad;

    gameState = getCurrentAllocation();
    numPlayersToCheck = gameState->numPlayers;
    maxRaceState = 0;
    i = 0;

    if (numPlayersToCheck > 0) {
        playerCount = numPlayersToCheck;
        player = gameState->players;
        do {
            raceState = player->unkB94;
            if ((u32)(raceState - 7) < 5U) {
                raceState &= 0xFFFF;
                if (maxRaceState < raceState) {
                    maxRaceState = raceState;
                }
            }
            i++;
            player = (Player *)((u8 *)player + sizeof(Player));
        } while (i < playerCount);
    }

    activeGhostCount = ((maxRaceState < 7) ^ 1) * 2;
    if (maxRaceState >= 8) {
        activeGhostCount = 4;
    }
    if (maxRaceState >= 9) {
        activeGhostCount = 6;
    }
    if (maxRaceState >= 10) {
        activeGhostCount = 8;
    }

    i = 0;
    if (activeGhostCount != 0) {
        do {
            (ghostSlots + i)[8] = 8;
            i++;
        } while (i < activeGhostCount);
    }

    i = activeGhostCount;
    while (i < 8) {
        (ghostSlots + i)[8] = 9;
        i++;
    }

    for (i = 0; i < 4; i++) {
        debugEnqueueCallback((u16)i, 4, func_800BC378_B0068, ghostSlots);
    }
}

void func_800BC340_B0030(GhostManager *ghostManager) {
    ghostManager->ghostSlotData = freeNodeMemory(ghostManager->ghostSlotData);
    ghostManager->unk4 = freeNodeMemory(ghostManager->unk4);
}

void func_800BC378_B0068(GhostRenderState *state) {
    OutputStruct_19E80 tableEntry;
    s32 prevTextureIndex;
    s32 i;

    prevTextureIndex = -1;
    gGraphicsMode = -1;
    gSPDisplayList(gRegionAllocPtr++, D_8009A780_9B380);

    for (i = 0; i < 8; i++) {
        if (isObjectCulled(&D_800BCA00_B06F0[i]) == 0) {
            u8 textureIndex = state->textureIndices[i];

            if (textureIndex != prevTextureIndex) {
                prevTextureIndex = textureIndex;
                getTableEntryByU16Index(state->textureTable, (u16)prevTextureIndex, &tableEntry);

                gDPLoadMultiBlock_4b(
                    gRegionAllocPtr++,
                    tableEntry.data_ptr,
                    0,
                    G_TX_RENDERTILE,
                    G_IM_FMT_CI,
                    tableEntry.field1,
                    tableEntry.field2,
                    0,
                    G_TX_CLAMP,
                    G_TX_CLAMP,
                    G_TX_NOMASK,
                    G_TX_NOMASK,
                    G_TX_NOLOD,
                    G_TX_NOLOD
                );

                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, tableEntry.index_ptr);
            }

            gSPMatrix(
                gRegionAllocPtr++,
                (u8 *)state->matrixData + (i << 6),
                G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW
            );

            gSPMatrix(gRegionAllocPtr++, D_800A8B14_9FE84, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);

            gSPVertex(gRegionAllocPtr++, D_800BCA60_B0750, 4, 0);

            gSP2Triangles(gRegionAllocPtr++, 0, 3, 2, 0, 2, 1, 0, 0);
        }
    }
}

void func_800BC72C_B041C(s16 *arg0) {
    *arg0 = 0;
    setCallback(func_800BC750_B0440);
}

void func_800BC750_B0440(s16 *targetLap) {
    s32 i;
    GameState *gameState;
    Player *player;
    u32 playerOffset;
    s16 lapValue;

    gameState = getCurrentAllocation();

    for (i = 0; i < gameState->unk5F; i++) {
        playerOffset = (u32)(i * sizeof(Player));
        player = (Player *)((u32)playerOffset + (u32)gameState->players);
        lapValue = *targetLap;

        if (player->unkBC5 == lapValue) {
            if ((u32)(player->unkB94 - 7) < 4) {
                *targetLap = lapValue + 1;
                func_80058530_59130(0xC, 6);
            }
        }
    }
}
