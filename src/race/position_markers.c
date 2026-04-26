#include "race/position_markers.h"
#include "audio/audio.h"
#include "common.h"
#include "data/asset_metadata.h"
#include "data/course_data.h"
#include "data/data_table.h"
#include "gamestate.h"
#include "graphics/clip_text_render.h"
#include "graphics/displaylist.h"
#include "graphics/graphics.h"
#include "math/geometry.h"
#include "math/rand.h"
#include "race/race_session.h"
#include "system/task_scheduler.h"
#include "text/font_assets.h"
#include "text/font_render.h"

typedef struct {
    void *assetTable;
    void *indicatorAsset;
} PlayerIndicatorTask;

typedef struct {
    u8 _pad[0x9F0];
    s16 unk9F0;
    u8 _pad2[0x1C6];
    u8 unkBB8;
    u8 _pad3[0x25];
    u8 pathFlags;
} PlayerIndicatorPlayer;

typedef struct {
    DataTable_19E80 *unk0;
    loadAssetMetadataByIndex_arg unk4;
    u8 _pad1[0x24 - 4 - sizeof(loadAssetMetadataByIndex_arg)];
    PlayerIndicatorPlayer *unk24;
    u8 _pad2[0x10];
    s32 unk38;
} PlayerIndicatorSpriteTask;

typedef struct {
    u8 high;
    u8 low;
} Bytes;

typedef union {
    s16 asS16;
    Bytes asBytes;
} S16OrBytes;

typedef struct {
    void *graphicAsset;
    void *textAsset;
    s16 yPosition;
    s16 useGraphicMode;
    S16OrBytes scaleX;
    S16OrBytes scaleY;
    S16OrBytes pulseAlpha;
    s16 pulseDirection;
} PushStartPromptTask;

typedef struct {
    /* 0x00 */ loadAssetMetadata_arg sprite;
    /* 0x1C */ s32 pad1C;
    /* 0x20 */ Vec3i worldPos;
    /* 0x2C */ Vec3i velocity;
} ConfettiParticle; /* 0x38 bytes */

typedef struct {
    /* 0x00 */ void *particleAsset;
    /* 0x04 */ ConfettiParticle *particles;
    /* 0x08 */ ViewportNode *cameraNode;
    /* 0x0C */ Vec3i lastCameraPos;
    /* 0x18 */ s16 frameCounter;
    /* 0x1A */ s16 particleCount;
    /* 0x1C */ u8 pauseWhenPaused;
} ConfettiEffectTask;

void updateStartGate(StartGate *);
void cleanupStartGate(StartGate *);
void updatePushStartText(PushStartPromptTask *);
void updatePushStartGraphic(PushStartPromptTask *);
void cleanupPushStartPrompt(PushStartPromptTask *);
void updateRacePlayerIndicatorSprite(PlayerIndicatorSpriteTask *arg0);
void awaitPlayerIndicatorReady(PlayerIndicatorSpriteTask *arg0);
void cleanupPlayerIndicator(PlayerIndicatorTask *arg0);
void setupConfettiParticles(ConfettiEffectTask *task);
void updateConfettiParticles(ConfettiEffectTask *task);
void cleanupConfettiEffect(ConfettiEffectTask *task);

extern s32 gFrameCounter;
extern u8 gConnectedControllerMask;

Vec3i gIndicatorSpriteOffset = { 0x00000000, 0x00200000, 0x00000000 };

Vec3i D_800907EC_913EC = { 0x001E0000, 0x00000000, 0xFFCE0000 };

s32 D_800907F8_913F8[] = {
    0xFFFC0005, 0x001D8042, 0x803F803B, 0xFFFB803C, 0x80338033, 0xFFFB8041, 0x80358032, 0xFFFB803D, 0x803C8044,
    0x8032803F, 0xFFFB802F, 0x80328033, 0x803C803F, 0x8032FFFD, 0x8036803B, 0x80408032, 0x803F8041, 0x8036803B,
    0x8034FFFB, 0x802EFFFB, 0x8030803C, 0x803B8041, 0x803F803C, 0x80398039, 0x8032803F, 0x8050FFFF,
};

u32 D_80090860_91460[] = {
    0xFFFE0002, 0x00000000, 0xFFF0FFF0, 0xFFFFFFFF, 0x00020002, 0x00000000, 0x03F0FFF0, 0xFFFFFFFF,
    0x0002FFFE, 0x00000000, 0x03F003F0, 0xFFFFFFFF, 0xFFFEFFFE, 0x00000000, 0xFFF003F0, 0xFFFFFFFF,
};

void initPlayerIndicator(PlayerIndicatorTask *task) {
    GameState *gameState = (GameState *)getCurrentAllocation();
    task->assetTable = load_3ECE40();
    task->indicatorAsset = (u8 *)&gameState->unk44->unk1380;
    setCleanupCallback(&cleanupPlayerIndicator);
    setCallbackWithContinue(&awaitPlayerIndicatorReady);
}

void awaitPlayerIndicatorReady(PlayerIndicatorSpriteTask *arg0) {
    PlayerIndicatorPlayer *player;

    player = arg0->unk24;
    if (player->pathFlags != 0) {
        arg0->unk38 = 6;
        setCallback(updateRacePlayerIndicatorSprite);
    }
}

void updateRacePlayerIndicatorSprite(PlayerIndicatorSpriteTask *arg0) {
    PlayerIndicatorPlayer *temp;
    loadAssetMetadataByIndex_arg *temp_s0;
    s32 temp_v0;

    temp = arg0->unk24;
    if (temp->pathFlags == 0) {
        temp_v0 = arg0->unk38 - 1;
        arg0->unk38 = temp_v0;
        if (temp_v0 == 0) {
            setCallback(awaitPlayerIndicatorReady);
            return;
        }
    } else {
        arg0->unk38 = 6;
    }
    transformVector((s16 *)&gIndicatorSpriteOffset, &arg0->unk24->unk9F0, &arg0->unk4.unk4);
    temp_s0 = &arg0->unk4;
    if (arg0->unk24->pathFlags >= 2 && (gFrameCounter & 1)) {
        loadAssetMetadataByIndex(temp_s0, arg0->unk0, 0x60, 0x14);
    } else {
        temp_s0++;
        temp_s0--;
        loadAssetMetadataByIndex(temp_s0, arg0->unk0, 0x60, 0x13);
    }
    enqueueTexturedBillboardSprite(arg0->unk24->unkBB8, (TexturedBillboardSprite *)temp_s0);
}

void cleanupPlayerIndicator(PlayerIndicatorTask *arg0) {
    arg0->assetTable = freeNodeMemory(arg0->assetTable);
}

void spawnPlayerIndicatorTask(void *cleanupArg) {
    Node *task = scheduleTask(&initPlayerIndicator, 0U, 0U, 0xF0U);
    if (task != NULL) {
        task->cleanupCallback = cleanupArg;
    }
}

void initStartGate(StartGate *gate) {
    s32 doorOffsetMatrix[8];
    Vec3i trackEndPos;
    s32 *transformMatrix;
    GameState *gameState;
    LevelConfig *levelConfig;
    u16 trackAngle;

    gameState = (GameState *)getCurrentAllocation();
    levelConfig = getLevelConfig(gameState->memoryPoolId);
    gate->mainGateDisplayLists = (DisplayLists *)((u8 *)getSkyDisplayLists3ByIndex(gameState->memoryPoolId) + 0x50);
    gate->mainGateSegment1 = loadUncompressedAssetByIndex(gameState->memoryPoolId);
    gate->mainGateSegment2 = loadCompressedSegment2AssetByIndex(gameState->memoryPoolId);
    gate->mainGateSegment3 = 0;
    trackAngle = getTrackEndInfo(&gameState->gameData, &trackEndPos);
    createYRotationMatrix(&gate->rotationMatrix, (trackAngle + levelConfig->yawOffset) & 0xFFFF);
    rotateVectorY(&D_800907EC_913EC, trackAngle + levelConfig->yawOffset, &gate->rotationMatrix.translation);
    gate->rotationMatrix.translation.x = gate->rotationMatrix.translation.x + levelConfig->shortcutPosX;
    gate->rotationMatrix.translation.z = gate->rotationMatrix.translation.z + levelConfig->shortcutPosZ;
    gate->rotationMatrix.translation.y = trackEndPos.y;
    gate->leftDoorDisplayLists = (DisplayLists *)((u8 *)getSkyDisplayLists3ByIndex(gameState->memoryPoolId) + 0x60);
    transformMatrix = doorOffsetMatrix;
    gate->leftDoorSegment1 = gate->mainGateSegment1;
    gate->leftDoorSegment2 = gate->mainGateSegment2;
    gate->leftDoorSegment3 = gate->mainGateSegment3;
    memcpy(transformMatrix, &identityMatrix, 0x20);
    transformMatrix[6] = 0x180000;
    func_8006B084_6BC84((Transform3D *)transformMatrix, (Transform3D *)gate, &gate->leftDoorTransform);
    gate->rightDoorSegment1 = gate->mainGateSegment1;
    gate->rightDoorSegment2 = gate->mainGateSegment2;
    gate->rightDoorSegment3 = gate->mainGateSegment3;
    transformMatrix[6] = 0x160000;
    transformMatrix[7] = 0xA3333;
    func_8006B084_6BC84((Transform3D *)transformMatrix, (Transform3D *)gate, &gate->rightDoorTransform);
    gate->gateRotation = 0;
    gate->animationState = 0;
    setCleanupCallback(cleanupStartGate);
    setCallback(updateStartGate);
}

void updateStartGate(StartGate *gate) {
    Transform3D localRotation;
    Transform3D *doorTransform;
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
            createZRotationMatrix(&localRotation, gate->gateRotation);
            localRotation.translation.x = 0;
            localRotation.translation.y = 0xC0000;
            localRotation.translation.z = 0;
            func_8006B084_6BC84(&localRotation, (Transform3D *)gate, &gate->leftDoorTransform);
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
            doorTransform = &gate->leftDoorTransform;
            createZRotationMatrix(doorTransform, gate->gateRotation);
            createZRotationMatrix(&localRotation, gate->gateRotation);
            localRotation.translation.x = 0;
            localRotation.translation.y = 0xC0000;
            localRotation.translation.z = 0;
            func_8006B084_6BC84(&localRotation, (Transform3D *)gate, doorTransform);
            break;
        case 4:
            if (gameState->shortcutGateState != 3) {
                gate->animationState = 0;
            }
            break;
    }

    if (gameState->shortcutGateState == 3) {
        gate->rightDoorDisplayLists = (DisplayLists *)((s32)getSkyDisplayLists3ByIndex(gameState->memoryPoolId) + 0x70);
    } else {
    block_else:
        gate->rightDoorDisplayLists = (DisplayLists *)((s32)getSkyDisplayLists3ByIndex(gameState->memoryPoolId) + 0x80);
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)gate);
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)&gate->leftDoorTransform);
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)&gate->rightDoorTransform);
    }
}

void cleanupStartGate(StartGate *gate) {
    gate->mainGateSegment1 = freeNodeMemory(gate->mainGateSegment1);
    gate->mainGateSegment2 = freeNodeMemory(gate->mainGateSegment2);
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

void updatePushStartText(PushStartPromptTask *task) {
    if (gFrameCounter & 8) {
        enqueueTextRender(
            -0x44,
            task->yPosition,
            0,
            (u8 *)&pushStartButtonText,
            (s32)task->scaleX.asS16,
            (s32)task->scaleY.asS16
        );
    }
}

void updatePushStartGraphic(PushStartPromptTask *task) {
    enqueueHudTextLayout(
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

    renderTintedSpriteGrid(
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
    s32 i;
    s32 gravity;
    loadAssetMetadata_arg *extPtr;

    (void)pad;
    i = 0;
    if (task->particleCount > 0) {
        gravity = 0x40000;
        extPtr = (loadAssetMetadata_arg *)&D_80090860_91460;
        do {
            task->particles[i].worldPos.x = (randA() & 0xFF) << 17;
            task->particles[i].worldPos.y = (randA() & 0xFF) << 17;
            task->particles[i].worldPos.z = (randA() & 0xFF) << 17;
            task->particles[i].velocity.x = gravity;
            task->particles[i].velocity.y = -((randA() & 0xFF) << 8) - gravity;
            task->particles[i].velocity.z = 0;
            loadAssetMetadata(&task->particles[i].sprite, task->particleAsset, 0);
            task->particles[i].sprite.assetTemplate = extPtr;
            i++;
        } while (i < task->particleCount);
    }
    memcpy(&task->lastCameraPos, &task->cameraNode->modelingMatrix.translation, sizeof(Vec3i));
    setCallbackWithContinue(&updateConfettiParticles);
}

void updateConfettiParticles(ConfettiEffectTask *task) {
    s32 pad[2];
    s32 i;
    s32 running;
    s32 mask;
    s32 cameraOffset;

    (void)pad;
    running = 1;
    i = 0;
    if (task->particleCount > 0) {
        mask = 0x1FFFFFF;
        cameraOffset = 0xFF000000;
        do {
            if (task->pauseWhenPaused != 0) {
                GameState *gameState = (GameState *)getCurrentAllocation();
                running &= -(gameState->gamePaused == 0);
            }
            if (running != 0) {
                task->particles[i].worldPos.x += task->particles[i].velocity.x;
                task->particles[i].worldPos.y += task->particles[i].velocity.y;
                if (task->particles[i].worldPos.y < 0) {
                    task->particles[i].worldPos.x = (randA() & 0xFF) << 17;
                    task->particles[i].worldPos.y = 0x02000000;
                    task->particles[i].worldPos.z = (randA() & 0xFF) << 17;
                    task->particles[i].velocity.y = 0xFFFC0000 - ((randA() & 0xFF) << 8);
                }
            }
            task->particles[i].worldPos.x -= task->cameraNode->modelingMatrix.translation.x - task->lastCameraPos.x;
            task->particles[i].worldPos.x &= mask;
            task->particles[i].worldPos.z -= task->cameraNode->modelingMatrix.translation.z - task->lastCameraPos.z;
            task->particles[i].worldPos.z &= mask;
            task->particles[i].sprite.position.x =
                task->particles[i].worldPos.x + (task->cameraNode->modelingMatrix.translation.x + cameraOffset);
            task->particles[i].sprite.position.y =
                task->particles[i].worldPos.y + (task->cameraNode->modelingMatrix.translation.y + cameraOffset);
            task->particles[i].sprite.position.z =
                task->particles[i].worldPos.z + (task->cameraNode->modelingMatrix.translation.z + cameraOffset);
            enqueueTexturedBillboardSprite(task->frameCounter, (TexturedBillboardSprite *)&task->particles[i]);
            i++;
        } while (i < task->particleCount);
    }
    memcpy(&task->lastCameraPos, &task->cameraNode->modelingMatrix.translation, sizeof(Vec3i));
}

void cleanupConfettiEffect(ConfettiEffectTask *task) {
    task->particleAsset = freeNodeMemory(task->particleAsset);
    task->particles = freeNodeMemory(task->particles);
}

void spawnConfettiEffectForAllPlayers(void) {
    GameState *gameState;
    ConfettiEffectTask *task;
    s32 i;

    gameState = (GameState *)getCurrentAllocation();
    for (i = 0; i < gameState->unk5D; i++) {
        task = (ConfettiEffectTask *)scheduleTask(&initConfettiEffect, 0, 0, 0xF0);
        if (task != NULL) {
            task->frameCounter = i;
            task->cameraNode = &gameState->unk8[(s16)i];
            switch (gameState->unk5D) {
                case 1:
                    task->particleCount = 0x64;
                    break;
                case 2:
                    task->particleCount = 0x32;
                    break;
                case 3:
                default:
                    task->particleCount = 0x19;
                    break;
            }
            task->pauseWhenPaused = 1;
        }
    }
}

void spawnConfettiEffect(ViewportNode *cameraNode) {
    ConfettiEffectTask *task;

    task = (ConfettiEffectTask *)scheduleTask(&initConfettiEffect, 0, 0, 0xF0);

    if (task != NULL) {
        task->frameCounter = 0;
        task->cameraNode = cameraNode;
        task->particleCount = 0x64;
        task->pauseWhenPaused = 0;
    }
}
