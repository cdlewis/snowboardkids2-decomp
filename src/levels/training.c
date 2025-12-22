#include "../3E160.h"
#include "36B80.h"
#include "56910.h"
#include "common.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_40E1C0);

extern void func_8006D7B0_6E3B0(s32, s16, s16, s16, s16, u8, u8, u8, u8, u8);
extern s32 gControllerInputs;
extern s8 *D_800BCB94_B7474[];

typedef struct {
    void *unk0;
    void *unk4;
    void *unk8;
    s16 unkC;
} func_800BB2B0_B5B90_state;

typedef struct {
    s32 unk0;
    void *unk4;
    s32 *unk8;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 unk16;
    u8 unk17;
} func_800BB5CC_state;

#define GET_UNK15(arg0) (((u8 *)&(arg0)->unk14)[1])

void func_800BB928_B6208(func_800BB5CC_state *arg0);

void func_800BB9E0_B62C0(func_800BB2B0_B5B90_state *arg0);

void func_800BB320_B5C00(func_800BB5CC_state *arg0);

void func_800BB2B0_B5B90(func_800BB2B0_B5B90_state *arg0) {
    getCurrentAllocation();
    arg0->unk4 = func_80035F80_36B80(1);
    arg0->unk0 = loadAsset_34F7E0();
    arg0->unk8 = dmaRequestAndUpdateStateWithSize(&_40E1C0_ROM_START, &_40E1C0_ROM_END, 0x1130);
    arg0->unkC = 0;
    setCleanupCallback(func_800BB9E0_B62C0);
    setCallback(func_800BB320_B5C00);
}

void func_800BB51C_B5DFC(func_800BB5CC_state *arg0);

void func_800BB320_B5C00(func_800BB5CC_state *arg0) {
    GameState *state = getCurrentAllocation();

    if (state->gamePaused == 0) {
        switch (arg0->unkC) {
        case 0:
            if (state->unk79 != 0) {
                break;
            }
            goto do_action;
        case 1:
            if ((state->players->unkB94 < 3) || (state->players->unkB84 & 1)) {
                break;
            }
            goto do_action;
        case 2:
            if (state->players->unkB94 < 0xE) {
                break;
            }
            goto do_action;
        case 3:
            if ((state->players->unkB94 < 0x12) || (state->players->unkB84 & 1)) {
                break;
            }
            goto do_action;
        case 4:
            if (state->players->unkB94 < 0x14) {
                break;
            }
            goto do_action;
        case 5:
            if (state->players->unkB94 < 0x1A) {
                break;
            }
            goto do_action;
        case 6:
            if (state->players->unkB94 < 0x28) {
                break;
            }
            goto do_action;
        case 7:
            if (state->players->unkB94 < 0x2D) {
                break;
            }
            goto do_action;
        case 8:
            if (state->players->unkB94 < 0x30) {
                break;
            }
            goto do_action;
        case 9:
            if (state->players->unkB94 < 0x32) {
                break;
            }
            goto do_action;
        case 10:
            if (state->players->unkB94 < 0x35) {
                break;
            }
            goto do_action;
        case 11:
            if (state->players->unkB94 < 0x38) {
                break;
            }
            goto do_action;
        case 12:
            if ((state->players->unkBC5 == 0) || (state->players->unkB94 == 0)) {
                break;
            }
            goto do_action;
        case 13:
            break;
        default:
        do_action:
            state->unk78 = 1;
            func_800585C8_591C8(0x2C);
            arg0->unk10 = 1;
            arg0->unk12 = 1;
            arg0->unk14 = 0xF0;
            arg0->unkE = 0;
            *(s16 *)&arg0->unk16 = 0xC0;
            setCallback(func_800BB51C_B5DFC);
            break;
        }
    }
}

void func_800BB670_B5F50(func_800BB5CC_state *arg0);
void func_800BB724_B6004(func_800BB5CC_state *arg0);
void func_800BB87C_B615C(func_800BB5CC_state *arg0);

void func_800BB51C_B5DFC(func_800BB5CC_state *arg0) {
    s16 temp_a7;
    s16 temp_a6;

    arg0->unk10 += 2;

    if (arg0->unk14 != 0x40) {
        arg0->unk14 -= 0x10;
    }

    if (arg0->unk10 == 0xD) {
        setCallback(func_800BB670_B5F50);
    }

    temp_a7 = arg0->unk10;
    temp_a6 = arg0->unk12;

    func_8006D7B0_6E3B0(
        (s32)arg0->unk0,
        ((-temp_a7) << 19) >> 16,
        ((-temp_a6) << 19) >> 16,
        temp_a7,
        temp_a6,
        0,
        GET_UNK15(arg0),
        arg0->unk17,
        0xC,
        0x6
    );
}

void func_800BB5CC_B5EAC(func_800BB5CC_state *arg0) {
    s16 temp_a7;
    s16 temp_a6;

    arg0->unk12--;
    arg0->unk14 += 0x10;

    if (arg0->unk12 == 1) {
        setCallback(func_800BB670_B5F50);
    }

    temp_a7 = arg0->unk10;
    temp_a6 = arg0->unk12;

    func_8006D7B0_6E3B0(
        (s32)arg0->unk0,
        ((-temp_a7) << 19) >> 16,
        ((-temp_a6) << 19) >> 16,
        temp_a7,
        temp_a6,
        0,
        (u8)arg0->unk14,
        arg0->unk17,
        0xC,
        0x6
    );
}

void func_800BB670_B5F50(func_800BB5CC_state *arg0) {
    s16 temp_a7;
    s16 temp_a6;

    arg0->unk12++;

    if (arg0->unk14 != 0x40) {
        arg0->unk14 -= 0x10;
    }

    if (arg0->unk12 == 6) {
        setCallback(func_800BB724_B6004);
    }

    temp_a7 = arg0->unk10;
    temp_a6 = arg0->unk12;

    func_8006D7B0_6E3B0(
        (s32)arg0->unk0,
        ((-temp_a7) << 19) >> 16,
        ((-temp_a6) << 19) >> 16,
        temp_a7,
        temp_a6,
        0,
        GET_UNK15(arg0),
        arg0->unk17,
        0xC,
        0x6
    );
}

void func_800BB724_B6004(func_800BB5CC_state *arg0) {
    s8 *table_ptr;
    s32 temp_v1_2;
    s16 temp_v0;
    s8 temp_v1_3;
    s32 s0_var = 6;
    s32 s1_var = 0xC;

    table_ptr = D_800BCB94_B7474[arg0->unkC];
    temp_v1_2 = arg0->unk8[table_ptr[arg0->unkE]];
    func_80035260_35E60(arg0->unk4, (void *)arg0->unk8 + temp_v1_2, -0x68, -0x30, 0xFF, 0xFF, 0, s1_var, s0_var);

    func_8006D7B0_6E3B0(arg0->unk0, -0x68, -0x30, 0xD, s0_var, 1, GET_UNK15(arg0), arg0->unk17, s1_var, s0_var);

    if (gControllerInputs & A_BUTTON) {
        temp_v0 = arg0->unkE + 1;
        arg0->unkE = temp_v0;
        table_ptr = D_800BCB94_B7474[arg0->unkC];
        temp_v1_3 = table_ptr[temp_v0];
        if (temp_v1_3 == -1) {
            func_800585C8_591C8(0x2D);
            arg0->unkC = arg0->unkC + 1;
            setCallback(func_800BB87C_B615C);
        } else {
            func_800585C8_591C8(0x2B);
            setCallback(func_800BB5CC_B5EAC);
        }
    }
}

#define GET_UNK16_AS_S16(arg0) (*(s16 *)&(arg0)->unk16)
#define SET_UNK16_AS_S16(arg0, val) (*(s16 *)&(arg0)->unk16 = (val))

void func_800BB87C_B615C(func_800BB5CC_state *arg0) {
    s16 temp_v1;
    s16 temp_a7;
    s16 temp_a6;

    arg0->unk12--;

    temp_v1 = GET_UNK16_AS_S16(arg0);
    if (temp_v1 != 0) {
        SET_UNK16_AS_S16(arg0, temp_v1 - 0x10);
    }

    if (arg0->unk12 == 1) {
        setCallback(func_800BB928_B6208);
    }

    temp_a7 = arg0->unk10;
    temp_a6 = arg0->unk12;

    func_8006D7B0_6E3B0(
        (s32)arg0->unk0,
        ((-temp_a7) << 19) >> 16,
        ((-temp_a6) << 19) >> 16,
        temp_a7,
        temp_a6,
        0,
        (u8)arg0->unk14,
        arg0->unk17,
        0xC,
        0x6
    );
}

void func_800BB928_B6208(func_800BB5CC_state *arg0) {
    s16 temp_v1;
    s16 temp_a7;
    s16 temp_a6;
    s8 *alloc;

    alloc = getCurrentAllocation();
    arg0->unk10--;

    temp_v1 = GET_UNK16_AS_S16(arg0);
    if (temp_v1 != 0) {
        SET_UNK16_AS_S16(arg0, temp_v1 - 0x10);
    }

    if (arg0->unk10 == 1) {
        alloc[0x78] = -1;
        setCallback(func_800BB320_B5C00);
    }

    temp_a7 = arg0->unk10;
    temp_a6 = arg0->unk12;

    func_8006D7B0_6E3B0(
        arg0->unk0,
        ((-temp_a7) << 19) >> 16,
        ((-temp_a6) << 19) >> 16,
        temp_a7,
        temp_a6,
        0,
        GET_UNK15(arg0),
        arg0->unk17,
        0xC,
        0x6
    );
}

void func_800BB9E0_B62C0(func_800BB2B0_B5B90_state *arg0) {
    arg0->unk0 = freeNodeMemory(arg0->unk0);
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk8 = freeNodeMemory(arg0->unk8);
}
