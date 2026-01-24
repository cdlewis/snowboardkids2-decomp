#include "9FF70.h"
#include "42170.h"
#include "46080.h"
#include "4CD70.h"
#include "51060.h"
#include "56910.h"
#include "594E0.h"
#include "5AA90.h"
#include "5DBC0.h"
#include "5E590.h"
#include "6E840.h"
#include "A9A40.h"
#include "common.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
#include "levels/starlight_highway.h"
#include "race_session.h"
#include "rand.h"
#include "task_scheduler.h"

extern u16 trickScoreTable[];
extern u16 trickBonusTable[];

/* X and Z offset arrays for player's 9 joint positions */
extern s32 D_800BA348_AA1F8; /* gPlayerJointXOffsets */
extern s32 D_800BA350_AA200; /* gPlayerJointZOffsets */
extern s32 D_800BAB40_AA9F0;
extern s32 D_800BAB44_AA9F4;
extern s32 D_800BAB3C_AA9EC;

s32 tryFinalizeTrickLanding(Player *);
void updateTrickFacingAngle(Player *);
void updateFlipSpinTrickAnimation(Player *);
void updateTrickRotationTransform(Player *);
void decayPlayerAirborneAngles(Player *);
void applyBoostVelocity(Player *);
void decayPlayerSteeringAngles(Player *);
void resetTrickScore(Player *);

typedef void (*RaceFinishBehaviorStepHandler)(void *);
extern RaceFinishBehaviorStepHandler raceFinishBehaviorStepHandlers[];

typedef void (*StunnedBehaviorPhaseHandler)(void *);
extern StunnedBehaviorPhaseHandler stunnedBehaviorPhaseHandlers[];

typedef void (*PostTrickLandingStepHandler)(void *);
extern PostTrickLandingStepHandler postTrickLandingStepHandlers[];

typedef void (*KnockbackBehaviorPhaseHandler)(void *);
extern KnockbackBehaviorPhaseHandler knockbackBehaviorPhaseHandlers[];

typedef void (*KnockbackBehaviorStepHandler)(void *);
extern KnockbackBehaviorStepHandler knockbackBehaviorStepHandlers[];

typedef void (*SharpTurnBehaviorStepHandler)(void *);
extern SharpTurnBehaviorStepHandler sharpTurnBehaviorStepHandlers[];

typedef void (*DefaultBehaviorPhaseHandler)(void *);
extern DefaultBehaviorPhaseHandler D_800BAAD4_AA984[];

typedef struct {
    u8 _pad0[0x5C];
    u8 unk5C;
} GameStatePartial5C;

void initStunnedAirborneBehavior(Player *);

void setPlayerBehaviorMode(Player *player, u8 mode) {
    player->behaviorMode = mode;
    player->behaviorPhase = 0;
    player->behaviorStep = 0;
    player->behaviorCounter = 0;
}

void setPlayerBehaviorPhase(Player *player, s32 phase) {
    player->behaviorPhase = phase;
    player->behaviorStep = 0;
    player->behaviorCounter = 0;
}

void resetPlayerBehaviorToDefault(void *arg) {
    BehaviorState *arg0 = arg;
    arg0->behaviorMode = 1;
    arg0->behaviorPhase = 0;
    arg0->behaviorStep = 0;
    arg0->behaviorCounter = 0;
}

void applyBoostVelocity(Player *player) {
    Vec3i result;
    GameState *gameState;

    gameState = getCurrentAllocation();

    switch (player->boostState) {
        case 1:
        case 7:
            if (player->unkBC9 == 3) {
                transformVector2(gameState->unk48 + 0x1D4, &player->unk9B0.animation_data, &result);
            } else {
                transformVector2(gameState->unk48 + 0xB4, &player->unk9B0.animation_data, &result);
            }
            player->velocity.x += result.x;
            player->velocity.y += result.y;
            player->velocity.z += result.z;
            break;
        case 2:
        case 8:
            if (player->unkBC9 == 3) {
                transformVector2(gameState->unk48 + 0x1E0, &player->unk9B0.animation_data, &result);
            } else {
                transformVector2(gameState->unk48 + 0xC0, &player->unk9B0.animation_data, &result);
            }
            player->velocity.x += result.x;
            player->velocity.y += result.y;
            player->velocity.z += result.z;
            break;
        case 5: {
            s32 velZ;
            s32 resultZ;
            transformVector2(gameState->unk48 + 0xCC, &player->unk9B0.animation_data, &result);
            player->velocity.x += result.x;
            player->velocity.y += result.y;
            velZ = player->velocity.z;
            resultZ = result.z;
            player->boostState = 7;
            player->velocity.z = velZ + resultZ;
            break;
        }
        case 6: {
            s32 velZ;
            s32 resultZ;
            transformVector2(gameState->unk48 + 0xD8, &player->unk9B0.animation_data, &result);
            player->velocity.x += result.x;
            player->velocity.y += result.y;
            velZ = player->velocity.z;
            resultZ = result.z;
            player->boostState = 8;
            player->velocity.z = velZ + resultZ;
            break;
        }
    }

    clampPlayerVelocityToMaxSpeed(player);
    player->worldPos.x += player->velocity.x;
    player->worldPos.y += player->velocity.y;
    player->worldPos.z += player->velocity.z;
}

typedef struct {
    u8 _pad0[0x434];
    Vec3i worldPos;
    u8 _pad440[0xC];
    Vec3i velocity;
    s32 unk458;
} func_800B0300_arg;

void applyClampedVelocityToPosition(Player *player) {
    clampPlayerVelocityToMaxSpeed(player);
    player->worldPos.x = player->worldPos.x + player->velocity.x;
    player->worldPos.y = player->worldPos.y + player->velocity.y;
    player->worldPos.z = player->worldPos.z + player->velocity.z;
}

void applyVelocityToPosition(Player *player) {
    player->worldPos.x = player->worldPos.x + player->velocity.x;
    player->worldPos.y = player->worldPos.y + player->velocity.y;
    player->worldPos.z = player->worldPos.z + player->velocity.z;
}

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B0334_A01E4);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B05B8_A0468);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B0F14_A0DC4);

void dispatchDefaultBehaviorPhase(BehaviorState *arg0) {
    D_800BAAD4_AA984[arg0->behaviorPhase](arg0);
}

s32 updatePlayerFinishWaiting(Player *arg0) {
    GameState *gameState = getCurrentAllocation();

    if (gameState->raceIntroState == 0) {
        arg0->unkAAC = 0;
        if (arg0->behaviorStep != 0) {
            if (arg0->unkB8C < 9) {
                tryActivateFinishBoost(arg0);
            }
        }
        resetPlayerBehaviorToDefault(arg0);
        return 1;
    }

    if (gameState->raceType < 10) {
        if (arg0->behaviorStep == 0) {
            if (arg0->unkB7E & 0x4000) {
                arg0->unkB8C = 0;
                arg0->behaviorStep = arg0->behaviorStep + 1;
            }
        } else {
            arg0->unkB8C = arg0->unkB8C + 1;
        }
    }

    arg0->velocity.x = 0;
    arg0->velocity.z = 0;
    arg0->velocity.y = arg0->velocity.y - arg0->unkAB8;
    applyClampedVelocityToPosition(arg0);
    func_8005D180_5DD80(arg0, 0);
    return 0;
}

s32 updatePlayerSlidingConstrained(Player *player) {
    Vec3i rotatedVelocity;

    if (!(player->unkB84 & 0x20000)) {
        resetPlayerBehaviorToDefault(player);
        return 1;
    }

    if (player->behaviorStep == 0) {
        player->behaviorStep++;
        if (player->velocity.y > 0) {
            player->velocity.y = 0;
        }
        player->unkB8C = 0;
    }

    if (player->unkB8C < 4) {
        player->unkB8C++;
    } else if (player->unkB8C == 4) {
        playTrickSuccessVoice(player);
        player->unkB8C++;
    }

    player->unkA90 = 0;
    player->unkA92 = 0;
    player->unk990.translation.x = 0;

    if (player->unkB84 & 2) {
        player->unkA8E = player->unkBB0;
        player->rotY = player->unkBB2;
    } else {
        player->unkA8E = -player->unkBB0;
        player->rotY = player->unkBB2 + 0x1000;
    }

    rotateVectorY(&player->velocity.x, -player->unkBB2, &rotatedVelocity);
    rotatedVelocity.x = 0;
    rotateVectorY(&rotatedVelocity, player->unkBB2, &player->velocity);

    player->velocity.y = player->velocity.y - player->unkAB8;
    applyClampedVelocityToPosition(player);

    if (player->behaviorCounter == 0) {
        if (func_8005D308_5DF08(player, 0x21) != 0) {
            player->behaviorCounter++;
        }
    } else {
        func_8005D180_5DD80(player, 0x22);
    }

    setPlayerBodyPartAnimState(player, 1, 0);

    return 0;
}

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B1544_A13F4);

s32 updatePlayerGroundedSliding(Player *player) {
    s16 currentAngle;
    s32 angleDelta;
    s32 velocityX;
    s32 velocityZ;
    u8 groundTimer;

    if (player->unkB7C & 0xF) {
        setPlayerBehaviorPhase(player, 4);
        player->behaviorStep = 5;
        resetTrickScore(player);
        return 1;
    }
    if (!(player->unkB84 & 1)) {
        if (player->behaviorCounter >= 0xBU) {
            setPlayerBehaviorPhase(player, 3);
        } else {
            setPlayerBehaviorPhase(player, 0);
        }
        return 1;
    }
    groundTimer = player->behaviorCounter;
    if (groundTimer < 0x64U) {
        player->behaviorCounter = groundTimer + 1;
    }
    if (player->unkBDA != 0) {
        calculateAITargetPosition(player);
        angleDelta =
            computeAngleToPosition(player->aiTargetX, player->aiTargetZ, player->worldPos.x, player->worldPos.z);
        currentAngle = player->rotY;
        angleDelta = (angleDelta - currentAngle) & 0x1FFF;
        if (angleDelta >= 0x1001) {
            angleDelta -= 0x2000;
        }
        if (angleDelta >= 0x41) {
            angleDelta = 0x40;
        }
        if (angleDelta < -0x40) {
            angleDelta = -0x40;
        }
        player->rotY = currentAngle + angleDelta;
    } else {
        player->rotY = (u16)player->rotY - (player->unkB7A * 4);
    }
    velocityX = player->velocity.x;
    velocityZ = player->velocity.z;
    player->velocity.y -= player->unkAB8;
    player->velocity.x = velocityX - (velocityX >> 7);
    player->velocity.z = velocityZ - (velocityZ >> 7);
    if (player->behaviorCounter >= 0xBU) {
        decayPlayerAirborneAngles(player);
    }
    applyBoostVelocity(player);
    angleDelta = player->unkA92;
    if (angleDelta >= 0) {
        if (angleDelta >= 0x401) {
            angleDelta = 0x400;
        }
        func_8005D48C_5E08C(player, 2, angleDelta / 2);
    } else {
        angleDelta = -angleDelta;
        if (angleDelta >= 0x401) {
            angleDelta = 0x400;
        }
        func_8005D48C_5E08C(player, 1, angleDelta / 2);
    }
    func_80058CFC_598FC(player);
    return 0;
}

typedef struct {
    u8 _pad0[0xB84];
    s32 unkB84;
    u8 _padB88[0x10]; // 0xB88 to 0xB98
    s16 unkB98;
    u8 _padB9A[0x28]; // 0xB9A to 0xBC2
    u8 unkBC2;
    u8 _padBC3[0x17]; // 0xBC3 to 0xBDA
    u8 unkBDA;
} shouldInitiateSharpTurn_arg;

s32 shouldInitiateSharpTurn(shouldInitiateSharpTurn_arg *player, s32 steeringValue) {
    if (player->unkBDA != 0) {
        goto end;
    }
    if (player->unkB98 == 0) {
        goto end;
    }
    if (player->unkBC2 != 0) {
        goto end;
    }
    if (0x8000 < steeringValue) {
        if (player->unkB84 & 2) {
            return 1;
        }
    }
    if (steeringValue < -0x8000) {
        if (player->unkB84 & 2) {
            goto end;
        }
        return 1;
    }
end:
    return 0;
}

void dispatchSharpTurnBehaviorStep(BehaviorState *arg0) {
    sharpTurnBehaviorStepHandlers[arg0->behaviorStep](arg0);
}

typedef struct {
    u8 _pad0[0xB8C];
    s32 unkB8C;
    u8 _padB90[0x2F];
    u8 behaviorStep;
} initSharpTurnSlidingStep_arg;

s32 initSharpTurnSlidingStep(initSharpTurnSlidingStep_arg *player) {
    player->unkB8C = 2;
    player->behaviorStep = player->behaviorStep + 1;
    return 1;
}

extern s32 applyVelocityDeadzone(Player *, s32, s32, s32);

s32 updateSharpTurnSlidingStep(Player *player) {
    s16 steeringAngle;
    s16 clampedAngle;
    s32 frameCounter;

    if (player->unkB84 & 0x20000) {
        setPlayerBehaviorPhase(player, 6);
        return 1;
    }

    if (tryActivateShortcut(player) != 0) {
        initKnockbackBehavior((BehaviorState *)player);
        return 1;
    }

    if (player->unkB84 & 1) {
        setPlayerBehaviorPhase(player, 1);
        return 1;
    }

    player->velocity.y = player->velocity.y - player->unkAB8;
    decayPlayerSteeringAngles(player);
    applyVelocityDeadzone(player, 0x200, 0x200, player->unkAB0);
    applyBoostVelocity(player);

    steeringAngle = player->unkA92;
    clampedAngle = steeringAngle;
    if (steeringAngle >= 0) {
        if (steeringAngle >= 0x401) {
            clampedAngle = 0x400;
        }
        func_8005D48C_5E08C(player, 2, clampedAngle / 2);
    } else {
        clampedAngle = -clampedAngle;
        if (clampedAngle >= 0x401) {
            clampedAngle = 0x400;
        }
        func_8005D48C_5E08C(player, 1, clampedAngle / 2);
    }

    frameCounter = player->unkB8C - 1;
    player->unkB8C = frameCounter;
    if (frameCounter == 0) {
        player->behaviorStep = player->behaviorStep + 1;
    }

    func_80058CFC_598FC(player);
    return 0;
}

typedef struct {
    u8 _pad0[0xB84];
    s32 unkB84;
    u8 _padB88[0x37];
    u8 behaviorStep;
} endSharpTurnSlidingStep_arg;

s32 endSharpTurnSlidingStep(endSharpTurnSlidingStep_arg *player) {
    s32 flags;

    player->behaviorStep = player->behaviorStep + 1;
    flags = player->unkB84;

    if (flags & 2) {
        player->unkB84 = flags & ~2;
    } else {
        player->unkB84 = flags | 2;
    }

    return 1;
}

s32 recoverSharpTurnSlidingStep(Player *player) {
    if (player->unkB84 & 0x20000) {
        setPlayerBehaviorPhase(player, 6);
        return 1;
    }

    if (tryActivateShortcut(player) != 0) {
        initKnockbackBehavior((BehaviorState *)player);
        return 1;
    }

    if (player->unkB84 & 1) {
        setPlayerBehaviorPhase(player, 1);
        return 1;
    }

    player->velocity.y = player->velocity.y - player->unkAB8;
    decayPlayerSteeringAngles(player);
    applyVelocityDeadzone(player, 0x200, 0x200, player->unkAB0);
    applyBoostVelocity(player);

    if (func_8005D308_5DF08(player, 6) != 0) {
        player->unkBC2 = 6;
        setPlayerBehaviorPhase(player, 0);
    }

    func_80058CFC_598FC(player);
    return 0;
}

extern void startRumbleEffect(Player *player, s32 effectType);

s32 initPostTrickLandingStep(Player *player) {
    u8 behaviorStep;

    if (player->unkB84 & 0x20000) {
        setPlayerBehaviorPhase(player, 6);
        startRumbleEffect(player, 0);
        queueSoundAtPosition(&player->worldPos, 0x25);
        return 1;
    }

    if (tryActivateShortcut(player) != 0) {
        initKnockbackBehavior((BehaviorState *)player);
        startRumbleEffect(player, 0);
        queueSoundAtPosition(&player->worldPos, 0x25);
        return 1;
    }

    if (player->unkB84 & 1) {
        setPlayerBehaviorPhase(player, 1);
        startRumbleEffect(player, 0);
        queueSoundAtPosition(&player->worldPos, 0x25);
        return 1;
    }

    player->velocity.y -= player->unkAB8;
    decayPlayerSteeringAngles(player);
    applyVelocityDeadzone(player, 0x200, 0x200, player->unkAB0);
    applyBoostVelocity(player);

    behaviorStep = player->behaviorStep;
    if (behaviorStep == 0) {
        player->behaviorStep = ++behaviorStep;
        startRumbleEffect(player, 0);
        if ((player->unkBCC & 0xF) == 7) {
            queueSoundAtPosition(&player->worldPos, 0x29);
        }
        if ((player->unkBCC & 0xF) == 3) {
            spawnCharacterAttackEffect(player);
            queueSoundAtPosition(&player->worldPos, 0xF);
        } else {
            queueSoundAtPosition(&player->worldPos, 0x25);
        }
    }

    if (func_8005D308_5DF08(player, 5) != 0) {
        setPlayerBehaviorPhase(player, 0);
    }

    func_80058CFC_598FC(player);
    return 0;
}

void dispatchPostTrickLandingStep(BehaviorState *arg0) {
    postTrickLandingStepHandlers[arg0->behaviorStep](arg0);
}

s32 beginPostTrickSlidingStep(Player *player) {
    player->unkB8C = 4;
    player->behaviorStep += 1;
    player->rotY += player->unkA90;
    player->unkA90 = 0;
    onTrickCompletedHook(player);
    player->trickCount = 0;
    player->unkBCD = -1;
    return 1;
}

s32 updatePostTrickSlidingStep(Player *player) {
    s32 steeringValue;

    if (player->unkB84 & 0x20000) {
        setPlayerBehaviorPhase(player, 6);
        return 1;
    }

    if (tryActivateShortcut(player) != 0) {
        initKnockbackBehavior((BehaviorState *)player);
        return 1;
    }

    if (player->unkB84 & 1) {
        setPlayerBehaviorPhase(player, 1);
        return 1;
    }

    player->velocity.y -= player->unkAB8;
    decayPlayerSteeringAngles(player);
    steeringValue = applyVelocityDeadzone(player, 0x200, 0x200, player->unkAB0);
    applyBoostVelocity(player);

    if (!(player->unkB7C & 0x8000)) {
        player->behaviorCounter = 1;
    }

    getTrickType(player);
    func_8005D308_5DF08(player, 3);

    player->unkB8C -= 1;
    if (player->unkB8C == 0) {
        player->behaviorStep += 1;
    }

    if (shouldInitiateSharpTurn((shouldInitiateSharpTurn_arg *)player, steeringValue) != 0) {
        setPlayerBehaviorPhase(player, 2);
    }

    func_80058CFC_598FC(player);
    return 0;
}

s32 updatePostTrickChargingStep(Player *player) {
    s32 steeringValue;

    if (player->unkB84 & 0x20000) {
        setPlayerBehaviorPhase(player, 6);
        return 1;
    }

    if (tryActivateShortcut(player) != 0) {
        initKnockbackBehavior((BehaviorState *)player);
        return 1;
    }

    if (player->unkB84 & 1) {
        setPlayerBehaviorPhase(player, 1);
        return 1;
    }

    player->velocity.y -= player->unkAB8;
    decayPlayerSteeringAngles(player);
    steeringValue = applyVelocityDeadzone(player, 0x200, 0x200, player->unkAB0);
    applyBoostVelocity(player);

    if (!(player->unkB7C & 0x8000)) {
        if (player->unkBDA != 0 && player->unkBDC != 0) {
            player->unkB8C = 0x10000;
        }
        player->behaviorCounter = 1;
        if (player->unkB8C > 0xFFFF) {
            player->unkBCD = getTrickType(player);
        }
    } else {
        getTrickType(player);
        if (player->unkB8C <= 0xFFFF) {
            player->unkB8C += 0x4000;
        }
    }

    if (player->behaviorCounter != 0) {
        player->behaviorStep += 1;
    }

    func_8005D308_5DF08(player, 3);

    if (shouldInitiateSharpTurn((shouldInitiateSharpTurn_arg *)player, steeringValue) != 0) {
        setPlayerBehaviorPhase(player, 2);
    }

    func_80058CFC_598FC(player);
    return 0;
}

s32 beginPostTrickLaunchStep(Player *player) {
    s32 rotationTemp1[8];
    s32 rotationTemp2[8];
    Vec3i launchVelocity;
    s32 *launchMagnitudePtr;

    player->behaviorCounter = 3;
    player->behaviorStep++;
    D_800BAB40_AA9F0 = player->unkB8C + player->unkABC;

    if (*(volatile s32 *)&player->unkB84 & 2) {
        D_800BAB44_AA9F4 = 0xFFFE0000;
    } else {
        D_800BAB44_AA9F4 = 0x20000;
    }

    if (player->unkBDA != 0) {
        if (player->unkBDC == 0) {
            if (player->unkB98 != 0) {
                launchMagnitudePtr = &D_800BAB44_AA9F4;
                *launchMagnitudePtr = *launchMagnitudePtr * 2;
            }
        }
    } else {
        if (*(u16 *)&player->unkB7A == 7) {
            launchMagnitudePtr = &D_800BAB44_AA9F4;
            *launchMagnitudePtr = *launchMagnitudePtr * 2;
        }
    }

    func_8006BDBC_6C9BC((BoneAnimationState *)&player->unk990, &player->unk970, rotationTemp1);
    func_8006BDBC_6C9BC(&player->unk9B0, rotationTemp1, rotationTemp2);
    transformVector2(&D_800BAB3C_AA9EC, rotationTemp2, &launchVelocity);

    player->velocity.x += launchVelocity.x;
    player->velocity.y += launchVelocity.y + player->unkAB8;
    player->velocity.z += launchVelocity.z;

    return 1;
}

void resetTrickScore(Player *player) {
    player->trickScore = 0;
    player->trickPoints = 0;
    player->tricksPerformedMask = 0;
    player->trickCount = 0;
    player->unkBD5 = 0;
    player->spinsPerformedMask = 0;
}

void addTrickScore(Player *player, s8 trickType) {
    player->trickScore = player->trickScore + trickScoreTable[player->trickCount];
    player->trickPoints = player->trickPoints + trickBonusTable[player->trickCount];

    if (player->tricksPerformedMask == 0) {
        player->tricksPerformedMask = player->tricksPerformedMask | (1 << trickType);
    } else {
        if (!((player->tricksPerformedMask >> trickType) & 1)) {
            player->trickScore += 100;
            player->trickPoints += 10;
        }

        player->tricksPerformedMask |= (1 << trickType);

        if (player->tricksPerformedMask == 0xFF) {
            player->trickScore += 1300;
            player->trickPoints += 565;
            player->tricksPerformedMask |= 0x100;
        }
    }

    player->trickCount++;
    if (player->trickCount >= 8) {
        player->trickCount = 7;
    }

    if (player->trickScore >= 10000) {
        player->trickScore = 9999;
    }

    if (player->trickPoints >= 1000) {
        player->trickPoints = 999;
    }
}

void addSpinTrickScore(Player *player, s32 spinDirection) {
    s8 currentMask;
    s8 temp_v1;

    player->trickScore += 0x1E;
    player->trickPoints += 1;
    currentMask = player->spinsPerformedMask;
    temp_v1 = currentMask;

    if (currentMask == 0) {
        player->spinsPerformedMask = temp_v1 | (1 << spinDirection);
    } else {
        if (!((currentMask >> (s8)spinDirection) & 1)) {
            player->trickScore += 0x14;
            player->trickPoints += 1;
        }

        player->spinsPerformedMask = player->spinsPerformedMask | (1 << (s8)spinDirection);

        if ((s8)player->spinsPerformedMask == 0xF) {
            player->trickScore += 0x14;
            player->trickPoints += 3;
            player->spinsPerformedMask |= 0x10;
        }
    }

    if (player->trickScore >= 10000) {
        player->trickScore = 9999;
    }

    if (player->trickPoints >= 1000) {
        player->trickPoints = 999;
    }
}

s32 updatePostTrickDescentStep(Player *player) {
    s32 pad[16];

    if (player->unkB84 & 1) {
        if (player->behaviorCounter < 3) {
            resetTrickScore(player);
            if (player->unkBCD < 0) {
                player->behaviorCounter = 0;
                player->behaviorStep = player->behaviorStep + 1;
                return 1;
            } else {
                player->behaviorStep = player->unkBCD + 6;
                player->behaviorCounter = 0;
                playTrickLandingVoice(player);
                return 1;
            }
        } else {
            setPlayerBehaviorPhase(player, 1);
            return 1;
        }
    } else {
        player->velocity.y = player->velocity.y - player->unkAB8;
        decayPlayerSteeringAngles(player);
        applyVelocityDeadzone(player, 0x200, 0x200, player->unkAB0);
        applyBoostVelocity(player);
        player->behaviorCounter = player->behaviorCounter - 1;
        if (player->behaviorCounter == 0) {
            setPlayerBehaviorPhase(player, 0);
        }
        func_8005D308_5DF08(player, 3);
        func_80058CFC_598FC(player);
        return 0;
    }
}

void updateTrickFacingAngle(Player *player) {
    s32 playerFlags;
    u16 currentAngle;
    s16 angleDelta;

    playerFlags = player->unkB84;
    if (!(playerFlags & 0x1000)) {
        if (player->unkBDA != 0) {
            calculateAITargetPosition(player);
            angleDelta =
                computeAngleToPosition(player->aiTargetX, player->aiTargetZ, player->worldPos.x, player->worldPos.z);
            currentAngle = (u16)player->rotY;
            angleDelta = (angleDelta - currentAngle) & 0x1FFF;
            if (angleDelta >= 0x1001) {
                angleDelta = angleDelta | 0xE000;
            }
            if (angleDelta >= 0x41) {
                angleDelta = 0x40;
            }
            if (angleDelta < -0x40) {
                angleDelta = -0x40;
            }
            player->rotY = currentAngle + angleDelta;
        } else {
            player->rotY = (u16)player->rotY - (player->unkB7A * 4);
        }
        func_80058CFC_598FC(player);
        return;
    }
    if (playerFlags & 0x4000) {
        spawnSkiTrailTask(player);
    }
}

s32 tryFinalizeTrickLanding(Player *player) {
    GameState *state;
    s32 playerFlags;
    u8 gameMode;
    s16 soundId;
    s16 scoreValue;

    state = getCurrentAllocation();
    playerFlags = player->unkB84;

    if (playerFlags & 1) {
        return 0;
    }

    if (playerFlags & 0x1000) {
        initStunnedAirborneBehavior(player);
        return 1;
    }

    if (playerFlags & 0x80000) {
        goto skip_to_end;
    }

    gameMode = state->raceType;
    if (gameMode != 0 && gameMode != 8) {
        if (!(gameMode == 9 || gameMode == 10)) {
            player->trickScore = 0;
        }
    }

    if (player->trickScore != 0) {
        addPlayerRaceGold(player, player->trickScore);
        if (player->isBossRacer == 0) {
            showTrickScoreDisplay(player->playerIndex, player->trickScore);

            scoreValue = player->trickScore;
            if (scoreValue < 0x96) {
                soundId = 0x11C;
            } else {
                soundId = 0x11D;
            }

            if (scoreValue >= 0x12C) {
                soundId = 0x11E;
            }
            if (scoreValue >= 0x190) {
                soundId = 0x11F;
            }
            if (scoreValue >= 0x1F4) {
                soundId = 0x120;
            }
            if (scoreValue >= 0xBB8) {
                soundId = 0x121;
            }

            playSoundEffectWithPriorityDefaultVolume(soundId, 6);
        }
    }

    if (state->raceType == 6) {
        if (player->trickPoints != 0) {
            showTrickPointsDisplay(player->trickPoints);
            addPlayerSkillPoints(player, player->trickPoints);

            scoreValue = player->trickPoints;
            if (scoreValue < 0xF) {
                soundId = 0x11C;
            } else {
                soundId = 0x11D;
            }

            if (scoreValue >= 0x1E) {
                soundId = 0x11E;
            }
            if (scoreValue >= 0x37) {
                soundId = 0x11F;
            }
            if (scoreValue >= 0x5F) {
                soundId = 0x120;
            }
            if (scoreValue >= 0x3E7) {
                soundId = 0x121;
            }

            playSoundEffectWithPriorityDefaultVolume(soundId, 6);
        }
    }

skip_to_end:
    if (player->tricksPerformedMask != 0) {
        playTrickSuccessVoice(player);
        setPlayerBodyPartAnimState(player, 1, 0xF);
    }

    setPlayerBehaviorPhase(player, 3);
    return 1;
}

void updateTrickAirborneVelocity(Player *player) {
    player->velocity.x = player->velocity.x - (player->velocity.x >> 7);
    player->velocity.y = player->velocity.y - player->unkAB8;
    player->velocity.z = player->velocity.z - (player->velocity.z >> 7);
    decayPlayerAirborneAngles(player);
    applyBoostVelocity(player);
}

s32 updateBasicTrickAirborne(Player *player) {
    if (tryFinalizeTrickLanding(player) != 0) {
        return 1;
    }
    updateTrickAirborneVelocity(player);
    updateFlipSpinTrickAnimation(player);
    updateTrickFacingAngle(player);
    return 0;
}

void beginSpinTrick(Player *player) {
    player->unkB8C = 0x200;
    player->unkA96 = 0;
    player->unkA98 = 0;
    player->unkA9A = 0;
    player->unkBCD = -1;
    player->behaviorCounter++;
    player->unkB84 |= 0x4000;
    onTrickCompletedHook(player);
    queueSoundAtPosition(&player->worldPos, 0xB);
}

void updateSpinTrickState(Player *player, s8 trickType) {
    switch (player->behaviorCounter) {
        case 0:
            beginSpinTrick(player);
            addTrickScore(player, trickType);
            break;
        case 1:
            if (player->unkBDA != 0 || (player->unkB7C & 0x8000)) {
                player->behaviorCounter = player->behaviorCounter + 1;
            }
            break;
        case 2:
            if (player->unkBDA != 0 || (!(player->unkB7C & 0x8000) && player->unkB8C == 0x200)) {
                player->unkBCD = getTrickType(player);
            } else if (player->unkB7C & 0x8000) {
                getTrickType(player);
            }
            break;
    }
}

s32 updateRightSpinTrick(Player *player) {
    if (tryFinalizeTrickLanding(player) != 0) {
        return 1;
    }

    updateSpinTrickState(player, 0);
    updateTrickAirborneVelocity(player);

    if (player->unkB8C != 0) {
        if (player->unkA96 >= 0x1001) {
            if (player->unkBCD == -1) {
                player->unkB8C = player->unkB8C - 0x22;
            }
        }

        player->unkA96 = player->unkA96 + player->unkB8C;

        if (player->unkA96 >= 0x2000) {
            player->unkB8C = 0;
            if (player->unkBCD < 0) {
                player->unkB84 = player->unkB84 & ~0x4000;
            } else {
                player->behaviorStep = player->unkBCD + 6;
                player->behaviorCounter = 0;
            }
        } else {
            updateTrickRotationTransform(player);
        }
    }

    updateFlipSpinTrickAnimation(player);
    updateTrickFacingAngle(player);
    return 0;
}

s32 updateLeftSpinTrick(Player *player) {
    if (tryFinalizeTrickLanding(player) != 0) {
        return 1;
    }

    updateSpinTrickState(player, 1);
    updateTrickAirborneVelocity(player);

    if (player->unkB8C != 0) {
        if (player->unkA96 < -0x1000) {
            if (player->unkBCD == -1) {
                player->unkB8C = player->unkB8C - 0x22;
            }
        }

        player->unkA96 = player->unkA96 - player->unkB8C;

        if (player->unkA96 < -0x1FFF) {
            player->unkB8C = 0;
            if (player->unkBCD < 0) {
                player->unkB84 = player->unkB84 & ~0x4000;
            } else {
                player->behaviorStep = player->unkBCD + 6;
                player->behaviorCounter = 0;
            }
        } else {
            updateTrickRotationTransform(player);
        }
    }

    updateFlipSpinTrickAnimation(player);
    updateTrickFacingAngle(player);
    return 0;
}

s32 updateForwardFlipTrick(Player *player) {
    if (tryFinalizeTrickLanding(player) != 0) {
        return 1;
    }

    updateSpinTrickState(player, 2);
    updateTrickAirborneVelocity(player);

    if (player->unkB8C != 0) {
        if (player->unkA98 < -0x1000) {
            if (player->unkBCD == -1) {
                player->unkB8C = player->unkB8C - 0x22;
            }
        }

        player->unkA98 = player->unkA98 - player->unkB8C;

        if (player->unkA98 < -0x1FFF) {
            player->unkB8C = 0;
            if (player->unkBCD < 0) {
                player->unkB84 = player->unkB84 & ~0x4000;
            } else {
                player->behaviorStep = player->unkBCD + 6;
                player->behaviorCounter = 0;
            }
        } else {
            updateTrickRotationTransform(player);
        }
    }

    updateFlipSpinTrickAnimation(player);
    updateTrickFacingAngle(player);
    return 0;
}

s32 updateBackwardFlipTrick(Player *player) {
    if (tryFinalizeTrickLanding(player) != 0) {
        return 1;
    }

    updateSpinTrickState(player, 3);
    updateTrickAirborneVelocity(player);

    if (player->unkB8C != 0) {
        if (player->unkA98 >= 0x1001) {
            if (player->unkBCD == -1) {
                player->unkB8C = player->unkB8C - 0x22;
            }
        }

        player->unkA98 = player->unkA98 + player->unkB8C;

        if (player->unkA98 >= 0x2000) {
            player->unkB8C = 0;
            if (player->unkBCD < 0) {
                player->unkB84 = player->unkB84 & ~0x4000;
            } else {
                player->behaviorStep = player->unkBCD + 6;
                player->behaviorCounter = 0;
            }
        } else {
            updateTrickRotationTransform(player);
        }
    }

    updateFlipSpinTrickAnimation(player);
    updateTrickFacingAngle(player);
    return 0;
}

s32 updateRightForwardFlipTrick(Player *player) {
    if (tryFinalizeTrickLanding(player) != 0) {
        return 1;
    }

    updateSpinTrickState(player, 4);
    updateTrickAirborneVelocity(player);

    if (player->unkB8C != 0) {
        if (player->unkA96 >= 0x1001) {
            if (player->unkBCD == -1) {
                player->unkB8C = player->unkB8C - 0x22;
            }
        }

        player->unkA96 = player->unkA96 + player->unkB8C;
        player->unkA98 = player->unkA98 - player->unkB8C;

        if (player->unkA96 >= 0x2000) {
            player->unkB8C = 0;
            if (player->unkBCD < 0) {
                player->unkB84 = player->unkB84 & ~0x4000;
            } else {
                player->behaviorStep = player->unkBCD + 6;
                player->behaviorCounter = 0;
            }
        } else {
            updateTrickRotationTransform(player);
        }
    }

    updateFlipSpinTrickAnimation(player);
    updateTrickFacingAngle(player);
    return 0;
}

s32 updateRightBackwardFlipTrick(Player *player) {
    if (tryFinalizeTrickLanding(player) != 0) {
        return 1;
    }

    updateSpinTrickState(player, 5);
    updateTrickAirborneVelocity(player);

    if (player->unkB8C != 0) {
        if (player->unkA96 >= 0x1001) {
            if (player->unkBCD == -1) {
                player->unkB8C = player->unkB8C - 0x22;
            }
        }

        player->unkA96 = player->unkA96 + player->unkB8C;
        player->unkA98 = player->unkA98 + player->unkB8C;

        if (player->unkA96 >= 0x2000) {
            player->unkB8C = 0;
            if (player->unkBCD < 0) {
                player->unkB84 = player->unkB84 & ~0x4000;
            } else {
                player->behaviorStep = player->unkBCD + 6;
                player->behaviorCounter = 0;
            }
        } else {
            updateTrickRotationTransform(player);
        }
    }

    updateFlipSpinTrickAnimation(player);
    updateTrickFacingAngle(player);
    return 0;
}

s32 updateLeftForwardFlipTrick(Player *player) {
    if (tryFinalizeTrickLanding(player) != 0) {
        return 1;
    }

    updateSpinTrickState(player, 6);
    updateTrickAirborneVelocity(player);

    if (player->unkB8C != 0) {
        if (player->unkA96 < -0x1000) {
            if (player->unkBCD == -1) {
                player->unkB8C = player->unkB8C - 0x22;
            }
        }

        player->unkA96 = player->unkA96 - player->unkB8C;
        player->unkA98 = player->unkA98 - player->unkB8C;

        if (player->unkA96 < -0x1FFF) {
            player->unkB8C = 0;
            if (player->unkBCD < 0) {
                player->unkB84 = player->unkB84 & ~0x4000;
            } else {
                player->behaviorStep = player->unkBCD + 6;
                player->behaviorCounter = 0;
            }
        } else {
            updateTrickRotationTransform(player);
        }
    }

    updateFlipSpinTrickAnimation(player);
    updateTrickFacingAngle(player);
    return 0;
}

s32 updateLeftBackwardFlipTrick(Player *player) {
    if (tryFinalizeTrickLanding(player) != 0) {
        return 1;
    }

    updateSpinTrickState(player, 7);
    updateTrickAirborneVelocity(player);

    if (player->unkB8C != 0) {
        if (player->unkA96 < -0x1000) {
            if (player->unkBCD == -1) {
                player->unkB8C = player->unkB8C - 0x22;
            }
        }

        player->unkA96 = player->unkA96 - player->unkB8C;
        player->unkA98 = player->unkA98 + player->unkB8C;

        if (player->unkA96 < -0x1FFF) {
            player->unkB8C = 0;
            if (player->unkBCD < 0) {
                player->unkB84 = player->unkB84 & ~0x4000;
            } else {
                player->behaviorStep = player->unkBCD + 6;
                player->behaviorCounter = 0;
            }
        } else {
            updateTrickRotationTransform(player);
        }
    }

    updateFlipSpinTrickAnimation(player);
    updateTrickFacingAngle(player);
    return 0;
}

s32 getInputSpinTrickAnimId(Player *player) {
    u16 buttonsPressed = player->unkB7E;
    s32 animId;

    if (buttonsPressed & 0x8) {
        animId = 0x17;
    } else {
        animId = 0;
    }

    if (buttonsPressed & 0x4) {
        animId = 0x19;
    }

    if (buttonsPressed & 0x1) {
        animId = 0x15;
        if (player->unkB84 & 0x2) {
            animId = 0x13;
        }
    }

    if ((player->unkB7E & 0x2)) {
        animId = 0x13;
        if (player->unkB84 & 0x2) {
            animId = 0x15;
        }
    }

    if (player->unkBDA != 0) {
        u8 trickFlags = player->unkBDC;
        if (trickFlags & 0x8) {
            player->unkBDC = trickFlags & 0xF7;
            animId = 0x15;
        }
    }

    return animId;
}

void updateFlipSpinTrickAnimation(Player *player) {
    s32 animId;
    u8 temp;

    switch (player->unkBD5) {
        default:
            break;

        case 0:
            player->unkBD5++;
            player->unkBD6 = 4;
        case 1:
            animId = getInputSpinTrickAnimId(player);
            if (animId != 0) {
                player->unkBD6 = animId;
                player->unkBD7 = 0;
                player->unkBD5 = player->unkBD5 + 1;
                player->unkB84 |= 0x8000;
            }
            func_8005D308_5DF08(player, player->unkBD6);
            break;

        case 2:
            animId = func_8005D308_5DF08(player, player->unkBD6);
            if (animId != 0) {
                if (player->unkBD6 == 0x17) {
                    addSpinTrickScore(player, 0);
                }

                if (player->unkBD6 == 0x19) {
                    addSpinTrickScore(player, 1);
                }

                if (player->unkBD6 == 0x15) {
                    addSpinTrickScore(player, 2);
                }

                if (player->unkBD6 == 0x13) {
                    addSpinTrickScore(player, 3);
                }

                player->unkBD5++;
                player->unkBD6++;
            }

            animId = getInputSpinTrickAnimId(player);
            if (animId != 0) {
                player->unkBD7 = animId;
            }

            break;

        case 3:
            animId = func_8005D308_5DF08(player, player->unkBD6);
            if (animId != 0) {
                if (player->unkBD7 != 0) {
                    player->unkBD5 = 2;
                    player->unkBD6 = player->unkBD7;
                    player->unkBD7 = 0;
                    break;
                } else {
                    player->unkB84 &= 0xFFFF7FFF;
                    player->unkBD5 = 1;
                }
            }

            animId = getInputSpinTrickAnimId(player);
            if (animId != 0) {
                player->unkBD7 = animId;
            }
            break;
    }

    if (player->unkB84 & 0xC000) {
        player->unkB84 |= 0x1000;
    } else {
        player->unkB84 &= ~0x1000;
    }
}

void dispatchRaceFinishBehaviorStep(BehaviorState *arg0) {
    raceFinishBehaviorStepHandlers[arg0->behaviorStep](arg0);
}

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B3980_A3830);

typedef struct {
    u8 pad[0x176C];
    s32 unk176C;
} GameStateUnk10;

typedef struct {
    u8 pad[0x10];
    GameStateUnk10 *unk10;
    u8 pad2[0x44];
    u8 unk58;
    u8 unk59;
    u8 unk5A;
    u8 pad3[0x1F];
    u8 raceType;
    u8 showResultHUD;
    u8 unk7C;
    u8 playerLost;
} GameStateLocal;

s32 updateRaceFinishWaitingStep(Player *player) {
    s32 playerWon = 0;
    GameStateLocal *gameState;

    gameState = getCurrentAllocation();
    player->velocity.x = 0;
    player->velocity.z = 0;
    player->velocity.y -= 0x6000;
    decayPlayerSteeringAngles(player);
    applyClampedVelocityToPosition(player);
    func_8005D180_5DD80(player, 0);
    if (gameState->showResultHUD != 0) {
        switch (gameState->raceType) {
            case 0:
            case 1:
            case 8:
            case 9:
                if (player->finishPosition == 0) {
                    playerWon = 1;
                }
                break;
            case 2:
            case 3:
                if (gameState->unk10->unk176C & 0x100000) {
                    playerWon = 1;
                }
                break;
            case 4:
                if (gameState->playerLost == 0) {
                    playerWon = 1;
                }
                break;
            case 5:
                if ((gameState->playerLost == 0) && (gameState->unk5A == 0x14)) {
                    playerWon = 1;
                }
                break;
            case 6:
                if ((gameState->playerLost == 0) && (player->skillPoints >= 0x12C)) {
                    playerWon = 1;
                }
                break;
        }
        if (playerWon != 0) {
            player->behaviorStep = 2;
        } else {
            player->behaviorStep = 3;
        }
        player->behaviorCounter = 0;
    }
    return 0;
}

s32 updateRaceFinishWinStep(Player *player) {
    player->velocity.x = 0;
    player->velocity.z = 0;
    player->velocity.y = player->velocity.y - 0x6000;
    decayPlayerSteeringAngles(player);
    applyClampedVelocityToPosition(player);

    if (player->collisionRadius <= 0xDFFFF) {
        player->collisionRadius = player->collisionRadius + 0x8000;
    }

    switch (player->behaviorCounter) {
        case 0:
            if (func_8005D308_5DF08(player, 0x1B) != 0) {
                player->behaviorCounter = player->behaviorCounter + 1;
                playRaceWinVoice(player);
            }
            break;
        case 1:
            if (func_8005D308_5DF08(player, 0x1D) != 0) {
                player->unkB8C = 0x14;
                player->behaviorCounter = player->behaviorCounter + 1;
            }
            break;
        case 2:
            if (player->unkB8C != 0) {
                player->unkB8C = player->unkB8C - 1;
            } else {
                player->behaviorCounter = player->behaviorCounter + 1;
            }
            break;
        case 3:
            if (func_8005D308_5DF08(player, 0x1C) == 0) {
                goto end;
            }
            player->behaviorCounter = player->behaviorCounter + 1;
            break;
        case 4:
            if (func_8005D308_5DF08(player, 0x1D) != 0) {
                player->behaviorCounter = 2;
                player->unkB8C = 0x14;
            }
            break;
    }

end:
    setPlayerBodyPartAnimState(player, 1, 0);
    return 0;
}

s32 updateRaceFinishLoseStep(Player *player) {
    u32 flags;
    s32 velocityY;
    s32 temp;
    u8 state;
    s32 collisionRadius;

    flags = player->unkB84;
    velocityY = player->velocity.y;

    player->velocity.x = 0;
    player->velocity.z = 0;

    flags |= 0x60;
    velocityY += -0x6000;

    player->unkB84 = flags;
    player->velocity.y = velocityY;

    decayPlayerSteeringAngles(player);
    applyClampedVelocityToPosition(player);

    collisionRadius = player->collisionRadius;
    if (!(0xDFFFF < collisionRadius)) {
        player->collisionRadius = collisionRadius + 0x8000;
    }

    state = player->behaviorCounter;

    switch (state) {
        case 0:
            if (func_8005D308_5DF08(player, 0x1E)) {
                temp = player->behaviorCounter;
                temp++;
                player->behaviorCounter = temp;
            }
            break;

        case 1:
            if (func_8005D308_5DF08(player, 0x20)) {
                velocityY = 0x14;
                temp = player->behaviorCounter;
                player->unkB8C = velocityY;
                temp++;
                player->behaviorCounter = temp;
            }
            break;

        case 2:
            temp = player->unkB8C;
            if (temp != 0) {
                temp--;
                player->unkB8C = temp;
            } else {
                temp = player->behaviorCounter;
                temp++;
                player->behaviorCounter = temp;
            }
            break;

        case 3:
            if (func_8005D308_5DF08(player, 0x1F)) {
                temp = player->behaviorCounter;
                temp++;
                player->behaviorCounter = temp;
            }
            break;

        case 4:
            if (func_8005D308_5DF08(player, 0x20)) {
                player->behaviorCounter = 2;
                player->unkB8C = 0x14;
            }
            break;
    }

    setPlayerBodyPartAnimState(player, 4, 0);

    return 0;
}

extern Transform3D D_8009A890_9B490;
extern Vec3i D_8009A8A4_9B4A4;

void updateTrickRotationTransform(Player *player) {
    Transform3D rotationMatrix;
    s16 *flipAngle;
    s16 *spinAngle;

    D_8009A8A4_9B4A4.x = 0;
    D_8009A8A4_9B4A4.y = 0xFFF00000;
    D_8009A8A4_9B4A4.z = 0;
    flipAngle = &player->unkA98;
    spinAngle = &player->unkA96;
    createRotationMatrixYX(&rotationMatrix, *flipAngle, *spinAngle);
    rotationMatrix.translation.x = 0;
    rotationMatrix.translation.y = 0x100000;
    rotationMatrix.translation.z = 0;
    func_8006B084_6BC84(&D_8009A890_9B490, &rotationMatrix, &player->unk9B0.prev_position);
    player->unkB84 |= 0x800;
}

void decayPlayerSteeringAngles(Player *player) {
    s32 normalizedAngle;
    s32 decayAmount;
    s32 currentAngle;
    s32 savedAngle;
    s32 tiltOffset;
    s32 tiltDecay;
    s32 pad[4];

    normalizedAngle = (u16)player->unkA92 & 0x1FFF;
    player->unkA92 = normalizedAngle;
    if (normalizedAngle >= 0x1001) {
        player->unkA92 = normalizedAngle - 0x2000;
    }
    currentAngle = player->unkA92;
    decayAmount = -currentAngle, savedAngle = currentAngle;
    if (decayAmount >= 0x81) {
        decayAmount = 0x80;
    }
    if (decayAmount < -0x80) {
        decayAmount = -0x80;
    }
    player->unkA92 = savedAngle + decayAmount;

    normalizedAngle = (u16)player->unkA90 & 0x1FFF;
    player->unkA90 = normalizedAngle;
    if (normalizedAngle >= 0x1001) {
        player->unkA90 = normalizedAngle - 0x2000;
    }
    currentAngle = player->unkA90;
    decayAmount = -currentAngle, savedAngle = currentAngle;
    if (decayAmount >= 0x81) {
        decayAmount = 0x80;
    }
    if (decayAmount < -0x80) {
        decayAmount = -0x80;
    }
    tiltOffset = player->unk990.translation.x;
    player->unkA90 = savedAngle + decayAmount;
    tiltDecay = -tiltOffset;
    if (tiltDecay > 0x8000) {
        tiltDecay = 0x8000;
    }
    if (tiltDecay < -0x8000) {
        tiltDecay = -0x8000;
    }
    player->unk990.translation.x = tiltOffset + tiltDecay;
}

void decayPlayerAirborneAngles(Player *player) {
    s16 normalizedAngle;
    s16 currentAngle;
    s32 decayAmount, savedAngle;
    s32 tiltOffset, tiltDecay, savedTilt;
    s16 result;
    s32 pad[5];

    // Decay unkA92
    normalizedAngle = player->unkA92 & 0x1FFF;
    player->unkA92 = normalizedAngle;
    if (!(normalizedAngle < 0x1001)) {
        player->unkA92 = normalizedAngle - 0x2000;
    }

    currentAngle = player->unkA92;
    decayAmount = -currentAngle, savedAngle = currentAngle;
    if (!(decayAmount < 0x29)) {
        decayAmount = 0x28;
    }
    if (decayAmount < -0x28) {
        decayAmount = -0x28;
    }
    player->unkA92 = savedAngle + decayAmount;

    // Decay unkA8E
    normalizedAngle = player->unkA8E & 0x1FFF;
    player->unkA8E = normalizedAngle;
    if (!(normalizedAngle < 0x1001)) {
        player->unkA8E = normalizedAngle - 0x2000;
    }

    currentAngle = player->unkA8E;
    decayAmount = -currentAngle, savedAngle = currentAngle;
    if (!(decayAmount < 0x29)) {
        decayAmount = 0x28;
    }
    if (decayAmount < -0x28) {
        decayAmount = -0x28;
    }
    player->unkA8E = savedAngle + decayAmount;

    // Decay unkA90
    normalizedAngle = player->unkA90 & 0x1FFF;
    player->unkA90 = normalizedAngle;
    if (!(normalizedAngle < 0x1001)) {
        player->unkA90 = normalizedAngle - 0x2000;
    }

    currentAngle = player->unkA90;
    decayAmount = -currentAngle, savedAngle = currentAngle;
    if (!(decayAmount < 0x29)) {
        decayAmount = 0x28;
    }
    if (decayAmount < -0x28) {
        decayAmount = -0x28;
    }
    result = savedAngle + decayAmount;

    // Decay unk990.translation.x (tilt offset)
    tiltOffset = player->unk990.translation.x;
    player->unkA90 = result;
    tiltDecay = -tiltOffset, savedTilt = tiltOffset;
    if (!(tiltDecay < 0x6001)) {
        tiltDecay = 0x6000;
    }
    if (tiltDecay < -0x6000) {
        tiltDecay = -0x6000;
    }
    player->unk990.translation.x = savedTilt + tiltDecay;
}

extern s16 identityMatrix[];

s32 applyVelocityDeadzone(Player *player, s32 forwardDeadzone, s32 backwardDeadzone, s32 lateralDeadzone) {
    Vec3i tempVec;
    Vec3i localVelocity;
    Vec3i forwardDir;
    Transform3D yawRotation;
    Transform3D pitchMatrix;
    Transform3D slopeMatrix;
    Transform3D combinedTransform;
    s32 magnitude;
    s32 sqrtResult;
    s32 localForwardVelocity;

    createYRotationMatrix(&yawRotation, player->unkA90 + player->rotY);

    forwardDir.x = yawRotation.m[2][0];
    forwardDir.y = yawRotation.m[2][1];
    forwardDir.z = yawRotation.m[2][2];

    memcpy(&pitchMatrix, identityMatrix, 0x20);
    memcpy(&slopeMatrix, identityMatrix, 0x20);

    sqrtResult = approximate_sqrt(player->unk458 * player->unk458 + player->unk460 * player->unk460);
    magnitude = sqrtResult & 0xFFFF;

    if (magnitude != 0) {
        slopeMatrix.m[0][0] = (player->unk460 << 13) / magnitude;
        slopeMatrix.m[0][2] = (player->unk458 << 13) / magnitude;
        slopeMatrix.m[2][0] = (-player->unk458 << 13) / magnitude;
        slopeMatrix.m[2][2] = (player->unk460 << 13) / magnitude;
    }

    pitchMatrix.m[1][1] = player->unk45C;
    pitchMatrix.m[1][2] = -magnitude;
    pitchMatrix.m[2][1] = magnitude;
    pitchMatrix.m[2][2] = player->unk45C;

    func_8006BDBC_6C9BC((void *)&slopeMatrix, &pitchMatrix, &combinedTransform);
    transformVector2(&player->velocity.x, &combinedTransform, &tempVec);

    if (tempVec.y < 0) {
        tempVec.y = 0;
    }

    transformVector2(&forwardDir, &combinedTransform, &localVelocity);
    memcpy(&slopeMatrix, identityMatrix, 0x20);

    sqrtResult = approximate_sqrt(localVelocity.x * localVelocity.x + localVelocity.z * localVelocity.z);
    magnitude = sqrtResult & 0xFFFF;

    if (magnitude != 0) {
        slopeMatrix.m[0][0] = (localVelocity.z << 13) / magnitude;
        slopeMatrix.m[2][2] = (localVelocity.z << 13) / magnitude;
        slopeMatrix.m[0][2] = (localVelocity.x << 13) / magnitude;
        slopeMatrix.m[2][0] = (-localVelocity.x << 13) / magnitude;
    }

    transformVector2(&tempVec, &slopeMatrix, &localVelocity);

    if (localVelocity.x > 0) {
        if (lateralDeadzone < localVelocity.x) {
            localVelocity.x = localVelocity.x - lateralDeadzone;
        } else {
            localVelocity.x = 0;
        }
    } else {
        if (localVelocity.x < -lateralDeadzone) {
            localVelocity.x = localVelocity.x + lateralDeadzone;
        } else {
            localVelocity.x = 0;
        }
    }

    if (localVelocity.z > 0) {
        if (forwardDeadzone < localVelocity.z) {
            localVelocity.z = localVelocity.z - forwardDeadzone;
        } else {
            localVelocity.z = 0;
        }
    } else {
        if (localVelocity.z < -backwardDeadzone) {
            localVelocity.z = localVelocity.z + backwardDeadzone;
        } else {
            localVelocity.z = 0;
        }
    }

    localForwardVelocity = localVelocity.z;
    transformVector3(&localVelocity, &slopeMatrix, &tempVec);
    transformVector3(&tempVec, &combinedTransform, &localVelocity);

    player->velocity.x = localVelocity.x;
    player->velocity.z = localVelocity.z;

    return localForwardVelocity;
}

void dispatchStunnedBehaviorPhase(BehaviorState *arg0) {
    stunnedBehaviorPhaseHandlers[arg0->behaviorPhase](arg0);
}

void initStunnedAirborneBehavior(Player *player) {
    player->behaviorMode = 2;
    player->behaviorPhase = 5;
    player->behaviorStep = 0;
    player->behaviorCounter = 0;
    player->boostTimer = 0;
    player->ghostEffectTimer = 0;
    player->invincibilityTimer = 0;
    player->unkB84 = player->unkB84 & 0xFFFF2FFF;
    playStunnedVoice(player);
}

s32 updateStunnedAirbornePhase(Player *player) {
    Vec3i effectPos;
    GameState *gameState;
    s16 angleDiff;
    s16 clampedDelta;
    s32 velX;
    s32 velZ;
    s16 knockbackAngle;

    gameState = getCurrentAllocation();

    if (player->behaviorStep == 0) {
        player->unkA8C = 0xFFFF;
        player->unkB88 = 0;
        player->velocity.x = player->unkAC8;
        player->velocity.z = player->unkAD0;
        player->behaviorStep++;
        knockbackAngle = atan2Fixed(-player->unkAC8, -player->unkAD0);
        rotateVectorY(gameState->unk48 + 0xE4, knockbackAngle, &effectPos);
        effectPos.x += player->worldPos.x;
        effectPos.z += player->worldPos.z;
        effectPos.y = player->worldPos.y + 0x100000;
        queueSoundAtPosition(&player->worldPos, 0xD);
        spawnImpactStar(&effectPos);
    }

    angleDiff = (getPlayerTargetTrackAngle(player) - player->rotY) & 0x1FFF;
    clampedDelta = angleDiff;
    if (angleDiff >= 0x1001) {
        clampedDelta = angleDiff | 0xE000;
    }
    if (clampedDelta >= 0x101) {
        clampedDelta = 0x100;
    }
    if (clampedDelta < -0x100) {
        clampedDelta = -0x100;
    }
    player->rotY = player->rotY + clampedDelta;
    player->velocity.y -= 0x6000;
    velX = player->velocity.x;
    player->velocity.x = velX - (velX >> 4);
    velZ = player->velocity.z;
    player->velocity.z = velZ - (velZ >> 4);
    decayPlayerAirborneAngles(player);
    applyClampedVelocityToPosition(player);
    if (func_8005D308_5DF08(player, 8) != 0) {
        player->aiLaneWidth = player->aiLaneWidth / 2;
        resetPlayerBehaviorToDefault(player);
    }
    setPlayerBodyPartAnimState(player, 3, 0);
    return 0;
}

s32 updateStunnedAirbornePhaseBoss(Player *player) {
    Vec3i effectPos;
    GameState *gameState;
    s16 angleDiff;
    s16 clampedDelta;
    s32 velX;
    s32 velZ;
    s16 knockbackAngle;

    gameState = getCurrentAllocation();

    if (player->behaviorStep == 0) {
        player->unkA8C = 0xFFFF;
        player->unkB88 = 0;
        player->velocity.x = player->unkAC8;
        player->velocity.z = player->unkAD0;
        player->behaviorStep++;
        knockbackAngle = atan2Fixed(-player->unkAC8, -player->unkAD0);
        rotateVectorY(gameState->unk48 + 0xE4, knockbackAngle, &effectPos);
        effectPos.x += player->worldPos.x;
        effectPos.z += player->worldPos.z;
        effectPos.y = player->worldPos.y + 0x100000;
        queueSoundAtPosition(&player->worldPos, 0xD);
        spawnImpactStar(&effectPos);
    }

    angleDiff = (getPlayerTargetTrackAngle(player) - player->rotY) & 0x1FFF;
    clampedDelta = angleDiff;
    if (angleDiff >= 0x1001) {
        clampedDelta = angleDiff | 0xE000;
    }
    if (clampedDelta >= 0x101) {
        clampedDelta = 0x100;
    }
    if (clampedDelta < -0x100) {
        clampedDelta = -0x100;
    }
    player->rotY = player->rotY + clampedDelta;
    player->velocity.y -= 0x6000;
    velX = player->velocity.x;
    player->velocity.x = velX - (velX >> 4);
    velZ = player->velocity.z;
    player->velocity.z = velZ - (velZ >> 4);
    decayPlayerAirborneAngles(player);
    applyClampedVelocityToPosition(player);
    if (func_8005D308_5DF08(player, 7) != 0) {
        player->aiLaneWidth = player->aiLaneWidth / 2;
        resetPlayerBehaviorToDefault(player);
    }
    setPlayerBodyPartAnimState(player, 3, 0);
    return 0;
}

s32 updateStunnedFallingPhase(Player *player) {
    player->unkB88 = 1;
    player->unkB84 |= 0x20;
    player->velocity.y -= 0x6000;
    player->velocity.x -= player->velocity.x >> 6;
    player->velocity.z -= player->velocity.z >> 6;
    decayPlayerSteeringAngles(player);
    applyClampedVelocityToPosition(player);

    if (func_8005D308_5DF08(player, 9) != 0) {
        if ((player->unkB84 & 1) == 0) {
            setPlayerBehaviorPhase(player, 3);
        }
    }

    player->unkB84 |= 0x10000;
    setPlayerBodyPartAnimState(player, 3, 0);
    return 0;
}

s32 updateStunnedLandingBouncePhase(Player *player) {
    s64 distSq;
    s32 dist;

    if (player->behaviorStep == 0) {
        player->unkB8C = 0x14;
        player->behaviorStep++;
    }

    player->unkB84 |= 0x60;
    player->velocity.y -= 0x6000;
    player->unkB88 = 1;

    if (player->unkB84 & 1) {
        player->unkA8C = -1;
        player->unkB8C = 0xA;
        player->behaviorCounter = 0;
        player->velocity.x -= (player->velocity.x >> 5);
        player->velocity.z -= (player->velocity.z >> 5);
    } else {
        if (player->behaviorCounter == 0) {
            player->behaviorCounter++;
            startRumbleEffect(player, 0);
            queueSoundAtPosition(&player->worldPos, 0x25);
            spawnCharacterAttackEffectByType(player, player->unkBCC & 0xF);
        }
        applyVelocityDeadzone(player, 0x8000, 0x8000, 0x8000);
    }

    decayPlayerSteeringAngles(player);
    applyClampedVelocityToPosition(player);

    if (func_8005D308_5DF08(player, 0xA) != 0) {
        if ((player->unkB84 & 1) == 0) {
            if (player->unkB8C == 0) {
                distSq = (s64)player->velocity.x * player->velocity.x + (s64)player->velocity.z * player->velocity.z;
                dist = isqrt64(distSq);

                if (dist <= 0x1FFFF) {
                    player->unkB88 = 0;
                    setPlayerBehaviorPhase(player, 4);
                }
            } else {
                player->unkB8C--;
            }
        }
    }

    setPlayerBodyPartAnimState(player, 3, 0);
    return 0;
}

s32 updateStunnedRecoveryFallingPhase(Player *player) {
    s16 targetAngle;
    s16 angleDiff;
    s16 clampedDelta;

    if (player->behaviorStep == 0) {
        player->unkB88 = 0;
        player->unkB8C = 0xF;
        player->behaviorStep++;
        if (player->velocity.y > 0) {
            player->velocity.y = 0;
        }
    }

    if (player->unkB8C != 0) {
        player->unkB8C--;
        player->unkB84 |= 0x40;
    }

    targetAngle = getPlayerTargetTrackAngle(player);
    angleDiff = targetAngle - player->rotY;
    clampedDelta = angleDiff & 0x1FFF;

    if (clampedDelta >= 0x1001) {
        clampedDelta |= 0xE000;
    }

    if (clampedDelta > 0x100) {
        clampedDelta = 0x100;
    }

    if (clampedDelta < -0x100) {
        clampedDelta = -0x100;
    }

    player->rotY += clampedDelta;
    player->velocity.x = 0;
    player->velocity.z = 0;
    player->velocity.y -= 0x6000;

    decayPlayerSteeringAngles(player);
    applyClampedVelocityToPosition(player);

    if (func_8005D308_5DF08(player, 0xB) != 0) {
        resetPlayerBehaviorToDefault(player);
    }

    return 0;
}

extern s32 recoverySlideBaseVelocity[3];

s32 updateStunnedRecoveryGroundSlidePhase(Player *player) {
    s32 velocityMagnitude;
    s32 timerDelta;

    if (player->behaviorStep == 0) {
        player->unkB8C = 0x1E;
        player->behaviorStep++;
    }

    player->unkB84 |= 0x60;
    player->velocity.y -= 0x6000;
    player->unkB88 = 3;

    if (player->unkB84 & 1) {
        player->velocity.x -= player->velocity.x >> 5;
        player->velocity.z -= player->velocity.z >> 5;
    } else {
        applyVelocityDeadzone(player, 0x100, 0x100, 0x100);
        spawnPlayerCharacterTrailParticle(player, player->unkBCC & 0xF);
    }

    velocityMagnitude = distance_3d(player->velocity.x, player->velocity.y, player->velocity.z);
    if (velocityMagnitude <= 0xFFFF) {
        if (player->unkB84 & 2) {
            rotateVectorY(recoverySlideBaseVelocity, player->rotY + 0x1000, &player->velocity);
        } else {
            rotateVectorY(recoverySlideBaseVelocity, player->rotY, &player->velocity);
        }
    }

    player->rotY = atan2Fixed(-player->velocity.x, -player->velocity.z);
    if (player->unkB84 & 2) {
        player->rotY += 0x1000;
    }

    decayPlayerSteeringAngles(player);
    applyClampedVelocityToPosition(player);
    func_8005D180_5DD80(player, 0xC);

    if (player->unkB8C == 0) {
        setPlayerBehaviorPhase(player, 2);
    } else {
        timerDelta = getTrickRecoveryTimerReduction(player);
        player->unkB8C -= timerDelta;
        if (player->unkB8C < 0) {
            player->unkB8C = 0;
        }
    }

    setPlayerBodyPartAnimState(player, 3, 0);

    return 0;
}

s32 updateEdgeFallRecoveryGetUpPhase(Player *arg0) {
    s16 angle;

    if (arg0->behaviorStep != 0) {
        arg0->unkB8C = 0;
        arg0->behaviorStep++;
    }

    arg0->unkB88 = 7;
    arg0->unkB84 |= 0x60;

    if (arg0->velocity.y > 0) {
        arg0->velocity.y = 0;
    }

    arg0->velocity.y -= 0x6000;

    if (arg0->unkB84 & 1) {
        arg0->velocity.x = arg0->velocity.x - (arg0->velocity.x >> 5);
        arg0->velocity.z = arg0->velocity.z - (arg0->velocity.z >> 5);
    } else {
        applyVelocityDeadzone(arg0, 0x100, 0x100, 0x100);
        spawnPlayerCharacterTrailParticle(arg0, arg0->unkBCC & 0xF);
    }

    angle = atan2Fixed(-arg0->velocity.x, -arg0->velocity.z);
    arg0->rotY = angle;

    if (arg0->unkB84 & 2) {
        arg0->rotY = angle + 0x1000;
    }

    decayPlayerSteeringAngles(arg0);
    applyClampedVelocityToPosition(arg0);
    func_8005D180_5DD80(arg0, 0xC);

    if (arg0->unkBC9 != 1) {
        if (!(arg0->unkB84 & 1)) {
            arg0->unkB8C++;
            if (arg0->unkB8C >= 0xB) {
                setPlayerBehaviorPhase(arg0, 5);
                goto skip;
            }
        }
    } else {
        arg0->unkB8C = 0;
    }
skip:
    setPlayerBodyPartAnimState(arg0, 3, 0);
    return 0;
}

s32 updateStunnedRecoveryBouncePhase(Player *arg0) {
    Vec3i stackVec;
    GameState *allocation;
    u16 angle;
    s32 vel1;
    s32 vel2;

    allocation = getCurrentAllocation();

    if (arg0->behaviorStep == 0) {
        arg0->behaviorStep++;
        angle = arg0->unkAC4;
        arg0->rotY = angle;
        rotateVectorY(&arg0->unkAC8, angle, &arg0->velocity);

        if (arg0->unkB84 & 2) {
            arg0->rotY += 0x1000;
        }

        rotateVectorY(allocation->unk48 + 0xE4, arg0->rotY, &stackVec);

        stackVec.x += arg0->worldPos.x;
        stackVec.z += arg0->worldPos.z;
        stackVec.y = arg0->worldPos.y + 0x140000;

        queueSoundAtPosition(&arg0->worldPos, 0xD);
        spawnImpactStar(&stackVec);
    }

    arg0->unkB88 = 1;
    arg0->unkB84 |= 0x20;
    arg0->velocity.y -= 0x6000;

    vel1 = arg0->velocity.x;
    arg0->velocity.x = vel1 - (vel1 >> 6);

    vel2 = arg0->velocity.z;
    arg0->velocity.z = vel2 - (vel2 >> 6);

    decayPlayerSteeringAngles(arg0);
    applyClampedVelocityToPosition(arg0);

    if (func_8005D308_5DF08(arg0, 0xE) != 0) {
        if ((arg0->unkB84 & 1) == 0) {
            setPlayerBehaviorPhase(arg0, 8);
        }
    }

    arg0->unkB84 |= 0x10000;
    setPlayerBodyPartAnimState(arg0, 3, 0);

    return 0;
}

s32 updateStunnedRecoveryBounceFallPhase(Player *arg0) {
    if (arg0->behaviorStep == 0) {
        arg0->velocity.y = 0;
        arg0->unkB8C = 0x14;
        arg0->behaviorStep = arg0->behaviorStep + 1;
    }

    arg0->unkB84 = arg0->unkB84 | 0x60;
    arg0->unkB88 = 1;
    arg0->velocity.y = arg0->velocity.y - 0x6000;

    if (arg0->unkB84 & 1) {
        arg0->unkA8C = 0xFFFF;
        arg0->unkB8C = 0xA;
        arg0->behaviorCounter = 0;
        arg0->velocity.x = arg0->velocity.x - (arg0->velocity.x >> 5);
        arg0->velocity.z = arg0->velocity.z - (arg0->velocity.z >> 5);
    } else {
        if (arg0->behaviorCounter == 0) {
            arg0->behaviorCounter++;
            startRumbleEffect(arg0, 0);
            queueSoundAtPosition(&arg0->worldPos, 0x25);
            spawnCharacterAttackEffectByType(arg0, arg0->unkBCC & 0xF);
        }
        applyVelocityDeadzone(arg0, 0x8000, 0x8000, 0x8000);
    }

    decayPlayerSteeringAngles(arg0);
    applyClampedVelocityToPosition(arg0);

    if (func_8005D308_5DF08(arg0, 0xF) != 0) {
        if (!(arg0->unkB84 & 1)) {
            if (arg0->unkB8C == 0) {
                arg0->unkB88 = 0;
                setPlayerBehaviorPhase(arg0, 0xD);
            } else {
                arg0->unkB8C = arg0->unkB8C - 1;
            }
        }
    }

    setPlayerBodyPartAnimState(arg0, 3, 0);
    return 0;
}

s32 updateStunnedRecoverySlidePhase(Player *arg0) {
    if (arg0->behaviorStep == 0) {
        arg0->behaviorStep = arg0->behaviorStep + 1;
        queueSoundAtPosition(&arg0->worldPos, 0xD);
    }

    arg0->unkB88 = 8;
    arg0->unkB84 = arg0->unkB84 | 0x20;

    if (arg0->velocity.y > 0) {
        arg0->velocity.y = 0;
    }

    arg0->velocity.x = arg0->velocity.x / 2;
    arg0->velocity.z = arg0->velocity.z / 2;
    arg0->velocity.y = arg0->velocity.y - 0x6000;

    applyVelocityDeadzone(arg0, 0x8000, 0x8000, 0x8000);
    decayPlayerSteeringAngles(arg0);
    applyClampedVelocityToPosition(arg0);

    if (func_8005D308_5DF08(arg0, 0xE) != 0) {
        setPlayerBehaviorPhase(arg0, 0xA);
    }

    setPlayerBodyPartAnimState(arg0, 3, 0);
    return 0;
}

s32 updateStunnedRecoverySlideBouncePhase(Player *arg0) {
    if (arg0->behaviorStep == 0) {
        arg0->velocity.y = 0;
        arg0->unkB8C = 0x14;
        arg0->behaviorStep = arg0->behaviorStep + 1;
        if ((arg0->unkB84 & 1) == 0) {
            queueSoundAtPosition(&arg0->worldPos, 0x25);
            spawnCharacterAttackEffectByType(arg0, arg0->unkBCC & 0xF);
        }
    }

    arg0->unkB88 = 8;
    arg0->unkB84 = arg0->unkB84 | 0x60;

    if (arg0->velocity.y > 0) {
        arg0->velocity.y = 0;
    }

    arg0->velocity.x = arg0->velocity.x / 2;
    arg0->velocity.y = arg0->velocity.y - 0x6000;
    arg0->velocity.z = arg0->velocity.z / 2;

    applyVelocityDeadzone(arg0, 0x8000, 0x8000, 0x8000);
    decayPlayerSteeringAngles(arg0);
    applyClampedVelocityToPosition(arg0);

    if (func_8005D308_5DF08(arg0, 0xF) != 0) {
        if ((arg0->unkB84 & 1) == 0) {
            if (arg0->unkB8C == 0) {
                setPlayerBehaviorPhase(arg0, 0xB);
            } else {
                arg0->unkB8C = arg0->unkB8C - 1;
            }
        }
    }

    setPlayerBodyPartAnimState(arg0, 3, 0);
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B55B4_A5464);

s32 updateStunnedRecoveryEndPhase(Player *arg0) {
    arg0->unkB8C -= 1;

    if (arg0->unkB8C == 0) {
        arg0->unkB88 = 0;
        resetPlayerBehaviorToDefault(arg0);
        return 1;
    }

    arg0->unkB88 = 8;
    arg0->worldPos.y = arg0->worldPos.y + 0xFFFF0000;
    decayPlayerSteeringAngles(arg0);
    func_8005D180_5DD80(arg0, 0);
    setPlayerBodyPartAnimState(arg0, 3, 0);
    return 0;
}

s32 updateStunnedRecoveryStandUpPhase(Player *arg0) {
    s16 angle;
    s16 angleDelta;

    if (arg0->behaviorStep == 0) {
        arg0->unkB88 = 0;
        arg0->unkB8C = 0xF;
        arg0->behaviorStep++;
        if (arg0->velocity.y > 0) {
            arg0->velocity.y = 0;
        }
        arg0->unkA8C = 0xFFFF;
    }

    if (arg0->unkB8C != 0) {
        arg0->unkB8C--;
        arg0->unkB84 |= 0x40;
    }

    angle = getPlayerTargetTrackAngle(arg0);
    angleDelta = (angle - arg0->rotY) & 0x1FFF;

    if (angleDelta >= 0x1001) {
        angleDelta |= 0xE000;
    }

    if (angleDelta >= 0x101) {
        angleDelta = 0x100;
    }

    if (angleDelta < -0x100) {
        angleDelta = -0x100;
    }

    arg0->rotY += angleDelta;
    arg0->velocity.x = 0;
    arg0->velocity.z = 0;
    arg0->velocity.y -= 0x6000;

    decayPlayerSteeringAngles(arg0);
    applyClampedVelocityToPosition(arg0);

    if (func_8005D308_5DF08(arg0, 0x10) != 0) {
        resetPlayerBehaviorToDefault(arg0);
    }

    return 0;
}

s32 updateKnockbackAirborneLaunchPhase(Player *player) {
    s32 prevFlags;

    if (!player->behaviorStep) {
        prevFlags = player->unkB84;
        player->behaviorStep++;
        player->velocity.y = 0xA0000;
        player->unkB88 = 0x20;
        player->unkB8C = 5;
        player->unkA8C = 0xFFFF;
        player->unkB84 = prevFlags | 0x20;
    }

    if (player->velocity.y > 0) {
        player->velocity.y = player->velocity.y - 0x8000;
    } else {
        if (player->behaviorStep == 1) {
            if (createFallingEffect(player) != 0) {
                player->behaviorStep++;
            }
        }

        player->velocity.y -= 0x800;

        if (getTrickRecoveryTimerReduction(player) != 1) {
            player->velocity.y += 0x100;
        }
    }

    player->velocity.x = player->velocity.x - (player->velocity.x >> 6);
    player->velocity.z = player->velocity.z - (player->velocity.z >> 6);

    decayPlayerAirborneAngles(player);

    player->rotY += 0x40;

    applyVelocityToPosition(player);
    func_8005D308_5DF08(player, 9);

    if (player->unkB8C != 0) {
        player->unkB8C--;
        if (player->unkB8C != 0) {
            goto skip;
        }
    }

    if ((player->unkB84 & 1) == 0) {
        setPlayerBehaviorPhase(player, 4);
    }

skip:
    setPlayerBodyPartAnimState(player, 3, 0);

    return 0;
}

s32 updateStunnedPanelHitFallPhase(Player *arg0) {
    s32 newTimer;
    u8 step;

    step = arg0->behaviorStep;
    if (step == 0) {
        arg0->behaviorStep = step + 1;
        if (arg0->velocity.y > 0) {
            arg0->velocity.y = 0;
        }
        arg0->unkB88 = 0x40;
        arg0->unkB8C = 0x1E;
        arg0->unkA8C = 0xFFFF;
        arg0->unkB84 |= 0x20;
        arg0->ufoFlags &= 0xFE;
        func_8005D308_5DF08(arg0, 0xE);
    }
    if (arg0->behaviorStep == 1) {
        scheduleShieldEffect(arg0);
        arg0->behaviorStep += 1;
    }
    arg0->velocity.x = 0;
    arg0->velocity.z = 0;
    arg0->velocity.y -= 0x6000;
    decayPlayerSteeringAngles(arg0);
    applyVelocityToPosition(arg0);
    if (arg0->ufoFlags & 1) {
        arg0->unkB8C = 0;
    }
    if (arg0->unkB8C == 0) {
        startRumbleEffect(arg0, 4);
        setPlayerBehaviorPhase(arg0, 0x10);
    } else {
        newTimer = arg0->unkB8C - getTrickRecoveryTimerReduction(arg0);
        arg0->unkB8C = newTimer;
        if (newTimer < 0) {
            arg0->unkB8C = 0;
        }
    }
    setPlayerBodyPartAnimState(arg0, 3, 0);
    return 0;
}

s32 updateStunnedBounceFallPhase(Player *arg0) {
    arg0->unkB88 = 1;
    arg0->unkB84 |= 0x20;
    arg0->velocity.y -= 0x6000;
    arg0->velocity.x -= arg0->velocity.x >> 6;
    arg0->velocity.z -= arg0->velocity.z >> 6;
    decayPlayerSteeringAngles(arg0);
    applyVelocityToPosition(arg0);

    if (func_8005D308_5DF08(arg0, 0xE) != 0) {
        if ((arg0->unkB84 & 1) == 0) {
            setPlayerBehaviorPhase(arg0, 8);
        }
    }

    setPlayerBodyPartAnimState(arg0, 3, 0);
    return 0;
}

s32 updateStunnedBounceFallRecoverPhase(Player *arg0) {
    if (arg0->behaviorStep == 0) {
        arg0->unkB8C = 0x78;
        arg0->behaviorStep = arg0->behaviorStep + 1;
    }

    if (arg0->behaviorStep == 1) {
        if (spawnCrashEffect(arg0) != 0) {
            arg0->behaviorStep = arg0->behaviorStep + 1;
        }
    }

    arg0->unkB88 = 0x80;
    arg0->velocity.y = arg0->velocity.y - arg0->unkAB8;
    decayPlayerSteeringAngles(arg0);

    if (arg0->unkB84 & 1) {
        arg0->velocity.x = arg0->velocity.x - (arg0->velocity.x >> 7);
        arg0->velocity.z = arg0->velocity.z - (arg0->velocity.z >> 7);
    } else {
        applyVelocityDeadzone(arg0, 0x200, 0x200, arg0->unkAB0);
    }

    applyClampedVelocityToPosition(arg0);
    func_8005D308_5DF08(arg0, 0);

    if (arg0->unkB8C == 0) {
        arg0->unkB88 = 0;
        startRumbleEffect(arg0, 4);
        resetPlayerBehaviorToDefault(arg0);
        return 0;
    } else {
        arg0->unkB8C = arg0->unkB8C - getTrickRecoveryTimerReduction(arg0);
        if (arg0->unkB8C < 0) {
            arg0->unkB8C = 0;
        }
    }

    return 0;
}

s32 updateStunnedBounceLaunchPhase(Player *arg0) {
    s16 angle;

    if (arg0->behaviorStep == 0) {
        arg0->unkB8C = 0x1E;
        arg0->behaviorStep = arg0->behaviorStep + 1;
    }

    arg0->unkB84 = arg0->unkB84 | 0x60;
    arg0->unkB88 = 0x102;
    arg0->velocity.y = arg0->velocity.y - 0x6000;

    if (arg0->unkB84 & 1) {
        arg0->velocity.x = arg0->velocity.x - (arg0->velocity.x >> 5);
        arg0->velocity.z = arg0->velocity.z - (arg0->velocity.z >> 5);
    } else {
        applyVelocityDeadzone(arg0, 0x100, 0x100, 0x100);
    }

    angle = atan2Fixed(-arg0->velocity.x, -arg0->velocity.z);
    arg0->rotY = angle;

    if (arg0->unkB84 & 2) {
        arg0->rotY = angle + 0x1000;
    }

    decayPlayerSteeringAngles(arg0);
    applyClampedVelocityToPosition(arg0);
    func_8005D180_5DD80(arg0, 0xC);

    if (arg0->unkB8C == 0) {
        setPlayerBehaviorPhase(arg0, 2);
    } else {
        arg0->unkB8C = arg0->unkB8C - getTrickRecoveryTimerReduction(arg0);
        if (arg0->unkB8C < 0) {
            arg0->unkB8C = 0;
        }
    }

    setPlayerBodyPartAnimState(arg0, 3, 0);
    return 0;
}

s32 updateKnockbackAirbornePhase(Player *arg0) {
    u8 step;
    s32 knockbackVelZ;
    s32 horizontalSpeed;

    step = arg0->behaviorStep;
    if (step == 0) {
        arg0->velocity.x = arg0->unkAC8;
        knockbackVelZ = arg0->unkAD0;
        arg0->behaviorStep = step + 1;
        arg0->velocity.z = knockbackVelZ;
        horizontalSpeed = isqrt64((s64)arg0->velocity.x * arg0->velocity.x + (s64)knockbackVelZ * knockbackVelZ);
        if (horizontalSpeed > 0x60000) {
            arg0->velocity.x = (s64)arg0->velocity.x * 0x60000 / horizontalSpeed;
            arg0->velocity.z = (s64)arg0->velocity.z * 0x60000 / horizontalSpeed;
        }
        arg0->velocity.y = 0x90000;
        arg0->unkA8C = 0xFFFF;
    }

    arg0->unkB88 = 0x200;
    arg0->unkB84 |= 0x20;
    arg0->velocity.y += 0xFFFF7000;
    arg0->velocity.x -= arg0->velocity.x >> 6;
    arg0->velocity.z -= arg0->velocity.z >> 6;

    decayPlayerSteeringAngles(arg0);
    applyVelocityToPosition(arg0);

    if (func_8005D308_5DF08(arg0, 9) != 0) {
        if ((arg0->unkB84 & 1) == 0) {
            setPlayerBehaviorPhase(arg0, 0x14);
        }
    }

    if (randA() & 1) {
        spawnCharacterTrailParticle(arg0);
    }

    setPlayerBodyPartAnimState(arg0, 3, 0);
    return 0;
}

s32 updateKnockbackBounceLaunchPhase(Player *player) {
    s16 angle;

    if (player->behaviorStep == 0) {
        player->unkB8C = 0x1E;
        player->behaviorStep = player->behaviorStep + 1;
    }

    player->unkB84 = player->unkB84 | 0x60;
    player->velocity.y = player->velocity.y - 0x6000;
    player->unkB88 = 0x202;

    if (player->unkB84 & 1) {
        player->velocity.x = player->velocity.x - (player->velocity.x >> 5);
        player->velocity.z = player->velocity.z - (player->velocity.z >> 5);
    } else {
        applyVelocityDeadzone(player, 0x100, 0x100, 0x100);
    }

    angle = atan2Fixed(-player->velocity.x, -player->velocity.z);
    player->rotY = angle;

    if (player->unkB84 & 2) {
        player->rotY = angle + 0x1000;
    }

    decayPlayerSteeringAngles(player);
    applyClampedVelocityToPosition(player);
    func_8005D180_5DD80(player, 0xC);

    if (player->unkB8C == 0) {
        setPlayerBehaviorPhase(player, 2);
    } else {
        player->unkB8C = player->unkB8C - getTrickRecoveryTimerReduction(player);
        if (player->unkB8C < 0) {
            player->unkB8C = 0;
        }
    }

    if (randA() & 1) {
        spawnCharacterTrailParticle(player);
    }

    setPlayerBodyPartAnimState(player, 3, 0);
    return 0;
}

s32 updateKnockbackLaunchWithHomingProjectilesPhase(Player *player) {
    u8 step;

    step = player->behaviorStep;
    if (step == 0) {
        step += 1;
        player->behaviorStep = step;
        player->unkB8C = 0x8000;
        player->unkA8C = 0xFFFF;
        func_8005D308_5DF08(player, 0xE);
    }
    if ((player->behaviorStep == 1) && (createLiftEffect(player) != 0)) {
        player->behaviorStep = player->behaviorStep + 1;
    }
    player->unkB88 = 0x400;
    player->velocity.x = 0;
    player->velocity.z = 0;
    player->unkB84 |= 0x20;
    player->velocity.y = player->unkB8C;
    player->unkB8C = player->unkB8C + 0x8000;
    decayPlayerAirborneAngles(player);
    player->rotY = (u16)player->rotY + 0x200;
    applyVelocityToPosition(player);
    if (player->unkB8C > 0x7FFFF) {
        if (player->unkBD4 != 0) {
            if (player->unkBD2 != 0) {
                if (randA() & 1) {
                    if (spawnItemHomingProjectile(
                            &player->worldPos.x,
                            player->sectorIndex,
                            &player->velocity.x,
                            0,
                            player->unkBD2
                        ) != 0) {
                        player->unkBD2 = 0;
                        player->unkBD3 = 0;
                    }
                    goto spawn_done;
                }
                if (spawnItemHomingProjectile(
                        &player->worldPos.x,
                        player->sectorIndex,
                        &player->velocity.x,
                        1,
                        player->unkBD4
                    ) != 0) {
                    player->unkBD4 = 0;
                }
                goto spawn_done;
            }
            goto check_secondary_item;
        }
        if (player->unkBD2 != 0) {
            if (spawnItemHomingProjectile(
                    &player->worldPos.x,
                    player->sectorIndex,
                    &player->velocity.x,
                    0,
                    player->unkBD2
                ) != 0) {
                player->unkBD2 = 0;
                player->unkBD3 = 0;
            }
        } else {
        check_secondary_item:
            if (player->unkBD4 != 0) {
                if (spawnItemHomingProjectile(
                        &player->worldPos.x,
                        player->sectorIndex,
                        &player->velocity.x,
                        1,
                        player->unkBD4
                    ) != 0) {
                    player->unkBD4 = 0;
                }
            }
        }
    spawn_done:
        setPlayerBehaviorPhase(player, 0x10);
    }
    setPlayerBodyPartAnimState(player, 3, 0);
    return 0;
}

extern s16 knockbackHomingBounceTimers;
extern u16 knockbackHomingBounceScales;

s32 updateKnockbackHomingBouncePhase(Player *arg0) {
    u8 temp_v0;

    temp_v0 = arg0->behaviorStep;
    if (temp_v0 == 0) {
        arg0->behaviorStep = temp_v0 + 1;
        arg0->unkB8C = 0;
        arg0->velocity.y = 0;
        arg0->unkA8C = 0xFFFF;
        func_8005D308_5DF08(arg0, 0xB);
        queueSoundAtPosition(&arg0->worldPos, 0x34);
        if (!(arg0->unkB84 & 1)) {
            spawnCharacterAttackEffectByType(arg0, arg0->unkBCC & 0xF);
        }
    }

    if (arg0->unkB8C == 0) {
        arg0->unkB8C = *(&knockbackHomingBounceTimers + (arg0->behaviorCounter * 2));
        arg0->unkB9E = *(&knockbackHomingBounceScales + (arg0->behaviorCounter * 2));
        arg0->behaviorCounter += 1;
    } else {
        s32 temp_v1;
        temp_v1 = arg0->unkB8C - getTrickRecoveryTimerReduction(arg0);
        arg0->unkB8C = temp_v1;
        if (temp_v1 < 0) {
            arg0->unkB8C = 0;
        }
    }

    arg0->unkB88 = 0x10;
    arg0->velocity.x = 0;
    arg0->velocity.z = 0;
    arg0->unkB84 |= 0x40;
    arg0->velocity.y -= 0x6000;
    decayPlayerSteeringAngles(arg0);
    applyVelocityToPosition(arg0);

    if (arg0->unkB8C == -1) {
        arg0->unkB88 = 0;
        setPlayerBehaviorPhase(arg0, 4);
    }

    setPlayerBodyPartAnimState(arg0, 3, 0);

    return 0;
}

void dispatchKnockbackBehaviorPhase(Player *player) {
    knockbackBehaviorPhaseHandlers[player->behaviorPhase](player);
}

void initKnockbackBehavior(BehaviorState *arg0) {
    arg0->behaviorMode = 3;
    arg0->behaviorPhase = 0;
    arg0->behaviorStep = 0;
    arg0->behaviorCounter = 0;
}

void dispatchKnockbackBehaviorStep(BehaviorState *arg0) {
    knockbackBehaviorStepHandlers[arg0->behaviorStep](arg0);
}

s32 beginKnockbackRecoveryStep(Player *player) {
    player->unkB8C = 10;
    player->unkB84 |= 0x180;

    if (player->unkB84 & 2) {
        player->behaviorCounter = 1;
        player->unkB84 &= ~2;
    }

    player->behaviorStep++;
    return 1;
}

s32 updateKnockbackRecoveryStep(Player *player) {
    s32 trackInfoBuffer[4];
    GameState *gameState;
    LevelConfig *levelData;
    s16 angleDelta;
    s16 clampedAngleDelta;
    s16 currentYaw;
    s32 currentPosX;
    s32 targetPosZ;
    s32 currentPosZ;
    s16 targetYaw;

    gameState = getCurrentAllocation();
    levelData = getLevelConfig(gameState->memoryPoolId);
    targetYaw = levelData->yawOffset + getTrackEndInfo(&gameState->gameData, trackInfoBuffer) + 0x1000;
    currentYaw = player->rotY;
    angleDelta = (targetYaw - currentYaw) & 0x1FFF;
    clampedAngleDelta = angleDelta;
    if (angleDelta >= 0x1001) {
        clampedAngleDelta = angleDelta | 0xE000;
    }
    player->velocity.y -= 0x6000;
    currentPosX = player->worldPos.x;
    player->velocity.x = 0;
    player->velocity.z = 0;
    player->rotY = currentYaw + (clampedAngleDelta / player->unkB8C);
    player->worldPos.x = currentPosX + ((levelData->shortcutPosX - currentPosX) >> 2);
    targetPosZ = levelData->shortcutPosZ;
    currentPosZ = player->worldPos.z;
    player->worldPos.z = currentPosZ + ((targetPosZ - currentPosZ) >> 2);
    applyClampedVelocityToPosition(player);
    decayPlayerSteeringAngles(player);
    player->unkB8C -= 1;

    if (player->behaviorCounter != 0) {
        if (func_8005D308_5DF08(player, 6) != 0) {
            player->behaviorCounter = 0;
        }
    } else {
        func_8005D180_5DD80(player, 0);
    }

    if (player->unkB8C == 0) {
        player->behaviorStep += 1;
    }

    return 0;
}

s32 fallToTrackCenterStep(Player *player) {
    LevelConfig *levelData;
    GameState *gameState = (GameState *)getCurrentAllocation();

    levelData = getLevelConfig(gameState->memoryPoolId);

    player->velocity.x = 0;
    player->velocity.z = 0;
    player->velocity.y = player->velocity.y - 0x6000;
    player->worldPos.x = player->worldPos.x + ((levelData->shortcutPosX - player->worldPos.x) >> 2);
    player->worldPos.z = player->worldPos.z + ((levelData->shortcutPosZ - player->worldPos.z) >> 2);
    applyClampedVelocityToPosition(player);
    decayPlayerSteeringAngles(player);

    if (func_8005D308_5DF08(player, 3) != 0) {
        player->behaviorCounter = 0;
        player->behaviorStep = player->behaviorStep + 1;
    }

    return 0;
}

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B6890_A6740);

extern Vec3i g_KnockbackRecoveryForwardVelocity;

s32 slideDuringKnockbackRecoveryStep(Player *player) {
    GameState *gameState = getCurrentAllocation();

    getLevelConfig(gameState->memoryPoolId);
    decayPlayerSteeringAngles(player);

    if (player->unkB8C != 0) {
        player->unkB8C = player->unkB8C - 1;
    } else {
        player->unkB8C = 0xB4;
        player->behaviorStep = player->behaviorStep + 1;
    }

    if (player->unkB8C < 0x12) {
        player->unkB84 = player->unkB84 | 0x2000;
        transformVector2(&g_KnockbackRecoveryForwardVelocity, &player->unk970, &player->velocity);
    }

    applyClampedVelocityToPosition(player);
    func_8005D308_5DF08(player, 0xD);

    return 0;
}

extern Vec3i g_KnockbackDiagonalSlideVelocity;
extern Vec3i g_FinishLineRespawnOffset;

s32 slideDiagonallyDuringKnockbackRecoveryStep(Player *player) {
    s32 pad[12];
    GameState *gameState = getCurrentAllocation();

    getLevelConfig(gameState->memoryPoolId);
    decayPlayerSteeringAngles(player);
    transformVector2(&g_KnockbackDiagonalSlideVelocity, &player->unk970, &player->velocity);
    applyClampedVelocityToPosition(player);

    if (player->unkB8C != 0) {
        player->unkB8C--;
        if (player->unkB8C == 0x11) {
            setViewportFadeValueBySlotIndex(player->playerIndex, 0xFF, 0x10);
        }
    } else {
        player->behaviorStep++;
        player->behaviorCounter = 0;
    }

    func_8005D308_5DF08(player, 0xD);
    return 0;
}

s32 respawnAtFinishLineAndSlideStep(Player *player) {
    Vec3i sp10;
    s32 pad[8];
    GameState *gameState;
    LevelConfig *levelData;

    gameState = getCurrentAllocation();
    levelData = getLevelConfig(gameState->memoryPoolId);

    if (player->behaviorCounter == 0) {
        player->rotY = 0x1000;
        player->shortcutLapCount = 0;
        player->sectorIndex = 0;
        player->unkB84 = player->unkB84 | 0x200;
        player->behaviorCounter++;
        player->currentLap++;

        createYRotationMatrix(&player->unk970, 0x1000);
        transformVector2(&g_FinishLineRespawnOffset, &player->unk970, &sp10);

        player->worldPos.x = levelData->spawnPos.x + sp10.x;
        player->worldPos.y = levelData->spawnPos.y + sp10.y;
        player->worldPos.z = levelData->spawnPos.z + sp10.z;

        memcpy(&player->unk440, &player->worldPos, sizeof(Vec3i));

        player->finishAnimState = 1;
        player->unkB8C = 0x32;

        setViewportFadeValueBySlotIndex(player->playerIndex, 0, 0x10);

        if (player->isBossRacer == 0) {
            if (player->currentLap == gameState->finalLapNumber) {
                showGoalBanner(player->playerIndex);
            }
        }
    }

    decayPlayerSteeringAngles(player);
    transformVector2(&g_KnockbackDiagonalSlideVelocity, &player->unk970, &player->velocity);
    applyClampedVelocityToPosition(player);

    if (player->unkB8C != 0) {
        player->unkB8C--;
    } else {
        player->unkB8C = 0x2C;
        player->behaviorStep++;
    }

    func_8005D308_5DF08(player, 0xD);
    return 0;
}

s32 slideForwardAndResetStep(Player *player) {
    GameState *gameState = getCurrentAllocation();

    getLevelConfig(gameState->memoryPoolId);
    transformVector2(&g_KnockbackRecoveryForwardVelocity, &player->unk970, &player->velocity);
    applyClampedVelocityToPosition(player);
    decayPlayerSteeringAngles(player);

    if (player->unkB8C != 0) {
        player->unkB8C = player->unkB8C - 1;
    } else {
        player->unkB84 = player->unkB84 & ~0x2380;
        resetPlayerBehaviorToDefault(player);
    }

    func_8005D308_5DF08(player, 0xD);
    return 0;
}

s32 dropDownwardStep(Player *player) {
    player->velocity.x = 0;
    player->velocity.z = 0;
    player->velocity.y -= 0x6000;
    applyClampedVelocityToPosition(player);
    decayPlayerSteeringAngles(player);
    func_8005D308_5DF08(player, 3);

    player->unkB8C--;
    if (player->unkB8C == 0) {
        player->behaviorCounter = 0;
        player->behaviorStep++;
        memcpy(&player->unk470, &player->worldPos.x, 0xC);
        player->unk480 = 0;
        player->unk468 = 0x30000;
        player->unkB8C = 0;
    }

    return 0;
}

s32 jumpUpwardStep(Player *player) {
    s32 yOffsetDelta;
    s32 prevYOffset;
    s32 updatedYOffset;
    s32 nextFrameCounter;

    yOffsetDelta = player->storedPosY;
    prevYOffset = player->unk474;
    yOffsetDelta = (yOffsetDelta - prevYOffset) * player->unkB8C;
    if (yOffsetDelta < 0) {
        yOffsetDelta += 0xF;
    }
    player->unk474 = (yOffsetDelta >> 4) + prevYOffset;
    player->unk480 = player->unk480 + player->unk468;
    updatedYOffset = player->unk474;
    nextFrameCounter = player->unkB8C + 1;
    player->unkB8C = nextFrameCounter;
    player->unk468 = player->unk468 - 0x6000;
    player->worldPos.y = updatedYOffset + player->unk480;
    decayPlayerSteeringAngles(player);
    func_8005D308_5DF08(player, 4);
    if (player->unkB8C == 16) {
        s32 flags = player->unkB84;
        u8 step = player->behaviorStep;
        player->behaviorCounter = 0;
        step = step + 1;
        flags = flags | 0x2000;
        player->unkB84 = flags;
        player->behaviorStep = step;
        queueSoundAtPosition(&player->worldPos, 0x25);
    }
    return 0;
}

s32 restoreStoredPositionStep(Player *arg0) {
    arg0->worldPos.x = arg0->storedPosX;
    arg0->worldPos.y = arg0->storedPosY;
    arg0->worldPos.z = arg0->storedPosZ;
    arg0->unkA8E = arg0->storedRotY;

    if (func_8005D308_5DF08(arg0, 5) != 0) {
        arg0->behaviorCounter = 0;
        arg0->behaviorStep = arg0->behaviorStep + 1;
    }

    return 0;
}

s32 handleUfoStoredPositionStep(Player *player) {
    GameState *gameState;
    u8 flags;

    gameState = getCurrentAllocation();
    player->worldPos.x = player->storedPosX;
    player->worldPos.y = player->storedPosY;
    player->worldPos.z = player->storedPosZ;
    player->unkA8E = player->storedRotY;
    flags = player->ufoFlags;

    if (flags & 8) {
        player->ufoFlags = flags & 0xF7;
        setViewportFadeValueBySlotIndex(player->playerIndex, 0xFF, 0x10);
    }

    if (player->ufoFlags & 2) {
        player->rotY = 0x1000;
        player->shortcutLapCount = 0;
        player->sectorIndex = 0;
        player->behaviorStep++;
        player->unkB84 |= 0x200;
        player->currentLap++;
        memcpy(&player->unk440, &player->worldPos.x, 0xC);
        player->finishAnimState = 1;
        setViewportFadeValueBySlotIndex(player->playerIndex, 0, 0x10);
        if (player->isBossRacer == 0 && player->currentLap == gameState->finalLapNumber) {
            showGoalBanner(player->playerIndex);
        }
    }

    func_8005D308_5DF08(player, 0);

    return 0;
}

s32 handleUfoAbductionRecoveryStep(Player *arg0) {
    arg0->worldPos.x = arg0->storedPosX;
    arg0->worldPos.y = arg0->storedPosY;
    arg0->worldPos.z = arg0->storedPosZ;
    arg0->unkA8E = arg0->storedRotY;

    func_8005D308_5DF08(arg0, 0);

    if (arg0->ufoFlags & 0x4) {
        arg0->unkB8C = 6;
        arg0->behaviorStep = arg0->behaviorStep + 1;
    }

    return 0;
}

s32 waitAtStoredPositionStep(Player *arg0) {
    arg0->worldPos.x = arg0->storedPosX;
    arg0->worldPos.y = arg0->storedPosY;
    arg0->worldPos.z = arg0->storedPosZ;
    arg0->unkA8E = arg0->storedRotY;

    func_8005D308_5DF08(arg0, 3);

    arg0->unkB8C = arg0->unkB8C - 1;
    if (arg0->unkB8C == 0) {
        arg0->unkB8C = 0xE;
        arg0->behaviorCounter = 0;
        arg0->behaviorStep = arg0->behaviorStep + 1;
    }

    return 0;
}

s32 handleFallFromUfoStep(Player *player) {
    if (player->behaviorCounter == 0) {
        player->velocity.z = 0xFFF80000;
        player->velocity.x = 0;
        player->velocity.y = 0x30000;
        player->behaviorCounter++;
    }

    player->velocity.y -= 0x6000;
    applyClampedVelocityToPosition(player);
    decayPlayerSteeringAngles(player);

    if (player->unkB8C != 0) {
        player->unkB8C--;
    } else {
        player->unkB84 &= ~0x2380;
        resetPlayerBehaviorToDefault(player);
    }

    func_8005D308_5DF08(player, 4);
    return 0;
}

s32 dropAfterUfoReleaseStep(Player *player) {
    player->velocity.x = 0;
    player->velocity.z = 0;
    player->velocity.y = player->velocity.y - 0x6000;
    applyClampedVelocityToPosition(player);
    decayPlayerSteeringAngles(player);
    func_8005D180_5DD80(player, 0);

    player->unkB8C = player->unkB8C - 1;
    if (player->unkB8C == 0) {
        player->behaviorCounter = 0;
        player->unkB8C = 0;
        player->behaviorStep = player->behaviorStep + 1;
        spawnDebugDisplayListTask(0);
        queueSoundAtPosition(&player->worldPos, 0x26);
    }

    return 0;
}

s32 spinRampUpStep(Player *arg0) {
    arg0->velocity.x = 0;
    arg0->velocity.z = 0;
    arg0->velocity.y = arg0->velocity.y - 0x6000;
    applyClampedVelocityToPosition(arg0);
    decayPlayerSteeringAngles(arg0);
    func_8005D180_5DD80(arg0, 0);
    arg0->rotY = arg0->rotY + arg0->unkB8C;
    arg0->unkB8C = arg0->unkB8C + 0x10;

    if (arg0->unkB8C == 0x400) {
        arg0->unkB8C = 10;
        arg0->behaviorStep = arg0->behaviorStep + 1;
    }

    return 0;
}

typedef struct {
    u8 _pad0[0x63];
    u8 shortcutGateState; // 0x63
} MaintainMaxSpinStep_alloc;

s32 maintainMaxSpinStep(Player *arg0) {
    MaintainMaxSpinStep_alloc *alloc = (MaintainMaxSpinStep_alloc *)getCurrentAllocation();
    s32 timerRemaining;

    arg0->velocity.x = 0;
    arg0->velocity.z = 0;
    arg0->velocity.y = arg0->velocity.y - 0x6000;
    applyClampedVelocityToPosition(arg0);
    decayPlayerSteeringAngles(arg0);
    func_8005D180_5DD80(arg0, 0);
    arg0->rotY = arg0->rotY + 0x400;
    timerRemaining = arg0->unkB8C - 1;
    arg0->unkB8C = timerRemaining;

    if (timerRemaining == 0) {
        u8 nextStep = arg0->behaviorStep;
        s32 flags = arg0->unkB84;
        arg0->unkBA0 = 0x2000;
        arg0->unkBA2 = 0x2000;
        arg0->unkB8C = 10;
        nextStep = nextStep + 1;
        flags = flags | 0x800000;
        arg0->behaviorStep = nextStep;
        arg0->unkB84 = flags;
        alloc->shortcutGateState = alloc->shortcutGateState & 2;
        queueSoundAtPosition(&arg0->worldPos, 0x4E);
    }

    return 0;
}

typedef struct {
    u8 _pad0[0x81];
    u8 shortcutWarpPlayerCount; // 0x81 - Number of players in shortcut warp sequence
} SpinRampDownStep_alloc;

s32 spinRampDownStep(Player *player) {
    SpinRampDownStep_alloc *alloc = (SpinRampDownStep_alloc *)getCurrentAllocation();
    u16 spinDecrementCounter;
    u16 spinIncrementCounter;

    player->velocity.x = 0;
    player->velocity.z = 0;
    player->velocity.y = player->velocity.y - 0x6000;
    applyClampedVelocityToPosition(player);
    decayPlayerSteeringAngles(player);
    func_8005D180_5DD80(player, 0);

    spinDecrementCounter = player->unkBA0;
    spinIncrementCounter = player->unkBA2;
    spinDecrementCounter -= 0x400;
    player->unkBA0 = spinDecrementCounter;
    player->rotY = player->rotY + 0x400;
    spinIncrementCounter += 0x400;
    player->unkBA2 = spinIncrementCounter;

    if (player->unkBA0 == 0) {
        player->unkB8C = 0x11;
        player->behaviorStep = player->behaviorStep + 1;
        setViewportFadeValueBySlotIndex(player->playerIndex, 0xFF, 0x10);
        alloc->shortcutWarpPlayerCount = alloc->shortcutWarpPlayerCount + 1;
        spawnDebugDisplayListTask(1);
    }

    return 0;
}

typedef struct {
    u8 _pad0[0x80];
    u8 fadeInPlayerCount; // 0x80 - Number of players in fade-in wait phase
} SpinFadeInWaitStep_alloc;

s32 spinFadeInWaitStep(Player *player) {
    SpinFadeInWaitStep_alloc *alloc = (SpinFadeInWaitStep_alloc *)getCurrentAllocation();
    s32 timerRemaining;

    player->velocity.x = 0;
    player->velocity.z = 0;
    player->velocity.y = player->velocity.y - 0x6000;
    applyClampedVelocityToPosition(player);
    decayPlayerSteeringAngles(player);
    func_8005D180_5DD80(player, 0);
    player->rotY = player->rotY + 0x400;

    timerRemaining = player->unkB8C - 1;
    player->unkB8C = timerRemaining;
    if (timerRemaining == 0) {
        player->behaviorStep = player->behaviorStep + 1;
        alloc->fadeInPlayerCount = alloc->fadeInPlayerCount - 1;
    }

    return 0;
}

s32 warpToShortcutSpinUpStep(Player *player) {
    GameState *gameState;
    LevelConfig *shortcutConfig;
    s32 pad[12];

    gameState = getCurrentAllocation();
    shortcutConfig = getLevelConfig(gameState->memoryPoolId);

    if (player->behaviorCounter == 0) {
        player->rotY = 0xE00;
        player->shortcutLapCount = 0;
        player->sectorIndex = 0;
        player->unkB84 |= 0x200;
        player->behaviorCounter++;
        player->currentLap++;

        createYRotationMatrix(&player->unk970, 0xE00);

        player->worldPos.x = shortcutConfig->spawnPos.x;
        player->worldPos.y = shortcutConfig->spawnPos.y;
        player->worldPos.z = shortcutConfig->spawnPos.z;

        memcpy(&player->unk440, &player->worldPos.x, 0xC);

        player->finishAnimState = 2;
        setViewportFadeValueBySlotIndex(player->playerIndex, 0, 0x10);

        if (player->isBossRacer == 0) {
            if (player->currentLap == gameState->finalLapNumber) {
                showGoalBanner(player->playerIndex);
            }
        }

        queueSoundAtPosition(&player->worldPos, 0x26);
    }

    decayPlayerSteeringAngles(player);
    func_8005D180_5DD80(player, 0);

    player->unkBA0 += 0x400;
    player->unkB8C = 0x400;
    player->rotY = player->rotY + 0x400;
    player->unkBA2 = player->unkBA2 - 0x400;

    if (player->unkBA0 == 0x2000) {
        player->unkB84 &= 0xFF7FFFFF;
        player->behaviorStep++;
    }

    player->unkB84 |= 0x10000;
    return 0;
}

s32 shortcutSpinDownStep(Player *player) {
    SpinRampDownStep_alloc *alloc = (SpinRampDownStep_alloc *)getCurrentAllocation();
    s32 newSpinRate;

    decayPlayerSteeringAngles(player);
    func_8005D180_5DD80(player, 0);

    player->rotY = player->rotY + player->unkB8C;
    newSpinRate = player->unkB8C - 0x10;
    player->unkB8C = newSpinRate;

    if (newSpinRate == 0) {
        player->unkB8C = 6;
        player->behaviorStep = player->behaviorStep + 1;
        alloc->shortcutWarpPlayerCount = alloc->shortcutWarpPlayerCount - 1;
    }

    player->unkB84 = player->unkB84 | 0x10000;
    return 0;
}

s32 shortcutPostSpinWaitStep(Player *player) {
    func_8005D308_5DF08(player, 3);

    player->unkB8C = player->unkB8C - 1;
    if (player->unkB8C == 0) {
        player->unkB8C = 0xE;
        player->behaviorCounter = 0;
        player->behaviorStep = player->behaviorStep + 1;
    }

    player->unkB84 = player->unkB84 | 0x10000;
    return 0;
}

s32 shortcutLaunchStep(Player *player) {
    if (player->behaviorCounter == 0) {
        player->velocity.z = 0xFFF80000;
        player->velocity.x = 0;
        player->velocity.y = 0x30000;
        player->behaviorCounter = player->behaviorCounter + 1;
    }

    player->unkB84 = player->unkB84 | 0x10000;
    player->velocity.y = player->velocity.y - 0x6000;
    applyClampedVelocityToPosition(player);
    decayPlayerSteeringAngles(player);

    if (player->unkB8C != 0) {
        player->unkB8C = player->unkB8C - 1;
    } else {
        player->unkB84 = player->unkB84 & 0xFFFFDC7F;
        resetPlayerBehaviorToDefault(player);
    }

    func_8005D308_5DF08(player, 4);
    return 0;
}

extern u8 gRumbleDurations[];

void startRumbleEffect(Player *player, s32 effectType) {
    player->rumbleEffectType = effectType;
    player->rumbleCounter = -1;
    player->rumbleDuration = gRumbleDurations[effectType];
}

u8 getRumbleDuration(Player *player, s32 effectType) {
    player->rumbleEffectType = effectType;
    player->rumbleDuration = gRumbleDurations[effectType];
    return player->rumbleDuration;
}

INCLUDE_ASM("asm/nonmatchings/9FF70", updateAndRenderRaceCharacters);

void func_800B82D8_A8188(Player *arg0) {
    Vec3i savedPos;
    Vec3i transformedVec;
    Vec3i temp;
    Transform3D matrix;
    GameState *gs;
    s32 sqrtDist;
    s32 var_s7;
    s32 temp_v0;
    s16 angle;
    s32 var_v0;
    Vec3i *temp2;
    s32 temp3;

    gs = getCurrentAllocation();

    switch (gs->raceType) {
        default:
        case 0:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            handlePlayerToPlayerCollision(arg0);
            break;
        case 1:
        case 2:
        case 3:
            handleCollisionWithTargetPlayer(arg0);
            break;
    }

    arg0->sectorIndex = getOrUpdatePlayerSectorIndex(arg0, (u8 *)&gs->gameData, arg0->sectorIndex, &arg0->worldPos);
    temp2 = &savedPos;
    memcpy(&arg0->unk970.translation, &arg0->worldPos, 0xC);

    if (!(arg0->unkB84 & 0x100)) {
        memcpy(temp2, &arg0->worldPos, sizeof(Vec3i));

        if (func_80059ED0_5AAD0(arg0) == -1 && ((savedPos.x != arg0->worldPos.x) || (savedPos.z != arg0->worldPos.z))) {
            arg0->unkB84 |= 0x10;
            savedPos.x = arg0->worldPos.x - savedPos.x;
            savedPos.z = arg0->worldPos.z - savedPos.z;
            sqrtDist = isqrt64((s64)savedPos.x * savedPos.x + (s64)savedPos.z * savedPos.z);

            if (sqrtDist > 0x8000) {
                if (arg0->unkB84 & 0x1000) {
                    setPlayerPullState(arg0, &savedPos);
                } else {
                    createYRotationMatrix(&matrix, arg0->rotY + arg0->unkA90);
                    transformVector3(&savedPos, &matrix, &transformedVec);
                    var_s7 = 0;
                    angle = atan2Fixed(transformedVec.x, transformedVec.z);
                    var_v0 = (angle - 0x981) < 0x67FU;
                    if (arg0->unkB84 & 2) {
                        temp3 = (angle - 0x1000) < 0x680U;
                        var_s7 = temp3;
                        var_v0 = (angle - 0x981) < 0x67FU;
                    } else {
                        if (angle >= 0x1981) {
                            var_s7 = 1;
                        }
                        var_v0 = angle < 0x680;
                    }

                    if (var_v0) {
                        var_s7 = 2;
                    }

                    if (var_s7 != 0) {
                        if (isqrt64(
                                (s64)arg0->velocity.x * arg0->velocity.x + (s64)arg0->velocity.z * arg0->velocity.z
                            ) > 0x30000) {
                            s32 normX = ((s64)savedPos.x * 0x2000) / sqrtDist;
                            s32 normZ = ((s64)savedPos.z * 0x2000) / sqrtDist;
                            s32 negNormX;
                            s32 dotPerp;
                            s32 dotPar;
                            s32 parComponent;

                            negNormX = -normX;

                            dotPerp = ((s64)normZ * arg0->velocity.x + (s64)negNormX * arg0->velocity.z) / 0x2000;
                            dotPar = ((s64)normX * arg0->velocity.x + (s64)normZ * arg0->velocity.z) / 0x2000;

                            parComponent = dotPar;
                            if (parComponent < 0) {
                                parComponent = -parComponent;
                                if (parComponent > 0x40000) {
                                    parComponent = 0x40000;
                                }

                                transformedVec.x =
                                    ((s64)normZ * dotPerp) / 0x2000 + ((s64)normX * parComponent) / 0x2000;
                                transformedVec.z =
                                    ((s64)negNormX * dotPerp) / 0x2000 + ((s64)normZ * parComponent) / 0x2000;

                                setPlayerHitStunState(arg0, var_s7, &transformedVec);
                            }
                        }
                    }
                }
            }
        } else {
            arg0->unkB84 &= ~0x10;
        }

        updatePlayerSectorAndClampYToTrack(arg0);

        temp_v0 = getTrackHeightInSector(&gs->gameData, arg0->sectorIndex, &arg0->worldPos, 0x100000) + 0x3C00000;
        if (temp_v0 < arg0->worldPos.y) {
            arg0->worldPos.y = temp_v0;
        }
    }

    if (!(arg0->unkB84 & 0x200)) {
        func_8005A26C_5AE6C(arg0);
        memcpy(&arg0->unk970.translation, &arg0->worldPos, sizeof(Vec3i));
    }

    if (arg0->unkB84 & 0x20000) {
        arg0->unkB84 &= ~1;
    }

    func_8005CFFC_5DBFC(
        (TrackGeometryFaceData *)&gs->gameData,
        arg0->sectorIndex,
        &arg0->worldPos,
        &arg0->unkBC9,
        &arg0->unkBCC
    );

    arg0->unkBCA = arg0->unkBC9 >> 4;
    arg0->unkBC9 &= 0xF;

    if (arg0->unkB84 & 0x10000) {
        arg0->unkBC9 = 0;
    } else if (arg0->unkB84 & 0x20000) {
        arg0->unkBC9 = 0;
        arg0->unkBCC &= 0xF0;
    }

    if (arg0->unkBC9 == 1) {
        if ((arg0->unkBCB & 0xFF) == 3) {
            setPlayerState80(arg0);
        } else {
            arg0->unkBCB++;
        }
    } else {
        arg0->unkBCB = 0;
    }

    if (arg0->unkBC9 == 2) {
        setPlayerState100(arg0);
    }
}

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B8894_A8744);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B9500_A93B0);

void updatePlayerJointPositions(Player *player) {
    GameState *alloc;
    GameDataLayout *gameData;
    s32 i;
    s32 offset;
    u8 *jointWritePtr;
    JointPosition *jointPos;
    s32 sectorIndex;

    alloc = getCurrentAllocation();
    i = 0;
    gameData = &alloc->gameData;
    offset = 0xA10;
    jointWritePtr = (u8 *)player;

    do {
        jointPos = (JointPosition *)((u8 *)player + offset);
        *(volatile s32 *)(jointWritePtr + 0xA10) =
            player->unk970.translation.x + *(s32 *)((u8 *)&D_800BA348_AA1F8 + offset);
        *(volatile s32 *)(jointWritePtr + 0xA18) =
            player->unk970.translation.z + *(s32 *)((u8 *)&D_800BA350_AA200 + offset);

        sectorIndex = getOrUpdatePlayerSectorIndex((void *)player, gameData, player->sectorIndex, jointPos);
        *(volatile s32 *)(jointWritePtr + 0xA14) = getTrackHeightInSector(gameData, sectorIndex, jointPos, 0x100000);

        i++;
        offset += 0xC;
        jointWritePtr += 0xC;
    } while (i < 9);

    player->unkBC1 = 1;

    if (!(player->unkB84 & 0x800000)) {
        for (i = 0; i < 4; i++) {
            debugEnqueueCallback(i, 1, func_800B9500_A93B0, (void *)player);
        }
    }
}

void loadPlayerCharacterAssets(void *varg0) {
    Player *player = (Player *)varg0;
    GameStatePartial5C *gameState;
    u8 characterId;
    u8 boardType;

    gameState = (GameStatePartial5C *)getCurrentAllocation();
    player->unk4 = loadAssetByIndex_94F90(player->characterId, player->boardIndex);
    player->unk8 = loadAssetByIndex_95200(player->characterId, player->boardIndex);
    player->unk0 = loadAssetByIndex_953E0(player->characterId);

    characterId = player->characterId;
    if (characterId < 6) {
        player->unk20 = loadAssetDataDMA(player->characterId, player->boardIndex);
        player->unk24 = loadAssetDataQueuedDMA(player->characterId, player->boardIndex);
    }

    player->unkC = loadAssetByIndex_95500(player->unkBBB);
    player->unk10 = loadAssetByIndex_95590(player->unkBBB);

    boardType = player->unkBBB;
    if (boardType < 9) {
        player->unk14 = loadAssetByIndex_95668(player->unkBBC);
    } else {
        player->unk14 = NULL;
    }

    player->unk18 = load_3ECE40();

    if (player->isBossRacer != 0) {
        player->unk1C = loadBossHudAssetByRaceType(gameState->unk5C);
    }

    if (player->unkBE3 != 0) {
        player->unk2C = loadAssetByIndex_5E990(player->unkBE4);
    }
}

void freePlayerCharacterAssets(void *varg0) {
    Player *player = (Player *)varg0;
    s32 pad[8];

    player->unk4 = freeNodeMemory(player->unk4);
    player->unk8 = freeNodeMemory(player->unk8);
    player->unk0 = freeNodeMemory(player->unk0);
    player->unk20 = freeNodeMemory(player->unk20);
    player->unk24 = freeNodeMemory(player->unk24);
    player->unkC = freeNodeMemory(player->unkC);
    player->unk10 = freeNodeMemory(player->unk10);
    player->unk14 = freeNodeMemory(player->unk14);
    player->unk18 = freeNodeMemory(player->unk18);
    player->unk1C = freeNodeMemory(player->unk1C);
    player->unk2C = freeNodeMemory(player->unk2C);
}
