#pragma once

#include "common.h"

typedef struct {
    /* 0x00 */ s16 values[0xA];
    /* 0x14 */ s32 position[3];
    /* 0x20 */ s32 prev_position[3];
    /* 0x2C */ s32 unknown[2];
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
    s32 unk14;
    s32 unk18;
    s32 unk1C;
} Mat3x3Padded;

void createYRotationMatrix(Mat3x3Padded *, u16 angle);
void createZRotationMatrix(Mat3x3Padded *, u16 angle);
void createXRotationMatrix(s16 matrix[3][3], u16 angle);

void func_8006BDBC_6C9BC(func_8005E800_5F400_arg *, void *, void *);

void func_8006B084_6BC84(void *, void *, void *);

void createCombinedRotationMatrix(void *, u16, u16);

void func_8006BEDC_6CADC(void *, s32, s32, s32, s32, s32, s32);

void createRotationMatrixYX(Mat3x3Padded *matrix, s16 angleY, s16 angleX);
void createRotationMatrixXZ(Mat3x3Padded *matrix, u16 angleX, u16 angleZ);

void matrixToEulerAngles(s32 *, s32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *);

void transformVector(s16 *, s16 *, void *);

void scaleMatrix(Mat3x3Padded *matrix, s16 scaleX, s16 scaleY, s16 scaleZ);

s16 func_8006D21C_6DE1C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

s32 atan2Fixed(s32, s32);
