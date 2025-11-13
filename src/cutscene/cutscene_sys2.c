#include "cutscene/cutscene_sys2.h"
#include "task_scheduler.h"

extern void func_8000C268_CE68(void *);
extern void func_8000C2CC_CECC(void *, s32, s16);
extern void func_8000C334_CF34(void *, Vec3 *, s16);
extern void func_8000C2B4_CEB4(void *, s32);

void func_800B29F0_1DFAA0(void *);

extern void func_800B993C_1E69EC(void);

void cutsceneSys2Wipe_init(void) {
}

s32 cutsceneSys2Wipe_validate(void) {
    return 0;
}

void cutsceneSys2Wipe_exec(cutsceneSys2Wipe_exec_arg0 *params, cutsceneSys2Wipe_exec_arg1 *state, s8 slot_index) {
    cutsceneSys2Wipe_exec_arg1_slot *slot;
    Vec3 world_pos;
    s32 grid_offset;
    s16 rotation;
    s32 temp_rotation;

    slot = &state->slots[slot_index];

    world_pos.x = slot->position.x;
    world_pos.y = slot->position.y;
    world_pos.z = slot->position.z;

    world_pos.x += params->grid_x * 209715;
    world_pos.y += params->grid_y * 209715;

    func_8000C268_CE68(&state->matrix);

    temp_rotation = (s32)(params->rotation_y) << 0x10;

    func_8000C2CC_CECC(&state->matrix, temp_rotation / 100, params->unk2);
    func_8000C334_CF34(&state->matrix, &world_pos, params->unk2);
    func_8000C2B4_CEB4(&state->matrix, params->render_flags);
}

s32 cutsceneSys2Wipe_isDone(void) {
    return 0;
}

void cutsceneSys2Wait_init(void) {
}

s32 cutsceneSys2Wait_validate(void) {
    return 0;
}

void cutsceneSys2Wait_exec(cutsceneSys2Wait_exec_arg0 *arg0, u8 *arg1, s8 arg2) {
    cutsceneSys2Wait_exec_asset *temp_v0;
    s32 *ptr;

    ptr = (s32 *)(arg1 + (arg2 * 244 + 0xA8));
    func_800B29F0_1DFAA0(arg1);

    temp_v0 = (cutsceneSys2Wait_exec_asset *)scheduleTask(&func_800B993C_1E69EC, 1, 0, 0x64);
    if (temp_v0) {
        temp_v0->unk0 = arg1;
        temp_v0->unk4 = ptr[0x48 / 4];
        temp_v0->unk10 = arg0->unk2;
        temp_v0->unk12 = arg0->unk4;
        temp_v0->unk86 = arg0->unk0;
    } else {
        arg1[0xFF7] = 0;
    }
}
