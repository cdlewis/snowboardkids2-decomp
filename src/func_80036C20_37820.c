#include "common.h"

extern s32 D_800AFF10_A7280;

typedef struct {
    char padding[1032];
    s32 unk408;
    char padding2[4];
    s32 unk410;
} func_80036274_36E74_large_struct_item;
typedef struct {
    func_80036274_36E74_large_struct_item *entries[4];
    u8 unk10;
    u8 padding1[106];
    u8 unk73;
    char padding[911];
    u8 unk403;
    char padding5[14];
    s32 numEntries;
    u8 unk420;
    u8 unk421;
    u8 unk422;
    char padding4[1];
    u8 unk424;
    u8 unk425;
    char padding2[4];
    u8 status;
    u8 unk42D;
} func_80036274_36E74_large_struct;
extern func_80036274_36E74_large_struct *getCurrentAllocation();
s32 func_8006C590_6D190(u32, u32);
extern s32 D_800AFF10_A7280;

typedef struct {
    char padding[52];
    s32 unk34;
    char padding2[2];
    s32 unk3C;
} func_80036C20_37820_arg;

s32 func_80036C20_37820(func_80036C20_37820_arg *arg0) {
    func_80036274_36E74_large_struct *temp_v0;
    s32 i;
    volatile s32 new_var;
    s32 distances[4];

    temp_v0 = (GameState *)getCurrentAllocation();

    if (temp_v0->numEntries == 0 || temp_v0->unk421 == 1 || temp_v0->unk422 == 1) {
        return 0;
    }

    new_var = 0x1000000;
    if (temp_v0->status == 0) {
        for (i = 0; i < temp_v0->numEntries; i++) {
            func_80036274_36E74_large_struct_item *entry = temp_v0->entries[i];
            s32 dx = entry->unk408 - arg0->unk34;
            s32 dy = entry->unk410 - arg0->unk3C;
            s32 dist = func_8006C590_6D190(dx, dy);
            distances[i] = dist;

            if ((dist <= 0x37FFFF) && (D_800AFF10_A7280 & 0x8000)) {
                temp_v0->status = 0x11;
            }
        }
    }

    if (temp_v0->status != 0x11) {
        return 0;
    }

    if (new_var < distances[0]) {
        return 1;
    } else {
        return 2;
    }
}
