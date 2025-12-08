#include "20F0.h"
#include "38C90.h"
#include "3E160.h"
#include "4050.h"
#include "6E840.h"
#include "common.h"
#include "graphics.h"
#include "task_scheduler.h"

extern s16 D_8008CE40_8DA40;

typedef struct {
    s32 value;
    void *ptr;
} ArrayElement_8;

extern ArrayElement_8 D_8008CEA0_8DAA0[];

typedef struct {
    void *ptr;
    s32 count;
} LookupEntry;

extern LookupEntry D_8008CE9C_8DA9C[];

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad[5];
    s8 unk7;
    u8 pad2[4];
} TableElement_F9B0;

typedef struct {
    TableElement_F9B0 *elements;
    s32 unk4;
} D_8008D348_Entry;

extern D_8008D348_Entry D_8008D348_8DF48[];
extern u8 func_80038070_38C70(void);

typedef struct {
    s8 unk0;
    s8 unk1;
    s8 unk2;
    s8 unk3;
    u8 pad4[0x4];
    void *unk8;
    void *unkC;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 unk16;
    u8 pad17;
    void *unk18;
    u8 pad1C[0x4];
    Node_70B00 unk20;
    Node_70B00 unk1F8;
    Node_70B00 unk3D0;
    u8 pad5A8[0x1C];
    void *unk5C4;
    u8 pad5C8[0x2C];
    void *unk5F4;
    u8 pad5F8[0x2C];
    SceneModel *unk624;
} E770_struct;

void func_8000DB70_E770(E770_struct *arg0, s16 arg1) {
    D_8008CE40_8DA40 = arg1;
    arg0->unk10 = 0x28;
    func_80057514_58114(arg1, 0x80, 0);
}

void func_8000DBA8_E7A8(E770_struct *arg0) {
    func_8006FDA0_709A0(&arg0->unk1F8, 0xFF, 10);
    arg0->unk12 = 10;
}

void func_8000DBE0_E7E0(E770_struct *arg0) {
    func_8006FDA0_709A0(&arg0->unk1F8, 0, 10);
    arg0->unk12 = 10;
}

s32 func_8000DC18_E818(E770_struct *arg0) {
    s8 temp = arg0->unk2;

    if (temp < 2) {
        return D_8008CEA0_8DAA0[temp].value;
    }
    return 0;
}

void *func_8000DC44_E844(E770_struct *arg0, s32 arg1) {
    void *result;
    s8 temp;
    LookupEntry *entry;

    result = NULL;
    temp = arg0->unk2;

    if (temp < 2) {
        entry = &D_8008CE9C_8DA9C[temp];
        if (arg1 < entry->count) {
            result = (u8 *)entry->ptr + arg1 * 8;
        }
    }
    return result;
}

void func_8000DC88_E888(E770_struct *arg0, s16 arg1, s16 arg2, s8 arg3, s16 arg4) {
    func_800021E8_2DE8(arg0->unk624, arg1, arg2, arg3);
    arg0->unk14 = arg4;
}

INCLUDE_ASM("asm/nonmatchings/E770", func_8000DCD8_E8D8);

void func_8000E154_ED54(E770_struct *arg0) {
    func_80002014_2C14(arg0->unk624);
    arg0->unk18 = freeNodeMemory(arg0->unk18);
    arg0->unkC = freeNodeMemory(arg0->unkC);
    arg0->unk8 = freeNodeMemory(arg0->unk8);
    arg0->unk5C4 = freeNodeMemory(arg0->unk5C4);
    arg0->unk5F4 = freeNodeMemory(arg0->unk5F4);
    unlinkNode(&arg0->unk3D0);
    unlinkNode(&arg0->unk20);
    unlinkNode(&arg0->unk1F8);
}

void func_8000E1D0_EDD0(E770_struct *arg0) {
    func_800394BC_3A0BC(&arg0->pad5C8, (s32)arg0->unk5C4);
    func_800394BC_3A0BC(&arg0->pad5F8, (s32)arg0->unk5F4);
    func_8006FDA0_709A0(NULL, 0, 10);
    func_8000DC88_E888(arg0, 0x90, 0x90, -1, 0);
    arg0->unk12 = 10;
    arg0->unk0 = 2;
}

void func_8000E240_EE40(E770_struct *arg0) {
    s16 temp = arg0->unk12;

    if (temp == 0) {
        if (arg0->unk624->unk16 == 0x15B) {
            func_8000DC88_E888(arg0, 0x15C, 0x90, 1, -1);
        }
        arg0->unk0 = 2;
    } else {
        arg0->unk12 = temp - 1;
    }
}

extern s32 gControllerInputs;

void func_8000E2AC_EEAC(E770_struct *arg0) {
    s32 inputs;
    u8 selection;
    s8 sel_signed;
    s32 sound;
    s32 sel2;

    inputs = gControllerInputs;
    selection = arg0->unk1;

    // B button - exit
    if (inputs & 0x4000) {
        arg0->unk0 = 5;
        return;
    }

    // A button - confirm
    if (inputs & 0x8000) {
        func_8000DBA8_E7A8(arg0);
        if (arg0->unk1 == 5) {
            func_80057564_58164(10);
            func_8006FDA0_709A0(0, 0xFF, 10);
        }
        func_8000DC88_E888(arg0, 0x15A, 0x15B, 1, 0x14);
        sound = 0xD3;
        arg0->unk0 = 3;
        goto play_sound;
    }

    // Left / D-pad left (0x80200)
    if (inputs & 0x80200) {
        sel_signed = (s8)selection;
        if (sel_signed <= 0) {
            goto check_bounds;
        }
        if (sel_signed < 3) {
            arg0->unk1 = selection - 1;
            goto check_bounds;
        }
        if (sel_signed >= 6) {
            goto check_bounds;
        }
        if (sel_signed < 4) {
            goto check_bounds;
        }
        arg0->unk1 = selection - 1;
        goto check_bounds;
    }

    // Right / D-pad right (0x40100)
    if (inputs & 0x40100) {
        sel_signed = (s8)selection;
        if (sel_signed < 0) {
            goto set_zero;
        }
        if (sel_signed < 2) {
            arg0->unk1 = selection + 1;
            goto check_bounds;
        }
        if (sel_signed >= 5) {
            goto check_bounds;
        }
        if (sel_signed < 3) {
            goto check_bounds;
        }
        arg0->unk1 = selection + 1;
        goto check_bounds;
    }

    // Up / D-pad up (0x10800)
    if (inputs & 0x10800) {
        sel_signed = (s8)selection;
        if (sel_signed >= 6) {
            goto check_bounds;
        }
        if (sel_signed < 3) {
            goto check_bounds;
        }
        arg0->unk1 = selection - 2;
        if (arg0->unk2 == 0) {
            goto check_bounds;
        }
        arg0->unk1 = selection - 3;
        goto check_bounds;
    }

    // Down / D-pad down (0x20400)
    if (!(inputs & 0x20400)) {
        goto check_bounds;
    }
    sel_signed = (s8)selection;
    if (sel_signed == 0) {
        goto add_three;
    }
    if (sel_signed < 0) {
        goto check_bounds;
    }
    if (sel_signed >= 3) {
        goto check_bounds;
    }
    if (arg0->unk2 != 0) {
        goto add_three;
    }
    arg0->unk1 = selection - 1;

add_three:
    arg0->unk1 = arg0->unk1 + 3;

check_bounds:
    if (arg0->unk1 >= 0) {
        goto check_max;
    }

set_zero:
    arg0->unk1 = 0;

check_max:
    sel2 = arg0->unk1;
    if (func_8000DC18_E818(arg0) - 1 < sel2) {
        arg0->unk1 = func_8000DC18_E818(arg0) - 1;
    }

    if (arg0->unk1 == (s8)selection) {
        return;
    }
    sound = 0x2B;

play_sound:
    func_800585C8_591C8(sound);
}

typedef struct FD98_struct {
    s8 unk0;
    s8 unk1;
    s8 unk2;
    u8 unk3;
    u8 unk4;
    u8 pad5[0x3];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
} FD98_struct;

extern void func_8000FD98_10998(FD98_struct *);

void func_8000E4CC_F0CC(E770_struct *arg0) {
    s16 temp = arg0->unk12;

    if (temp == 0) {
        if (arg0->unk1 == 5) {
            func_8000E154_ED54(arg0);
            func_80003450_4050(0, 2);
            createTaskQueue(loadCutsceneOverlay, 0x64);
            arg0->unk16 = 1;
            arg0->unk0 = 7;
        } else {
            scheduleTask(func_8000FD98_10998, 0, 0, 0);
            arg0->unk3 = 0;
            arg0->unk0 = 4;
        }
    } else {
        arg0->unk12 = temp - 1;
    }
}

void func_8000E56C_F16C(E770_struct *arg0) {
    if (arg0->unk3 != 0) {
        terminateTasksByType(1);
        func_8000DBE0_E7E0(arg0);
        arg0->unk0 = 1;
    }
}

void func_8000E5B0_F1B0(E770_struct *arg0) {
    func_800585C8_591C8(0xDB);
    func_8006FDA0_709A0(NULL, 0xFF, 10);
    func_8000DC88_E888(arg0, 0x15A, 0x15B, 1, -1);
    arg0->unk12 = 10;
    arg0->unk0 = 6;
}

extern void func_8000ED88_F988(void);

s32 func_8000E614_F214(E770_struct *arg0) {
    s16 temp;

    if (arg0->unk12 == 8) {
        func_80057564_58164(8);
    }
    temp = arg0->unk12;
    if (temp == 0) {
        func_8000E154_ED54(arg0);
        terminateSchedulerWithCallback(func_8000ED88_F988);
        return 1;
    }
    arg0->unk12 = temp - 1;
    return 0;
}

extern void func_8000E6E0_F2E0(void);

void func_8000E680_F280(void) {
    allocateTaskMemory(0xCC0);
    D_8008CE40_8DA40 = 5;
    setupTaskSchedulerNodes(0x40, 4, 0, 0, 0, 0, 0, 0);
    func_800585C8_591C8(0xD0);
    setGameStateHandler(func_8000E6E0_F2E0);
}

INCLUDE_ASM("asm/nonmatchings/E770", func_8000E6E0_F2E0);

extern s16 func_80069810_6A410(void);
extern void func_8000DCD8_E8D8(E770_struct *);

void func_8000EC98_F898(void) {
    E770_struct *s0;

    s0 = getCurrentAllocation();
    switch (s0->unk0) {
        case 0:
            func_8000E1D0_EDD0(s0);
            break;
        case 1:
            func_8000E240_EE40(s0);
            break;
        case 2:
            func_8000E2AC_EEAC(s0);
            break;
        case 3:
            func_8000E4CC_F0CC(s0);
            break;
        case 4:
            func_8000E56C_F16C(s0);
            break;
        case 5:
        default:
            func_8000E5B0_F1B0(s0);
            break;
        case 6:
            if (func_8000E614_F214(s0)) {
                return;
            }
            break;
        case 7:
            if ((s16)func_80069810_6A410()) {
                setGameStateHandler(func_8000E6E0_F2E0);
            }
            return;
    }
    func_8000DCD8_E8D8(s0);
}

void func_8000ED88_F988(void) {
    D_8008CE40_8DA40 = 5;
    func_800697F4_6A3F4(1);
}

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEB0_9EAB0);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEB4_9EAB4);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEB8_9EAB8);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEBC_9EABC);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEC0_9EAC0);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEC4_9EAC4);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEC8_9EAC8);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DECC_9EACC);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DED0_9EAD0);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DED4_9EAD4);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DED8_9EAD8);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEDC_9EADC);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEE0_9EAE0);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEE4_9EAE4);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEE8_9EAE8);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEEC_9EAEC);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEF0_9EAF0);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEF4_9EAF4);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEF8_9EAF8);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEFC_9EAFC);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF00_9EB00);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF04_9EB04);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF08_9EB08);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF0C_9EB0C);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF10_9EB10);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF14_9EB14);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF18_9EB18);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF1C_9EB1C);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF20_9EB20);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF24_9EB24);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF28_9EB28);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF2C_9EB2C);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF30_9EB30);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF34_9EB34);

u8 func_8000EDB0_F9B0(u8 arg0) {
    E770_struct *alloc;
    s8 idx;
    TableElement_F9B0 *entry;
    s8 unk7;
    u8 arg0_masked;

    alloc = getCurrentAllocation();
    idx = alloc->unk1;
    arg0_masked = arg0;
    entry = &D_8008D348_8DF48[idx].elements[arg0_masked];

    switch (idx) {
        case 1:
            return func_80038058_38C58(arg0) & 0xFF;
        case 2:
            return func_80038000_38C00(arg0) & 0xFF;
        case 3:
            unk7 = entry->unk7;
            if (unk7 == -1) {
                return 1;
            }
            if (unk7 == -2) {
                return func_80038070_38C70() & 0xFF;
            }
            return func_80037FE0_38BE0(unk7 & 0xFF) & 0xFF;
        case 0:
        case 4:
            return func_80037FE0_38BE0(arg0) & 0xFF;
        case 5:
            return func_80038070_38C70() & 0xFF;
    }
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/E770", func_8000EE88_FA88);

void func_8000F4BC_100BC(E770_struct *arg0) {
    void *alloc = getCurrentAllocation();
    arg0->unk0 = 1;
    func_8000DBE0_E7E0(alloc);
}

void func_8000F4F0_100F0(E770_struct *arg0) {
    E770_struct *alloc = getCurrentAllocation();
    s16 temp;

    temp = alloc->unk12;
    if (temp == 0) {
        if (alloc->unk624->unk16 == 0x15B) {
            func_8000DC88_E888(alloc, 0x15C, 0x90, 1, -1);
        }
        arg0->unk0 = 2;
    } else {
        alloc->unk12 = temp - 1;
    }
}

void func_8000F564_10164(FD98_struct *arg0) {
    s32 inputs;
    s8 temp;
    u8 val;

    inputs = gControllerInputs;

    if (inputs & 0x80200) {
        if (arg0->unk2 == 0) {
            arg0->unk2 = 14;
        } else {
            arg0->unk2 = arg0->unk2 - 1;
        }
        func_800585C8_591C8(0x2B);
        return;
    }

    if (inputs & 0x40100) {
        if (arg0->unk2 == 14) {
            arg0->unk2 = 0;
        } else {
            arg0->unk2 = arg0->unk2 + 1;
        }
        func_800585C8_591C8(0x2B);
        return;
    }

    if (inputs & 0x10800) {
        val = arg0->unk2;
        if ((u8)(val - 8) < 7u) {
            arg0->unk2 = val - 7;
        } else {
            if (val == 0) {
                arg0->unk3 = val + 8;
                arg0->unk4 = arg0->unk2;
            } else {
                arg0->unk3 = val + 7;
                arg0->unk4 = arg0->unk2;
            }
            arg0->unk1 = 1;
        }
        func_800585C8_591C8(0x2B);
        return;
    }

    if (inputs & 0x20400) {
        temp = arg0->unk2;
        if (temp == 0) {
            arg0->unk2 = 8;
        } else if ((u8)(temp - 1) < 7u) {
            arg0->unk2 = temp + 7;
        } else {
            arg0->unk3 = temp;
            arg0->unk1 = 1;
            arg0->unk4 = arg0->unk2 - 7;
        }
        func_800585C8_591C8(0x2B);
    }
}

void func_8000F690_10290(FD98_struct *arg0) {
    s32 inputs;
    s8 temp2;
    s8 newVal;

    inputs = gControllerInputs;

    if (inputs & 0x80200) {
        s8 temp = arg0->unk2;
        if (!temp) {
            newVal = 8;
        } else if (temp == 9) {
            newVal = 17;
        } else {
            newVal = temp - 1;
        }
        arg0->unk2 = newVal;
        func_800585C8_591C8(0x2B);
        return;
    }

    if (inputs & 0x40100) {
        s8 temp = arg0->unk2;
        if (temp == 8) {
            arg0->unk2 = 0;
        } else if (temp == 17) {
            arg0->unk2 = 9;
        } else {
            arg0->unk2 = temp + 1;
        }
        func_800585C8_591C8(0x2B);
        return;
    }

    if (inputs & 0x10800) {
        u8 val = arg0->unk2;
        temp2 = val - 9;
        if ((u8)temp2 < 9u) {
            arg0->unk2 = temp2;
        } else {
            arg0->unk3 = val + 9;
            arg0->unk1 = 1;
            arg0->unk4 = arg0->unk2;
        }
        func_800585C8_591C8(0x2B);
        return;
    }

    if (inputs & 0x20400) {
        u8 val = arg0->unk2;
        if (val < 9u) {
            arg0->unk2 = val + 9;
        } else {
            arg0->unk3 = val;
            arg0->unk1 = 1;
            arg0->unk4 = arg0->unk2 - 9;
        }
        func_800585C8_591C8(0x2B);
    }
}

void func_8000F7B0_103B0(FD98_struct *arg0) {
    s32 inputs;

    inputs = gControllerInputs;

    if (inputs & 0x80200) {
        if (arg0->unk2 == 0) {
            arg0->unk2 = 8;
        } else {
            arg0->unk2 = arg0->unk2 - 1;
        }
        func_800585C8_591C8(0x2B);
        return;
    }

    if (inputs & 0x40100) {
        if (arg0->unk2 == 8) {
            arg0->unk2 = 0;
        } else {
            arg0->unk2 = arg0->unk2 + 1;
        }
        func_800585C8_591C8(0x2B);
        return;
    }

    if (inputs & 0x10800) {
        goto set_values;
    }
    if (inputs & 0x20400) {
    set_values:
        arg0->unk3 = arg0->unk2;
        arg0->unk4 = arg0->unk2;
        arg0->unk1 = 1;
        func_800585C8_591C8(0x2B);
    }
}

INCLUDE_ASM("asm/nonmatchings/E770", func_8000F884_10484);

void func_8000FA90_10690(FD98_struct *arg0) {
    s32 inputs;
    s8 temp;
    u8 val;

    inputs = gControllerInputs;

    if (inputs & 0x80200) {
        if (arg0->unk2 == 0) {
            arg0->unk2 = 14;
        } else {
            arg0->unk2 = arg0->unk2 - 1;
        }
        func_800585C8_591C8(0x2B);
        return;
    }

    if (inputs & 0x40100) {
        if (arg0->unk2 == 14) {
            arg0->unk2 = 0;
        } else {
            arg0->unk2 = arg0->unk2 + 1;
        }
        func_800585C8_591C8(0x2B);
        return;
    }

    if (inputs & 0x10800) {
        val = arg0->unk2;
        if ((u8)(val - 8) < 7u) {
            arg0->unk2 = val - 7;
        } else {
            if (val == 0) {
                arg0->unk3 = val + 8;
                arg0->unk4 = arg0->unk2;
            } else {
                arg0->unk3 = val + 7;
                arg0->unk4 = arg0->unk2;
            }
            arg0->unk1 = 1;
        }
        func_800585C8_591C8(0x2B);
        return;
    }

    if (inputs & 0x20400) {
        temp = arg0->unk2;
        if (temp == 0) {
            arg0->unk2 = 8;
        } else if ((u8)(temp - 1) < 7u) {
            arg0->unk2 = temp + 7;
        } else {
            arg0->unk3 = temp;
            arg0->unk1 = 1;
            arg0->unk4 = arg0->unk2 - 7;
        }
        func_800585C8_591C8(0x2B);
    }
}

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF6C_9EB6C);

extern void func_8000F884_10484(FD98_struct *);

void func_8000FBBC_107BC(FD98_struct *arg0) {
    FD98_struct *alloc;
    s32 inputs;
    s8 temp;
    u8 newVal;
    s32 sound;

    alloc = getCurrentAllocation();
    inputs = gControllerInputs;

    if (inputs & 0x4000) {
        func_8000DC88_E888((E770_struct *)alloc, 0x90, 0x90, -1, -1);
        sound = 0x2E;
        arg0->unk0 = 3;
        goto play_sound;
    }

    temp = arg0->unk1;
    if (temp == 0) {
        goto do_switch;
    }
    if (temp == 1) {
        goto button_check;
    }
    return;

do_switch:
    switch (alloc->unk1) {
        case 0:
            func_8000F564_10164(arg0);
            break;
        case 1:
            func_8000F690_10290(arg0);
            break;
        case 2:
            func_8000F7B0_103B0(arg0);
            break;
        case 3:
            func_8000F884_10484(arg0);
            break;
        case 4:
            func_8000FA90_10690(arg0);
            break;
    }
    return;

button_check:
    if (inputs & 0x8000) {
        arg0->unk0 = 3;
        func_8000DC88_E888((E770_struct *)alloc, 0x90, 0x90, -1, -1);
        sound = 0x2E;
        goto play_sound;
    }
    if (inputs & 0x10800) {
        newVal = arg0->unk3;
        goto set_2b;
    }
    if (inputs & 0x20400) {
        newVal = arg0->unk4;
    set_2b:
        sound = 0x2B;
        arg0->unk1 = 0;
        arg0->unk2 = newVal;
    play_sound:
        func_800585C8_591C8(sound);
    }
}

void func_8000FD1C_1091C(E770_struct *arg0) {
    void *alloc = getCurrentAllocation();
    func_8000DBA8_E7A8(alloc);
    arg0->unk0 = 4;
}

s32 func_8000FD50_10950(E770_struct *arg0) {
    E770_struct *alloc = getCurrentAllocation();
    s16 temp = alloc->unk12;

    if (temp == 0) {
        func_80069CF8_6A8F8();
        return 1;
    }
    alloc->unk12 = temp - 1;
    return 0;
}

extern void func_8000FEA0_10AA0(void);
extern void func_8000FE00_10A00(E770_struct *);

void func_8000FD98_10998(FD98_struct *arg0) {
    getCurrentAllocation();
    setCleanupCallback(func_8000FEA0_10AA0);
    arg0->unk10 = 0x780000;
    arg0->unk14 = 0x0F0000;
    arg0->unk8 = 0xFF0000;
    arg0->unk0 = 0;
    arg0->unk1 = 0;
    arg0->unk2 = 0;
    arg0->unkC = 0xFFF10000;
    setCallback(func_8000FE00_10A00);
}

extern void func_8000EE88_FA88(E770_struct *);

void func_8000FE00_10A00(E770_struct *arg0) {
    getCurrentAllocation();
    switch (arg0->unk0) {
        case 0:
            func_8000F4BC_100BC(arg0);
            break;
        case 1:
            func_8000F4F0_100F0(arg0);
            break;
        case 2:
            func_8000FBBC_107BC((FD98_struct *)arg0);
            break;
        case 3:
            func_8000FD1C_1091C(arg0);
            break;
        case 4:
            if (func_8000FD50_10950(arg0)) {
                return;
            }
            break;
    }
    func_8000EE88_FA88(arg0);
}

void func_8000FEA0_10AA0(void) {
    E770_struct *alloc = getCurrentAllocation();
    alloc->unk3 = 1;
}
