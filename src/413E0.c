#include "36B80.h"
#include "56910.h"
#include "5E590.h"
#include "68CF0.h"
#include "6DE50.h"
#include "audio.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
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

extern s32 gFrameCounter;

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

// Start gate display object structure (3 parts: main gate, left door, right door)
// Each part is a DisplayListObject with transform, display lists, and segment pointers
typedef struct {
    /* Main gate structure (offset 0x00) */
    Transform3D rotationMatrix;         // 0x00-0x1F, with position at 0x14-0x1F
    DisplayLists *mainGateDisplayLists; // 0x20
    void *mainGateSegment1;             // 0x24 - uncompressed asset
    void *mainGateSegment2;             // 0x28 - compressed asset
    s32 mainGateSegment3;               // 0x2C
    u8 pad[0xC];
    /* Left door structure (offset 0x3C) */
    Transform3D leftDoorTransform;      // 0x3C
    DisplayLists *leftDoorDisplayLists; // 0x5C
    void *leftDoorSegment1;             // 0x60
    void *leftDoorSegment2;             // 0x64
    s32 leftDoorSegment3;               // 0x68
    u8 pad2[0xC];
    /* Right door structure (offset 0x78) */
    Transform3D rightDoorTransform;      // 0x78
    DisplayLists *rightDoorDisplayLists; // 0x98
    void *rightDoorSegment1;             // 0x9C
    void *rightDoorSegment2;             // 0xA0
    s32 rightDoorSegment3;               // 0xA4
    u8 pad3[0xC];
    s16 gateRotation;   // 0xB4
    s16 animationState; // 0xB6
    s16 pauseTimer;     // 0xB8
} StartGate;

void updateStartGate(StartGate *);
void updatePushStartText(PushStartTextState *);
void updateRacePlayerIndicatorSprite(PlayerIndicatorSpriteTask *arg0);

typedef struct {
    void *unk0;
} func_80040948_41548_arg;

typedef struct {
    u8 _pad[0x24];
    void *uncompressedAsset; // 0x24 - same as mainGateSegment1
    void *compressedAsset;   // 0x28 - same as mainGateSegment2
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
void awaitPlayerIndicatorReady(PlayerIndicatorSpriteTask *arg0);
void cleanupPlayerIndicator(func_80040948_41548_arg *arg0);

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
    trackAngle = getTrackEndInfo((u8 *)gameState + 0x30, &trackEndPos);
    createYRotationMatrix(&gate->rotationMatrix, (trackAngle + levelConfig->yawOffset) & 0xFFFF);
    rotateVectorY(&D_800907EC_913EC, trackAngle + levelConfig->yawOffset, &gate->rotationMatrix.translation);
    gate->rotationMatrix.translation.x = gate->rotationMatrix.translation.x + levelConfig->shortcutPosX;
    gate->rotationMatrix.translation.z = gate->rotationMatrix.translation.z + levelConfig->shortcutPosZ;
    gate->rotationMatrix.translation.y = trackEndPos.y;
    gate->leftDoorDisplayLists = (DisplayLists *)((u8 *)getSkyDisplayLists3ByIndex(gameState->memoryPoolId) + 0x60);
    transformMatrix = tempMatrix;
    gate->leftDoorSegment1 = gate->mainGateSegment1;
    gate->leftDoorSegment2 = gate->mainGateSegment2;
    gate->leftDoorSegment3 = gate->mainGateSegment3;
    memcpy(transformMatrix, &identityMatrix, 0x20);
    transformMatrix[6] = 0x180000;
    func_8006B084_6BC84(transformMatrix, gate, &gate->leftDoorTransform);
    gate->rightDoorSegment1 = gate->mainGateSegment1;
    gate->rightDoorSegment2 = gate->mainGateSegment2;
    gate->rightDoorSegment3 = gate->mainGateSegment3;
    transformMatrix[6] = 0x160000;
    transformMatrix[7] = 0xA3333;
    func_8006B084_6BC84(transformMatrix, gate, &gate->rightDoorTransform);
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
            func_8006B084_6BC84(&sp10, gate, &gate->leftDoorTransform);
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
            s0 = &gate->leftDoorTransform;
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

void cleanupStartGate(StartGateCleanupArg *arg) {
    arg->uncompressedAsset = freeNodeMemory(arg->uncompressedAsset);
    arg->compressedAsset = freeNodeMemory(arg->compressedAsset);
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
void updateConfettiParticles(ConfettiEffectTask *task);
void cleanupConfettiEffect(ConfettiEffectTask *task);

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
                /* Initialize particle physics state */
                *(s32 *)(offset + (s32)task->particles + 0x20) = (randA() & 0xFF) << 17;             /* worldX */
                *(s32 *)(offset + (s32)task->particles + 0x24) = (randA() & 0xFF) << 17;             /* worldY */
                *(s32 *)(offset + (s32)task->particles + 0x28) = (randA() & 0xFF) << 17;             /* worldZ */
                *(s32 *)(offset + (s32)task->particles + 0x2C) = gravity;                            /* velX */
                *(s32 *)(offset + (s32)task->particles + 0x30) = -((randA() & 0xFF) << 8) - gravity; /* velY */
                *(s32 *)(offset + (s32)task->particles + 0x34) = 0;                                  /* velZ */
                loadAssetMetadata((loadAssetMetadata_arg *)(task->particles + offset), task->particleAsset, 0);
                *(void **)(offset + (s32)task->particles) = extPtr; /* vertices/asset ptr */
                offset += 0x38;
            } while (i < task->particleCount);
        }
    } while (0);
    memcpy(&task->lastCameraX, (u8 *)task->cameraNode + 0x134, 0xC);
    setCallbackWithContinue(&updateConfettiParticles);
}

typedef struct {
    /* 0x00 */ loadAssetMetadata_arg sprite;
    /* 0x1C */ s32 pad1C;
    /* 0x20 */ s32 worldX; /* World-space X position (for physics) */
    /* 0x24 */ s32 worldY; /* World-space Y position (for physics) */
    /* 0x28 */ s32 worldZ; /* World-space Z position (for physics) */
    /* 0x2C */ s32 velX;   /* X velocity */
    /* 0x30 */ s32 velY;   /* Y velocity (includes gravity) */
    /* 0x34 */ s32 velZ;   /* Z velocity */
} ConfettiParticle;        /* 0x38 bytes */

void updateConfettiParticles(ConfettiEffectTask *task) {
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
            /* ConfettiParticle struct fields:
             * worldX - physics position X (offset 0x20)
             * worldY - physics position Y (offset 0x24)
             * worldZ - physics position Z (offset 0x28)
             * velX   - velocity X (offset 0x2C)
             * velY   - velocity Y (offset 0x30)
             * velZ   - velocity Z (offset 0x34)
             */
            if (task->pauseWhenPaused != 0) {
                GameState *gameState = (GameState *)getCurrentAllocation();
                running &= -(gameState->gamePaused == 0);
            }
            if (running != 0) {
                /* Update particle physics position */
                {
                    s32 *p = (s32 *)(offset + (s32)task->particles);
                    p[8] += p[0xB]; /* worldX += velX */
                }
                {
                    s32 *p = (s32 *)(offset + (s32)task->particles);
                    p[9] += p[0xC]; /* worldY += velY */
                }
                /* Respawn particle if it falls below the world */
                if (*(s32 *)(offset + (s32)task->particles + 0x24) < 0) {
                    *(s32 *)(offset + (s32)task->particles + 0x20) = (randA() & 0xFF) << 17;
                    *(s32 *)(offset + (s32)task->particles + 0x24) = 0x02000000;
                    *(s32 *)(offset + (s32)task->particles + 0x28) = (randA() & 0xFF) << 17;
                    *(s32 *)(offset + (s32)task->particles + 0x30) = 0xFFFC0000 - ((randA() & 0xFF) << 8);
                }
            }
            /* Adjust world position relative to camera movement */
            {
                ConfettiParticle *p = (ConfettiParticle *)(offset + (s32)task->particles);
                p->worldX -= *(s32 *)((u8 *)task->cameraNode + 0x134) - task->lastCameraX;
            }
            {
                ConfettiParticle *p = (ConfettiParticle *)(offset + (s32)task->particles);
                p->worldX &= mask;
            }
            {
                ConfettiParticle *p = (ConfettiParticle *)(offset + (s32)task->particles);
                p->worldZ -= *(s32 *)((u8 *)task->cameraNode + 0x13C) - task->lastCameraZ;
            }
            {
                ConfettiParticle *p = (ConfettiParticle *)(offset + (s32)task->particles);
                p->worldZ &= mask;
            }
            /* Update render position based on world position and camera */
            {
                ConfettiParticle *p = (ConfettiParticle *)(offset + (s32)task->particles);
                s32 camX = *(s32 *)((u8 *)task->cameraNode + 0x134) + cameraOffset;
                p->sprite.position.x = p->worldX + camX;
            }
            {
                ConfettiParticle *p = (ConfettiParticle *)(offset + (s32)task->particles);
                s32 camY = *(s32 *)((u8 *)task->cameraNode + 0x138) + cameraOffset;
                p->sprite.position.y = p->worldY + camY;
            }
            {
                ConfettiParticle *p = (ConfettiParticle *)(offset + (s32)task->particles);
                s32 camZ = *(s32 *)((u8 *)task->cameraNode + 0x13C) + cameraOffset;
                p->sprite.position.z = p->worldZ + camZ;
            }
            enqueueTexturedBillboardSprite(task->frameCounter, (void *)((s32)task->particles + offset));
            i++;
            offset += sizeof(ConfettiParticle);
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
