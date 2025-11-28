#include "common.h"

typedef struct {
    s32 unk0;
    s32 unk4;
} Struct_800B0DC0;

s32 func_800B0DC0_1DCF60(Struct_800B0DC0 *arg0) {
    return *(s32 *)(arg0->unk4 + (s32)arg0);
}

INCLUDE_ASM("asm/nonmatchings/1DCF60", func_800B0DD0_1DCF70);

INCLUDE_ASM("asm/nonmatchings/1DCF60", func_800B0E24_1DCFC4);

INCLUDE_ASM("asm/nonmatchings/1DCF60", func_800B0E80_1DD020);
