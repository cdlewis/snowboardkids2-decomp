#include "52880.h"
#include "56910.h"
#include "594E0.h"
#include "5AA90.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
#include "rand.h"
#include "task_scheduler.h"

typedef struct {
    u8 padding[0x24];
    void *unk24;
    void *unk28;
} func_800BBA60_B7CA0_arg;

typedef struct {
    Transform3D matrix;
    DisplayLists *unk20;
    void *unk24;
    void *unk28;
    s32 unk2C;
    u8 PAD[0xC];
    s16 unk3C;
    s16 unk3E;
} func_800BB808_B7A48_arg;

typedef struct {
    u16 rotX;
    u16 rotY;
    s32 position[3];
} RockPositionEntry;

extern RockPositionEntry crazyJungleRockPositions[];

typedef struct {
    /* 0x00 */ DisplayListObject node1;
    /* 0x3C */ DisplayListObject node2;
    s32 rotationMatrix;
    s8 pad_7C[0x10];
    s32 posX;
    s8 pad_90[0x4];
    s32 posY;
    s32 posZ;
    s32 animTimer;
    s32 posOffset;
    s16 xRotation;
    s16 positionIndex;
    s16 respawnTimer;
} FallingRockHazard;

typedef struct {
    Node n;
    u8 padding[0x7A];
    s16 positionIndex;
} NodeWithPayload;

typedef struct {
    /* 0x0 */ DisplayListObject node;
    u16 unk3C;
    s16 unk3E;
} func_800BB8B8_B7AF8_arg;

extern s32 D_8009A8A4_9B4A4;

void renderFallingRockHazard(FallingRockHazard *arg0);
void updateFallingRockHazard(FallingRockHazard *arg0);
void func_800BB658_B7898(FallingRockHazard *arg0);
void func_800BBA60_B7CA0(func_800BBA60_B7CA0_arg *arg0);
void func_800BB7D0_B7A10(func_800BBA60_B7CA0_arg *);
void func_800BB71C_B795C(FallingRockHazard *);
void func_800BB8B8_B7AF8(func_800BB8B8_B7AF8_arg *arg0);

void initFallingRockHazard(FallingRockHazard *rock) {
    GameState *gameState;
    s32 randVal;
    func_80055E68_56A68_result *result;

    gameState = (GameState *)getCurrentAllocation();
    randVal = randA();

    rock->positionIndex += (randVal & 1);
    result = func_80055E68_56A68(gameState->memoryPoolId);
    rock->node1.displayLists = &result->unk90;
    result = func_80055E68_56A68(gameState->memoryPoolId);
    rock->node2.displayLists = &result->unkA0;
    rock->node1.segment1 = func_80055DC4_569C4(gameState->memoryPoolId);
    rock->node1.segment2 = func_80055DF8_569F8(gameState->memoryPoolId);
    rock->node1.segment3 = 0;
    rock->posZ = 0;
    rock->animTimer = 0;
    rock->posOffset = 0;
    rock->node2.segment1 = rock->node1.segment1;
    rock->node2.segment2 = rock->node1.segment2;
    rock->node2.segment3 = rock->node1.segment3;
    memcpy(&rock->posX, &crazyJungleRockPositions[rock->positionIndex].position, 0xC);
    createCombinedRotationMatrix(&rock->rotationMatrix, crazyJungleRockPositions[rock->positionIndex].rotX, crazyJungleRockPositions[rock->positionIndex].rotY);
    rock->xRotation = 0;

    setCleanupCallback(func_800BB7D0_B7A10);
    setCallback(updateFallingRockHazard);
}

void renderFallingRockHazard(FallingRockHazard *rock) {
    s32 matrix[8];
    s32 i;

    memcpy(&D_8009A8A4_9B4A4, &rock->posZ, 0xC);
    func_8006B084_6BC84(&D_8009A8A4_9B4A4 - 5, &rock->rotationMatrix, rock);
    createXRotationMatrix((s16(*)[3])matrix, rock->xRotation);

    matrix[6] = 0x3b333;
    matrix[5] = 0;
    matrix[7] = 0x170000;

    func_8006B084_6BC84(&matrix[0], rock, &rock->node2);

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, &rock->node1);
        enqueueDisplayListWithFrustumCull(i, &rock->node2);
    }
}

void updateFallingRockHazard(FallingRockHazard *arg0) {
    GameState *gs;
    s32 sp20;
    s32 sp28;
    s32 flag;
    s32 i;

    gs = (GameState *)getCurrentAllocation();
    flag = 0;

    for (i = 0; i < gs->numPlayers; i++) {
        sp20 = gs->players[i].worldPos.x - arg0->posX;
        sp28 = gs->players[i].worldPos.z - arg0->posY;
        if (((0x27FFFFE >= ((u32)sp20) + 0x13FFFFF) & (0x13FFFFF >= sp28)) == 0) {
            continue;
        }

        if ((s32)0xFF600000 < sp28) {
            flag = 1;
            break;
        }
    }

    if (flag) {
        if (gs->gamePaused == 0) {
            if (arg0->animTimer != 0x60000) {
                arg0->animTimer += 0x20000;
            }

            if (arg0->xRotation != (-0x600)) {
                arg0->xRotation -= 0x100;
            }
        }

        renderFallingRockHazard(arg0);

        if (gs->gamePaused == 0) {
            for (i = 0; i < gs->numPlayers; i++) {
                if (isPlayerInRangeAndPull(&arg0->node2.transform.translation, 0x12A000, &gs->players[i]) != 0) {
                    if (isPlayerInRangeAndPull(&arg0->node2.transform.translation, 0x1E3000, &gs->players[i]) != 0) {
                        setPlayerState50(&gs->players[i]);
                        queueSoundAtPosition(&arg0->node2.transform.translation, 0x2A);
                        setCallback(func_800BB658_B7898);
                    }
                }
            }
        }
    } else {
        if (!gs->gamePaused) {
            if (arg0->animTimer > 0) {
                arg0->animTimer += 0xFFFE0000;
            }

            if (arg0->animTimer < 0) {
                arg0->animTimer += 0x20000;
            }

            if (arg0->xRotation != 0) {
                arg0->xRotation += 0x100;
            }
        }

        renderFallingRockHazard(arg0);
    }
}

void func_800BB658_B7898(FallingRockHazard *arg0) {
    GameState *gs;
    s32 i;

    gs = (GameState *)getCurrentAllocation();
    if (arg0->animTimer != 0xFFF00000) {
        if (gs->gamePaused == FALSE) {
            arg0->animTimer = arg0->animTimer - 0x8000;
        }
    } else {
        arg0->respawnTimer = 0x12C;
        setCallback(&func_800BB71C_B795C);
    }
    arg0->xRotation = 0;
    renderFallingRockHazard(arg0);

    for (i = 0; i < gs->numPlayers; i++) {
        isPlayerInRangeAndPull(&arg0->node2.transform.translation, 0x12A000, &gs->players[i]);
    }
}

void func_800BB71C_B795C(FallingRockHazard *arg0) {
    u8 temp;

    if (((GameState *)getCurrentAllocation())->gamePaused == FALSE) {
        arg0->respawnTimer--;
    }

    if (!arg0->respawnTimer) {
        temp = randA() & 1;
        arg0->positionIndex = temp + (arg0->positionIndex & 0xFE);
        memcpy(&arg0->posX, &crazyJungleRockPositions[arg0->positionIndex].position, 0xC);
        createCombinedRotationMatrix(
            &arg0->rotationMatrix,
            crazyJungleRockPositions[arg0->positionIndex].rotX,
            crazyJungleRockPositions[arg0->positionIndex].rotY
        );
        setCallback(&updateFallingRockHazard);
    }
}

void func_800BB7D0_B7A10(func_800BBA60_B7CA0_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void func_800BB808_B7A48(func_800BB808_B7A48_arg *arg0) {
    GameState *gs = (GameState *)getCurrentAllocation();
    arg0->unk24 = func_80055DC4_569C4(gs->memoryPoolId);
    arg0->unk28 = func_80055DF8_569F8(gs->memoryPoolId);
    arg0->unk2C = 0;
    createYRotationMatrix(&arg0->matrix, 0x6C0);
    arg0->matrix.translation.x = 0xDD196FEA;
    arg0->matrix.translation.y = 0x0ABD4CA3;
    arg0->matrix.translation.z = 0xE270649E;
    arg0->unk3C = 0x12C;
    arg0->unk3E = 0;
    arg0->unk20 = &func_80055E68_56A68(gs->memoryPoolId)->unkB0;
    setCleanupCallback(&func_800BBA60_B7CA0);
    setCallback(&func_800BB8B8_B7AF8);
}

void func_800BB8B8_B7AF8(func_800BB8B8_B7AF8_arg *arg0) {
    GameState *gameState;
    s16 state;
    s32 i;
    u8 numPlayers;
    s32 randVal;

    gameState = getCurrentAllocation();
    state = arg0->unk3E;

    switch (state) {
        case 0:
            if (!gameState->gamePaused) {
                arg0->unk3C--;
                if ((arg0->unk3C << 16) == 0) {
                    arg0->unk3E++;
                }
            }
            break;
        case 1:
            if (gameState->gamePaused == 0) {
                numPlayers = gameState->numPlayers;

                for (i = 0; i < numPlayers; i++) {
                    if ((u32)gameState->players[i].sectorIndex - 0x60 < 6) {
                        arg0->node.displayLists = &func_80055E68_56A68(gameState->memoryPoolId)->unkC0;
                        randVal = (u8)randA();
                        randVal = randVal - 0x60;
                        i = randVal << 1;
                        i = i + randVal;
                        i = i + 0x6C0;
                        spawnFallingStarProjectile(i, (((u8)randA()) << 12) | 0x100000);
                        queueSoundAtPosition(&arg0->node.transform.translation, 0x23);
                        arg0->unk3C = 0x18;
                        arg0->unk3E++;
                        break;
                    }
                }
            }
            break;
        case 2:
            if (gameState->gamePaused == 0) {
                arg0->unk3C--;
                if ((arg0->unk3C << 16) == 0) {
                    arg0->unk3E = 0;
                    arg0->node.displayLists = &func_80055E68_56A68(gameState->memoryPoolId)->unkB0;
                    arg0->unk3C = 0x14;
                }
            }
            break;
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, &arg0->node);
    }
}

void func_800BBA60_B7CA0(func_800BBA60_B7CA0_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void func_800BBA98(void) {
    s32 i;
    NodeWithPayload *temp;

    temp = (NodeWithPayload *)scheduleTask(&initFallingRockHazard, 0, 0, 0x32);
    if (temp != NULL) {
        temp->positionIndex = 0;
    }

    temp = (NodeWithPayload *)scheduleTask(&initFallingRockHazard, 0, 0, 0x32);
    if (temp != NULL) {
        temp->positionIndex = 2;
    }

    temp = (NodeWithPayload *)scheduleTask(&initFallingRockHazard, 0, 0, 0x32);
    if (temp != NULL) {
        temp->positionIndex = 4;
    }

    temp = (NodeWithPayload *)scheduleTask(&initFallingRockHazard, 0, 0, 0x32);
    if (temp != NULL) {
        temp->positionIndex = 6;
    }

    temp = (NodeWithPayload *)scheduleTask(&initFallingRockHazard, 0, 0, 0x32);
    if (temp != NULL) {
        temp->positionIndex = 8;
    }

    scheduleTask(&func_800BB808_B7A48, 0, 0, 0xC8);
}
