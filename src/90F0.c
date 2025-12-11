#include "90F0.h"

#include "6E840.h"
#include "common.h"
#include "task_scheduler.h"

extern s16 D_8008C930_8D530[][10];
extern s32 D_8008C920_8D520[];

s32 func_800084F0_90F0(void) {
    return 10;
}

s16 func_800084F8_90F8(s32 arg0) {
    return D_8008C930_8D530[arg0][0];
}

INCLUDE_ASM("asm/nonmatchings/90F0", func_80008514_9114);

INCLUDE_ASM("asm/nonmatchings/90F0", func_80008D18_9918);

INCLUDE_ASM("asm/nonmatchings/90F0", func_80009548_A148);

extern void func_80008514_9114(void *);
extern void func_80008D18_9918(void);
extern void func_80009548_A148(void);

void func_80009D58_A958(u16 arg0, Node *arg1) {
    arg1->unk18 = NULL;
    arg1->unk1C = NULL;
    arg1->callback = NULL;
    arg1->cleanupCallback = NULL;
    debugEnqueueCallback(arg0, 4, &func_80008514_9114, arg1);
}

void func_80009D94_A994(u16 arg0, Node *arg1) {
    arg1->unk18 = NULL;
    arg1->unk1C = NULL;
    arg1->callback = NULL;
    arg1->cleanupCallback = NULL;
    debugEnqueueCallback(arg0, 4, &func_80008D18_9918, arg1);
}

void func_80009DD0_A9D0(u16 arg0, Node *arg1) {
    arg1->unk18 = NULL;
    arg1->unk1C = NULL;
    arg1->callback = NULL;
    arg1->cleanupCallback = NULL;
    debugEnqueueCallback(arg0, 4, &func_80009548_A148, arg1);
}

void func_80009E0C_AA0C(void *arg0) {
    ((u8 *)arg0)[6] |= 1;
}

void func_80009E1C_AA1C(void *arg0) {
    ((u8 *)arg0)[6] &= 0xFE;
}

s32 func_80009E2C_AA2C(void *arg0) {
    return ((u8 *)arg0)[6] & 1;
}

void func_80009E38_AA38(void *arg0) {
    ((u8 *)arg0)[6] |= 2;
}

void func_80009E48_AA48(void *arg0) {
    ((u8 *)arg0)[6] &= 0xFD;
}

s32 func_80009E58_AA58(void *arg0) {
    u8 val;
    val = ((u8 *)arg0)[6] & 2;
    return val != 0;
}

s32 func_80009E68_AA68(void *arg0, s16 arg1) {
    s32 *entry;
    s16 temp_s0;

    temp_s0 = arg1;
    if (temp_s0 >= func_800084F0_90F0()) {
        *(void **)arg0 = NULL;
        ((s16 *)arg0)[2] = 0;
        return 0;
    }
    entry = &D_8008C920_8D520[temp_s0 * 5];
    *(void **)arg0 = dmaRequestAndUpdateStateWithSize((void *)entry[0], (void *)entry[1], entry[2]);
    ((s16 *)arg0)[2] = arg1;
    ((u8 *)arg0)[6] = 0;
    return 1;
}

void *func_80009EF8_AAF8(s16 arg0) {
    s32 *entry;

    if (arg0 >= func_800084F0_90F0()) {
        return NULL;
    }
    entry = &D_8008C920_8D520[arg0 * 5];
    return dmaRequestAndUpdateStateWithSize((void *)entry[0], (void *)entry[1], entry[2]);
}

void func_80009F5C_AB5C(func_80009F5C_AB5C_arg **arg0) {
    if (arg0 != NULL) {
        *arg0 = (func_80009F5C_AB5C_arg *)freeNodeMemory(*arg0);
    }
}

INCLUDE_ASM("asm/nonmatchings/90F0", func_80009F90_AB90);

typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ u16 unk4;
    /* 0x06 */ u16 unk6;
} AnimationEntry;

typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ void *unk8;
    /* 0x0C */ AnimationEntry *unkC;
    /* 0x10 */ u16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
} AnimationState;

s32 func_8000A030_AC30(AnimationState *arg0, s32 arg1) {
    AnimationState *state = arg0;
    s32 result = 0;
    s32 looped = 0;
    s16 timer = state->unk16;
    AnimationEntry *entry;
    s16 type;

    if (timer > 0) {
        state->unk16 = timer - 1;
    } else {
        state->unk14++;
        if (state->unk14 >= ((s16 *)state->unk8)[2]) {
            state->unk14 = 0;
            looped = 1;
        }

        entry = &state->unkC[state->unk14];
        type = entry->unk2;

        if (type != 1) {
            if (type >= 2) {
                goto case_default;
            }
            if (type != 0) {
                goto case_default;
            }
            // type == 0
            state->unk12 = entry->unk4;
            state->unk16 = entry->unk6;
        } else {
            // type == 1
            state->unk14 = entry->unk6;
            entry = &state->unkC[state->unk14];
            state->unk12 = entry->unk4;
            state->unk16 = entry->unk6;
            result = 1;
        }
        goto end;

    case_default:
        state->unk14--;
        entry = &state->unkC[state->unk14];
        state->unk12 = entry->unk4;
        state->unk16 = entry->unk6;
        result = 2;
    }

end:
    if (looped == 1) {
        result = 1;
    }

    return result;
}

extern void func_8000A1E4_ADE4(s32, s32, s32, s32, s32, s32, s32, s16, u8, u8);

typedef void (*func_8000A1E4_ADE4_11_t)(s32, s32, s32, s32, s32, s32, s32, s16, u8, u8, s32);

void func_8000A13C_AD3C(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s16 arg7, u8 arg8) {
    ((func_8000A1E4_ADE4_11_t)func_8000A1E4_ADE4)((s32)arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, 0xFF, 0);
}

void func_8000A190_AD90(
    s32 arg0,
    s32 arg1,
    s32 arg2,
    s32 arg3,
    s32 arg4,
    s32 arg5,
    s32 arg6,
    s16 arg7,
    u8 arg8,
    u8 arg9
) {
    s32 pad[2];

    pad[0] = 0;
    func_8000A1E4_ADE4(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
}

INCLUDE_ASM("asm/nonmatchings/90F0", func_8000A1E4_ADE4);

u16 func_8000A410_B010(func_8000A410_B010_arg *arg0) {
    OutputStruct_19E80 output;
    getTableEntryByU16Index(arg0->table, arg0->index, &output);
    return output.field1;
}
