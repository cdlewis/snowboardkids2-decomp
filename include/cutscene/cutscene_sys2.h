#include "common.h"

typedef struct {
    s16 rotation_y;
    s16 unk2;
    s16 grid_x;
    s16 grid_y;
    s8 unk8;
    s8 render_flags;
} cutsceneSys2Wipe_exec_arg0;

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} Vec3;

typedef struct {
    s8 pad[0x70];
    Vec3 position;
    s8 pad2[0xF4 - 0x7C];
} cutsceneSys2Wipe_exec_arg1_slot;

typedef struct {
    s8 padding[0xA8];
    cutsceneSys2Wipe_exec_arg1_slot slots[5];
    u8 padding2[0xA8C];
    /* 0xFF8 */ void *matrix;
} cutsceneSys2Wipe_exec_arg1;

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
} cutsceneSys2Wait_exec_arg0;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s8 unk12;
    s8 unk13;
    s8 unk14;
    u8 padding[3];
} cutsceneSys2WaitSlot;

typedef struct {
    void *unk0;
    s32 unk4;
    void *unk8;
    void *unkC;
    s16 unk10;
    s16 unk12;
    cutsceneSys2WaitSlot slots[4];
    s32 unk74;
    s32 unk78;
    u8 padding2[0x8];
    s16 unk84;
    s16 unk86;
    s16 unk88;
    s16 unk8A;
    s16 unk8C;
    s16 unk8E;
    void *unk90;
    void *unk94;
    s16 unk98;
    s16 unk9A;
    u8 unk9C;
    u8 unk9D;
    u8 padding3[0x2];
    void *unkA0;
    s16 unkA4;
    s16 unkA6;
    void *unkA8;
    s16 unkAC;
    u8 padding4[0x2];
} cutsceneSys2Wait_exec_asset;

void cutsceneSys2Wipe_init(void);
s32 cutsceneSys2Wipe_validate(void);
void cutsceneSys2Wipe_exec(cutsceneSys2Wipe_exec_arg0 *params, cutsceneSys2Wipe_exec_arg1 *state, s8 slot_index);
s32 cutsceneSys2Wipe_isDone(void);
void cutsceneSys2Wait_init(void);
s32 cutsceneSys2Wait_validate(void);
void cutsceneSys2Wait_exec(cutsceneSys2Wait_exec_arg0 *arg0, u8 *arg1, s8 arg2);
