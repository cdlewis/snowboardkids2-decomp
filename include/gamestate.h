#pragma once

#include "common.h"
#include "gbi.h"
#include "graphics/displaylist.h"
#include "graphics/graphics.h"
#include "libaudio.h"
#include "math/geometry.h"

struct ShootCrossTargets;

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

typedef enum {
    SNOWBOARD_BALANCE_LEVEL_1 = 0,
    SNOWBOARD_BALANCE_LEVEL_2 = 1,
    SNOWBOARD_BALANCE_LEVEL_3 = 2,
    SNOWBOARD_TRICK_LEVEL_1 = 3,
    SNOWBOARD_TRICK_LEVEL_2 = 4,
    SNOWBOARD_TRICK_LEVEL_3 = 5,
    SNOWBOARD_SPEED_LEVEL_1 = 6,
    SNOWBOARD_SPEED_LEVEL_2 = 7,
    SNOWBOARD_SPEED_LEVEL_3 = 8,
    SNOWBOARD_STAR = 9,
    SNOWBOARD_FEATHER = 10,
    SNOWBOARD_ICE = 11,
    SNOWBOARD_CHARM = 12,
    SNOWBOARD_POVERTY = 13,
    SNOWBOARD_RICH = 14,
    SNOWBOARD_NINJA = 15,
    SNOWBOARD_HIGH_TECH = 16,
    SNOWBOARD_DRAGON = 17,
    SNOWBOARD_COUNT = 18,
} SnowboardId;

typedef enum {
    BOOST_STATE_NONE = 0,
    BOOST_STATE_SPEED_FAN_FORWARD = 1,
    BOOST_STATE_SPEED_FAN_REVERSE = 2,
    BOOST_STATE_ROCKET_START_FORWARD = 3,
    BOOST_STATE_ROCKET_START_REVERSE = 4,
    BOOST_STATE_ROCKET_IMPULSE_FORWARD = 5,
    BOOST_STATE_ROCKET_IMPULSE_REVERSE = 6,
    BOOST_STATE_ROCKET_SUSTAIN_FORWARD = 7,
    BOOST_STATE_ROCKET_SUSTAIN_REVERSE = 8,
} BoostState;

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
    u8 bytes[sizeof(DisplayListObject)];
} PlayerDisplayObjectStride;

/**
 * Shared view of a Player display-object slot. Player's asset pointers precede
 * the contiguous model objects by 0x38 bytes, while each slot begins 0x3C bytes
 * after the previous one.
 */
typedef struct {
    /* 0x00 */ u8 assetPointers[0x38];
    /* 0x38 */ DisplayListObject displayObject;
} PlayerDisplayObjectView;

typedef struct {
    /* 0x000 */ void *raceAnimationData;
    /* 0x004 */ void *bodyPartDisplayListAsset;
    /* 0x008 */ void *bodyPartCompressedAsset;
    /* 0x00C */ void *snowboardDisplayListAsset;
    /* 0x010 */ void *snowboardCompressedAsset;
    /* 0x014 */ void *snowboardPaletteAsset;
    /* 0x018 */ void *itemSpriteAssetTable;
    /* 0x01C */ void *bossRaceData;
    /* 0x020 */ void *alternateBodyPartDisplayListAsset;
    /* 0x024 */ void *alternateBodyPartCompressedAsset;
    /* 0x028 */ void *aiPathData;
    /* 0x02C */ InputRecord *inputRecordData;
    /* 0x030 */ Vtx *shadowVertices;
    /* 0x034 */ Mtx *shadowMatrix;
    /* 0x038 */ DisplayListObject bodyPartDisplayObjects[16];
    /* 0x3F8 */ DisplayListObject snowboardDisplayObject;
    /* 0x434 */ Vec3i worldPos;
    /* 0x440 */ Vec3i prevWorldPos;
    /* 0x44C */ Vec3i velocity;
    /* 0x458 */ Vec3i surfaceNormal;
    /* 0x464 */ u8 padding0x464[0x4];
    /* 0x468 */ s32 unk468;
    /* 0x46C */ s32 unk46C;
    /* 0x470 */ Vec3i unk470;
    /* 0x47C */ s32 unk47C;
    /* 0x480 */ s32 unk480;
    /* 0x484 */ s32 unk484;
    /* 0x488 */ BoneAnimationStateIndexed boneAnimationStates[16];
    /* 0x908 */ u8 padding0x908[0x20];
    /* 0x928 */ Transform3D snowboardBoneTransform;
    /* 0x948 */ u8 padding0x948[0x8];
    /* 0x950 */ Transform3D modelTransform;
    /* 0x970 */ Transform3D headingTransform;
    /* 0x990 */ Transform3D orientationTransform;
    /* 0x9B0 */ Transform3D tiltTransform;
    /* 0x9D0 */ Transform3D trickRotationTransform;
    /* 0x9F0 */ Transform3D orientationHeadingTransform;
    /* 0xA10 */ Vec3i shadowSamplePositions[9];
    /* 0xA7C */ Vec3i aiTarget;
    /* 0xA88 */ u8 padding0xA88[0x4];
    /* 0xA8C */ u16 animationIndex;
    /* 0xA8E */ s16 pitchAngle;
    /* 0xA90 */ s16 steeringAngle;
    /* 0xA92 */ s16 rollAngle;
    /* 0xA94 */ s16 rotY;
    /* 0xA96 */ s16 trickRotationX;
    /* 0xA98 */ s16 trickRotationY;
    /* 0xA9A */ s16 unkA9A;
    /* 0xA9C */ u16 jingleBossPitchAngle;
    /* 0xA9E */ s16 bossYawAngle;
    /* 0xAA0 */ s32 baseMaxSpeed;
    /* 0xAA4 */ s32 maxSpeedCap;
    /* 0xAA8 */ s32 smoothedSpeedCap;
    /* 0xAAC */ s32 catchUpSpeedAdjustment;
    /* 0xAB0 */ s32 lateralDeadzone;
    /* 0xAB4 */ s32 baseGravity;
    /* 0xAB8 */ s32 gravity;
    /* 0xABC */ s32 baseAcceleration;
    /* 0xAC0 */ u8 handling;
    /* 0xAC1 */ u8 cornering;
    /* 0xAC2 */ s16 hitReactionState;
    /* 0xAC4 */ s16 knockbackAngle;
    /* 0xAC6 */ u8 padding0xAC6[0x2];
    /* 0xAC8 */ Vec3i knockbackVelocity;
    /* 0xAD4 */ Vec3i collisionOffset;
    /* 0xAE0 */ s32 collisionRadius;
    /* 0xAE4 */ Vec3i extraCollisionOffsets[6];
    /* 0xB2C */ s32 extraCollisionRadii[6];
    /* 0xB44 */ Vec3i storedPosition;
    /* 0xB50 */ ListNode_5AA90 collisionListNode;
    /* 0xB6C */ s32 raceGold;
    /* 0xB70 */ s32 skillPoints;
    /* 0xB74 */ u16 storedRotY;
    /* 0xB76 */ s16 aiPrimaryItemUseTimer;
    /* 0xB78 */ s16 aiItemUseTimer;
    /* 0xB7A */ s8 inputStickX;
    /* 0xB7B */ s8 inputStickY;
    /* 0xB7C */ s16 inputButtonsHeld;
    /* 0xB7E */ u16 inputButtonsPressed;
    /* 0xB80 */ s8 prevInputStickX;
    /* 0xB81 */ s8 prevInputStickY;
    /* 0xB82 */ u16 prevInputButtonsHeld;
    /* 0xB84 */ s32 animationFlags;
    /* 0xB88 */ s32 behaviorFlags;
    /* 0xB8C */ s32 unkB8C;
    /* 0xB90 */ s32 unkB90;
    /* 0xB94 */ u16 sectorIndex;
    /* 0xB96 */ s16 segmentProgress;
    /* 0xB98 */ s16 lapProgressRemaining;
    /* 0xB9A */ s16 boostTimer;
    /* 0xB9C */ u16 shortcutActivationSnapshot;
    /* 0xB9E */ u16 squashStretchScale;
    /* 0xBA0 */ u16 characterScaleXZ;
    /* 0xBA2 */ s16 characterScaleY;
    /* 0xBA4 */ u16 invincibilityTimer;
    /* 0xBA6 */ s16 wingsTimer;
    /* 0xBA8 */ u16 inputRecordFrameIndex;
    /* 0xBAA */ s16 trickPoints;
    /* 0xBAC */ s16 trickScore;
    /* 0xBAE */ s16 tricksPerformedMask;
    /* 0xBB0 */ u16 unkBB0;
    /* 0xBB2 */ u16 unkBB2;
    /* 0xBB4 */ u8 collisionSphereCount;
    /* 0xBB5 */ s8 trickCount;
    /* 0xBB6 */ u8 spinsPerformedMask;
    /* 0xBB7 */ u8 animationBoneCount;
    /* 0xBB8 */ u8 playerIndex;
    /* 0xBB9 */ u8 characterId;
    /* 0xBBA */ u8 boardModelId;
    /* 0xBBB */ u8 snowboardId;
    /* 0xBBC */ u8 colorSlot;
    /* 0xBBD */ u8 behaviorMode;
    /* 0xBBE */ u8 behaviorPhase;
    /* 0xBBF */ u8 behaviorStep;
    /* 0xBC0 */ u8 behaviorCounter;
    /* 0xBC1 */ u8 shadowMeshNeedsUpdate;
    /* 0xBC2 */ u8 unkBC2;
    /* 0xBC3 */ u8 finishAnimationState;
    /* 0xBC4 */ u8 finishPosition;
    /* 0xBC5 */ u8 currentLap;
    /* 0xBC6 */ u8 unkBC6;
    /* 0xBC7 */ u8 isBossRacer;
    /* 0xBC8 */ u8 stunCollisionCounter; /* increments during stunned state for increasing wall collision threshold */
    /* 0xBC9 */ u8 trackFaceType;
    /* 0xBCA */ u8 trackFaceSubtype;
    /* 0xBCB */ u8 trackFaceType1Timer;
    /* 0xBCC */ u8 surfaceInfo;
    /* 0xBCD */ s8 queuedTrickType;
    /* 0xBCE */ u8 chairliftFlags;
    /* 0xBCF */ u8 slowdownLevel;
    /* 0xBD0 */ u8 boostState;
    /* 0xBD1 */ u8 wingsActive;
    /* 0xBD2 */ u8 primaryItemId;
    /* 0xBD3 */ u8 primaryItemAmmo;
    /* 0xBD4 */ u8 secondaryItemId;
    /* 0xBD5 */ u8 trickAnimationPhase;
    /* 0xBD6 */ u8 currentTrickAnimation;
    /* 0xBD7 */ u8 queuedTrickAnimation;
    /* 0xBD8 */ u8 itemHudNotificationFlags;
    /* 0xBD9 */ u8 flyingAttackState; /* 0=normal, 1-3=flying attack modes */
    /* 0xBDA */ u8 isCpuControlled;
    /* 0xBDB */ u8 bossHealth;
    /* 0xBDC */ u8 cpuInputFlags;
    /* 0xBDD */ u8 aiDifficultyIndex;
    /* 0xBDE */ u8 aiPathFlags; /* Bit 3: shortcut available */
    /* 0xBDF */ s8 rumbleEffectType;
    /* 0xBE0 */ s8 rumbleCounter;
    /* 0xBE1 */ u8 rumbleDuration;
    /* 0xBE2 */ u8 rumbleFrame;
    /* 0xBE3 */ u8 inputPlaybackMode;
    /* 0xBE4 */ u8 inputRecordSetId;
    /* 0xBE5 */ u8 aiShortcutDecisionMade; /* AI has made shortcut decision */
    /* 0xBE6 */ u8 bodyPartStateIndex;
    /* 0xBE7 */ u8 bodyPartStateTimer;
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
    /* 0x24 */ struct ShootCrossTargets *shootCrossTargets;
    /* 0x28 */ RaceAssetHeader *unk28;
    /* 0x2C */ u8 PAD_3[0x4];
    /* 0x30 */ GameDataLayout gameData;
    /* 0x44 */ GameStateUnk44 *unk44;
    /* 0x48 */ u8 *raceTransformData;
    /* 0x4C */ s32 stateDelayTimer;
    /* 0x50 */ u32 raceFrameCounter;
    /* 0x54 */ s32 raceTimerElapsedTicks;
    /* 0x58 */ u16 shortcutActivationCounter;
    /* 0x5A */ u8 shootCrossTargetsHit;
    /* 0x5B */ u8 availableHomingProjectileSlots;
    /* 0x5C */ u8 memoryPoolId;
    /* 0x5D */ u8 humanPlayerCount;
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
    /* 0x3B0 */ Transform3D storyMapCharacterTransform;
    s32 unk3D0;
    s32 unk3D4;
    s32 unk3D8;
    void *unk3DC;
    void *unk3E0;
    void *unk3E4;
    s16 unk3E8;
    u8 PAD_D[0x2];
    /* 0x3EC */ s32 storyMapCameraX;
    /* 0x3F0 */ s32 storyMapCameraZ;
    /* 0x3F4 */ s16 storyMapCameraOrbitAngle;
    /* 0x3F8 */ s32 storyMapCameraOrbitRadius;
    /* 0x3FC */ u16 storyMapCameraViewAngle;
    /* 0x3FE */ s16 storyMapPlayerCollisionRadius;
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
    /* 0x42C */ u8 dialogueLineIndex;
    /* 0x42D */ u8 storyMapItemType;
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
    /* 0x7A2 */ u8 padding7A2[0x113E];
} GameState;
