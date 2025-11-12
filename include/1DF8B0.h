#include "common.h"

typedef struct {
    s16 rotation_y;
    s16 unk2;
    s16 grid_x;
    s16 grid_y;
    s8 unk8;
    s8 render_flags;
} func_800B2810_1DF8C0_arg0;

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} Vec3;

typedef struct {
    s8 pad[0x70];
    Vec3 position;
    s8 pad2[0xF4 - 0x7C];
} func_800B2810_1DF8C0_arg1_slot;

typedef struct {
    s8 padding[0xA8];
    func_800B2810_1DF8C0_arg1_slot slots[5];
    u8 padding2[0xA8C];
    /* 0xFF8 */ void *matrix;
} func_800B2810_1DF8C0_arg1;

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
} func_800B2944_1DF9F4_arg0;

typedef struct {
    void *unk0;
    s32 unk4;
    u8 padding[0x8];
    s16 unk10;
    s16 unk12;
    u8 padding2[0x72];
    s16 unk86;
} func_800B2944_1DF9F4_asset;

void noop_1DF8B(void);
s32 returnZero_1DF8B8(void);
void func_800B2810_1DF8C0(func_800B2810_1DF8C0_arg0 *params, func_800B2810_1DF8C0_arg1 *state, s8 slot_index);
s32 returnZero_1DF9DC(void);
void noop_1DF9E4(void);
s32 returnZero_1DF9EC(void);
void func_800B2944_1DF9F4(func_800B2944_1DF9F4_arg0 *arg0, u8 *arg1, s8 arg2);