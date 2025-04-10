#include "common.h"

extern s8 D_8018F800;
extern s32 D_800AB064_A23D4;
typedef struct LinkedList_6AE20 {
    /* 0x0 */ struct LinkedList_6AE20 *prev;  // 0x0
    /* 0x4 */ struct LinkedList_6AE20 *next;  // 0x4
    u8 padding1[0x4];
    s32 unk_0C;
    u8 padding2[0x4];
    s32 unk_14;
    s32 unk_18;
} LinkedList_6AE20;

extern LinkedList_6AE20 *D_800AB130_A24A0;
extern s32 D_800AB12C_A249C;

void func_8006A220_6AE20(void) {
    s32 var_v1;
    s8 *var_a0;

    D_800AB130_A24A0 = NULL;
    var_a0 = &D_8018F800;
    var_v1 = 0;
    do {
        *var_a0 = 0;
        var_v1 += 1;
        var_a0 += 1;
    } while (var_v1 <= 0x1FFFFF);
}

INCLUDE_ASM("asm/nonmatchings/6AE20", func_8006A258_6AE58);

s32 func_8006A3FC_6AFFC(s32 *arg0) {
    if (arg0 == NULL) {
        return 0;
    }
    arg0[-5]--;
    arg0[-2] = D_800AB064_A23D4;
    return 0;
}

void func_8006A428_6B028() {
    s32 globalVal;

    LinkedList_6AE20 *curr = D_800AB130_A24A0;
    if (curr == 0) {
        return;
    }

    globalVal = D_800AB12C_A249C;

    while (curr != 0) {
        s32 diff = (globalVal - curr->unk_18) & 0x0FFFFFFF;
        if (diff <= 0x07FFFFFF) {
            curr->unk_18 = -1;
        }

        if (((curr->unk_0C == 0) && (curr->unk_14 == 0)) && (curr->unk_18 < 0)) {
            LinkedList_6AE20 *prev;
            LinkedList_6AE20 *next;
            if (curr->prev != 0) {
                curr->prev->next = curr->next;
            } else {
                D_800AB130_A24A0 = curr->next;
            }
            if (curr->next != 0) {
                curr->next->prev = curr->prev;
            }
        }
        curr = curr->next;
    }
}

void func_8006A4DC_6B0DC(s32 *arg0) {
    arg0[-0x3] = 1;
}

void func_8006A4E8_6B0E8(s32 *arg0) {
    u32 mask = osSetIntMask(1);
    arg0[-3] = 0;
    osSetIntMask(mask);
}

s32 func_8006A51C_6B11C(void *arg0, void *arg1) {
    return (s32)(*((void **)((u8 *)arg0 - 0xC)));
}

void func_8006A524_6B124(void *arg0, void *arg1) {
    *((void **)((u8 *)arg0 - 4)) = arg1;
}

s32 func_8006A52C_6B12C(void *arg0) {
    return (s32) * ((void **)((u8 *)arg0 - 4));
}
