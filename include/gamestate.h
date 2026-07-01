#pragma once

#include "common.h"
#include "gbi.h"
#include "graphics/displaylist.h"
#include "graphics/graphics.h"
#include "libaudio.h"
#include "math/geometry.h"

typedef struct ListNode_5AA90 {
    /* 0x00 */ struct ListNode_5AA90 *next;
    /* 0x04 */ Vec3i *posPtr;
    /* 0x08 */ Vec3i localPos;
    /* 0x14 */ s32 radius;
    /* 0x18 */ u8 id;
} ListNode_5AA90;

typedef enum {
    PRIMARY_ITEM_NONE = 0,
    PRIMARY_ITEM_SLAPSTICK = 1,
    PRIMARY_ITEM_PARACHUTE = 2,
    PRIMARY_ITEM_FREEZE_SHOT = 3,
    PRIMARY_ITEM_SNOWMAN = 4,
    PRIMARY_ITEM_BOMB = 5,
    PRIMARY_ITEM_WHIRLWIND = 6,
} PrimaryItemId;

typedef enum {
    SECONDARY_ITEM_NONE = 0,
    SECONDARY_ITEM_PAN = 1,
    SECONDARY_ITEM_GHOST = 2,
    SECONDARY_ITEM_SUPER_GHOST = 3,
    SECONDARY_ITEM_SPEED_FAN = 4,
    SECONDARY_ITEM_ROCKET = 5,
    SECONDARY_ITEM_INVISIBLE = 6,
    SECONDARY_ITEM_ROCK = 7,
    SECONDARY_ITEM_RAT_FACE = 8,
    SECONDARY_ITEM_SUPER_RAT_FACE = 9,
    SECONDARY_ITEM_WING = 10,
} SecondaryItemId;

typedef struct {
    /* 0x00 */ BoneAnimationTransformState transform;
    /* 0x40 */ u16 flags;
    /* 0x42 */ u16 counter;
    /* 0x44 */ u16 animation_index;
    /* 0x46 */ u16 pad;
} BoneAnimationStateIndexed;

typedef struct {
    loadAssetMetadata_arg asset;
    u8 padding[0x24];
} GameStateUnk44_Item;

typedef struct {
    u8 data[0x180];
} ItemBoxBurstFrameData;

typedef struct {
    u8 padding[0x2C0];
    ItemBoxBurstFrameData itemBoxBurstFrames[8];
    u8 padding2[0x100];
    GameStateUnk44_Item unkFC0[3];
    GameStateUnk44_Item unk1080[3];
    u8 padding3[0x1FF];
    s32 unk1340;
    u8 padding4[0x3C];
    s32 unk1380;
} GameStateUnk44;

typedef struct {
    s32 assetTableOffset;
    s32 transformDataOffset;
} RaceAssetHeader;

typedef struct {
    s8 stickX;
    s8 stickY;
    u16 buttons;
} InputRecord;

typedef struct {
    void *unk0;
    void *unk4;
    void *unk8;
    void *unkC;
    void *unk10;
    void *unk14;
    void *assetTable;
    void *unk1C;
    void *unk20;
    void *unk24;
    /* 0x28 */ void *aiPathData;
    InputRecord *unk2C;
    /* 0x30 */ Vtx *shadowVertices;
    /* 0x34 */ Mtx *shadowMatrix;
    /* 0x38 */ DisplayListObject boneDisplayObjects[16];
    /* 0x3F8 */ DisplayListObject playerModel;
    Vec3i worldPos;
    Vec3i prevWorldPos;
    /* 0x44C */ Vec3i velocity;
    s32 surfaceNormalX;
    s32 surfaceNormalY;
    s32 surfaceNormalZ;
    u8 padding2a_1[0x4];
    s32 unk468;
    s32 unk46C;
    void *unk470;
    s32 unk474;
    s32 unk478;
    s32 unk47C;
    s32 unk480;
    s32 unk484;
    BoneAnimationStateIndexed unk488[1];
    /* 0x4D0 */ u8 padding_4D0[0x458];
    /* 0x928 */ Transform3D unk928;
    /* 0x948 */ u8 padding_948[0x8];
    /* 0x950 */ Transform3D modelTransform;
    Transform3D headingTransform;
    Transform3D orientationTransform;
    /* 0x9B0 */ Transform3D tiltTransform;
    /* 0x9D0 */ Transform3D unk9D0;
    /* 0x9F0 */ Transform3D orientationHeadingTransform;
    /* 0xA10 */ Vec3i shadowSamplePositions[9];
    /* 0xA7C */ Vec3i aiTarget;
    u8 _padA88[0x4];
    u16 leanAnimIndex;
    s16 pitchAngle;
    s16 steeringAngle;
    s16 rollAngle;
    s16 rotY;
    s16 unkA96;
    s16 unkA98;
    s16 unkA9A;
    u16 jingleBossPitchAngle;
    s16 unkA9E;
    s32 baseMaxSpeed;
    /* 0xAA4 */ s32 maxSpeedCap;
    s32 smoothedSpeedCap;
    s32 speedHandicap;
    s32 lateralDeadzone;
    s32 baseGravity;
    s32 gravity;
    s32 baseAcceleration;
    u8 handling;
    u8 cornering;
    s16 hitReactionState;
    s16 knockbackAngle;
    u8 padding2b[0x2];
    Vec3i knockbackVelocity;
    Vec3i collisionOffset;
    s32 collisionRadius;
    Vec3i extraCollisionOffsets[6];
    s32 extraCollisionRadii;
    s32 unkB30;
    s32 unkB34;
    s32 unkB38;
    s32 unkB3C;
    s32 unkB40;
    s32 storedPosX;
    s32 storedPosY;
    s32 storedPosZ;
    /* 0xB50 */ ListNode_5AA90 collisionListNode;
    s32 raceCoins;
    s32 skillPoints;
    u16 storedRotY;
    /* 0xB76 */ s16 aiPrimaryItemUseTimer;
    /* 0xB78 */ s16 aiItemUseTimer;
    /* 0xB7A */ s8 inputStickX;
    /* 0xB7B */ s8 inputStickY;
    /* 0xB7C */ s16 inputButtons;
    /* 0xB7E */ u16 inputButtonsPressed;
    /* 0xB80 */ s8 prevInputStickX;
    /* 0xB81 */ s8 prevInputStickY;
    /* 0xB82 */ u16 prevInputButtons;
    /* 0xB84 */ s32 animFlags;
    /* 0xB88 */ s32 behaviorFlags;
    /* 0xB8C */ s32 unkB8C;
    /* 0xB90 */ s32 unkB90;
    /* 0xB94 */ u16 sectorIndex;
    /* 0xB96 */ s16 segmentProgress;
    /* 0xB98 */ s16 raceProgress;
    /* 0xB9A */ s16 boostTimer;
    /* 0xB9C */ u16 shortcutLapCount;
    /* 0xB9E */ u16 squashStretchScale;
    /* 0xBA0 */ u16 characterScaleXZ;
    /* 0xBA2 */ s16 characterScaleY;
    /* 0xBA4 */ u16 invincibilityTimer;
    /* 0xBA6 */ s16 featherItemTimer;
    /* 0xBA8 */ u16 inputRecordIndex;
    /* 0xBAA */ s16 trickPoints;
    /* 0xBAC */ s16 trickScore;
    /* 0xBAE */ s16 tricksPerformedMask;
    /* 0xBB0 */ u16 unkBB0;
    /* 0xBB2 */ u16 unkBB2;
    /* 0xBB4 */ u8 unkBB4;
    /* 0xBB5 */ s8 trickCount;
    /* 0xBB6 */ u8 spinsPerformedMask;
    /* 0xBB7 */ u8 leanBoneCount;
    /* 0xBB8 */ u8 playerIndex;
    /* 0xBB9 */ u8 characterId;
    /* 0xBBA */ u8 boardType;
    /* 0xBBB */ u8 costumeID;
    /* 0xBBC */ u8 colorSlot;
    /* 0xBBD */ u8 behaviorMode;
    /* 0xBBE */ u8 behaviorPhase;
    /* 0xBBF */ u8 behaviorStep;
    /* 0xBC0 */ u8 behaviorCounter;
    /* 0xBC1 */ u8 shadowMeshNeedsUpdate;
    /* 0xBC2 */ u8 unkBC2;
    /* 0xBC3 */ u8 finishAnimState;
    /* 0xBC4 */ u8 finishPosition;
    /* 0xBC5 */ u8 currentLap;
    /* 0xBC6 */ u8 unkBC6;
    /* 0xBC7 */ u8 isBossRacer;
    /* 0xBC8 */ u8 stunCollisionCounter; /* increments during stunned state for increasing wall collision threshold */
    /* 0xBC9 */ u8 trackFaceType;
    /* 0xBCA */ u8 trackFaceSubtype;
    /* 0xBCA */ u8 trackFaceType1Timer;
    /* 0xBCC */ u8 surfaceInfo;
    /* 0xBCD */ s8 unkBCD;
    /* 0xBCE */ u8 chairliftFlags;
    /* 0xBCF */ u8 slowdownLevel;
    /* 0xBD0 */ u8 boostState;
    /* 0xBD1 */ u8 featherItemActive;
    /* 0xBD2 */ u8 primaryItemId;
    /* 0xBD3 */ u8 primaryItemAmmo;
    /* 0xBD4 */ u8 secondaryItemId;
    /* 0xBD5 */ u8 unkBD5;
    /* 0xBD6 */ u8 unkBD6;
    /* 0xBD7 */ u8 unkBD7;
    /* 0xBD8 */ u8 unkBD8;
    /* 0xBD9 */ u8 flyingAttackState; /* 0=normal, 1-3=flying attack modes */
    /* 0xBDA */ u8 inputDisabled;
    /* 0xBDB */ u8 unkBDB;
    /* 0xBDC */ u8 unkBDC;
    /* 0xBDD */ u8 speedPenaltyIndex;
    /* 0xBDE */ u8 pathFlags; /* Bit 3: shortcut available */
    /* 0xBDF */ s8 rumbleEffectType;
    /* 0xBE0 */ s8 rumbleCounter;
    /* 0xBE1 */ u8 rumbleDuration;
    /* 0xBE2 */ u8 rumbleFrame;
    /* 0xBE3 */ u8 inputMode;
    /* 0xBE4 */ u8 inputRecordSet;
    /* 0xBE5 */ u8 aiShortcutChosen; /* AI has made shortcut decision */
    /* 0xBE6 */ u8 bodyPartAnimIndex;
    /* 0xBE7 */ u8 bodyPartAnimFlags;
} Player;

typedef struct {
    /* 0x0 */ ViewportNode *audioViewport;
    /* 0x4 */ ViewportNode *playerOverlayViewports;
    /* 0x8 */ ViewportNode *playerCameraViewports;
    /* 0xC */ ViewportNode *playerRootViewports;
    /* 0x10 */ Player *players;
    /* 0x14 */ void *introAnimationData;
    /* 0x18 */ void *unk18;
    /* 0x1C */ void *unk1C;
    /* 0x20 */ void *unk20;
    /* 0x24 */ u8 PAD_2[0x4];
    /* 0x28 */ RaceAssetHeader *unk28;
    /* 0x2C */ u8 PAD_3[0x4];
    /* 0x30 */ GameDataLayout gameData;
    /* 0x44 */ GameStateUnk44 *unk44;
    /* 0x48 */ u8 *unk48;
    /* 0x4C */ s32 stateDelayTimer;
    /* 0x50 */ u32 raceFrameCounter;
    /* 0x54 */ u8 unk54;
    /* 0x55 */ u8 unk55;
    /* 0x56 */ u8 unk56;
    /* 0x57 */ u8 unk57;
    /* 0x58 */ u16 shortcutActivationCounter;
    /* 0x5A */ u8 shootCrossTargetsHit;
    /* 0x5B */ u8 availableHomingProjectileSlots;
    /* 0x5C */ u8 memoryPoolId;
    /* 0x5D */ u8 unk5D;
    /* 0x5E */ u8 numPlayers;
    /* 0x5F */ u8 playerCount;
    /* 0x60 */ u8 pendingPlayerRenderTasks;
    /* 0x61 */ u8 introSkipState;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ u8 shortcutGateState;
    /* 0x64 */ u8 rankOrder[0x10];
    /* 0x74 */ u8 finalLapNumber;
    /* 0x75 */ u8 unk6B;
    /* 0x76 */ u8 gamePaused;
    /* 0x77 */ u8 pauseMenuSelection;
    /* 0x78 */ s8 trainingPanelState;
    /* 0x79 */ u8 raceIntroState;
    /* 0x7A */ u8 raceType;
    /* 0x7B */ u8 showResultHUD;
    /* 0x7C */ s8 showGoldReward;
    /* 0x7D */ u8 playerLost;
    /* 0x7E */ u8 bestBoardBonus;
    /* 0x7F */ u8 introCutsceneVariant;
    /* 0x80 */ u8 fadeInPlayerCount;
    /* 0x81 */ u8 shortcutWarpPlayerCount;
    /* 0x82 */ u8 introFrameEventIndex;
    /* 0x83 */ u8 unk83;
    /* 0x84 */ u8 itemBoxesDisabled;
    /* 0x85 */ u8 introCameraEventIndex;
    u8 unk86;
    u8 PAD_7B[0x53];
    u16 unkDA;
    u8 PAD_8[0xF9];
    /* 0x1D8 */ ALPlayer *audioPlayer2;
    u8 PAD_9[0x4];
    /* 0x1E0 */ ALPlayer *audioPlayer3;
    u8 PAD_A[0x2];
    /* 0x1E6 */ u8 unk1E6;
    u8 PAD_A2[0x1C9];
    /* 0x3B0 */ void *unk3B0;
    /* 0x3B4 */ void *unk3B4;
    /* 0x3B8 */ u16 unk3B8;
    /* 0x3BA */ u8 unk3BA;
    /* 0x3BB */ u8 unk3BB;
    /* 0x3BC */ u8 unk3BC;
    /* 0x3BD */ u8 unk3BD;
    /* 0x3BE */ u8 unk3BE;
    /* 0x3BF */ u8 partialUnlockCheatProgress;
    /* 0x3C0 */ u8 unlockAllCheatProgress;
    /* 0x3C1 */ u8 unk3C1;
    u8 PAD_C2[0xE];
    s32 unk3D0;
    s32 unk3D4;
    s32 unk3D8;
    void *unk3DC;
    void *unk3E0;
    void *unk3E4;
    s16 unk3E8;
    u8 PAD_D[0x2];
    s32 unk3EC;
    s32 unk3F0;
    s16 unk3F4;
    s32 unk3F8;
    u16 unk3FC;
    s16 unk3FE;
    u8 animState;
    u8 PAD_E[0x2];
    u8 dialogueResult;
    u8 unk404;
    u8 PAD_0A_1[0x3];
    s32 npcPosX[2];
    s32 npcPosZ[2];
    s16 npcCollisionRadius[2];
    s32 unk41C;
    u8 activeNpcIndex;
    u8 unk421;
    u8 cancelDialogue;
    u8 unk423;
    u8 locationDiscovered;
    u8 discoveredLocationId;
    u8 unk426;
    u8 unk427;
    u8 unk428;
    u8 unk429;
    u8 dialogueTurnState;
    u8 unk42B;
    u8 PAD_11[2];
    s8 unk42E;
    u8 PAD_12[0x158];
    s32 *unk588;
    s32 *unk58C;
    u16 unk590;
    u8 PAD_13[0x8];
    u8 playerSlotState[8];
    u8 PAD_13B[0xB];
    s8 unk5AD;
    s8 unk5AE;
    u8 unk5AF;
    u8 unk5B0;
    u8 unk5B1;
    s8 unk5B2;
    u8 isStoryMapInitializing;
    u8 PAD_14[0x4];
    u8 unk5B8[8];
    u16 unk5C0;
    u8 PAD_14B[0x3];
    u8 unk5C5;
    u8 unk5C6;
    u8 unk5C7;
    s8 unk5C8;
    u8 unk5C9;
    u8 unk5CA[0xD];
    u8 unk5D7;
    u8 unk5D8;
    u8 PAD_15B[0x1A3];
    s16 unk77C;
    u8 PAD_16[0x6];
    u8 unk784[4];
    u8 PAD_16B[0x10];
    u8 unk798;
    u8 PAD_16C[2];
    u8 unk79B;
    u8 unk79C;
    u8 PAD_17[4];
    s8 unk7A1;
    u8 PAD_18[0x103E];
    s8 unk17E0;
    u8 PAD_19[0x17];
    Transform3D charSelectRotations[4];
    u8 PAD_19B[0x8];
    u16 charSelectCarouselAngles[4];
    u16 charSelectPreviewAngles[8];
    u16 charSelectMenuStates[4];
    u8 charSelectFrameCounters[8];
    u8 charSelectCharRow[8];
    u8 charSelectCharCol[8];
    u8 charSelectBoardIds[4];
    u8 charSelectSavedBoardIds[4];
    u8 charSelectScrollDirection[4];
    u8 charSelectSlideState[4];
    u8 iconDisplayState[4];
    u8 charSelectMaxMenuOption;
    u8 PAD_23[0x5];
    u8 charSelectCursorIndices[8];
} GameState;
