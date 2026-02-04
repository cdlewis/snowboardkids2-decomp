#include "3CD70.h"
#include "42170.h"
#include "46080.h"
#include "51060.h"
#include "52880.h"
#include "59290.h"
#include "5AA90.h"
#include "5DBC0.h"
#include "5E590.h"
#include "5EA60.h"
#include "9FF70.h"
#include "A9A40.h"
#include "audio.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "rand.h"
#include "task_scheduler.h"

typedef void (*FuncPtr)(void *);

typedef struct {
    /* 0x0 */ void *ptr;
    u8 _pad[0x38];
} Element0x3C; // Size: 0x3C

typedef struct {
    u8 _pad0[0x38];
    u8 unk38[0x6C - 0x38];
    u8 unk6C;
    u8 unk6D;
    u8 unk6E;
    u8 _pad6F[1];
    u8 unk70;
    u8 unk71;
    u8 unk72;
    u8 _pad73[0x328 - 0x73];
    // Element0x3C elements[12] overlaps at offset 0x58
    u8 _pad328[0x434 - 0x328];
    Vec3i unk434;
    u8 _pad440[0x44C - 0x440];
    Vec3i velocity;
    u8 _pad458[0x984 - 0x458];
    u8 unk984[0xC];
    u8 _pad990[0xAE0 - 0x990];
    s32 unkAE0;
    u8 _padAE4[0xB2C - 0xAE4];
    s32 unkB2C;
    u8 _padB30[0xB84 - 0xB30];
    s32 bossFlags;
    s32 behaviorFlags;
    u8 _padB8C[0xB94 - 0xB8C];
    u16 sectorIndex;
    u8 _padB96[0xBB4 - 0xB96];
    u8 unkBB4;
    u8 _padBB5[0xBB7 - 0xBB5];
    u8 leanBoneCount;
    u8 _padBB8[0xBB9 - 0xBB8];
    u8 characterId;
    u8 boardIndex;
    u8 _padBBB[0xBBD - 0xBBB];
    u8 behaviorMode;
    u8 behaviorPhase;
    u8 behaviorStep;
    u8 behaviorCounter;
    u8 _padBC1[0xBC9 - 0xBC1];
    u8 unkBC9;
    u8 unkBCA;
    u8 _padBCB[1];
    u8 unkBCC;
    u8 _padBCD[0xBDB - 0xBCD];
    u8 unkBDB;
} IceLandBossAttackArg;

typedef struct {
    void *unk0;
    void *unk4;
    void *unk8;
    void *unkC;
    void *unk10;
    void *unk14;
    void *unk18;
    void *unk1C;
    void *unk20;
    void *unk24;
    void *unk28;
    void *unk2C;
    u8 padding[12];
} Unk0x3CElem;

typedef struct {
    Transform3D unk38;
    void *unk58;
    s32 unk5C;
    s32 unk60;
    s32 unk64;
} Unk0x3CElemExtra;

typedef struct {
    Unk0x3CElem unk0_3C[12];
    u8 padding3C[0x434 - (12 * 60)];
    s32 unk434;
    s32 unk438;
    s32 unk43C;
    s32 unk440;
    s32 unk444;
    s32 unk448;
    Vec3i velocity;
    s32 unk458;
    s32 unk45C;
    s32 unk460;
    u8 padding464[0x4];
    s32 unk468;
    s32 unk46C;
    void *unk470;
    s32 unk474;
    s32 unk478;
    s32 unk47C;
    s32 unk480;
    u8 padding484[0x4];
    BoneAnimationStateIndexed unk488[12];
    u8 padding888[0x4EC - 0x4 - 0x48 * 12];
    Transform3D unk970;
    Transform3D unk990;
    Transform3D unk9B0;
    u8 paddingA88[0xA8C - 0x9D0];
    u16 leanAnimIndex;
    s16 unkA8E;
    s16 unkA90;
    s16 unkA92;
    s16 rotY;
    s16 unkA96;
    s16 unkA98;
    s16 unkA9A;
    u8 padding9E[0x2];
    s16 unkA9E;
    s32 unkAA0;
    s32 maxSpeedCap;
    s32 unkAA8;
    s32 unkAAC;
    s32 unkAB0;
    s32 unkAB4;
    s32 unkAB8;
    s32 unkABC;
    u8 unkAC0;
    u8 unkAC1;
    s16 unkAC2;
    s16 unkAC4;
    u8 paddingAC6[0x2];
    s32 unkAC8;
    s32 unkACC;
    s32 unkAD0;
    s32 unkAD4[3];
    s32 unkAE0;
    Vec3i unkAE4[6];
    s32 unkB2C;
    s32 unkB30;
    s32 unkB34;
    s32 unkB38;
    s32 unkB3C;
    s32 unkB40;
    s32 unkB44;
    s32 unkB48;
    s32 unkB4C;
    s32 unkB50;
    void *unkB54;
    s32 unkB58;
    s32 unkB5C;
    s32 unkB60;
    s32 unkB64;
    u8 unkB68;
    u8 paddingB69[0x3];
    s32 unkB6C;
    s32 unkB70;
    u16 unkB74;
    u8 paddingB76[0x2];
    s16 unkB78;
    s8 unkB7A;
    s8 unkB7B;
    s16 unkB7C;
    u16 unkB7E;
    s8 unkB80;
    s8 unkB81;
    u8 paddingB82[0x2];
    s32 animFlags;
    s32 unkB88;
    s32 unkB8C;
    s32 unkB90;
    u16 sectorIndex;
    u8 paddingB96[0x2];
    s16 unkB98;
    s16 boostTimer;
    s16 unkB9C;
    s16 unkB9E;
    u16 unkBA0;
    s16 unkBA2;
    u16 unkBA4;
    s16 unkBA6;
    u8 paddingBA8[0x2];
    s16 trickPoints;
    s16 trickScore;
    s16 tricksPerformedMask;
    u16 unkBB0;
    u16 unkBB2;
    u8 unkBB4;
    s8 trickCount;
    u8 spinsPerformedMask;
    u8 boneCount;
    u8 unkBB8;
    u8 characterId;
    u8 boardIndex;
    u8 unkBBB;
    u8 unkBBC;
    u8 behaviorMode;
    u8 behaviorPhase;
    u8 behaviorStep;
    u8 behaviorCounter;
    u8 unkBC1;
    u8 unkBC2;
    u8 unkBC3;
    u8 finishPosition;
    u8 unkBC5;
    u8 unkBC6;
    u8 unkBC7;
    u8 paddingBC8[0xBDB - 0xBC8];
    u8 unkBDB;
} IceLandBossArg;

typedef struct {
    u8 primaryR;
    u8 primaryG;
    u8 primaryB;
    u8 pad1;
    u8 secondaryR;
    u8 secondaryG;
    u8 secondaryB;
    u8 pad2;
} BossSurfaceColor;

typedef struct {
    u8 pad[0x38];
    s16 groundJointOffsets[6];
    u8 pad44[0xB0 - 0x44];
    s16 groundJointOffsets2[6];
    u8 padBC[0x128 - 0xBC];
    s16 groundJointOffsets3[6];
    u8 pad134[0x434 - 0x134];
    Vec3i position;
    Vec3i prevPosition;
    Vec3i velocity;
    u8 pad458[0x970 - 0x458];
    Transform3D yRotationMatrix;
    Transform3D combinedRotationMatrix;
    Transform3D zRotationMatrix;
    u8 pad9D0[0xA8E - 0x9D0];
    u16 unkA8E;
    u16 unkA90;
    u16 unkA92;
    u16 rotY;
    u8 padA96[0xAA4 - 0xA96];
    s32 targetSpeed;
    s32 currentSpeed;
    u8 padAAC[0xAC2 - 0xAAC];
    s16 behaviorTrigger;
    u8 padAC4[0xAD4 - 0xAC4];
    Vec3i transformedPos;
    u8 padAE0[0x4];
    Vec3i unkAE4;
    Vec3i unkAF0;
    Vec3i unkAFC;
    u8 padB08[0xB50 - 0xB08];
    ListNode_5AA90 sectorListNode;
    u8 padB6C[0xB84 - 0xB6C];
    s32 bossFlags;
    u8 padB88[0xBBD - 0xB88];
    u8 behaviorMode;
    u8 behaviorPhase;
    u8 behaviorStep;
    u8 behaviorCounter;
    u8 padBC1[3];
    u8 finishPosition;
} IceBossArg;

void func_800BC61C_B1B0C(Player *);

typedef s32 (*StateFunc)(void *);

// Forward declarations for function pointer arrays
s32 initIceLandBoss(IceLandBossArg *);
void dispatchIceLandBossChasePhase(IceLandBossAttackArg *);
void dispatchIceLandBossAttackPhase(IceLandBossAttackArg *);
s32 iceLandBossDefeatedBehavior(IceLandBossAttackArg *);
s32 iceLandBossChaseIntroPhase(IceLandBossAttackArg *);
s32 iceLandBossChaseAttackPhase(Player *);
s32 iceLandBossChaseExitPhase(IceLandBossAttackArg *);
s32 iceLandBossGroundProjectileAttackPhase(Player *);
s32 iceLandBossHoverAttackPhase(Player *);

s32 D_800BC9F0_B1EE0[] = {
    0x00000000, 0x00068000, 0x0003F000, 0x00000000, 0xFFF04000, 0x00018000, 0x00000000, 0xFFF04000, 0x00018000,
};

StateFunc D_800BCA14_B1F04[] = {
    (StateFunc)initIceLandBoss,
    (StateFunc)dispatchIceLandBossChasePhase,
    (StateFunc)dispatchIceLandBossAttackPhase,
    (StateFunc)iceLandBossDefeatedBehavior,
};

s32 D_800BCA24_B1F14[] = {
    0x00000000,
    0x00117000,
    0x00000000,
};

s32 D_800BCA30_B1F20[] = {
    0x00000000,
    0x00100000,
    0x00000000,
};

s32 D_800BCA3C_B1F2C[] = {
    0xFFF00000,
    0x00300000,
};

FuncPtr D_800BCA44_B1F34[] = {
    (FuncPtr)iceLandBossChaseIntroPhase,
    (FuncPtr)iceLandBossChaseAttackPhase,
    (FuncPtr)iceLandBossChaseExitPhase,
};

s32 D_800BCA50_B1F40[] = {
    0x00000000,
    0x00000000,
    0x00006000,
};

FuncPtr gIceLandBossAttackPhaseHandlers[] = {
    (FuncPtr)iceLandBossGroundProjectileAttackPhase,
    (FuncPtr)iceLandBossHoverAttackPhase,
};

Vec3i D_800BCA64[] = {
    { 0xFFE79000, 0x00000000, 0x00187000 },
    { 0x00000000, 0x00000000, 0x00187000 },
    { 0x00187000, 0x00000000, 0x00187000 },
    { 0xFFE79000, 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000, 0x00000000 },
    { 0x00187000, 0x00000000, 0x00000000 },
    { 0xFFE79000, 0x00000000, 0xFFE79000 },
    { 0x00000000, 0x00000000, 0xFFE79000 },
    { 0x00187000, 0x00000000, 0xFFE79000 },
    { 0xFFF40000, 0x00000000, 0x000C0000 },
    { 0x00000000, 0x00000000, 0x000C0000 },
    { 0x000C0000, 0x00000000, 0x000C0000 },
    { 0xFFF40000, 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000, 0x00000000 },
    { 0x000C0000, 0x00000000, 0x00000000 },
    { 0xFFF40000, 0x00000000, 0xFFF40000 },
    { 0x00000000, 0x00000000, 0xFFF40000 },
    { 0x000C0000, 0x00000000, 0xFFF40000 },
};

extern u16 D_8009ADE0_9B9E0;
extern BossSurfaceColor gBossSurfaceColors[];
extern s32 identityMatrix[];
extern s32 gControllerInputs[];

/**
 * Main update function for the Ice Land boss.
 * Handles:
 * - Velocity calculation from position delta
 * - Speed adjustment based on distance to player and flying state
 * - Behavior mode transitions
 * - Transform matrix updates
 * - Ground joint position calculations
 */
void func_800BB2B0_B07A0(IceBossArg *boss) {
    Transform3D combinedRotMatrix;
    Transform3D fullTransform;
    GameState *gameState;
    IceBossArg *player;
    s32 distanceToPlayer;
    s32 speedDiff;

    gameState = getCurrentAllocation();

    // Calculate velocity based on position change since last frame
    boss->velocity.x = boss->position.x - boss->prevPosition.x;
    boss->velocity.y = boss->position.y - boss->prevPosition.y;
    boss->velocity.z = boss->position.z - boss->prevPosition.z;
    memcpy(&boss->prevPosition, &boss->position, 0xC);

    // Get distance to player for AI speed calculation
    player = (IceBossArg *)gameState->players;
    distanceToPlayer = distance_3d(
        boss->position.x - player->position.x,
        boss->position.y - player->position.y,
        boss->position.z - player->position.z
    );

    // Set target speed based on race state and distance
    if ((boss->finishPosition == 0) & (distanceToPlayer > 0xE00000)) {
        if (boss->bossFlags & 0x400000) { // Flying mode
            boss->targetSpeed = getCharacterBoardStatParam0(0, 4) + -0x8000;
        } else if (distanceToPlayer > 0x8C00000) {
            boss->targetSpeed = 0x70000;
        } else {
            boss->targetSpeed = getCharacterBoardStatParam0(0, 0) + -0x8000;
        }
    } else {
        boss->targetSpeed = getCharacterBoardStatParam0(0, 8) + 0x18000;
    }

    // Clamp target speed to maximum
    if (boss->targetSpeed > 0x180000) {
        boss->targetSpeed = 0x180000;
    }

    // Gradually adjust current speed toward target
    speedDiff = boss->targetSpeed - boss->currentSpeed;
    if (speedDiff >= 0x1001) {
        speedDiff = 0x1000;
    }
    if (speedDiff < -0x80) {
        speedDiff = -0x80;
    }

    boss->currentSpeed = boss->currentSpeed + speedDiff;
    boss->bossFlags &= 0xFFFBFFFF; // Clear bit 22 (0x40000)

    // Check for attack trigger (0x3D)
    if (boss->behaviorMode != 3) {
        if (boss->behaviorTrigger != 0) {
            if (boss->behaviorTrigger == 0x3D) {
                boss->behaviorMode = 2;
                boss->behaviorPhase = 0;
                boss->behaviorStep = 0;
                boss->behaviorCounter = 0;
                if (boss->bossFlags & 0x400000) { // Skip to phase 1 if flying
                    boss->behaviorPhase = 1;
                }
            }
        }
    }
    boss->behaviorTrigger = 0;

    // Dispatch to current behavior mode handler
    do {
    } while (D_800BCA14_B1F04[boss->behaviorMode](boss) != 0);

    // Build transformation matrices for rendering and collision
    createZRotationMatrix(&boss->zRotationMatrix, boss->unkA92);
    createCombinedRotationMatrix(&boss->combinedRotationMatrix, boss->unkA8E, boss->unkA90);
    createYRotationMatrix(&boss->yRotationMatrix, boss->rotY);

    func_8006B084_6BC84(&boss->zRotationMatrix, &boss->combinedRotationMatrix, &combinedRotMatrix);
    func_8006B084_6BC84(&combinedRotMatrix, &boss->yRotationMatrix, &fullTransform);

    fullTransform.translation.x -= boss->yRotationMatrix.translation.x;
    fullTransform.translation.y -= boss->yRotationMatrix.translation.y;
    fullTransform.translation.z -= boss->yRotationMatrix.translation.z;

    // Transform position for collision detection
    if (boss->bossFlags & 0x400000) { // Flying mode offset
        transformVector((s16 *)D_800BCA30_B1F20, (s16 *)&fullTransform, &boss->transformedPos);
    } else {
        transformVector((s16 *)D_800BCA24_B1F14, (s16 *)&fullTransform, &boss->transformedPos);
    }
    memcpy(&boss->sectorListNode.localPos, &boss->transformedPos, 0xC);
    addCollisionSectorNodeToList(&boss->sectorListNode);
    func_800BC61C_B1B0C((Player *)boss);

    // Calculate ground joint positions for leg animation
    if (boss->bossFlags & 0x400000) { // Flying mode
        transformVector((s16 *)D_800BCA30_B1F20, (s16 *)&fullTransform, &boss->unkAE4);
    } else {
        // Transform three sets of ground joint offsets
        transformVector((s16 *)D_800BC9F0_B1EE0, boss->groundJointOffsets, &boss->unkAE4);
        boss->unkAE4.x -= boss->yRotationMatrix.translation.x;
        boss->unkAE4.y -= boss->yRotationMatrix.translation.y;
        boss->unkAE4.z -= boss->yRotationMatrix.translation.z;

        transformVector((s16 *)D_800BC9F0_B1EE0 + 6, boss->groundJointOffsets2, &boss->unkAF0);
        boss->unkAF0.x -= boss->yRotationMatrix.translation.x;
        boss->unkAF0.y -= boss->yRotationMatrix.translation.y;
        boss->unkAF0.z -= boss->yRotationMatrix.translation.z;

        transformVector((s16 *)D_800BC9F0_B1EE0 + 12, boss->groundJointOffsets3, &boss->unkAFC);
        boss->unkAFC.x -= boss->yRotationMatrix.translation.x;
        boss->unkAFC.y -= boss->yRotationMatrix.translation.y;
        boss->unkAFC.z -= boss->yRotationMatrix.translation.z;
    }
}

s32 initIceLandBoss(IceLandBossArg *arg0) {
    Vec3i waypoint1;
    Vec3i waypoint2;
    GameState *gameState;
    s32 i;
    u16 trackIdx;
    s32 assetOffset;

    gameState = getCurrentAllocation();

    // Initialize rotation matrices for bone transformations
    memcpy(&arg0->unk970, identityMatrix, sizeof(Transform3D));
    createYRotationMatrix(&arg0->unk970, arg0->rotY);
    memcpy(&arg0->unk990, identityMatrix, sizeof(Transform3D));
    memcpy(&arg0->unk9B0, identityMatrix, sizeof(Transform3D));

    // Set initial position based on boss index
    arg0->unk434 = D_800BCA3C_B1F2C[arg0->unkBB8];
    getTrackSegmentWaypoints(&gameState->gameData, 0, &waypoint1, &waypoint2);
    arg0->unk43C = waypoint1.z + 0x200000;
    trackIdx = getOrUpdatePlayerSectorIndex(arg0, &gameState->gameData, 0, (Vec3i *)&arg0->unk434);
    arg0->sectorIndex = trackIdx;
    arg0->unk438 = getTrackHeightInSector(&gameState->gameData, trackIdx, (Vec3i *)&arg0->unk434, 0x100000);
    memcpy(&arg0->unk440, &arg0->unk434, sizeof(Vec3i));

    // Zero out velocity and set initial rotation
    arg0->velocity.x = 0;
    arg0->velocity.y = 0;
    arg0->velocity.z = 0;
    arg0->rotY = 0x1000;

    // Initialize body part transforms (12 body parts, each 0x3C bytes apart)
    // Each has Transform3D at offset 0x38, asset pointer at 0x58
    for (i = 0; i < 12; i++) {
        Unk0x3CElemExtra *extra = (Unk0x3CElemExtra *)((u8 *)&arg0->unk0_3C[i] + 0x38);
        memcpy(&extra->unk38, identityMatrix, sizeof(Transform3D));
        extra->unk5C = (s32)arg0->unk0_3C[0].unk4;
        extra->unk60 = (s32)arg0->unk0_3C[0].unk8;
        assetOffset = i * 0x10;
        extra->unk64 = 0;
        extra->unk58 = (void *)(loadAssetByIndex_953B0(arg0->characterId, arg0->boardIndex) + assetOffset);
    }

    // Initialize bone animation state
    arg0->leanAnimIndex = 0;
    arg0->boneCount = getAnimationBoneCount(arg0->unk0_3C[0].unk0, 0);
    for (i = 0; i < arg0->boneCount; i++) {
        resetBoneAnimation(arg0->unk0_3C[0].unk0, arg0->leanAnimIndex, i, &arg0->unk488[i]);
    }

    // Initialize behavior state
    arg0->behaviorMode = 1;
    arg0->unkB2C = 0x240000;
    arg0->unkBB4 = 3;
    arg0->unkB54 = (void *)&arg0->unk434;
    arg0->behaviorPhase = 0;
    arg0->unkB30 = 0x11C000;
    arg0->unkB34 = 0x11C000;
    arg0->unkB64 = 0x150000;
    arg0->unkB68 = arg0->unkBB8;

    // Spawn chase camera if needed
    if (arg0->unkBC7 == 0) {
        spawnChaseCameraTask(arg0->unkBB8);
    }

    arg0->unkAA0 = ((s32 *)gameState->players)[0xAA0 / 4] - 0x10000;

    // Initialize asset offset table
    if (arg0->unk0_3C[0].unk1C != 0) {
        arg0->unk0_3C[0].unk28 = arg0->unk0_3C[0].unk1C + ((s32 *)arg0->unk0_3C[0].unk1C)[arg0->unkBB8];
    }

    arg0->unkBDB = 0xA;

    return 1;
}

void dispatchIceLandBossChasePhase(IceLandBossAttackArg *arg0) {
    D_800BCA44_B1F34[arg0->behaviorPhase](arg0);
}

void setIceBossFlyingMode(Player *arg0) {
    s32 i;
    Element0x3C *elements = (Element0x3C *)arg0;

    for (i = 0; i < 12; i++) {
        *(void **)((u8 *)elements + i * 0x3C + 0x58) =
            (void *)(loadAssetByIndex_953B0(arg0->characterId, arg0->boardIndex) + i * 16 + 0xC0);
    }

    arg0->collisionRadius = 0x100000;
    arg0->unkB2C = 0x100000;
    arg0->unkBB4 = 1;
    arg0->animFlags = arg0->animFlags | 0x400000;
}

s32 iceLandBossChaseIntroPhase(IceLandBossAttackArg *arg0) {
    s32 pad[3];
    GameState *gameState = getCurrentAllocation();

    if (gameState->raceIntroState == 0) {
        setPlayerBehaviorPhase((Player *)arg0, 1);
        return 1;
    }

    arg0->velocity.x -= arg0->velocity.x / 8;
    arg0->velocity.z -= arg0->velocity.z / 8;
    arg0->velocity.y += -0x8000;
    applyClampedVelocityToPosition((Player *)arg0);

    if (arg0->bossFlags & 0x400000) {
        advancePlayerLeanAnimationAuto(arg0, 2);
    } else {
        advancePlayerLeanAnimationAuto(arg0, 0);
    }

    return 0;
}

s32 iceLandBossChaseAttackPhase(Player *arg0) {
    Transform3D sp10;
    Vec3i sp30;
    Vec3i sp40;
    GameState *gameState;
    s16 angleDiff;
    Player *player;

    gameState = (GameState *)getCurrentAllocation();

    if (arg0->animFlags & 0x100000) {
        setPlayerBehaviorMode(arg0, 3);
        return 1;
    }

    if (arg0->animFlags & 0x80000) {
        setPlayerBehaviorPhase(arg0, 2);
        return 1;
    }

    if (gControllerInputs[1] & L_TRIG) {
        setIceBossFlyingMode(arg0);
    }

    if (arg0->behaviorStep == 0) {
        arg0->behaviorStep++;

        if (gameState->raceFrameCounter < 0x1EU) {
            arg0->unkB8C = ((randA() & 0xFF) >> 2) + 0x5A;
        } else if (arg0->animFlags & 0x400000) {
            arg0->unkB8C = randA() & 0xF;
        } else {
            arg0->unkB8C = (randA() & 0xFF) >> 1;
        }
        arg0->unkB90 = 0;
    }

    arg0->animFlags = arg0->animFlags | 0x40000;
    calculateAITargetPosition(arg0);

    angleDiff =
        computeAngleToPosition(arg0->aiTarget.x, arg0->aiTarget.z, arg0->worldPos.x, arg0->worldPos.z) - arg0->rotY;
    angleDiff = angleDiff & 0x1FFF;

    if (angleDiff >= 0x1001) {
        angleDiff = angleDiff | 0xE000;
    }

    if (angleDiff >= 0x39) {
        angleDiff = 0x38;
    }

    if (angleDiff < -0x38) {
        angleDiff = -0x38;
    }

    arg0->rotY = arg0->rotY + angleDiff;

    if (!(arg0->animFlags & 0x1)) {
        createYRotationMatrix(&arg0->unk970, arg0->rotY);
        func_8006BDBC_6C9BC((BoneAnimationState *)&arg0->unk990, &arg0->unk970, &sp10);
        transformVector3(&arg0->velocity, &sp10, &sp30);
        sp30.x = 0;
        transformVector2(&sp30, &sp10, &arg0->velocity);
        transformVector2(&D_800BCA50_B1F40, &sp10, &sp30);

        if (sp30.y > 0) {
            sp30.y = 0;
        }

        arg0->velocity.x += sp30.x;
        arg0->velocity.y += sp30.y;
        arg0->velocity.z += sp30.z;
    } else {
        arg0->velocity.x -= arg0->velocity.x / 16;
        arg0->velocity.z -= arg0->velocity.z / 16;
    }

    if (arg0->velocity.y > 0) {
        arg0->velocity.y = 0;
    }

    arg0->velocity.y -= 0x10000;
    applyClampedVelocityToPosition(arg0);
    func_800BC61C_B1B0C(arg0);

    transformVectorRelative(&gameState->players->worldPos.x, arg0->unk164, &sp40);

    angleDiff = atan2Fixed(-sp40.x, -sp40.z) & 0x1FFF;

    if (angleDiff >= 0x1000) {
        angleDiff = angleDiff | 0xE000;
    }

    if (angleDiff >= 0x81) {
        angleDiff = 0x80;
    }

    if (angleDiff < -0x80) {
        angleDiff = -0x80;
    }

    arg0->unkA9E = arg0->unkA9E + angleDiff;

    if (arg0->animFlags & 0x400000) {
        if (advancePlayerLeanAnimationAuto(arg0, 3) != 0) {
            arg0->unkB90 = 0;
        }

        arg0->unkB90++;

        if (arg0->unkB90 == 4) {
            arg0->unkB90 = 0;
            queueSoundAtPosition(&arg0->worldPos, 0x4B);
        }

        if (arg0->unkB8C == 0) {
            spawnBossHomingProjectileVariant2Task(arg0);

            if (gameState->unk86 != 0) {
                arg0->unkB8C = ((u32)(randA() & 0xFF) >> 1) + 0x1E;
            } else {
                arg0->unkB8C = (randA() & 0xFF) + 0x3C;
            }
        } else {
            arg0->unkB8C = arg0->unkB8C - 1;
        }
    } else {
        if (advancePlayerLeanAnimationAuto(arg0, 1) != 0) {
            arg0->unkB90 = 0;
        }

        arg0->unkB90 = arg0->unkB90 + 1;

        if ((arg0->unkB90 == 4) || (arg0->unkB90 == 0xC)) {
            queueSoundAtPosition(&arg0->worldPos, 0x4A);
        }

        player = gameState->players;
        if (player->behaviorFlags != 0) {
            return 0;
        }

        if (arg0->unkB8C == 0) {
            if (arg0->behaviorCounter != 0) {
                if (arg0->behaviorStep == 2) {
                    arg0->behaviorCounter = arg0->behaviorCounter - 1;
                    if (spawnPlayerGuidedStarProjectile(arg0->playerIndex) != 0) {
                        spawnGlintEffect(arg0);
                    }

                    if (arg0->behaviorCounter == 0) {
                        if (gameState->unk86 != 0) {
                            arg0->unkB8C = (randA() & 0xF) + 8;
                        } else {
                            arg0->unkB8C = ((u32)(randA() & 0xFF) >> 1) + 0x1E;
                        }
                    } else {
                        arg0->unkB8C = 0xA;
                    }
                } else {
                    arg0->behaviorCounter = arg0->behaviorCounter - 1;
                    if (spawnRandomEffectProjectile(arg0->playerIndex) != 0) {
                        spawnGlintEffect(arg0);
                    }

                    if (arg0->behaviorCounter == 0) {
                        arg0->unkB8C = ((u32)(randA() & 0xFF) >> 1) + 0x1E;
                    } else {
                        arg0->unkB8C = 0xA;
                    }
                }
            } else {
                // unkBC0 is zero - calculate distance
                if ((u32)distance_3d(
                        arg0->worldPos.x - player->worldPos.x,
                        arg0->worldPos.y - player->worldPos.y,
                        arg0->worldPos.z - player->worldPos.z
                    ) <= 0xDFFFFF) {
                    // Close range
                    if (spawnPlayerGuidedStarProjectile(arg0->playerIndex) != 0) {
                        spawnGlintEffect(arg0);
                    }

                    arg0->behaviorCounter = randA() & 0x3;

                    if (arg0->behaviorCounter != 0) {
                        arg0->behaviorStep = 2;
                        arg0->unkB8C = 0xA;
                    } else {
                        if (gameState->unk86 != 0) {
                            arg0->unkB8C = (randA() & 0xF) + 8;
                        } else {
                            arg0->unkB8C = ((u32)(randA() & 0xFF) >> 1) + 0x3C;
                        }
                    }
                } else {
                    // Far range
                    if (spawnRandomEffectProjectile(arg0->playerIndex) != 0) {
                        spawnGlintEffect(arg0);
                    }

                    if (gameState->unk86 != 0) {
                        arg0->unkB8C = (randA() & 0xF) + 8;
                    } else {
                        arg0->unkB8C = ((u32)(randA() & 0xFF) >> 1) + 0x3C;
                    }
                }
            }
        } else {
            arg0->unkB8C = arg0->unkB8C - 1;
        }
    }

    return 0;
}

s32 iceLandBossChaseExitPhase(IceLandBossAttackArg *arg0) {
    s32 pad[3];

    getCurrentAllocation();

    arg0->velocity.x -= arg0->velocity.x / 8;
    arg0->velocity.z -= arg0->velocity.z / 8;
    arg0->velocity.y += -0x8000;
    applyClampedVelocityToPosition((Player *)arg0);

    if (arg0->bossFlags & 0x400000) {
        advancePlayerLeanAnimationAuto(arg0, 2);
    } else {
        advancePlayerLeanAnimationAuto(arg0, 0);
    }

    return 0;
}

void dispatchIceLandBossAttackPhase(IceLandBossAttackArg *arg0) {
    gIceLandBossAttackPhaseHandlers[arg0->behaviorPhase](arg0);
}

s32 iceLandBossGroundProjectileAttackPhase(Player *boss) {
    Transform3D rotMatrix;
    Vec3i tempVec;
    s32 pad[4];
    s16 angleDiff;
    s32 i;

    getCurrentAllocation();

    if (boss->behaviorStep == 0) {
        boss->leanAnimIndex = 0xFFFF;
        boss->unkB8C = 0;
        boss->behaviorStep += 1;

        if (!(boss->animFlags & 0x80000)) {
            if (boss->unkBDB == 0) {
                queueSoundAtPosition(&boss->worldPos, 0x4C);
            } else {
                boss->unkBDB -= 1;
                if (boss->unkBDB == 0) {
                    queueSoundAtPosition(&boss->worldPos, 0x4C);
                }
            }
        }
    }

    boss->unkB8C += 1;

    if ((boss->unkB8C == 5) || (boss->unkB8C == 0xF)) {
        spawnBossHomingProjectileVariant1Task(boss);
        spawnBossHomingProjectileVariant1Task(boss);
        spawnBossHomingProjectileVariant1Task(boss);
        spawnBossHomingProjectileVariant1Task(boss);
    }

    boss->behaviorFlags = 0x200;
    calculateAITargetPosition(boss);

    angleDiff =
        (computeAngleToPosition(boss->aiTarget.x, boss->aiTarget.z, boss->worldPos.x, boss->worldPos.z) - boss->rotY) &
        0x1FFF;

    if (angleDiff >= 0x1001) {
        angleDiff = angleDiff | 0xE000;
    }

    if (angleDiff >= 0x39) {
        angleDiff = 0x38;
    }

    if (angleDiff < -0x38) {
        angleDiff = -0x38;
    }

    boss->rotY = boss->rotY + angleDiff;

    if (!(boss->animFlags & 1)) {
        createYRotationMatrix(&boss->unk970, boss->rotY);
        func_8006BDBC_6C9BC((BoneAnimationState *)&boss->unk990, &boss->unk970, &rotMatrix);
        transformVector3(&boss->velocity, &rotMatrix, &tempVec);
        tempVec.x = 0;
        transformVector2(&tempVec, &rotMatrix, &boss->velocity);
        transformVector2(&D_800BCA50_B1F40, &rotMatrix, &tempVec);

        if (tempVec.y > 0) {
            tempVec.y = 0;
        }

        boss->velocity.x += tempVec.x;
        boss->velocity.y += tempVec.y;
        boss->velocity.z += tempVec.z;
    } else {
        boss->velocity.x -= boss->velocity.x / 16;
        boss->velocity.z -= boss->velocity.z / 16;
    }

    if (boss->velocity.y > 0) {
        boss->velocity.y = 0;
    }

    boss->velocity.y += -0x10000;
    applyClampedVelocityToPosition(boss);

    if (advancePlayerLeanAnimation(boss, 4) != 0) {
        boss->behaviorFlags = 0;
        boss->behaviorMode = 1;
        boss->behaviorPhase = 1;
        boss->behaviorStep = 0;
        boss->behaviorCounter = 0;

        if (boss->unkBDB == 0) {
            for (i = 0; i < 0x1E; i++) {
                spawnBossHomingProjectileVariant1Task(boss);
            }
            memcpy(&tempVec, &boss->worldPos, sizeof(Vec3i));
            tempVec.y += 0x300000;
            spawnSparkleEffectWithPlayer(&tempVec, boss->playerIndex);
            setIceBossFlyingMode(boss);
            boss->unkBDB = 3;
            advancePlayerLeanAnimationAuto(boss, 2);
        }
    }

    return 0;
}

s32 iceLandBossHoverAttackPhase(Player *arg0) {
    u8 savedStep;
    u8 hoverCount;

    savedStep = arg0->behaviorStep;
    if (savedStep == 0) {
        arg0->behaviorStep = savedStep + 1;
        arg0->velocity.y = 0x80000;
        if (!(arg0->animFlags & 0x80000)) {
            if (arg0->unkBDB != 0) {
                arg0->unkBDB = arg0->unkBDB - 1;
            }
        }
    }

    arg0->behaviorFlags = 0x200;
    arg0->velocity.x = 0;
    arg0->velocity.z = 0;
    arg0->velocity.y = arg0->velocity.y + (-0x8000);

    applyClampedVelocityToPosition(arg0);
    advancePlayerLeanAnimationAuto(arg0, 5);

    if ((D_8009ADE0_9B9E0 & 0xF) == 0) {
        spawnBossHomingProjectileVariant2Task(arg0);
    }

    if (arg0->velocity.y < 0) {
        if (!(arg0->animFlags & 0x1)) {
            hoverCount = arg0->unkBDB;
            arg0->behaviorFlags = 0;
            arg0->behaviorMode = 1;
            arg0->behaviorPhase = 1;
            arg0->behaviorStep = 0;
            arg0->behaviorCounter = 0;

            if (hoverCount == 0) {
                arg0->animFlags = arg0->animFlags | 0x100000;
            }

            return 0;
        }
    }

    return 0;
}

s32 iceLandBossDefeatedBehavior(IceLandBossAttackArg *arg0) {
    s32 pad[3];

    getCurrentAllocation();

    arg0->velocity.y += -0x8000;
    applyClampedVelocityToPosition((Player *)arg0);

    if (arg0->behaviorStep == 0) {
        if (advancePlayerLeanAnimation(arg0, 6) != 0) {
            arg0->behaviorStep = arg0->behaviorStep + 1;
        }
        return 0;
    }

    advancePlayerLeanAnimationAuto(arg0, 7);

    return 0;
}

void updateIceLandBossPositionAndTrackCollision(IceLandBossAttackArg *boss) {
    s32 pad[8];
    Vec3i collisionOffset;
    s32 pad2[8];
    GameState *gameState;
    GameDataLayout *gameData;
    u16 newSectorIndex;

    gameState = getCurrentAllocation();
    memcpy(boss->unk984, &boss->unk434, 0xC);
    gameData = &gameState->gameData;
    newSectorIndex = getOrUpdatePlayerSectorIndex(boss, gameData, boss->sectorIndex, &boss->unk434);
    boss->sectorIndex = newSectorIndex;
    func_80060CDC_618DC(gameData, newSectorIndex, &boss->unk434, 0x187000, &collisionOffset);
    boss->unk434.x = boss->unk434.x + collisionOffset.x;
    boss->unk434.z = boss->unk434.z + collisionOffset.z;
    func_8005C868_5D468(boss);

    if (boss->bossFlags & 0x10000) {
        boss->unkBC9 = 0;
    } else {
        func_8005CFFC_5DBFC(gameData, boss->sectorIndex, &boss->unk434, &boss->unkBC9, &boss->unkBCC);
        boss->unkBCA = boss->unkBC9 >> 4;
        boss->unkBC9 = boss->unkBC9 & 0xF;
    }
}

extern void *getIndexedAnimationDataPtr(void *, s16);

typedef struct {
    u8 boneIndex;
    u8 parentBone;
} BoneHierarchyEntry;

void func_800BC61C_B1B0C(Player *arg0) {
    Transform3D sp10;
    Transform3D sp30;
    BoneHierarchyEntry *animData;
    s32 i;
    u8 parentBone;
    void *animDataPtr;
    Transform3D *temp;

    animDataPtr = &arg0->unk9B0.animation_data;
    animData = (BoneHierarchyEntry *)getIndexedAnimationDataPtr(arg0->unk0, (s16)arg0->leanAnimIndex);
    func_8006B084_6BC84(&arg0->unk990, &arg0->unk970, animDataPtr);
    func_8006B084_6BC84(&arg0->unk9B0, animDataPtr, &arg0->padding2a_3[0x480]);

    for (i = 0; i < arg0->leanBoneCount; i++) {
        if (animData[i].parentBone == 0xFF) {
            if (arg0->behaviorFlags & 0x10) {
                memcpy(&sp30, &identityMatrix, sizeof(Transform3D));
                sp30.m[1][1] = arg0->unkB9E;
                func_8006B084_6BC84(&arg0->unk488[animData[i].boneIndex].prev_position, &sp30, &sp10);
                func_8006B084_6BC84(&sp10, &arg0->padding2a_3[0x480], animData[i].boneIndex * 0x3C + 0x38 + (u8 *)arg0);
            } else {
                func_8006B084_6BC84(
                    &arg0->unk488[animData[i].boneIndex].prev_position,
                    &arg0->padding2a_3[0x480],
                    animData[i].boneIndex * 0x3C + 0x38 + (u8 *)arg0
                );
            }
        } else {
            func_8006B084_6BC84(
                &arg0->unk488[animData[i].boneIndex].prev_position,
                animData[i].parentBone * 0x3C + 0x38 + (u8 *)arg0,
                animData[i].boneIndex * 0x3C + 0x38 + (u8 *)arg0
            );
        }
    }

    temp = &sp10;
    memcpy(temp, &arg0->unk164, sizeof(Transform3D));
    createYRotationMatrix(&sp30, (u16)arg0->unkA9E);
    func_8006BDBC_6C9BC((BoneAnimationState *)&sp30, &sp10, &arg0->unk164);
}

void renderIceLandBossWithSurfaceColors(Player *arg0) {
    s32 pad[36];
    s32 i;
    s32 index;

    getCurrentAllocation();
    func_800BC61C_B1B0C(arg0);

    index = arg0->unkBCC >> 4;

    if (index == 0) {
        for (i = 0; i < 4; i++) {
            enqueuePreLitMultiPartDisplayList(i, (enqueueMultiPartDisplayList_arg1 *)&arg0->unk38, arg0->leanBoneCount);
        }
    } else {
        arg0->unk6C = gBossSurfaceColors[index].primaryR;
        arg0->unk6D = gBossSurfaceColors[index].primaryG;
        arg0->unk6E = gBossSurfaceColors[index].primaryB;
        arg0->unk70 = gBossSurfaceColors[index].secondaryR;
        arg0->unk71 = gBossSurfaceColors[index].secondaryG;
        arg0->unk72 = gBossSurfaceColors[index].secondaryB;

        for (i = 0; i < 4; i++) {
            enqueueMultiPartDisplayList(i, (enqueueMultiPartDisplayList_arg1 *)&arg0->unk38, arg0->leanBoneCount);
        }
    }
}

/**
 * Updates ground contact positions for the Ice Land boss's 9 joints.
 * For each joint, computes X/Z world position from local offsets,
 * then finds the terrain height at that position.
 * Uses different joint offset arrays based on whether the boss is flying or on ground.
 * Enqueues debug callbacks to render joint positions.
 */
void updateIceLandBossJointPositions(Player *boss) {
    GameState *gameState;
    GameDataLayout *gameData;
    s32 jointIndex;
    s32 flyingFlag;
    s32 jointOffset;
    Vec3i *jointPos;
    u16 sectorIndex;

    gameState = getCurrentAllocation();
    jointIndex = 0;
    flyingFlag = 0x400000;
    gameData = &gameState->gameData;

    for (jointIndex = 0; jointIndex < 9; jointIndex++) {
        if (boss->animFlags & flyingFlag) {
            boss->jointPositions[jointIndex].x = boss->unk970.translation.x + D_800BCA64[jointIndex + 9].x;
            boss->jointPositions[jointIndex].z = boss->unk970.translation.z + D_800BCA64[jointIndex + 9].z;
        } else {
            boss->jointPositions[jointIndex].x = boss->unk970.translation.x + D_800BCA64[jointIndex].x;
            boss->jointPositions[jointIndex].z = boss->unk970.translation.z + D_800BCA64[jointIndex].z;
        }

        jointPos = &boss->jointPositions[jointIndex];
        sectorIndex = getOrUpdatePlayerSectorIndex((void *)boss, gameData, boss->sectorIndex, jointPos);
        boss->jointPositions[jointIndex].y = getTrackHeightInSector(gameData, sectorIndex, jointPos, 0x100000);
    }

    boss->unkBC1 = 1;

    for (jointIndex = 0; jointIndex < 4; jointIndex++) {
        debugEnqueueCallback(jointIndex, 1, func_800B9500_A93B0, (void *)boss);
    }
}
