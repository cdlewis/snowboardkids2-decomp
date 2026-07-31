#include "common.h"
#include "ui/save_data.h"

s32 isSaveSlotUnlocked(u8 slotIndex) {
    return EepromSaveData->levelUnlockStatus[slotIndex] == 1;
}

u8 isBoardUnlocked(u8 boardIndex) {
    boardIndex &= 0xFF;
    if (boardIndex < 6) {
        return 1;
    }
    if (boardIndex == 6) {
        return EepromSaveData->specialBoardUnlocked[0];
    }
    if (boardIndex == 7) {
        return EepromSaveData->specialBoardUnlocked[1];
    }
    return EepromSaveData->specialBoardUnlocked[2];
}

u8 getCharacterPaletteId(u8 characterIndex) {
    return EepromSaveData->characterPaletteIds[characterIndex];
}

s32 isCreditsUnlocked(void) {
    return EepromSaveData->levelUnlockStatus[11] == 1;
}
