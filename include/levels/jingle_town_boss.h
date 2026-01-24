#pragma once

#include "common.h"
#include "gamestate.h"
#include "geometry.h"

// Type definitions
typedef void (*FuncPtr)(void *);
typedef s32 (*StateFunc)(void *);

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
} JingleTownBossArg;

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

// Global variable declarations
extern s32 gJingleTownBossSpawnPos[];
extern BossSurfaceColor gBossSurfaceColors[];

// Function declarations
void updateJingleTownBoss(JingleTownBossArg *arg0);
void updateJingleTownBossModelTransforms(JingleTownBossArg *arg0);
void renderJingleTownBossWithEffects(JingleTownBossArg *arg0);
void updateJingleTownBossPositionAndTrackCollision(JingleTownBossArg *arg0);
void updateJingleTownBossJointPositions(JingleTownBossArg *arg0);
void applyPitchAngleDamping(JingleTownBossArg *arg0);
void dispatchChaseAttackPhase(JingleTownBossArg *arg0);
void dispatchHoverAttackPhase(JingleTownBossArg *arg0);
s32 func_800BB66C_B2C2C(JingleTownBossArg *arg0);
s32 jingleTownBossHoverAttackExitPhase(JingleTownBossArg *arg0);
s32 jingleTownBossChaseAttackIntroPhase(JingleTownBossArg *arg0);
s32 func_800BB930_B2EF0(JingleTownBossArg *arg0);
s32 jingleTownBossChaseAttackExitPhase(JingleTownBossArg *arg0);
s32 jingleTownBossHoverAttackIntroPhase(JingleTownBossArg *arg0);
s32 jingleTownBossHoverAttackMainPhase(JingleTownBossArg *arg0);
