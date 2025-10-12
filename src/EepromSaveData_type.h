#include "common.h"

typedef struct {
    /* 0x0 */ u8 header_data[8];
    /* 0x8 */ u32 checksum;
    /* 0xC */ u8 unknown_0C[4];
    /* 0x10 */ u8 save_slot_status[16];
    /* 0x20 */ u8 save_slot_data[16];
    /* 0x30 */ u8 character_or_settings[19];
    /* 0x43 */ u8 setting_43[8];
    /* 0x4B */ u8 setting_4B;
    /* 0x4C */ u8 setting_4C;
    /* 0x4D */ u8 setting_4D;
    /* 0x4E */ u8 setting_4E;
    /* 0x4F */ u8 setting_4F;
    /* 0x50 */ u8 setting_50;
    /* 0x51 */ u8 padding_51[7];
} EepromSaveData_type;

extern EepromSaveData_type *EepromSaveData;
