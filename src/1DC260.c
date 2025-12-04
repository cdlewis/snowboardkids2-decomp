#include "common.h"

INCLUDE_ASM("asm/nonmatchings/1DC260", func_800B00C0_1DC260);

extern void debugEnqueueCallback(s32, s32, void *, s32);
extern void func_80012004_12C04(void);

void func_800B016C(void *arg0) {
    s16 temp_v1;
    s32 temp_v0;
    s32 value;

    temp_v1 = *(s16 *)((u8 *)arg0 + 0x2);

    switch (temp_v1) {
        case 0x1A22:
            *(s32 *)((u8 *)arg0 + 0xDFC) = 0xC0000;
            break;
        case 0x1AD6:
            *(s32 *)((u8 *)arg0 + 0xDFC) = 0xFFF40000;
            break;
        case 0x1B30:
            *(s32 *)((u8 *)arg0 + 0xDF4) = 0xC0000;
            break;
    }

    value = *(s32 *)((u8 *)arg0 + 0xDF4);
    if (value != 0) {
        temp_v0 = *(s32 *)((u8 *)arg0 + 0xDF0) + value;
        *(s32 *)((u8 *)arg0 + 0xDF0) = temp_v0;
        if (temp_v0 > 0xFF0000) {
            *(s32 *)((u8 *)arg0 + 0xDF0) = 0xFF0000;
        }
        if (*(s32 *)((u8 *)arg0 + 0xDF0) < 0) {
            *(s32 *)((u8 *)arg0 + 0xDF0) = 0;
        }
    }

    value = *(s32 *)((u8 *)arg0 + 0xDFC);
    if (value != 0) {
        temp_v0 = *(s32 *)((u8 *)arg0 + 0xDF8) + value;
        *(s32 *)((u8 *)arg0 + 0xDF8) = temp_v0;
        if (temp_v0 > 0xFF0000) {
            *(s32 *)((u8 *)arg0 + 0xDF8) = 0xFF0000;
        }
        if (*(s32 *)((u8 *)arg0 + 0xDF8) < 0) {
            *(s32 *)((u8 *)arg0 + 0xDF8) = 0;
        }
    }

    temp_v0 = *(s32 *)((u8 *)arg0 + 0xDF0);
    if (temp_v0 != 0) {
        s16 shortVal = temp_v0 >> 16;
        void *callback = func_80012004_12C04;
        *(s16 *)((u8 *)arg0 + 0xE1A) = shortVal;
        *(s16 *)((u8 *)arg0 + 0xE0A) = shortVal;
        debugEnqueueCallback(1, 4, callback, (s32)arg0 + 0xE00);
        debugEnqueueCallback(1, 4, callback, (s32)arg0 + 0xE10);
    }

    temp_v0 = *(s32 *)((u8 *)arg0 + 0xDF8);
    if (temp_v0 != 0) {
        s16 shortVal = temp_v0 >> 16;
        void *callback = func_80012004_12C04;
        *(s16 *)((u8 *)arg0 + 0xE3A) = shortVal;
        *(s16 *)((u8 *)arg0 + 0xE2A) = shortVal;
        debugEnqueueCallback(1, 4, callback, (s32)arg0 + 0xE20);
        debugEnqueueCallback(1, 4, callback, (s32)arg0 + 0xE30);
    }
}
