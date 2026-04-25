#include "race/hit_reactions.h"
#include "audio/audio.h"
#include "common.h"
#include "gamestate.h"
#include "math/geometry.h"
#include "math/rand.h"
#include "race/particle_items.h"
#include "race/race_session.h"
#include "race/track_height.h"

extern u16 D_80093F90_94B90[];
extern u16 D_80093FA4_94BA4[];
extern u16 D_80093FB8_94BB8[];
extern u16 D_80093FCC_94BCC[];
extern u16 D_80093FE0_94BE0[];
extern u16 gFinishBoostVoiceSounds[];
extern u16 D_80094008_94C08[];
extern u16 gStunnedVoiceSounds1[];
extern u16 gStunnedVoiceSounds2[];
extern u16 gCharacterVoiceSounds1[];
extern u16 gCharacterVoiceSounds2[];

void setPlayerHitStunState(Player *player, s16 hitState, void *knockbackData) {
    if (player->hitReactionState != 0) {
        return;
    }
    if (player->behaviorFlags & ~0x80) {
        return;
    }
    player->hitReactionState = hitState;
    memcpy(&player->knockbackVelocity, knockbackData, sizeof(Vec3i));
}

void setPlayerProjectileHitState(Player *player) {
    if (player->hitReactionState < 3) {
        if (!(player->behaviorFlags & 0x77F)) {
            player->hitReactionState = 3;
        }
    }
}

void setPlayerCollisionKnockbackState(Player *player, s16 knockbackAngle, s32 knockbackStrength) {
    if (player->hitReactionState < 4) {
        if ((player->behaviorFlags & 0x77F) == 0) {
            player->hitReactionState = 4;
            player->knockbackAngle = knockbackAngle;
            player->knockbackVelocity.x = 0;
            player->knockbackVelocity.y = 0;
            if (knockbackStrength > 0x60000) {
                knockbackStrength = 0x60000;
            }
            player->knockbackVelocity.z = -knockbackStrength;
        }
    }
}

void setPlayerState50(Player *player) {
    if (player->hitReactionState < 0x32) {
        if (!(player->behaviorFlags & 0x71F)) {
            player->hitReactionState = 0x32;
        }
    }
}

void setPlayerPullState(Player *player, void *pullTarget) {
    if (player->hitReactionState < 0x31) {
        if (!(player->behaviorFlags & 0x71F)) {
            player->hitReactionState = 0x31;
            memcpy(&player->knockbackVelocity, pullTarget, sizeof(Vec3i));
        }
    }
}

void setPlayerHomingProjectileRetaliationState(Player *player) {
    if (player->hitReactionState < 0x33) {
        if (!(player->behaviorFlags & 0x59F)) {
            player->hitReactionState = 0x33;
        }
    }
}

void setPlayerParachuteState(Player *player) {
    if (player->hitReactionState < 0x34) {
        if (!(player->behaviorFlags & 0x4BC)) {
            player->hitReactionState = 0x34;
        }
    }
}

void setPlayerShrinkState(Player *arg0) {
    if (arg0->hitReactionState < 0x35) {
        if (!(arg0->behaviorFlags & 0xD8)) {
            arg0->hitReactionState = 0x35;
        }
    }
}

void setPlayerPanelHitState(Player *arg0) {
    if (arg0->hitReactionState < 0x36) {
        if (!(arg0->behaviorFlags & 0x49C)) {
            arg0->hitReactionState = 0x36;
        }
    }
}

void setPlayerFrozenState(Player *player) {
    if (player->hitReactionState < 0x3C) {
        if (!(player->behaviorFlags & 0x9C)) {
            player->hitReactionState = 0x3C;
        }
    }
}

void setPlayerStarHitState(Player *player, Vec3i *hitPosition) {
    if (player->hitReactionState < 0x3C) {
        if (!(player->behaviorFlags & 0x218)) {
            player->hitReactionState = 0x3D;
            memcpy(&player->knockbackVelocity, hitPosition, sizeof(Vec3i));
        }
    }
}

void setPlayerBouncedBackState(Player *player) {
    if (player->hitReactionState < 0x3C) {
        if (player->invincibilityTimer == 0) {
            if (!(player->behaviorFlags & 0x18)) {
                player->hitReactionState = 0x3E;
            }
        }
    }
}

void setPlayerState80(Player *player) {
    if (player->hitReactionState < 0x50) {
        if (!(player->behaviorFlags & 0x1C)) {
            player->hitReactionState = 0x50;
        }
    }
}

void setPlayerState100(Player *player) {
    if (!(player->behaviorFlags & 8)) {
        player->hitReactionState = 100;
    }
}

s32 tryActivateFinishBoost(Player *arg0) {
    if (arg0->boostState == 0) {
        if (spawnPlayerFlashEffect(arg0) != 0) {
            if (arg0->animFlags & 2) {
                arg0->boostState = 4;
            } else {
                arg0->boostState = 3;
            }
            arg0->boostTimer = 0x5A;
            return 1;
        }
    }
    return 0;
}

s32 tryActivateGhostEffect(Player *player) {
    if (player->ghostEffectState == 0) {
        if (spawnGhostEffect(player) != NULL) {
            player->ghostEffectState = 1;
            player->ghostEffectTimer = 300;
            return 1;
        }
    }
    return 0;
}

s32 tryActivateBoost(Player *arg0) {
    if (arg0->boostState == 0) {
        if (spawnPlayerAuraEffect(arg0) != NULL) {
            if (arg0->animFlags & 2) {
                arg0->boostState = 2;
            } else {
                arg0->boostState = 1;
            }
            arg0->boostTimer = 0xB4;
            return 1;
        }
    }
    return 0;
}

s32 grantInvincibilityWithSound(Player *player) {
    if (player->invincibilityTimer == 0) {
        player->invincibilityTimer = 0x96;
        queueSoundAtPosition(&player->worldPos, 0x48);
        return 1;
    }
    return 0;
}

extern void *getCurrentAllocation(void);
s32 findPrimaryItemTarget(Player *);
s32 shouldUseSecondaryItem(Player *);
extern s32 createWarpEffect(void *, void *, s16);
extern s32 spawnAttackProjectile(s32, s32, s32);
extern s32 spawnStarEffect(void *, void *, s16);
extern s32 spawnGoldStealEffect(void *, void *, s16);
extern s32 spawnPanelProjectile(void *);
extern s32 getFreeNodeCount(s32);

void processPlayerItemUsage(Player *player) {
    GameState *gs;
    s32 delay;
    s32 i;
    s32 result;
    Player *target;
    s32 targetIndices[2];
    u8 playerCount;
    s32 *idxPtr;
    s32 pad;

    gs = (GameState *)getCurrentAllocation();
    result = findPrimaryItemTarget(player);

    if (result >= 0) {
        if (spawnAttackProjectile(player->primaryItemId - 1, player->playerIndex, result) != 0) {
            player->primaryItemAmmo--;
            if (player->primaryItemAmmo == 0) {
                player->primaryItemId = 0;
            }
        }
    }

    if (shouldUseSecondaryItem(player) != 0) {
        switch (player->secondaryItemId) {
            case 1:
                result = gs->numPlayers - 1;
                if (result != 0) {
                    if (getFreeNodeCount(0) < result) {
                        break;
                    }
                    delay = 0;
                    for (i = 0; i < gs->numPlayers; i++) {
                        if (player->playerIndex != gs->players[i].playerIndex) {
                            createWarpEffect(player, &gs->players[i], (s16)delay);
                            delay += 4;
                        }
                    }
                } else {
                    if (createWarpEffect(player, player, 0) == 0) {
                        break;
                    }
                }
                player->secondaryItemId = 0;
                queueSoundAtPosition(&player->worldPos, 0x18);
                break;

            case 2:
                target = player;
                if (randA() & 0xFF) {
                    for (result = 0; result < gs->numPlayers; result++) {
                        if (gs->players[gs->PAD_6B_2[result]].playerIndex != player->playerIndex) {
                            target = &gs->players[gs->PAD_6B_2[result]];
                            if (target->unkBC6 == 0) {
                                break;
                            }
                        }
                    }
                }
                if (spawnStarEffect(player, target, 0) != 0) {
                    player->secondaryItemId = 0;
                }
                break;

            case 3:
                randA();
                if (getFreeNodeCount(0) < 3) {
                    break;
                }
                switch (gs->numPlayers) {
                    case 1:
                        for (i = 0; i < 3; i++) {
                            spawnStarEffect(player, player, (s16)(i * 8));
                        }
                        player->secondaryItemId = 0;
                        break;
                    case 2:
                        for (i = 0; i < gs->numPlayers; i++) {
                            if (player->playerIndex != gs->players[i].playerIndex) {
                                for (delay = 0; delay < 3; delay++) {
                                    spawnStarEffect(player, &gs->players[i], (s16)(delay * 8));
                                }
                                break;
                            }
                        }
                        player->secondaryItemId = 0;
                        break;
                    case 3:
                        i = 0;
                        idxPtr = targetIndices;
                        for (; i < gs->numPlayers; i++) {
                            playerCount = gs->PAD_6B_2[i];
                            if (player->playerIndex != gs->players[playerCount].playerIndex) {
                                *idxPtr = playerCount;
                                idxPtr++;
                            }
                        }
                        spawnStarEffect(player, &gs->players[targetIndices[0]], 0);
                        spawnStarEffect(player, &gs->players[targetIndices[0]], 8);
                        spawnStarEffect(player, &gs->players[targetIndices[1]], 16);
                        player->secondaryItemId = 0;
                        break;
                    case 4:
                        delay = 0;
                        for (i = 0; i < gs->numPlayers; i++) {
                            if (player->playerIndex != gs->players[i].playerIndex) {
                                spawnStarEffect(player, &gs->players[i], (s16)delay);
                                delay += 8;
                            }
                        }
                        player->secondaryItemId = 0;
                        break;
                    default:
                        player->secondaryItemId = 0;
                        break;
                }
                break;

            case 4:
                if (tryActivateBoost(player) != 0) {
                    player->secondaryItemId = 0;
                }
                if (player->boostState == 0) {
                    if (spawnPlayerAuraEffect(player) != NULL) {
                        if (player->animFlags & 2) {
                            player->boostState = 2;
                        } else {
                            player->boostState = 1;
                        }
                        player->boostTimer = 0xB4;
                        player->secondaryItemId = 0;
                    }
                }
                break;

            case 5:
                if (tryActivateFinishBoost(player) != 0) {
                    player->secondaryItemId = 0;
                    playFinishBoostVoice(player);
                }
                break;

            case 6:
                if (grantInvincibilityWithSound(player) != 0) {
                    player->secondaryItemId = 0;
                }
                break;

            case 7:
                if (spawnPanelProjectile(player) != 0) {
                    player->secondaryItemId = 0;
                }
                break;

            case 8:
                target = player;
                result = randA() & 0xFF;
                i = !result;
                if (result == 1) {
                    i = 2;
                }
                playerCount = gs->numPlayers;
                if (i >= (s32)playerCount) {
                    i = playerCount - 1;
                }
                delay = 0;
                for (result = 0; result < (s32)playerCount; result++) {
                    if (gs->players[gs->PAD_6B_2[result]].playerIndex == player->playerIndex) {
                        continue;
                    }
                    target = &gs->players[gs->PAD_6B_2[result]];
                    if (delay < i) {
                        delay++;
                        continue;
                    }
                    if (target->unkBC6 == 0) {
                        break;
                    }
                    delay++;
                }
                if (spawnGoldStealEffect(player, target, 0) != 0) {
                    player->secondaryItemId = 0;
                }
                break;

            case 9:
                result = gs->numPlayers - 1;
                if (result != 0) {
                    if (getFreeNodeCount(0) < result) {
                        break;
                    }
                    delay = 0;
                    for (i = 0; i < gs->numPlayers; i++) {
                        if (player->playerIndex != gs->players[i].playerIndex) {
                            spawnGoldStealEffect(player, &gs->players[i], (s16)delay);
                            delay += 16;
                        }
                    }
                    player->secondaryItemId = 0;
                } else {
                    if (spawnGoldStealEffect(player, player, 0) != 0) {
                        player->secondaryItemId = 0;
                    }
                }
                break;

            case 10:
                if (tryActivateGhostEffect(player) != 0) {
                    player->secondaryItemId = 0;
                }
                break;
        }
    }

    if (player->unkBC9 == 3) {
        tryActivateFinishBoost(player);
    }
    if (!(player->animFlags & 0x80000)) {
        switch (player->unkBBB) {
            case 17:
                tryActivateFinishBoost(player);
                tryActivateGhostEffect(player);
                break;
            case 16:
                tryActivateBoost(player);
                break;
            case 15:
                grantInvincibilityWithSound(player);
                break;
        }
    }
}

s32 findPrimaryItemTarget(Player *player) {
    s32 i;
    Player *otherPlayer;
    GameState *gs;
    Player *bestTarget;
    s32 targetingMode;
    u32 angleDiff;
    s32 dist;
    Vec3i posDiff;
    u8 pad[2];
    s32 numPlayers;
    s32 maxRange;

    bestTarget = NULL;
    targetingMode = 0;
    gs = getCurrentAllocation();
    if (player->inputDisabled == 0) {
        if (player->primaryItemAmmo != 0) {
            if (player->inputButtonsPressed & Z_TRIG) {
                return (*(u16 *)&player->inputStickX) == 0xF9;
            }
        }
        return -1;
    }

    if (player->primaryItemAmmo == 0) {
        player->aiPrimaryItemUseTimer = 0x100;
        return -1;
    }

    if (player->aiPrimaryItemUseTimer > gAIPlayerParams[player->speedPenaltyIndex][player->primaryItemId].delay) {
        player->aiPrimaryItemUseTimer = gAIPlayerParams[player->speedPenaltyIndex][player->primaryItemId].delay;
        player->aiPrimaryItemUseTimer -= (randA() * gAIPlayerParams[player->speedPenaltyIndex][0].delay) / 255;
    }
    maxRange = 0x05000000;
    if (player->aiPrimaryItemUseTimer <= 0) {
        numPlayers = gs->numPlayers;
        i = 0;
        if (numPlayers > 0) {
            do {
                otherPlayer = &gs->players[i];
                if (otherPlayer->playerIndex != player->playerIndex) {
                    posDiff.x = otherPlayer->worldPos.x - player->worldPos.x;
                    posDiff.z = otherPlayer->worldPos.z - player->worldPos.z;
                    angleDiff = (((s32 (*)(s32, s32))atan2Fixed)(-posDiff.x, -posDiff.z) - (u16)player->rotY) & 0x1FFF;
                    if ((angleDiff - 0x200) > 0x1C00) {
                        if (((u32)(posDiff.x + 0x4FFFFFF) < 0x9FFFFFFU) &&
                            ((u32)(posDiff.z + 0x4FFFFFF) < 0x9FFFFFFU)) {
                            dist = distance_2d(posDiff.x, posDiff.z);
                            if ((((player->primaryItemId == 3) | (player->primaryItemId == 5)) != 0) &&
                                (dist <= 0x9FFFFF)) {
                                dist = 0x05000000;
                            }
                            if (dist < maxRange) {
                                bestTarget = otherPlayer;
                                targetingMode = 0;
                            }
                        }
                    } else if ((player->primaryItemId == 5) && (angleDiff - 0xE01) < 0x3FF &&
                               ((u32)(posDiff.x + 0x4FFFFFF) < 0x9FFFFFFU) &&
                               ((u32)(posDiff.z + 0x4FFFFFF) < 0x9FFFFFFU)) {
                        dist = distance_2d(posDiff.x, posDiff.z);
                        if (dist < maxRange) {
                            bestTarget = otherPlayer;
                            targetingMode = 1;
                        }
                    }
                }
                i++;
            } while (i < gs->numPlayers);
        }
        player->aiPrimaryItemUseTimer = gAIPlayerParams[player->speedPenaltyIndex][player->primaryItemId].delay;
        player->aiPrimaryItemUseTimer -= (randA() * (gAIPlayerParams[player->speedPenaltyIndex][0].delay)) / 255;
        if (bestTarget == NULL) {
            return -1;
        }
        if (randA() >= gAIPlayerParams[player->speedPenaltyIndex][player->primaryItemId].useChance) {
            return -1;
        }
        if (bestTarget->isBossRacer == 0) {
            return targetingMode;
        }
        if (randA() >= gAIPlayerParams[player->speedPenaltyIndex][player->primaryItemId].altChance) {
            return -1;
        }
        i = player->finishPosition;
        if (i >= 0) {
            for (; i >= 0; i--) {
                if ((gs->PAD_6B_2[i] + (gs->players))->isBossRacer == 0) {
                    break;
                }
            }
            if (i >= 0) {
                return -1;
            }
        }
        return targetingMode;
    }
    player->aiPrimaryItemUseTimer--;
    return -1;
}

s32 shouldUseSecondaryItem(Player *player) {
    GameState *gs;

    gs = (GameState *)getCurrentAllocation();

    if (player->inputDisabled == 0) {
        if (player->secondaryItemId == 0) {
            return 0;
        }
        if (player->inputButtonsPressed & B_BUTTON) {
            return 1;
        }
    } else {
        if (player->secondaryItemId != 0) {
            if (player->aiItemUseTimer >
                gAIPlayerParams[player->speedPenaltyIndex][player->secondaryItemId + 6].delay) {
                player->aiItemUseTimer = gAIPlayerParams[player->speedPenaltyIndex][player->secondaryItemId + 6].delay;
                player->aiItemUseTimer -=
                    (randA() & 0xFF) * gAIPlayerParams[player->speedPenaltyIndex][0].altChance / 255;
            }

            if (player->aiItemUseTimer <= 0) {
                player->aiItemUseTimer = gAIPlayerParams[player->speedPenaltyIndex][player->secondaryItemId + 6].delay;
                player->aiItemUseTimer -=
                    (randA() & 0xFF) * gAIPlayerParams[player->speedPenaltyIndex][0].altChance / 255;

                switch (player->secondaryItemId) {
                    case 1:
                    case 2:
                    case 3:
                        if ((gs->players[gs->PAD_6B_2[0]].animFlags & 0x100)) {
                            return 0;
                        }
                        if (randA() <
                            gAIPlayerParams[player->speedPenaltyIndex][player->secondaryItemId + 6].useChance) {
                            return 1;
                        }
                        break;
                    case 6:
                        if (player->pathFlags != 0) {
                            if (randA() <
                                gAIPlayerParams[player->speedPenaltyIndex][player->secondaryItemId + 6].altChance) {
                                return 1;
                            }
                        } else {
                            if (randA() <
                                gAIPlayerParams[player->speedPenaltyIndex][player->secondaryItemId + 6].useChance) {
                                return 1;
                            }
                        }
                        break;
                    case 7:
                        if (player->finishPosition >= 3) {
                            return 0;
                        }
                        if (randA() <
                            gAIPlayerParams[player->speedPenaltyIndex][player->secondaryItemId + 6].useChance) {
                            return 1;
                        }
                        break;
                    case 4:
                    case 5:
                    case 8:
                    case 9:
                    case 10:
                        if (randA() <
                            gAIPlayerParams[player->speedPenaltyIndex][player->secondaryItemId + 6].useChance) {
                            return 1;
                        }
                        break;
                    default:
                        return 0;
                }
            } else {
                player->aiItemUseTimer--;
            }
        } else {
            player->aiItemUseTimer = 0x100;
        }
    }
    return 0;
}

void addPlayerRaceGold(Player *player, s32 amount) {
    if (player->unkBC6 != 0) {
        return;
    }

    player->raceGold += amount;

    if (player->raceGold > 99999) {
        player->raceGold = 99999;
    }

    if (player->raceGold < 0) {
        player->raceGold = 0;
    }
}

void addPlayerSkillPoints(Player *player, s32 amount) {
    if (player->unkBC6 != 0) {
        return;
    }
    player->skillPoints += amount;
    if (player->skillPoints >= 10000) {
        player->skillPoints = 9999;
    }
    if (player->skillPoints < 0) {
        player->skillPoints = 0;
    }
}

s32 getPlayerRaceGold(Player *player) {
    if (!player->unkBC6) {
        return player->raceGold;
    }
    return 0;
}

void playTrickLandingVoice(Player *player) {
    if (player->characterId < 9) {
        switch (randA() % 3) {
            case 0:
                queueSoundAtPositionWithPriority(
                    &player->worldPos,
                    D_80093F90_94B90[player->characterId],
                    5,
                    player->playerIndex + 4
                );
                break;
            case 1:
                queueSoundAtPositionWithPriority(
                    &player->worldPos,
                    D_80093FA4_94BA4[player->characterId],
                    5,
                    player->playerIndex + 4
                );
                break;
            case 2:
                queueSoundAtPositionWithPriority(
                    &player->worldPos,
                    D_80093FB8_94BB8[player->characterId],
                    5,
                    player->playerIndex + 4
                );
                break;
        }
    }
}

void playTrickSuccessVoice(Player *player) {
    u8 index = player->characterId;
    if (index < 9) {
        if (player->behaviorFlags == 0) {
            queueSoundAtPositionWithPriority(&player->worldPos, D_80093FCC_94BCC[index], 5, player->playerIndex + 4);
        }
    }
}

void playAttackHitVoice(Player *player) {
    u8 index = player->characterId;
    if (index < 9) {
        if (player->behaviorFlags == 0) {
            queueSoundAtPositionWithPriority(&player->worldPos, D_80093FE0_94BE0[index], 5, player->playerIndex + 4);
            setPlayerBodyPartAnimState(player, 1, 0xF);
        }
    }
}

void playFinishBoostVoice(Player *player) {
    u8 index = player->characterId;
    if (index < 9) {
        if (player->behaviorFlags == 0) {
            queueSoundAtPositionWithPriority(
                &player->worldPos,
                gFinishBoostVoiceSounds[index],
                5,
                player->playerIndex + 4
            );
        }
    }
}

void playRaceWinVoice(Player *player) {
    u8 index = player->characterId;
    if (index < 9) {
        if (player->behaviorFlags == 0) {
            queueSoundAtPositionWithPriority(&player->worldPos, D_80094008_94C08[index], 5, player->playerIndex + 4);
        }
    }
}

void playStunnedVoice(Player *arg0) {
    if (arg0->characterId < 9) {
        if (randA() & 1) {
            queueSoundAtPositionWithPriority(
                &arg0->worldPos,
                gStunnedVoiceSounds1[arg0->characterId],
                5,
                arg0->playerIndex + 4
            );
        } else {
            queueSoundAtPositionWithPriority(
                &arg0->worldPos,
                gStunnedVoiceSounds2[arg0->characterId],
                5,
                arg0->playerIndex + 4
            );
        }
    }
}

void playCharacterVoice1(Player *arg0) {
    u8 index = arg0->characterId;
    if (index < 9) {
        queueSoundAtPositionWithPriority(&arg0->worldPos, gCharacterVoiceSounds1[index], 5, arg0->playerIndex + 4);
    }
}

void playCharacterVoice2(Player *arg0) {
    u8 index = arg0->characterId;
    if (index < 9) {
        queueSoundAtPositionWithPriority(&arg0->worldPos, gCharacterVoiceSounds2[index], 5, arg0->playerIndex + 4);
    }
}

extern u16 gCharacterVoiceSounds3[];

void playCharacterVoice3(Player *arg0) {
    u8 index = arg0->characterId;
    if (index < 9) {
        queueSoundAtPositionWithPriority(&arg0->worldPos, gCharacterVoiceSounds3[index], 5, arg0->playerIndex + 4);
    }
}
