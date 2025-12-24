#include "common.h"

typedef struct {
    /* 0x0 */ u8 header_data[8];
    /* 0x8 */ u32 checksum;
    /* 0xC */ u8 unknown_0C[4];
    /* 0x10 */ u8 save_slot_status[16];
    /* 0x20 */ u8 save_slot_data[16];
    /* 0x30 */ u8 character_or_settings[18];
    union {
        /* 0x42 */ u8 setting_42[9];
        struct {
            /* 0x42 */ u8 character_or_settings_18;
            /* 0x43 */ u8 setting_43[8];
        } s;
    } u;
    /* 0x4B */ u8 setting_4B[3];
    /* 0x4E */ u8 setting_4E;
    /* 0x4F */ u8 setting_4F;
    /* 0x50 */ u8 setting_50;
    /* 0x51 */ u8 unk51;
    /* 0x52 */ u8 padding_52[6];
    /* 0x58 */ u32 padding_58;
} EepromSaveData_type;

extern EepromSaveData_type *EepromSaveData;
