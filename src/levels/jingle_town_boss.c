#include "36BE0.h"
#include "3CD70.h"
#include "42170.h"
#include "51060.h"
#include "52880.h"
#include "59290.h"
#include "5AA90.h"
#include "5DBC0.h"
#include "5E590.h"
#include "9FF70.h"
#include "A9A40.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
#include "rand.h"
#include "task_scheduler.h"

extern s16 identityMatrix[];
extern Transform3D D_8009A890_9B490;
extern Vec3i D_8009A8A4_9B4A4;
extern s32 gJingleTownBossSpawnPos[];

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
    u8 pad28[0x38 - 0x28];
    /* 0x38 */ Transform3D groundTransform;
    u8 pad58[0x6C - 0x58];
    /* 0x6C */ u8 groundPrimaryR;
    /* 0x6D */ u8 groundPrimaryG;
    /* 0x6E */ u8 groundPrimaryB;
    u8 pad6F;
    /* 0x70 */ u8 groundSecondaryR;
    /* 0x71 */ u8 groundSecondaryG;
    /* 0x72 */ u8 groundSecondaryB;
    u8 pad73;
    /* 0x74 */ Transform3D flyingTransform;
    u8 pad94[0xA8 - 0x94];
    /* 0xA8 */ u8 flyingPrimaryR;
    /* 0xA9 */ u8 flyingPrimaryG;
    /* 0xAA */ u8 flyingPrimaryB;
    u8 padAB;
    /* 0xAC */ u8 flyingSecondaryR;
    /* 0xAD */ u8 flyingSecondaryG;
    /* 0xAE */ u8 flyingSecondaryB;
    u8 padAF;
    /* 0xB0 */ Transform3D unkB0;
    u8 padD0[0x434 - 0xD0];
    /* 0x434 */ Vec3i unk434;
    /* 0x440 */ s32 unk440;
    /* 0x444 */ s32 unk444;
    /* 0x448 */ s32 unk448;
    /* 0x44C */ Vec3i velocity;
    u8 pad458[0x10];
    /* 0x468 */ s32 unk468;
    u8 pad46C[0x8];
    /* 0x474 */ s32 unk474;
    u8 pad478[0x950 - 0x478];
    /* 0x950 */ Transform3D unk950;
    /* 0x970 */ Transform3D unk970;
    /* 0x990 */ Transform3D unk990;
    /* 0x9B0 */ Transform3D unk9B0;
    u8 pad9D0[0x9F0 - 0x9D0];
    /* 0x9F0 */ Transform3D unk9F0;
    /* 0xA10 */ JointPosition jointPositions[9];
    /* 0xA7C */ s32 aiTargetX;
    u8 padA80[4];
    /* 0xA84 */ s32 aiTargetZ;
    u8 padA88[4];
    /* 0xA8C */ u16 unkA8C;
    /* 0xA8E */ u16 unkA8E;
    /* 0xA90 */ u16 unkA90;
    /* 0xA92 */ u16 unkA92;
    /* 0xA94 */ u16 unkA94;
    u8 padA96[0xA9C - 0xA96];
    /* 0xA9C */ u16 pitchAngle;
    /* 0xA9E */ u16 yawAngle;
    u8 padAA0[0x4];
    /* 0xAA4 */ s32 maxSpeedCap;
    /* 0xAA8 */ s32 unkAA8;
    u8 padAAC[0xAC2 - 0xAAC];
    /* 0xAC2 */ s16 unkAC2;
    u8 padAC4[0xAD4 - 0xAC4];
    /* 0xAD4 */ s32 unkAD4;
    /* 0xAD8 */ s32 unkAD8;
    /* 0xADC */ s32 unkADC;
    /* 0xAE0 */ s32 unkAE0;
    /* 0xAE4 */ s32 unkAE4;
    /* 0xAE8 */ s32 unkAE8;
    /* 0xAEC */ s32 unkAEC;
    /* 0xAF0 */ s32 unkAF0;
    /* 0xAF4 */ s32 unkAF4;
    /* 0xAF8 */ s32 unkAF8;
    u8 padAFC[0xB2C - 0xAFC];
    /* 0xB2C */ s32 unkB2C;
    /* 0xB30 */ s32 unkB30;
    /* 0xB34 */ s32 unkB34;
    /* 0xB38 */ s32 unkB38;
    /* 0xB3C */ s32 unkB3C;
    /* 0xB40 */ s32 unkB40;
    /* 0xB44 */ s32 unkB44;
    /* 0xB48 */ s32 unkB48;
    /* 0xB4C */ s32 unkB4C;
    /* 0xB50 */ s32 unkB50;
    /* 0xB54 */ s32 unkB54;
    /* 0xB58 */ s32 unkB58;
    /* 0xB5C */ s32 unkB5C;
    /* 0xB60 */ s32 unkB60;
    /* 0xB64 */ s32 unkB64;
    /* 0xB68 */ u8 unkB68;
    u8 padB69[0xB7A - 0xB69];
    /* 0xB7A */ s8 unkB7A;
    /* 0xB7B */ s8 unkB7B;
    /* 0xB7C */ u16 unkB7C;
    /* 0xB7E */ u16 unkB7E;
    u8 padB80[0x2];
    /* 0xB82 */ u16 unkB82;
    /* 0xB84 */ s32 unkB84;
    /* 0xB88 */ s32 unkB88;
    /* 0xB8C */ s32 unkB8C;
    u8 padB90[0xB94 - 0xB90];
    /* 0xB94 */ u16 sectorIndex;
    u8 padB96[0xB9E - 0xB96];
    /* 0xB9E */ u16 unkB9E;
    u8 padBA0[0xBB4 - 0xBA0];
    /* 0xBB4 */ u8 unkBB4;
    u8 padBB5[0xBB8 - 0xBB5];
    /* 0xBB8 */ u8 unkBB8;
    /* 0xBB9 */ u8 characterId;
    /* 0xBBA */ u8 boardIndex;
    u8 padBBB[0xBBD - 0xBBB];
    /* 0xBBD */ u8 behaviorMode;
    /* 0xBBE */ u8 behaviorPhase;
    /* 0xBBF */ u8 behaviorStep;
    /* 0xBC0 */ u8 behaviorCounter;
    /* 0xBC1 */ u8 unkBC1;
    u8 padBC2[0x2];
    /* 0xBC4 */ u8 finishPosition;
    u8 padBC5[0x2];
    /* 0xBC7 */ u8 unkBC7;
    /* 0xBC8 */ u8 unkBC8;
    /* 0xBC9 */ u8 unkBC9;
    /* 0xBCA */ u8 unkBCA;
    u8 padBCB[1];
    /* 0xBCC */ u8 unkBCC;
    u8 padBCD[0x2];
    /* 0xBCF */ u8 unkBCF;
    u8 padBD0[0xBDB - 0xBD0];
    /* 0xBDB */ u8 unkBDB;
} Arg0Struct;

typedef struct {
    Transform3D transform;
    u8 pad20[0x58 - 0x20];
    void *ptr;
    s32 unk5C;
    s32 unk60;
    s32 unk64;
} Element0x3C;

typedef struct {
    s16 frames;
    s16 anim;
} HoverIntroAnimFrame;

typedef void (*FuncPtr)(void *);
typedef s32 (*StateFunc)(void *);

extern s8 gAnalogStickY[];
extern s8 gAnalogStickX[];
extern s32 gButtonsPressed[];
void updateJingleTownBossModelTransforms(Arg0Struct *);
extern BossSurfaceColor gBossSurfaceColors[];

s32 func_800BB66C_B2C2C(Arg0Struct *);
void dispatchChaseAttackPhase(Arg0Struct *);
void dispatchHoverAttackPhase(Arg0Struct *);
s32 jingleTownBossHoverAttackExitPhase(Arg0Struct *);
s32 jingleTownBossChaseAttackIntroPhase(Arg0Struct *);
s32 func_800BB930_B2EF0(Arg0Struct *);
s32 jingleTownBossChaseAttackExitPhase(Arg0Struct *);
s32 jingleTownBossHoverAttackIntroPhase(Arg0Struct *);
s32 jingleTownBossHoverAttackMainPhase(Arg0Struct *);

StateFunc gJingleTownBossBehaviorModeHandlers[] = {
    (StateFunc)func_800BB66C_B2C2C,
    (StateFunc)dispatchChaseAttackPhase,
    (StateFunc)dispatchHoverAttackPhase,
    (StateFunc)jingleTownBossHoverAttackExitPhase,
};

s16 D_800BCB30_B40F0[] = { 0x0000, 0x0000, 0x0013, (s16)0x9000, 0x0000, 0x0000 };

s16 D_800BCB3C_B40FC[] = { 0x0000, 0x0000, 0x0013, (s16)0x9000, 0x0000, 0x0000,
                           0x0000, 0x0000, 0x0000, 0x0000,      0x0000, 0x0000 };

s32 gJingleTownBossSpawnPos[] = { 0xFFF00000, 0x00300000 };

FuncPtr gChaseAttackPhaseHandlers[] = {
    (FuncPtr)jingleTownBossChaseAttackIntroPhase,
    (FuncPtr)func_800BB930_B2EF0,
    (FuncPtr)jingleTownBossChaseAttackExitPhase,
};

Vec3i D_800BCB68_B4128 = { .x = 0, .y = 0, .z = 0x10000 };

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

void updateJingleTownBoss(Arg0Struct *arg0) {
    Transform3D sp10;
    Transform3D sp30;
    GameState *alloc;
    s32 temp;
    s32 i;
    s16 playerState;
    s32 diff;

    alloc = getCurrentAllocation();
    if (arg0->unkBC7 != 0) {
        calculateAITargetPosition((Player *)arg0);
    } else {
        arg0->unkB7A = gAnalogStickX[arg0->unkBB8] / 4;
        arg0->unkB7B = gAnalogStickY[arg0->unkBB8] / 4;

        arg0->unkB82 = arg0->unkB7C;
        arg0->unkB7C = gButtonsPressed[arg0->unkBB8];
        arg0->unkB7E = arg0->unkB7C & ~arg0->unkB82;
    }

    arg0->velocity.x = arg0->unk434.x - arg0->unk440;
    arg0->velocity.y = arg0->unk434.y - arg0->unk444;
    arg0->velocity.z = arg0->unk434.z - arg0->unk448;
    memcpy(&arg0->unk440, &arg0->unk434, 0xC);

    temp = distance_3d(
        arg0->unk434.x - alloc->players->worldPos.x,
        arg0->unk434.y - alloc->players->worldPos.y,
        arg0->unk434.z - alloc->players->worldPos.z
    );

    if ((arg0->finishPosition == 0) && (temp > 0x1000000)) {
        arg0->maxSpeedCap = getCharacterBoardStatParam0(0, 0) - 0x40000;
    } else {
        arg0->maxSpeedCap = getCharacterBoardStatParam0(0, 0) + 0x180000;
    }

    if (arg0->maxSpeedCap > 0x180000) {
        arg0->maxSpeedCap = 0x180000;
    }

    for (i = 0; i < arg0->unkBCF; i++) {
        arg0->maxSpeedCap = arg0->maxSpeedCap - (arg0->maxSpeedCap >> 2);
    }

    diff = arg0->maxSpeedCap - arg0->unkAA8;
    if (diff >= 0x101) {
        diff = 0x100;
    }
    if (diff < -0x80) {
        diff = -0x80;
    }
    arg0->unkAA8 = arg0->unkAA8 + diff;

    arg0->unkB84 &= 0xFFFBFFFF;

    if (arg0->behaviorMode != 3) {
        playerState = arg0->unkAC2;
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
    arg0->unkAC2 = 0;

    do {
    } while (gJingleTownBossBehaviorModeHandlers[arg0->behaviorMode](arg0) != 0);

    createZRotationMatrix(&arg0->unk9B0, arg0->unkA92);
    createCombinedRotationMatrix(&arg0->unk990, arg0->unkA8E, arg0->unkA90);
    createYRotationMatrix(&arg0->unk970, arg0->unkA94);

    func_8006B084_6BC84(&arg0->unk9B0, &arg0->unk990, &sp10);
    func_8006B084_6BC84(&sp10, &arg0->unk970, &sp30);

    sp30.translation.x -= arg0->unk970.translation.x;
    sp30.translation.y -= arg0->unk970.translation.y;
    sp30.translation.z -= arg0->unk970.translation.z;

    transformVector(D_800BCB30_B40F0, (s16 *)&sp30, &arg0->unkAD4);
    memcpy(&arg0->unkB58, &arg0->unkAD4, 0xC);
    addCollisionSectorNodeToList((ListNode_5AA90 *)&arg0->unkB50);
    updateJingleTownBossModelTransforms(arg0);

    transformVector(D_800BCB3C_B40FC, (s16 *)&arg0->groundTransform, &arg0->unkAE4);
    arg0->unkAE4 -= arg0->unk970.translation.x;
    arg0->unkAE8 -= arg0->unk970.translation.y;
    arg0->unkAEC -= arg0->unk970.translation.z;

    transformVector(D_800BCB3C_B40FC + 6, (s16 *)&arg0->flyingTransform, &arg0->unkAF0);
    arg0->unkAF0 -= arg0->unk970.translation.x;
    arg0->unkAF4 -= arg0->unk970.translation.y;
    arg0->unkAF8 -= arg0->unk970.translation.z;
}

/**
 * Initialize the Jingle Town Boss state.
 *
 * This function is the entry point for the boss behavior state machine
 * (behaviorMode = 0). It initializes:
 * - Transform matrices for rotation
 * - Boss spawn position based on track waypoints
 * - Display object transforms and asset pointers
 * - Various behavior state variables
 *
 * @return 1 to indicate the state machine should continue processing
 */
s32 func_800BB66C_B2C2C(Arg0Struct *arg0) {
    Vec3i spawnWaypoint1;
    Vec3i spawnWaypoint2;
    GameState *gameState;
    s32 i;

    gameState = getCurrentAllocation();
    memcpy(&arg0->unk970, identityMatrix, sizeof(Transform3D));
    createYRotationMatrix(&arg0->unk970, arg0->unkA94);
    memcpy(&arg0->unk990, identityMatrix, sizeof(Transform3D));
    memcpy(&arg0->unk9B0, identityMatrix, sizeof(Transform3D));

    arg0->unk434.x = gJingleTownBossSpawnPos[arg0->unkBB8];
    getTrackSegmentWaypoints(&gameState->gameData, 0, &spawnWaypoint1, &spawnWaypoint2);
    arg0->unk434.z = spawnWaypoint1.z + 0x200000;
    arg0->sectorIndex = getOrUpdatePlayerSectorIndex(arg0, &gameState->gameData, 0, &arg0->unk434);
    arg0->unk434.y = getTrackHeightInSector(&gameState->gameData, arg0->sectorIndex, &arg0->unk434, 0x100000);
    memcpy(&arg0->unk440, &arg0->unk434, sizeof(Vec3i));
    arg0->velocity.x = 0;
    arg0->velocity.y = 0;
    arg0->velocity.z = 0;
    arg0->unkA94 = 0x1000;

    /* Initialize 3 display objects:
     * - Each element is 0x3C bytes apart
     * - Transform3D at offset 0x38 (groundTransform, flyingTransform, unkB0)
     * - Asset pointer and params at offsets 0x58, 0x5C, 0x60, 0x64 */
    for (i = 0; i < 3; i++) {
        u8 *elem = (u8 *)arg0 + i * 0x3C;
        memcpy(elem + 0x38, identityMatrix, sizeof(Transform3D));
        *(s32 *)(elem + 0x5C) = (s32)arg0->unk4;
        *(s32 *)(elem + 0x60) = (s32)arg0->unk8;
        *(s32 *)(elem + 0x64) = 0;
        *(void **)(elem + 0x58) = (void *)(loadAssetByIndex_953B0(arg0->characterId, arg0->boardIndex) + i * 0x10);
    }

    arg0->behaviorMode = 1;
    arg0->unkB30 = 0x180000;
    arg0->unkBB4 = 2;
    arg0->behaviorPhase = 0;
    arg0->unkB2C = 0x1EC000;
    arg0->unkAE0 = 0x1EC000;
    arg0->unkB54 = (s32)&arg0->unk434;
    arg0->unkB64 = 0x1EC000;
    arg0->unkB68 = arg0->unkBB8;
    if (arg0->unkBC7 == 0) {
        spawnChaseCameraTask(arg0->unkBB8);
    }
    arg0->unkBDB = 0xA;
    if ((s32)arg0->unk1C != 0) {
        *(s32 *)arg0->pad28 = (s32)arg0->unk1C + ((s32 *)arg0->unk1C)[arg0->unkBB8];
    }
    return 1;
}

void dispatchChaseAttackPhase(Arg0Struct *arg0) {
    ((void (*)(void *))gChaseAttackPhaseHandlers[arg0->behaviorPhase])(arg0);
}

s32 jingleTownBossChaseAttackIntroPhase(Arg0Struct *arg0) {
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

    return 0;
}

s32 func_800BB930_B2EF0(Arg0Struct *arg0) {
    Transform3D sp10;
    Vec3i sp30;
    Transform3D *temp_s0;
    Vec3i *temp_s1;
    s16 angleDiff;
    GameState *gameState;

    gameState = getCurrentAllocation();

    if (arg0->unkB84 & 0x100000) {
        setPlayerBehaviorMode((Player *)arg0, 3);
        return 1;
    }

    if (arg0->unkB84 & 0x80000) {
        setPlayerBehaviorPhase((Player *)arg0, 2);
        return 1;
    }

    if (arg0->behaviorStep == 0) {
        if ((u32)gameState->raceFrameCounter < 0x1E) {
            arg0->unkB8C = ((randA() & 0xFF) >> 2) + 0x69;
        } else {
            arg0->unkB8C = (randA() & 0xFF) >> 1;
        }
        arg0->behaviorStep++;
    }

    angleDiff = computeAngleToPosition(arg0->aiTargetX, arg0->aiTargetZ, arg0->unk434.x, arg0->unk434.z) - arg0->unkA94;
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

    if (!(arg0->unkB84 & 1)) {
        temp_s0 = &arg0->unk970;
        createYRotationMatrix(temp_s0, arg0->unkA94);
        func_8006BDBC_6C9BC((BoneAnimationState *)&arg0->unk990, temp_s0, &sp10);
        temp_s1 = &arg0->velocity;
        transformVector3(temp_s1, &sp10, &sp30);
        sp30.x = 0;
        transformVector2(&sp30, &sp10, temp_s1);
        transformVector2(&D_800BCB68_B4128, &sp10, &sp30);

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
    applyClampedVelocityToPosition((Player *)arg0);

    switch (arg0->behaviorCounter) {
        case 0:
            updateJingleTownBossModelTransforms(arg0);
            transformVectorRelative(&gameState->players->worldPos.x, &arg0->flyingTransform, &sp30);

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

            arg0->yawAngle = (arg0->yawAngle + angleDiff) & 0x1FFF;

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
                short temp = (arg0->pitchAngle + angleDiff) & 0x1FFF;
                arg0->pitchAngle = temp;
                if (temp >= 0x1000) {
                    arg0->pitchAngle = temp - 0x2000;
                }
            }

            if ((s16)arg0->pitchAngle >= 0x201) {
                arg0->pitchAngle = 0x200;
            }

            if ((s16)arg0->pitchAngle < -0x200) {
                s16 negVal = -0x200;
                arg0->pitchAngle = negVal;
            }

            if (arg0->unkB8C != 0) {
                arg0->unkB8C--;
            } else {
                if (gameState->players->unkB88 != 0) {
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
            arg0->yawAngle = arg0->yawAngle - 0x100;
            arg0->unkB8C--;
            if (arg0->unkB8C == 0) {
                spawnAttackProjectile(2, arg0->unkBB8, 0);
                arg0->unkB8C = 4;
                arg0->behaviorCounter++;
            }
            break;

        case 2:
            arg0->yawAngle = arg0->yawAngle + 0x100;
            arg0->unkB8C--;
            if (arg0->unkB8C == 0) {
                arg0->unkB8C = 4;
                arg0->behaviorCounter++;
                spawnAttackProjectile(3, arg0->unkBB8, 0);
                return 0;
            }
            break;

        case 3:
            arg0->yawAngle = arg0->yawAngle + 0x100;
            arg0->unkB8C--;
            if (arg0->unkB8C == 0) {
                spawnAttackProjectile(2, arg0->unkBB8, 0);
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
                spawnAttackProjectile(3, arg0->unkBB8, 0);
            }
            arg0->yawAngle = arg0->yawAngle - 0x100;
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

void applyPitchAngleDamping(Arg0Struct *arg0) {
    s16 currentAngle;
    s16 damping;

    currentAngle = arg0->pitchAngle;
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

    arg0->pitchAngle = currentAngle + damping;
}

s32 jingleTownBossChaseAttackExitPhase(Arg0Struct *arg0) {
    s32 pad[3];

    applyPitchAngleDamping(arg0);
    arg0->yawAngle -= 0x100;
    arg0->velocity.x -= arg0->velocity.x / 8;
    arg0->velocity.z -= arg0->velocity.z / 8;
    arg0->velocity.y += -0x8000;
    applyClampedVelocityToPosition((Player *)arg0);
    return 0;
}

void dispatchHoverAttackPhase(Arg0Struct *arg0) {
    ((void (*)(void *))gHoverAttackPhaseHandlers[arg0->behaviorPhase])(arg0);
}

s32 jingleTownBossHoverAttackIntroPhase(Arg0Struct *arg0) {
    if (arg0->behaviorStep == 0) {
        arg0->behaviorStep++;
        arg0->unkB8C = 1;
        if (arg0->velocity.y > 0) {
            arg0->velocity.y = 0;
        }
        if (!(arg0->unkB84 & 0x80000)) {
            if (arg0->unkBDB != 0) {
                arg0->unkBDB--;
            }
        }
    }

    arg0->unkB8C--;
    if (arg0->unkB8C == 0) {
        arg0->unkB8C = gHoverIntroAnimFrames[arg0->behaviorCounter].frames;
        arg0->unkB9E = gHoverIntroAnimFrames[arg0->behaviorCounter].anim;
        arg0->behaviorCounter++;
    }

    arg0->unkB88 = 0x10;
    arg0->velocity.x = 0;
    arg0->velocity.z = 0;
    arg0->velocity.y -= 0x8000;
    applyClampedVelocityToPosition((Player *)arg0);

    if (arg0->unkB8C == -1) {
        arg0->unkB88 = 0;
        arg0->behaviorMode = 1;
        arg0->behaviorPhase = 1;
        arg0->behaviorStep = 0;
        arg0->behaviorCounter = 0;
        arg0->unk474 = 0;
        if (arg0->unkBDB == 0) {
            arg0->unkB84 |= 0x100000;
        }
    }

    return 0;
}

s32 jingleTownBossHoverAttackMainPhase(Arg0Struct *arg0) {
    if (arg0->behaviorStep == 0) {
        s32 bossFlags = arg0->unkB84;
        arg0->behaviorStep++;
        arg0->unk468 = 0x80000;
        arg0->unkB8C = 4;
        if ((bossFlags & 0x80000) == 0) {
            if (arg0->unkBDB != 0) {
                arg0->unkBDB--;
            }
        }
    }

    arg0->unkB88 = 0x200;
    arg0->velocity.y += -0x8000;
    arg0->unk468 += -0x8000;

    applyPitchAngleDamping(arg0);

    arg0->velocity.x = 0;
    arg0->velocity.z = 0;
    arg0->yawAngle = (arg0->yawAngle + 0x100) & 0x1FFF;

    if (arg0->unkB8C == 0) {
        u8 rand_val = randA();
        s32 param = (rand_val & 1) | 2;
        spawnAttackProjectile(param, arg0->unkBB8, 0);
        arg0->unkB8C = 4;
    } else {
        arg0->unkB8C--;
    }

    applyClampedVelocityToPosition((Player *)arg0);

    arg0->unk474 += arg0->unk468;

    if (arg0->unk474 == 0) {
        arg0->unkB88 = 0;
        arg0->behaviorMode = 1;
        arg0->behaviorPhase = 1;
        arg0->behaviorStep = 0;
        arg0->behaviorCounter = 0;
        arg0->unk474 = 0;

        if (arg0->unkBDB == 0) {
            arg0->unkB84 |= 0x100000;
        }
    }

    return 0;
}

s32 jingleTownBossHoverAttackExitPhase(Arg0Struct *arg0) {
    Vec3i posOffset;
    Vec3i burstPos;
    u8 phase;

    getCurrentAllocation();

    phase = arg0->behaviorPhase;
    if (phase == 0) {
        arg0->behaviorPhase++;
        transformVector2((s16 *)&gJingleTownBossHoverExitOffsets[0], (s16 *)&arg0->groundTransform, &posOffset);
        arg0->unk434.x += posOffset.x;
        arg0->unk434.y += posOffset.y;
        arg0->unk434.z += posOffset.z;
        memcpy(&arg0->unk440, &arg0->unk434, 0xC);
        arg0->unkB84 |= 0x200000;
        transformVector((s16 *)&gJingleTownBossHoverExitOffsets[1], (s16 *)&arg0->groundTransform, &burstPos);
        spawnBurstEffect(&burstPos);
        transformVector((s16 *)&gJingleTownBossHoverExitOffsets[2], (s16 *)&arg0->groundTransform, &burstPos);
        spawnBurstEffect(&burstPos);
        transformVector((s16 *)&gJingleTownBossHoverExitOffsets[3], (s16 *)&arg0->groundTransform, &burstPos);
        spawnBurstEffect(&burstPos);
        transformVector((s16 *)&gJingleTownBossHoverExitOffsets[4], (s16 *)&arg0->groundTransform, &burstPos);
        spawnBurstEffect(&burstPos);
        transformVector((s16 *)&gJingleTownBossHoverExitOffsets[5], (s16 *)&arg0->groundTransform, &burstPos);
        spawnBurstEffect(&burstPos);
        arg0->unk468 = 0x100;
    }

    arg0->velocity.x -= arg0->velocity.x / 8;
    arg0->velocity.z -= arg0->velocity.z / 8;
    arg0->velocity.y += -0x8000;
    applyPitchAngleDamping(arg0);
    arg0->yawAngle += arg0->unk468;
    if (arg0->unk468 != 0) {
        arg0->unk468 -= 2;
    }
    applyClampedVelocityToPosition((Player *)arg0);
    return 0;
}

void updateJingleTownBossPositionAndTrackCollision(Arg0Struct *arg0) {
    s32 pad[8];
    Vec3i collisionOffset;
    s32 pad2[8];
    GameState *gameState;
    GameDataLayout *gameData;
    u16 newSectorIndex;

    gameState = getCurrentAllocation();
    memcpy(&arg0->unk970.translation.x, &arg0->unk434, 0xC);
    gameData = &gameState->gameData;
    newSectorIndex = getOrUpdatePlayerSectorIndex(arg0, gameData, arg0->sectorIndex, &arg0->unk434);
    arg0->sectorIndex = newSectorIndex;
    func_80060CDC_618DC(gameData, newSectorIndex, &arg0->unk434, 0x187000, &collisionOffset);
    arg0->unk434.x = arg0->unk434.x + collisionOffset.x;
    arg0->unk434.z = arg0->unk434.z + collisionOffset.z;
    func_8005C868_5D468(arg0);

    if (arg0->unkB84 & 0x10000) {
        arg0->unkBC9 = 0;
    } else {
        func_8005CFFC_5DBFC(gameData, arg0->sectorIndex, &arg0->unk434, &arg0->unkBC9, &arg0->unkBCC);
        arg0->unkBCA = arg0->unkBC9 >> 4;
        arg0->unkBC9 = arg0->unkBC9 & 0xF;
    }
}

void updateJingleTownBossModelTransforms(Arg0Struct *arg0) {
    s32 pad1[8];
    Transform3D scaledMatrix;
    Transform3D pitchYawMatrix;
    s32 sp70[4];
    Transform3D *scaledMatrixPtr;
    Transform3D *combinedTransform;

    // Combine rotation matrices: unk990 (Z rotation) * unk970 (Y rotation) = unk9F0
    func_8006B084_6BC84(&arg0->unk990, &arg0->unk970, &arg0->unk9F0);
    combinedTransform = &arg0->unk950;
    // Combine with unk9B0 (X rotation) to get base transform
    func_8006B084_6BC84(&arg0->unk9B0, &arg0->unk9F0, combinedTransform);

    if (arg0->unkB88 & 0x10) {
        // Apply vertical scale transformation during intro animation
        scaledMatrixPtr = &scaledMatrix;
        memcpy(scaledMatrixPtr, identityMatrix, 0x20);
        scaledMatrixPtr->m[1][1] = arg0->unkB9E;
        func_8006B084_6BC84(scaledMatrixPtr, combinedTransform, &arg0->groundTransform);
    } else {
        memcpy(&arg0->groundTransform, combinedTransform, 0x20);
    }

    // Create pitch and yaw rotation matrix for the flying/floating transform
    createCombinedRotationMatrix(&pitchYawMatrix, arg0->pitchAngle, arg0->yawAngle);
    pitchYawMatrix.translation.x = 0;
    pitchYawMatrix.translation.z = 0;
    // Y offset: lower (0x140000) during hover exit, higher (0x3A0000) otherwise
    if (arg0->unkB84 & 0x200000) {
        pitchYawMatrix.translation.y = 0x140000;
    } else {
        pitchYawMatrix.translation.y = 0x3A0000;
    }

    // Apply rotation/offset to ground transform to get flying transform
    func_8006B084_6BC84(&pitchYawMatrix, &arg0->groundTransform, &arg0->flyingTransform);

    // Add hover height offset to flying transform
    arg0->flyingTransform.translation.y = arg0->flyingTransform.translation.y + arg0->unk474;

    // Create translation-only matrix for the third transform (unkB0)
    D_8009A8A4_9B4A4.x = 0;
    D_8009A8A4_9B4A4.y = 0x140000;
    D_8009A8A4_9B4A4.z = 0;

    func_8006B084_6BC84((Transform3D *)((s32 *)&D_8009A8A4_9B4A4 - 5), &arg0->flyingTransform, &arg0->unkB0);
}

void renderJingleTownBossWithEffects(Arg0Struct *arg0) {
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

    index = arg0->unkBCC >> 4;

    if (index == 0) {
        if (arg0->unkB84 & 0x200000) {
            for (i = 0; i < 4; i++) {
                enqueuePreLitMultiPartDisplayList(i, (enqueueMultiPartDisplayList_arg1 *)&arg0->flyingTransform, 2);
            }
        } else {
            for (i = 0; i < 4; i++) {
                enqueuePreLitMultiPartDisplayList(i, (enqueueMultiPartDisplayList_arg1 *)&arg0->groundTransform, 3);
            }
        }
    } else {
        if (arg0->unkB84 & 0x200000) {
            arg0->flyingPrimaryR = gBossSurfaceColors[index].primaryR;
            arg0->flyingPrimaryG = gBossSurfaceColors[index].primaryG;
            arg0->flyingPrimaryB = gBossSurfaceColors[index].primaryB;
            arg0->flyingSecondaryR = gBossSurfaceColors[index].secondaryR;
            arg0->flyingSecondaryG = gBossSurfaceColors[index].secondaryG;
            arg0->flyingSecondaryB = gBossSurfaceColors[index].secondaryB;

            for (i = 0; i < 4; i++) {
                enqueueMultiPartDisplayList(i, (enqueueMultiPartDisplayList_arg1 *)&arg0->flyingTransform, 2);
            }
        } else {
            arg0->groundPrimaryR = gBossSurfaceColors[index].primaryR;
            arg0->groundPrimaryG = gBossSurfaceColors[index].primaryG;
            arg0->groundPrimaryB = gBossSurfaceColors[index].primaryB;
            arg0->groundSecondaryR = gBossSurfaceColors[index].secondaryR;
            arg0->groundSecondaryG = gBossSurfaceColors[index].secondaryG;
            arg0->groundSecondaryB = gBossSurfaceColors[index].secondaryB;

            for (i = 0; i < 4; i++) {
                enqueueMultiPartDisplayList(i, (enqueueMultiPartDisplayList_arg1 *)&arg0->groundTransform, 3);
            }
        }
    }

    if (alloc->gamePaused != 0) {
        stopSoundEffectChannel(arg0->unkBB8, 0);
        return;
    }

    if (!(arg0->unkB84 & 0x10000)) {
        volume = isqrt64(
                     (s64)arg0->velocity.x * arg0->velocity.x + (s64)arg0->velocity.y * arg0->velocity.y +
                     (s64)arg0->velocity.z * arg0->velocity.z
                 ) >>
                 12;
        if (volume >= 0x81) {
            volume = 0x80;
        }
        queueSoundAtPositionWithVolumeAndFlags(&arg0->unk434, 1, 0, 2, arg0->unkBB8, volume);
    } else {
        stopSoundEffectChannel(arg0->unkBB8, 0);
    }

    if (!(arg0->unkB84 & 1)) {
        if (isqrt64(
                (s64)arg0->velocity.x * arg0->velocity.x + (s64)arg0->velocity.y * arg0->velocity.y +
                (s64)arg0->velocity.z * arg0->velocity.z
            ) > 0x40000) {
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
            sp68.x += arg0->unk434.x;
            sp68.y += arg0->unk434.y;
            sp68.z += arg0->unk434.z;
            sp78.x += arg0->unk434.x;
            sp78.y += arg0->unk434.y;
            sp78.z += arg0->unk434.z;

            temp = arg0->unkBCC & 0xF;
            if (temp >= 0) {
                if (temp < 2) {
                    spawnDualSnowSprayEffect_SingleSlot(outVec1, outVec2, &arg0->velocity, 0);
                }
            }
        }
    }
}

/**
 * Updates joint positions for the jingle town boss.
 * For each of the 9 joints, computes the X/Z position from the boss's position
 * plus joint-specific offsets, then calculates the track height at that position.
 * Finally enqueues debug callbacks to render the joint positions.
 */
void updateJingleTownBossJointPositions(Arg0Struct *arg0) {
    s32 i;
    GameDataLayout *temp_s5;
    GameState *alloc;

    alloc = getCurrentAllocation();
    i = 0;
    temp_s5 = &alloc->gameData;

    for (i = 0; i < 9; i++) {
        s32 *posPtr;
        u16 temp;

        arg0->jointPositions[i].x = arg0->unk970.translation.x + gJingleTownBossHoverExitOffsets[6 + i].x;
        arg0->jointPositions[i].z = arg0->unk970.translation.z + gJingleTownBossHoverExitOffsets[6 + i].z;
        posPtr = &arg0->jointPositions[i].x;
        temp = getOrUpdatePlayerSectorIndex(arg0, temp_s5, arg0->sectorIndex, posPtr);
        arg0->jointPositions[i].y = getTrackHeightInSector(temp_s5, temp, posPtr, 0x100000);
    }

    arg0->unkBC1 = 1;

    for (i = 0; i < 4; i++) {
        debugEnqueueCallback((u16)i, 1, func_800B9500_A93B0, arg0);
    }
}
