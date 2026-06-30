#include "animation/bone_animation.h"
#include "audio/audio.h"
#include "common.h"
#include "data/asset_metadata.h"
#include "gamestate.h"
#include "graphics/displaylist.h"
#include "graphics/graphics.h"
#include "math/geometry.h"
#include "math/rand.h"
#include "race/ai_pathfinding.h"
#include "race/character_stats.h"
#include "race/race_hud.h"
#include "race/race_main.h"
#include "race/track_collision.h"
#include "race/track_height.h"
#include "system/task_scheduler.h"
#include "text/text_elements.h"

#define CHECKPOINT_DATA_ROW 214

typedef void (*FuncPtr)(void *);
typedef s32 (*StateFunc)(void *);

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
    s16 unk0;
    s16 unk2;
} D_800BC468_ACC98_type;

extern BossSurfaceColor gBossSurfaceColors[];
extern D_800BC468_ACC98_type D_800BC468_ACC98[];
extern Vec3i gCrazyJungleBossCheckpointOffsets[];
extern s32 D_800BBA7C_AC2AC[][3];
extern s32 D_800BBA84_AC2B4[][3];

extern s32 getIndexedAnimationDataPtr(void *, s16);

s32 initCrazyJungleBoss(Player *arg0);
void dispatchCrazyJungleBossChasePhase(Player *arg0);
void dispatchCrazyJungleBossHoverPhase(Player *arg0);
s32 crazyJungleBossChaseIntroPhase(Player *arg0);
s32 crazyJungleBossChaseAttackPhase(Player *arg0);
s32 crazyJungleBossChaseExitPhase(Player *arg0);
s32 crazyJungleBossHoverAttackPhase(Player *arg0);
s32 crazyJungleBossHoverJumpPhase(Player *arg0);
void updateCrazyJungleBossLeanBoneTransforms(Player *arg0);

StateFunc D_800BC440_ACC70[] = {
    (StateFunc)initCrazyJungleBoss,
    (StateFunc)dispatchCrazyJungleBossChasePhase,
    (StateFunc)dispatchCrazyJungleBossHoverPhase,
};

s32 D_800BC44C_ACC7C[] = { 0xFFF00000, 0x00300000 };

FuncPtr gCrazyJungleBossChasePhaseHandlers[] = {
    (FuncPtr)crazyJungleBossChaseIntroPhase,
    (FuncPtr)crazyJungleBossChaseAttackPhase,
    (FuncPtr)crazyJungleBossChaseExitPhase,
};

FuncPtr gCrazyJungleBossHoverPhaseHandlers[] = {
    (FuncPtr)crazyJungleBossHoverAttackPhase,
    (FuncPtr)crazyJungleBossHoverJumpPhase,
};

D_800BC468_ACC98_type D_800BC468_ACC98[] = {
    { 0x003C, 0x0200 },
    { 0x0001, 0x0400 },
    { 0x0001, 0x0800 },
    { 0x0001, 0x1000 },
    { 0x0001, 0x1400 },
    { 0x0001, 0x0800 },
    { 0x0001, 0x1000 },
    { 0x0001, 0x1800 },
    { 0xFFFF, 0x2000 },
};

Vec3i gCrazyJungleBossCheckpointOffsets[] = {
    { 0xFFE79000, 0x00000000, 0x00187000 },
    { 0x00000000, 0x00000000, 0x00187000 },
    { 0x00187000, 0x00000000, 0x00187000 },
    { 0xFFE79000, 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000, 0x00000000 },
    { 0x00187000, 0x00000000, 0x00000000 },
    { 0xFFE79000, 0x00000000, 0xFFE79000 },
    { 0x00000000, 0x00000000, 0xFFE79000 },
    { 0x00187000, 0x00000000, 0xFFE79000 },
};

void updateCrazyJungleBoss(Player *arg0) {
    Transform3D sp10;
    Transform3D sp30;
    volatile u8 pad[8];
    GameState *alloc;
    Player *player;
    s32 dist;
    s32 diff;
    s32 i;
    s32 count;
    s32 loopBound;

    alloc = getCurrentAllocation();
    calculateAITargetPosition(arg0);

    arg0->velocity.x = arg0->worldPos.x - arg0->prevWorldPosX;
    arg0->velocity.y = arg0->worldPos.y - arg0->prevWorldPosY;
    arg0->velocity.z = arg0->worldPos.z - arg0->prevWorldPosZ;
    memcpy(&arg0->prevWorldPosX, &arg0->worldPos, sizeof(Vec3i));

    if (arg0->sectorIndex < 0x16) {
        arg0->maxSpeedCap = 0x180000;
    } else {
        player = alloc->players;
        dist = distance_3d(
            arg0->worldPos.x - player->worldPos.x,
            arg0->worldPos.y - player->worldPos.y,
            arg0->worldPos.z - player->worldPos.z
        );

        if (alloc->unk86 != 0) {
            if ((arg0->finishPosition == 0) & (dist > 0xE00000)) {
                arg0->maxSpeedCap = getCharacterBoardStatParam0(0, 8) + 0x40000;
            } else {
                arg0->maxSpeedCap = 0x180000;
            }
        } else {
            if ((arg0->finishPosition == 0) & (dist > 0xE00000)) {
                arg0->maxSpeedCap = getCharacterBoardStatParam0(0, 6) + -0x8000;
            } else {
                arg0->maxSpeedCap = getCharacterBoardStatParam0(0, 6) + 0x18000;
            }
        }
    }

    if (arg0->maxSpeedCap > 0x180000) {
        arg0->maxSpeedCap = 0x180000;
    }

    count = arg0->slowdownLevel;
    i = 0;
    if (count > 0) {
        loopBound = count;
        do {
            arg0->maxSpeedCap = arg0->maxSpeedCap - (arg0->maxSpeedCap >> 2);
            i++;
        } while (i < loopBound);
    }

    diff = arg0->maxSpeedCap - arg0->smoothedSpeedCap;
    if (diff >= 0x101) {
        diff = 0x100;
    }
    if (diff < -0x80) {
        diff = -0x80;
    }
    arg0->smoothedSpeedCap = arg0->smoothedSpeedCap + diff;
    arg0->animFlags &= 0xFFFBFFFF;

    if (arg0->behaviorMode != 3) {
        if (arg0->hitReactionState != 0) {
            if (arg0->hitReactionState != 0x3D) {
                if (arg0->hitReactionState == 0x3E) {
                    arg0->behaviorMode = 2;
                    arg0->behaviorPhase = 0;
                    arg0->behaviorStep = 0;
                    arg0->behaviorCounter = 0;
                }
            } else {
                arg0->behaviorMode = 2;
                arg0->behaviorPhase = 1;
                arg0->behaviorStep = 0;
                arg0->behaviorCounter = 0;
            }
        }
    }
    arg0->hitReactionState = 0;

    do {
    } while (D_800BC440_ACC70[arg0->behaviorMode](arg0) != 0);

    createZRotationMatrix(&arg0->tiltTransform, arg0->rollAngle);
    createCombinedRotationMatrix(&arg0->orientationTransform, arg0->pitchAngle, arg0->steeringAngle);
    createYRotationMatrix(&arg0->headingTransform, arg0->rotY);

    composeTransform3D(&arg0->tiltTransform, &arg0->orientationTransform, &sp10);
    composeTransform3D(&sp10, &arg0->headingTransform, &sp30);

    sp30.translation.x -= arg0->headingTransform.translation.x;
    sp30.translation.y -= arg0->headingTransform.translation.y;
    sp30.translation.z -= arg0->headingTransform.translation.z;

    transformVector((s16 *)(alloc->unk48 + 0xF0), (s16 *)&sp30, &arg0->collisionOffset);
    memcpy(&arg0->collisionListNode.localPos, &arg0->collisionOffset, sizeof(Vec3i));
    addCollisionSectorNodeToList(&arg0->collisionListNode);
    updateCrazyJungleBossLeanBoneTransforms(arg0);

    transformVector(
        (s16 *)(alloc->unk48 + 0xFC),
        (s16 *)arg0->boneDisplayObjects[0].transform.m,
        &arg0->extraCollisionOffsets[0]
    );
    arg0->extraCollisionOffsets[0].x -= arg0->headingTransform.translation.x;
    arg0->extraCollisionOffsets[0].y -= arg0->headingTransform.translation.y;
    arg0->extraCollisionOffsets[0].z -= arg0->headingTransform.translation.z;

    transformVector(
        (s16 *)(alloc->unk48 + 0x108),
        (s16 *)arg0->boneDisplayObjects[13].transform.m,
        &arg0->extraCollisionOffsets[1]
    );
    arg0->extraCollisionOffsets[1].x -= arg0->headingTransform.translation.x;
    arg0->extraCollisionOffsets[1].y -= arg0->headingTransform.translation.y;
    arg0->extraCollisionOffsets[1].z -= arg0->headingTransform.translation.z;

    transformVector(
        (s16 *)(alloc->unk48 + 0x114),
        (s16 *)arg0->boneDisplayObjects[14].transform.m,
        &arg0->extraCollisionOffsets[2]
    );
    arg0->extraCollisionOffsets[2].x -= arg0->headingTransform.translation.x;
    arg0->extraCollisionOffsets[2].y -= arg0->headingTransform.translation.y;
    arg0->extraCollisionOffsets[2].z -= arg0->headingTransform.translation.z;

    transformVector(
        (s16 *)(alloc->unk48 + 0x120),
        (s16 *)arg0->boneDisplayObjects[15].transform.m,
        &arg0->extraCollisionOffsets[3]
    );
    arg0->extraCollisionOffsets[3].x -= arg0->headingTransform.translation.x;
    arg0->extraCollisionOffsets[3].y -= arg0->headingTransform.translation.y;
    arg0->extraCollisionOffsets[3].z -= arg0->headingTransform.translation.z;

    transformVector(
        (s16 *)(alloc->unk48 + 0x12C),
        (s16 *)arg0->boneDisplayObjects[3].transform.m,
        &arg0->extraCollisionOffsets[4]
    );
    arg0->extraCollisionOffsets[4].x -= arg0->headingTransform.translation.x;
    arg0->extraCollisionOffsets[4].y -= arg0->headingTransform.translation.y;
    arg0->extraCollisionOffsets[4].z -= arg0->headingTransform.translation.z;

    transformVector(
        (s16 *)(alloc->unk48 + 0x138),
        (s16 *)arg0->boneDisplayObjects[6].transform.m,
        &arg0->extraCollisionOffsets[5]
    );
    arg0->extraCollisionOffsets[5].x -= arg0->headingTransform.translation.x;
    arg0->extraCollisionOffsets[5].y -= arg0->headingTransform.translation.y;
    arg0->extraCollisionOffsets[5].z -= arg0->headingTransform.translation.z;
}

s32 initCrazyJungleBoss(Player *arg0) {
    Vec3i waypoint1;
    Vec3i waypoint2;
    GameState *gameState;
    s32 i;
    u16 trackIdx;
    u8 *elem;

    gameState = getCurrentAllocation();

    // Initialize rotation matrices
    memcpy(&arg0->headingTransform, &identityMatrix, sizeof(Transform3D));
    createYRotationMatrix(&arg0->headingTransform, arg0->rotY);
    memcpy(&arg0->orientationTransform, &identityMatrix, sizeof(Transform3D));
    memcpy(&arg0->tiltTransform, &identityMatrix, sizeof(Transform3D));

    // Set initial position based on boss index
    arg0->worldPos.x = D_800BC44C_ACC7C[arg0->playerIndex];
    getTrackSegmentWaypoints((TrackGeometryData *)&gameState->gameData, 0, &waypoint1, &waypoint2);

    arg0->worldPos.z = waypoint1.z + 0x200000;

    // Initialize track/sector info
    trackIdx = getOrUpdatePlayerSectorIndex(arg0, &gameState->gameData, 0, &arg0->worldPos);
    arg0->sectorIndex = trackIdx;
    arg0->worldPos.y = getTrackHeightInSector(&gameState->gameData, trackIdx, &arg0->worldPos, 0x100000);

    memcpy(&arg0->prevWorldPosX, &arg0->worldPos, sizeof(Vec3i));

    // Zero out velocity
    arg0->velocity.x = 0;
    arg0->velocity.y = 0;
    arg0->velocity.z = 0;

    arg0->rotY = 0x1000;

    // Initialize body part elements (17 elements, each 0x3C bytes)
    // Each element contains Transform3D at offset 0x38, asset pointer at 0x58
    for (i = 0; i < 17; i++) {
        elem = (u8 *)arg0 + i * 0x3C;
        memcpy(elem + 0x38, &identityMatrix, sizeof(Transform3D));
        *(s32 *)(elem + 0x5C) = (s32)arg0->unk4;
        *(s32 *)(elem + 0x60) = (s32)arg0->unk8;
        *(s32 *)(elem + 0x64) = 0;
        *(void **)(elem + 0x58) = (void *)&loadAssetByIndex_953B0(arg0->characterId, arg0->boardType)[i];
    }

    arg0->leanAnimIndex = 0;

    // Get number of bones and reset animations
    arg0->leanBoneCount = getAnimationBoneCount(arg0->unk0, 0);

    for (i = 0; i < arg0->leanBoneCount; i++) {
        resetBoneAnimation(arg0->unk0, arg0->leanAnimIndex, i, &arg0->unk488[i]);
    }

    // Initialize behavior state
    arg0->behaviorMode = 1;
    arg0->collisionRadius = 0xA0000;
    arg0->extraCollisionRadii = 0x240000;
    arg0->unkBB4 = 6;
    arg0->behaviorPhase = 0;
    arg0->unkB30 = 0x174000;
    arg0->unkB34 = 0xDC000;
    arg0->unkB38 = 0xDC000;
    arg0->unkB3C = 0x148000;
    arg0->unkB40 = 0x148000;
    arg0->collisionListNode.posPtr = &arg0->worldPos;
    arg0->collisionListNode.radius = 0x15E000;
    arg0->collisionListNode.id = arg0->playerIndex;

    // Spawn chase camera if needed
    if (arg0->isBossRacer == 0) {
        spawnChaseCameraTask(arg0->playerIndex);
    }

    arg0->baseMaxSpeed = gameState->players->baseMaxSpeed - 0x10000;

    if (arg0->unk1C != NULL) {
        arg0->aiPathData = (void *)((s32)arg0->unk1C + ((s32 *)arg0->unk1C)[arg0->playerIndex]);
    }

    return 1;
}

void dispatchCrazyJungleBossChasePhase(Player *arg0) {
    gCrazyJungleBossChasePhaseHandlers[arg0->behaviorPhase](arg0);
}

s32 crazyJungleBossChaseIntroPhase(Player *arg0) {
    s32 pad[3];
    GameState *gameState = getCurrentAllocation();

    if (gameState->raceIntroState == 0) {
        setPlayerBehaviorPhase(arg0, 1);
        return 1;
    }

    arg0->velocity.x -= arg0->velocity.x / 8;
    arg0->velocity.z -= arg0->velocity.z / 8;
    arg0->velocity.y += -0x8000;
    applyClampedVelocityToPosition(arg0);
    advancePlayerLeanAnimationAuto(arg0, 0);

    return 0;
}

s32 crazyJungleBossChaseAttackPhase(Player *arg0) {
    Transform3D sp10;
    Vec3i sp30;
    GameState *gameState;
    s16 clampedAngle;
    s16 angleDiff;
    u16 currentAngle;
    u16 newAngle;
    s32 temp;
    s32 temp2;

    gameState = getCurrentAllocation();

    if (arg0->unkBC6 != 0) {
        setPlayerBehaviorPhase(arg0, 2);
        return 1;
    }

    arg0->animFlags |= 0x40000;

    if (arg0->behaviorStep == 0) {
        if (gameState->raceFrameCounter < 0x1EU) {
            arg0->unkB8C = ((randA() & 0xFF) >> 2) + 0x5A;
        } else {
            arg0->unkB8C += (randA() & 0xFF) >> 1;
        }
        arg0->unkB90 = 0;
        arg0->leanAnimIndex = 0xFFFF;
        arg0->behaviorStep++;
    }

    clampedAngle = computeAngleToPosition(arg0->aiTarget.x, arg0->aiTarget.z, arg0->worldPos.x, arg0->worldPos.z);
    currentAngle = arg0->rotY;
    clampedAngle = (clampedAngle - currentAngle) & 0x1FFF;
    if (clampedAngle >= 0x1001) {
        clampedAngle = clampedAngle | 0xE000;
    }
    if (clampedAngle >= 0x39) {
        clampedAngle = 0x38;
    }
    if (clampedAngle < -0x38) {
        clampedAngle = -0x38;
    }
    arg0->rotY = currentAngle + clampedAngle;

    if (!(arg0->animFlags & 1)) {
        createYRotationMatrix(&arg0->headingTransform, arg0->rotY);
        func_8006BDBC_6C9BC((&arg0->orientationTransform), &arg0->headingTransform, &sp10);
        transformVector3(&arg0->velocity, &sp10, &sp30);
        sp30.x = 0;
        transformVector2(&sp30, &sp10, &arg0->velocity);
        transformVector2((s32 *)(gameState->unk48 + 0x144), &sp10, &sp30);
        if (sp30.y > 0) {
            sp30.y = 0;
        }
        arg0->velocity.x += sp30.x;
        arg0->velocity.y += sp30.y;
        arg0->velocity.z += sp30.z;
    } else {
        temp = arg0->velocity.x;
        arg0->velocity.x = temp - (temp / 16);
        temp2 = arg0->velocity.z;
        arg0->velocity.z = temp2 - (temp2 / 16);
    }

    if (arg0->velocity.y > 0) {
        arg0->velocity.y = 0;
    }
    arg0->velocity.y += -0x10000;

    applyClampedVelocityToPosition(arg0);
    if (advancePlayerLeanAnimationAuto(arg0, 1) != 0) {
        arg0->unkB90 = 0;
    }

    arg0->unkB90++;
    if (arg0->unkB90 == 4 || arg0->unkB90 == 0xC) {
        queueSoundAtPosition(&arg0->worldPos, 0x49);
    }

    if (arg0->unkB8C == 0) {
        spawnBossHomingProjectileTask(arg0);
        if (gameState->unk86 != 0) {
            arg0->unkB8C = arg0->unkB8C + (0xF + ((randA() & 0xFF) >> 4));
        } else {
            arg0->unkB8C = arg0->unkB8C + (0xF + ((randA() & 0xFF) >> 2));
        }
    } else {
        arg0->unkB8C--;
    }

    return 0;
}

s32 crazyJungleBossChaseExitPhase(Player *arg0) {
    s32 pad[3];
    getCurrentAllocation();

    arg0->velocity.x -= arg0->velocity.x / 8;
    arg0->velocity.z -= arg0->velocity.z / 8;
    arg0->velocity.y += -0x8000;
    applyClampedVelocityToPosition(arg0);
    advancePlayerLeanAnimationAuto(arg0, 0);

    return 0;
}

void dispatchCrazyJungleBossHoverPhase(Player *arg0) {
    gCrazyJungleBossHoverPhaseHandlers[arg0->behaviorPhase](arg0);
}

s32 crazyJungleBossHoverAttackPhase(Player *arg0) {
    if (arg0->behaviorStep == 0) {
        s32 temp = arg0->velocity.y;
        arg0->behaviorStep++;
        arg0->unkB8C = 1;
        if (temp > 0) {
            arg0->velocity.y = 0;
        }
        queueSoundAtPosition(&arg0->worldPos, 0x21);
    }

    arg0->unkB8C--;
    if (arg0->unkB8C == 0) {
        arg0->unkB8C = D_800BC468_ACC98[arg0->behaviorCounter].unk0;
        arg0->squashStretchScale = D_800BC468_ACC98[arg0->behaviorCounter].unk2;
        arg0->behaviorCounter++;
    }

    arg0->behaviorFlags = 0x10;
    arg0->velocity.x = 0;
    arg0->velocity.z = 0;
    arg0->velocity.y += -0x8000;
    applyClampedVelocityToPosition(arg0);

    if (arg0->unkB8C == -1) {
        arg0->behaviorFlags = 0;
        arg0->behaviorMode = 1;
        arg0->behaviorPhase = 1;
        arg0->behaviorStep = 0;
        arg0->behaviorCounter = 0;
    }

    return 0;
}

s32 crazyJungleBossHoverJumpPhase(Player *arg0) {
    if (arg0->behaviorStep == 0) {
        arg0->leanAnimIndex = 0xFFFF;
        arg0->velocity.y = 0x80000;
        arg0->behaviorStep = arg0->behaviorStep + 1;
        queueSoundAtPosition(&arg0->worldPos, 0x21);
    }

    arg0->behaviorFlags = 0x200;
    arg0->velocity.x = 0;
    arg0->velocity.z = 0;
    arg0->velocity.y = arg0->velocity.y + -0x8000;
    applyClampedVelocityToPosition(arg0);
    advancePlayerLeanAnimationAuto(arg0, 2);

    if (arg0->animFlags & 1) {
    } else {
        arg0->behaviorFlags = 0;
        arg0->behaviorMode = 1;
        arg0->behaviorPhase = 1;
        arg0->behaviorStep = 0;
        arg0->behaviorCounter = 0;
    }
    return 0;
}

void updateCrazyJungleBossPositionAndTrackCollision(Player *arg0) {
    s32 pad[8];
    Vec3i collisionOffset;
    s32 pad2[8];
    GameState *gameState;
    GameDataLayout *gameData;
    u16 newSectorIndex;

    gameState = getCurrentAllocation();
    memcpy(&arg0->headingTransform.translation.x, &arg0->worldPos, sizeof(Vec3i));
    gameData = &gameState->gameData;
    newSectorIndex = getOrUpdatePlayerSectorIndex(arg0, gameData, arg0->sectorIndex, &arg0->worldPos);
    arg0->sectorIndex = newSectorIndex;
    resolveTrackWallCollision(gameData, newSectorIndex, &arg0->worldPos, 0x187000, &collisionOffset);
    arg0->worldPos.x = arg0->worldPos.x + collisionOffset.x;
    arg0->worldPos.z = arg0->worldPos.z + collisionOffset.z;
    computePlayerTerrainAlignment(arg0);

    if (arg0->animFlags & 0x10000) {
        arg0->trackFaceType = 0;
    } else {
        findTrackFaceInSector(gameData, arg0->sectorIndex, &arg0->worldPos, &arg0->trackFaceType, &arg0->surfaceInfo);
        arg0->trackFaceSubtype = arg0->trackFaceType >> 4;
        arg0->trackFaceType = arg0->trackFaceType & 0xF;
    }
}

void updateCrazyJungleBossLeanBoneTransforms(Player *arg0) {
    Transform3D scratch;
    Transform3D squashMatrix;
    BoneHierarchyEntry *hierarchy;
    s32 i;

    hierarchy = (BoneHierarchyEntry *)getIndexedAnimationDataPtr(arg0->unk0, (s16)arg0->leanAnimIndex);
    composeTransform3D(&arg0->orientationTransform, &arg0->headingTransform, &arg0->orientationHeadingTransform);
    composeTransform3D(&arg0->tiltTransform, &arg0->orientationHeadingTransform, &arg0->modelTransform);

    for (i = 0; i < arg0->leanBoneCount; i++) {
        if (hierarchy[i].parentBone == 0xFF) {
            if (arg0->behaviorFlags & 0x10) {
                memcpy(&squashMatrix, &identityMatrix, sizeof(Transform3D));
                squashMatrix.m[1][1] = arg0->squashStretchScale;
                composeTransform3D(&arg0->unk488[hierarchy[i].boneIndex].transform.previous, &squashMatrix, &scratch);
                composeTransform3D(
                    &scratch,
                    &arg0->modelTransform,
                    &arg0->boneDisplayObjects[hierarchy[i].boneIndex].transform
                );
            } else {
                composeTransform3D(
                    &arg0->unk488[hierarchy[i].boneIndex].transform.previous,
                    &arg0->modelTransform,
                    &arg0->boneDisplayObjects[hierarchy[i].boneIndex].transform
                );
            }
        } else {
            composeTransform3D(
                &arg0->unk488[hierarchy[i].boneIndex].transform.previous,
                &arg0->boneDisplayObjects[hierarchy[i].parentBone].transform,
                &arg0->boneDisplayObjects[hierarchy[i].boneIndex].transform
            );
        }
    }
}

void renderCrazyJungleBossWithSurfaceColors(Player *arg0) {
    s32 i;
    s32 surfaceColorIndex;
    s32 pad[36];

    getCurrentAllocation();
    updateCrazyJungleBossLeanBoneTransforms(arg0);

    surfaceColorIndex = arg0->surfaceInfo >> 4;

    if (surfaceColorIndex == 0) {
        for (i = 0; i < 4; i++) {
            enqueuePreLitMultiPartDisplayList(i, arg0->boneDisplayObjects, arg0->leanBoneCount);
        }
    } else {
        arg0->boneDisplayObjects[0].light1R = gBossSurfaceColors[surfaceColorIndex].primaryR;
        arg0->boneDisplayObjects[0].light1G = gBossSurfaceColors[surfaceColorIndex].primaryG;
        arg0->boneDisplayObjects[0].light1B = gBossSurfaceColors[surfaceColorIndex].primaryB;
        arg0->boneDisplayObjects[0].light2R = gBossSurfaceColors[surfaceColorIndex].secondaryR;
        arg0->boneDisplayObjects[0].light2G = gBossSurfaceColors[surfaceColorIndex].secondaryG;
        arg0->boneDisplayObjects[0].light2B = gBossSurfaceColors[surfaceColorIndex].secondaryB;

        for (i = 0; i < 4; i++) {
            enqueueMultiPartDisplayList(i, arg0->boneDisplayObjects, arg0->leanBoneCount);
        }
    }
}

void updateBossProximityCheckpoints(Player *arg0) {
    s32 checkpointIndex;
    GameDataLayout *gameData;
    GameState *gameState;

    gameState = getCurrentAllocation();
    checkpointIndex = 0;
    gameData = &gameState->gameData;

    do {
        s32 *checkpointPos;
        u16 sectorIndex;

        arg0->shadowSamplePositions[checkpointIndex].x =
            arg0->headingTransform.translation.x + gCrazyJungleBossCheckpointOffsets[checkpointIndex].x;
        arg0->shadowSamplePositions[checkpointIndex].z =
            arg0->headingTransform.translation.z + gCrazyJungleBossCheckpointOffsets[checkpointIndex].z;
        checkpointPos = &arg0->shadowSamplePositions[checkpointIndex].x;
        sectorIndex = getOrUpdatePlayerSectorIndex(arg0, (u8 *)gameData, arg0->sectorIndex, checkpointPos);
        arg0->shadowSamplePositions[checkpointIndex].y =
            getTrackHeightInSector((u8 *)gameData, sectorIndex, checkpointPos, 0x100000);
        checkpointIndex++;
    } while (checkpointIndex < 9);

    arg0->shadowMeshNeedsUpdate = 1;

    for (checkpointIndex = 0; checkpointIndex < 4; checkpointIndex++) {
        enqueueCallbackBySlotIndex((u16)checkpointIndex, 1, renderRacerProjectedShadow, arg0);
    }
}
