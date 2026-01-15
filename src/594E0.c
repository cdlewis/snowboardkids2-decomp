#include "42170.h"
#include "5DBC0.h"
#include "common.h"
#include "gamestate.h"
#include "graphics.h"
#include "race_session.h"
#include "rand.h"

extern u16 D_80093F90_94B90[];
extern u16 D_80093FA4_94BA4[];
extern u16 D_80093FB8_94BB8[];
extern u16 D_80093FCC_94BCC[];
extern u16 D_80093FE0_94BE0[];
extern u16 gFinishBoostVoiceSounds[];
extern u16 D_80094008_94C08[];
extern u16 D_8009401C_94C1C[];
extern u16 D_80094030_94C30[];
extern u16 D_80094044_94C44[];
extern u16 D_80094058_94C58[];

void setPlayerHitStunState(Player *player, s16 hitState, void *knockbackData) {
    if (player->unkAC2 != 0) {
        return;
    }
    if (player->unkB88 & ~0x80) {
        return;
    }
    player->unkAC2 = hitState;
    memcpy(&player->unkAC8, knockbackData, 0xC);
}

void setPlayerProjectileHitState(Player *player) {
    if (player->unkAC2 < 3) {
        if (!(player->unkB88 & 0x77F)) {
            player->unkAC2 = 3;
        }
    }
}

void setPlayerCollisionKnockbackState(Player *player, s16 knockbackAngle, s32 knockbackStrength) {
    if (player->unkAC2 < 4) {
        if ((player->unkB88 & 0x77F) == 0) {
            player->unkAC2 = 4;
            player->unkAC4 = knockbackAngle;
            player->unkAC8 = 0;
            player->unkACC = 0;
            if (knockbackStrength > 0x60000) {
                knockbackStrength = 0x60000;
            }
            player->unkAD0 = -knockbackStrength;
        }
    }
}

void setPlayerState50(Player *player) {
    if (player->unkAC2 < 0x32) {
        if (!(player->unkB88 & 0x71F)) {
            player->unkAC2 = 0x32;
        }
    }
}

void setPlayerPullState(Player *player, void *pullTarget) {
    if (player->unkAC2 < 0x31) {
        if (!(player->unkB88 & 0x71F)) {
            player->unkAC2 = 0x31;
            memcpy(&player->unkAC8, pullTarget, 0xC);
        }
    }
}

void setPlayerHomingProjectileRetaliationState(Player *player) {
    if (player->unkAC2 < 0x33) {
        if (!(player->unkB88 & 0x59F)) {
            player->unkAC2 = 0x33;
        }
    }
}

void setPlayerParachuteState(Player *player) {
    if (player->unkAC2 < 0x34) {
        if (!(player->unkB88 & 0x4BC)) {
            player->unkAC2 = 0x34;
        }
    }
}

void setPlayerShrinkState(Player *arg0) {
    if (arg0->unkAC2 < 0x35) {
        if (!(arg0->unkB88 & 0xD8)) {
            arg0->unkAC2 = 0x35;
        }
    }
}

void setPlayerPanelHitState(Player *arg0) {
    if (arg0->unkAC2 < 0x36) {
        if (!(arg0->unkB88 & 0x49C)) {
            arg0->unkAC2 = 0x36;
        }
    }
}

void setPlayerFrozenState(Player *player) {
    if (player->unkAC2 < 0x3C) {
        if (!(player->unkB88 & 0x9C)) {
            player->unkAC2 = 0x3C;
        }
    }
}

void setPlayerStarHitState(Player *player, Vec3i *hitPosition) {
    if (player->unkAC2 < 0x3C) {
        if (!(player->unkB88 & 0x218)) {
            player->unkAC2 = 0x3D;
            memcpy(&player->unkAC8, hitPosition, 0xC);
        }
    }
}

void setPlayerBouncedBackState(Player *player) {
    if (player->unkAC2 < 0x3C) {
        if (player->invincibilityTimer == 0) {
            if (!(player->unkB88 & 0x18)) {
                player->unkAC2 = 0x3E;
            }
        }
    }
}

void setPlayerState80(Player *player) {
    if (player->unkAC2 < 0x50) {
        if (!(player->unkB88 & 0x1C)) {
            player->unkAC2 = 0x50;
        }
    }
}

void setPlayerState100(Player *player) {
    if (!(player->unkB88 & 8)) {
        player->unkAC2 = 100;
    }
}

s32 tryActivateFinishBoost(Player *arg0) {
    if (arg0->boostState == 0) {
        if (spawnPlayerFlashEffect(arg0) != 0) {
            if (arg0->unkB84 & 2) {
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
            if (arg0->unkB84 & 2) {
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

INCLUDE_ASM("asm/nonmatchings/594E0", func_80058CFC_598FC);

INCLUDE_ASM("asm/nonmatchings/594E0", func_80059394_59F94);

INCLUDE_ASM("asm/nonmatchings/594E0", func_800597C0_5A3C0);

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
        if (player->unkB88 == 0) {
            queueSoundAtPositionWithPriority(&player->worldPos, D_80093FCC_94BCC[index], 5, player->playerIndex + 4);
        }
    }
}

void playAttackHitVoice(Player *player) {
    u8 index = player->characterId;
    if (index < 9) {
        if (player->unkB88 == 0) {
            queueSoundAtPositionWithPriority(&player->worldPos, D_80093FE0_94BE0[index], 5, player->playerIndex + 4);
            func_8005D804_5E404(player, 1, 0xF);
        }
    }
}

void playFinishBoostVoice(Player *player) {
    u8 index = player->characterId;
    if (index < 9) {
        if (player->unkB88 == 0) {
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
        if (player->unkB88 == 0) {
            queueSoundAtPositionWithPriority(&player->worldPos, D_80094008_94C08[index], 5, player->playerIndex + 4);
        }
    }
}

void func_80059D30_5A930(Player *arg0) {
    if (arg0->characterId < 9) {
        if (randA() & 1) {
            queueSoundAtPositionWithPriority(
                &arg0->worldPos,
                D_8009401C_94C1C[arg0->characterId],
                5,
                arg0->playerIndex + 4
            );
        } else {
            queueSoundAtPositionWithPriority(
                &arg0->worldPos,
                D_80094030_94C30[arg0->characterId],
                5,
                arg0->playerIndex + 4
            );
        }
    }
}

void func_80059DB8_5A9B8(Player *arg0) {
    u8 index = arg0->characterId;
    if (index < 9) {
        queueSoundAtPositionWithPriority(&arg0->worldPos, D_80094044_94C44[index], 5, arg0->playerIndex + 4);
    }
}

void func_80059DFC_5A9FC(Player *arg0) {
    u8 index = arg0->characterId;
    if (index < 9) {
        queueSoundAtPositionWithPriority(&arg0->worldPos, D_80094058_94C58[index], 5, arg0->playerIndex + 4);
    }
}

extern u16 D_8009406C_94C6C[];

void func_80059E40_5AA40(Player *arg0) {
    u8 index = arg0->characterId;
    if (index < 9) {
        queueSoundAtPositionWithPriority(&arg0->worldPos, D_8009406C_94C6C[index], 5, arg0->playerIndex + 4);
    }
}
