#pragma once

#include "common.h"
#include "gamestate.h"
#include "math/geometry.h"

typedef struct {
    u8 boneIndex;
    u8 parentBone;
} BoneHierarchyEntry;

typedef struct {
    s32 baseOffset;
    s32 offsetTable[0];
} IndexedAnimationDataLayout;

typedef struct {
    u16 padding;
    u16 index;
} IndexedAnimationDataEntry;

typedef struct {
    /* 0x00 */ u8 header[8];
    /* 0x08 */ u32 boneCountOffsets[0]; // Array of offsets to u16 bone count values
} AnimationBoneCountTable;

void *getAnimationFrameData(void *animData);

u16 *getAnimationDataByIndex(void *animData, s16 tableIndex, s16 boneIndex);

void createBoneRotMatrix(s16 angleX, s16 angleY, s16 angleZ, s16 *matrix);

s32 getAnimationBoneCount(AnimationBoneCountTable *, s32);

void initBoneAnimationState(void *animData, s32 tableIndex, s32 boneIndex, BoneAnimationState *state);

s32 updateBoneAnimation(BoneAnimationState *state);

void interpolateBoneAnimation(BoneAnimationState *state, u16 progress);

void interpolatedBoneAnimationMirrored(BoneAnimationState *state, u16 progress);

s32 advanceBoneAnimation(void *animData, s32 tableIndex, s32 boneIndex, BoneAnimationState *state);

s32 advanceBoneAnimationMirrored(void *animData, s32 tableIndex, s32 boneIndex, void *state);

s32 advanceIndexedBoneAnimation(void *animData, s16 tableIndex, s16 boneIndex, BoneAnimationStateIndexed *state);

s32 advanceIndexedBoneAnimationMirrored(
    void *animData,
    s16 tableIndex,
    s16 boneIndex,
    BoneAnimationStateIndexed *state
);

void interpolateIndexedBoneAnimation(
    void *animData,
    s16 tableIndex,
    s16 boneIndex,
    BoneAnimationStateIndexed *entity,
    s16 param_2
);

void interpolateIndexedBoneAnimationMirrored(
    void *animData,
    s16 tableIndex,
    s16 boneIndex,
    BoneAnimationStateIndexed *entity,
    u16 param_2
);

s32 advanceIndexedBoneAnimationAuto(void *animData, s16 tableIndex, s16 boneIndex, BoneAnimationStateIndexed *state);

s32 advanceIndexedBoneAnimationAutoMirrored(
    void *animData,
    s16 tableIndex,
    s16 boneIndex,
    BoneAnimationStateIndexed *state
);

void resetBoneAnimation(void *animData, s16 tableIndex, s16 boneIndex, BoneAnimationStateIndexed *state);

BoneHierarchyEntry *getIndexedAnimationDataPtr(IndexedAnimationDataLayout *data, s16 index);