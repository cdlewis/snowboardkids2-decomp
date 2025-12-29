#include "10AD0.h"
#include "6E840.h"
#include "common.h"

typedef struct {
    s16 x;
    s16 y;
    s32 unk4;
    s16 digit;
    s8 palette;
    s8 padB;
} TextElement12;

typedef struct {
    s16 x;
    s16 y;
    s32 unk4;
    s16 digit;
    s16 unkA;
    s8 unkC;
    s8 palette;
    s8 unkE;
    s8 padF;
} TextElement16;

void func_8003BD60_3C960(u8 *arg0, s16 arg1, s16 arg2, s16 arg3, s32 arg4, u16 arg5, u16 arg6) {
    s32 count = 0;
    s32 palette = 0;
    s32 c;
    TextElement12 *elem12;
    TextElement16 *elem16;
    s32 i;
    TextElement16 *initPtr;

    for (i = 0; arg0[i] != NULL; i++) {
        switch (arg0[i]) {
            case 1:
            case 2:
            case 3:
            case 4:
            case ' ':
                break;
            default:
                count++;
                break;
        }
    }

    if (arg3 == 0xFF) {
        elem12 = advanceLinearAlloc(count * 0xC);
        for (i = 0; arg0[i] != NULL; i++) {
            switch (arg0[i]) {
                case 1:
                    palette = 1;
                    break;
                case 2:
                    palette = 2;
                    break;
                case 3:
                    palette = 3;
                    break;
                case 4:
                    palette = 4;
                    break;
                case 0x20:
                    arg1 += 8;
                    break;
                case 0x47:
                    elem12->digit = 0xA;
                    elem12->x = arg1;
                    arg1 += 8;
                    elem12->y = arg2;
                    elem12->unk4 = arg4;
                    elem12->palette = palette;
                    debugEnqueueCallback(arg5, arg6 & 0xFF, func_80010240_10E40, elem12);
                    elem12++;
                    break;
                case 0x3A:
                    elem12->digit = 0xD;
                    elem12->x = arg1;
                    arg1 += 8;
                    elem12->y = arg2;
                    elem12->unk4 = arg4;
                    elem12->palette = palette;
                    debugEnqueueCallback(arg5, arg6 & 0xFF, func_80010240_10E40, elem12);
                    elem12++;
                    break;
                case 0x22:
                    elem12->digit = 0xB;
                    elem12->x = arg1;
                    arg1 += 8;
                    elem12->y = arg2;
                    elem12->unk4 = arg4;
                    elem12->palette = palette;
                    debugEnqueueCallback(arg5, arg6 & 0xFF, func_80010240_10E40, elem12);
                    elem12++;
                    break;
                case 0x27:
                    elem12->digit = 0xC;
                    elem12->x = arg1;
                    arg1 += 8;
                    elem12->y = arg2;
                    elem12->unk4 = arg4;
                    elem12->palette = palette;
                    debugEnqueueCallback(arg5, arg6 & 0xFF, func_80010240_10E40, elem12);
                    elem12++;
                    break;
                default:
                    elem12->digit = arg0[i] - 0x30;
                    elem12->x = arg1;
                    elem12->y = arg2;
                    arg1 += 8;
                    elem12->unk4 = arg4;
                    elem12->palette = palette;
                    debugEnqueueCallback(arg5, arg6 & 0xFF, func_80010240_10E40, elem12);
                    elem12++;
                    break;
            }
        }
    } else {
        elem16 = advanceLinearAlloc(count * 0x10);
        for (i = 0; i < count; i++) {
            elem16[i].unkC = 0;
            elem16[i].palette = 0;
            elem16[i].unkE = arg3;
            elem16[i].unkA = 0xFF;
        }
        for (i = 0; arg0[i] != NULL; i++) {
            switch (arg0[i]) {
                case 1:
                    palette = 1;
                    break;
                case 2:
                    palette = 2;
                    break;
                case 3:
                    palette = 3;
                    break;
                case 4:
                    palette = 4;
                    break;
                case 0x20:
                    arg1 += 8;
                    break;
                case 0x47:
                    elem16->digit = 0xA;
                    elem16->x = arg1;
                    arg1 += 8;
                    elem16->y = arg2;
                    elem16->unk4 = arg4;
                    elem16->palette = (s8)(palette + 1);
                    debugEnqueueCallback(arg5, arg6 & 0xFF, func_80012518_13118, elem16);
                    elem16++;
                    break;
                case 0x3A:
                    elem16->digit = 0xD;
                    elem16->x = arg1;
                    arg1 += 8;
                    elem16->y = arg2;
                    elem16->unk4 = arg4;
                    elem16->palette = (s8)(palette + 1);
                    debugEnqueueCallback(arg5, arg6 & 0xFF, func_80012518_13118, elem16);
                    elem16++;
                    break;
                case 0x22:
                    elem16->digit = 0xB;
                    elem16->x = arg1;
                    arg1 += 8;
                    elem16->y = arg2;
                    elem16->unk4 = arg4;
                    elem16->palette = (s8)(palette + 1);
                    debugEnqueueCallback(arg5, arg6 & 0xFF, func_80012518_13118, elem16);
                    elem16++;
                    break;
                case 0x27:
                    elem16->digit = 0xC;
                    elem16->x = arg1;
                    arg1 += 8;
                    elem16->y = arg2;
                    elem16->unk4 = arg4;
                    elem16->palette = (s8)(palette + 1);
                    debugEnqueueCallback(arg5, arg6 & 0xFF, func_80012518_13118, elem16);
                    elem16++;
                    break;
                default:
                    elem16->digit = arg0[i] - 0x30;
                    elem16->x = arg1;
                    elem16->y = arg2;
                    arg1 += 8;
                    elem16->unk4 = arg4;
                    elem16->palette = (s8)(palette + 1);
                    debugEnqueueCallback(arg5, arg6 & 0xFF, func_80012518_13118, elem16);
                    elem16++;
                    break;
            }
        }
    }
}