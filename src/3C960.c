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

void drawNumericString(u8 *text, s16 x, s16 y, s16 z, s32 texture, u16 priority, u16 layer) {
    s32 count = 0;
    s32 palette = 0;
    s32 c;
    TextElement12 *elem12;
    TextElement16 *elem16;
    s32 i;
    TextElement16 *initPtr;

    for (i = 0; text[i] != NULL; i++) {
        switch (text[i]) {
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

    if (z == 0xFF) {
        elem12 = advanceLinearAlloc(count * 0xC);
        for (i = 0; text[i] != NULL; i++) {
            switch (text[i]) {
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
                    x += 8;
                    break;
                case 0x47:
                    elem12->digit = 0xA;
                    elem12->x = x;
                    x += 8;
                    elem12->y = y;
                    elem12->unk4 = texture;
                    elem12->palette = palette;
                    debugEnqueueCallback(priority, layer & 0xFF, func_80010240_10E40, elem12);
                    elem12++;
                    break;
                case 0x3A:
                    elem12->digit = 0xD;
                    elem12->x = x;
                    x += 8;
                    elem12->y = y;
                    elem12->unk4 = texture;
                    elem12->palette = palette;
                    debugEnqueueCallback(priority, layer & 0xFF, func_80010240_10E40, elem12);
                    elem12++;
                    break;
                case 0x22:
                    elem12->digit = 0xB;
                    elem12->x = x;
                    x += 8;
                    elem12->y = y;
                    elem12->unk4 = texture;
                    elem12->palette = palette;
                    debugEnqueueCallback(priority, layer & 0xFF, func_80010240_10E40, elem12);
                    elem12++;
                    break;
                case 0x27:
                    elem12->digit = 0xC;
                    elem12->x = x;
                    x += 8;
                    elem12->y = y;
                    elem12->unk4 = texture;
                    elem12->palette = palette;
                    debugEnqueueCallback(priority, layer & 0xFF, func_80010240_10E40, elem12);
                    elem12++;
                    break;
                default:
                    elem12->digit = text[i] - 0x30;
                    elem12->x = x;
                    elem12->y = y;
                    x += 8;
                    elem12->unk4 = texture;
                    elem12->palette = palette;
                    debugEnqueueCallback(priority, layer & 0xFF, func_80010240_10E40, elem12);
                    elem12++;
                    break;
            }
        }
    } else {
        elem16 = advanceLinearAlloc(count * 0x10);
        for (i = 0; i < count; i++) {
            elem16[i].unkC = 0;
            elem16[i].palette = 0;
            elem16[i].unkE = z;
            elem16[i].unkA = 0xFF;
        }
        for (i = 0; text[i] != NULL; i++) {
            switch (text[i]) {
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
                    x += 8;
                    break;
                case 0x47:
                    elem16->digit = 0xA;
                    elem16->x = x;
                    x += 8;
                    elem16->y = y;
                    elem16->unk4 = texture;
                    elem16->palette = (s8)(palette + 1);
                    debugEnqueueCallback(priority, layer & 0xFF, func_80012518_13118, elem16);
                    elem16++;
                    break;
                case 0x3A:
                    elem16->digit = 0xD;
                    elem16->x = x;
                    x += 8;
                    elem16->y = y;
                    elem16->unk4 = texture;
                    elem16->palette = (s8)(palette + 1);
                    debugEnqueueCallback(priority, layer & 0xFF, func_80012518_13118, elem16);
                    elem16++;
                    break;
                case 0x22:
                    elem16->digit = 0xB;
                    elem16->x = x;
                    x += 8;
                    elem16->y = y;
                    elem16->unk4 = texture;
                    elem16->palette = (s8)(palette + 1);
                    debugEnqueueCallback(priority, layer & 0xFF, func_80012518_13118, elem16);
                    elem16++;
                    break;
                case 0x27:
                    elem16->digit = 0xC;
                    elem16->x = x;
                    x += 8;
                    elem16->y = y;
                    elem16->unk4 = texture;
                    elem16->palette = (s8)(palette + 1);
                    debugEnqueueCallback(priority, layer & 0xFF, func_80012518_13118, elem16);
                    elem16++;
                    break;
                default:
                    elem16->digit = text[i] - 0x30;
                    elem16->x = x;
                    elem16->y = y;
                    x += 8;
                    elem16->unk4 = texture;
                    elem16->palette = (s8)(palette + 1);
                    debugEnqueueCallback(priority, layer & 0xFF, func_80012518_13118, elem16);
                    elem16++;
                    break;
            }
        }
    }
}