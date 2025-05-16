#include "common.h"

typedef struct {
    u8 padding[0x8];
    u32 unk8;
} func_8006097C_6157C_arg_item;

typedef struct {
    func_8006097C_6157C_arg_item data[0];
} func_8006097C_6157C_arg;

s32 func_8006097C_6157C(func_8006097C_6157C_arg*, s32);

typedef struct {
    u8 padding[0x40];
    void* unk40;
    void* unk44;
    s16 unk48;
} func_8005DE98_5EA98_arg;

void func_8005DE98_5EA98(void*, s32, s32, func_8005DE98_5EA98_arg*);