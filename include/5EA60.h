#include "common.h"
#include "geometry.h"

typedef struct {
    u8 padding[0x8];
    u32 countOffset;
} func_8006097C_6157C_arg_item;

typedef struct {
    func_8006097C_6157C_arg_item data[0];
} func_8006097C_6157C_arg;

s32 func_8006097C_6157C(func_8006097C_6157C_arg *, s32);

void func_8005DE98_5EA98(void *animData, s32 tableIndex, s32 boneIndex, func_8005E800_5F400_arg *state);

void func_8005E800_5F400(func_8005E800_5F400_arg *entity, u16 param_2);

s32 func_8005ECB8_5F8B8(void *arg0, s32 arg1, s32 arg2, void *arg3);
