#include "5EA60.h"
#include "common.h"
#include "geometry.h"

void *func_8005DE60_5EA60(void *arg0) {
    return (void *)((s8 *)arg0 + *(s32 *)((s8 *)arg0 + 4));
}

u16 *func_8005DE6C_5EA6C(void *base, s16 tableIndex, s16 subIndex) {
    void *temp;

    temp = base + *(s32 *)(base + tableIndex * 4 + 8);
    return temp + *(u16 *)(temp + subIndex * 2 + 4);
}

void func_8005DE98_5EA98(void *arg0, s32 arg1, s32 arg2, func_8005E800_5F400_arg *arg3) {
    void *result;
    s16 s_arg1;
    s16 s_arg2;

    result = func_8005DE60_5EA60(arg0);
    arg3->frame_data = result;

    s_arg1 = (s16)(arg1 << 16 >> 16);
    s_arg2 = (s16)(arg2 << 16 >> 16);

    result = func_8005DE6C_5EA6C(arg0, s_arg1, s_arg2);
    arg3->animation_data = result;

    arg3->flags = *(u16 *)result + 0x8000;
}

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005DF10_5EB10);

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005E22C_5EE2C);

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005E500_5F100);

extern void func_8005DF10_5EB10(s16, s16, s16, s16 *);

void func_8005E800_5F400(func_8005E800_5F400_arg *entity, u16 param_2) {
    s16 stack_data[0x10];

    if (entity->flags & 0x8000) {
        entity->flags &= 0x7FFF;
        func_8005DF10_5EB10(
            entity->animation_data[1],
            entity->animation_data[2],
            entity->animation_data[3],
            entity->values
        );
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

void func_8005EA44_5F644(func_8005E800_5F400_arg *entity, u16 param_2) {
    s16 stack_data[16];
    u16 idx;
    s16 temp_val;
    s32 source_val;
    s32 dest_val;
    s32 result;

    if (entity->flags & 0x8000) {
        entity->flags &= 0x7FFF;
        func_8005DF10_5EB10(
            entity->animation_data[1],
            -entity->animation_data[2],
            -entity->animation_data[3],
            entity->values
        );

        idx = entity->animation_data[4];
        temp_val = entity->frame_data[idx * 3];
        entity->position[0] = -temp_val << 10;

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

    result = entity->animation_data[3] * (param_2 & 0xFFFF);
    if (result < 0) {
        result += 0x1FF;
    }
    entity->counter = result >> 9;

    if (entity->counter & 0xFFFF) {
        func_8005DF10_5EB10(entity->animation_data[1], -entity->animation_data[2], -entity->counter, stack_data);
        func_8006BDBC_6C9BC(entity, stack_data, entity->prev_position);
    }

    idx = entity->animation_data[4];
    source_val = -entity->frame_data[idx * 3] << 10;
    dest_val = entity->position[0];
    result = (source_val - dest_val) * param_2;
    if (result < 0) {
        result += 0x1FF;
    }
    entity->interpolated[0] = (result >> 9) + dest_val;

    idx = entity->animation_data[4];
    source_val = entity->frame_data[(idx * 3) + 1] << 10;
    dest_val = entity->position[1];
    result = (source_val - dest_val) * param_2;
    if (result < 0) {
        result += 0x1FF;
    }
    entity->interpolated[1] = (result >> 9) + dest_val;

    idx = entity->animation_data[4];
    source_val = entity->frame_data[(idx * 3) + 2] << 10;
    result = (source_val - entity->position[2]) * param_2;
    if (result < 0) {
        result += 0x1FF;
    }
    entity->interpolated[2] = (result >> 9) + entity->position[2];
}

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005ECB8_5F8B8);

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005EFC4_5FBC4);

typedef struct {
    u8 padding[0x40];
    s16 unk40;
    s16 padding2;
    s16 unk44;
} func_8005F2FC_5FEFC_arg;
void func_8005F2FC_5FEFC(s16 arg0, s16 arg1, s16 arg2, func_8005F2FC_5FEFC_arg *arg3) {
    func_8005F2FC_5FEFC_arg *new_var2;
    u16 new_var;

    arg3->unk44 = 0;
    new_var = *func_8005DE6C_5EA6C(new_var2, arg1, arg2);
    arg3->unk40 = (s16)(new_var + 0x8000);
}

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005F344_5FF44);

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005F6DC_602DC);

typedef struct {
    s16 values[10];
    s32 position[3];
    s16 prev_position[10];
    s32 interpolated[3];
    u16 flags;
    u16 counter;
    u16 animation_index;
} func_8005FAA0_606A0_arg;

void func_8005FAA0_606A0(void *arg0, s16 arg1, s16 arg2, func_8005FAA0_606A0_arg *entity, s16 param_2) {
    s16 *frame_data;
    u16 *animation_data;
    s16 stack_data[16];
    u16 idx;
    s16 temp_val;
    s32 source_val;
    s32 dest_val;
    s32 result;

    animation_data = func_8005DE6C_5EA6C(arg0, arg1, arg2);
    frame_data = func_8005DE60_5EA60(arg0);

    if (entity->flags & 0x8000) {
        entity->flags &= 0x7FFF;

        idx = entity->animation_index;
        func_8005DF10_5EB10(
            animation_data[idx * 5 + 1],
            animation_data[idx * 5 + 2],
            animation_data[idx * 5 + 3],
            entity->values
        );

        idx = entity->animation_index;
        temp_val = frame_data[animation_data[idx * 5 + 4] * 3];
        entity->position[0] = temp_val << 10;

        idx = entity->animation_index;
        temp_val = frame_data[animation_data[idx * 5 + 4] * 3 + 1];
        entity->position[1] = temp_val << 10;

        idx = entity->animation_index;
        temp_val = frame_data[animation_data[idx * 5 + 4] * 3 + 2];
        entity->position[2] = temp_val << 10;

        memcpy(entity->prev_position, entity->values, 0x20);
        entity->animation_index++;
        entity->counter = animation_data[entity->animation_index * 5 + 3];
    }

    idx = entity->animation_index;
    result = (s16)animation_data[idx * 5 + 3] * param_2;
    if (result < 0) {
        result += 0x1FF;
    }
    entity->counter = result >> 9;

    if (entity->counter != 0) {
        idx = entity->animation_index;
        func_8005DF10_5EB10(animation_data[idx * 5 + 1], animation_data[idx * 5 + 2], entity->counter, stack_data);
        func_8006BDBC_6C9BC((void *)entity, stack_data, entity->prev_position);
    }

    idx = entity->animation_index;
    source_val = frame_data[animation_data[idx * 5 + 4] * 3] << 10;
    dest_val = entity->position[0];
    result = (source_val - dest_val) * param_2;
    if (result < 0) {
        result += 0x1FF;
    }
    entity->interpolated[0] = (result >> 9) + dest_val;

    idx = entity->animation_index;
    source_val = frame_data[animation_data[idx * 5 + 4] * 3 + 1] << 10;
    dest_val = entity->position[1];
    result = (source_val - dest_val) * param_2;
    if (result < 0) {
        result += 0x1FF;
    }
    entity->interpolated[1] = (result >> 9) + dest_val;

    idx = entity->animation_index;
    source_val = frame_data[animation_data[idx * 5 + 4] * 3 + 2] << 10;
    result = (source_val - entity->position[2]) * param_2;
    if (result < 0) {
        result += 0x1FF;
    }
    entity->interpolated[2] = (result >> 9) + entity->position[2];
}

typedef struct {
    s16 values[10];
    s32 position[3];
    s16 prev_position[10];
    s32 interpolated[3];
    u16 flags;
    u16 counter;
    u16 animation_index;
} func_8005FDAC_609AC_arg;

void func_8005FDAC_609AC(void *arg0, s16 arg1, s16 arg2, func_8005FDAC_609AC_arg *entity, u16 param_2) {
    u16 *animation_data;
    s16 *frame_data;
    s16 stack_data[16];
    u16 idx;
    s16 temp_val;
    s32 source_val;
    s32 dest_val;
    s32 result;

    animation_data = func_8005DE6C_5EA6C(arg0, arg1, arg2);
    frame_data = func_8005DE60_5EA60(arg0);

    if (entity->flags & 0x8000) {
        entity->flags &= 0x7FFF;

        idx = entity->animation_index;
        func_8005DF10_5EB10(
            animation_data[idx * 5 + 1],
            -animation_data[idx * 5 + 2],
            -animation_data[idx * 5 + 3],
            entity->values
        );

        idx = entity->animation_index;
        temp_val = frame_data[animation_data[idx * 5 + 4] * 3];
        entity->position[0] = -temp_val << 10;

        idx = entity->animation_index;
        temp_val = frame_data[animation_data[idx * 5 + 4] * 3 + 1];
        entity->position[1] = temp_val << 10;

        idx = entity->animation_index;
        temp_val = frame_data[animation_data[idx * 5 + 4] * 3 + 2];
        entity->position[2] = temp_val << 10;

        memcpy(entity->prev_position, entity->values, 0x20);
        entity->animation_index++;
        entity->counter = animation_data[entity->animation_index * 5 + 3];
    }

    idx = entity->animation_index;
    result = (s16)animation_data[idx * 5 + 3] * param_2;
    if (result < 0) {
        result += 0x1FF;
    }
    entity->counter = result >> 9;

    if (entity->counter != 0) {
        idx = entity->animation_index;
        func_8005DF10_5EB10(animation_data[idx * 5 + 1], -animation_data[idx * 5 + 2], -entity->counter, stack_data);
        func_8006BDBC_6C9BC((void *)entity, stack_data, entity->prev_position);
    }

    idx = entity->animation_index;
    source_val = -frame_data[animation_data[idx * 5 + 4] * 3] << 10;
    dest_val = entity->position[0];
    result = (source_val - dest_val) * param_2;
    if (result < 0) {
        result += 0x1FF;
    }
    entity->interpolated[0] = (result >> 9) + dest_val;

    idx = entity->animation_index;
    source_val = frame_data[animation_data[idx * 5 + 4] * 3 + 1] << 10;
    dest_val = entity->position[1];
    result = (source_val - dest_val) * param_2;
    if (result < 0) {
        result += 0x1FF;
    }
    entity->interpolated[1] = (result >> 9) + dest_val;

    idx = entity->animation_index;
    source_val = frame_data[animation_data[idx * 5 + 4] * 3 + 2] << 10;
    result = (source_val - entity->position[2]) * param_2;
    if (result < 0) {
        result += 0x1FF;
    }
    entity->interpolated[2] = (result >> 9) + entity->position[2];
}

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

s32 func_80060950_61550(func_80060950_61550_arg *data, s16 index) {
    s32 baseOffset;
    s32 targetOffset;
    func_80060950_61550_target *target;
    s32 indexValue;
    func_80060950_61550_arg *baseData;

    targetOffset = data->offsetTable[index + 1];
    target = (func_80060950_61550_target *)(((s8 *)data) + targetOffset);
    baseOffset = data->baseOffset;
    indexValue = target->index;
    baseData = (func_80060950_61550_arg *)(((s8 *)data) + baseOffset);
    return (s32)(((s8 *)baseData) + (indexValue << 1));
}

s32 func_8006097C_6157C(func_8006097C_6157C_arg *table, s32 index) {
    s32 adjusted_index = (index << 16) >> 14;
    func_8006097C_6157C_arg_item *entry = (func_8006097C_6157C_arg_item *)((s8 *)table + adjusted_index);
    u16 *data_ptr = (u16 *)((s8 *)table + entry->unk8);

    return *data_ptr;
}