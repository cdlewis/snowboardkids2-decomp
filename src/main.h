#include "6E840.h"
#include "common.h"
#include "gamestate.h"

typedef struct {
    char padding[0xD5];
    s32 unkDA;
} func_80000460_1060_arg0_unk0;

typedef struct {
    func_80000460_1060_arg0_unk0 *unk0;
    char padding[0x80];
    s16 unk84;
} func_80000460_1060_arg0;

void func_80000460_1060(func_80000460_1060_arg0 *arg0, ColorData *arg1, ColorData *arg2);

void func_8000056C_116C(func_80000460_1060_arg0 *, s32, GameState *);

void func_80000760_1360(void);

void func_80000710_1310(s32 *);

typedef struct {
    char padding[0x87];
    char unk87;
} setModelRenderMode_arg;

void setModelRenderMode(setModelRenderMode_arg *arg0, s8 arg1);
