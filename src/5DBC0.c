#include "5DBC0.h"
#include "5E590.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "rand.h"
#include "task_scheduler.h"

s32 getTrackHeightInSector(void *gameData, s32 sectorIndex, void *pos, s32 heightOffset) {
    return func_80061A64_62664(gameData, sectorIndex, pos);
}

s32 getTrackHeightInSectorWithOffset(void *gameData, u16 sectorIndex, void *pos, s32 unused, s32 heightOffset) {
    return func_80061D6C_6296C(gameData, sectorIndex, pos, heightOffset);
}

void func_8005CFFC_5DBFC(void *arg0, u16 arg1, void *arg2, void *arg3, void *arg4) {
    findTrackFaceAtPosition(arg0, arg1, arg2, arg3, arg4);
}

s32 getSectorTrackHeight(TrackGeometryFaceData *geom, u16 groupIdx, Vec3i *pos, s32 yOffset) {
    return func_80062274_62E74(geom, groupIdx, pos, yOffset);
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

INCLUDE_ASM("asm/nonmatchings/5DBC0", func_8005D180_5DD80);

INCLUDE_ASM("asm/nonmatchings/5DBC0", func_8005D308_5DF08);

INCLUDE_ASM("asm/nonmatchings/5DBC0", func_8005D48C_5E08C);

extern u8 gCharacterBodyPartAnimTable[];
extern u8 gBodyPartRemapTable[];

void loadCharacterBodyParts(Player *player) {
    s32 flags;
    s32 partIndex;
    u8 tableValue;
    BodyPart *bodyPart;
    BodyPart *bodyPart2;
    u8 *lookupTable;

    if (((GameState *)getCurrentAllocation())->raceType == 0xB) {
        flags = player->unkB84;
        if (flags & 4) {
            player->unkB84 = flags | 8;
            partIndex = 0;
            bodyPart = (BodyPart *)player;
            do {
                *(s32 *)((u8 *)bodyPart + 0x58) = (s32)loadAssetByIndex_95380(player->characterId, player->boardIndex) +
                                                  gBodyPartRemapTable[partIndex] * 0x10;
                partIndex++;
                bodyPart++;
            } while (partIndex < 0x10);
        } else {
            player->unkB84 = flags & ~8;
            partIndex = 0;
            bodyPart = (BodyPart *)player;
            do {
                *(s32 *)((u8 *)bodyPart + 0x58) =
                    (s32)loadAssetByIndex_95380(player->characterId, player->boardIndex) + partIndex * 0x10;
                bodyPart++;
            } while (++partIndex < 0x10);
        }
    } else {
        flags = player->unkB84;
        if (flags & 4) {
            player->unkB84 = flags | 8;
            partIndex = 0;
            bodyPart2 = (BodyPart *)player;
            do {
                *(s32 *)((u8 *)bodyPart2 + 0x58) = loadAssetByIndex_953B0(player->characterId, player->boardIndex) +
                                                   gBodyPartRemapTable[partIndex] * 0x10;
                partIndex++;
                bodyPart2++;
            } while (partIndex < 0x10);
        } else {
            player->unkB84 = flags & ~8;
            partIndex = 0;
            bodyPart2 = (BodyPart *)player;
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

extern s8 gSpecialTrickTypeTable[];

s16 getTrickType(Player *player) {
    s16 trickType;
    s8 stickX;
    s8 stickY;
    u16 stickCombo;
    s8 *tablePtr;
    s8 trickCount;

    trickType = -1;
    if (player->unkBDA != 0) {
        stickY = player->unkBDC & 7;
        if (stickY != 0) {
            tablePtr = gSpecialTrickTypeTable;
            trickCount = player->trickCount;
            return *(tablePtr + ((s32)stickY << 3) + trickCount);
        }
        goto return_trick;
    }
    stickY = player->unkB7B;
    trickType = -(stickY != 7);
    if (stickY == -7) {
        trickType = 1;
    }
    stickX = player->unkB7A;
    if (stickX == 7) {
        trickType = 2;
    }
    if (stickX == -7) {
        trickType = 3;
    }
    stickCombo = *(u16 *)&player->unkB7A;
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

    if (player->unkBDA != 0) {
        temp = randA();
        temp = temp & 3;
        temp = temp < 1;
        count = temp + 1;
        goto end;
    }

    temp_b7a = player->unkB7A;
    if (temp_b7a == 7) {
        temp = player->unkB80;
        temp = temp ^ 7;
        temp = temp != 0;
        count = temp + 1;
    }

    if (temp_b7a == -7) {
        temp = player->unkB80;
        temp = temp ^ temp_b7a;
        temp = temp != 0;
        count += temp;
    }

    temp_b7b = player->unkB7B;
    if (temp_b7b == 7) {
        temp = player->unkB81;
        temp = temp ^ 7;
        temp = temp != 0;
        count += temp;
    }

    if (temp_b7b == -7) {
        temp = player->unkB81;
        temp = temp ^ temp_b7b;
        temp = temp != 0;
        count += temp;
    }

    temp_b7e = player->unkB7E;
    temp = temp_b7e & 0xC000;
    temp = temp != 0;
    count += temp;

end:
    return count;
}
