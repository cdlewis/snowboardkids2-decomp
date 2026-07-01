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
#include "race/obstacle_sprites.h"
#include "race/particle_items.h"
#include "race/race_main.h"
#include "race/spray_particles.h"
#include "race/track_collision.h"
#include "race/track_height.h"
#include "system/task_scheduler.h"
#include "text/text_elements.h"

extern s32 gJingleTownBossSpawnPos[];

typedef struct {
    u8 padding38[0x38];
    Transform3D transform;
    void *assetPointer;
    s32 param1;
    s32 param2;
    s32 param3;
} DisplayObjectElement;

typedef struct {
    s16 frames;
    s16 anim;
} HoverIntroAnimFrame;

typedef void (*FuncPtr)(void *);
typedef s32 (*StateFunc)(void *);

extern s8 gAnalogStickY[];
extern s8 gAnalogStickX[];
extern s32 gButtonsPressed[];
void updateJingleTownBossModelTransforms(Player *);
extern BossSurfaceColor gBossSurfaceColors[];

s32 initJingleTownBoss(Player *);
void dispatchChaseAttackPhase(Player *);
void dispatchHoverAttackPhase(Player *);
s32 jingleTownBossHoverAttackExitPhase(Player *);
s32 jingleTownBossChaseAttackIntroPhase(Player *);
s32 jingleTownBossChaseAttackMainPhase(Player *);
s32 jingleTownBossChaseAttackExitPhase(Player *);
s32 jingleTownBossHoverAttackIntroPhase(Player *);
s32 jingleTownBossHoverAttackMainPhase(Player *);

StateFunc gJingleTownBossBehaviorModeHandlers[] = {
    (StateFunc)initJingleTownBoss,
    (StateFunc)dispatchChaseAttackPhase,
    (StateFunc)dispatchHoverAttackPhase,
    (StateFunc)jingleTownBossHoverAttackExitPhase,
};

s16 gCollisionLocalPos[] = { 0x0000, 0x0000, 0x0013, (s16)0x9000, 0x0000, 0x0000 };

s16 gTrackLocalPosArray[] = { 0x0000, 0x0000, 0x0013, (s16)0x9000, 0x0000, 0x0000,
                              0x0000, 0x0000, 0x0000, 0x0000,      0x0000, 0x0000 };

s32 gJingleTownBossSpawnPos[] = { 0xFFF00000, 0x00300000 };

FuncPtr gChaseAttackPhaseHandlers[] = {
    (FuncPtr)jingleTownBossChaseAttackIntroPhase,
    (FuncPtr)jingleTownBossChaseAttackMainPhase,
    (FuncPtr)jingleTownBossChaseAttackExitPhase,
};

Vec3i gForwardDirection = { .x = 0, .y = 0, .z = 0x10000 };

FuncPtr gHoverAttackPhaseHandlers[] = {
    (FuncPtr)jingleTownBossHoverAttackIntroPhase,
    (FuncPtr)jingleTownBossHoverAttackMainPhase,
};

HoverIntroAnimFrame gHoverIntroAnimFrames[] = {
    { 0x0019, 0x0200 },
    { 0x0001, 0x0400 },
    { 0x0001, 0x0800 },
    { 0x0001, 0x1000 },
    { 0x0001, 0x1400 },
    { 0x0001, 0x0800 },
    { 0x0001, 0x1000 },
    { 0x0001, 0x1800 },
    { 0xFFFF, 0x2000 },
};

Vec3i gJingleTownBossHoverExitOffsets[] = {
    { .x = 0x00000000, .y = 0x00260000, .z = 0x00000000 },
    { .x = 0x00000000, .y = 0x00150000, .z = 0x00000000 },
    { .x = 0x00150000, .y = 0x00120000, .z = 0x00000000 },
    { .x = 0xFFEB0000, .y = 0x00120000, .z = 0x00000000 },
    { .x = 0x00000000, .y = 0x00120000, .z = 0x00150000 },
    { .x = 0x00000000, .y = 0x00120000, .z = 0xFFEB0000 },
    { .x = 0xFFE79000, .y = 0x00000000, .z = 0x00187000 },
    { .x = 0x00000000, .y = 0x00000000, .z = 0x00187000 },
    { .x = 0x00187000, .y = 0x00000000, .z = 0x00187000 },
    { .x = 0xFFE79000, .y = 0x00000000, .z = 0x00000000 },
    { .x = 0x00000000, .y = 0x00000000, .z = 0x00000000 },
    { .x = 0x00187000, .y = 0x00000000, .z = 0x00000000 },
    { .x = 0xFFE79000, .y = 0x00000000, .z = 0xFFE79000 },
    { .x = 0x00000000, .y = 0x00000000, .z = 0xFFE79000 },
    { .x = 0x00187000, .y = 0x00000000, .z = 0xFFE79000 },
};

void updateJingleTownBoss(Player *arg0) {
    Transform3D sp10;
    Transform3D sp30;
    GameState *alloc;
    s32 distanceToPlayer;
    s32 i;
    s16 playerState;
    s32 speedDelta;

    alloc = getCurrentAllocation();
    if (arg0->isBossRacer != 0) {
        calculateAITargetPosition(arg0);
    } else {
        arg0->inputStickX = gAnalogStickX[arg0->playerIndex] / 4;
        arg0->inputStickY = gAnalogStickY[arg0->playerIndex] / 4;

        arg0->prevInputButtons = arg0->inputButtons;
        arg0->inputButtons = gButtonsPressed[arg0->playerIndex];
        arg0->inputButtonsPressed = arg0->inputButtons & ~arg0->prevInputButtons;
    }

    arg0->velocity.x = arg0->worldPos.x - arg0->prevWorldPos.x;
    arg0->velocity.y = arg0->worldPos.y - arg0->prevWorldPos.y;
    arg0->velocity.z = arg0->worldPos.z - arg0->prevWorldPos.z;
    memcpy(&arg0->prevWorldPos, &arg0->worldPos, sizeof(Vec3i));

    distanceToPlayer = distance_3d(
        arg0->worldPos.x - alloc->players->worldPos.x,
        arg0->worldPos.y - alloc->players->worldPos.y,
        arg0->worldPos.z - alloc->players->worldPos.z
    );

    if ((arg0->finishPosition == 0) && (distanceToPlayer > 0x1000000)) {
        arg0->maxSpeedCap = getCharacterBoardStatParam0(0, 0) - 0x40000;
    } else {
        arg0->maxSpeedCap = getCharacterBoardStatParam0(0, 0) + 0x180000;
    }

    if (arg0->maxSpeedCap > 0x180000) {
        arg0->maxSpeedCap = 0x180000;
    }

    for (i = 0; i < arg0->slowdownLevel; i++) {
        arg0->maxSpeedCap = arg0->maxSpeedCap - (arg0->maxSpeedCap >> 2);
    }

    speedDelta = arg0->maxSpeedCap - arg0->smoothedSpeedCap;
    if (speedDelta >= 0x101) {
        speedDelta = 0x100;
    }
    if (speedDelta < -0x80) {
        speedDelta = -0x80;
    }
    arg0->smoothedSpeedCap = arg0->smoothedSpeedCap + speedDelta;

    arg0->animFlags &= 0xFFFBFFFF;

    if (arg0->behaviorMode != 3) {
        playerState = arg0->hitReactionState;
        if (playerState != 0) {
            if (playerState != 0x3D) {
                if (playerState == 0x3E) {
                    arg0->behaviorMode = 2;
                    arg0->behaviorPhase = 0;
                    arg0->behaviorStep = 0;
                    arg0->behaviorCounter = 0;
                    arg0->unk474 = 0;
                }
            } else {
                arg0->behaviorMode = 2;
                arg0->behaviorPhase = 1;
                arg0->behaviorStep = 0;
                arg0->behaviorCounter = 0;
                arg0->unk474 = 0;
            }
        }
    }
    arg0->hitReactionState = 0;

    do {
    } while (gJingleTownBossBehaviorModeHandlers[arg0->behaviorMode](arg0) != 0);

    createZRotationMatrix(&arg0->tiltTransform, arg0->rollAngle);
    createCombinedRotationMatrix(&arg0->orientationTransform, arg0->pitchAngle, arg0->steeringAngle);
    createYRotationMatrix(&arg0->headingTransform, arg0->rotY);

    composeTransform3D(&arg0->tiltTransform, &arg0->orientationTransform, &sp10);
    composeTransform3D(&sp10, &arg0->headingTransform, &sp30);

    sp30.translation.x -= arg0->headingTransform.translation.x;
    sp30.translation.y -= arg0->headingTransform.translation.y;
    sp30.translation.z -= arg0->headingTransform.translation.z;

    transformVector(gCollisionLocalPos, (s16 *)&sp30, &arg0->collisionOffset);
    memcpy(&arg0->collisionListNode.localPos, &arg0->collisionOffset, sizeof(Vec3i));
    addCollisionSectorNodeToList(&arg0->collisionListNode);
    updateJingleTownBossModelTransforms(arg0);

    transformVector(
        gTrackLocalPosArray,
        (s16 *)&arg0->boneDisplayObjects[0].transform,
        &arg0->extraCollisionOffsets[0]
    );
    arg0->extraCollisionOffsets[0].x -= arg0->headingTransform.translation.x;
    arg0->extraCollisionOffsets[0].y -= arg0->headingTransform.translation.y;
    arg0->extraCollisionOffsets[0].z -= arg0->headingTransform.translation.z;

    transformVector(
        gTrackLocalPosArray + 6,
        (s16 *)&arg0->boneDisplayObjects[1].transform,
        &arg0->extraCollisionOffsets[1]
    );
    arg0->extraCollisionOffsets[1].x -= arg0->headingTransform.translation.x;
    arg0->extraCollisionOffsets[1].y -= arg0->headingTransform.translation.y;
    arg0->extraCollisionOffsets[1].z -= arg0->headingTransform.translation.z;
}

s32 initJingleTownBoss(Player *arg0) {
    Vec3i waypoint1;
    Vec3i waypoint2;
    GameState *gameState;
    s32 i;

    gameState = getCurrentAllocation();

    memcpy(&arg0->headingTransform, &identityMatrix, sizeof(Transform3D));
    createYRotationMatrix(&arg0->headingTransform, arg0->rotY);
    memcpy(&arg0->orientationTransform, &identityMatrix, sizeof(Transform3D));
    memcpy(&arg0->tiltTransform, &identityMatrix, sizeof(Transform3D));

    arg0->worldPos.x = gJingleTownBossSpawnPos[arg0->playerIndex];
    getTrackSegmentWaypoints((TrackGeometryData *)&gameState->gameData, 0, &waypoint1, &waypoint2);
    arg0->worldPos.z = waypoint1.z + 0x200000;
    arg0->sectorIndex = getOrUpdatePlayerSectorIndex(arg0, &gameState->gameData, 0, &arg0->worldPos);
    arg0->worldPos.y = getTrackHeightInSector(&gameState->gameData, arg0->sectorIndex, &arg0->worldPos, 0x100000);
    memcpy(&arg0->prevWorldPos, &arg0->worldPos, sizeof(Vec3i));

    arg0->velocity.x = 0;
    arg0->velocity.y = 0;
    arg0->velocity.z = 0;
    arg0->rotY = 0x1000;

    for (i = 0; i < 3; i++) {
        DisplayObjectElement *dispObj = (DisplayObjectElement *)((u8 *)arg0 + i * sizeof(DisplayListObject));
        memcpy(&dispObj->transform, &identityMatrix, sizeof(Transform3D));
        dispObj->param1 = (s32)arg0->unk4;
        dispObj->param2 = (s32)arg0->unk8;
        dispObj->param3 = 0;
        dispObj->assetPointer = (void *)((s32)loadAssetByIndex_953B0(arg0->characterId, arg0->boardType) + i * 0x10);
    }

    arg0->behaviorMode = 1;
    arg0->unkB30 = 0x180000;
    arg0->unkBB4 = 2;
    arg0->behaviorPhase = 0;
    arg0->extraCollisionRadii = 0x1EC000;
    arg0->collisionRadius = 0x1EC000;
    arg0->collisionListNode.posPtr = &arg0->worldPos;
    arg0->collisionListNode.radius = 0x1EC000;
    arg0->collisionListNode.id = arg0->playerIndex;

    if (arg0->isBossRacer == 0) {
        spawnChaseCameraTask(arg0->playerIndex);
    }

    arg0->unkBDB = 0xA;

    if (arg0->unk1C != NULL) {
        arg0->aiPathData = (void *)((s32)arg0->unk1C + ((s32 *)arg0->unk1C)[arg0->playerIndex]);
    }

    return 1;
}

void dispatchChaseAttackPhase(Player *arg0) {
    ((void (*)(void *))gChaseAttackPhaseHandlers[arg0->behaviorPhase])(arg0);
}

s32 jingleTownBossChaseAttackIntroPhase(Player *arg0) {
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

    return 0;
}

s32 jingleTownBossChaseAttackMainPhase(Player *arg0) {
    Transform3D sp10;
    Vec3i sp30;
    Transform3D *temp_s0;
    Vec3i *temp_s1;
    s16 angleDiff;
    GameState *gameState;

    gameState = getCurrentAllocation();

    if (arg0->animFlags & 0x100000) {
        setPlayerBehaviorMode(arg0, 3);
        return 1;
    }

    if (arg0->animFlags & 0x80000) {
        setPlayerBehaviorPhase(arg0, 2);
        return 1;
    }

    if (arg0->behaviorStep == 0) {
        if (gameState->raceFrameCounter < 0x1E) {
            arg0->unkB8C = ((randA() & 0xFF) >> 2) + 0x69;
        } else {
            arg0->unkB8C = (randA() & 0xFF) >> 1;
        }
        arg0->behaviorStep++;
    }

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

    if (!(arg0->animFlags & 1)) {
        temp_s0 = &arg0->headingTransform;
        createYRotationMatrix(temp_s0, arg0->rotY);
        func_8006BDBC_6C9BC((&arg0->orientationTransform), temp_s0, &sp10);
        temp_s1 = &arg0->velocity;
        transformVector3(temp_s1, &sp10, &sp30);
        sp30.x = 0;
        transformVector2(&sp30, &sp10, temp_s1);
        transformVector2(&gForwardDirection, &sp10, &sp30);

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

    arg0->velocity.y -= 0x8000;
    applyClampedVelocityToPosition(arg0);

    switch (arg0->behaviorCounter) {
        case 0:
            updateJingleTownBossModelTransforms(arg0);
            transformVectorRelative(&gameState->players->worldPos, &arg0->boneDisplayObjects[1].transform, &sp30);

            angleDiff = atan2Fixed(-sp30.x, -sp30.z) & 0x1FFF;

            if (angleDiff >= 0x1000) {
                angleDiff = angleDiff | 0xE000;
            }

            if (angleDiff >= 0x81) {
                angleDiff = 0x80;
            }

            if (angleDiff < -0x80) {
                angleDiff = -0x80;
            }

            arg0->unkA9E = (arg0->unkA9E + angleDiff) & 0x1FFF;

            angleDiff = atan2Fixed(sp30.y, -distance_2d(sp30.x, sp30.z)) & 0x1FFF;

            if (angleDiff >= 0x1000) {
                angleDiff = angleDiff | 0xE000;
            }

            if (angleDiff >= 0x41) {
                angleDiff = 0x40;
            }

            if (angleDiff < -0x40) {
                angleDiff = -0x40;
            }

            {
                short temp = (arg0->jingleBossPitchAngle + angleDiff) & 0x1FFF;
                arg0->jingleBossPitchAngle = temp;
                if (temp >= 0x1000) {
                    arg0->jingleBossPitchAngle = temp - 0x2000;
                }
            }

            if ((s16)arg0->jingleBossPitchAngle >= 0x201) {
                arg0->jingleBossPitchAngle = 0x200;
            }

            if ((s16)arg0->jingleBossPitchAngle < -0x200) {
                s16 negVal = -0x200;
                arg0->jingleBossPitchAngle = negVal;
            }

            if (arg0->unkB8C != 0) {
                arg0->unkB8C--;
            } else {
                if (gameState->players->behaviorFlags != 0) {
                    return 0;
                }

                if ((randA() & 3) == 0) {
                    arg0->unkB8C = 0x20;
                    arg0->behaviorCounter = 5;
                } else {
                    arg0->unkB8C = 4;
                    arg0->behaviorCounter = 1;
                }
            }
            break;

        case 1:
            arg0->unkA9E = arg0->unkA9E - 0x100;
            arg0->unkB8C--;
            if (arg0->unkB8C == 0) {
                spawnAttackProjectile(2, arg0->playerIndex, 0);
                arg0->unkB8C = 4;
                arg0->behaviorCounter++;
            }
            break;

        case 2:
            arg0->unkA9E = arg0->unkA9E + 0x100;
            arg0->unkB8C--;
            if (arg0->unkB8C == 0) {
                arg0->unkB8C = 4;
                arg0->behaviorCounter++;
                spawnAttackProjectile(3, arg0->playerIndex, 0);
                return 0;
            }
            break;

        case 3:
            arg0->unkA9E = arg0->unkA9E + 0x100;
            arg0->unkB8C--;
            if (arg0->unkB8C == 0) {
                spawnAttackProjectile(2, arg0->playerIndex, 0);
                arg0->behaviorCounter = 0;
                if (gameState->unk86 != 0) {
                    arg0->unkB8C = (randA() & 0xF) + 8;
                } else {
                    arg0->unkB8C = ((randA() & 0xFF) >> 2) + 0x3C;
                }
            }
            break;

        case 5:
            if ((arg0->unkB8C & 3) == 0) {
                spawnAttackProjectile(3, arg0->playerIndex, 0);
            }
            arg0->unkA9E = arg0->unkA9E - 0x100;
            arg0->unkB8C--;
            if (arg0->unkB8C == 0) {
                arg0->behaviorCounter = 0;
                if (gameState->unk86 != 0) {
                    arg0->unkB8C = (randA() & 0xF) + 8;
                } else {
                    arg0->unkB8C = ((randA() & 0xFF) >> 2) + 0x5A;
                }
            }
            break;
    }

    return 0;
}

void applyPitchAngleDamping(Player *arg0) {
    s16 currentAngle;
    s16 damping;

    currentAngle = arg0->jingleBossPitchAngle;
    damping = -currentAngle;

    if (damping >= 0x1000) {
        damping -= 0x2000;
    }

    if (damping > 0x80) {
        damping = 0x80;
    }

    if (damping < -0x80) {
        damping = -0x80;
    }

    arg0->jingleBossPitchAngle = currentAngle + damping;
}

s32 jingleTownBossChaseAttackExitPhase(Player *arg0) {
    s32 pad[3];

    applyPitchAngleDamping(arg0);
    arg0->unkA9E -= 0x100;
    arg0->velocity.x -= arg0->velocity.x / 8;
    arg0->velocity.z -= arg0->velocity.z / 8;
    arg0->velocity.y += -0x8000;
    applyClampedVelocityToPosition(arg0);
    return 0;
}

void dispatchHoverAttackPhase(Player *arg0) {
    ((void (*)(void *))gHoverAttackPhaseHandlers[arg0->behaviorPhase])(arg0);
}

s32 jingleTownBossHoverAttackIntroPhase(Player *arg0) {
    if (arg0->behaviorStep == 0) {
        arg0->behaviorStep++;
        arg0->unkB8C = 1;
        if (arg0->velocity.y > 0) {
            arg0->velocity.y = 0;
        }
        if (!(arg0->animFlags & 0x80000)) {
            if (arg0->unkBDB != 0) {
                arg0->unkBDB--;
            }
        }
    }

    arg0->unkB8C--;
    if (arg0->unkB8C == 0) {
        arg0->unkB8C = gHoverIntroAnimFrames[arg0->behaviorCounter].frames;
        arg0->squashStretchScale = gHoverIntroAnimFrames[arg0->behaviorCounter].anim;
        arg0->behaviorCounter++;
    }

    arg0->behaviorFlags = 0x10;
    arg0->velocity.x = 0;
    arg0->velocity.z = 0;
    arg0->velocity.y -= 0x8000;
    applyClampedVelocityToPosition(arg0);

    if (arg0->unkB8C == -1) {
        arg0->behaviorFlags = 0;
        arg0->behaviorMode = 1;
        arg0->behaviorPhase = 1;
        arg0->behaviorStep = 0;
        arg0->behaviorCounter = 0;
        arg0->unk474 = 0;
        if (arg0->unkBDB == 0) {
            arg0->animFlags |= 0x100000;
        }
    }

    return 0;
}

s32 jingleTownBossHoverAttackMainPhase(Player *arg0) {
    if (arg0->behaviorStep == 0) {
        s32 bossFlags = arg0->animFlags;
        arg0->behaviorStep++;
        arg0->unk468 = 0x80000;
        arg0->unkB8C = 4;
        if ((bossFlags & 0x80000) == 0) {
            if (arg0->unkBDB != 0) {
                arg0->unkBDB--;
            }
        }
    }

    arg0->behaviorFlags = 0x200;
    arg0->velocity.y += -0x8000;
    arg0->unk468 += -0x8000;

    applyPitchAngleDamping(arg0);

    arg0->velocity.x = 0;
    arg0->velocity.z = 0;
    arg0->unkA9E = (arg0->unkA9E + 0x100) & 0x1FFF;

    if (arg0->unkB8C == 0) {
        u8 rand_val = randA();
        s32 param = (rand_val & 1) | 2;
        spawnAttackProjectile(param, arg0->playerIndex, 0);
        arg0->unkB8C = 4;
    } else {
        arg0->unkB8C--;
    }

    applyClampedVelocityToPosition(arg0);

    arg0->unk474 += arg0->unk468;

    if (arg0->unk474 == 0) {
        arg0->behaviorFlags = 0;
        arg0->behaviorMode = 1;
        arg0->behaviorPhase = 1;
        arg0->behaviorStep = 0;
        arg0->behaviorCounter = 0;
        arg0->unk474 = 0;

        if (arg0->unkBDB == 0) {
            arg0->animFlags |= 0x100000;
        }
    }

    return 0;
}

s32 jingleTownBossHoverAttackExitPhase(Player *arg0) {
    Vec3i posOffset;
    Vec3i burstPos;
    u8 phase;

    getCurrentAllocation();

    phase = arg0->behaviorPhase;
    if (phase == 0) {
        arg0->behaviorPhase++;
        transformVector2(
            (s16 *)&gJingleTownBossHoverExitOffsets[0],
            (s16 *)&arg0->boneDisplayObjects[0].transform,
            &posOffset
        );
        arg0->worldPos.x += posOffset.x;
        arg0->worldPos.y += posOffset.y;
        arg0->worldPos.z += posOffset.z;
        memcpy(&arg0->prevWorldPos, &arg0->worldPos, sizeof(Vec3i));
        arg0->animFlags |= 0x200000;
        transformVector(
            (s16 *)&gJingleTownBossHoverExitOffsets[1],
            (s16 *)&arg0->boneDisplayObjects[0].transform,
            &burstPos
        );
        spawnBurstEffect(&burstPos);
        transformVector(
            (s16 *)&gJingleTownBossHoverExitOffsets[2],
            (s16 *)&arg0->boneDisplayObjects[0].transform,
            &burstPos
        );
        spawnBurstEffect(&burstPos);
        transformVector(
            (s16 *)&gJingleTownBossHoverExitOffsets[3],
            (s16 *)&arg0->boneDisplayObjects[0].transform,
            &burstPos
        );
        spawnBurstEffect(&burstPos);
        transformVector(
            (s16 *)&gJingleTownBossHoverExitOffsets[4],
            (s16 *)&arg0->boneDisplayObjects[0].transform,
            &burstPos
        );
        spawnBurstEffect(&burstPos);
        transformVector(
            (s16 *)&gJingleTownBossHoverExitOffsets[5],
            (s16 *)&arg0->boneDisplayObjects[0].transform,
            &burstPos
        );
        spawnBurstEffect(&burstPos);
        arg0->unk468 = 0x100;
    }

    arg0->velocity.x -= arg0->velocity.x / 8;
    arg0->velocity.z -= arg0->velocity.z / 8;
    arg0->velocity.y += -0x8000;
    applyPitchAngleDamping(arg0);
    arg0->unkA9E += arg0->unk468;
    if (arg0->unk468 != 0) {
        arg0->unk468 -= 2;
    }
    applyClampedVelocityToPosition(arg0);
    return 0;
}

void updateJingleTownBossPositionAndTrackCollision(Player *arg0) {
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

void updateJingleTownBossModelTransforms(Player *arg0) {
    s32 pad1[8];
    Transform3D scaledMatrix;
    Transform3D pitchYawMatrix;
    s32 sp70[4];

    // Combine orientation and heading into the movement/render basis.
    composeTransform3D(&arg0->orientationTransform, &arg0->headingTransform, &arg0->orientationHeadingTransform);
    // Apply tilt to produce the model transform.
    composeTransform3D(&arg0->tiltTransform, &arg0->orientationHeadingTransform, &arg0->modelTransform);

    if (arg0->behaviorFlags & 0x10) {
        // Apply vertical scale transformation during intro animation
        memcpy(&scaledMatrix, &identityMatrix, sizeof(Transform3D));
        scaledMatrix.m[1][1] = arg0->squashStretchScale;
        composeTransform3D(&scaledMatrix, &arg0->modelTransform, &arg0->boneDisplayObjects[0].transform);
    } else {
        memcpy(&arg0->boneDisplayObjects[0].transform, &arg0->modelTransform, sizeof(Transform3D));
    }

    // Create pitch and yaw rotation matrix for the flying/floating transform
    createCombinedRotationMatrix(&pitchYawMatrix, arg0->jingleBossPitchAngle, arg0->unkA9E);
    pitchYawMatrix.translation.x = 0;
    pitchYawMatrix.translation.z = 0;
    // Y offset: lower (0x140000) during hover exit, higher (0x3A0000) otherwise
    if (arg0->animFlags & 0x200000) {
        pitchYawMatrix.translation.y = 0x140000;
    } else {
        pitchYawMatrix.translation.y = 0x3A0000;
    }

    // Apply rotation/offset to ground transform to get flying transform
    composeTransform3D(&pitchYawMatrix, &arg0->boneDisplayObjects[0].transform, &arg0->boneDisplayObjects[1].transform);

    // Add hover height offset to flying transform
    arg0->boneDisplayObjects[1].transform.translation.y =
        arg0->boneDisplayObjects[1].transform.translation.y + arg0->unk474;

    // Create translation-only matrix for the third transform (unkB0).
    gScaleMatrix.translation.x = 0;
    gScaleMatrix.translation.y = 0x140000;
    gScaleMatrix.translation.z = 0;

    composeTransform3D(&gScaleMatrix, &arg0->boneDisplayObjects[1].transform, &arg0->boneDisplayObjects[2].transform);
}

void renderJingleTownBossWithEffects(Player *arg0) {
    s32 pad10[16];
    Vec3i sp58;
    Vec3i sp68;
    Vec3i sp78;
    s32 sp88;
    u32 sp8C;
    GameState *alloc;
    s32 index;
    s32 i;
    s32 volume;
    s16 angle;
    Vec3i *inputVec;
    Vec3i *outVec1;
    Vec3i *outVec2;

    (void)pad10;

    alloc = getCurrentAllocation();
    updateJingleTownBossModelTransforms(arg0);

    index = arg0->surfaceInfo >> 4;

    if (index == 0) {
        if (arg0->animFlags & 0x200000) {
            for (i = 0; i < 4; i++) {
                enqueuePreLitMultiPartDisplayList(i, &arg0->boneDisplayObjects[1], 2);
            }
        } else {
            for (i = 0; i < 4; i++) {
                enqueuePreLitMultiPartDisplayList(i, &arg0->boneDisplayObjects[0], 3);
            }
        }
    } else {
        if (arg0->animFlags & 0x200000) {
            arg0->boneDisplayObjects[1].light1R = gBossSurfaceColors[index].primaryR;
            arg0->boneDisplayObjects[1].light1G = gBossSurfaceColors[index].primaryG;
            arg0->boneDisplayObjects[1].light1B = gBossSurfaceColors[index].primaryB;
            arg0->boneDisplayObjects[1].light2R = gBossSurfaceColors[index].secondaryR;
            arg0->boneDisplayObjects[1].light2G = gBossSurfaceColors[index].secondaryG;
            arg0->boneDisplayObjects[1].light2B = gBossSurfaceColors[index].secondaryB;

            for (i = 0; i < 4; i++) {
                enqueueMultiPartDisplayList(i, &arg0->boneDisplayObjects[1], 2);
            }
        } else {
            arg0->boneDisplayObjects[0].light1R = gBossSurfaceColors[index].primaryR;
            arg0->boneDisplayObjects[0].light1G = gBossSurfaceColors[index].primaryG;
            arg0->boneDisplayObjects[0].light1B = gBossSurfaceColors[index].primaryB;
            arg0->boneDisplayObjects[0].light2R = gBossSurfaceColors[index].secondaryR;
            arg0->boneDisplayObjects[0].light2G = gBossSurfaceColors[index].secondaryG;
            arg0->boneDisplayObjects[0].light2B = gBossSurfaceColors[index].secondaryB;

            for (i = 0; i < 4; i++) {
                enqueueMultiPartDisplayList(i, &arg0->boneDisplayObjects[0], 3);
            }
        }
    }

    if (alloc->gamePaused != 0) {
        stopSoundEffectChannel(arg0->playerIndex, 0);
        return;
    }

    if (!(arg0->animFlags & 0x10000)) {
        volume = isqrt64(MAGNITUDE_SQ_3D(arg0->velocity.x, arg0->velocity.y, arg0->velocity.z)) >> 12;
        if (volume >= 0x81) {
            volume = 0x80;
        }
        queueSoundAtPositionWithVolumeAndFlags(&arg0->worldPos, 1, 0, 2, arg0->playerIndex, volume);
    } else {
        stopSoundEffectChannel(arg0->playerIndex, 0);
    }

    if (!(arg0->animFlags & 1)) {
        if (isqrt64(MAGNITUDE_SQ_3D(arg0->velocity.x, arg0->velocity.y, arg0->velocity.z)) > 0x40000) {
            s32 temp;

            angle = atan2Fixed(-arg0->velocity.x, -arg0->velocity.z);
            temp = randA();
            inputVec = &sp58;
            sp58.x = (temp & 0xFF) << 13;
            sp58.z = 0;
            sp58.y = 0;
            outVec1 = &sp68;
            rotateVectorY(inputVec, angle, outVec1);
            temp = randA();
            sp58.x = -(temp & 0xFF) << 13;
            outVec2 = &sp78;
            rotateVectorY(inputVec, angle, outVec2);
            sp68.x += arg0->worldPos.x;
            sp68.y += arg0->worldPos.y;
            sp68.z += arg0->worldPos.z;
            sp78.x += arg0->worldPos.x;
            sp78.y += arg0->worldPos.y;
            sp78.z += arg0->worldPos.z;

            temp = arg0->surfaceInfo & 0xF;
            if (temp >= 0) {
                if (temp < 2) {
                    spawnDualSnowSprayEffect_SingleSlot(outVec1, outVec2, &arg0->velocity, 0);
                }
            }
        }
    }
}

/**
 * Updates projected shadow sample positions for the jingle town boss.
 * For each of the 9 samples, computes the X/Z position from the boss's position
 * plus sample-specific offsets, then calculates the track height at that position.
 * Finally enqueues callbacks to render the projected shadow.
 */
void updateJingleTownBossJointPositions(Player *arg0) {
    s32 i;
    GameDataLayout *temp_s5;
    GameState *alloc;

    alloc = getCurrentAllocation();
    i = 0;
    temp_s5 = &alloc->gameData;

    for (i = 0; i < 9; i++) {
        s32 *posPtr;
        u16 temp;

        arg0->shadowSamplePositions[i].x =
            arg0->headingTransform.translation.x + gJingleTownBossHoverExitOffsets[6 + i].x;
        arg0->shadowSamplePositions[i].z =
            arg0->headingTransform.translation.z + gJingleTownBossHoverExitOffsets[6 + i].z;
        posPtr = &arg0->shadowSamplePositions[i].x;
        temp = getOrUpdatePlayerSectorIndex(arg0, temp_s5, arg0->sectorIndex, posPtr);
        arg0->shadowSamplePositions[i].y = getTrackHeightInSector(temp_s5, temp, posPtr, 0x100000);
    }

    arg0->shadowMeshNeedsUpdate = 1;

    for (i = 0; i < 4; i++) {
        enqueueCallbackBySlotIndex((u16)i, 1, renderRacerProjectedShadow, arg0);
    }
}
