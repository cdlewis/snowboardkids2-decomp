#pragma once

#include "common.h"

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} Vec3i;

typedef struct {
    s16 x;
    s16 y;
    s16 z;
} Vec3s;

typedef struct {
    /* 0x00 */ s16 values[0xA];
    /* 0x14 */ s32 position[3];
    /* 0x20 */ s16 prev_position[0xA];
    /* 0x34 */ s32 interpolated[3];
    /* 0x40 */ s16 *animation_data;
    /* 0x44 */ s16 *frame_data;
    /* 0x48 */ u16 flags;
    /* 0x4A */ u16 counter;
} BoneAnimationState;

typedef struct {
    s16 m[9];
} Mat3x3;

// Matrices always seem to be copied into size 0x20 buffers :/
typedef struct {
    s16 m[3][3];
    Vec3i translation;
} Transform3D;

extern Transform3D identityMatrix;

void createYRotationMatrix(Transform3D *, u16 angle);
void createZRotationMatrix(Transform3D *, u16 angle);
void createXRotationMatrix(s16 matrix[3][3], u16 angle);

void func_8006BDBC_6C9BC(BoneAnimationState *, void *, void *);

void setBonePosition(BoneAnimationState *state, s32 x, s32 y, s32 z);

void func_mulMatrix3x3T(Transform3D *, Transform3D *, Transform3D *);

void func_8006B084_6BC84(void *, void *, void *);

void createCombinedRotationMatrix(void *, u16, u16);

void func_8006BEDC_6CADC(void *output, s32 posX, s32 posY, s32 posZ, s32 tempPosZ, s32 pitchAngle, s32 yawAngle);

void transform3DToMtx(void *src, void *dst);

void createRotationMatrixYX(Transform3D *matrix, u16 angleY, u16 angleX);
void createRotationMatrixXZ(Transform3D *matrix, u16 angleX, u16 angleZ);
void createRotationMatrixYZ(s16 *matrix, u16 angleY, u16 angleZ);

void matrixToEulerAngles(s32 *, s32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *);

void transformVector(s16 *input, s16 *matrix, void *output);
void transformVector2(void *input, void *matrix, Vec3i *output);
void transformVector3(Vec3i *input, Transform3D *matrix, Vec3i *output);

void rotateVectorY(void *, s16, Vec3i *);

void scaleMatrix(Transform3D *matrix, s16 scaleX, s16 scaleY, s16 scaleZ);

s16 computeAngleToPosition(s32 fromX, s32 fromZ, s32 toX, s32 toZ);

s32 atan2Fixed(s32, s32);

s32 approximateSin(s16 inputAngle);
s32 approximateCos(s16 inputAngle);

s32 isqrt64(s64 val);
s32 distance_2d(s32 x, s32 y);
s32 distance_3d(s32 x, s32 y, s32 z);
s32 approximate_sqrt(u32 input);

void computeLookAtMatrix(void *arg0, void *arg1, void *arg2);

void transformVectorRelative(void *arg0, void *arg1, void *arg2);

void transform3DToN64Mtx(Transform3D *transform, Mtx *mtx);

typedef struct QuadDisplayListElement {
    /* 0x00 */ void *model;  // SceneModel *model
    /* 0x04 */ u8 transformMatrix[0x20];
    /* 0x24 */ void *renderSettings;
    /* 0x28 */ void *displayList;
    /* 0x2C */ void *vertexData;
    /* 0x30 */ s32 renderFlag;
    /* 0x34 */ u8 _pad34[0x8];
} QuadDisplayListElement;

void initializeQuadDisplayList(QuadDisplayListElement *elements);