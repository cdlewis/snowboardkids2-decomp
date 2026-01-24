#include "EepromSaveData_type.h"
#include "common.h"

s32 isSaveSlotUnlocked(u8 slotIndex) {
    return EepromSaveData->save_slot_status[slotIndex] == 1;
}

u8 isBoardUnlocked(u8 boardIndex) {
    boardIndex &= 0xFF;
    if (boardIndex < 6) {
        return 1;
    }
    if (boardIndex == 6) {
        return EepromSaveData->setting_4E;
    }
    if (boardIndex == 7) {
        return EepromSaveData->setting_4F;
    }
    return EepromSaveData->setting_50;
}

u8 getCharacterPaletteId(u8 characterIndex) {
    return EepromSaveData->character_or_settings[characterIndex];
}

s32 isCreditsUnlocked(void) {
    return EepromSaveData->save_slot_status[11] == 1;
}
