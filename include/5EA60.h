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
    u8 padding[0x40];
    s16 unk40;
    s16 padding2;
    s16 unk44;
    s16 _pad2;
} func_8005F2FC_5FEFC_arg;

s32 func_8006097C_6157C(func_8006097C_6157C_arg *, s32);

void initBoneAnimationState(void *animData, s32 tableIndex, s32 boneIndex, BoneAnimationState *state);

s32 updateBoneAnimation(BoneAnimationState *state);

void interpolateBoneAnimation(BoneAnimationState *state, u16 progress);

void interpolatedBoneAnimationMirrored(BoneAnimationState *state, u16 progress);

void func_8005DE98_5EA98(void *animData, s32 tableIndex, s32 boneIndex, BoneAnimationState *state);
s32 advanceBoneAnimation(void *animData, s32 tableIndex, s32 boneIndex, BoneAnimationState *state);
s32 func_8005EFC4_5FBC4(void *arg0, s32 arg1, s32 arg2, void *arg3);
void func_8005F2FC_5FEFC(void *arg0, s16 arg1, s16 arg2, func_8005F2FC_5FEFC_arg *arg3);
