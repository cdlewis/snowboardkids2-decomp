#include "common.h"
#include "geometry.h"

void *getAnimationFrameData(void *animData);
u16 *getAnimationDataByIndex(void *animData, s16 tableIndex, s16 boneIndex);

void createBoneRotMatrix(s16 angleX, s16 angleY, s16 angleZ, s16 *matrix);

typedef struct {
    u8 padding[0x8];
    u32 countOffset;
} func_8006097C_6157C_arg_item;

typedef struct {
    func_8006097C_6157C_arg_item data[0];
} func_8006097C_6157C_arg;

typedef struct {
    /* 0x00 */ s16 values[10];
    /* 0x14 */ s32 position[3];
    /* 0x20 */ s16 prev_position[10];
    /* 0x34 */ s32 interpolated[3];
    /* 0x40 */ u16 flags;
    /* 0x42 */ u16 counter;
    /* 0x44 */ u16 animation_index;
    /* 0x46 */ u16 pad;
} BoneAnimationStateIndexed;

s32 func_8006097C_6157C(func_8006097C_6157C_arg *, s32);

void initBoneAnimationState(void *animData, s32 tableIndex, s32 boneIndex, BoneAnimationState *state);

s32 updateBoneAnimation(BoneAnimationState *state);

void interpolateBoneAnimation(BoneAnimationState *state, u16 progress);

void interpolatedBoneAnimationMirrored(BoneAnimationState *state, u16 progress);

void func_8005DE98_5EA98(void *animData, s32 tableIndex, s32 boneIndex, BoneAnimationState *state);
s32 advanceBoneAnimation(void *animData, s32 tableIndex, s32 boneIndex, BoneAnimationState *state);
s32 advanceBoneAnimationMirrored(void *animData, s32 tableIndex, s32 boneIndex, void *state);
s32 advanceIndexedBoneAnimation(void *animData, s16 tableIndex, s16 boneIndex, BoneAnimationStateIndexed *state);
s32 advanceIndexedBoneAnimationMirrored(void *animData, s16 tableIndex, s16 boneIndex, BoneAnimationStateIndexed *state);
void interpolateIndexedBoneAnimation(void *animData, s16 tableIndex, s16 boneIndex, BoneAnimationStateIndexed *entity, s16 param_2);
void interpolateIndexedBoneAnimationMirrored(void *animData, s16 tableIndex, s16 boneIndex, BoneAnimationStateIndexed *entity, u16 param_2);
s32 advanceIndexedBoneAnimationAuto(void *animData, s16 tableIndex, s16 boneIndex, BoneAnimationStateIndexed *state);
s32 advanceIndexedBoneAnimationAutoMirrored(void *animData, s16 tableIndex, s16 boneIndex, BoneAnimationStateIndexed *state);
void resetBoneAnimation(void *animData, s16 tableIndex, s16 boneIndex, BoneAnimationStateIndexed *state);
