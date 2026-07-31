#pragma once

#include "common.h"

typedef struct {
    /* 0x00 */ u8 magic[8];
    /* 0x08 */ u32 checksum;
    /* 0x0C */ s32 gold;
    /* 0x10 */ u8 levelUnlockStatus[16];
    /* 0x20 */ u8 versusLevelAvailability[16];
    /* 0x30 */ u8 characterPaletteIds[18];
    /* 0x42 */ u8 unlockedCutsceneIds[9];
    /* 0x4B */ u8 unlockedBoardIds[3];
    /* 0x4E */ u8 specialBoardUnlocked[3];
    /* 0x51 */ u8 postCreditsCutscenePending;
    /* 0x52 */ u8 reserved[10];
} EepromSaveDataBlock;

extern EepromSaveDataBlock *EepromSaveData;

void processRaceUnlocks(s16 raceResult);
s32 areCharacterPalettesUnlocked(s32 count);
s32 tryAddUnlockedCutsceneId(u8 cutsceneId);
void tryAddUnlockedBoardId(u8 boardId);
