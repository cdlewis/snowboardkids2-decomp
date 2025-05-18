#include "5EA60.h"

#include "common.h"

void* func_8005DE60_5EA60(void*);
u16* func_8005DE6C_5EA6C(void* base, s16 tableIndex, s16 subIndex);

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005DE60_5EA60);

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005DE6C_5EA6C);

void func_8005DE98_5EA98(void* arg0, s32 arg1, s32 arg2, func_8005DE98_5EA98_arg* arg3) {
    void* result;
    s16 s_arg1;
    s16 s_arg2;

    result = func_8005DE60_5EA60(arg0);
    arg3->unk44 = result;

    s_arg1 = (s16)(arg1 << 16 >> 16);
    s_arg2 = (s16)(arg2 << 16 >> 16);

    result = func_8005DE6C_5EA6C(arg0, s_arg1, s_arg2);
    arg3->unk40 = result;

    arg3->unk48 = *(u16*)result + 0x8000;
}

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005DF10_5EB10);

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005E22C_5EE2C);

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005E500_5F100);

typedef struct {
    /* 0x00 */ s16 values[0xA];
    /* 0x14 */ s32 position[3];
    /* 0x20 */ s32 prev_position[3];
    /* 0x2C */ s32 unknown[2];
    /* 0x34 */ s32 interpolated[3];
    /* 0x40 */ s16* animation_data;
    /* 0x44 */ s16* frame_data;
    /* 0x48 */ u16 flags;
    /* 0x4A */ u16 counter;
} func_8005E800_5F400_arg;

extern void func_8005DF10_5EB10(s16, s16, s16, s16*);
extern void func_8006BDBC_6C9BC(func_8005E800_5F400_arg*, void*, void*);
extern void memcpy(void*, void*, s32);

void func_8005E800_5F400(func_8005E800_5F400_arg* entity, u16 param_2) {
    s16 stack_data[0x10];

    if (entity->flags & 0x8000) {
        entity->flags &= 0x7FFF;
        func_8005DF10_5EB10(entity->animation_data[1], entity->animation_data[2], entity->animation_data[3], entity->values);
        {
            u16 idx;
            s16 temp_val;

            idx = entity->animation_data[4];
            temp_val = entity->frame_data[idx * 3];
            entity->position[0] = temp_val << 10;

            idx = entity->animation_data[4];
            temp_val = entity->frame_data[(idx * 3) + 1];
            entity->position[1] = temp_val << 10;

            idx = entity->animation_data[4];
            temp_val = entity->frame_data[(idx * 3) + 2];
            entity->position[2] = temp_val << 10;

            memcpy(entity->prev_position, entity->values, 0x20);
            entity->animation_data += 5;
            entity->counter = entity->animation_data[3];
        }
    }

    {
        s32 result;

        result = entity->animation_data[3] * (param_2 & 0xFFFF);
        if (result < 0) {
            result += 0x1FF;
        }
        entity->counter = result >> 9;

        if (entity->counter & 0xFFFF) {
            func_8005DF10_5EB10(entity->animation_data[1], entity->animation_data[2], entity->counter, stack_data);
            func_8006BDBC_6C9BC(entity, stack_data, entity->prev_position);
        }
    }

    {
        u16 idx;
        s32 source_val;
        s32 dest_val;
        s32 result;

        /* First interpolation */
        idx = entity->animation_data[4];
        source_val = entity->frame_data[idx * 3] << 10;
        dest_val = entity->position[0];
        result = (source_val - dest_val) * param_2;
        if (result < 0) {
            result += 0x1FF;
        }
        entity->interpolated[0] = (result >> 9) + dest_val;

        /* Second interpolation */
        idx = entity->animation_data[4];
        source_val = entity->frame_data[(idx * 3) + 1] << 10;
        dest_val = entity->position[1];
        result = (source_val - dest_val) * param_2;
        if (result < 0) {
            result += 0x1FF;
        }
        entity->interpolated[1] = (result >> 9) + dest_val;

        /* Third interpolation */
        idx = entity->animation_data[4];
        source_val = entity->frame_data[(idx * 3) + 2] << 10;
        result = (source_val - entity->position[2]) * param_2;
        if (result < 0) {
            result += 0x1FF;
        }
        entity->interpolated[2] = (result >> 9) + entity->position[2];
    }
}

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005EA44_5F644);

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005ECB8_5F8B8);

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005EFC4_5FBC4);

extern u16* func_8005DE6C_5EA6C(void* base, s16 tableIndex, s16 subIndex);
typedef struct
{
    u8 padding[0x40];
    s16 unk40;
    s16 padding2;
    s16 unk44;
} func_8005F2FC_5FEFC_arg;
void func_8005F2FC_5FEFC(s16 arg0, s16 arg1, s16 arg2, func_8005F2FC_5FEFC_arg* arg3) {
    func_8005F2FC_5FEFC_arg* new_var2;
    u16 new_var;

    arg3->unk44 = 0;
    new_var = *func_8005DE6C_5EA6C(new_var2, arg1, arg2);
    arg3->unk40 = (s16)(new_var + 0x8000);
}

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005F344_5FF44);

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005F6DC_602DC);

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005FAA0_606A0);

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005FDAC_609AC);

INCLUDE_ASM("asm/nonmatchings/5EA60", func_800600E4_60CE4);

INCLUDE_ASM("asm/nonmatchings/5EA60", func_80060504_61104);

typedef struct {
    s32 baseOffset;
    s32 offsetTable[0];
} func_80060950_61550_arg;

typedef struct {
    u16 padding;
    u16 index;
} func_80060950_61550_target;

s32 func_80060950_61550(func_80060950_61550_arg* data, s16 index) {
    s32 baseOffset;
    s32 targetOffset;
    func_80060950_61550_target* target;
    s32 indexValue;
    func_80060950_61550_arg* baseData;

    targetOffset = data->offsetTable[index + 1];
    target = (func_80060950_61550_target*)(((s8*)data) + targetOffset);
    baseOffset = data->baseOffset;
    indexValue = target->index;
    baseData = (func_80060950_61550_arg*)(((s8*)data) + baseOffset);
    return (s32)(((s8*)baseData) + (indexValue << 1));
}

s32 func_8006097C_6157C(func_8006097C_6157C_arg* table, s32 index) {
    s32 adjusted_index = (index << 16) >> 14;
    func_8006097C_6157C_arg_item* entry = (func_8006097C_6157C_arg_item*)((s8*)table + adjusted_index);
    u16* data_ptr = (u16*)((s8*)table + entry->unk8);

    return *data_ptr;
}