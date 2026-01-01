#include "EepromSaveData_type.h"
#include "common.h"

s32 isSaveSlotUnlocked(u8 slotIndex) {
    return EepromSaveData->save_slot_status[slotIndex] == 1;
}

u8 func_80038000_38C00(u8 arg0) {
    arg0 &= 0xFF;
    if (arg0 < 6) {
        return 1;
    }
    if (arg0 == 6) {
        return EepromSaveData->setting_4E;
    }
    if (arg0 == 7) {
        return EepromSaveData->setting_4F;
    }
    return EepromSaveData->setting_50;
}

u8 func_80038058_38C58(u8 arg0) {
    return EepromSaveData->character_or_settings[arg0];
}

s32 func_80038070_38C70(void) {
    return EepromSaveData->save_slot_status[11] == 1;
}
