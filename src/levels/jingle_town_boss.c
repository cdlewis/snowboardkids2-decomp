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

typedef void (*FuncPtr)(void *);

extern s16 identityMatrix[];
extern Transform3D D_8009A890_9B490;
extern s32 D_8009A8A4_9B4A4;
extern s32 D_8009A8A8_9B4A8;
extern s32 D_8009A8AC_9B4AC;
extern s32 gJingleTownBossSpawnPos[];

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} UnkA10Entry;

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
    s16 unk38[0x10]; /* 0x38 - 0x57 */
    u8 pad58[0x6C - 0x58];
    u8 unk6C; /* 0x6C */
    u8 unk6D; /* 0x6D */
    u8 unk6E; /* 0x6E */
    u8 pad6F;
    u8 unk70; /* 0x70 */
    u8 unk71; /* 0x71 */
    u8 unk72; /* 0x72 */
    u8 pad73;
    u8 unk74[0x20]; /* 0x74 - 0x93 */
    u8 pad94[0xA8 - 0x94];
    u8 unkA8; /* 0xA8 */
    u8 unkA9; /* 0xA9 */
    u8 unkAA; /* 0xAA */
    u8 padAB;
    u8 unkAC; /* 0xAC */
    u8 unkAD; /* 0xAD */
    u8 unkAE; /* 0xAE */
    u8 padAF;
    u8 unkB0[0x20]; /* 0xB0 - 0xCF */
    u8 padD0[0x434 - 0xD0];
    Vec3i unk434; /* 0x434 */
    s32 unk440;   /* 0x440 */
    s32 unk444;   /* 0x444 */
    s32 unk448;   /* 0x448 */
    s32 sideVelocity;     /* 0x44C */
    s32 verticalVelocity; /* 0x450 */
    s32 forwardVelocity;  /* 0x454 */
    u8 pad458[0x10];
    s32 unk468; /* 0x468 */
    u8 pad46C[0x8];
    s32 unk474; /* 0x474 */
    u8 pad478[0x950 - 0x478];
    u8 unk950[0x20];    /* 0x950 - 0x96F */
    Transform3D unk970; /* 0x970 - 0x98F */
    Transform3D unk990; /* 0x990 - 0x9AF */
    Transform3D unk9B0; /* 0x9B0 - 0x9CF */
    u8 pad9D0[0x9F0 - 0x9D0];
    u8 unk9F0[0x20];       /* 0x9F0 - 0xA0F */
    UnkA10Entry unkA10[9]; /* 0xA10 - 0xA7B (9 * 12 = 108 = 0x6C) */
    s32 unkA7C;            /* 0xA7C */
    u8 padA80[4];
    s32 unkA84; /* 0xA84 */
    u8 padA88[4];
    u16 unkA8C; /* 0xA8C */
    u16 unkA8E; /* 0xA8E */
    u16 unkA90; /* 0xA90 */
    u16 unkA92; /* 0xA92 */
    u16 unkA94; /* 0xA94 */
    u8 padA96[0xA9C - 0xA96];
    u16 pitchAngle; /* 0xA9C */
    u16 yawAngle;   /* 0xA9E */
    u8 padAA0[0x4];
    s32 unkAA4; /* 0xAA4 */
    s32 unkAA8; /* 0xAA8 */
    u8 padAAC[0xAC2 - 0xAAC];
    s16 unkAC2; /* 0xAC2 */
    u8 padAC4[0xAD4 - 0xAC4];
    s32 unkAD4; /* 0xAD4 */
    s32 unkAD8; /* 0xAD8 */
    s32 unkADC; /* 0xADC */
    u8 padAE0[0x4];
    s32 unkAE4; /* 0xAE4 */
    s32 unkAE8; /* 0xAE8 */
    s32 unkAEC; /* 0xAEC */
    s32 unkAF0; /* 0xAF0 */
    s32 unkAF4; /* 0xAF4 */
    s32 unkAF8; /* 0xAF8 */
    u8 padAFC[0xB50 - 0xAFC];
    s32 unkB50; /* 0xB50 */
    s32 unkB54; /* 0xB54 */
    s32 unkB58; /* 0xB58 */
    s32 unkB5C; /* 0xB5C */
    u8 padB60[0xB7A - 0xB60];
    s8 unkB7A;  /* 0xB7A */
    s8 unkB7B;  /* 0xB7B */
    u16 unkB7C; /* 0xB7C */
    u16 unkB7E; /* 0xB7E */
    u8 padB80[0x2];
    u16 unkB82; /* 0xB82 */
    s32 unkB84; /* 0xB84 */
    s32 unkB88; /* 0xB88 */
    s32 unkB8C; /* 0xB8C */
    u8 padB90[0xB94 - 0xB90];
    u16 sectorIndex; /* 0xB94 */
    u8 padB96[0xB9E - 0xB96];
    u16 unkB9E; /* 0xB9E */
    u8 padBA0[0xBB8 - 0xBA0];
    u8 unkBB8; /* 0xBB8 */
    u8 padBB9[0xBBD - 0xBB9];
    u8 behaviorMode;    /* 0xBBD */
    u8 behaviorPhase;   /* 0xBBE */
    u8 behaviorStep;    /* 0xBBF */
    u8 behaviorCounter; /* 0xBC0 */
    u8 unkBC1;          /* 0xBC1 */
    u8 padBC2[0x2];
    u8 finishPosition; /* 0xBC4 */
    u8 padBC5[0x2];
    u8 unkBC7; /* 0xBC7 */
    u8 unkBC8; /* 0xBC8 */
    u8 unkBC9; /* 0xBC9 */
    u8 unkBCA; /* 0xBCA */
    u8 padBCB[1];
    u8 unkBCC; /* 0xBCC */
    u8 padBCD[0x2];
    u8 unkBCF; /* 0xBCF */
    u8 padBD0[0xBDB - 0xBD0];
    u8 unkBDB; /* 0xBDB */
} Arg0Struct;

typedef struct {
    s16 m[3][3];
    s16 pad;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
} LocalMat;

typedef struct {
    Transform3D transform;
    u8 pad20[0x58 - 0x20];
    void *ptr;
    s32 unk5C;
    s32 unk60;
    s32 unk64;
} Element0x3C;

typedef s32 (*StateFunc)(void *);

extern s8 gAnalogStickY[];
extern s8 gAnalogStickX[];
extern StateFunc D_800BCB20_B40E0[];
extern s16 D_800BCB30_B40F0[];
extern s16 D_800BCB3C_B40FC[];
extern s32 gButtonsPressed[];
extern FuncPtr gChaseAttackPhaseHandlers[];
extern FuncPtr gHoverAttackPhaseHandlers[];
extern s16 gHoverIntroAnimTimers[];
extern u16 gHoverIntroAnimFrames[];
extern s32 D_800BCBA0_B4160[][3];
extern Vec3i D_800BCB68_B4128;
extern s32 gJingleTownBossSpawnPos[];
void func_800BC474_B3A34(Arg0Struct *);
extern D_800BACC8_AAB78_type D_800BACC8_AAB78[];

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
        func_800B9B90_A9A40((Player *)arg0);
    } else {
        arg0->unkB7A = gAnalogStickX[arg0->unkBB8] / 4;
        arg0->unkB7B = gAnalogStickY[arg0->unkBB8] / 4;

        arg0->unkB82 = arg0->unkB7C;
        arg0->unkB7C = gButtonsPressed[arg0->unkBB8];
        arg0->unkB7E = arg0->unkB7C & ~arg0->unkB82;
    }

    arg0->sideVelocity = arg0->unk434.x - arg0->unk440;
    arg0->verticalVelocity = arg0->unk434.y - arg0->unk444;
    arg0->forwardVelocity = arg0->unk434.z - arg0->unk448;
    memcpy(&arg0->unk440, &arg0->unk434, 0xC);

    temp = distance_3d(
        arg0->unk434.x - alloc->players->worldPos.x,
        arg0->unk434.y - alloc->players->worldPos.y,
        arg0->unk434.z - alloc->players->worldPos.z
    );

    if ((arg0->finishPosition == 0) && (temp > 0x1000000)) {
        arg0->unkAA4 = getCharacterBoardStatParam0(0, 0) - 0x40000;
    } else {
        arg0->unkAA4 = getCharacterBoardStatParam0(0, 0) + 0x180000;
    }

    if (arg0->unkAA4 > 0x180000) {
        arg0->unkAA4 = 0x180000;
    }

    for (i = 0; i < arg0->unkBCF; i++) {
        arg0->unkAA4 = arg0->unkAA4 - (arg0->unkAA4 >> 2);
    }

    diff = arg0->unkAA4 - arg0->unkAA8;
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
    } while (D_800BCB20_B40E0[arg0->behaviorMode](arg0) != 0);

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
    func_8005C838_5D438((ListNode_5AA90 *)&arg0->unkB50);
    func_800BC474_B3A34(arg0);

    transformVector(D_800BCB3C_B40FC, arg0->unk38, &arg0->unkAE4);
    arg0->unkAE4 -= arg0->unk970.translation.x;
    arg0->unkAE8 -= arg0->unk970.translation.y;
    arg0->unkAEC -= arg0->unk970.translation.z;

    transformVector(D_800BCB3C_B40FC + 6, (s16 *)arg0->unk74, &arg0->unkAF0);
    arg0->unkAF0 -= arg0->unk970.translation.x;
    arg0->unkAF4 -= arg0->unk970.translation.y;
    arg0->unkAF8 -= arg0->unk970.translation.z;
}

s32 func_800BB66C_B2C2C(Arg0Struct *arg0) {
    Vec3i sp10;
    Vec3i sp20;
    void *alloc;

    alloc = getCurrentAllocation();
    memcpy(&arg0->unk970, identityMatrix, sizeof(Transform3D));
    createYRotationMatrix(&arg0->unk970, arg0->unkA94);
    memcpy(&arg0->unk990, identityMatrix, sizeof(Transform3D));
    memcpy(&arg0->unk9B0, identityMatrix, sizeof(Transform3D));

    arg0->unk434.x = gJingleTownBossSpawnPos[arg0->unkBB8];
    getTrackSegmentWaypoints((u8 *)alloc + 0x30, 0, &sp10, &sp20);
    arg0->unk434.z = sp10.z + 0x200000;
    arg0->sectorIndex = func_80059E90_5AA90(arg0, (u8 *)alloc + 0x30, 0, &arg0->unk434);
    arg0->unk434.y = getTrackHeightInSector(
        (u8 *)alloc + 0x30,
        arg0->sectorIndex,
        &arg0->unk434,
        0x100000
    );
    memcpy(&arg0->unk440, &arg0->unk434, 0xC);
    arg0->sideVelocity = 0;
    arg0->verticalVelocity = 0;
    arg0->forwardVelocity = 0;
    arg0->unkA94 = 0x1000;

    {
        s32 i;
        u8 *elem;

        for (i = 0; i < 3; i++) {
            elem = (u8 *)arg0 + i * 0x3C;
            memcpy(elem + 0x38, identityMatrix, sizeof(Transform3D));
            *(s32 *)(elem + 0x5C) = *(s32 *)((u8 *)arg0 + 4);
            *(s32 *)(elem + 0x60) = *(s32 *)((u8 *)arg0 + 8);
            *(s32 *)(elem + 0x64) = 0;
            *(void **)(elem + 0x58) =
                (void *)(loadAssetByIndex_953B0(*(u8 *)((u8 *)arg0 + 0xBB9), *(u8 *)((u8 *)arg0 + 0xBBA)) + i * 0x10);
        }
    }

    arg0->behaviorMode = 1;
    *(s32 *)((u8 *)arg0 + 0xB30) = 0x180000;
    *(u8 *)((u8 *)arg0 + 0xBB4) = 2;
    arg0->behaviorPhase = 0;
    *(s32 *)((u8 *)arg0 + 0xB2C) = 0x1EC000;
    *(s32 *)((u8 *)arg0 + 0xAE0) = 0x1EC000;
    *(s32 *)((u8 *)arg0 + 0xB54) = (s32)&arg0->unk434;
    *(s32 *)((u8 *)arg0 + 0xB64) = 0x1EC000;
    *(u8 *)((u8 *)arg0 + 0xB68) = arg0->unkBB8;
    if (arg0->unkBC7 == 0) {
        spawnChaseCameraTask(arg0->unkBB8);
    }
    arg0->unkBDB = 0xA;
    if (*(s32 *)((u8 *)arg0 + 0x1C) != 0) {
        *(s32 *)((u8 *)arg0 + 0x28) =
            *(s32 *)((u8 *)arg0 + 0x1C) + ((s32 *)*(s32 *)((u8 *)arg0 + 0x1C))[arg0->unkBB8];
    }
    return 1;
}

void dispatchChaseAttackPhase(Arg0Struct *arg0) {
    gChaseAttackPhaseHandlers[arg0->behaviorPhase](arg0);
}

s32 jingleTownBossChaseAttackIntroPhase(Arg0Struct *arg0) {
    s32 pad[3];
    GameState *gameState = getCurrentAllocation();

    if (gameState->unk79 == 0) {
        setPlayerBehaviorPhase((Player *)arg0, 1);
        return 1;
    }

    arg0->sideVelocity -= arg0->sideVelocity / 8;
    arg0->forwardVelocity -= arg0->forwardVelocity / 8;
    arg0->verticalVelocity += -0x8000;
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
        if ((u32)gameState->unk50 < 0x1E) {
            arg0->unkB8C = ((randA() & 0xFF) >> 2) + 0x69;
        } else {
            arg0->unkB8C = (randA() & 0xFF) >> 1;
        }
        arg0->behaviorStep++;
    }

    angleDiff = func_8006D21C_6DE1C(arg0->unkA7C, arg0->unkA84, arg0->unk434.x, arg0->unk434.z) - arg0->unkA94;
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
        temp_s1 = (Vec3i *)&arg0->sideVelocity;
        transformVector3(temp_s1, &sp10, &sp30);
        sp30.x = 0;
        transformVector2(&sp30, &sp10, temp_s1);
        transformVector2(&D_800BCB68_B4128, &sp10, &sp30);

        if (sp30.y > 0) {
            sp30.y = 0;
        }

        arg0->sideVelocity += sp30.x;
        arg0->verticalVelocity += sp30.y;
        arg0->forwardVelocity += sp30.z;
    } else {
        arg0->sideVelocity -= arg0->sideVelocity / 16;
        arg0->forwardVelocity -= arg0->forwardVelocity / 16;
    }

    if (arg0->verticalVelocity > 0) {
        arg0->verticalVelocity = 0;
    }

    arg0->verticalVelocity -= 0x8000;
    applyClampedVelocityToPosition((Player *)arg0);

    switch (arg0->behaviorCounter) {
        case 0:
            func_800BC474_B3A34(arg0);
            transformVectorRelative(&gameState->players->worldPos.x, arg0->unk74, &sp30);

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
    arg0->sideVelocity -= arg0->sideVelocity / 8;
    arg0->forwardVelocity -= arg0->forwardVelocity / 8;
    arg0->verticalVelocity += -0x8000;
    applyClampedVelocityToPosition((Player *)arg0);
    return 0;
}

void dispatchHoverAttackPhase(Arg0Struct *arg0) {
    gHoverAttackPhaseHandlers[arg0->behaviorPhase](arg0);
}

s32 jingleTownBossHoverAttackIntroPhase(Arg0Struct *arg0) {
    u8 step;

    step = arg0->behaviorStep;
    if (step == 0) {
        arg0->behaviorStep = step + 1;
        arg0->unkB8C = 1;
        if (arg0->verticalVelocity > 0) {
            arg0->verticalVelocity = 0;
        }
        if (!(arg0->unkB84 & 0x80000)) {
            u8 hoverCount = arg0->unkBDB;
            if (hoverCount != 0) {
                arg0->unkBDB = hoverCount - 1;
            }
        }
    }

    arg0->unkB8C--;
    if (arg0->unkB8C == 0) {
        arg0->unkB8C = gHoverIntroAnimTimers[arg0->behaviorCounter * 2];
        arg0->unkB9E = gHoverIntroAnimFrames[arg0->behaviorCounter * 2];
        arg0->behaviorCounter++;
    }

    arg0->unkB88 = 0x10;
    arg0->sideVelocity = 0;
    arg0->forwardVelocity = 0;
    arg0->verticalVelocity -= 0x8000;
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

s32 func_800BC094_B3654(Arg0Struct *arg0) {
    if (arg0->behaviorStep == 0) {
        s32 temp_v1 = arg0->unkB84;
        arg0->behaviorStep++;
        arg0->unk468 = 0x80000;
        arg0->unkB8C = 4;
        if ((temp_v1 & 0x80000) == 0) {
            if (arg0->unkBDB != 0) {
                arg0->unkBDB--;
            }
        }
    }

    arg0->unkB88 = 0x200;
    arg0->verticalVelocity += -0x8000;
    arg0->unk468 += -0x8000;

    applyPitchAngleDamping(arg0);

    arg0->sideVelocity = 0;
    arg0->forwardVelocity = 0;
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

s32 func_800BC1C0_B3780(Arg0Struct *arg0) {
    Vec3i sp10;
    Vec3i sp20;
    u8 temp_v0;

    getCurrentAllocation();

    temp_v0 = arg0->behaviorPhase;
    if (temp_v0 == 0) {
        arg0->behaviorPhase++;
        transformVector2((s16 *)D_800BCBA0_B4160[0], arg0->unk38, &sp10);
        arg0->unk434.x += sp10.x;
        arg0->unk434.y += sp10.y;
        arg0->unk434.z += sp10.z;
        memcpy(&arg0->unk440, &arg0->unk434, 0xC);
        arg0->unkB84 |= 0x200000;
        transformVector((s16 *)D_800BCBA0_B4160[1], arg0->unk38, &sp20);
        spawnBurstEffect(&sp20);
        transformVector((s16 *)D_800BCBA0_B4160[2], arg0->unk38, &sp20);
        spawnBurstEffect(&sp20);
        transformVector((s16 *)D_800BCBA0_B4160[3], arg0->unk38, &sp20);
        spawnBurstEffect(&sp20);
        transformVector((s16 *)D_800BCBA0_B4160[4], arg0->unk38, &sp20);
        spawnBurstEffect(&sp20);
        transformVector((s16 *)D_800BCBA0_B4160[5], arg0->unk38, &sp20);
        spawnBurstEffect(&sp20);
        arg0->unk468 = 0x100;
    }

    arg0->sideVelocity -= arg0->sideVelocity / 8;
    arg0->forwardVelocity -= arg0->forwardVelocity / 8;
    arg0->verticalVelocity += -0x8000;
    applyPitchAngleDamping(arg0);
    arg0->yawAngle += arg0->unk468;
    if (arg0->unk468 != 0) {
        arg0->unk468 -= 2;
    }
    applyClampedVelocityToPosition((Player *)arg0);
    return 0;
}

void func_800BC378_B3938(Arg0Struct *arg0) {
    s32 pad[8];
    Vec3i sp38;
    s32 pad2[8];
    GameState *alloc;
    GameDataLayout *allocPlus30;
    u16 temp;

    alloc = getCurrentAllocation();
    memcpy(&arg0->unk970.translation.x, &arg0->unk434, 0xC);
    allocPlus30 = &alloc->gameData;
    temp = func_80059E90_5AA90(arg0, allocPlus30, arg0->sectorIndex, &arg0->unk434);
    arg0->sectorIndex = temp;
    func_80060CDC_618DC(allocPlus30, temp, &arg0->unk434, 0x187000, &sp38);
    arg0->unk434.x = arg0->unk434.x + sp38.x;
    arg0->unk434.z = arg0->unk434.z + sp38.z;
    func_8005C868_5D468(arg0);

    if (arg0->unkB84 & 0x10000) {
        arg0->unkBC9 = 0;
    } else {
        func_8005CFFC_5DBFC(allocPlus30, arg0->sectorIndex, &arg0->unk434, &arg0->unkBC9, &arg0->unkBCC);
        arg0->unkBCA = arg0->unkBC9 >> 4;
        arg0->unkBC9 = arg0->unkBC9 & 0xF;
    }
}

void func_800BC474_B3A34(Arg0Struct *arg0) {
    s32 pad1[8];
    LocalMat sp30;
    LocalMat sp50;
    s32 sp70[4];
    LocalMat *temp_s0;
    void *temp_s2;
    s32 *ptr;

    func_8006B084_6BC84(&arg0->unk990, &arg0->unk970, arg0->unk9F0);
    temp_s2 = arg0->unk950;
    func_8006B084_6BC84(&arg0->unk9B0, arg0->unk9F0, temp_s2);

    if (arg0->unkB88 & 0x10) {
        temp_s0 = &sp30;
        memcpy(temp_s0, identityMatrix, 0x20);
        temp_s0->m[1][1] = arg0->unkB9E;
        func_8006B084_6BC84(temp_s0, temp_s2, arg0->unk38);
    } else {
        memcpy(arg0->unk38, temp_s2, 0x20);
    }

    createCombinedRotationMatrix(&sp50, arg0->pitchAngle, arg0->yawAngle);
    sp50.unk14 = 0;
    sp50.unk1C = 0;

    if (arg0->unkB84 & 0x200000) {
        sp50.unk18 = 0x140000;
    } else {
        sp50.unk18 = 0x3A0000;
    }

    func_8006B084_6BC84(&sp50, arg0->unk38, arg0->unk74);

    ptr = &D_8009A8A4_9B4A4;
    *(s32 *)&arg0->unk74[0x18] = *(s32 *)&arg0->unk74[0x18] + arg0->unk474;

    *ptr = 0;
    D_8009A8A8_9B4A8 = 0x140000;
    D_8009A8AC_9B4AC = 0;

    func_8006B084_6BC84(ptr - 5, arg0->unk74, arg0->unkB0);
}

void func_800BC5A8_B3B68(Arg0Struct *arg0) {
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
    func_800BC474_B3A34(arg0);

    index = arg0->unkBCC >> 4;

    if (index == 0) {
        if (arg0->unkB84 & 0x200000) {
            for (i = 0; i < 4; i++) {
                enqueuePreLitMultiPartDisplayList(i, (enqueueMultiPartDisplayList_arg1 *)arg0->unk74, 2);
            }
        } else {
            for (i = 0; i < 4; i++) {
                enqueuePreLitMultiPartDisplayList(i, (enqueueMultiPartDisplayList_arg1 *)arg0->unk38, 3);
            }
        }
    } else {
        if (arg0->unkB84 & 0x200000) {
            arg0->unkA8 = D_800BACC8_AAB78[index].unk0;
            arg0->unkA9 = D_800BACC8_AAB78[index].unk1;
            arg0->unkAA = D_800BACC8_AAB78[index].unk2;
            arg0->unkAC = D_800BACC8_AAB78[index].unk4;
            arg0->unkAD = D_800BACC8_AAB78[index].unk5;
            arg0->unkAE = D_800BACC8_AAB78[index].unk6;

            for (i = 0; i < 4; i++) {
                enqueueMultiPartDisplayList(i, (enqueueMultiPartDisplayList_arg1 *)arg0->unk74, 2);
            }
        } else {
            arg0->unk6C = D_800BACC8_AAB78[index].unk0;
            arg0->unk6D = D_800BACC8_AAB78[index].unk1;
            arg0->unk6E = D_800BACC8_AAB78[index].unk2;
            arg0->unk70 = D_800BACC8_AAB78[index].unk4;
            arg0->unk71 = D_800BACC8_AAB78[index].unk5;
            arg0->unk72 = D_800BACC8_AAB78[index].unk6;

            for (i = 0; i < 4; i++) {
                enqueueMultiPartDisplayList(i, (enqueueMultiPartDisplayList_arg1 *)arg0->unk38, 3);
            }
        }
    }

    if (alloc->gamePaused != 0) {
        stopSoundEffectChannel(arg0->unkBB8, 0);
        return;
    }

    if (!(arg0->unkB84 & 0x10000)) {
        volume = isqrt64(
                     (s64)arg0->sideVelocity * arg0->sideVelocity + (s64)arg0->verticalVelocity * arg0->verticalVelocity +
                     (s64)arg0->forwardVelocity * arg0->forwardVelocity
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
                (s64)arg0->sideVelocity * arg0->sideVelocity + (s64)arg0->verticalVelocity * arg0->verticalVelocity +
                (s64)arg0->forwardVelocity * arg0->forwardVelocity
            ) > 0x40000) {
            s32 temp;

            angle = atan2Fixed(-arg0->sideVelocity, -arg0->forwardVelocity);
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
                    spawnDualSnowSprayEffect_SingleSlot(outVec1, outVec2, (Vec3i *)&arg0->sideVelocity, 0);
                }
            }
        }
    }
}

void func_800BCA10_B3FD0(Arg0Struct *arg0) {
    s32 i;
    GameDataLayout *temp_s5;
    GameState *alloc;

    alloc = getCurrentAllocation();
    i = 0;
    temp_s5 = &alloc->gameData;

    do {
        s32 *posPtr;
        u16 temp;

        arg0->unkA10[i].unk0 = arg0->unk970.translation.x + D_800BCBA0_B4160[6 + i][0];
        arg0->unkA10[i].unk8 = arg0->unk970.translation.z + D_800BCBA0_B4160[6 + i][2];
        posPtr = &arg0->unkA10[i].unk0;
        temp = func_80059E90_5AA90(arg0, temp_s5, arg0->sectorIndex, posPtr);
        arg0->unkA10[i].unk4 = getTrackHeightInSector(temp_s5, temp, posPtr, 0x100000);
        i++;
    } while (i < 9);

    arg0->unkBC1 = 1;

    for (i = 0; i < 4; i++) {
        debugEnqueueCallback((u16)i, 1, func_800B9500_A93B0, arg0);
    }
}
