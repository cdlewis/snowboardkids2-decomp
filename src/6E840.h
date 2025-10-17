#include "common.h"

typedef struct {
    u8 padding[0xBF];
    s8 unkBF;
    s8 unkC0;
    s8 unkC1;
} func_8006FDA0_709A0_arg;

typedef struct Node_70B00 {
    /* 0x00 */ union {
        struct Node_70B00 *next;
        u16 callback_selector;
    } unk0;
    /* 0x04 */ struct Node_70B00 *prev;
    /* 0x08 */ union {
        struct Node_70B00 *list2_next;
        u16 callback_selector;
    } unk8;
    /* 0x0C */ struct Node_70B00 *list2_prev;
    /* 0x10 */ struct Node_70B00 *list3_next;
    s8 unk14;
    s8 unk15;
    /* 0x16 */ u16 slot_index;
    u8 padding2[0xC2];
    /* 0xDA */ u16 id;
    u8 padding3[0xFC];
} Node_70B00;

void func_8006FDA0_709A0(func_8006FDA0_709A0_arg *arg0, u8 arg1, u8 arg2);

void debugEnqueueCallback(u16 index, u8 arg1, void *arg2, void *arg3);

void *arenaAlloc16(s32 size);

void *advanceLinearAlloc(s32 size);

void func_8006E02C_6EC2C(void);

void func_8006E000_6EC00(s32 arg0);

typedef struct {
    u8 padding[0xBC];
    s8 unkBC;
    s8 unkBD;
    s8 unkBE;
} func_8006FE28_70A28_arg;

void func_8006FE28_70A28(func_8006FE28_70A28_arg *arg0, s8 arg1, s8 arg2, s8 arg3);

typedef struct {
    u8 padding[0xC8];
    s16 unkC8;
    s16 unkCA;
    u8 padding2[0x104];
    f32 unk1D0;
    s32 unk1D4;
} func_8006F9BC_705BC_arg;

void func_8006F9BC_705BC(func_8006F9BC_705BC_arg *arg0, f32 arg1, f32 arg2);

void func_8006E054_6EC54(u16);

void func_8006DC40_6E840(void);

void func_8006FEF8_70AF8(void *, s32);

void func_8006FD3C_7093C(u16, void *);

void func_8006FAA4_706A4(void *, s32, s32, s32, s32);

void func_8006FA0C_7060C(void *, f32, f32, f32, f32);

void setModelCameraTransform(void *, s32, s32, s32, s32, s32, s32);

void unlinkNode(Node_70B00 *player);

s32 func_8006FE10_70A10(s32);

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} func_80070140_70D40_arg;

s32 func_80070140_70D40(func_80070140_70D40_arg *arg0);

void func_8006FC70_70870(u16, u16, s8 *, s8 *);

typedef struct {
    u8 padding[0xB8];
    s8 unkB8;
    s8 unkB9;
    s8 unkBA;
    s8 unkBB;
} func_8006FE94_70A94_arg;

void func_8006FE94_70A94(func_8006FE94_70A94_arg *arg0, s8 arg1, s8 arg2, s8 arg3);