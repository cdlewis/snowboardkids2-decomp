#pragma once

#include "common.h"
#include "graphics/displaylist.h"
#include "graphics/sprite_table.h"
#include "math/geometry.h"
#include "system/memory_allocator.h"

#define MODE_DMA 0
#define MODE_QUEUED_DMA 1
#define MODE_DIRECT_FETCH 2
#define SCENE_MODEL_BONE_SLOT_COUNT 32
#define SCENE_MODEL_MAX_ANIMATED_BONES 31

struct ViewportNode;

typedef struct {
    void *displayListStart;
    void *displayListEnd;
    void *compressedDataStart;
    void *compressedDataEnd;
    u16 decompressedSize;
    u16 padding;
    void *directDataPtr;
} Asset;

typedef struct {
    void *displayListStart;
    void *displayListEnd;
    void *compressedDataStart;
    void *compressedDataEnd;
    s32 decompressedSize;
    DisplayLists *displayLists;
} ItemAssetEntry;

typedef union {
    s32 packed;
    struct {
        s8 assetIndex;
        u8 shadowScale;
        u16 padding;
    } fields;
} AssetGroupAnimationInfo;

typedef struct {
    /* 0x00 */ char name[8];
    /* 0x08 */ void *displayListStart;
    /* 0x0C */ void *displayListEnd;
    /* 0x10 */ void *compressedDataStart;
    /* 0x14 */ void *compressedDataEnd;
    /* 0x18 */ s32 decompressedSize;
    /* 0x1C */ DisplayLists *displayLists;
    /* 0x20 */ s8 numAssets;
    /* 0x21 */ s8 assetGroupIndex;
    /* 0x22 */ s16 animationCount;
    /* 0x24 */ void *animationDataStart;
    /* 0x28 */ void *animationDataEnd;
    /* 0x2C */ u32 animationDataSize;
    /* 0x30 */ s32 packedAnimationInfo;
    /* 0x34 */ void *soundSequenceDataStart;
    /* 0x38 */ void *soundSequenceDataEnd;
    /* 0x3C */ s32 soundSequenceDataSize;
    /* 0x40 */ void *initCallback;
    /* 0x44 */ Asset *assets;
    /* 0x48 */ s8 count;
    /* 0x49 */ u8 countPadding[3];
} AssetGroup;

typedef struct {
    u8 padding[0x13];
    s8 animationCount;
} ModelAnimationData;

typedef struct {
    void *unk0;
    Vec3i shadowPosition;
    void *unk10;
    s16 unk14;
    s8 unk16;
    s8 unk17;
    void *unk18;
    void *unk1C;
    void (*callback)(void *);
    void (*cleanupCallback)(void *);
    s32 shadowSizeX;
    s32 shadowSizeZ;
    s16 unk30;
    s16 unk32;
} SceneModelShadowSprite;

typedef struct {
    DisplayListObject *boneDisplayObjects;
    BoneAnimationState *boneAnimationStates;
    void *animationBoneData;
    /* 0xC */ s16 index;
    s16 assetCount;
    struct ViewportNode *viewport;
    s16 previousAnimationIndex;
    s16 currentAnimationIndex;
    Transform3D transform;
    s16 queuedAnimationIndex;
    s16 transitionAnimationIndex;
    s8 isDestroyed;
    s8 actionMode;
    s8 animationLoopCount;
    s8 displayEnabled;
    Vec3i effectOffset;
    s16 itemAssetIndex;
    s8 itemDisplayEnabled;
    s8 shadowScale;
    u8 padding2[0x38];
    s8 visibilityEnabled;
    s8 flags;
    s16 animFrameCount;
    s16 currentAnimationFrame;
    s16 targetAnimationIndex;
    s32 completedBoneMask;
    s8 animationIndex;
    s8 completedLoopCount;
    u8 alpha;
    s8 shadowEnabled;
    DisplayListObject *specialAnimationDisplayObject;
    ModelAnimationData *animationDataTable;
    void *animationDisplayLists;
    SpriteAssetState spriteAsset;
    Transform3D baseTransform;
    s32 partDisplayFlags;
    void *spriteEffectModelData;
    void *spriteEffectTextureData;
    void *soundData;
    void *shadowTextureData;
    SceneModelShadowSprite shadowSprite;
    s32 height;
    s8 renderEnabled;
} SceneModel;

SceneModel *
createSceneModelEx(s32 assetGroupIndex, void *allocation, s8 assetPairIndex, s8 param4, s8 param5, s16 assetIndex);
void *loadAssetDataByMode(s16 groupIndex, s16 entityIndex, s16 mode);
void *loadAssetGroupSoundData(SceneModel *);
void *loadAssetGroupDisplayList(SceneModel *);
void *loadAssetGroupCompressedData(SceneModel *);
SceneModel *createSceneModel(s32 assetGroupIndex, void *allocation);

void setModelAnimation(SceneModel *model, s16 animationIndex);

void updateModelGeometry(SceneModel *);

void setItemDisplayEnabled(SceneModel *arg0, s8 arg1);
void setItemFlags(SceneModel *arg0, s8 arg1);
void setModelPendingDestroy(SceneModel *arg0);
void clearModelPendingDestroy(SceneModel *arg0);
s8 getAnimationIndex(SceneModel *arg0);
void setModelHeight(SceneModel *arg0, s32 height);

typedef struct {
    Transform3D unk0;
    union {
        SceneModel *unk20;
        s32 unk20_s32;
        s16 unk20_s16;
    } unk20_u;
} applyTransformToModel_arg1;

void applyTransformToModel(SceneModel *arg0, Transform3D *arg1);

void initializeGameEntity(SceneModel *, s32, struct ViewportNode *, s8, s8, s8, s16);

SceneModel *destroySceneModel(SceneModel *arg0);
void *cleanupSceneModel(SceneModel *arg0);

s32 clearModelRotation(SceneModel *);

void setModelDisplayEnabled(SceneModel *arg0, s8 arg1);

void setModelVisibility(SceneModel *, s8);

s32 setModelRotation(SceneModel *, s16);

void setAnimationIndex(SceneModel *arg0, s8 arg1);
s8 getModelAnimationCount(SceneModel *arg0);
void clearModelAnimationState(SceneModel *model);

void disableEntityRendering(SceneModel *arg0);
void enableEntityRendering(SceneModel *arg0);
void enableModelShadow(SceneModel *arg0);
void disableModelShadow(SceneModel *arg0);

void setModelAnimationLooped(SceneModel *model, s16 animIndex, s16 transitionAnimIndex, s8 loopCount);

void setModelAnimationQueued(SceneModel *arg0, s16 arg1, s16 arg2, s8 arg3, s16 arg4);

void setModelAnimationEx(SceneModel *arg0, s16 arg1, s16 arg2, s8 arg3, short arg4, s8 arg5);

void setModelActionMode(SceneModel *model, s8 actionMode);
s8 getModelActionMode(SceneModel *model);
void markEntityDestroyed(SceneModel *model);

void setModelPartDisplayFlag(SceneModel *model, s32 partIndex);

void clearModelPartDisplayFlag(SceneModel *model, s32 partIndex);

s32 getWalkAnimationId(s16);

s32 getRunAnimationId(s16);

u8 getModelAlpha(SceneModel *arg0);

void setModelAlpha(SceneModel *arg0, u8 arg1);

void enqueueModelDisplayList(SceneModel *arg0, DisplayListObject *arg1);

s32 isAssetGroupEmpty(s16);

s32 hasModelGraphicsData(SceneModel *);

typedef struct {
    void *owner;
    u8 transformMatrix[0x18];
    s32 unk1C;
    u8 padding[0x4];
    void *unk24;
    MemoryAllocatorNode *displayList;
    MemoryAllocatorNode *vertexData;
    s32 unk30;
    u8 padding2[0xC];
    s16 rotationAngle;
    s16 stretchAngle;
} StretchingModelTaskState;

void cleanupStretchingModelTask(StretchingModelTaskState *);
void initStretchingModelTask(StretchingModelTaskState *);
void updateStretchingModelTask(StretchingModelTaskState *);

typedef struct {
    SceneModel *model;
    s16 rotationX;
    s16 rotationY;
    s16 rotationZ;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 angularVelocityX;
    s16 angularVelocityY;
    s16 angularVelocityZ;
    s16 unk16;
} CameraRotationTaskState;

void initCameraRotationTask(CameraRotationTaskState *);

typedef struct {
    SceneModel *model;
    u8 transformMatrix[0x20];
    DisplayLists *displayLists;
    void *displayList;
    void *vertexData;
    s32 renderFlags;
    u8 _pad34[0x10];
    u8 bobbingEnabled;
    u8 _pad45[3];
    s32 verticalOffset;
    s32 verticalVelocity;
} BobbingModelTaskState;

void initBobbingModelTask(BobbingModelTaskState *);

s32 isModelVisible(SceneModel *model);
