#include "EepromSaveData_type.h"
#include "common.h"

INCLUDE_ASM("asm/nonmatchings/38C90", func_80038090_38C90);

INCLUDE_ASM("asm/nonmatchings/38C90", func_80038340_38F40);

INCLUDE_ASM("asm/nonmatchings/38C90", func_80038388_38F88);

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
