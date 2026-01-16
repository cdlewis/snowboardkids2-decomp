#include "36B80.h"
#include "56910.h"
#include "5E590.h"
#include "68CF0.h"
#include "6DE50.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
#include "race_session.h"
#include "rand.h"
#include "task_scheduler.h"

typedef struct {
    void *assetTable;
    void *indicatorAsset;
} PlayerIndicatorTask;

typedef struct {
    u8 padding[0x8];
    s16 yPosition;
    s16 padding2;
    s16 scaleX;
    s16 scaleY;
} PushStartTextState;

typedef struct {
    u8 _pad[0x9F0];
    s16 unk9F0;
    u8 _pad2[0x1C6];
    u8 unkBB8;
    u8 _pad3[0x25];
    u8 unkBDE;
} func_8004083C_4143C_struct24;

typedef struct {
    DataTable_19E80 *unk0;
    loadAssetMetadataByIndex_arg unk4;
    u8 _pad1[0x24 - 4 - sizeof(loadAssetMetadataByIndex_arg)];
    func_8004083C_4143C_struct24 *unk24;
    u8 _pad2[0x10];
    s32 unk38;
} func_80040870_arg;

extern s32 gFrameCounter;
extern s32 D_800907F8_913F8;
extern s32 D_800907EC_913EC[];
extern s16 identityMatrix[];
extern s16 D_800907E0_913E0;
extern void *D_80090860_91460;

typedef struct {
    Transform3D rotationMatrix; // 0x00-0x1F, with position at 0x14-0x1F
    void *unk20;
    void *unk24;
    void *unk28;
    s32 unk2C;
    u8 pad[0xC];
    Transform3D unk3C;
    void *unk5C;
    void *unk60;
    void *unk64;
    s32 unk68;
    u8 pad2[0xC];
    Transform3D unk78;
    s32 unk98;
    void *unk9C;
    void *unkA0;
    s32 unkA4;
    u8 pad3[0xC];
    s16 gateRotation;
    s16 animationState;
    s16 pauseTimer;
} StartGate;

void updateStartGate(StartGate *);
void updatePushStartText(PushStartTextState *);
void func_80040870_41470(func_80040870_arg *arg0);

typedef struct {
    void *unk0;
} func_80040948_41548_arg;

typedef struct {
    u8 _pad[0x24];
    void *unk24;
    void *unk28;
} StartGateCleanupArg;

void cleanupStartGate(StartGateCleanupArg *);

typedef struct {
    u8 high;
    u8 low;
} Bytes;

typedef union {
    s16 asS16;
    Bytes asBytes;
} S16OrBytes;

typedef union {
    s16 asS16;
    Bytes asBytes;
} S16OrBytesC;

typedef union {
    s16 asS16;
    Bytes asBytes;
} S16OrBytesE;

typedef struct {
    void *graphicAsset;
    void *textAsset;
    s16 yPosition;
    s16 useGraphicMode;
    S16OrBytesC scaleX;
    S16OrBytesE scaleY;
    S16OrBytes pulseAlpha;
    s16 pulseDirection;
} PushStartPromptTask;

void updatePushStartGraphic(PushStartPromptTask *);
void cleanupPushStartPrompt(PushStartPromptTask *);
void awaitPlayerIndicatorReady(func_80040870_arg *arg0);
void cleanupPlayerIndicator(func_80040948_41548_arg *arg0);

void initPlayerIndicator(PlayerIndicatorTask *task) {
    GameState *gameState = (GameState *)getCurrentAllocation();
    task->assetTable = load_3ECE40();
    task->indicatorAsset = (u8 *)&gameState->unk44->unk1380;
    setCleanupCallback(&cleanupPlayerIndicator);
    setCallbackWithContinue(&awaitPlayerIndicatorReady);
}

void awaitPlayerIndicatorReady(func_80040870_arg *arg0) {
    func_8004083C_4143C_struct24 *player;

    player = (func_8004083C_4143C_struct24 *)arg0->unk24;
    if (player->unkBDE != 0) {
        arg0->unk38 = 6;
        setCallback(func_80040870_41470);
    }
}

void func_80040870_41470(func_80040870_arg *arg0) {
    func_8004083C_4143C_struct24 *temp;
    loadAssetMetadataByIndex_arg *temp_s0;
    s32 temp_v0;

    temp = arg0->unk24;
    if (temp->unkBDE == 0) {
        temp_v0 = arg0->unk38 - 1;
        arg0->unk38 = temp_v0;
        if (temp_v0 == 0) {
            setCallback(awaitPlayerIndicatorReady);
            return;
        }
    } else {
        arg0->unk38 = 6;
    }
    transformVector(&D_800907E0_913E0, &arg0->unk24->unk9F0, &arg0->unk4.unk4);
    temp_s0 = &arg0->unk4;
    if (arg0->unk24->unkBDE >= 2 && (gFrameCounter & 1)) {
        loadAssetMetadataByIndex(temp_s0, arg0->unk0, 0x60, 0x14);
    } else {
        temp_s0++;
        temp_s0--;
        loadAssetMetadataByIndex(temp_s0, arg0->unk0, 0x60, 0x13);
    }
    enqueueTexturedBillboardSprite(arg0->unk24->unkBB8, (TexturedBillboardSprite *)temp_s0);
}

void cleanupPlayerIndicator(func_80040948_41548_arg *arg0) {
    arg0->unk0 = freeNodeMemory(arg0->unk0);
}

void spawnPlayerIndicatorTask(void *cleanupArg) {
    Node *task = scheduleTask(&initPlayerIndicator, 0U, 0U, 0xF0U);
    if (task != NULL) {
        task->cleanupCallback = cleanupArg;
    }
}

void initStartGate(StartGate *gate) {
    s32 tempMatrix[8];
    s32 worldPos[4];
    s32 *transformMatrix;
    GameState *gameState;
    LevelConfig *spawnData;
    u16 trackAngle;

    gameState = (GameState *)getCurrentAllocation();
    spawnData = getLevelConfig(gameState->memoryPoolId);
    gate->unk20 = (void *)((u8 *)getSkyDisplayLists3ByIndex(gameState->memoryPoolId) + 0x50);
    gate->unk24 = loadUncompressedAssetByIndex(gameState->memoryPoolId);
    gate->unk28 = loadCompressedSegment2AssetByIndex(gameState->memoryPoolId);
    gate->unk2C = 0;
    trackAngle = getTrackEndInfo((u8 *)gameState + 0x30, worldPos);
    createYRotationMatrix(&gate->rotationMatrix, (trackAngle + spawnData->yawOffset) & 0xFFFF);
    rotateVectorY(&D_800907EC_913EC, trackAngle + spawnData->yawOffset, &gate->rotationMatrix.translation);
    gate->rotationMatrix.translation.x = gate->rotationMatrix.translation.x + spawnData->shortcutPosX;
    gate->rotationMatrix.translation.z = gate->rotationMatrix.translation.z + spawnData->shortcutPosZ;
    gate->rotationMatrix.translation.y = worldPos[1];
    gate->unk5C = (void *)((u8 *)getSkyDisplayLists3ByIndex(gameState->memoryPoolId) + 0x60);
    transformMatrix = tempMatrix;
    gate->unk60 = gate->unk24;
    gate->unk64 = gate->unk28;
    gate->unk68 = gate->unk2C;
    memcpy(transformMatrix, identityMatrix, 0x20);
    transformMatrix[6] = 0x180000;
    func_8006B084_6BC84(transformMatrix, gate, &gate->unk3C);
    gate->unk9C = gate->unk24;
    gate->unkA0 = gate->unk28;
    gate->unkA4 = gate->unk2C;
    transformMatrix[6] = 0x160000;
    transformMatrix[7] = 0xA3333;
    func_8006B084_6BC84(transformMatrix, gate, &gate->unk78);
    gate->gateRotation = 0;
    gate->animationState = 0;
    setCleanupCallback(cleanupStartGate);
    setCallback(updateStartGate);
}

void updateStartGate(StartGate *gate) {
    Transform3D sp10;
    Transform3D *s0;
    GameState *gameState;
    s32 i;

    gameState = (GameState *)getCurrentAllocation();

    switch (gate->animationState) {
        case 0:
            if (gameState->shortcutGateState != 3) {
                goto block_else;
            }
            gate->animationState++;
            queueSoundAtPosition(&gate->rotationMatrix.translation, 0xA);
            /* fallthrough */
        case 1:
            if (gameState->gamePaused == 0) {
                gate->gateRotation -= 0x80;
            }
            if (gate->gateRotation == -0x800) {
                gate->pauseTimer = 0xA;
                gate->animationState++;
            }
            createZRotationMatrix(&sp10, gate->gateRotation);
            sp10.translation.x = 0;
            sp10.translation.y = 0xC0000;
            sp10.translation.z = 0;
            func_8006B084_6BC84(&sp10, gate, &gate->unk3C);
            break;
        case 2:
            if (gameState->gamePaused == 0) {
                gate->pauseTimer--;
            }
            if (gate->pauseTimer == 0) {
                gate->animationState++;
            }
            break;
        case 3:
            if (gameState->gamePaused == 0) {
                gate->gateRotation += 0x100;
            }
            if (gate->gateRotation == 0) {
                gate->animationState++;
            }
            s0 = &gate->unk3C;
            createZRotationMatrix(s0, gate->gateRotation);
            createZRotationMatrix(&sp10, gate->gateRotation);
            sp10.translation.x = 0;
            sp10.translation.y = 0xC0000;
            sp10.translation.z = 0;
            func_8006B084_6BC84(&sp10, gate, s0);
            break;
        case 4:
            if (gameState->shortcutGateState != 3) {
                gate->animationState = 0;
            }
            break;
    }

    if (gameState->shortcutGateState == 3) {
        gate->unk98 = (s32)getSkyDisplayLists3ByIndex(gameState->memoryPoolId) + 0x70;
    } else {
    block_else:
        gate->unk98 = (s32)getSkyDisplayLists3ByIndex(gameState->memoryPoolId) + 0x80;
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)gate);
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)&gate->unk3C);
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)&gate->unk78);
    }
}

void cleanupStartGate(StartGateCleanupArg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void initPushStartPrompt(PushStartPromptTask *task) {
    if (task->useGraphicMode == 0) {
        task->graphicAsset = NULL;
        task->textAsset = NULL;
        setCallback(&updatePushStartText);
    } else {
        task->textAsset = loadTextRenderAsset(1);
        task->graphicAsset = loadAsset_34F7E0();
        task->pulseAlpha.asS16 = 0x80;
        task->pulseDirection = -8;
        setCallback(&updatePushStartGraphic);
    }
    setCleanupCallback(&cleanupPushStartPrompt);
}

const char pushStartButtonText[] = "PUSH START BUTTON";

void updatePushStartText(PushStartTextState *state) {
    if (gFrameCounter & 8) {
        enqueueTextRender(
            -0x44,
            state->yPosition,
            0,
            (u8 *)&pushStartButtonText,
            (s32)state->scaleX,
            (s32)state->scaleY
        );
    }
}

void updatePushStartGraphic(PushStartPromptTask *task) {
    func_80035260_35E60(
        task->textAsset,
        (void *)&D_800907F8_913F8,
        -0x68,
        task->yPosition,
        0xFF,
        0xFF,
        0,
        task->scaleX.asBytes.low,
        task->scaleY.asBytes.low
    );

    task->pulseAlpha.asS16 += task->pulseDirection;

    if (task->pulseAlpha.asS16 == 0x20) {
        task->pulseDirection = 8;
    }

    if (task->pulseAlpha.asS16 == 0x80) {
        task->pulseDirection = -8;
    }

    func_8006D4B8_6E0B8(
        task->graphicAsset,
        -0x68,
        task->yPosition,
        0xD,
        2,
        0,
        task->pulseAlpha.asBytes.low,
        0xFF,
        0,
        0,
        0xC0,
        task->scaleX.asBytes.low,
        task->scaleY.asBytes.low
    );
}

void cleanupPushStartPrompt(PushStartPromptTask *task) {
    task->graphicAsset = freeNodeMemory(task->graphicAsset);
    task->textAsset = freeNodeMemory(task->textAsset);
}

extern u8 gConnectedControllerMask;

void spawnPushStartPrompt(s32 arg0, s16 yPosition, u8 arg2, u8 arg3, s16 scaleX, s16 scaleY) {
    PushStartPromptTask *task;

    if (((s16)arg0 == 0) || !(gConnectedControllerMask & 1)) {
        task = (PushStartPromptTask *)scheduleTask(&initPushStartPrompt, arg2, arg3, 0xC8);
        if (task != NULL) {
            if (gConnectedControllerMask & 1) {
                task->useGraphicMode = 0;
            } else {
                task->useGraphicMode = 1;
            }
            task->yPosition = yPosition;
            task->scaleX.asS16 = scaleX;
            task->scaleY.asS16 = scaleY;
        }
    }
}

typedef struct {
    void *particleAsset;
    void *particles;
    void *cameraNode;
    s32 lastCameraX;
    s32 lastCameraY;
    s32 lastCameraZ;
    s16 frameCounter;
    s16 particleCount;
    u8 pauseWhenPaused;
} ConfettiEffectTask;

void setupConfettiParticles(ConfettiEffectTask *task);
void cleanupConfettiEffect(ConfettiEffectTask *task);
void func_8004119C_41D9C(ConfettiEffectTask *task);

void initConfettiEffect(ConfettiEffectTask *task) {
    s16 count;

    task->particleAsset = loadSpriteAssetByIndex(0xB);
    count = task->particleCount;
    task->particles = allocateNodeMemory((count * 7) * 8);
    setCleanupCallback(&cleanupConfettiEffect);
    setCallbackWithContinue(&setupConfettiParticles);
}

void setupConfettiParticles(ConfettiEffectTask *task) {
    s32 pad[2];
    s32 offset;
    s32 i;
    s32 gravity;
    void *extPtr;

    do {
        (void)pad;
        i = 0;
        if (task->particleCount > 0) {
            gravity = 0x40000;
            extPtr = &D_80090860_91460;
            offset = 0;
            do {
                i++;
                *(s32 *)(offset + (s32)task->particles + 0x20) = (randA() & 0xFF) << 17;
                *(s32 *)(offset + (s32)task->particles + 0x24) = (randA() & 0xFF) << 17;
                *(s32 *)(offset + (s32)task->particles + 0x28) = (randA() & 0xFF) << 17;
                *(s32 *)(offset + (s32)task->particles + 0x2C) = gravity;
                *(s32 *)(offset + (s32)task->particles + 0x30) = -((randA() & 0xFF) << 8) - gravity;
                *(s32 *)(offset + (s32)task->particles + 0x34) = 0;
                loadAssetMetadata((loadAssetMetadata_arg *)(task->particles + offset), task->particleAsset, 0);
                *(void **)(offset + (s32)task->particles) = extPtr;
                offset += 0x38;
            } while (i < task->particleCount);
        }
    } while (0);
    memcpy(&task->lastCameraX, (u8 *)task->cameraNode + 0x134, 0xC);
    setCallbackWithContinue(&func_8004119C_41D9C);
}

void func_8004119C_41D9C(ConfettiEffectTask *task) {
    s32 pad[2];
    s32 i;
    s32 offset;
    s32 running;
    s32 mask;
    s32 cameraOffset;

    (void)pad;
    running = 1;
    i = 0;
    if (task->particleCount > 0) {
        mask = 0x1FFFFFF;
        cameraOffset = 0xFF000000;
        offset = 0;
        do {
            if (task->pauseWhenPaused != 0) {
                running &= -(*(u8 *)((u8 *)getCurrentAllocation() + 0x76) == 0);
            }
            if (running != 0) {
                {
                    s32 *particle = (s32 *)(offset + (s32)task->particles);
                    particle[8] += particle[0xB];
                }
                {
                    s32 *particle = (s32 *)(offset + (s32)task->particles);
                    particle[9] += particle[0xC];
                }
                if (*(s32 *)(offset + (s32)task->particles + 0x24) < 0) {
                    *(s32 *)(offset + (s32)task->particles + 0x20) = (randA() & 0xFF) << 17;
                    *(s32 *)(offset + (s32)task->particles + 0x24) = 0x02000000;
                    *(s32 *)(offset + (s32)task->particles + 0x28) = (randA() & 0xFF) << 17;
                    *(s32 *)(offset + (s32)task->particles + 0x30) = 0xFFFC0000 - ((randA() & 0xFF) << 8);
                }
            }
            {
                s32 *particle = (s32 *)(offset + (s32)task->particles);
                particle[8] -= *(s32 *)((u8 *)task->cameraNode + 0x134) - task->lastCameraX;
            }
            {
                s32 *particle = (s32 *)(offset + (s32)task->particles);
                particle[8] &= mask;
            }
            {
                s32 *particle = (s32 *)(offset + (s32)task->particles);
                particle[0xA] -= *(s32 *)((u8 *)task->cameraNode + 0x13C) - task->lastCameraZ;
            }
            {
                s32 *particle = (s32 *)(offset + (s32)task->particles);
                particle[0xA] &= mask;
            }
            {
                s32 *particle = (s32 *)(offset + (s32)task->particles);
                s32 camX = *(s32 *)((u8 *)task->cameraNode + 0x134) + cameraOffset;
                particle[1] = particle[8] + camX;
            }
            {
                s32 *particle = (s32 *)(offset + (s32)task->particles);
                s32 camY = *(s32 *)((u8 *)task->cameraNode + 0x138) + cameraOffset;
                particle[2] = particle[9] + camY;
            }
            {
                s32 *particle = (s32 *)(offset + (s32)task->particles);
                s32 camZ = *(s32 *)((u8 *)task->cameraNode + 0x13C) + cameraOffset;
                particle[3] = particle[0xA] + camZ;
            }
            enqueueTexturedBillboardSprite(task->frameCounter, (void *)((s32)task->particles + offset));
            i++;
            offset += 0x38;
        } while (i < task->particleCount);
    }
    memcpy(&task->lastCameraX, (u8 *)task->cameraNode + 0x134, 0xC);
}

void cleanupConfettiEffect(ConfettiEffectTask *task) {
    task->particleAsset = freeNodeMemory(task->particleAsset);
    task->particles = freeNodeMemory(task->particles);
}

INCLUDE_ASM("asm/nonmatchings/413E0", func_80041418_42018);

typedef struct {
    u8 _pad[0x8];
    void *cameraNode;
    u8 _pad2[0xC];
    s16 frameCounter;
    s16 particleCount;
    u8 pauseWhenPaused;
} ConfettiSpawnArgs;

void spawnConfettiEffect(void *cameraNode) {
    ConfettiSpawnArgs *task;

    task = (ConfettiSpawnArgs *)scheduleTask(&initConfettiEffect, 0, 0, 0xF0);

    if (task != NULL) {
        task->frameCounter = 0;
        task->cameraNode = cameraNode;
        task->particleCount = 0x64;
        task->pauseWhenPaused = 0;
    }
}
