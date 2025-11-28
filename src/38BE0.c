#include "EepromSaveData_type.h"
#include "common.h"

INCLUDE_ASM("asm/nonmatchings/38BE0", func_80037FE0_38BE0);

INCLUDE_ASM("asm/nonmatchings/38BE0", func_80038000_38C00);

u8 func_80038058_38C58(u8 arg0) {
    return EepromSaveData->character_or_settings[arg0];
}

INCLUDE_ASM("asm/nonmatchings/38BE0", func_80038070_38C70);
