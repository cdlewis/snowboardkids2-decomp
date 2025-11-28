#include "EepromSaveData_type.h"
#include "common.h"

INCLUDE_ASM("asm/nonmatchings/38C90", func_80038090_38C90);

s32 func_80038340_38F40(s32 arg0) {
    s32 result = 1;
    s32 i = 0;
    u8 *base;
    s32 pad[2];

    if (arg0 > 0) {
        base = (u8 *)EepromSaveData;
        do {
            if ((base + i)[0x30] == 0) {
                result = 0;
                goto done;
            }
        } while (++i < arg0);
    }
done:
    return result;
}

s32 func_80038388_38F88(u8 arg0) {
    s32 result = 1;
    s32 i = 0;
    u8 val = arg0;

    do {
        u8 curr = EepromSaveData->u.setting_42[i];
        if (curr == 0) {
            EepromSaveData->u.setting_42[i] = arg0;
            break;
        }
        if (curr == val) {
            result = 0;
            break;
        }
        i++;
    } while (i < 9);

    return result;
}

void func_800383D8_38FD8(u8 arg0) {
    s32 i;
    u8 val = arg0;

    for (i = 0; i < 3; i++) {
        u8 current = EepromSaveData->setting_4B[i];
        if (current == 0) {
            EepromSaveData->setting_4B[i] = arg0;
            return;
        }
        if (current == val) {
            return;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/38C90", func_80038420_39020);

INCLUDE_ASM("asm/nonmatchings/38C90", func_800394BC_3A0BC);
