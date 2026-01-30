#include "42170.h"
#include "19E80.h"
#include "56910.h"
#include "5AA90.h"
#include "5E590.h"
#include "audio.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
#include "race_session.h"
#include "rom_loader.h"
#include "task_scheduler.h"

USE_ASSET(_3F3EF0);

extern void *D_8009A6B0_9B2B0;
extern void *D_8009A6C0_9B2C0;
extern void *D_8009A6D0_9B2D0;
extern void *D_8009A6E0_9B2E0;
extern void *D_8009A6F0_9B2F0;
extern void *D_8009A700_9B300;
extern void *D_8009A710_9B310;
extern void *D_8009A720_9B320;
extern void *D_8009A760_9B360;
extern void *D_8009A770_9B370;
extern Vec3i D_8009A8A4_9B4A4;
extern Transform3D D_8009A8B0_9B4B0;
extern s32 gFrameCounter;

void cleanupStarEffect(void **);

typedef struct {
    u8 _pad0[0x9F0];
    s16 unk9F0[3];
    u8 _pad9F6[0x1C5];
    u8 unkBBB;
    u8 _padBBC[0x13];
    u8 unkBCF;
    u8 _padBD0[0x9];
    u8 unkBD9;
} Func43CA4Unk28;

typedef struct {
    void *assetData;              /* Pointer to asset table */
    loadAssetMetadata_arg sprite; /* Sprite metadata for rendering */
    u8 padding[0x2];
    Func43CA4Unk28 *unk24;  /* Unknown pointer at offset 0x24 */
    Func43CA4Unk28 *player; /* Target player for the star effect */
    s32 offsetX;            /* X offset from player position */
    s32 offsetY;            /* Y offset from player position */
    s32 offsetZ;            /* Z offset from player position */
    s16 animFrameIndex;     /* Current animation frame index */
    s16 startDelay;         /* Timer for initial delay and animation frames */
    s16 displayTimer;       /* How long to display the orbiting star */
    s16 alphaPulseDir;      /* Direction of alpha pulse (0=increasing, 1=decreasing) */
    s16 rotationAngle;      /* Unused in star effect, present for struct compatibility */
    u8 playSoundFlag;       /* Flag to play star spawn sound */
    u8 immediateMode;       /* Non-zero to skip expand animation and go straight to orbit */
} StarEffectState;

typedef struct {
    /* 0x00  */ u8 _pad0[0x1F0];
    /* 0x1F0 */ s32 pos1F0;
    /* 0x1F4 */ s32 pos1F4;
    /* 0x1F8 */ s32 pos1F8;
    /* 0x1FC */ u8 _pad1FC[0x98C];
    /* 0xB88 */ s32 unkB88;
} FallingEffectPlayer;

typedef struct {
    /* 0x00 */ s16 matrix[3][3];
    /* 0x12 */ u8 pad12[0x2];
    /* 0x14 */ Vec3i position;
    /* 0x20 */ void *displayData;
    /* 0x24 */ void *asset1;
    /* 0x28 */ void *asset2;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u8 pad30[0xC];
    /* 0x3C */ FallingEffectPlayer *player;
    /* 0x40 */ s32 height;
    /* 0x44 */ s32 playSound;
} FallingEffectState;

typedef struct {
    s32 unk0;
    s32 displayList1;
    s32 displayList2;
    s32 displayList3;
} ShieldEffectDisplayConfig;

typedef struct {
    u8 matrix[0x20]; /* 0x00 - matrix data copied via memcpy */
    ShieldEffectDisplayConfig *displayConfig;
    void *displayAsset1;
    void *displayAsset2;
    s32 unk2C;
    s32 unk30;     /* 0x30 */
    u8 pad34[0x8]; /* 0x34 */
    Player *player;
    union {
        s32 full;
        struct {
            s16 hi;
            s16 lo;
        } half;
    } scale; /* 0x40 */
} ShieldEffectState;

typedef struct {
    DisplayListObject displayList;
    Player *player;
} ShieldEffectRenderState;

struct Func42D54Arg {
    u8 _pad0[0x4];
    loadAssetMetadata_arg unk4;
    void *unk20;
    Func43CA4Unk28 *unk24;
    Func43CA4Unk28 *unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s16 unk3C;
    u8 _pad3E[0x4];
    u8 unk42;
};

typedef struct {
    u8 padding[0x76];
    u8 paused;
} updateBurstParticles_alloc;

void cleanupShieldEffect(ShieldEffectState *);
void renderShieldLayer1(ShieldEffectRenderState *);
void renderShieldLayer2(ShieldEffectRenderState *);
void renderShieldLayer3(ShieldEffectRenderState *);

extern s32 identityMatrix[];

typedef struct {
    u8 _pad0[0x4];
    loadAssetMetadata_arg sprite;
    u8 _pad20[0x4];
    Func43CA4Unk28 *unk24;
    u8 _pad28[0x4];
    s16 unk2C[2];
    s32 scale;
    u8 _pad34[0xE];
    u8 playSoundFlag;
} ExpandStarEffectState;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ Vec3i position;
    /* 0x20 */ void *displayData;
    /* 0x24 */ void *asset1;
    /* 0x28 */ void *asset2;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u8 pad30[0xC];
    /* 0x3C */ Player *player;
    /* 0x40 */ s32 velocityY;
    /* 0x44 */ s16 rotation;
    /* 0x46 */ s16 scale;
} GhostEffectState;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ Vec3i position;
    /* 0x20 */ void *displayData;
    /* 0x24 */ void *asset1;
    /* 0x28 */ void *asset2;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u8 pad30[0x0C];
    /* 0x3C */ Player *player;
    /* 0x40 */ s16 rotationMatrix[3][3];
    /* 0x52 */ u8 pad52[2];
    /* 0x54 */ s32 unk54;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 playSound;
} CrashEffectState;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ Vec3i position;
    /* 0x20 */ void *displayData;
    /* 0x24 */ void *asset1;
    /* 0x28 */ void *asset2;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u8 pad30[0xC];
    /* 0x3C */ Player *player;
    /* 0x40 */ s16 unk40;
    /* 0x42 */ s16 scaleFactor;
    /* 0x44 */ u16 rotationAngle;
    /* 0x46 */ s16 playSound;
} LiftEffectState;

void cleanupFallingEffect(FallingEffectState *);

typedef struct {
    u8 _pad0[0x30];
    u8 unk30[0x14];   /* 0x30 */
    void *spriteData; /* 0x44 - sprite/asset data buffer */
    u8 _pad48[0x14];
    u8 unk5C; /* 0x5C - course/level index */
    u8 _pad5D[0x19];
    u8 paused; /* 0x76 - non-zero when paused */
} EffectTaskState;

typedef struct {
    u8 _pad0[0xB44];
    /* 0xB44 */ u8 storedPos[0xC];
    /* 0xB50 */ u8 _padB50[0x24];
    /* 0xB74 */ s16 storedRotY;
    /* 0xB76 */ u8 _padB76[0x58];
    /* 0xBCE */ u8 flags;
} UfoAbductionPlayerState;

typedef struct {
    /* 0x00 */ u8 _pad0[0x14];
    /* 0x14 */ Vec3i unk14;
    /* 0x20 */ void *unk20;
    /* 0x24 */ void *unk24;
    /* 0x28 */ void *unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u8 _pad30[0xC];
    /* 0x3C */ DisplayListObject leftWing;
    /* 0x78 */ DisplayListObject rightWing;
    /* 0xB4 */ UfoAbductionPlayerState *target;
    /* 0xB8 */ s32 movementOffset;
    /* 0xBC */ s32 fallVelocity;
    /* 0xC0 */ s32 flyAwayDistance;
    /* 0xC4 */ s16 phaseTimer;
    /* 0xC6 */ s16 yRotation;
    /* 0xC8 */ u16 baseRotation;
    /* 0xCA */ u16 wingOscillationAngle;
} UfoEffectState;

typedef struct {
    s32 unk0;
    s32 unk4;
    /* 0x08 */ Vec3i unk8;
    /* 0x14 */ u8 _pad14[0x14];
    /* 0x28 */ Func43CA4Unk28 *unk28;
    /* 0x2C */ s16 unk2C[3];
    /* 0x32 */ u8 _pad32[0xE];
    /* 0x40 */ s16 animFrameIndex;
    /* 0x42 */ s16 frameTimer;
} Func43CA4Arg;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ void *unk4;
    /* 0x08 */ Vec3i position;
    /* 0x14 */ u8 _pad14[0x10];
    /* 0x24 */ Func43CA4Unk28 *recipientPlayer;
    /* 0x28 */ Func43CA4Unk28 *victimPlayer;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 stolenGold;
    /* 0x40 */ s16 animFrameIndex;
    /* 0x42 */ s16 frameTimer;
    /* 0x44 */ u8 _pad44[0x2];
    /* 0x46 */ s16 unk46;
    /* 0x48 */ s16 swingAngle;
    /* 0x4A */ u8 _pad4A[0x2];
    /* 0x4C */ s32 unk4C;
} GoldStealEffectState;

typedef struct {
    void *unk0;
    loadAssetMetadata_arg_base sprite;
    u8 _pad1C[0x2];
    u8 animFrameIndex;
    u8 _pad1F[0x1];
    void *unk20;
    void *unk24;
    Func43CA4Unk28 *player;
    s16 orbitOffset[3];
    u8 _pad32[0xA];
    s16 displayTimer;
    u8 _pad3E[0x2];
    u16 rotationAngle;
    u8 playSoundFlag;
} OrbitStarEffectState;

void renderUfoEffectWithWings(UfoEffectState *);
void initUfoEffect(UfoEffectState *);
void flyInUfoEffect(UfoEffectState *);
void descendUfoEffect(UfoEffectState *);
void setupUfoFlyAway(UfoEffectState *);
void flyAwayUfoEffect(UfoEffectState *);
void holdUfoEffect(UfoEffectState *);
void fadeOutUfoEffect(UfoEffectState *);
void cleanupUfoEffect(void *);
void cleanupGhostEffect(GhostEffectState *);
void updateGhostEffect(GhostEffectState *);
void fadeOutGhostEffect(GhostEffectState *);
void contractStarEffect(StarEffectState *);
void updateStarEffect(StarEffectState *);
void expandStarEffect(ExpandStarEffectState *);
void orbitStarEffect(OrbitStarEffectState *);
void updateFallingEffect(FallingEffectState *);
void animateFallingEffectDescent(FallingEffectState *);
void cleanupSparkleEffect(SparkleEffectState *);
void updateSparkleEffect(SparkleEffectState *);
void fadeOutSparkleEffect(SparkleEffectState *);

extern s8 starAnimFrameDurations[];
extern s8 starAnimFrameIndices[];
extern s32 D_80090964_91564;
extern s32 D_80090974_91574;
extern s32 D_8009093C_9153C;
extern s8 D_80090958_91558;
extern s8 D_8009095C_9155C;
extern s32 D_80090AA0_916A0;
extern s32 D_80090AAC_916AC;
extern s8 D_80090950_91550;

void initFallingEffect(FallingEffectState *arg0) {
    getCurrentAllocation();
    createXRotationMatrix(arg0->matrix, 0x800);
    arg0->displayData = &D_8009A6B0_9B2B0;
    arg0->asset1 = loadAsset_B7E70();
    arg0->asset2 = loadAsset_216290();
    arg0->height = 0x2000;
    arg0->unk2C = 0;
    arg0->playSound = 1;
    setCleanupCallback(cleanupFallingEffect);
    setCallbackWithContinue(updateFallingEffect);
}

void updateFallingEffect(FallingEffectState *arg0) {
    s32 i;

    arg0->position.x = arg0->player->pos1F0;
    arg0->position.y = arg0->player->pos1F4;
    arg0->position.z = arg0->player->pos1F8;

    if ((arg0->player->unkB88 & 0x20) == 0) {
        setCallback(animateFallingEffectDescent);
    }

    if (arg0->playSound != 0) {
        arg0->playSound = 0;
        queueSoundAtPosition(&arg0->position, 0xE);
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void animateFallingEffectDescent(FallingEffectState *arg0) {
    EffectTaskState *allocation;
    s32 i;

    allocation = (EffectTaskState *)getCurrentAllocation();
    if (allocation->paused == 0) {
        arg0->height -= 0x200;
    }

    if (arg0->height == 0x200) {
        terminateCurrentTask();
    }

    arg0->matrix[2][1] = -arg0->height;
    arg0->position.y += 0xFFFF0000;

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void cleanupFallingEffect(FallingEffectState *arg0) {
    arg0->asset1 = freeNodeMemory(arg0->asset1);
    arg0->asset2 = freeNodeMemory(arg0->asset2);
}

typedef struct {
    u8 _pad0[0x3C];
    void *player;
} FallingEffectTask;

void *createFallingEffect(void *arg0) {
    FallingEffectTask *task;

    task = (FallingEffectTask *)scheduleTask(&initFallingEffect, 0, 0, 0xC8);
    if (task != NULL) {
        task->player = arg0;
    }
    return task;
}

void updateShieldEffect(ShieldEffectState *);
void spawnShieldBurstEffect(Vec3i *);

void initShieldEffect(ShieldEffectState *arg0) {
    getCurrentAllocation();
    arg0->displayConfig = (ShieldEffectDisplayConfig *)&D_8009A6C0_9B2C0;
    arg0->displayAsset1 = loadAsset_B7E70();
    arg0->displayAsset2 = loadAsset_216290();
    arg0->unk2C = 0;
    arg0->scale.full = 0x400;
    queueSoundAtPosition(&arg0->player->worldPos, 0x13);
    setCleanupCallback(cleanupShieldEffect);
    setCallbackWithContinue(updateShieldEffect);
}

void updateShieldEffect(ShieldEffectState *arg0) {
    Vec3i effectPos;
    s32 scale;
    s16 scaleFactor;
    s32 i;

    if (!(arg0->player->unkB88 & 0x40)) {
        effectPos.x = arg0->player->worldPos.x;
        effectPos.y = arg0->player->worldPos.y + 0x100000;
        effectPos.z = arg0->player->worldPos.z;
        spawnShieldBurstEffect(&effectPos);
        terminateCurrentTask();
        return;
    }

    scale = arg0->scale.full;
    if (scale != 0x2000) {
        arg0->scale.full = scale + 0x400;
        memcpy(arg0, identityMatrix, 0x20);
        scaleFactor = arg0->scale.half.lo;
        scaleMatrix((Transform3D *)arg0, scaleFactor, scaleFactor, scaleFactor);
    }

    effectPos.x = arg0->player->worldPos.x;
    effectPos.y = arg0->player->worldPos.y;
    effectPos.z = arg0->player->worldPos.z;
    effectPos.y = effectPos.y + 0xFFFE0000;

    if (checkPositionPlayerCollisionWithKnockback(&effectPos, 0x180000, 0x320000, arg0->player->playerIndex) != 0) {
        arg0->player->ufoFlags |= 1;
    }
    arg0->unk30 = 0;

    for (i = 0; i < 4; i++) {
        if (arg0->displayConfig->displayList1 != 0) {
            debugEnqueueCallback((u16)i, 1, renderShieldLayer1, arg0);
        }
        if (arg0->displayConfig->displayList2 != 0) {
            debugEnqueueCallback((u16)i, 3, renderShieldLayer2, arg0);
        }
        if (arg0->displayConfig->displayList3 != 0) {
            debugEnqueueCallback((u16)i, 5, renderShieldLayer3, arg0);
        }
    }
}

void cleanupShieldEffect(ShieldEffectState *arg0) {
    arg0->displayAsset1 = freeNodeMemory(arg0->displayAsset1);
    arg0->displayAsset2 = freeNodeMemory(arg0->displayAsset2);
}

void scheduleShieldEffect(void *arg0) {
    ShieldEffectState *task;

    task = scheduleTask(&initShieldEffect, 0, 0, 0x63);
    if (task != NULL) {
        task->player = arg0;
    }
}

void renderShieldLayer1(ShieldEffectRenderState *arg0) {
    arg0->displayList.transform.translation.x = arg0->player->worldPos.x;
    arg0->displayList.transform.translation.y = arg0->player->worldPos.y;
    arg0->displayList.transform.translation.z = arg0->player->worldPos.z;
    renderOpaqueDisplayListCallback(&arg0->displayList);
}

void renderShieldLayer2(ShieldEffectRenderState *arg0) {
    arg0->displayList.transform.translation.x = arg0->player->worldPos.x;
    arg0->displayList.transform.translation.y = arg0->player->worldPos.y;
    arg0->displayList.transform.translation.z = arg0->player->worldPos.z;
    renderTransparentDisplayListCallback(&arg0->displayList);
}

void renderShieldLayer3(ShieldEffectRenderState *arg0) {
    arg0->displayList.transform.translation.x = arg0->player->worldPos.x;
    arg0->displayList.transform.translation.y = arg0->player->worldPos.y;
    arg0->displayList.transform.translation.z = arg0->player->worldPos.z;
    renderOverlayDisplayListCallback(&arg0->displayList);
}

typedef struct {
    loadAssetMetadata_arg *unk0; // 0x00
    Vec3i position;              // 0x04-0x0F
    u8 *data_ptr;                // 0x10
    void *index_ptr;             // 0x14
    u8 unk18;                    // 0x18
    u8 unk19;                    // 0x19
    u8 unk1A;                    // 0x1A
    u8 pad1B;                    // 0x1B
    u8 pad1C[0x4];               // 0x1C-0x1F
    s32 unk20;                   // 0x20-0x23
} BurstParticle;                 // size 0x24

typedef struct {
    BurstParticle particles[6]; // 0x00-0xD7
    void *assetTable;           // 0xD8
    s8 particleType;            // 0xDC
    u8 unkDD;                   // 0xDD
} BurstEffectState;

extern loadAssetMetadata_arg D_800908A0_914A0;
extern Vec3i D_800908E0_914E0[];

void updateBurstParticles(BurstEffectState *);
void setupBurstParticles(BurstEffectState *);
void cleanupBurstEffect(BurstEffectState *);

void initBurstEffect(BurstEffectState *state) {
    state->assetTable = load_3ECE40();
    setCleanupCallback(cleanupBurstEffect);
    setCallbackWithContinue(setupBurstParticles);
}

void setupBurstParticles(BurstEffectState *state) {
    s32 i;

    for (i = 0; i < 6; i++) {
        s32 temp;

        loadAssetMetadata((loadAssetMetadata_arg *)&state->particles[i], state->assetTable, state->particleType);
        state->particles[i].unk20 = D_800908E0_914E0[i].y;
        state->particles[i].position.x += D_800908E0_914E0[i].x;
        state->particles[i].position.y += D_800908E0_914E0[i].y;
        state->particles[i].position.z += D_800908E0_914E0[i].z;
        state->particles[i].unk0 = &D_800908A0_914A0;
    }

    if (state->particleType != 0x2F) {
        for (i = 0; i < 6; i++) {
            state->particles[i].unk1A = 0xC0;
        }
    }

    state->unkDD = 0;
    setCallbackWithContinue(updateBurstParticles);
}

void updateBurstParticles(BurstEffectState *arg0) {
    s32 i;
    s32 j;
    updateBurstParticles_alloc *alloc;

    alloc = getCurrentAllocation();

    if (alloc->paused == 0) {
        for (i = 0; i < 6; i++) {
            arg0->particles[i].position.x += D_800908E0_914E0[i].x;
            arg0->particles[i].position.y += arg0->particles[i].unk20;
            arg0->particles[i].position.z += D_800908E0_914E0[i].z;
            arg0->particles[i].unk20 += 0xFFFF0000;
        }

        if ((arg0->unkDD & 3) == 0) {
            loadAssetMetadata(
                (loadAssetMetadata_arg *)arg0,
                arg0->assetTable,
                arg0->particleType + ((s8)arg0->unkDD >> 2)
            );

            for (i = 1; i < 6; i++) {
                arg0->particles[i].data_ptr = arg0->particles[0].data_ptr;
                arg0->particles[i].index_ptr = arg0->particles[0].index_ptr;
                arg0->particles[i].unk18 = arg0->particles[0].unk18;
                arg0->particles[i].unk19 = arg0->particles[0].unk19;
            }
        }

        arg0->unkDD++;
        if ((s8)arg0->unkDD == 0x10) {
            terminateCurrentTask();
        }
    }

    if (arg0->particleType != 0x2F) {
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 6; j++) {
                enqueueAlphaBillboardSprite(i, (loadAssetMetadata_arg *)&arg0->particles[j]);
            }
        }
    } else {
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 6; j++) {
                enqueueTexturedBillboardSprite(i, (TexturedBillboardSprite *)&arg0->particles[j]);
            }
        }
    }
}

void cleanupBurstEffect(BurstEffectState *state) {
    state->assetTable = freeNodeMemory(state->assetTable);
}

void spawnShieldBurstEffect(Vec3i *position) {
    u8 *task;
    s32 i;
    u8 *ptr;

    task = (u8 *)scheduleTask(initBurstEffect, 2, 0, 0xE7);
    if (task != NULL) {
        i = 0;
        ptr = task;
        do {
            memcpy(ptr + 4, position, 0xC);
            i++;
            ptr += 0x24;
        } while (i < 6);
        task[0xDC] = 0x2B;
        queueSoundAtPosition(position, 0x16);
    }
}

void spawnBurstEffect(Vec3i *position) {
    u8 *task;
    s32 i;
    u8 *ptr;

    task = (u8 *)scheduleTask(initBurstEffect, 2, 0, 0xE7);
    if (task != NULL) {
        i = 0;
        ptr = task;
        do {
            memcpy(ptr + 4, position, 0xC);
            i++;
            ptr += 0x24;
        } while (i < 6);
        task[0xDC] = 0x2F;
        queueSoundAtPosition(position, 0xE);
    }
}

void updateCrashEffect(CrashEffectState *);
void cleanupCrashEffect(CrashEffectState *);

void initCrashEffect(CrashEffectState *arg0) {
    getCurrentAllocation();
    arg0->displayData = &D_8009A6D0_9B2D0;
    arg0->asset1 = loadAsset_B7E70();
    arg0->asset2 = loadAsset_216290();
    arg0->unk2C = 0;
    createYRotationMatrix((Transform3D *)arg0->rotationMatrix, 0xF800);
    arg0->unk54 = 0;
    arg0->unk58 = 0;
    arg0->unk5C = 0;
    arg0->playSound = 1;
    setCleanupCallback(cleanupCrashEffect);
    setCallbackWithContinue(updateCrashEffect);
}

void updateCrashEffect(CrashEffectState *arg0) {
    Vec3i pos;
    s32 i;

    func_8006B084_6BC84(arg0->rotationMatrix, &arg0->player->unk3F8, arg0);

    if ((arg0->player->unkB88 & 0x80) == 0) {
        pos.x = arg0->player->worldPos.x;
        pos.y = arg0->player->worldPos.y + 0x100000;
        pos.z = arg0->player->worldPos.z;
        spawnBurstEffect(&pos);
        terminateCurrentTask();
    }

    if (arg0->playSound != 0) {
        arg0->playSound = 0;
        queueSoundAtPosition(&arg0->position, 0x12);
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void cleanupCrashEffect(CrashEffectState *arg0) {
    arg0->asset1 = freeNodeMemory(arg0->asset1);
    arg0->asset2 = freeNodeMemory(arg0->asset2);
}

void *spawnCrashEffect(void *arg0) {
    CrashEffectState *task;

    task = scheduleTask(&initCrashEffect, 0, 0, 0xC8);
    if (task != NULL) {
        task->player = arg0;
    }
    return task;
}

void initSparkleEffect(SparkleEffectState *arg0) {
    getCurrentAllocation();
    arg0->displayData = &D_8009A6E0_9B2E0;
    arg0->asset1 = loadAsset_B7E70();
    arg0->asset2 = loadAsset_216290();
    arg0->unk2C = 0;
    arg0->scale = 0;
    arg0->rotation = 0;
    arg0->opacity = 0xFF;
    queueSoundAtPosition(&arg0->position, 0x14);
    setCleanupCallback(cleanupSparkleEffect);
    setCallbackWithContinue(updateSparkleEffect);
}

void updateSparkleEffect(SparkleEffectState *arg0) {
    EffectTaskState *gameState;
    s32 temp;
    s32 diff;
    s32 i;
    s32 pad[5];

    gameState = (EffectTaskState *)getCurrentAllocation();
    if (gameState->paused == 0) {
        temp = arg0->scale;
        diff = 0x4000 - temp;
        if (diff < 0) {
            diff += 3;
        }
        arg0->scale = temp + (diff >> 2);
    }

    if (arg0->scale == 0) {
        arg0->scale = arg0->scale + 0x1000;
    }

    arg0->rotation += 0x300;
    createYRotationMatrix((Transform3D *)arg0, arg0->rotation);

    temp = arg0->scale;
    scaleMatrix((Transform3D *)arg0, temp, temp, temp);

    checkStarHitCollisionWithVulnerablePlayers((Vec3i *)&arg0->position, arg0->playerIndex, arg0->scale * 0xF0);

    arg0->alpha = (u8)arg0->opacity;

    i = 0;
    do {
        enqueueDisplayListObjectWithSegments(i, (DisplayListObject *)arg0);
        i++;
    } while (i < 4);

    if (arg0->scale >= 0x3C00) {
        setCallback(fadeOutSparkleEffect);
    }
}

void fadeOutSparkleEffect(SparkleEffectState *arg0) {
    EffectTaskState *gameState;
    s32 i;
    s32 pad[3];

    gameState = (EffectTaskState *)getCurrentAllocation();
    if (gameState->paused == 0) {
        arg0->opacity -= 0x18;
    }

    if (arg0->opacity < 0x18) {
        terminateCurrentTask();
        return;
    }

    arg0->rotation += 0x300;
    createYRotationMatrix((Transform3D *)arg0, arg0->rotation);
    scaleMatrix((Transform3D *)arg0, arg0->scale, arg0->scale, arg0->scale);
    arg0->alpha = (u8)arg0->opacity;

    for (i = 0; i < 4; i++) {
        enqueueDisplayListObjectWithSegments(i, (DisplayListObject *)arg0);
    }
}

void cleanupSparkleEffect(SparkleEffectState *arg0) {
    arg0->asset1 = freeNodeMemory(arg0->asset1);
    arg0->asset2 = freeNodeMemory(arg0->asset2);
}

SparkleEffectState *spawnSparkleEffect(void *arg0) {
    SparkleEffectState *task;

    task = (SparkleEffectState *)scheduleTask(initSparkleEffect, 0, 0, 0xC8);
    if (task != NULL) {
        memcpy(&task->position, arg0, 0xC);
        task->playerIndex = -1;
    }
    return task;
}

SparkleEffectState *spawnSparkleEffectWithPlayer(void *arg0, s32 arg1) {
    SparkleEffectState *task;

    task = (SparkleEffectState *)scheduleTask(&initSparkleEffect, 0, 0, 0xC8);
    if (task != NULL) {
        memcpy(&task->position, arg0, 0xC);
        task->playerIndex = arg1;
    }
    return task;
}

void cleanupLiftEffect(LiftEffectState *);
void updateLiftEffect(LiftEffectState *);
void fadeOutLiftEffect(LiftEffectState *);

void initLiftEffect(LiftEffectState *state) {
    getCurrentAllocation();
    state->displayData = &D_8009A6F0_9B2F0;
    state->asset1 = loadAsset_B7E70();
    state->asset2 = loadAsset_216290();
    *(s32 *)&state->unk40 = 0x200;
    state->unk2C = 0;
    state->playSound = 1;
    setCleanupCallback(cleanupLiftEffect);
    setCallbackWithContinue(updateLiftEffect);
}

void updateLiftEffect(LiftEffectState *state) {
    EffectTaskState *gameState;
    Player *player;
    Vec3i *pos;
    s32 i;

    gameState = (EffectTaskState *)getCurrentAllocation();
    if (gameState->paused == 0) {
        if (*(s32 *)&state->unk40 != 0x2000) {
            *(s32 *)&state->unk40 += 0x200;
        }
    }

    state->rotationAngle += 0x300;
    createYRotationMatrix((Transform3D *)state, state->rotationAngle);
    scaleMatrix((Transform3D *)state, state->scaleFactor, state->scaleFactor, state->scaleFactor);
    player = state->player;
    pos = &state->position;
    memcpy(pos, &player->worldPos, 0xC);
    state->position.y += 0xFFEC0000;

    if (state->playSound != 0) {
        queueSoundAtPosition(pos, 0x15);
        state->playSound = 0;
    }

    if ((state->player->unkB88 & 0x400) == 0) {
        setCallback(fadeOutLiftEffect);
    }

    i = 0;
    do {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)state);
        i++;
    } while (i < 4);
}

void fadeOutLiftEffect(LiftEffectState *state) {
    EffectTaskState *gameState;
    s32 i;

    gameState = (EffectTaskState *)getCurrentAllocation();
    if (gameState->paused == 0) {
        *(s32 *)&state->unk40 -= 0x200;
    }

    if (*(s32 *)&state->unk40 == 0x200) {
        terminateCurrentTask();
    }

    state->rotationAngle += 0x12C;
    createYRotationMatrix((Transform3D *)state, state->rotationAngle);
    scaleMatrix((Transform3D *)state, state->scaleFactor, state->scaleFactor, state->scaleFactor);
    memcpy(&state->position, &state->player->worldPos, 0xC);
    state->position.y += 0xFFEC0000;

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)state);
    }
}

void cleanupLiftEffect(LiftEffectState *state) {
    state->asset1 = freeNodeMemory(state->asset1);
    state->asset2 = freeNodeMemory(state->asset2);
}

void *createLiftEffect(Player *player) {
    LiftEffectState *task;

    task = (LiftEffectState *)scheduleTask(initLiftEffect, 0, 0, 0xC8);
    if (task != NULL) {
        task->player = player;
    }
    return task;
}

typedef struct {
    u8 pad0[0x434];        /* 0x00 */
    s32 sourcePosition[3]; /* 0x434 - warp source position (12 bytes) */
} WarpEffectSource;

typedef struct {
    s16 matrix[3][3];         /* 0x00 (0x12 bytes: 9 * s16) */
    u8 pad12[0x2];            /* 0x12 */
    Vec3i position;           /* 0x14 */
    void *displayData;        /* 0x20 */
    void *asset1;             /* 0x24 */
    void *asset2;             /* 0x28 */
    s32 unk2C;                /* 0x2C */
    u8 pad30[0xC];            /* 0x30 */
    WarpEffectSource *source; /* 0x3C */
    Player *player;           /* 0x40 */
    s32 delayFrames;          /* 0x44 */
    s32 scale;                /* 0x48 */
    s32 height;               /* 0x4C */
    s32 velocity;             /* 0x50 */
} WarpEffectState;

typedef struct {
    u8 pad0[0x24];
    void *asset1;
    void *asset2;
} WarpEffectCleanupArg;

void cleanupWarpEffect(WarpEffectCleanupArg *);
void updateWarpEffect(WarpEffectState *);

void initWarpEffect(WarpEffectState *state) {
    if (state->delayFrames == 0) {
        getCurrentAllocation();
        state->displayData = &D_8009A700_9B300;
        state->asset1 = loadAsset_B7E70();
        state->asset2 = loadAsset_216290();
        state->scale = 0x400;
        state->unk2C = 0;
        state->height = 0x100000;
        state->velocity = 0;
        setCleanupCallback(cleanupWarpEffect);
        setCallbackWithContinue(updateWarpEffect);
    } else {
        state->delayFrames = state->delayFrames - 1;
    }
}

void finishWarpEffect(WarpEffectState *);
void descendWarpEffect(WarpEffectState *);

void updateWarpEffect(WarpEffectState *state) {
    GameState *gameState;
    s16 scale;
    s32 i;

    gameState = getCurrentAllocation();
    if (gameState->gamePaused == 0) {
        if (state->scale != 0x2000) {
            state->scale += 0x400;
        }

        state->velocity += 0x20000;
        state->height += state->velocity;

        if (state->height > 0x500000) {
            setCallback(descendWarpEffect);
        }
    }

    createXRotationMatrix(state->matrix, 0);
    scale = (s16)state->scale;
    scaleMatrix((Transform3D *)state, scale, scale, scale);
    memcpy(&state->position, state->source->sourcePosition, 0xC);
    state->position.y += state->height;

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)state);
    }
}

void descendWarpEffect(WarpEffectState *state) {
    GameState *gameState;
    s32 i;

    gameState = getCurrentAllocation();
    if (gameState->gamePaused == 0) {
        state->player->pathFlags |= 4;
        state->velocity += 0xFFFE0000;
        state->height += state->velocity;

        if (state->height == 0x220000) {
            setPlayerBouncedBackState(state->player);
            queueSoundAtPosition(&state->position, 0x19);
        }

        if (state->height <= 0x10000) {
            state->velocity = 0x50000;
            setCallback(finishWarpEffect);
        }
    }

    memcpy(&state->position, &state->player->worldPos, 0xC);
    state->position.y += state->height;

    i = 0;
    do {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)state);
        i++;
    } while (i < 4);
}

void finishWarpEffect(WarpEffectState *state) {
    GameState *gameState;
    s32 i;

    gameState = getCurrentAllocation();
    if (gameState->gamePaused == 0) {
        state->velocity = state->velocity + (-0x8000);
        state->height = state->height + state->velocity;

        if (state->height < (s32)0xFFF00000 && state->velocity < 0) {
            terminateCurrentTask();
        }

        state->position.x = state->position.x + 0x20000;
        state->position.y = state->position.y + state->velocity;
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)state);
    }
}

void cleanupWarpEffect(WarpEffectCleanupArg *arg0) {
    arg0->asset1 = freeNodeMemory(arg0->asset1);
    arg0->asset2 = freeNodeMemory(arg0->asset2);
}

WarpEffectState *createWarpEffect(WarpEffectSource *source, Player *player, s16 delayFrames) {
    WarpEffectState *task;

    task = (WarpEffectState *)scheduleTask(initWarpEffect, 0, 0, 0xC8);
    if (task != NULL) {
        task->source = source;
        task->player = player;
        task->delayFrames = delayFrames;
    }
    return task;
}

void updateStarEffectAnimation(StarEffectState *arg0) {
    unsigned int new_var;
    arg0->startDelay--; /* startDelay is reused as frameTimer after initialization */
    if ((arg0->startDelay << 0x10) == 0) {
        new_var = 2;
        loadAssetMetadata(&arg0->sprite, arg0->assetData, starAnimFrameIndices[arg0->animFrameIndex * new_var]);
        arg0->startDelay = starAnimFrameDurations[arg0->animFrameIndex * 2];
        arg0->animFrameIndex++;
        if (starAnimFrameDurations[arg0->animFrameIndex * new_var] == 0) {
            arg0->animFrameIndex = 0;
        }
    }
    if (arg0->alphaPulseDir != 0) {
        arg0->sprite.alpha -= 0x10;
        if (arg0->sprite.alpha == 0x40) {
            arg0->alphaPulseDir++;
        }
    } else {
        arg0->sprite.alpha += 0x10;
        if (arg0->sprite.alpha == 0xE0) {
            arg0->alphaPulseDir--;
        }
    }
}

void initStarEffect(void **arg0) {
    *arg0 = load_3ECE40();
    setCleanupCallback(cleanupStarEffect);
    setCallbackWithContinue(updateStarEffect);
}

void updateStarEffect(StarEffectState *arg0) {
    EffectTaskState *effectTask;
    s16 initialDelay;
    void *spriteDataTable;

    effectTask = (EffectTaskState *)getCurrentAllocation();
    initialDelay = arg0->startDelay;

    if (initialDelay == 0) {
        /* Initialize star effect animation state */
        arg0->startDelay = 1; /* Reuse as frame timer after initialization */
        arg0->alphaPulseDir = 0;
        arg0->sprite.alpha = 0;
        spriteDataTable = effectTask->spriteData;
        arg0->offsetY = 0x200000; /* Y offset above player */
        arg0->offsetX = 0;        /* X offset */
        arg0->offsetZ = 0;        /* Z offset */
        arg0->animFrameIndex = 0;
        arg0->playSoundFlag = 1;
        arg0->sprite.assetTemplate = (void *)((u8 *)spriteDataTable + 0xF00);
        updateStarEffectAnimation(arg0);

        if (arg0->immediateMode != 0) {
            /* Immediate orbit mode - skip expand animation */
            arg0->player->unkBCF++;

            if (arg0->player->unkBBB == 0xC) {
                arg0->displayTimer = 0x1E; /* Shorter display time for boss */
            } else {
                arg0->displayTimer = 0x12C; /* Normal display time */
            }

            arg0->offsetX = 0x140000; /* X offset for orbit */
            arg0->offsetY = 0x190000; /* Y offset for orbit */
            setCallbackWithContinue(orbitStarEffect);
        } else {
            setCallbackWithContinue(expandStarEffect);
        }
    } else {
        /* Wait for initial delay to expire */
        if (effectTask->paused == 0) {
            arg0->startDelay = initialDelay - 1;
        }
    }
}

void expandStarEffect(ExpandStarEffectState *arg0) {
    EffectTaskState *gameState;
    s32 i;

    gameState = (EffectTaskState *)getCurrentAllocation();
    if (gameState->paused == 0) {
        arg0->scale += 0x10000;
        if (arg0->scale > 0x2FFFFF) {
            setCallback(contractStarEffect);
        }

        transformVector(arg0->unk2C, arg0->unk24->unk9F0, &arg0->sprite.position);

        if (arg0->playSoundFlag != 0) {
            arg0->playSoundFlag = 0;
            queueSoundAtPosition(&arg0->sprite.position, 0x1A);
        }

        updateStarEffectAnimation((StarEffectState *)arg0);
    }

    for (i = 0; i < 4; i++) {
        enqueueAlphaBillboardSprite(i, &arg0->sprite);
    }
}

void contractStarEffect(StarEffectState *state) {
    EffectTaskState *gameState;
    s32 i;

    gameState = (EffectTaskState *)getCurrentAllocation();
    if (gameState->paused == 0) {
        updateStarEffectAnimation(state);
        transformVector((s16 *)&state->offsetX, state->unk24->unk9F0, &state->sprite.position);

        if (state->sprite.alpha == 0x40) {
            if (state->player->unkBD9 != 0) {
                state->offsetX = 0x300000;
                state->offsetY = 0x300000;
            } else {
                state->offsetX = 0x140000;
                state->offsetY = 0x190000;
            }

            if (state->player->unkBBB == 0xC) {
                state->displayTimer = 0x1E;
            } else {
                state->displayTimer = 0x12C;
            }

            state->player->unkBCF++;
            state->playSoundFlag = 1;
            setCallback(orbitStarEffect);
        }
    }

    for (i = 0; i < 4; i++) {
        enqueueAlphaBillboardSprite(i, &state->sprite);
    }
}

void orbitStarEffect(OrbitStarEffectState *arg0) {
    EffectTaskState *gameState;
    s32 i;
    s32 pad;
    Vec3i rotated;

    gameState = (EffectTaskState *)getCurrentAllocation();
    if (gameState->paused == 0) {
        updateStarEffectAnimation((StarEffectState *)arg0);
        arg0->rotationAngle += 0x100;
        rotateVectorY(arg0->orbitOffset, arg0->rotationAngle, &rotated);
        transformVector((s16 *)&rotated, arg0->player->unk9F0, &arg0->sprite.position);
        if (arg0->playSoundFlag != 0) {
            arg0->playSoundFlag = 0;
            queueSoundAtPosition(&arg0->sprite.position, 0x1A);
        }
        if (arg0->displayTimer != 0) {
            arg0->displayTimer--;
        } else if (arg0->animFrameIndex == 0x40) {
            arg0->player->unkBCF--;
            terminateCurrentTask();
        }
    }

    for (i = 0; i < 4; i++) {
        enqueueAlphaBillboardSprite(i, (loadAssetMetadata_arg *)&arg0->sprite);
    }
}

void cleanupStarEffect(void **arg0) {
    *arg0 = freeNodeMemory(*arg0);
}

void spawnStarEffect(void *arg0, void *arg1, s16 arg2) {
    StarEffectTask *task;

    task = (StarEffectTask *)scheduleTask(initStarEffect, 0, 0, 0xDC);
    if (task != NULL) {
        task->unk24 = arg0;
        task->player = arg1;
        task->startDelay = arg2;
        task->immediateMode = 0;
    }
}

StarEffectTask *spawnStarEffectImmediate(void *arg0) {
    StarEffectTask *task;

    task = (StarEffectTask *)scheduleTask(initStarEffect, 0, 0, 0xDC);
    if (task != NULL) {
        task->unk24 = arg0;
        task->player = arg0;
        task->startDelay = 0;
        task->immediateMode = 1;
    }
    return task;
}

typedef struct {
    u8 pad0[0x24];
    void *unk24;
    void *unk28;
} Func432D8Arg;

void cleanupPlayerAuraEffect(Func432D8Arg *);
void func_80044CA4_458A4(Func432D8Arg *);

typedef struct {
    u8 pad0[0x18];              /* 0x00 */
    s32 unk18;                  /* 0x18 */
    u8 pad1C[0x4];              /* 0x1C */
    void *unk20;                /* 0x20 */
    void *unk24;                /* 0x24 */
    void *unk28;                /* 0x28 */
    s32 unk2C;                  /* 0x2C */
    u8 pad30[0xC];              /* 0x30 */
    DisplayListObject orbitObj; /* 0x3C */
    Player *player;             /* 0x78 */
    s32 fallVelocity;           /* 0x7C */
    u16 yRotation;              /* 0x80 */
    u16 orbitAngle;             /* 0x82 */
    s16 scale;                  /* 0x84 */
} PlayerAuraEffectState;

void updatePlayerAuraEffect(PlayerAuraEffectState *state);
void fadeOutPlayerAuraEffect(PlayerAuraEffectState *state);

void initPlayerAuraEffect(PlayerAuraEffectState *state) {
    getCurrentAllocation();
    state->unk20 = &D_8009A710_9B310;
    state->unk24 = loadAsset_B7E70();
    state->unk28 = loadAsset_216290();
    state->orbitObj.displayLists = (DisplayLists *)&D_8009A720_9B320;
    state->unk2C = 0;
    state->orbitObj.segment3 = 0;
    state->scale = 0x200;
    state->orbitObj.segment1 = state->unk24;
    state->orbitObj.segment2 = state->unk28;
    setCleanupCallback(cleanupPlayerAuraEffect);
    setCallbackWithContinue(updatePlayerAuraEffect);
}

void updatePlayerAuraEffect(PlayerAuraEffectState *state) {
    EffectTaskState *gameState;
    s32 i;
    Transform3D matrix;

    gameState = (EffectTaskState *)getCurrentAllocation();
    createYRotationMatrix(&D_8009A8B0_9B4B0, state->yRotation);
    func_8006B084_6BC84(&D_8009A8B0_9B4B0, &state->player->unk3F8, state);
    scaleMatrix((Transform3D *)state, state->scale, state->scale, state->scale);

    state->orbitAngle += 0x300;
    createZRotationMatrix(&matrix, state->orbitAngle);

    matrix.translation.x = 0;
    matrix.translation.y = 0xBB333;
    matrix.translation.z = 0xFFEA0000;

    func_8006B084_6BC84(&matrix, state, &state->orbitObj);

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)state);
        enqueueDisplayListWithFrustumCull(i, (&state->orbitObj));
    }

    if (state->player->unkB84 & 0x80000) {
        state->player->boostTimer = 0;
    }

    if (state->player->boostTimer != 0) {
        if (gameState->paused == 0) {
            state->player->boostTimer--;
            if (state->player->boostTimer == 0) {
                if (state->player->unkBBB == 0x10) {
                    state->player->boostTimer++;
                }
            }
            if (state->scale != 0x2000) {
                state->scale += 0x200;
            }
        }
    } else {
        state->player->boostState = 0;
        state->fallVelocity = 0x40000;
        setCallback(fadeOutPlayerAuraEffect);
    }
}

void fadeOutPlayerAuraEffect(PlayerAuraEffectState *state) {
    EffectTaskState *gameState;
    s32 i;
    Transform3D matrix;

    gameState = (EffectTaskState *)getCurrentAllocation();
    if (gameState->paused == 0) {
        state->fallVelocity -= 0x8000;
        if (state->fallVelocity <= (s32)0xFFF80000) {
            terminateCurrentTask();
        }
        state->unk18 += state->fallVelocity;
        createZRotationMatrix(&matrix, state->orbitAngle);
        matrix.translation.x = 0;
        matrix.translation.y = 0xBB333;
        matrix.translation.z = 0xFFEA0000;
        func_8006B084_6BC84(&matrix, state, &state->orbitObj);
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)state);
        enqueueDisplayListWithFrustumCull(i, &state->orbitObj);
    }
}

void cleanupPlayerAuraEffect(Func432D8Arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void *spawnPlayerAuraEffect(Player *player) {
    PlayerAuraEffectState *task;

    task = (PlayerAuraEffectState *)scheduleTask(initPlayerAuraEffect, 0, 0, 0xC8);
    if (task != NULL) {
        task->player = player;
        task->yRotation = 0;
        if (player->unkB84 & 2) {
            task->yRotation = 0x1000;
        }
    }
    return task;
}

extern void *D_8009A730_9B330;
extern void *D_8009A740_9B340;
extern void *D_8009A750_9B350;
void cleanupPlayerFlashEffect(Func432D8Arg *);
void updatePlayerFlashEffect(PlayerFlashEffectState *);
void fadeOutPlayerFlashEffect(PlayerFlashEffectState *);

void initPlayerFlashEffect(PlayerFlashEffectState *state) {
    getCurrentAllocation();
    state->unk20 = &D_8009A730_9B330;
    state->unk24 = loadAsset_B7E70();
    state->unk28 = loadAsset_216290();
    state->unk2C = 0;
    state->unk68 = 0;
    state->scale = 0x200;
    state->unk60 = state->unk24;
    state->unk64 = state->unk28;
    setCleanupCallback(cleanupPlayerFlashEffect);
    setCallbackWithContinue(updatePlayerFlashEffect);
}

void updatePlayerFlashEffect(PlayerFlashEffectState *state) {
    s32 pad[8];
    GameState *allocation;
    s32 *ptr;
    s32 i;
    Player *player;
    Player *player2;
    s16 scale;

    allocation = (GameState *)getCurrentAllocation();
    createYRotationMatrix(&D_8009A8B0_9B4B0, state->yRotation);
    func_8006B084_6BC84(&D_8009A8B0_9B4B0, &state->player->unk3F8, state);
    scale = state->scale;
    scaleMatrix((Transform3D *)state, scale, scale, scale);

    D_8009A8A4_9B4A4.x = 0;
    D_8009A8A4_9B4A4.y = 0x9CCCC;
    D_8009A8A4_9B4A4.z = 0xFFE44CCD;
    ptr = (s32 *)&D_8009A8A4_9B4A4;
    func_8006B084_6BC84(ptr - 5, state, state->secondaryObj);

    if (gFrameCounter & 1) {
        state->unk5C = &D_8009A740_9B340;
    } else {
        state->unk5C = &D_8009A750_9B350;
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)state);
        if (state->scale == 0x2000) {
            enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)state->secondaryObj);
        }
    }

    player = state->player;
    if (player->unkBC9 == 3) {
        if (player->boostTimer != 0) {
            if (player->boostTimer < 0x3C) {
                player->boostTimer = 0x3C;
            }
        }
        player = state->player;
    }

    if (player->unkB84 & 0x80000) {
        player->boostTimer = 0;
    }

    player2 = state->player;
    if (player2->boostTimer != 0) {
        if (allocation->gamePaused == 0) {
            if (state->scale == 0x2000) {
                player2->boostTimer--;
                player = state->player;
                if (player->boostTimer == 0 && player->unkBBB == 0x11) {
                    player->boostTimer++;
                }
            } else {
                state->scale = state->scale + 0x200;
                if (state->scale == 0x2000) {
                    state->player->boostState += 2;
                }
            }
        }
    } else {
        player2->boostState = 0;
        state->fallVelocity = 0x40000;
        setCallback(fadeOutPlayerFlashEffect);
    }
}

void fadeOutPlayerFlashEffect(PlayerFlashEffectState *state) {
    GameState *gameState;
    s32 i;
    s32 pad[7];

    gameState = (GameState *)getCurrentAllocation();
    if (gameState->gamePaused == 0) {
        state->fallVelocity -= 0x8000;
        if (state->fallVelocity <= (s32)0xFFF80000) {
            terminateCurrentTask();
        }
        state->unk18 += state->fallVelocity;
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)state);
    }
}

void cleanupPlayerFlashEffect(Func432D8Arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

PlayerFlashEffectState *spawnPlayerFlashEffect(Player *player) {
    PlayerFlashEffectState *task;

    task = (PlayerFlashEffectState *)scheduleTask(initPlayerFlashEffect, 0, 0, 0xC8);
    if (task != NULL) {
        task->player = player;
        task->yRotation = 0;
        if (player->unkB84 & 2) {
            task->yRotation = 0x1000;
        }
    }
    return task;
}

s32 advanceAnimationFrame(void *arg0_void, void *animData_void) {
    GoldStealEffectState *state = (GoldStealEffectState *)arg0_void;
    u8 *animData = (u8 *)animData_void;
    s8 nextFrameDuration;

    state->frameTimer--;

    if (state->frameTimer != 0) {
        return 0;
    }

    loadAssetMetadata((void *)((s32)state + 4), (void *)state->unk0, (s8)animData[state->animFrameIndex * 2 + 1]);

    state->frameTimer = (s8)animData[state->animFrameIndex * 2];
    state->animFrameIndex++;

    nextFrameDuration = animData[state->animFrameIndex * 2];
    if (nextFrameDuration != 0) {
        return 0;
    }

    state->animFrameIndex--;
    return 1;
}

void advanceAnimationFrameLooping(GoldStealEffectState *arg0, s8 *arg1) {
    s16 temp;

    temp = arg0->frameTimer - 1;
    arg0->frameTimer = temp;

    if ((temp << 16) == 0) {
        loadAssetMetadata(
            (loadAssetMetadata_arg *)((s32)arg0 + 4),
            (void *)arg0->unk0,
            arg1[arg0->animFrameIndex * 2 + 1]
        );
        arg0->frameTimer = arg1[arg0->animFrameIndex * 2];
        temp = (u16)arg0->animFrameIndex + 1;
        arg0->animFrameIndex = temp;
        if (arg1[temp * 2] == 0) {
            arg0->animFrameIndex = 0;
        }
    }
}

void cleanupGoldStealEffect(void **);
void animateGoldStealApproach(GoldStealEffectState *);
void prepareGoldStealEffect(GoldStealEffectState *);
void animateGoldStealLoop(GoldStealEffectState *);
void animateGoldStealTransfer(GoldStealEffectState *);
void animateGoldStealFinish(GoldStealEffectState *);

void initGoldStealEffect(void **arg0) {
    *arg0 = load_3ECE40();
    setCleanupCallback(cleanupGoldStealEffect);
    setCallbackWithContinue(prepareGoldStealEffect);
}

void prepareGoldStealEffect(GoldStealEffectState *arg0) {
    EffectTaskState *gameState;
    s16 temp;
    void *ptr;

    gameState = (EffectTaskState *)getCurrentAllocation();
    temp = arg0->frameTimer;

    if (temp == 0) {
        arg0->frameTimer = 1;
        arg0->unk46 = 0;
        ptr = (void *)((u8 *)gameState->spriteData + 0xF40);
        arg0->unk30 = 0x300000;
        arg0->unk2C = 0;
        arg0->unk34 = 0;
        arg0->animFrameIndex = 0;
        arg0->unk4C = 1;
        arg0->unk4 = ptr;
        advanceAnimationFrame(arg0, &D_8009093C_9153C);
        setCallbackWithContinue(animateGoldStealApproach);
    } else {
        if (gameState->paused == 0) {
            arg0->frameTimer = temp - 1;
        }
    }
}

void animateGoldStealApproach(GoldStealEffectState *arg0) {
    EffectTaskState *gameState;
    s32 i;

    gameState = (EffectTaskState *)getCurrentAllocation();
    if (gameState->paused == 0) {
        if (advanceAnimationFrame(arg0, &D_8009093C_9153C) != 0) {
            arg0->unk38 = 0;
            arg0->animFrameIndex = 0;
            arg0->frameTimer = 1;
            setCallback(animateGoldStealLoop);
        }
    }

    transformVector((s16 *)&arg0->unk2C, arg0->recipientPlayer->unk9F0, &arg0->position);

    if (arg0->unk4C != 0) {
        arg0->unk4C = 0;
        queueSoundAtPosition(&arg0->position, 0x12);
    }

    for (i = 0; i < 4; i++) {
        enqueueTexturedBillboardSprite(i, (TexturedBillboardSprite *)&arg0->unk4);
    }
}

void animateGoldStealLoop(GoldStealEffectState *arg0) {
    EffectTaskState *gameState;
    s32 i;

    gameState = (EffectTaskState *)getCurrentAllocation();
    if (gameState->paused == 0) {
        advanceAnimationFrameLooping(arg0, &D_80090950_91550);
        arg0->unk38 = arg0->unk38 + 0x8000;
        arg0->unk30 = arg0->unk30 + arg0->unk38;
        if (arg0->unk30 > 0x600000) {
            arg0->swingAngle = 0;
            arg0->animFrameIndex = 0;
            arg0->frameTimer = 1;
            setCallback(animateGoldStealTransfer);
        }
    }

    transformVector((s16 *)&arg0->unk2C, arg0->recipientPlayer->unk9F0, &arg0->position);

    for (i = 0; i < 4; i++) {
        enqueueTexturedBillboardSprite(i, (TexturedBillboardSprite *)&arg0->unk4);
    }
}

void animateGoldStealRetreat(GoldStealEffectState *);

void animateGoldStealTransfer(GoldStealEffectState *arg0) {
    EffectTaskState *gameState;
    s32 sinVal;
    s32 i;

    gameState = (EffectTaskState *)getCurrentAllocation();
    if (gameState->paused != 0) {
        goto transform_and_loop;
    }

    arg0->swingAngle = arg0->swingAngle + 0x80;
    sinVal = approximateSin(arg0->swingAngle);
    arg0->unk30 = 0x700000 - (sinVal * 3 << 8);

    if (arg0->swingAngle < 0x800) {
        advanceAnimationFrameLooping(arg0, &D_80090958_91558);
    } else if (arg0->swingAngle == 0x800) {
        arg0->animFrameIndex = 0;
        arg0->frameTimer = 1;
        arg0->stolenGold = getPlayerRaceGold((Player *)arg0->victimPlayer);
        addPlayerRaceGold((Player *)arg0->victimPlayer, -arg0->stolenGold);
        advanceAnimationFrameLooping(arg0, &D_8009095C_9155C);
    } else {
        advanceAnimationFrameLooping(arg0, &D_8009095C_9155C);
    }

    if (arg0->swingAngle == 0xC00) {
        arg0->animFrameIndex = 0;
        arg0->frameTimer = 1;
        addPlayerRaceGold((Player *)arg0->recipientPlayer, arg0->stolenGold);
        setCallback(animateGoldStealRetreat);
    }

transform_and_loop:
    transformVector((s16 *)&arg0->unk2C, arg0->victimPlayer->unk9F0, &arg0->position);

    if (arg0->swingAngle == 0x80) {
        queueSoundAtPosition(&arg0->position, 0x1C);
    }

    for (i = 0; i < 4; i++) {
        enqueueTexturedBillboardSprite(i, (TexturedBillboardSprite *)&arg0->unk4);
    }
}

void animateGoldStealRetreat(GoldStealEffectState *arg0) {
    EffectTaskState *gameState;
    s32 i;

    gameState = (EffectTaskState *)getCurrentAllocation();
    if (gameState->paused == 0) {
        if (advanceAnimationFrame(arg0, &D_80090964_91564) != 0) {
            arg0->animFrameIndex = 0;
            arg0->frameTimer = 1;
            queueSoundAtPosition(&arg0->position, 0x12);
            setCallback(animateGoldStealFinish);
        }
    }

    transformVector((s16 *)&arg0->unk2C, arg0->victimPlayer->unk9F0, &arg0->position);

    for (i = 0; i < 4; i++) {
        enqueueTexturedBillboardSprite(i, (TexturedBillboardSprite *)&arg0->unk4);
    }
}

void animateGoldStealFinish(GoldStealEffectState *arg0) {
    EffectTaskState *gameState;
    s32 i;

    gameState = (EffectTaskState *)getCurrentAllocation();
    if (gameState->paused == 0) {
        if (advanceAnimationFrame(arg0, &D_80090974_91574) != 0) {
            terminateCurrentTask();
        }
    }

    transformVector((s16 *)&arg0->unk2C, arg0->victimPlayer->unk9F0, &arg0->position);

    for (i = 0; i < 4; i++) {
        enqueueTexturedBillboardSprite(i, (TexturedBillboardSprite *)&arg0->unk4);
    }
}

void cleanupGoldStealEffect(void **arg0) {
    *arg0 = freeNodeMemory(*arg0);
}

void spawnGoldStealEffect(void *arg0, void *arg1, s16 arg2) {
    GoldStealEffectState *task;

    task = (GoldStealEffectState *)scheduleTask(initGoldStealEffect, 0, 0, 0xDC);
    if (task != NULL) {
        task->recipientPlayer = arg0;
        task->victimPlayer = arg1;
        task->frameTimer = arg2;
    }
}

void initGhostEffect(GhostEffectState *arg0) {
    getCurrentAllocation();
    arg0->displayData = &D_8009A760_9B360;
    arg0->asset1 = loadAsset_B7E70();
    arg0->asset2 = loadAsset_216290();
    arg0->unk2C = 0;
    arg0->scale = 0x200;
    setCleanupCallback(cleanupGhostEffect);
    setCallbackWithContinue(updateGhostEffect);
}

void updateGhostEffect(GhostEffectState *arg0) {
    EffectTaskState *allocation;
    Player *player;
    Player *temp_player;
    u16 count;
    u16 new_count;
    s32 i;

    allocation = (EffectTaskState *)getCurrentAllocation();
    createYRotationMatrix(&D_8009A8B0_9B4B0, arg0->rotation);
    func_8006B084_6BC84(&D_8009A8B0_9B4B0, (u8 *)arg0->player + 0x3F8, arg0);

    if (arg0->scale == 0x200) {
        queueSoundAtPosition(&arg0->position, 0x1D);
    }

    if (arg0->scale != 0x2000) {
        arg0->scale += 0x200;
    }

    scaleMatrix((Transform3D *)arg0, arg0->scale, arg0->scale, arg0->scale);

    if (gFrameCounter & 4) {
        arg0->displayData = &D_8009A760_9B360;
    } else {
        arg0->displayData = &D_8009A770_9B370;
    }

    i = 0;
    do {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
        i++;
    } while (i < 4);

    player = arg0->player;
    if (player->unkB84 & 0x80000) {
        player->ghostEffectTimer = 0;
    }

    temp_player = arg0->player;
    count = temp_player->ghostEffectTimer;
    if (count != 0) {
        if (allocation->paused == 0) {
            temp_player->ghostEffectTimer = count - 1;
            player = arg0->player;
            new_count = player->ghostEffectTimer;
            if (new_count == 0) {
                if (player->unkBBB == 0x11) {
                    player->ghostEffectTimer = new_count + 1;
                }
            }
        }
    } else {
        temp_player->ghostEffectState = 0;
        arg0->velocityY = 0x40000;
        setCallback(fadeOutGhostEffect);
    }
}

void fadeOutGhostEffect(GhostEffectState *arg0) {
    GameState *state;
    s32 i;
    s32 pad[8];

    state = getCurrentAllocation();
    if (state->gamePaused == 0) {
        arg0->velocityY = arg0->velocityY - 0x8000;
        if ((s32)0xFFF80000 >= arg0->velocityY) {
            terminateCurrentTask();
        }
        arg0->position.y = arg0->position.y + arg0->velocityY;
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void cleanupGhostEffect(GhostEffectState *arg0) {
    arg0->asset1 = freeNodeMemory(arg0->asset1);
    arg0->asset2 = freeNodeMemory(arg0->asset2);
}

void *spawnGhostEffect(Player *arg0) {
    GhostEffectState *task;

    task = scheduleTask(initGhostEffect, 0, 0, 0xC8);
    if (task != NULL) {
        task->player = arg0;
        task->rotation = 0;
        if (arg0->unkB84 & 2) {
            task->rotation = 0x1000;
        }
    }
    return task;
}

extern s32 D_80090980_91580[];
extern s32 D_80090984_91584[];
extern s32 D_80090988_91588[];
extern s32 D_80090994_91594[];
extern s32 D_80090996_91596[];

typedef struct {
    u8 _pad0[0x5C];
    u8 unk5C;
} Func44CB4Allocation;

typedef struct {
    u8 pad0[0x14]; /* 0x00 */
    s32 unk14[3];  /* 0x14 */
    void *unk20;   /* 0x20 */
    void *unk24;   /* 0x24 */
    void *unk28;   /* 0x28 */
    s32 unk2C;     /* 0x2C */
    u8 pad30[0xC]; /* 0x30 */
    s16 zoneIndex; /* 0x3C */
} PushZoneState;

void cleanupPushZone(Func432D8Arg *);
void func_800441A4_44DA4(PushZoneState *);

void initPushZone(PushZoneState *arg0) {
    Func44CB4Allocation *allocation;
    s32 offset;

    allocation = getCurrentAllocation();
    offset = arg0->zoneIndex * 9;
    createCombinedRotationMatrix(arg0, *(u16 *)&D_80090994_91594[offset], *(u16 *)&D_80090996_91596[offset]);
    memcpy(&arg0->unk14, &D_80090988_91588[arg0->zoneIndex * 9], 0xC);
    arg0->unk20 = (void *)(D_80090980_91580[arg0->zoneIndex * 9] + (D_80090984_91584[arg0->zoneIndex * 9] << 4));
    arg0->unk24 = loadUncompressedAssetByIndex(allocation->unk5C);
    arg0->unk28 = loadCompressedSegment2AssetByIndex(allocation->unk5C);
    arg0->unk2C = 0;
    setCleanupCallback(cleanupPushZone);
    setCallbackWithContinue(func_800441A4_44DA4);
}

INCLUDE_ASM("asm/nonmatchings/42170", func_800441A4_44DA4);

void cleanupPushZone(Func432D8Arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void spawnPushZone(s16 zoneIndex) {
    PushZoneState *task = (PushZoneState *)scheduleTask(initPushZone, 0, 0, 0x32);
    if (task != NULL) {
        task->zoneIndex = zoneIndex;
    }
}

// Note: I have been unable to trigger this codepath. It does not appear
// to be a regular item, cutscene action, or the ufo chairlift from starlight
// highway.

void renderUfoEffectWithWings(UfoEffectState *arg0) {
    Transform3D matrix;
    s32 sinVal;
    s32 i;

    arg0->wingOscillationAngle = arg0->wingOscillationAngle + 0x100;
    sinVal = approximateSin((s16)arg0->wingOscillationAngle);
    createZRotationMatrix(&matrix, (sinVal >> 5) & 0xFFFF);

    matrix.translation.x = 0xFFF7490A;
    matrix.translation.y = 0xFFF98007;
    matrix.translation.z = 0xCB326;
    func_8006B084_6BC84(&matrix, arg0, &arg0->leftWing);

    sinVal = approximateSin(arg0->wingOscillationAngle);
    createZRotationMatrix(&matrix, (-(sinVal >> 5)) & 0xFFFF);

    matrix.translation.x = 0x8B6F6;
    matrix.translation.y = 0xFFF98007;
    matrix.translation.z = 0xCB326;
    func_8006B084_6BC84(&matrix, arg0, &arg0->rightWing);

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
        enqueueDisplayListWithFrustumCull(i, &arg0->leftWing);
        enqueueDisplayListWithFrustumCull(i, &arg0->rightWing);
    }
}

void initUfoEffect(UfoEffectState *arg0) {
    Vec3i posOutput;
    Vec3i transformOutput;
    EffectTaskState *allocation;
    u16 rotation;
    LevelConfig *item;
    s32 temp_unk18;

    allocation = (EffectTaskState *)getCurrentAllocation();
    rotation = getTrackEndInfo(&allocation->unk30, &posOutput) + 0x800;
    item = getLevelConfig(allocation->unk5C);

    arg0->unk20 = (u8 *)getSkyDisplayLists3ByIndex(allocation->unk5C) + 0x10;

    arg0->unk24 = loadUncompressedAssetByIndex(allocation->unk5C);
    arg0->unk28 = loadCompressedSegment2AssetByIndex(allocation->unk5C);
    arg0->unk2C = 0;

    arg0->leftWing.displayLists = (DisplayLists *)((u8 *)getSkyDisplayLists3ByIndex(allocation->unk5C) + 0x90);
    arg0->leftWing.segment3 = 0;
    arg0->leftWing.segment1 = arg0->unk24;
    arg0->leftWing.segment2 = arg0->unk28;

    arg0->rightWing.displayLists = (DisplayLists *)((u8 *)getSkyDisplayLists3ByIndex(allocation->unk5C) + 0xA0);
    arg0->rightWing.segment3 = 0;
    arg0->rightWing.segment1 = arg0->unk24;
    arg0->rightWing.segment2 = arg0->unk28;

    arg0->baseRotation = rotation + item->yawOffset;
    createYRotationMatrix((Transform3D *)arg0, arg0->baseRotation);

    arg0->yRotation = 0;
    transformVector2(&D_80090AA0_916A0, arg0, &transformOutput);

    arg0->unk14.x = item->shortcutPosX + transformOutput.x;
    arg0->unk14.z = item->shortcutPosZ + transformOutput.z;
    temp_unk18 = posOutput.y + transformOutput.y;
    arg0->phaseTimer = 0x30;
    arg0->unk14.y = temp_unk18;

    setCleanupCallback(cleanupUfoEffect);
    setCallbackWithContinue(flyInUfoEffect);
}

void flyInUfoEffect(UfoEffectState *arg0) {
    EffectTaskState *gameState;
    Vec3i output;

    gameState = getCurrentAllocation();
    if (gameState->paused == 0) {
        transformVector2(&D_80090AAC_916AC, arg0, &output);
        arg0->unk14.x = arg0->unk14.x + output.x;
        arg0->unk14.z = arg0->unk14.z + output.z;
        memcpy(&arg0->target->storedPos, &arg0->unk14, 0xC);
        arg0->target->storedRotY = arg0->yRotation;
        if (arg0->phaseTimer != 0) {
            arg0->phaseTimer = arg0->phaseTimer - 1;
        } else {
            arg0->phaseTimer = 0xB4;
            setCallback(descendUfoEffect);
        }
    }

    renderUfoEffectWithWings(arg0);
}

void descendUfoEffect(UfoEffectState *arg0) {
    EffectTaskState *gameState;
    Vec3i output;

    gameState = (EffectTaskState *)getCurrentAllocation();
    if (gameState->paused != 0) {
        goto end;
    }

    {
        s16 temp = arg0->yRotation;
        if (temp != -0x400) {
            arg0->yRotation = temp - 0x10;
        }
    }

    createCombinedRotationMatrix(arg0, arg0->yRotation, arg0->baseRotation);
    transformVector2(&D_80090AAC_916AC, arg0, &output);

    arg0->unk14.x = arg0->unk14.x + output.x;
    arg0->unk14.y = arg0->unk14.y + output.y;
    arg0->unk14.z = arg0->unk14.z + output.z;

    memcpy(&arg0->target->storedPos, &arg0->unk14, 0xC);
    arg0->target->storedRotY = arg0->yRotation;

    {
        s16 temp = arg0->phaseTimer;
        if (temp != 0) {
            s16 temp2 = temp - 1;
            arg0->phaseTimer = temp2;
            if (temp2 == 0x12) {
                arg0->target->flags |= 8;
            }
        } else {
            setCallback(setupUfoFlyAway);
        }
    }

end:
    renderUfoEffectWithWings(arg0);
}

void setupUfoFlyAway(UfoEffectState *arg0) {
    LevelConfig *temp_v0;
    UfoAbductionPlayerState *temp_a1;
    Vec3i output;
    Vec3i input;
    s32 i;
    s32 temp_unk14;

    temp_v0 = getLevelConfig(((EffectTaskState *)getCurrentAllocation())->unk5C);
    temp_a1 = arg0->target;
    temp_a1->flags |= 2;
    arg0->yRotation = -0x300;
    arg0->phaseTimer = 0x5A;
    arg0->baseRotation = 0x1000;
    createCombinedRotationMatrix(arg0, (u16)arg0->yRotation, 0x1000);
    arg0->unk14.x = temp_v0->spawnPos.x + 0xFFD00000;
    arg0->unk14.y = temp_v0->spawnPos.y;
    temp_unk14 = temp_v0->spawnPos.z;
    i = 0;
    arg0->flyAwayDistance = 0;
    arg0->fallVelocity = 0;
    arg0->movementOffset = 0;
    arg0->unk14.z = temp_unk14 + 0x200000;
    input.x = 0;
    input.y = 0;
    input.z = 0;
    do {
        arg0->flyAwayDistance += 0x2000;
        i++;
        input.z += arg0->flyAwayDistance;
    } while (i < 0x5A);
    transformVector2(&input, arg0, &output);
    arg0->unk14.x -= output.x;
    arg0->unk14.y -= output.y;
    arg0->unk14.z -= output.z;
    setCallbackWithContinue(flyAwayUfoEffect);
}

void flyAwayUfoEffect(UfoEffectState *arg0) {
    EffectTaskState *gameState;
    Vec3i output;

    gameState = getCurrentAllocation();
    if (gameState->paused != 0) {
        goto end;
    }

    if (arg0->flyAwayDistance > 0x20000) {
        goto skip_rotation;
    }

    {
        s16 temp = arg0->yRotation;
        if (temp != 0) {
            arg0->yRotation = temp + 0x40;
        }
    }

    createCombinedRotationMatrix(arg0, arg0->yRotation, arg0->baseRotation);

skip_rotation:
    transformVector2(&arg0->movementOffset, arg0, &output);
    arg0->unk14.x = arg0->unk14.x + output.x;
    arg0->unk14.y = arg0->unk14.y + output.y;
    arg0->unk14.z = arg0->unk14.z + output.z;
    memcpy(arg0->target->storedPos, &arg0->unk14, 0xC);
    arg0->target->storedRotY = arg0->yRotation;

    if (arg0->flyAwayDistance != 0) {
        arg0->flyAwayDistance = arg0->flyAwayDistance - 0x2000;
    } else {
        arg0->phaseTimer = 4;
        arg0->target->flags |= 4;
        setCallback(holdUfoEffect);
    }

end:
    renderUfoEffectWithWings(arg0);
}

void holdUfoEffect(UfoEffectState *arg0) {
    EffectTaskState *gameState = (EffectTaskState *)getCurrentAllocation();
    s32 pad[4];

    if (gameState->paused == 0) {
        arg0->phaseTimer--;
        if (arg0->phaseTimer == 0) {
            setCallback(fadeOutUfoEffect);
        }
        memcpy(&arg0->target->storedPos, &arg0->unk14, 0xC);
        arg0->target->storedRotY = arg0->yRotation;
    }

    renderUfoEffectWithWings(arg0);
}

void fadeOutUfoEffect(UfoEffectState *arg0) {
    EffectTaskState *gameState = (EffectTaskState *)getCurrentAllocation();
    s32 pad[4];

    if (gameState->paused == 0) {
        arg0->fallVelocity -= 0x8000;
        if (arg0->fallVelocity < (s32)0xFFF00000) {
            terminateCurrentTask();
        }
        arg0->unk14.y += arg0->fallVelocity;
    }

    renderUfoEffectWithWings(arg0);
}

void cleanupUfoEffect(void *arg0) {
    Func432D8Arg *cleanupArg = (Func432D8Arg *)arg0;
    cleanupArg->unk24 = freeNodeMemory(cleanupArg->unk24);
    cleanupArg->unk28 = freeNodeMemory(cleanupArg->unk28);
}

typedef struct {
    u8 _pad0[0xB4];
    void *target;
} UfoEffectTaskMem;

void spawnUfoEffect(void *arg0) {
    UfoEffectTaskMem *task;

    task = (UfoEffectTaskMem *)scheduleTask(initUfoEffect, 0, 0, 0x32);
    if (task != NULL) {
        task->target = arg0;
    }
}

extern void *D_80090AB8_916B8[];
extern void *D_80090ABC_916BC[];
extern s32 D_80090AC0_916C0[];

typedef struct {
    s8 active;      /* 0x00 - 1 when active, set to 0 when processed */
    s8 type;        /* 0x01 - 0 for primary item, non-zero for secondary */
    s16 itemId;     /* 0x02 - item ID to give to the player */
    Vec3i position; /* 0x04 - world position for player detection */
} ItemTriggerEntry; /* size: 0x10 */

typedef struct {
    void *matrices;                /* 0x00 - allocated matrix memory for each item */
    void *vertices;                /* 0x04 - vertex data pointer */
    ItemTriggerEntry *items;       /* 0x08 - array of item trigger entries */
    DataTable_19E80 *textureTable; /* 0x0C - texture table for rendering items */
    s32 *itemData;                 /* 0x10 - loaded item trigger data */
    s16 courseIndex;               /* 0x14 - course/level index */
    s16 numItems;                  /* 0x16 - number of item trigger entries */
} ItemTriggerTaskState;

void setupItemTriggerEntries(ItemTriggerTaskState *);
void processItemTriggers(ItemTriggerTaskState *);

typedef struct Func45010Arg Func45010Arg;
void cleanupItemTriggerTask(Func45010Arg *);

void initItemTriggerTask(ItemTriggerTaskState *arg0) {
    s16 idx;

    arg0->textureTable = loadCompressedData(&_3F3EF0_ROM_START, &_3F3EF0_ROM_END, 0x2608);
    idx = arg0->courseIndex;
    arg0->itemData =
        loadCompressedData(D_80090AB8_916B8[idx * 3], D_80090ABC_916BC[idx * 3], D_80090AC0_916C0[idx * 3]);
    arg0->matrices = NULL;
    setCleanupCallback(cleanupItemTriggerTask);
    setCallback(setupItemTriggerEntries);
}

void renderItemTriggers(ItemTriggerTaskState *);

void setupItemTriggerEntries(ItemTriggerTaskState *arg0) {
    s32 i;
    s32 *ptr;
    ItemTriggerEntry *entries;
    s32 pad[2];
    s32 one;
    s32 offset;

    arg0->vertices = (void *)((u8 *)((GameState *)getCurrentAllocation())->unk44 + 0xF80);
    arg0->items = (ItemTriggerEntry *)((s8 *)arg0->itemData + *arg0->itemData);
    entries = *(ItemTriggerEntry *volatile *)&arg0->items;
    arg0->numItems = 0;

    if (entries->active >= 0) {
        do {
            arg0->numItems++;
        } while (entries[arg0->numItems].active >= 0);
    }

    i = 0;
    arg0->matrices = allocateNodeMemory(arg0->numItems << 6);

    if (arg0->numItems > 0) {
        one = 1;
        ptr = (s32 *)&D_8009A8A4_9B4A4;
        do {
            offset = i << 4;
            *((s8 *)(offset + (s32)arg0->items)) = one;
            memcpy(ptr, (s8 *)(offset + (s32)arg0->items + 4), 0xC);
            transform3DToMtx(ptr - 5, (u8 *)arg0->matrices + (i << 6));
            i++;
        } while (i < arg0->numItems);
    }

    setCallback(processItemTriggers);
}

/**
 * Processes item trigger entries to give items to nearby players.
 *
 * For each active entry in arg0->items:
 * - Finds a player within range of the entry's position
 * - If player found (and not a boss), gives them the item:
 *   - Type 0 (primary): Sets unkBD2 to itemId, unkBD3 to 3 (or 9 in special mode)
 *   - Type 1 (secondary): Sets unkBD4 to itemId
 * - Marks entry as inactive and plays pickup sound
 */
void processItemTriggers(ItemTriggerTaskState *arg0) {
    s32 i;
    GameState *gameState;
    ItemTriggerEntry *entry;
    Player *player;
    ItemTriggerEntry *entryForItem;
    u8 itemFlags;
    s32 offset;
    u16 itemId;
    s32 pad[2];
    s32 three;
    s32 nine;

    gameState = (GameState *)getCurrentAllocation();
    i = 0;
    if (arg0->numItems > 0) {
        three = 3;
        nine = 9;
        offset = 0;
        do {
            entry = (ItemTriggerEntry *)(offset + (s32)arg0->items);
            if (entry->active != 0) {
                player = findPlayerNearPosition(&entry->position, -1, 0x100000);
                if (player != NULL && player->unkBD9 == 0) {
                    entryForItem = (ItemTriggerEntry *)(offset + (s32)arg0->items);
                    if (entryForItem->type == 0) {
                        itemId = entryForItem->itemId;
                        player->unkBD3 = three;
                        player->unkBD2 = (u8)itemId;
                        if (gameState->raceType == three) {
                            player->unkBD3 = nine;
                        }
                        itemFlags = player->unkBD8 | 1;
                    } else {
                        itemFlags = player->unkBD8 | 2;
                        player->unkBD4 = (u8)entryForItem->itemId;
                    }
                    player->unkBD8 = itemFlags;
                    ((ItemTriggerEntry *)(offset + (s32)arg0->items))->active = 0;
                    queueSoundAtPosition((Vec3i *)((s8 *)((s32)arg0->items + offset) + 4), 8);
                }
            }
            i++;
            offset += 0x10;
        } while (i < arg0->numItems);
        i = 0;
    }
    do {
        debugEnqueueCallback((u16)i, 4, &renderItemTriggers, arg0);
        i++;
    } while (i < 4);
}

struct Func45010Arg {
    void *matrices;     /* 0x00 */
    u8 pad4[0x8];       /* 0x04 */
    void *textureTable; /* 0x0C */
    void *itemData;     /* 0x10 */
};

void cleanupItemTriggerTask(Func45010Arg *arg0) {
    arg0->matrices = freeNodeMemory(arg0->matrices);
    arg0->textureTable = freeNodeMemory(arg0->textureTable);
    arg0->itemData = freeNodeMemory(arg0->itemData);
}

extern Gfx D_8009A780_9B380[];
extern s32 gLookAtPtr;
extern s16 gGraphicsMode;
extern Gfx *gRegionAllocPtr;

void renderItemTriggers(ItemTriggerTaskState *arg0) {
    OutputStruct_19E80 tableEntry;
    s32 prevTextureIndex;
    s32 i;
    s32 offset;
    s32 dxtBase;
    s32 new_var;
    u32 line;
    s32 lrs;
    u16 widthDiv16;
    Gfx *loadBlockCmd;
    long loadBlockWord;
    u32 tileLine;
    ItemTriggerEntry *entry;
    s32 textureIndex;

    prevTextureIndex = -1;
    gSPDisplayList(gRegionAllocPtr++, D_8009A780_9B380);
    gGraphicsMode = -1;

    for (i = 0; i < arg0->numItems; i++) {
        offset = i << 4;

        if (isObjectCulled(&arg0->items[i].position) == 0) {
            entry = (ItemTriggerEntry *)(offset + (s32)arg0->items);

            if (entry->active != 0) {
                if (entry->type == 0) {
                    textureIndex = entry->itemId;
                } else {
                    textureIndex = entry->itemId + 7;
                }

                if (textureIndex != prevTextureIndex) {
                    getTableEntryByU16Index(arg0->textureTable, textureIndex & 0xFFFF, &tableEntry);

                    gDPSetTextureImage(gRegionAllocPtr++, G_IM_FMT_CI, G_IM_SIZ_16b, 1, tableEntry.data_ptr);

                    gDPSetTile(
                        gRegionAllocPtr++,
                        G_IM_FMT_CI,
                        G_IM_SIZ_16b,
                        0,
                        0x0000,
                        G_TX_LOADTILE,
                        0,
                        G_TX_CLAMP,
                        G_TX_NOMASK,
                        G_TX_NOLOD,
                        G_TX_CLAMP,
                        G_TX_NOMASK,
                        G_TX_NOLOD
                    );

                    gDPLoadSync(gRegionAllocPtr++);

                    loadBlockCmd = gRegionAllocPtr++;
                    loadBlockCmd->words.w0 = 0xF3000000;
                    widthDiv16 = tableEntry.field1 >> 4;
                    dxtBase = 0x800;
                    if (widthDiv16 != 0) {
                        dxtBase = widthDiv16 + 0x7FF;
                    }
                    lrs = (((s32)((tableEntry.field1 * tableEntry.field2) + 3)) >> 2) - 1;
                    if (lrs < 0x800) {
                    } else {
                        lrs = 0x7FF;
                    }
                    line = lrs & 0xFFF;
                    new_var = (line << 12) | 0x07000000;
                    loadBlockWord = new_var;
                    if (widthDiv16 != 0) {
                        loadBlockWord |= (dxtBase / widthDiv16) & 0xFFF;
                    } else {
                        loadBlockWord |= dxtBase & 0xFFF;
                    }
                    loadBlockCmd->words.w1 = loadBlockWord;

                    gDPPipeSync(gRegionAllocPtr++);

                    tileLine = (((tableEntry.field1 >> 1) + 7) >> 3) & 0x1FF;
                    new_var = G_TX_NOMIRROR;
                    gDPSetTile(
                        gRegionAllocPtr++,
                        G_IM_FMT_CI,
                        G_IM_SIZ_4b,
                        tileLine,
                        0,
                        G_TX_RENDERTILE,
                        0,
                        G_TX_CLAMP,
                        G_TX_NOMASK,
                        G_TX_NOLOD,
                        G_TX_CLAMP,
                        G_TX_NOMASK,
                        G_TX_NOLOD
                    );

                    gDPSetTileSize(
                        gRegionAllocPtr++,
                        G_TX_RENDERTILE,
                        0,
                        0,
                        (tableEntry.field1 - 1) << 2,
                        (tableEntry.field2 - 1) << 2
                    );

                    gDPSetTextureImage(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, tableEntry.index_ptr);

                    gDPTileSync(gRegionAllocPtr++);

                    gDPSetTile(
                        gRegionAllocPtr++,
                        G_IM_FMT_RGBA,
                        G_IM_SIZ_4b,
                        0,
                        0x0100,
                        G_TX_LOADTILE,
                        0,
                        G_TX_NOMIRROR | G_TX_WRAP,
                        G_TX_NOMASK,
                        G_TX_NOLOD,
                        G_TX_NOMIRROR | G_TX_WRAP,
                        G_TX_NOMASK,
                        G_TX_NOLOD
                    );

                    gDPLoadSync(gRegionAllocPtr++);

                    gDPLoadTLUTCmd(gRegionAllocPtr++, G_TX_LOADTILE, 15);

                    gDPPipeSync(gRegionAllocPtr++);

                    prevTextureIndex = textureIndex;
                }

                gSPMatrix(
                    gRegionAllocPtr++,
                    (u8 *)arg0->matrices + (i << 6),
                    G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW
                );

                gSPMatrix(gRegionAllocPtr++, gLookAtPtr, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);

                gSPVertex(gRegionAllocPtr++, arg0->vertices, 4, 0);

                gSP2Triangles(gRegionAllocPtr++, 0, 3, 2, 0, 2, 1, 0, 0);
            }
        }
    }
}

typedef struct {
    u8 _pad0[0x14];
    s16 courseIndex;
} ItemTriggerTaskMem;

void spawnItemTriggerTask(s16 arg0) {
    ItemTriggerTaskMem *task;

    task = (ItemTriggerTaskMem *)scheduleTask(initItemTriggerTask, 0, 0, 0xD3);
    if (task != NULL) {
        task->courseIndex = arg0;
    }
}
