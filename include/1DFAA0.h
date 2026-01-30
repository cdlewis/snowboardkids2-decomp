#pragma once

#include "20F0.h"
#include "common.h"
#include "graphics.h"
#include "main.h"

typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 pad;
} RGBColor;

typedef struct {
    union {
        struct {
            s16 frameCounter;
            RGBColor colors[2];
            u8 colorToggle;
        } FlashPayload;
        struct {
            s32 unk0;
            s32 unk4;
            u8 unk8;
            u8 unk9;
            s8 unkA;
        } Two;
        struct {
            s32 targetTimer;
            s32 stepDelta;
            s16 framesRemaining;
            s8 padding;
        } CurtainPayload;
        struct {
            s16 frameCount;  // Number of frames remaining for trick effect
            s16 effectParam; // Effect parameter passed to trick effect function
        } TrickPayload;
        struct {
            s16 remainingFrames;
            u8 padding[0xE];
            s32 currentAlphaFixed;
            s32 easingParam0;
            s32 easingParam1;
            s32 easingParam2;
            s32 targetAlphaFixed;
            s32 alphaIncrement;
        } ChrPayload;
    } unk0;
    u8 padding[0x18];
    s8 unk40;
    u8 padding2[0x3];
    s8 unk44;
    u8 padding3[0xA7];
    s8 unkEC;
    u8 padding4[0x3];
    s32 unkF4;
} CutsceneCmdItem;

typedef struct {
    ViewportNode *unk0;
    u8 header[0xA4];
    CutsceneCmdItem items[18];
    u8 padding[0x50];
    s32 unk1220;
} CutsceneCmdList;

typedef struct {
    u8 padding[0xFF7];
    /* 0xFF7 */ s8 skipAnimation;
} enableCutsceneSkip_arg;

typedef struct {
    void *start;
    void *end;
    u32 size;
    s8 fadeType;
    u8 padding[3];
} CutsceneFadeAssetNode;

typedef struct {
    s8 state;
    s8 slotIndex;
    s16 duration;
    s32 unk4;
    u8 padding[0xDC];
    s16 fadeAlpha;
} TaskData;

typedef struct {
    s16 headIndex;
    s16 unk2;
    s16 characterId;
    s8 unk6;
    s8 unk7;
    s8 unk8;
    s8 unk9;
} StateEntryItem;

typedef struct {
    u8 pad[0x210];
    s32 renderFlags;
} Func8000C268Arg;

typedef struct {
    u8 scriptData[0xC];
    /* 0xC */ u16 current_index;
    u8 padding[0x2];
    /* 0x10 */ u16 allocatedEventCount;
    /* 0x12 */ u8 stateEntryItemSize;
    /* 0x13 */ u8 slotCount;
    /* 0x14 */ u16 frameMask;
    /* 0x16 */ s16 maxEntries;
    /* 0x18 */ s16 initModelIndex;
    /* 0x1A */ s16 defaultEndFrame;
    /* 0x1C */ u8 configByte;
    u8 padding4[0x3];
    StateEntryItem items[2];
    s32 unk34;
    /* 0x38 */ u16 next_index;
    /* 0x3A */ u16 prev_index;
    u16 frameNumber;
    u8 commandCategory;
    u8 commandType;
} StateEntry;

typedef struct {
    u8 padding[0x16];
    u16 unk16;
} UIResource;

typedef struct {
    union {
        s32 One;
        s8 animMode;
        s8 bytes[4];
    } unk0;
    Transform3D transform;
    union {
        SceneModel *unk20;
        s32 unk20_s32;
        s16 unk20_s16;
    } unk20_u;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 posVelX;
    /* 0x40 */ s32 posVelY;
    /* 0x44 */ s32 posVelZ;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 scaleCurrentX;
    /* 0x58 */ s32 scaleCurrentY;
    /* 0x5C */ s32 scaleCurrentZ;
    /* 0x60 */ s32 scaleTargetX;
    /* 0x64 */ s32 scaleTargetY;
    /* 0x68 */ s32 scaleTargetZ;
    /* 0x6C */ s32 scaleVelX;
    /* 0x70 */ s32 scaleVelY;
    /* 0x74 */ s32 scaleVelZ;
    /* 0x78 */ s16 rotY;
    /* 0x7A */ s16 rotYTarget;
    /* 0x7C */ s16 rotYVel;
    /* 0x7E */ s16 rotYVelTarget;
    /* 0x80 */ s16 rotX;
    /* 0x82 */ s16 rotZ;
    /* 0x84 */ s16 animFramesRemaining;
    /* 0x86 */ s16 animFramesDuration;
    /* 0x88 */ s16 finalRotY;
    /* 0x8A */ s16 unk8A;
    /* 0x8C */ s16 unk8C;
    /* 0x8E */ s16 angle;
    /* 0x90 */ s16 walkAnimIndex;
    /* 0x92 */ s16 orbitSpeedParam;
    /* 0x94 */ s32 orbitAngularVelocity;
    /* 0x98 */ s32 orbitRadius;
    union {
        s32 orbitAngle_s32;
        struct {
            s16 orbitAngle_low;
            s16 orbitAngle_high;
        } s;
    } orbitAngle;
    /* 0xA0 */ s32 unkA0;
    /* 0xA4 */ union {
        SceneModel *ptr;
        s8 byte;
    } unkA4;
} CutsceneSlotData;

typedef struct {
    union {
        struct {
            s16 frameCounter;
            RGBColor colors[2];
            u8 colorToggle;
        } FlashPayload;
        struct {
            s32 unk0;
            s32 unk4;
            u8 unk8;
            u8 unk9;
            s8 unkA;
        } Two;
        struct {
            s32 targetTimer;
            s32 stepDelta;
            s16 framesRemaining;
            s8 padding;
        } CurtainPayload;
        struct {
            s16 frameCount;  // Number of frames remaining for trick effect
            s16 effectParam; // Effect parameter passed to trick effect function
        } TrickPayload;
        struct {
            s16 remainingFrames;
            u8 padding[0xE];
            s32 currentAlphaFixed;
            s32 easingParam0;
            s32 easingParam1;
            s32 easingParam2;
            s32 targetAlphaFixed;
            s32 alphaIncrement;
        } ChrPayload;
    } unk0;
    u8 padding[0x18];
    /* 0x40 */ u16 unk40;
    /* 0x42 */ u8 unk42;
    /* 0x43 */ u8 unk43;
    /* 0x44 */ u8 needsUpdate;
    /* 0x45 */ u8 padding3[0x3];
    /* 0x48 */ SceneModel *model;
    /* 0x4C */ CutsceneSlotData slotData;
} CutsceneSlot; /* Total size: 0xF4 */

typedef struct {
    /* 0x0 */ ViewportNode *uiResource;
    /* 0x4 */ s8 pad4[0x8];
    /* 0xC */ void *sceneContext;
    /* 0x10 */ setModelRenderMode_arg unk10;
    /* 0x98 */ u16 currentFrame;
    /* 0x9A */ u16 maxFrame;
    /* 0x9C */ u16 endFrame;
    /* 0x9E */ s8 pad9E[0x2];
    /* 0xA0 */ s8 debugText[0x8];
    /* 0xA8 */ CutsceneSlot slots[16];
    /* 0xFE8 */ void *textRenderer;
    /* 0xFEC */ s8 padFEC[0x8];
    /* 0xFF4 */ s8 showDebugInfo;
    /* 0xFF5 */ s8 enableTransparency;
    /* 0xFF6 */ s8 unused_FF6;
    /* 0xFF7 */ s8 skipAnimation;
    /* 0xFF8 */ Func8000C268Arg unkFF8;
    u8 padding[0xC];
    /* 0x1218 */ void *shadowModel;
    /* 0x121C */ void *reflectionModel;
    /* 0x1220 */ s32 cameraAnimationTimer;
} CutsceneManager;

void enableCutsceneSkip(CutsceneManager *arg0);
void disableCutsceneSkipOnInput(CutsceneManager *arg0);
void enableSlotUpdate(CutsceneManager *arg0, s16 arg1);
void disableSlotUpdate(CutsceneManager *arg0, s16 arg1);
CutsceneSlot *getCutsceneSlot(CutsceneManager *manager, s16 slotIndex);
void initCutsceneManager(CutsceneManager *manager, ViewportNode *sceneNode, void *shadowModel, void *reflectionModel);
void cleanupCutsceneManager(CutsceneManager *manager);
void hideAllSlotModels(CutsceneSlot *slots);
void showAllSlotModels(CutsceneSlot *slots);
void resetAllSlotModels(CutsceneSlot *slots);
void resetAllSlotTransforms(CutsceneManager *manager);
void prepareCutsceneForPlayback(
    CutsceneManager *manager,
    s32 uiResource,
    s32 pad4_0,
    s32 pad4_4,
    u16 maxFrame,
    u8 showDebugInfo
);
s32 processCutsceneFrame(CutsceneManager *cutsceneManager);
s16 getCutsceneFrameCount(s16 slotIndex, s16 cutsceneType);
void *getCutsceneDataMagicPrimary(void);
void *getCutsceneDataMagicSecondary(void);
u16 getCutsceneAllocatedEventCount(void);
u8 getCutsceneStateEntryItemSize(void);
u8 getCutsceneSlotCount(void);
u16 getCutsceneFrameMask(void);
u16 getCutsceneMaxStateEntries(void);
s16 getCutsceneInitModelIndex(void);
void setCutsceneInitModelIndex(s16 arg0);
StateEntryItem *getCurrentStateEntryItem(s32 itemIndex);
s16 getCutsceneDefaultEndFrame(void);
void setCutsceneDefaultEndFrame(s16 arg0);
u8 getCutsceneConfigByte(void);
void setCutsceneConfigByte(u8 arg0);
s32 saveCutsceneStateTableStub(void);
s32 loadCutsceneStateTableStub(void);
void *loadCutsceneFrameData(s16 slotIndex, s16 cutsceneType, s16 frameIndex);
s32 verifyAndLoadCutsceneState(void *stateBuffer);
void saveCutsceneStateTableToControllerPak(void);
s32 loadCutsceneStateTableFromControllerPak(void);
u16 allocateStateEntry(void);
void resetScriptState(u8 *arg0);
void initializeStateEntry(s32 arg0);
void cleanupCutsceneSystem(void);
u16 findStateEntryIndex(u8 slotIndex, u16 frameNumber, s32 findInsertionPoint);
s32 findEventAtFrame(u8 a0, u16 a1);
s32 insertCutsceneEvent(u8 slotIndex, u16 frameNumber);
void reorderCutsceneEvent(u16 eventIndex, u16 oldPreviousIndex, u16 newPreviousIndex);
StateEntry *getStateEntry(u16 arg0);
u16 getMaxCutsceneFrameNumber(void);
u16 getSlotLastFrameNumber(u8 slotIndex);
StateEntry *getCutsceneStateTable(void);
u8 *getCutsceneStateTableLastBytePtr(void);
u16 getCutEntryBufferSlotIndex(u16 defaultSlotIndex);
u16 getCutEntryBufferFrameNumber(u16 defaultFrameNumber);
void pasteCutsceneEntry(void);
void cutCutsceneEntry(u8 slotIndex, s16 frameNumber);
void copyCutsceneEntry(u8 slotIndex, u16 frameNumber);
void pasteCutsceneEntryToSlot(u8 slotIndex, u16 frameNumber);
void shiftCutsceneEntryFrame(u8 slotIndex, u16 frameNumber, s32 frameDelta);
void *loadCutsceneSlotAsset(s8 slotIndex);
void startCutsceneFadeEffect(s32 arg0, s8 slotIndex, s16 duration);
void initCutsceneFadeTask(void *);
void initializeCutsceneSystem(void *);
