#include "race/track_height.h"
#include "animation/bone_animation.h"
#include "common.h"
#include "data/asset_metadata.h"
#include "gamestate.h"
#include "graphics/displaylist.h"
#include "math/geometry.h"
#include "math/rand.h"
#include "system/task_scheduler.h"

u8 gCharacterBodyPartAnimTable[] = {
    0x00, 0x01, 0x03, 0x05, 0x06, 0x00, 0x01, 0x03, 0x05, 0x04, 0x00, 0x01, 0x03, 0x05, 0x03,
    0x00, 0x01, 0x03, 0x05, 0x03, 0x00, 0x05, 0x01, 0x03, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

u8 gBodyPartRemapTable[] = {
    0x00, 0x04, 0x05, 0x06, 0x01, 0x02, 0x03, 0x07,
};

s8 gSpecialTrickTypeTable[] = {
    0x08, 0x09, 0x0D, 0x0E, 0x0F, 0x0A, 0x0B, 0x0C, -1, -1, -1,   -1,   -1,   -1,   -1,   -1,   0x00, -1,
    -1,   -1,   -1,   -1,   -1,   -1,   0x02, 0x02, -1, -1, -1,   -1,   -1,   -1,   0x01, 0x07, -1,   -1,
    -1,   -1,   -1,   -1,   0x01, 0x01, 0x01, -1,   -1, -1, -1,   -1,   0x06, 0x02, 0x04, -1,   -1,   -1,
    -1,   -1,   0x00, 0x05, 0x03, 0x07, 0x01, -1,   -1, -1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

s32 getTrackHeightInSector(void *gameData, s32 sectorIndex, void *pos, s32 heightOffset) {
    return getTrackHeightAtPosition(gameData, sectorIndex, pos);
}

s32 getTrackHeightInSectorWithOffset(void *gameData, u16 sectorIndex, void *pos, s32 unused, s32 heightOffset) {
    return getTrackHeightWithNormalAtPosition(gameData, sectorIndex, pos, heightOffset);
}

void findTrackFaceInSector(void *geom, u16 sectorIndex, void *pos, u8 *surfaceFlags, u8 *surfaceIndex) {
    findTrackFaceAtPosition(geom, sectorIndex, pos, surfaceFlags, surfaceIndex);
}

s32 getSectorTrackHeight(TrackGeometryFaceData *geom, u16 groupIdx, Vec3i *pos, s32 yOffset) {
    return computeSectorTrackHeight(geom, groupIdx, pos, yOffset);
}

void clampPlayerVelocityToMaxSpeed(Player *player) {
    s32 velX;
    s32 velY;
    s32 velZ;
    s64 sum;
    s32 magnitude;
    s32 maxMagnitude;

    velX = player->velocity.x;
    velY = player->velocity.y;
    velZ = player->velocity.z;

    sum = (s64)velX * velX + (s64)velY * velY + (s64)velZ * velZ;
    magnitude = isqrt64(sum);

    maxMagnitude = player->maxSpeedCap;
    if (maxMagnitude < magnitude) {
        player->velocity.x = (s32)(((s64)player->velocity.x * player->maxSpeedCap) / magnitude);
        player->velocity.y = (s32)(((s64)player->velocity.y * player->maxSpeedCap) / magnitude);
        player->velocity.z = (s32)(((s64)player->velocity.z * player->maxSpeedCap) / magnitude);
    }
}

s32 advancePlayerLeanAnimationAuto(void *arg0_, s16 arg1) {
    Player *arg0 = arg0_;
    s32 result;
    s32 flags;
    s32 i;
    s16 animIndex;

    flags = arg0->animFlags;
    result = 0;
    animIndex = arg1;

    if (flags & 2) {
        if (!(flags & 4)) {
            arg0->leanAnimIndex = 0xFFFF;
            arg0->animFlags = arg0->animFlags | 4;
        }
    } else if (flags & 4) {
        arg0->leanAnimIndex = 0xFFFF;
        arg0->animFlags = arg0->animFlags & ~4;
    }

    if (arg0->leanAnimIndex != animIndex) {
        arg0->leanAnimIndex = animIndex;
        arg0->leanBoneCount = getAnimationBoneCount(arg0->unk0, animIndex);
        for (i = 0; i < arg0->leanBoneCount; i++) {
            resetBoneAnimation(arg0->unk0, (s16)arg0->leanAnimIndex, (s16)i, &arg0->unk488[i]);
        }
    }

    if (arg0->animFlags & 4) {
        for (i = 0; i < arg0->leanBoneCount; i++) {
            result |=
                advanceIndexedBoneAnimationAutoMirrored(arg0->unk0, (s16)arg0->leanAnimIndex, (s16)i, &arg0->unk488[i]);
        }
    } else {
        for (i = 0; i < arg0->leanBoneCount; i++) {
            result |= advanceIndexedBoneAnimationAuto(arg0->unk0, (s16)arg0->leanAnimIndex, (s16)i, &arg0->unk488[i]);
        }
    }

    return result;
}

s32 advancePlayerLeanAnimation(void *arg0_, s16 arg1) {
    Player *arg0 = arg0_;
    s32 result;
    s32 flags;
    s32 i;

    flags = arg0->animFlags;

    if (flags & 2) {
        if (!(flags & 4)) {
            arg0->leanAnimIndex = 0xFFFF;
            arg0->animFlags = arg0->animFlags | 4;
        }
    } else if (flags & 4) {
        arg0->leanAnimIndex = 0xFFFF;
        arg0->animFlags = arg0->animFlags & ~4;
    }

    if (arg0->leanAnimIndex != arg1) {
        arg0->leanAnimIndex = arg1;
        arg0->leanBoneCount = getAnimationBoneCount(arg0->unk0, arg1);
        for (i = 0; i < arg0->leanBoneCount; i++) {
            resetBoneAnimation(arg0->unk0, arg0->leanAnimIndex, (s16)i, &arg0->unk488[i]);
        }
    }

    if (arg0->animFlags & 4) {
        for (i = 0; i < arg0->leanBoneCount; i++) {
            result = advanceIndexedBoneAnimationMirrored(arg0->unk0, arg0->leanAnimIndex, (s16)i, &arg0->unk488[i]);
        }
    } else {
        for (i = 0; i < arg0->leanBoneCount; i++) {
            result = advanceIndexedBoneAnimation(arg0->unk0, arg0->leanAnimIndex, (s16)i, &arg0->unk488[i]);
        }
    }

    return result;
}

void setPlayerLeanAnimation(Player *player, s32 animIndex, s32 progress) {
    s32 i;

    if (player->animFlags & 2) {
        if (!(player->animFlags & 4)) {
            player->leanAnimIndex = 0xFFFF;
            player->animFlags = player->animFlags | 4;
        }
    } else if (player->animFlags & 4) {
        player->leanAnimIndex = 0xFFFF;
        player->animFlags = player->animFlags & ~4;
    }

    if (player->leanAnimIndex != (s16)animIndex) {
        player->leanAnimIndex = animIndex;
        i = (s16)animIndex;
        player->leanBoneCount = getAnimationBoneCount(player->unk0, i);
        for (i = 0; i < player->leanBoneCount; i++) {
            resetBoneAnimation(player->unk0, (s16)player->leanAnimIndex, (s16)i, &player->unk488[i]);
        }
    }

    if (player->animFlags & 4) {
        for (i = 0; i < player->leanBoneCount; i++) {
            interpolateIndexedBoneAnimationMirrored(
                player->unk0,
                (s16)player->leanAnimIndex,
                (s16)i,
                &player->unk488[i],
                progress & 0xFFFF
            );
        }
    } else {
        for (i = 0; i < player->leanBoneCount; i++) {
            interpolateIndexedBoneAnimation(
                player->unk0,
                (s16)player->leanAnimIndex,
                (s16)i,
                &player->unk488[i],
                (s16)progress
            );
        }
    }
}

/* Load character body part display lists into the bone results array.
 * Iterates 16 body parts (0x3C stride), writing the display list pointer at
 * offset 0x58 (offsetof(boneResults) + offsetof(BoneResult, displayList)).
 * For race type 0xB, uses loadAssetByIndex_95380; otherwise uses loadAssetByIndex_953B0.
 */
void loadCharacterBodyParts(Player *player) {
    s32 flags;
    s32 partIndex;
    u8 tableValue;
    BoneResult *bodyPart;
    BoneResult *bodyPart2;
    u8 *lookupTable;

    if (((GameState *)getCurrentAllocation())->raceType == 0xB) {
        flags = player->animFlags;
        if (flags & 4) {
            player->animFlags = flags | 8;
            partIndex = 0;
            bodyPart = (BoneResult *)player;
            do {
                *(s32 *)((u8 *)bodyPart + 0x58) = (s32)loadAssetByIndex_95380(player->characterId, player->boardIndex) +
                                                  gBodyPartRemapTable[partIndex] * 0x10;
                partIndex++;
                bodyPart++;
            } while (partIndex < 0x10);
        } else {
            player->animFlags = flags & ~8;
            partIndex = 0;
            bodyPart = (BoneResult *)player;
            do {
                *(s32 *)((u8 *)bodyPart + 0x58) =
                    (s32)loadAssetByIndex_95380(player->characterId, player->boardIndex) + partIndex * 0x10;
                bodyPart++;
            } while (++partIndex < 0x10);
        }
    } else {
        flags = player->animFlags;
        if (flags & 4) {
            player->animFlags = flags | 8;
            partIndex = 0;
            bodyPart2 = (BoneResult *)player;
            do {
                *(s32 *)((u8 *)bodyPart2 + 0x58) = loadAssetByIndex_953B0(player->characterId, player->boardIndex) +
                                                   gBodyPartRemapTable[partIndex] * 0x10;
                partIndex++;
                bodyPart2++;
            } while (partIndex < 0x10);
        } else {
            player->animFlags = flags & ~8;
            partIndex = 0;
            bodyPart2 = (BoneResult *)player;
            do {
                *(s32 *)((u8 *)bodyPart2 + 0x58) =
                    loadAssetByIndex_953B0(player->characterId, player->boardIndex) + partIndex * 0x10;
                bodyPart2++;
            } while (++partIndex < 0x10);
        }
    }

    lookupTable = gCharacterBodyPartAnimTable + player->characterId * 5;
    tableValue = lookupTable[player->bodyPartAnimIndex];
    if (tableValue == 0) {
        goto copy_basic;
    }
    flags = (s32)player->unk20;
    if (flags == 0) {
    copy_basic:
        player->unk23C = (s32)player->unk4;
        player->unk240 = (s32)player->unk8;
        return;
    }
    player->unk23C = flags;
    player->unk240 = (s32)player->unk24;
    player->unk238 = (s32)getAssetDataDirect(player->characterId, player->boardIndex) + tableValue * 0x10 - 0x10;
}

void setPlayerBodyPartAnimState(Player *player, u8 animIndex, u8 animFlags) {
    player->bodyPartAnimIndex = animIndex;
    player->bodyPartAnimFlags = animFlags;
}

s16 onTrickCompletedHook(Player *player) {
    (void)player;
}

s16 getTrickType(Player *player) {
    s16 trickType;
    s8 stickX;
    s8 stickY;
    u16 stickCombo;
    s8 *tablePtr;
    s8 trickCount;

    trickType = -1;
    if (player->inputDisabled != 0) {
        stickY = player->unkBDC & 7;
        if (stickY != 0) {
            tablePtr = gSpecialTrickTypeTable;
            trickCount = player->trickCount;
            return *(tablePtr + ((s32)stickY << 3) + trickCount);
        }
        goto return_trick;
    }
    stickY = player->inputStickY;
    trickType = -(stickY != 7);
    if (stickY == -7) {
        trickType = 1;
    }
    stickX = player->inputStickX;
    if (stickX == 7) {
        trickType = 2;
    }
    if (stickX == -7) {
        trickType = 3;
    }
    stickCombo = *(u16 *)&player->inputStickX;
    if (stickCombo == 0x707) {
        trickType = 4;
    }
    if (stickCombo == 0xF907) {
        trickType = 5;
    }
    if (stickCombo == 0x7F9) {
        trickType = 6;
    }
    if (stickCombo == 0xF9F9) {
        trickType = 7;
    }
return_trick:
    return trickType;
}

s32 getTrickRecoveryTimerReduction(Player *player) {
    s16 count;
    u32 temp;
    s8 temp_b7a;
    s8 temp_b7b;
    s16 temp_b7e;

    count = 1;

    if (player->inputDisabled != 0) {
        temp = randA();
        temp = temp & 3;
        temp = temp < 1;
        count = temp + 1;
        goto end;
    }

    temp_b7a = player->inputStickX;
    if (temp_b7a == 7) {
        temp = player->prevInputStickX;
        temp = temp ^ 7;
        temp = temp != 0;
        count = temp + 1;
    }

    if (temp_b7a == -7) {
        temp = player->prevInputStickX;
        temp = temp ^ temp_b7a;
        temp = temp != 0;
        count += temp;
    }

    temp_b7b = player->inputStickY;
    if (temp_b7b == 7) {
        temp = player->prevInputStickY;
        temp = temp ^ 7;
        temp = temp != 0;
        count += temp;
    }

    if (temp_b7b == -7) {
        temp = player->prevInputStickY;
        temp = temp ^ temp_b7b;
        temp = temp != 0;
        count += temp;
    }

    temp_b7e = player->inputButtonsPressed;
    temp = temp_b7e & 0xC000;
    temp = temp != 0;
    count += temp;

end:
    return count;
}
