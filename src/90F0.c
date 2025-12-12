#include "90F0.h"

#include "6E840.h"
#include "common.h"
#include "task_scheduler.h"

extern s16 D_8008C930_8D530[][10];
extern s32 D_8008C920_8D520[];

// Data pointers for different sprite dimension combinations
extern void *D_8008C9E8_8D5E8;
extern void *D_8008CA28_8D628;
extern void *D_8008CA68_8D668;
extern void *D_8008CAA8_8D6A8;
extern void *D_8008CAE8_8D6E8;
extern void *D_8008CB28_8D728;
extern void *D_8008CB68_8D768;
extern void *D_8008CBA8_8D7A8;

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

s32 func_8000A030_AC30(void *arg0, s32 arg1) {
    AnimationState *state = (AnimationState *)arg0;
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

void func_8000A1E4_ADE4(
    SpriteState *arg0,
    s32 arg1,
    s32 arg2,
    s32 arg3,
    s32 arg4,
    s32 arg5,
    s32 arg6,
    s16 arg7,
    u8 arg8,
    u8 arg9,
    u16 arg10
);

typedef void (*func_8000A1E4_ADE4_11_t)(SpriteState *, s32, s32, s32, s32, s32, s32, s16, u8, u8, s32);

void func_8000A13C_AD3C(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s16 arg7, u8 arg8) {
    ((func_8000A1E4_ADE4_11_t
    )func_8000A1E4_ADE4)((SpriteState *)arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, 0xFF, 0);
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
    ((void (*)(s32, s32, s32, s32, s32, s32, s32, s16, u8, u8)
    )func_8000A1E4_ADE4)(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
}

void func_8000A1E4_ADE4(
    SpriteState *arg0,
    s32 arg1,
    s32 arg2,
    s32 arg3,
    s32 arg4,
    s32 arg5,
    s32 arg6,
    s16 arg7,
    u8 arg8,
    u8 arg9,
    u16 arg10
) {
    OutputStruct_19E80 sp10;
    SpriteEntry *entry;
    s32 temp_v1;
    s32 sign;
    s32 temp;

    sign = -(arg8 != 0) | 1;
    entry = arg0->unkC + arg0->unk14;
    temp = entry->unk0;
    arg0->unk1C = arg2 + ((temp * sign) << 16);
    arg0->unk20 = arg3 + (entry->unk1 << 16);
    arg0->unk24 = arg4;
    arg0->unk2C = entry->unk4;
    arg0->unk2F = arg8;
    arg0->unk48 = arg7;
    arg0->unk40 = arg5 >> 4;
    arg0->unk44 = arg6 >> 4;
    arg0->unk2E = arg9;
    arg0->unk4A = arg10;

    getTableEntryByU16Index(arg0->unk28, arg0->unk2C, &sp10);

    temp_v1 = (sp10.field1 << 16) | sp10.field2;

    switch (temp_v1) {
        case 0x80008:
            arg0->unk18 = &D_8008C9E8_8D5E8;
            break;
        case 0x100010:
            arg0->unk18 = &D_8008CA28_8D628;
            break;
        case 0x100020:
            arg0->unk18 = &D_8008CA68_8D668;
            break;
        case 0x200010:
            arg0->unk18 = &D_8008CAA8_8D6A8;
            break;
        case 0x200020:
            arg0->unk18 = &D_8008CAE8_8D6E8;
            break;
        case 0x400020:
            arg0->unk18 = &D_8008CB28_8D728;
            break;
        case 0x200040:
            arg0->unk18 = &D_8008CB68_8D768;
            break;
        case 0x400040:
            arg0->unk18 = &D_8008CBA8_8D7A8;
            break;
    }

    if ((arg9 & 0xFF) == 0xFF) {
        ((void (*)(s32, Node *))func_80009D58_A958)(arg1, (Node *)&arg0->unk18);
    } else {
        ((void (*)(s32, Node *))func_80009D94_A994)(arg1, (Node *)&arg0->unk18);
    }
}

u16 func_8000A410_B010(func_8000A410_B010_arg *arg0) {
    OutputStruct_19E80 output;
    getTableEntryByU16Index(arg0->table, arg0->index, &output);
    return output.field1;
}
