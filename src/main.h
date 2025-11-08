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

typedef struct {
    struct {
        u8 padding[0x16];
        u16 unk16;
    } *unk0;
    u8 padding[0x8];
    DisplayListObject unkC;
    s32 unk48;
    u8 padding4[0x20];
    s32 unk6C;
    u8 padding5[0x17];
    s8 unk87;
} func_80000760_1360_arg;

void func_80000760_1360(func_80000760_1360_arg *);

typedef struct {
    s32 unk0;
    void *unk4;
    void *unk8;
    u8 padding[0x7A];
    s8 unk86;
} func_80000710_1310_arg;

void func_80000710_1310(func_80000710_1310_arg *);

typedef struct {
    char padding[0x87];
    char unk87;
} setModelRenderMode_arg;

void setModelRenderMode(setModelRenderMode_arg *arg0, s8 arg1);
