#include "common.h"

typedef struct {
    u8 padding[0x8];
    u32 unk8;
} func_8006097C_6157C_arg_item;

typedef struct {
    func_8006097C_6157C_arg_item data[0];
} func_8006097C_6157C_arg;

s32 func_8006097C_6157C(func_8006097C_6157C_arg *, s32);

typedef struct {
    u8 padding[0x40];
    void *unk40;
    void *unk44;
    s16 unk48;
} func_8005DE98_5EA98_arg;

void func_8005DE98_5EA98(void *, s32, s32, func_8005DE98_5EA98_arg *);

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

void func_8005E800_5F400(func_8005E800_5F400_arg *entity, u16 param_2);