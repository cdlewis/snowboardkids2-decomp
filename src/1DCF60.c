#include "common.h"

typedef struct {
    s32 unk0;
    s32 unk4;
} Struct_800B0DC0;

s32 func_800B0DC0_1DCF60(Struct_800B0DC0 *arg0) {
    return *(s32 *)(arg0->unk4 + (s32)arg0);
}

s16 func_800B0DD0_1DCF70(Struct_800B0DC0 *arg0, s32 arg1) {
    s32 temp;
    if (arg1 < func_800B0DC0_1DCF60(arg0)) {
        temp = arg0->unk0 + (s32)arg0 + arg1 * 8;
        return *(s16 *)temp;
    }
    return -1;
}

void *func_800B0E24_1DCFC4(Struct_800B0DC0 *arg0, s32 arg1) {
    s32 temp;
    s32 offset;
    if (arg1 < func_800B0DC0_1DCF60(arg0)) {
        offset = arg0->unk0 + (s32)arg0 + arg1 * 8;
        temp = *(s32 *)(offset + 4);
        if (temp != -1) {
            return (void *)(temp + (s32)arg0);
        }
    }
    return 0;
}

s16 func_800B0E80_1DD020(Struct_800B0DC0 *arg0, s32 arg1) {
    s32 temp;
    if (arg1 < func_800B0DC0_1DCF60(arg0)) {
        temp = arg0->unk0 + (s32)arg0 + arg1 * 8;
        return *(s16 *)(temp + 2);
    }
    return -1;
}
