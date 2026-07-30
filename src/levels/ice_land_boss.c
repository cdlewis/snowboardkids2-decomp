#include "animation/bone_animation.h"
#include "audio/audio.h"
#include "common.h"
#include "common_bss.h"
#include "data/asset_metadata.h"
#include "data/global_frame_counter.h"
#include "gamestate.h"
#include "graphics/displaylist.h"
#include "graphics/graphics.h"
#include "math/geometry.h"
#include "math/rand.h"
#include "os_cont.h"
#include "race/ai_pathfinding.h"
#include "race/character_stats.h"
#include "race/obstacle_sprites.h"
#include "race/particle_items.h"
#include "race/race_hud.h"
#include "race/race_main.h"
#include "race/spray_particles.h"
#include "race/track_collision.h"
#include "race/track_height.h"
#include "system/task_scheduler.h"
#include "text/text_elements.h"

#define ICE_BOSS_FLAG_TURBO 0x40000
#define ICE_BOSS_FLAG_FLYING 0x400000

#define ICE_BOSS_MODE_ATTACK 2
#define ICE_BOSS_MODE_DEFEATED 3

#define ICE_BOSS_TRIGGER_ATTACK 0x3D

typedef void (*FuncPtr)(void *);

typedef s32 (*StateFunc)(void *);

// Forward declarations for function pointer arrays
void updateIceLandBossLeanBoneTransforms(Player *);
s32 initIceLandBoss(Player *);
void dispatchIceLandBossChasePhase(Player *);
void dispatchIceLandBossAttackPhase(Player *);
s32 iceLandBossDefeatedBehavior(Player *);
s32 iceLandBossChaseIntroPhase(Player *);
s32 iceLandBossChaseAttackPhase(Player *);
s32 iceLandBossChaseExitPhase(Player *);
s32 iceLandBossGroundProjectileAttackPhase(Player *);
s32 iceLandBossHoverAttackPhase(Player *);

s32 gIceLandBossGroundCollisionLocalOffsets[] = {
    0x00000000, 0x00068000, 0x0003F000, 0x00000000, 0xFFF04000, 0x00018000, 0x00000000, 0xFFF04000, 0x00018000,
};

StateFunc gIceLandBossBehaviorModeHandlers[] = {
    (StateFunc)initIceLandBoss,
    (StateFunc)dispatchIceLandBossChasePhase,
    (StateFunc)dispatchIceLandBossAttackPhase,
    (StateFunc)iceLandBossDefeatedBehavior,
};

s32 gIceLandBossGroundCollisionOffset[] = {
    0x00000000,
    0x00117000,
    0x00000000,
};

s32 gIceLandBossFlyingCollisionOffset[] = {
    0x00000000,
    0x00100000,
    0x00000000,
};

s32 gIceLandBossSpawnXPositions[] = {
    0xFFF00000,
    0x00300000,
};

FuncPtr gIceLandBossChasePhaseHandlers[] = {
    (FuncPtr)iceLandBossChaseIntroPhase,
    (FuncPtr)iceLandBossChaseAttackPhase,
    (FuncPtr)iceLandBossChaseExitPhase,
};

s32 gIceLandBossForwardAcceleration[] = {
    0x00000000,
    0x00000000,
    0x00006000,
};

FuncPtr gIceLandBossAttackPhaseHandlers[] = {
    (FuncPtr)iceLandBossGroundProjectileAttackPhase,
    (FuncPtr)iceLandBossHoverAttackPhase,
};

Vec3i gIceLandBossShadowSampleOffsets[] = {
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

void updateIceLandBoss(Player *boss) {
    Transform3D rotationMatrix;
    Transform3D fullTransform;
    GameState *gameState;
    Player *player;
    s32 distanceToPlayer;
    s32 speedDelta;

    gameState = getCurrentAllocation();

    boss->velocity.x = boss->worldPos.x - boss->prevWorldPos.x;
    boss->velocity.y = boss->worldPos.y - boss->prevWorldPos.y;
    boss->velocity.z = boss->worldPos.z - boss->prevWorldPos.z;
    memcpy(&boss->prevWorldPos, &boss->worldPos, sizeof(Vec3i));

    player = gameState->players;
    distanceToPlayer = distance_3d(
        boss->worldPos.x - player->worldPos.x,
        boss->worldPos.y - player->worldPos.y,
        boss->worldPos.z - player->worldPos.z
    );

    if ((boss->finishPosition == 0) & (distanceToPlayer > 0xE00000)) {
        if (boss->animationFlags & ICE_BOSS_FLAG_FLYING) {
            boss->maxSpeedCap = getCharacterSnowboardMaxSpeed(0, SNOWBOARD_TRICK_LEVEL_2) - 0x8000;
        } else if (distanceToPlayer > 0x8C00000) {
            boss->maxSpeedCap = 0x70000;
        } else {
            boss->maxSpeedCap = getCharacterSnowboardMaxSpeed(0, SNOWBOARD_BALANCE_LEVEL_1) - 0x8000;
        }
    } else {
        boss->maxSpeedCap = getCharacterSnowboardMaxSpeed(0, SNOWBOARD_SPEED_LEVEL_3) + 0x18000;
    }

    if (boss->maxSpeedCap > 0x180000) {
        boss->maxSpeedCap = 0x180000;
    }

    speedDelta = boss->maxSpeedCap - boss->smoothedSpeedCap;
    if (speedDelta >= 0x1001) {
        speedDelta = 0x1000;
    }
    if (speedDelta < -0x80) {
        speedDelta = -0x80;
    }

    boss->smoothedSpeedCap = boss->smoothedSpeedCap + speedDelta;
    boss->animationFlags &= ~ICE_BOSS_FLAG_TURBO;

    if (boss->behaviorMode != ICE_BOSS_MODE_DEFEATED) {
        if (boss->hitReactionState != 0) {
            if (boss->hitReactionState == ICE_BOSS_TRIGGER_ATTACK) {
                boss->behaviorMode = ICE_BOSS_MODE_ATTACK;
                boss->behaviorPhase = 0;
                boss->behaviorStep = 0;
                boss->behaviorCounter = 0;
                if (boss->animationFlags & ICE_BOSS_FLAG_FLYING) {
                    boss->behaviorPhase = 1;
                }
            }
        }
    }
    boss->hitReactionState = 0;

    do {
    } while (gIceLandBossBehaviorModeHandlers[boss->behaviorMode](boss) != 0);

    createZRotationMatrix(&boss->tiltTransform, boss->rollAngle);
    createCombinedRotationMatrix(&boss->orientationTransform, boss->pitchAngle, boss->steeringAngle);
    createYRotationMatrix(&boss->headingTransform, boss->rotY);

    composeTransform3D(&boss->tiltTransform, &boss->orientationTransform, &rotationMatrix);
    composeTransform3D(&rotationMatrix, &boss->headingTransform, &fullTransform);

    fullTransform.translation.x -= boss->headingTransform.translation.x;
    fullTransform.translation.y -= boss->headingTransform.translation.y;
    fullTransform.translation.z -= boss->headingTransform.translation.z;

    if (boss->animationFlags & ICE_BOSS_FLAG_FLYING) {
        transformVector((s16 *)gIceLandBossFlyingCollisionOffset, (s16 *)&fullTransform, &boss->collisionOffset);
    } else {
        transformVector((s16 *)gIceLandBossGroundCollisionOffset, (s16 *)&fullTransform, &boss->collisionOffset);
    }
    memcpy(&boss->collisionListNode.localPos, &boss->collisionOffset, sizeof(Vec3i));
    addCollisionSectorNodeToList(&boss->collisionListNode);
    updateIceLandBossLeanBoneTransforms(boss);

    if (boss->animationFlags & ICE_BOSS_FLAG_FLYING) {
        transformVector(
            (s16 *)gIceLandBossFlyingCollisionOffset,
            (s16 *)&fullTransform,
            &boss->extraCollisionOffsets[0]
        );
    } else {
        transformVector(
            (s16 *)gIceLandBossGroundCollisionLocalOffsets,
            (s16 *)&boss->bodyPartDisplayObjects[0].transform,
            &boss->extraCollisionOffsets[0]
        );
        boss->extraCollisionOffsets[0].x -= boss->headingTransform.translation.x;
        boss->extraCollisionOffsets[0].y -= boss->headingTransform.translation.y;
        boss->extraCollisionOffsets[0].z -= boss->headingTransform.translation.z;

        transformVector(
            (s16 *)gIceLandBossGroundCollisionLocalOffsets + 6,
            (s16 *)&boss->bodyPartDisplayObjects[2].transform,
            &boss->extraCollisionOffsets[1]
        );
        boss->extraCollisionOffsets[1].x -= boss->headingTransform.translation.x;
        boss->extraCollisionOffsets[1].y -= boss->headingTransform.translation.y;
        boss->extraCollisionOffsets[1].z -= boss->headingTransform.translation.z;

        transformVector(
            (s16 *)gIceLandBossGroundCollisionLocalOffsets + 12,
            (s16 *)&boss->bodyPartDisplayObjects[4].transform,
            &boss->extraCollisionOffsets[2]
        );
        boss->extraCollisionOffsets[2].x -= boss->headingTransform.translation.x;
        boss->extraCollisionOffsets[2].y -= boss->headingTransform.translation.y;
        boss->extraCollisionOffsets[2].z -= boss->headingTransform.translation.z;
    }
}

s32 initIceLandBoss(Player *arg0) {
    Vec3i waypoint1;
    Vec3i waypoint2;
    GameState *gameState;
    s32 i;
    u16 trackIdx;

    gameState = getCurrentAllocation();

    // Initialize rotation matrices for bone transformations
    memcpy(&arg0->headingTransform, &identityMatrix, sizeof(Transform3D));
    createYRotationMatrix(&arg0->headingTransform, arg0->rotY);
    memcpy(&arg0->orientationTransform, &identityMatrix, sizeof(Transform3D));
    memcpy(&arg0->tiltTransform, &identityMatrix, sizeof(Transform3D));

    // Set initial position based on boss index
    arg0->worldPos.x = gIceLandBossSpawnXPositions[arg0->playerIndex];
    getTrackSegmentWaypoints((TrackGeometryData *)&gameState->gameData, 0, &waypoint1, &waypoint2);
    arg0->worldPos.z = waypoint1.z + 0x200000;
    trackIdx = getOrUpdatePlayerSectorIndex(arg0, &gameState->gameData, 0, &arg0->worldPos);
    arg0->sectorIndex = trackIdx;
    arg0->worldPos.y = getTrackHeightInSector(&gameState->gameData, trackIdx, &arg0->worldPos, 0x100000);
    memcpy(&arg0->prevWorldPos, &arg0->worldPos, sizeof(Vec3i));

    // Zero out velocity and set initial rotation
    arg0->velocity.x = 0;
    arg0->velocity.y = 0;
    arg0->velocity.z = 0;
    arg0->rotY = 0x1000;

    // Initialize body part transforms (12 body parts, each 0x3C bytes apart)
    for (i = 0; i < 12; i++) {
        PlayerDisplayObjectStride *strides = (PlayerDisplayObjectStride *)arg0;
        PlayerDisplayObjectView *view = (PlayerDisplayObjectView *)&strides[i];
        DisplayListObject *displayObject = &view->displayObject;

        memcpy(&displayObject->transform, &identityMatrix, sizeof(Transform3D));
        displayObject->segment1 = arg0->bodyPartDisplayListAsset;
        displayObject->segment2 = arg0->bodyPartCompressedAsset;
        displayObject->segment3 = NULL;
        displayObject->displayLists = &loadAssetByIndex_953B0(arg0->characterId, arg0->boardModelId)[i];
    }

    // Initialize bone animation state
    arg0->animationIndex = 0;
    arg0->animationBoneCount = getAnimationBoneCount(arg0->raceAnimationData, 0);
    for (i = 0; i < arg0->animationBoneCount; i++) {
        resetBoneAnimation(arg0->raceAnimationData, arg0->animationIndex, i, &arg0->boneAnimationStates[i]);
    }

    // Initialize behavior state
    arg0->behaviorMode = 1;
    arg0->extraCollisionRadii[0] = 0x240000;
    arg0->collisionSphereCount = 3;
    arg0->collisionListNode.posPtr = &arg0->worldPos;
    arg0->behaviorPhase = 0;
    arg0->extraCollisionRadii[1] = 0x11C000;
    arg0->extraCollisionRadii[2] = 0x11C000;
    arg0->collisionListNode.radius = 0x150000;
    arg0->collisionListNode.id = arg0->playerIndex;

    // Spawn chase camera if needed
    if (arg0->isBossRacer == 0) {
        spawnChaseCameraTask(arg0->playerIndex);
    }

    arg0->baseMaxSpeed = gameState->players[0].baseMaxSpeed - 0x10000;

    // Initialize asset offset table
    if (arg0->bossRaceData != NULL) {
        arg0->aiPathData = arg0->bossRaceData + ((s32 *)arg0->bossRaceData)[arg0->playerIndex];
    }

    arg0->bossHealth = 0xA;

    return 1;
}

void dispatchIceLandBossChasePhase(Player *arg0) {
    gIceLandBossChasePhaseHandlers[arg0->behaviorPhase](arg0);
}

void setIceBossFlyingMode(Player *arg0) {
    s32 i;

    for (i = 0; i < 12; i++) {
        arg0->bodyPartDisplayObjects[i].displayLists =
            loadAssetByIndex_953B0(arg0->characterId, arg0->boardModelId) + i + 12;
    }

    arg0->collisionRadius = 0x100000;
    arg0->extraCollisionRadii[0] = 0x100000;
    arg0->collisionSphereCount = 1;
    arg0->animationFlags = arg0->animationFlags | 0x400000;
}

s32 iceLandBossChaseIntroPhase(Player *arg0) {
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

    if (arg0->animationFlags & 0x400000) {
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

    if (arg0->animationFlags & 0x100000) {
        setPlayerBehaviorMode(arg0, 3);
        return 1;
    }

    if (arg0->animationFlags & 0x80000) {
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
        } else if (arg0->animationFlags & 0x400000) {
            arg0->unkB8C = randA() & 0xF;
        } else {
            arg0->unkB8C = (randA() & 0xFF) >> 1;
        }
        arg0->unkB90 = 0;
    }

    arg0->animationFlags = arg0->animationFlags | 0x40000;
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

    if (!(arg0->animationFlags & 0x1)) {
        createYRotationMatrix(&arg0->headingTransform, arg0->rotY);
        func_8006BDBC_6C9BC((&arg0->orientationTransform), &arg0->headingTransform, &sp10);
        transformVector3(&arg0->velocity, &sp10, &sp30);
        sp30.x = 0;
        transformVector2(&sp30, &sp10, &arg0->velocity);
        transformVector2(&gIceLandBossForwardAcceleration, &sp10, &sp30);

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
    updateIceLandBossLeanBoneTransforms(arg0);

    transformVectorRelative(&gameState->players->worldPos, &arg0->bodyPartDisplayObjects[5].transform, &sp40);

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

    arg0->bossYawAngle = arg0->bossYawAngle + angleDiff;

    if (arg0->animationFlags & 0x400000) {
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

s32 iceLandBossChaseExitPhase(Player *arg0) {
    s32 pad[3];

    getCurrentAllocation();

    arg0->velocity.x -= arg0->velocity.x / 8;
    arg0->velocity.z -= arg0->velocity.z / 8;
    arg0->velocity.y += -0x8000;
    applyClampedVelocityToPosition(arg0);

    if (arg0->animationFlags & 0x400000) {
        advancePlayerLeanAnimationAuto(arg0, 2);
    } else {
        advancePlayerLeanAnimationAuto(arg0, 0);
    }

    return 0;
}

void dispatchIceLandBossAttackPhase(Player *arg0) {
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
        boss->animationIndex = 0xFFFF;
        boss->unkB8C = 0;
        boss->behaviorStep += 1;

        if (!(boss->animationFlags & 0x80000)) {
            if (boss->bossHealth == 0) {
                queueSoundAtPosition(&boss->worldPos, 0x4C);
            } else {
                boss->bossHealth -= 1;
                if (boss->bossHealth == 0) {
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

    if (!(boss->animationFlags & 1)) {
        createYRotationMatrix(&boss->headingTransform, boss->rotY);
        func_8006BDBC_6C9BC((&boss->orientationTransform), &boss->headingTransform, &rotMatrix);
        transformVector3(&boss->velocity, &rotMatrix, &tempVec);
        tempVec.x = 0;
        transformVector2(&tempVec, &rotMatrix, &boss->velocity);
        transformVector2(&gIceLandBossForwardAcceleration, &rotMatrix, &tempVec);

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

        if (boss->bossHealth == 0) {
            for (i = 0; i < 0x1E; i++) {
                spawnBossHomingProjectileVariant1Task(boss);
            }
            memcpy(&tempVec, &boss->worldPos, sizeof(Vec3i));
            tempVec.y += 0x300000;
            spawnSparkleEffectWithPlayer(&tempVec, boss->playerIndex);
            setIceBossFlyingMode(boss);
            boss->bossHealth = 3;
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
        if (!(arg0->animationFlags & 0x80000)) {
            if (arg0->bossHealth != 0) {
                arg0->bossHealth = arg0->bossHealth - 1;
            }
        }
    }

    arg0->behaviorFlags = 0x200;
    arg0->velocity.x = 0;
    arg0->velocity.z = 0;
    arg0->velocity.y = arg0->velocity.y + (-0x8000);

    applyClampedVelocityToPosition(arg0);
    advancePlayerLeanAnimationAuto(arg0, 5);

    if ((gGlobalFrameCounter & 0xF) == 0) {
        spawnBossHomingProjectileVariant2Task(arg0);
    }

    if (arg0->velocity.y < 0) {
        if (!(arg0->animationFlags & 0x1)) {
            hoverCount = arg0->bossHealth;
            arg0->behaviorFlags = 0;
            arg0->behaviorMode = 1;
            arg0->behaviorPhase = 1;
            arg0->behaviorStep = 0;
            arg0->behaviorCounter = 0;

            if (hoverCount == 0) {
                arg0->animationFlags = arg0->animationFlags | 0x100000;
            }

            return 0;
        }
    }

    return 0;
}

s32 iceLandBossDefeatedBehavior(Player *arg0) {
    s32 pad[3];

    getCurrentAllocation();

    arg0->velocity.y += -0x8000;
    applyClampedVelocityToPosition(arg0);

    if (arg0->behaviorStep == 0) {
        if (advancePlayerLeanAnimation(arg0, 6) != 0) {
            arg0->behaviorStep = arg0->behaviorStep + 1;
        }
        return 0;
    }

    advancePlayerLeanAnimationAuto(arg0, 7);

    return 0;
}

void updateIceLandBossPositionAndTrackCollision(Player *boss) {
    s32 pad[8];
    Vec3i collisionOffset;
    s32 pad2[8];
    GameState *gameState;
    GameDataLayout *gameData;
    u16 newSectorIndex;

    gameState = getCurrentAllocation();
    memcpy(&boss->headingTransform.translation, &boss->worldPos, sizeof(Vec3i));
    gameData = &gameState->gameData;
    newSectorIndex = getOrUpdatePlayerSectorIndex(boss, gameData, boss->sectorIndex, &boss->worldPos);
    boss->sectorIndex = newSectorIndex;
    resolveTrackWallCollision(gameData, newSectorIndex, &boss->worldPos, 0x187000, &collisionOffset);
    boss->worldPos.x += collisionOffset.x;
    boss->worldPos.z += collisionOffset.z;
    computePlayerTerrainAlignment(boss);

    if (boss->animationFlags & 0x10000) {
        boss->trackFaceType = 0;
    } else {
        findTrackFaceInSector(gameData, boss->sectorIndex, &boss->worldPos, &boss->trackFaceType, &boss->surfaceInfo);
        boss->trackFaceSubtype = boss->trackFaceType >> 4;
        boss->trackFaceType = boss->trackFaceType & 0xF;
    }
}

void updateIceLandBossLeanBoneTransforms(Player *arg0) {
    Transform3D scratch;
    Transform3D squashMatrix;
    BoneHierarchyEntry *hierarchy;
    s32 i;
    u8 parentBone;
    Transform3D *temp;

    hierarchy = getIndexedAnimationDataPtr(arg0->raceAnimationData, (s16)arg0->animationIndex);
    composeTransform3D(&arg0->orientationTransform, &arg0->headingTransform, &arg0->orientationHeadingTransform);
    composeTransform3D(&arg0->tiltTransform, &arg0->orientationHeadingTransform, &arg0->modelTransform);

    for (i = 0; i < arg0->animationBoneCount; i++) {
        if (hierarchy[i].parentBone == 0xFF) {
            if (arg0->behaviorFlags & 0x10) {
                memcpy(&squashMatrix, &identityMatrix, sizeof(Transform3D));
                squashMatrix.m[1][1] = arg0->squashStretchScale;
                composeTransform3D(
                    &arg0->boneAnimationStates[hierarchy[i].boneIndex].transform.previous,
                    &squashMatrix,
                    &scratch
                );
                composeTransform3D(
                    &scratch,
                    &arg0->modelTransform,
                    &arg0->bodyPartDisplayObjects[hierarchy[i].boneIndex].transform
                );
            } else {
                composeTransform3D(
                    &arg0->boneAnimationStates[hierarchy[i].boneIndex].transform.previous,
                    &arg0->modelTransform,
                    &arg0->bodyPartDisplayObjects[hierarchy[i].boneIndex].transform
                );
            }
        } else {
            composeTransform3D(
                &arg0->boneAnimationStates[hierarchy[i].boneIndex].transform.previous,
                &arg0->bodyPartDisplayObjects[hierarchy[i].parentBone].transform,
                &arg0->bodyPartDisplayObjects[hierarchy[i].boneIndex].transform
            );
        }
    }

    temp = &scratch;
    memcpy(temp, &arg0->bodyPartDisplayObjects[5].transform, sizeof(Transform3D));
    createYRotationMatrix(&squashMatrix, (u16)arg0->bossYawAngle);
    func_8006BDBC_6C9BC((&squashMatrix), &scratch, &arg0->bodyPartDisplayObjects[5].transform);
}

void renderIceLandBossWithSurfaceColors(Player *arg0) {
    s32 pad[36];
    s32 i;
    s32 index;

    getCurrentAllocation();
    updateIceLandBossLeanBoneTransforms(arg0);

    index = arg0->surfaceInfo >> 4;

    if (index == 0) {
        for (i = 0; i < 4; i++) {
            enqueuePreLitMultiPartDisplayList(i, arg0->bodyPartDisplayObjects, arg0->animationBoneCount);
        }
    } else {
        arg0->bodyPartDisplayObjects[0].light1R = gBossSurfaceColors[index].primaryR;
        arg0->bodyPartDisplayObjects[0].light1G = gBossSurfaceColors[index].primaryG;
        arg0->bodyPartDisplayObjects[0].light1B = gBossSurfaceColors[index].primaryB;
        arg0->bodyPartDisplayObjects[0].light2R = gBossSurfaceColors[index].secondaryR;
        arg0->bodyPartDisplayObjects[0].light2G = gBossSurfaceColors[index].secondaryG;
        arg0->bodyPartDisplayObjects[0].light2B = gBossSurfaceColors[index].secondaryB;

        for (i = 0; i < 4; i++) {
            enqueueMultiPartDisplayList(i, arg0->bodyPartDisplayObjects, arg0->animationBoneCount);
        }
    }
}

/**
 * Updates projected shadow sample positions for the Ice Land boss.
 * For each sample, computes X/Z world position from local offsets,
 * then finds the terrain height at that position.
 * Uses different sample offset arrays based on whether the boss is flying or on ground.
 * Enqueues callbacks to render the projected shadow.
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
        if (boss->animationFlags & flyingFlag) {
            boss->shadowSamplePositions[jointIndex].x =
                boss->headingTransform.translation.x + gIceLandBossShadowSampleOffsets[jointIndex + 9].x;
            boss->shadowSamplePositions[jointIndex].z =
                boss->headingTransform.translation.z + gIceLandBossShadowSampleOffsets[jointIndex + 9].z;
        } else {
            boss->shadowSamplePositions[jointIndex].x =
                boss->headingTransform.translation.x + gIceLandBossShadowSampleOffsets[jointIndex].x;
            boss->shadowSamplePositions[jointIndex].z =
                boss->headingTransform.translation.z + gIceLandBossShadowSampleOffsets[jointIndex].z;
        }

        jointPos = &boss->shadowSamplePositions[jointIndex];
        sectorIndex = getOrUpdatePlayerSectorIndex((void *)boss, gameData, boss->sectorIndex, jointPos);
        boss->shadowSamplePositions[jointIndex].y = getTrackHeightInSector(gameData, sectorIndex, jointPos, 0x100000);
    }

    boss->shadowMeshNeedsUpdate = 1;

    for (jointIndex = 0; jointIndex < 4; jointIndex++) {
        enqueueCallbackBySlotIndex(jointIndex, 1, renderRacerProjectedShadow, (void *)boss);
    }
}
