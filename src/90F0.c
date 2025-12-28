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

s32 getSpriteAssetCount(void) {
    return 10;
}

s16 getSpriteAssetId(s32 index) {
    return D_8008C930_8D530[index][0];
}

INCLUDE_ASM("asm/nonmatchings/90F0", func_80008514_9114);

INCLUDE_ASM("asm/nonmatchings/90F0", func_80008D18_9918);

INCLUDE_ASM("asm/nonmatchings/90F0", func_80009548_A148);

extern void func_80008514_9114(void *);
extern void func_80008D18_9918(void);
extern void func_80009548_A148(void);

void enqueueOpaqueSprite(u16 slot, Node *node) {
    node->unk18 = NULL;
    node->unk1C = NULL;
    node->callback = NULL;
    node->cleanupCallback = NULL;
    debugEnqueueCallback(slot, 4, &func_80008514_9114, node);
}

void enqueueTranslucentSprite(u16 slot, Node *node) {
    node->unk18 = NULL;
    node->unk1C = NULL;
    node->callback = NULL;
    node->cleanupCallback = NULL;
    debugEnqueueCallback(slot, 4, &func_80008D18_9918, node);
}

void enqueueTransformedSprite(u16 slot, Node *node) {
    node->unk18 = NULL;
    node->unk1C = NULL;
    node->callback = NULL;
    node->cleanupCallback = NULL;
    debugEnqueueCallback(slot, 4, &func_80009548_A148, node);
}

void setSpriteAssetEnabled(SpriteAssetState *state) {
    state->flags |= 1;
}

void clearSpriteAssetEnabled(SpriteAssetState *state) {
    state->flags &= 0xFE;
}

s32 func_80009E2C_AA2C(SpriteAssetState *state) {
    return state->flags & 1;
}

void setSpriteAssetVisible(SpriteAssetState *state) {
    state->flags |= 2;
}

void clearSpriteAssetVisible(SpriteAssetState *state) {
    state->flags &= 0xFD;
}

s32 isSpriteAssetVisible(SpriteAssetState *state) {
    u8 val;

    val = state->flags & 2;
    return val != 0;
}

s32 loadSpriteAsset(SpriteAssetState *state, s16 index) {
    s32 *entry;
    s16 savedIndex;

    savedIndex = index;
    if (savedIndex >= getSpriteAssetCount()) {
        state->spriteData = NULL;
        state->assetIndex = 0;
        return 0;
    }
    entry = &D_8008C920_8D520[savedIndex * 5];
    state->spriteData = loadCompressedData((void *)entry[0], (void *)entry[1], entry[2]);
    state->assetIndex = index;
    state->flags = 0;
    return 1;
}

void *func_80009EF8_AAF8(s16 arg0) {
    s32 *entry;

    if (arg0 >= getSpriteAssetCount()) {
        return NULL;
    }
    entry = &D_8008C920_8D520[arg0 * 5];
    return loadCompressedData((void *)entry[0], (void *)entry[1], entry[2]);
}

void func_80009F5C_AB5C(func_80009F5C_AB5C_arg **arg0) {
    if (arg0 != NULL) {
        *arg0 = (func_80009F5C_AB5C_arg *)freeNodeMemory(*arg0);
    }
}

void func_80009F90_AB90(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    SpriteAssetState *state = (SpriteAssetState *)arg0;
    s32 *entry;
    s16 saved = arg2;
    s16 index = arg2;
    u16 t16;
    void *t0;
    u16 t7;

    if (index == -1) {
        state->unk8 = NULL;
        return;
    }

    entry = &D_8008C920_8D520[state->assetIndex * 5];
    if (index < *(s16 *)(&entry[4])) {
        state->unk8 = (AnimSetEntry *)entry[3] + index;
        state->unkC = state->unk8->unk0;
        state->unk10 = saved;
        state->unk12 = state->unkC->unk4;
        state->unk14 = 0;
        t16 = state->unkC->unk6;
        state->unk16 = t16;
        t0 = state->spriteData;
        t7 = state->unk8->unk6;
        state->unk28 = t0;
        state->unk7 = t7;
    }
}

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
        if (state->unk14 >= state->unk8->unk4) {
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
        ((void (*)(s32, Node *))enqueueOpaqueSprite)(arg1, (Node *)&arg0->unk18);
    } else {
        ((void (*)(s32, Node *))enqueueTranslucentSprite)(arg1, (Node *)&arg0->unk18);
    }
}

s32 func_8000A410_B010(func_8000A410_B010_arg *arg0) {
    OutputStruct_19E80 output;
    getTableEntryByU16Index(arg0->table, arg0->index, &output);
    return output.field1;
}
