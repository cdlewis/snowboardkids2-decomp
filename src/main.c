#include "main.h"

#include "4050.h"
#include "5520.h"
#include "6E840.h"
#include "common.h"
#include "geometry.h"
#include "rand.h"
#include "task_scheduler.h"

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s16 unk14;
    s16 unk16;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
} SubEntry;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s16 unk18;
    s16 unk1A;
    s32 unk1C;
    s32 unk20;
} SubEntryVariant;

typedef struct {
    SubEntry *sub_entries;
    u32 param1;
    u32 color1;
    u32 color2;
    char name[8];
    u32 addr1;
    u32 addr2;
    u32 addr3;
    u32 addr4;
    u32 size;
    void *ptr1;
    u32 param2;
    void *ptr2;
    u32 param3;
} DataEntry; // Total: 60 bytes
extern DataEntry D_800891D4_89DD4[];

typedef struct {
    char padding[44];
    void *unk2C;
    void *unk30;
} func_80000BF4_17F4_arg;

typedef struct {
    char padding[0x2C];
    s32 *unk2C;
    s32 *unk30;
    char padding2[0x10];
    s32 *unk44;
} func_80001114_1D14_arg;

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 padding[0x1C];
} D_800891A8_89DA8_entry_2C;

typedef struct {
    u8 unk0;
    s32 unk4;
    /* 0x8 */ void *asset1Start;
    /* 0xC */ void *asset1End;
    /* 0x10 */ void *asset2Start;
    /* 0x14 */ void *asset2End;
    /* 0x18 */ s32 asset2Size;
    u8 padding[0x10];
    D_800891A8_89DA8_entry_2C *unk2C;
    s32 unk30;
    u8 unk34;
    u8 unk35;
    u8 unk36;
    u8 unk37;
    u8 unk38;
    u8 unk39;
    u8 unk3A;
    u8 unk3B;
} D_800891A8_89DA8_entry;
extern D_800891A8_89DA8_entry D_800891A8_89DA8[];

typedef struct {
    char padding[0x87];
    char unk87;
} func_80000450_1050_arg;

typedef struct {
    struct {
        struct {
            u8 padding[0x14];
            s16 unk14;
            u16 unk16;
        } *ptr;
        u8 padding2[0x80];
        s16 unk84;
        s8 unk86;
        s8 unk87;
    } *unk0;
    s8 unk4;
    s8 unk5;
    func_80004FF8_5BF8_arg1 *unk8;
    u8 padding2[0x10];
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    void *unk2C;
    void *unk30;
    s32 unk34;
    u8 padding[0xC];
    u16 unk44;
    u16 unk46;
    u16 unk48;
    u16 unk4A;
    s16 unk4C;
    s16 unk4E;
    u16 unk50;
    u16 unk52;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    s32 unk60;
} func_80000C2C_182C_arg;

s32 D_800894F0_8A0F0[];

void *func_80009F5C_AB5C(func_80000C2C_182C_arg *arg0);
void func_80000DA4_19A4(func_80000C2C_182C_arg *arg0);
void func_80001264_1E64(func_80000C2C_182C_arg *arg0);
void func_800011DC_1DDC(void *);
void func_80000CAC_18AC(func_80000C2C_182C_arg *);
void func_80009F90_AB90(void *, s32, s16, s32);
void func_8000A13C_AD3C(void *, u16, s32, s32, s32, s32, s32, s32, s32); /* extern */
void func_80000A68_1668(func_80000C2C_182C_arg *);
void func_80000BF4_17F4(func_80000BF4_17F4_arg *);
void func_800007C4_13C4(void *, void *);
void func_800013B8_1FB8(func_80000C2C_182C_arg *arg0);
void func_800014C8_20C8(func_80000C2C_182C_arg *arg0);

extern s32 func_8000A030_AC30(void *, s32);
extern void func_80009E68_AA68(void *, s16);
extern s32 identityMatrix;

void func_80000450_1050(func_80000450_1050_arg *arg0, s8 arg1) {
    arg0->unk87 = arg1;
}

s32 osVoiceCheckWord(u8 *data) {
    return 0xE;
}

void func_80000460_1060(func_80000460_1060_arg0 *arg0, s8 *arg1, s8 *arg2) {
    s32 temp_v0;
    s32 temp_v0_2;
    int new_var3;
    s32 temp_t1;
    s32 var_a1;
    u8 temp_a0;
    s32 new_var;
    D_800891A8_89DA8_entry *temp_a2;
    s16 index;

    temp_a2 = &D_800891A8_89DA8[arg0->unk84];
    new_var3 = temp_a2->unk34 + temp_a2->unk35;
    temp_v0 = new_var3;
    temp_a0 = temp_a2->unk36;
    new_var = (temp_v0 + temp_a0) / 3;
    var_a1 = new_var;
    temp_v0 = ((u32)new_var) >> 0x1F;
    temp_v0 = var_a1 + ((s8)temp_v0);
    new_var = temp_v0 >> 0x1;
    temp_t1 = new_var;
    if (var_a1 < 0) {
        var_a1 += 3;
    }
    arg1[0x4] = 0;
    arg1[0x5] = 0x7F;
    arg1[0x6] = 0x7F;
    arg1[0x0] = temp_a2->unk34;
    arg1[0x1] = temp_a2->unk35;
    temp_v0_2 = var_a1 >> 2;
    arg1[0x2] = temp_a2->unk36;
    arg1[0x8] = temp_v0_2;
    arg1[0x9] = temp_v0_2;
    arg1[0xA] = temp_v0_2;
    arg1[0xC] = 0x7F;
    arg1[0xD] = 0x7F;
    arg1[0xE] = 0;
    arg1[0x14] = -0x7F;
    arg1[0x15] = 0x7F;
    arg1[0x16] = 0;
    arg1[0x10] = temp_t1;
    arg1[0x11] = temp_t1;
    arg1[0x12] = temp_t1;
    arg2[0x0] = temp_a2->unk38;
    arg2[0x1] = temp_a2->unk39;
    arg2[0x2] = temp_a2->unk3A;
    func_8006FC70_70870(arg0->unk0->unkDA, 3, arg1, arg2);
}

INCLUDE_ASM("asm/nonmatchings/main", func_8000056C_116C);

INCLUDE_ASM("asm/nonmatchings/main", func_80000710_1310);

INCLUDE_ASM("asm/nonmatchings/main", func_80000760_1360);

INCLUDE_ASM("asm/nonmatchings/main", func_800007C4_13C4);

INCLUDE_ASM("asm/nonmatchings/main", func_800007F0_13F0);

void func_800008D0_14D0(func_80000C2C_182C_arg *arg0) {
    u16 temp;
    DataEntry *entry = &D_800891D4_89DD4[arg0->unk0->unk84];
    SubEntry *subEntry = &entry->sub_entries[arg0->unk4];

    if (arg0->unk0->unk86 != 0) {
        func_80069CF8_6A8F8();
    }

    temp = ((SubEntryVariant *)subEntry)->unk14;
    temp = arg0->unk44 + temp;
    arg0->unk44 = temp;

    createYRotationMatrix(&arg0->unk8, temp);
    func_800007C4_13C4(arg0->unk0, &arg0->unk8);
}

INCLUDE_ASM("asm/nonmatchings/main", func_80000968_1568);

void func_800009A0_15A0(func_80000C2C_182C_arg *arg0) {
    D_800891A8_89DA8_entry *temp_s0;
    D_800891A8_89DA8_entry_2C *temp_s2;

    temp_s0 = &D_800891A8_89DA8[arg0->unk0->unk84];
    temp_s2 = &temp_s0->unk2C[arg0->unk4];

    setCleanupCallback(&func_80000BF4_17F4);

    memcpy(&arg0->unk8, &identityMatrix, 0x20);

    arg0->unk2C = dmaRequestAndUpdateState(temp_s0->asset1Start, temp_s0->asset1End);
    arg0->unk30 = dmaRequestAndUpdateStateWithSize(temp_s0->asset2Start, temp_s0->asset2End, temp_s0->asset2Size);
    arg0->unk34 = 0;
    arg0->unk28 = temp_s2->unk4;
    arg0->unk44 = 0;

    setCallback(&func_80000A68_1668);
}

void func_80000A68_1668(func_80000C2C_182C_arg *arg0) {
    s16 yRotMatrix[3][3];
    s16 padding2[3];
    s16 zRotMatrix[3][3];
    s16 padding[3];
    DataEntry *entry;
    SubEntry *subEntry;

    entry = &D_800891D4_89DD4[arg0->unk0->unk84];
    subEntry = &entry->sub_entries[arg0->unk4];
    if (arg0->unk0->unk86 != 0) {
        func_80069CF8_6A8F8();
    }

    switch (arg0->unk5) {
        case 0:
            arg0->unk44 += subEntry->unk18;
            if (subEntry->unk18 > 0) {
                if ((s16)arg0->unk44 > subEntry->unk1C) {
                    arg0->unk5 = 1;
                }
            } else {
                if ((s16)arg0->unk44 < -subEntry->unk1C) {
                    arg0->unk5 = 1;
                }
            }
            break;

        case 1:
            arg0->unk44 -= subEntry->unk18;
            if (subEntry->unk18 > 0) {
                if ((s16)arg0->unk44 < -subEntry->unk1C) {
                    arg0->unk5 = 0;
                }
            } else {
                if ((s16)arg0->unk44 > subEntry->unk1C) {
                    arg0->unk5 = 0;
                }
            }
            break;
    }

    createYRotationMatrix(yRotMatrix, subEntry->unk16);
    createZRotationMatrix(zRotMatrix, arg0->unk44);
    func_8006B084_6BC84(zRotMatrix, yRotMatrix, &arg0->unk8);

    arg0->unk1C = subEntry->unk8;
    arg0->unk20 = subEntry->unkC;
    arg0->unk24 = subEntry->unk10;

    func_800007C4_13C4(arg0->unk0, &arg0->unk8);
}

void func_80000BF4_17F4(func_80000BF4_17F4_arg *arg0) {
    arg0->unk30 = freeNodeMemory(arg0->unk30);
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}

void func_80000C2C_182C(func_80000C2C_182C_arg *arg0) {
    DataEntry *entry = &D_800891D4_89DD4[arg0->unk0->unk84];
    SubEntry *subEntry = &entry->sub_entries[arg0->unk4];
    setCleanupCallback(&func_80000DA4_19A4);
    func_80009E68_AA68(&arg0->unk8, subEntry->unk16);
    setCallback(&func_80000CAC_18AC);
}

void func_80000CAC_18AC(func_80000C2C_182C_arg *arg0) {
    DataEntry *entry = &D_800891D4_89DD4[arg0->unk0->unk84];
    SubEntryVariant *subEntry = (SubEntryVariant *)&entry->sub_entries[arg0->unk4];

    if (arg0->unk0->unk86 != 0) {
        func_80069CF8_6A8F8();
    }

    switch (arg0->unk5) {
        case 0:
            func_80009F90_AB90(&arg0->unk8, 0x10000, subEntry->unk1A, -1);
            arg0->unk5 = 1;
            break;
        case 1:
            func_8000A030_AC30(&arg0->unk8, 0x10000);
            break;
    }

    if (arg0->unk0->unk87 != 0) {
        func_8000A13C_AD3C(
            &arg0->unk8,
            arg0->unk0->ptr->unk16,
            subEntry->unk8,
            subEntry->unkC,
            subEntry->unk10,
            subEntry->unk1C,
            subEntry->unk1C,
            0,
            0
        );
    }
}

void func_80000DA4_19A4(func_80000C2C_182C_arg *arg0) {
    func_80009F5C_AB5C(&arg0->unk8);
}

INCLUDE_ASM("asm/nonmatchings/main", func_80000DC0_19C0);

void func_80000E84_1A84(func_80000C2C_182C_arg *arg0) {
    DataEntry *entry = &D_800891D4_89DD4[arg0->unk0->unk84];
    SubEntry *subEntry = &entry->sub_entries[arg0->unk4];

    if (arg0->unk0->unk86 != 0) {
        func_80069CF8_6A8F8();
    }

    arg0->unk1C = subEntry->unk8;
    arg0->unk20 = subEntry->unkC;
    arg0->unk24 = subEntry->unk10;
    func_800007C4_13C4(arg0->unk0, &arg0->unk8);
}

void func_80000F14_1B14(func_80000BF4_17F4_arg *arg0) {
    arg0->unk30 = freeNodeMemory(arg0->unk30);
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}

INCLUDE_ASM("asm/nonmatchings/main", func_80000F4C_1B4C);

void func_80001040_1C40(func_80000C2C_182C_arg *arg0) {
    DataEntry *entry;
    SubEntry *subEntry;

    entry = &D_800891D4_89DD4[arg0->unk0->unk84];
    subEntry = &entry->sub_entries[arg0->unk4];

    if (arg0->unk0->unk86 != 0) {
        func_80069CF8_6A8F8();
    }

    arg0->unk4C = arg0->unk4C + arg0->unk50;
    arg0->unk4E = arg0->unk4E + arg0->unk52;
    arg0->unk4C = (u8)arg0->unk4C;
    arg0->unk4E = (u8)arg0->unk4E;
    arg0->unk1C = subEntry->unk8;
    arg0->unk20 = subEntry->unkC;
    arg0->unk24 = subEntry->unk10;

    if (arg0->unk0->unk87 != 0) {
        func_80004FF8_5BF8(arg0->unk0->ptr->unk16, &arg0->unk8);
    }
}

void func_80001114_1D14(func_80001114_1D14_arg *arg0) {
    arg0->unk44 = freeNodeMemory(arg0->unk44);
    arg0->unk30 = freeNodeMemory(arg0->unk30);
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}

void func_80001158_1D58(func_80000C2C_182C_arg *arg0) {
    DataEntry *entry = &D_800891D4_89DD4[arg0->unk0->unk84];
    SubEntry *subEntry = &entry->sub_entries[arg0->unk4];
    setCleanupCallback(&func_80001264_1E64);
    func_80009E68_AA68(&arg0->unk8, subEntry->unk16);
    *(s16 *)&arg0->unk54 = 0;
    setCallback(&func_800011DC_1DDC);
}

INCLUDE_ASM("asm/nonmatchings/main", func_800011DC_1DDC);

void func_80001264_1E64(func_80000C2C_182C_arg *arg0) {
    func_80009F5C_AB5C(&arg0->unk8);
}

void func_80001280_1E80(func_80000C2C_182C_arg *arg0) {
    volatile s32 sp10;
    volatile s32 sp14;
    volatile s32 sp18;
    volatile s32 sp1C;
    volatile s32 sp20[8];
    DataEntry *entry;
    SubEntry *subEntry;

    entry = &D_800891D4_89DD4[arg0->unk0->unk84];
    subEntry = &entry->sub_entries[arg0->unk4];

    memcpy(&sp20, &identityMatrix, 0x20);
    setCleanupCallback(&func_800014C8_20C8);
    func_80009E68_AA68(&arg0->unk8, subEntry->unk16);

    sp10 = ((randA() & 0x1F) - 0x10) << 0x10;
    sp14 = ((randA() & 1) - 4) << 0x10;
    sp18 = 0;

    createYRotationMatrix(&sp20, 0x1D83);
    transformVector(&sp10, &sp20, &arg0->unk54);

    arg0->unk54 = arg0->unk54 + subEntry->unk8;
    arg0->unk58 = arg0->unk58 + subEntry->unkC;
    arg0->unk5C = arg0->unk5C + subEntry->unk10;
    arg0->unk60 = D_800894F0_8A0F0[randA() & 3];

    setCallback(&func_800013B8_1FB8);
}

void func_800013B8_1FB8(func_80000C2C_182C_arg *arg0) {
    DataEntry *entry;
    SubEntryVariant *subEntry;

    entry = &D_800891D4_89DD4[arg0->unk0->unk84];
    subEntry = (SubEntryVariant *)&entry->sub_entries[arg0->unk4];

    if (arg0->unk0->unk86 != 0) {
        func_80069CF8_6A8F8();
    }

    switch (arg0->unk5) {
        case 0:
            func_80009F90_AB90(&arg0->unk8, 0x10000, subEntry->unk1A, -1);
            arg0->unk5 = 1;
            break;
        case 1:
            if (func_8000A030_AC30(&arg0->unk8, 0x10000) != 0) {
                func_80069CF8_6A8F8();
                return;
            }
            break;
    }

    if (arg0->unk0->unk87 != 0) {
        func_8000A13C_AD3C(
            &arg0->unk8,
            arg0->unk0->ptr->unk16,
            arg0->unk54,
            arg0->unk58,
            arg0->unk5C,
            arg0->unk60,
            arg0->unk60,
            0,
            0
        );
    }
}

void func_800014C8_20C8(func_80000C2C_182C_arg *arg0) {
    func_80009F5C_AB5C(&arg0->unk8);
}
