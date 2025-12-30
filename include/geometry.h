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
} func_8005E800_5F400_arg;

typedef struct {
    s16 m[9];
} Mat3x3;

// Matrices always seem to be copied into size 0x20 buffers :/
typedef struct {
    s16 m[3][3];
    Vec3i translation;
} Transform3D;

typedef struct {
    s16 unk0;
    u16 unk2;
    s16 unk4;
    s16 unk6;
    u16 unk8;
    s16 unkA;
    s16 unkC;
    u16 unkE;
    s16 unk10;
    s32 unk14;
    u16 unk18;
    u16 unk1A;
    s32 unk1C;
} LookAtData;

void createYRotationMatrix(Transform3D *, u16 angle);
void createZRotationMatrix(Transform3D *, u16 angle);
void createXRotationMatrix(s16 matrix[3][3], u16 angle);

void func_8006BDBC_6C9BC(func_8005E800_5F400_arg *, void *, void *);

void func_8006B084_6BC84(void *, void *, void *);

void createCombinedRotationMatrix(void *, u16, u16);

void func_8006BEDC_6CADC(void *, s32, s32, s32, s32, s32, s32);

void func_8006BFB8_6CBB8(void *, void *);

void createRotationMatrixYX(Transform3D *matrix, u16 angleY, u16 angleX);
void createRotationMatrixXZ(Transform3D *matrix, u16 angleX, u16 angleZ);
void createRotationMatrixYZ(s16 *matrix, u16 angleY, u16 angleZ);

void matrixToEulerAngles(s32 *, s32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *);

void transformVector(s16 *, s16 *, void *);
void transformVector2(void *matrix, void *vector, s32 *output);
void transformVector3(s32 *, Transform3D *, s32 *);

void rotateVectorY(void *, s16, void *);

void scaleMatrix(Transform3D *matrix, s16 scaleX, s16 scaleY, s16 scaleZ);

s16 func_8006D21C_6DE1C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

s32 atan2Fixed(s32, s32);

s32 approximateSin(s16 inputAngle);
s32 approximateCos(s16 inputAngle);

s32 isqrt64(s64 val);
s32 distance_2d(s32 x, s32 y);
s32 distance_3d(s32 x, s32 y, s32 z);
s32 approximate_sqrt(u32 input);

void computeLookAtMatrix(void *arg0, void *arg1, void *arg2);

void transformVectorRelative(void *arg0, void *arg1, void *arg2);

void func_8006C130_6CD30(LookAtData *arg0, Mtx *arg1);