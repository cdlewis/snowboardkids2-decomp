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
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
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
    s32 unkB88;
    u8 _padB8C[0xB94 - 0xB8C];
    u16 sectorIndex;
    u8 _padB96[0xBB4 - 0xB96];
    u8 unkBB4;
    u8 _padBB5[0xBB7 - 0xBB5];
    u8 unkBB7;
    u8 _padBB8[0xBB9 - 0xBB8];
    u8 unkBB9;
    u8 unkBBA;
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
} func_800BC4AC_arg;

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
    func_8005F2FC_5FEFC_arg unk488[12];
    u8 padding888[0x4EC - 0x4 - 0x48 * 12];
    Transform3D unk970;
    Transform3D unk990;
    Transform3D unk9B0;
    u8 paddingA88[0xA8C - 0x9D0];
    u16 unkA8C;
    s16 unkA8E;
    s16 unkA90;
    s16 unkA92;
    s16 unkA94;
    s16 unkA96;
    s16 unkA98;
    s16 unkA9A;
    u8 padding9E[0x2];
    s16 unkA9E;
    s32 unkAA0;
    s32 unkAA4;
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
    s32 unkB84;
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
    u8 unkBB7;
    u8 unkBB8;
    u8 unkBB9;
    u8 unkBBA;
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
} func_800BB66C_B0B5C_arg;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unk7;
} D_800BACC8_AAB78_type;

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
    u16 unkA94;
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
extern StateFunc D_800BCA14_B1F04[];
extern s16 D_800BC9F0_B1EE0[];
extern s16 D_800BCA24_B1F14[];
extern s16 D_800BCA30_B1F20[];
extern FuncPtr D_800BCA44_B1F34[];
extern FuncPtr D_800BCA5C_B1F4C[];
extern u16 D_8009ADE0_9B9E0;
extern D_800BACC8_AAB78_type D_800BACC8_AAB78[];
extern s32 identityMatrix[];
extern s32 D_800BCA50_B1F40;
extern s32 gControllerInputs[];
extern s32 D_800BC054_B1544;
extern s32 D_800BC05C_B154C;
extern s32 D_800BCA3C_B1F2C[];

void func_800BB2B0_B07A0(IceBossArg *arg0) {
    Transform3D sp10;
    Transform3D sp30;
    GameState *alloc;
    IceBossArg *player;
    s32 dist;
    s32 diff;

    alloc = getCurrentAllocation();

    arg0->velocity.x = arg0->position.x - arg0->prevPosition.x;
    arg0->velocity.y = arg0->position.y - arg0->prevPosition.y;
    arg0->velocity.z = arg0->position.z - arg0->prevPosition.z;
    memcpy(&arg0->prevPosition, &arg0->position, 0xC);

    player = (IceBossArg *)alloc->players;
    dist = distance_3d(
        arg0->position.x - player->position.x,
        arg0->position.y - player->position.y,
        arg0->position.z - player->position.z
    );

    if ((arg0->finishPosition == 0) & (dist > 0xE00000)) {
        if (arg0->bossFlags & 0x400000) {
            arg0->targetSpeed = getCharacterBoardStatParam0(0, 4) + -0x8000;
        } else if (dist > 0x8C00000) {
            arg0->targetSpeed = 0x70000;
        } else {
            arg0->targetSpeed = getCharacterBoardStatParam0(0, 0) + -0x8000;
        }
    } else {
        arg0->targetSpeed = getCharacterBoardStatParam0(0, 8) + 0x18000;
    }

    if (arg0->targetSpeed > 0x180000) {
        arg0->targetSpeed = 0x180000;
    }

    diff = arg0->targetSpeed - arg0->currentSpeed;
    if (diff >= 0x1001) {
        diff = 0x1000;
    }
    if (diff < -0x80) {
        diff = -0x80;
    }

    arg0->currentSpeed = arg0->currentSpeed + diff;
    arg0->bossFlags &= 0xFFFBFFFF;

    if (arg0->behaviorMode != 3) {
        if (arg0->behaviorTrigger != 0) {
            if (arg0->behaviorTrigger == 0x3D) {
                arg0->behaviorMode = 2;
                arg0->behaviorPhase = 0;
                arg0->behaviorStep = 0;
                arg0->behaviorCounter = 0;
                if (arg0->bossFlags & 0x400000) {
                    arg0->behaviorPhase = 1;
                }
            }
        }
    }
    arg0->behaviorTrigger = 0;

    do {
    } while (D_800BCA14_B1F04[arg0->behaviorMode](arg0) != 0);

    createZRotationMatrix(&arg0->zRotationMatrix, arg0->unkA92);
    createCombinedRotationMatrix(&arg0->combinedRotationMatrix, arg0->unkA8E, arg0->unkA90);
    createYRotationMatrix(&arg0->yRotationMatrix, arg0->unkA94);

    func_8006B084_6BC84(&arg0->zRotationMatrix, &arg0->combinedRotationMatrix, &sp10);
    func_8006B084_6BC84(&sp10, &arg0->yRotationMatrix, &sp30);

    sp30.translation.x -= arg0->yRotationMatrix.translation.x;
    sp30.translation.y -= arg0->yRotationMatrix.translation.y;
    sp30.translation.z -= arg0->yRotationMatrix.translation.z;

    if (arg0->bossFlags & 0x400000) {
        transformVector(D_800BCA30_B1F20, (s16 *)&sp30, &arg0->transformedPos);
    } else {
        transformVector(D_800BCA24_B1F14, (s16 *)&sp30, &arg0->transformedPos);
    }
    memcpy(&arg0->sectorListNode.localPos, &arg0->transformedPos, 0xC);
    func_8005C838_5D438(&arg0->sectorListNode);
    func_800BC61C_B1B0C((Player *)arg0);

    if (arg0->bossFlags & 0x400000) {
        transformVector(D_800BCA30_B1F20, (s16 *)&sp30, &arg0->unkAE4);
    } else {
        transformVector(D_800BC9F0_B1EE0, arg0->groundJointOffsets, &arg0->unkAE4);
        arg0->unkAE4.x -= arg0->yRotationMatrix.translation.x;
        arg0->unkAE4.y -= arg0->yRotationMatrix.translation.y;
        arg0->unkAE4.z -= arg0->yRotationMatrix.translation.z;

        transformVector(D_800BC9F0_B1EE0 + 6, arg0->groundJointOffsets2, &arg0->unkAF0);
        arg0->unkAF0.x -= arg0->yRotationMatrix.translation.x;
        arg0->unkAF0.y -= arg0->yRotationMatrix.translation.y;
        arg0->unkAF0.z -= arg0->yRotationMatrix.translation.z;

        transformVector(D_800BC9F0_B1EE0 + 12, arg0->groundJointOffsets3, &arg0->unkAFC);
        arg0->unkAFC.x -= arg0->yRotationMatrix.translation.x;
        arg0->unkAFC.y -= arg0->yRotationMatrix.translation.y;
        arg0->unkAFC.z -= arg0->yRotationMatrix.translation.z;
    }
}

s32 func_800BB66C_B0B5C(func_800BB66C_B0B5C_arg *arg0) {
    Vec3i sp10;
    Vec3i sp20;
    GameState *state;
    s32 i;
    u16 trackIdx;
    s32 temp;

    state = getCurrentAllocation();
    memcpy(&arg0->unk970, identityMatrix, sizeof(Transform3D));
    createYRotationMatrix(&arg0->unk970, arg0->unkA94);
    memcpy(&arg0->unk990, identityMatrix, sizeof(Transform3D));
    memcpy(&arg0->unk9B0, identityMatrix, sizeof(Transform3D));

    arg0->unk434 = D_800BCA3C_B1F2C[arg0->unkBB8];
    getTrackSegmentWaypoints(&state->gameData, 0, &sp10, &sp20);
    arg0->unk43C = sp10.z + 0x200000;
    trackIdx = func_80059E90_5AA90(arg0, &state->gameData, 0, (Vec3i *)&arg0->unk434);
    arg0->sectorIndex = trackIdx;
    arg0->unk438 = func_8005CFC0_5DBC0(&state->gameData, trackIdx, (Vec3i *)&arg0->unk434, 0x100000);
    memcpy(&arg0->unk440, &arg0->unk434, sizeof(Vec3i));
    arg0->velocity.x = 0;
    arg0->velocity.y = 0;
    arg0->velocity.z = 0;
    arg0->unkA94 = 0x1000;

    for (i = 0; i < 12; i++) {
        Unk0x3CElemExtra *extra = (Unk0x3CElemExtra *)((u8 *)&arg0->unk0_3C[i] + 0x38);
        memcpy(&extra->unk38, identityMatrix, sizeof(Transform3D));
        extra->unk5C = (s32)arg0->unk0_3C[0].unk4;
        extra->unk60 = (s32)arg0->unk0_3C[0].unk8;
        temp = i * 0x10;
        extra->unk64 = 0;
        extra->unk58 = (void *)(loadAssetByIndex_953B0(arg0->unkBB9, arg0->unkBBA) + temp);
    }

    arg0->unkA8C = 0;
    arg0->unkBB7 = func_8006097C_6157C(arg0->unk0_3C[0].unk0, 0);
    for (i = 0; i < arg0->unkBB7; i++) {
        func_8005F2FC_5FEFC(arg0->unk0_3C[0].unk0, arg0->unkA8C, i, &arg0->unk488[i]);
    }

    arg0->behaviorMode = 1;
    arg0->unkB2C = 0x240000;
    arg0->unkBB4 = 3;
    arg0->unkB54 = (void *)&arg0->unk434;
    arg0->behaviorPhase = 0;
    arg0->unkB30 = 0x11C000;
    arg0->unkB34 = 0x11C000;
    arg0->unkB64 = 0x150000;
    arg0->unkB68 = arg0->unkBB8;
    if (arg0->unkBC7 == 0) {
        spawnChaseCameraTask(arg0->unkBB8);
    }

    arg0->unkAA0 = ((s32 *)state->players)[0xAA0 / 4] - 0x10000;

    if (arg0->unk0_3C[0].unk1C != 0) {
        arg0->unk0_3C[0].unk28 = arg0->unk0_3C[0].unk1C + ((s32 *)arg0->unk0_3C[0].unk1C)[arg0->unkBB8];
    }

    arg0->unkBDB = 0xA;

    return 1;
}

void func_800BB8E0_B0DD0(func_800BC4AC_arg *arg0) {
    D_800BCA44_B1F34[arg0->behaviorPhase](arg0);
}

void func_800BB910_B0E00(Player *arg0) {
    s32 i;
    Element0x3C *elements = (Element0x3C *)arg0;

    for (i = 0; i < 12; i++) {
        *(void **)((u8 *)elements + i * 0x3C + 0x58) =
            (void *)(loadAssetByIndex_953B0(arg0->unkBB9, arg0->unkBBA) + i * 16 + 0xC0);
    }

    arg0->unkAE0 = 0x100000;
    arg0->unkB2C = 0x100000;
    arg0->unkBB4 = 1;
    arg0->unkB84 = arg0->unkB84 | 0x400000;
}

s32 func_800BB998_B0E88(func_800BC4AC_arg *arg0) {
    s32 pad[3];
    GameState *gameState = getCurrentAllocation();

    if (gameState->unk79 == 0) {
        setPlayerBehaviorPhase((Player *)arg0, 1);
        return 1;
    }

    arg0->velocity.x -= arg0->velocity.x / 8;
    arg0->velocity.z -= arg0->velocity.z / 8;
    arg0->velocity.y += -0x8000;
    applyClampedVelocityToPosition((Player *)arg0);

    if (arg0->bossFlags & 0x400000) {
        func_8005D180_5DD80(arg0, 2);
    } else {
        func_8005D180_5DD80(arg0, 0);
    }

    return 0;
}

s32 func_800BBA54_B0F44(Player *arg0) {
    Transform3D sp10;
    Vec3i sp30;
    Vec3i sp40;
    GameState *gameState;
    s16 angleDiff;
    Player *player;
    s32 v1, v0;

    gameState = (GameState *)getCurrentAllocation();

    if (arg0->unkB84 & 0x100000) {
        setPlayerBehaviorMode(arg0, 3);
        return 1;
    }

    if (arg0->unkB84 & 0x80000) {
        setPlayerBehaviorPhase(arg0, 2);
        return 1;
    }

    if (gControllerInputs[1] & L_TRIG) {
        func_800BB910_B0E00(arg0);
    }

    if (arg0->behaviorStep == 0) {
        arg0->behaviorStep++;

        if (gameState->unk50 < 0x1EU) {
            arg0->unkB8C = ((randA() & 0xFF) >> 2) + 0x5A;
        } else if (arg0->unkB84 & 0x400000) {
            arg0->unkB8C = randA() & 0xF;
        } else {
            arg0->unkB8C = (randA() & 0xFF) >> 1;
        }
        arg0->unkB90 = 0;
    }

    arg0->unkB84 = arg0->unkB84 | 0x40000;
    func_800B9B90_A9A40(arg0);

    angleDiff = func_8006D21C_6DE1C(arg0->unkA7C, arg0->unkA84, arg0->worldPos.x, arg0->worldPos.z) - arg0->unkA94;
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

    arg0->unkA94 = arg0->unkA94 + angleDiff;

    if (!(arg0->unkB84 & 0x1)) {
        createYRotationMatrix(&arg0->unk970, arg0->unkA94);
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

    if (arg0->unkB84 & 0x400000) {
        if (func_8005D180_5DD80(arg0, 3) != 0) {
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
        if (func_8005D180_5DD80(arg0, 1) != 0) {
            arg0->unkB90 = 0;
        }

        arg0->unkB90 = arg0->unkB90 + 1;

        if ((arg0->unkB90 == 4) || (arg0->unkB90 == 0xC)) {
            queueSoundAtPosition(&arg0->worldPos, 0x4A);
        }

        player = gameState->players;
        if (player->unkB88 != 0) {
            return 0;
        }

        if (arg0->unkB8C == 0) {
            if (arg0->behaviorCounter != 0) {
                if (arg0->behaviorStep == 2) {
                    arg0->behaviorCounter = arg0->behaviorCounter - 1;
                    if (spawnPlayerGuidedStarProjectile(arg0->playerIndex) != 0) {
                        func_8005182C_5242C(arg0);
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
                        func_8005182C_5242C(arg0);
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
                        func_8005182C_5242C(arg0);
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
                        func_8005182C_5242C(arg0);
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

s32 func_800BC008_B14F8(func_800BC4AC_arg *arg0) {
    s32 pad[3];

    getCurrentAllocation();

    arg0->velocity.x -= arg0->velocity.x / 8;
    arg0->velocity.z -= arg0->velocity.z / 8;
    arg0->velocity.y += -0x8000;
    applyClampedVelocityToPosition((Player *)arg0);

    if (arg0->bossFlags & 0x400000) {
        func_8005D180_5DD80(arg0, 2);
    } else {
        func_8005D180_5DD80(arg0, 0);
    }

    return 0;
}

void func_800BC0A8_B1598(func_800BC4AC_arg *arg0) {
    D_800BCA5C_B1F4C[arg0->behaviorPhase](arg0);
}

s32 func_800BC0D8_B15C8(Player *arg0) {
    Transform3D sp10;
    Vec3i sp30;
    s32 sp3C[4];
    Transform3D *temp_s0;
    Vec3i *temp_s1;
    s16 angleDiff;
    s32 i;

    getCurrentAllocation();

    if (arg0->behaviorStep == 0) {
        arg0->unkA8C = 0xFFFF;
        arg0->unkB8C = 0;
        arg0->behaviorStep += 1;

        if (!(arg0->unkB84 & 0x80000)) {
            if (arg0->unkBDB == 0) {
                queueSoundAtPosition(&arg0->worldPos, 0x4C);
            } else {
                arg0->unkBDB -= 1;
                if (arg0->unkBDB == 0) {
                    queueSoundAtPosition(&arg0->worldPos, 0x4C);
                }
            }
        }
    }

    arg0->unkB8C += 1;

    if ((arg0->unkB8C == 5) || (arg0->unkB8C == 0xF)) {
        spawnBossHomingProjectileVariant1Task(arg0);
        spawnBossHomingProjectileVariant1Task(arg0);
        spawnBossHomingProjectileVariant1Task(arg0);
        spawnBossHomingProjectileVariant1Task(arg0);
    }

    arg0->unkB88 = 0x200;
    func_800B9B90_A9A40(arg0);

    angleDiff =
        (func_8006D21C_6DE1C(arg0->unkA7C, arg0->unkA84, arg0->worldPos.x, arg0->worldPos.z) - arg0->unkA94) & 0x1FFF;

    if (angleDiff >= 0x1001) {
        angleDiff = angleDiff | 0xE000;
    }

    if (angleDiff >= 0x39) {
        angleDiff = 0x38;
    }

    if (angleDiff < -0x38) {
        angleDiff = -0x38;
    }

    arg0->unkA94 = arg0->unkA94 + angleDiff;

    if (!(arg0->unkB84 & 1)) {
        temp_s0 = &arg0->unk970;
        createYRotationMatrix(temp_s0, arg0->unkA94);
        func_8006BDBC_6C9BC((BoneAnimationState *)&arg0->unk990, temp_s0, &sp10);
        temp_s1 = &arg0->velocity;
        transformVector3(temp_s1, &sp10, &sp30);
        sp30.x = 0;
        transformVector2(&sp30, &sp10, temp_s1);
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

    arg0->velocity.y += -0x10000;
    applyClampedVelocityToPosition(arg0);

    if (func_8005D308_5DF08(arg0, 4) != 0) {
        arg0->unkB88 = 0;
        arg0->behaviorMode = 1;
        arg0->behaviorPhase = 1;
        arg0->behaviorStep = 0;
        arg0->behaviorCounter = 0;

        if (arg0->unkBDB == 0) {
            for (i = 0; i < 0x1E; i++) {
                spawnBossHomingProjectileVariant1Task(arg0);
            }
            memcpy(&sp30, &arg0->worldPos, sizeof(Vec3i));
            sp30.y += 0x300000;
            spawnSparkleEffectWithPlayer(&sp30, arg0->playerIndex);
            func_800BB910_B0E00(arg0);
            arg0->unkBDB = 3;
            func_8005D180_5DD80(arg0, 2);
        }
    }

    return 0;
}

s32 func_800BC3B8_B18A8(Player *arg0) {
    u8 temp;

    temp = arg0->behaviorStep;
    if (temp == 0) {
        arg0->behaviorStep = temp + 1;
        arg0->velocity.y = 0x80000;
        if (!(arg0->unkB84 & 0x80000)) {
            if (arg0->unkBDB != 0) {
                arg0->unkBDB = arg0->unkBDB - 1;
            }
        }
    }

    arg0->unkB88 = 0x200;
    arg0->velocity.x = 0;
    arg0->velocity.z = 0;
    arg0->velocity.y = arg0->velocity.y + (-0x8000);

    applyClampedVelocityToPosition(arg0);
    func_8005D180_5DD80(arg0, 5);

    if ((D_8009ADE0_9B9E0 & 0xF) == 0) {
        spawnBossHomingProjectileVariant2Task(arg0);
    }

    if (arg0->velocity.y < 0) {
        if (!(arg0->unkB84 & 0x1)) {
            temp = arg0->unkBDB;
            arg0->unkB88 = 0;
            arg0->behaviorMode = 1;
            arg0->behaviorPhase = 1;
            arg0->behaviorStep = 0;
            arg0->behaviorCounter = 0;

            if (temp == 0) {
                arg0->unkB84 = arg0->unkB84 | 0x100000;
            }

            return 0;
        }
    }

    return 0;
}

s32 func_800BC4AC_B199C(func_800BC4AC_arg *arg0) {
    s32 pad[3];

    getCurrentAllocation();

    arg0->velocity.y += -0x8000;
    applyClampedVelocityToPosition((Player *)arg0);

    if (arg0->behaviorStep == 0) {
        if (func_8005D308_5DF08(arg0, 6) != 0) {
            arg0->behaviorStep = arg0->behaviorStep + 1;
        }
        return 0;
    }

    func_8005D180_5DD80(arg0, 7);

    return 0;
}

void func_800BC520_B1A10(func_800BC4AC_arg *arg0) {
    s32 pad[8];
    Vec3i sp38;
    s32 pad2[8];
    GameState *alloc;
    GameDataLayout *allocPlus30;
    u16 temp;

    alloc = getCurrentAllocation();
    memcpy(arg0->unk984, &arg0->unk434, 0xC);
    allocPlus30 = &alloc->gameData;
    temp = func_80059E90_5AA90(arg0, allocPlus30, arg0->sectorIndex, &arg0->unk434);
    arg0->sectorIndex = temp;
    func_80060CDC_618DC(allocPlus30, temp, &arg0->unk434, 0x187000, &sp38);
    arg0->unk434.x = arg0->unk434.x + sp38.x;
    arg0->unk434.z = arg0->unk434.z + sp38.z;
    func_8005C868_5D468(arg0);

    if (arg0->bossFlags & 0x10000) {
        arg0->unkBC9 = 0;
    } else {
        func_8005CFFC_5DBFC(allocPlus30, arg0->sectorIndex, &arg0->unk434, &arg0->unkBC9, &arg0->unkBCC);
        arg0->unkBCA = arg0->unkBC9 >> 4;
        arg0->unkBC9 = arg0->unkBC9 & 0xF;
    }
}

INCLUDE_ASM("asm/nonmatchings/levels/ice_land_boss", func_800BC61C_B1B0C);

void func_800BC7A8_B1C98(Player *arg0) {
    s32 pad[36];
    s32 i;
    s32 index;

    getCurrentAllocation();
    func_800BC61C_B1B0C(arg0);

    index = arg0->unkBCC >> 4;

    if (index == 0) {
        for (i = 0; i < 4; i++) {
            enqueuePreLitMultiPartDisplayList(i, (enqueueMultiPartDisplayList_arg1 *)&arg0->unk38, arg0->unkBB7);
        }
    } else {
        arg0->unk6C = D_800BACC8_AAB78[index].unk0;
        arg0->unk6D = D_800BACC8_AAB78[index].unk1;
        arg0->unk6E = D_800BACC8_AAB78[index].unk2;
        arg0->unk70 = D_800BACC8_AAB78[index].unk4;
        arg0->unk71 = D_800BACC8_AAB78[index].unk5;
        arg0->unk72 = D_800BACC8_AAB78[index].unk6;

        for (i = 0; i < 4; i++) {
            enqueueMultiPartDisplayList(i, (enqueueMultiPartDisplayList_arg1 *)&arg0->unk38, arg0->unkBB7);
        }
    }
}

/**
 * Updates ground contact positions for the boss's 9 joints.
 * For each joint, computes X/Z world position from local offsets,
 * then finds the terrain height at that position.
 * Enqueues debug callbacks to render joint positions.
 */
void func_800BC89C_B1D8C(Player *boss) {
    GameState *gameState;
    GameDataLayout *gameData;
    s32 jointIndex;
    s32 isFlying;
    s32 jointOffset;
    u8 *jointWritePtr;
    JointPosition *jointPos;
    u16 sectorIndex;

    gameState = getCurrentAllocation();
    jointIndex = 0;
    isFlying = 0x400000;
    gameData = &gameState->gameData;
    jointWritePtr = (u8 *)boss;
    jointOffset = 0xA10;

    do {
        if (boss->unkB84 & isFlying) {
            // Flying mode: use flying joint offsets
            *(volatile s32 *)(jointWritePtr + 0xA10) =
                boss->unk970.translation.x + *(s32 *)((u8 *)func_800BC0A8_B1598 + 0x18 + jointOffset);
            *(volatile s32 *)(jointWritePtr + 0xA18) =
                boss->unk970.translation.z + *(s32 *)((u8 *)func_800BC0A8_B1598 + 0x20 + jointOffset);
        } else {
            // Ground mode: use ground joint offsets
            *(volatile s32 *)(jointWritePtr + 0xA10) =
                boss->unk970.translation.x + *(s32 *)((u8 *)&D_800BC054_B1544 + jointOffset);
            *(volatile s32 *)(jointWritePtr + 0xA18) =
                boss->unk970.translation.z + *(s32 *)((u8 *)&D_800BC05C_B154C + jointOffset);
        }

        jointPos = (JointPosition *)((u8 *)boss + jointOffset);
        sectorIndex = func_80059E90_5AA90((void *)boss, gameData, boss->sectorIndex, jointPos);
        jointOffset += 0xC;
        jointIndex += 1;
        // Compute terrain height at joint position
        *(volatile s32 *)(jointWritePtr + 0xA14) = func_8005CFC0_5DBC0(gameData, sectorIndex, jointPos, 0x100000);

        jointWritePtr += 0xC;
    } while (jointIndex < 9);

    boss->unkBC1 = 1;

    // Enqueue debug render callbacks for all 4 viewports
    for (jointIndex = 0; jointIndex < 4; jointIndex++) {
        debugEnqueueCallback(jointIndex, 1, func_800B9500_A93B0, (void *)boss);
    }
}
