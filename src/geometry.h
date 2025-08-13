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
    u8 padding[0x40];
    void *unk40;
    void *unk44;
    s16 unk48;
} func_8005DE98_5EA98_arg;

typedef struct {
    s16 m[9];
} Mat3x3;

void createYRotationMatrix(s16 matrix[3][3], u16 angle);
void createZRotationMatrix(s16 matrix[3][3], u16 angle);
void createXRotationMatrix(s16 matrix[3][3], u16 angle);

void func_8006BDBC_6C9BC(func_8005E800_5F400_arg *, void *, void *);

void func_8006B084_6BC84(void *, void *, void *);

void createCombinedRotationMatrix(void *, u16, u16);

void func_8006BEDC_6CADC(void *, s32, s32, s32, s32, s32, s32);

void createRotationMatrixYX(void *, s32, s32);

void matrixToEulerAngles(s32 *, s32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *);

void transformVector(s16 *, s16 *, void *);
