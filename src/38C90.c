#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "common.h"

s32 func_80038388_38F88(u8 arg0);
void func_800383D8_38FD8(u8 arg0);

void func_80038090_38C90(s16 arg0) {
    u8 temp;

    temp = D_800AFE8C_A71FC->saveSlotIndex;
    if (temp == 0xB) {
        EepromSaveData->setting_4E = 1;
        func_800383D8_38FD8(0xF);
    }

    temp = D_800AFE8C_A71FC->saveSlotIndex;
    if (temp == 0xE) {
        EepromSaveData->setting_4F = 1;
        func_800383D8_38FD8(0xD);
    }

    temp = D_800AFE8C_A71FC->saveSlotIndex;
    if (temp == 0xD) {
        if (arg0 == 7) {
            EepromSaveData->setting_50 = 1;
            func_800383D8_38FD8(0xE);
        }
    }

    if (D_800AFE8C_A71FC->unk24 != 0) {
        if (D_800AFE8C_A71FC->saveSlotIndex == 1) {
            if (func_80038388_38F88(0xA) & 0xFF) {
                D_800AFE8C_A71FC->errorFlag = 2;
            }
        }

        if (D_800AFE8C_A71FC->saveSlotIndex == 3) {
            if (func_80038388_38F88(0xB) & 0xFF) {
                D_800AFE8C_A71FC->errorFlag = 3;
            }
        }

        if (D_800AFE8C_A71FC->saveSlotIndex == 9) {
            if (func_80038388_38F88(0xC) & 0xFF) {
                D_800AFE8C_A71FC->errorFlag = 4;
            }
        }

        if (D_800AFE8C_A71FC->saveSlotIndex == 5) {
            if (func_80038388_38F88(0xE) & 0xFF) {
                D_800AFE8C_A71FC->errorFlag = 6;
            }
        }

        if (D_800AFE8C_A71FC->saveSlotIndex == 8) {
            if (func_80038388_38F88(0xF) & 0xFF) {
                D_800AFE8C_A71FC->errorFlag = 7;
            }
        }

        if (D_800AFE8C_A71FC->saveSlotIndex == 0xB) {
            if (func_80038388_38F88(0x10) & 0xFF) {
                D_800AFE8C_A71FC->errorFlag = 8;
            }
        }

        if (D_800AFE8C_A71FC->saveSlotIndex == 0) {
            if (func_80038388_38F88(0xD) & 0xFF) {
                D_800AFE8C_A71FC->errorFlag = 5;
            }
        }

        if (D_800AFE8C_A71FC->saveSlotIndex == 4) {
            if (func_80038388_38F88(9) & 0xFF) {
                D_800AFE8C_A71FC->errorFlag = 1;
            }
        }

        if (D_800AFE8C_A71FC->saveSlotIndex == 7) {
            if (func_80038388_38F88(0x11) & 0xFF) {
                D_800AFE8C_A71FC->errorFlag = 9;
            }
        }
    }
}

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
