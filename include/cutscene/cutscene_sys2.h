#ifndef _CUTSCENE_SYS2_H_
#define _CUTSCENE_SYS2_H_

#include "1DFAA0.h"
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
    /* 0xFF8 */ Func8000C268Arg matrix;
} cutsceneSys2Wipe_exec_arg1;

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
} cutsceneSys2Wait_exec_arg0;

typedef struct {
    s16 posX;
    s16 posY;
    void *model;
    s16 unk8;
    u16 zoomScaleX;
    u16 zoomScaleY;
    s16 unkE;
    s16 unk10;
    s8 unk12;
    s8 unk13;
    s8 unk14;
    u8 padding[3];
} cutsceneSys2WaitSlot;

typedef struct {
    void *cutsceneManager;
    s32 model;
    void *sprites;
    void *commandTable;
    u16 basePosX;
    u16 basePosY;
    cutsceneSys2WaitSlot slots[4];
    s32 zoomLevelX;
    s32 zoomLevelY;
    s32 zoomDecelRate;
    s32 zoomSpeed;
    s16 state;
    s16 tableRowIndex;
    s16 tableColumnIndex;
    s16 unk8A;
    s16 unk8C;
    s16 unk8E;
    void *currentCommandEntry;
    void *unk94;
    s16 unk98;
    s16 unk9A;
    u8 unk9C;
    u8 commandOffset;
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
void cutsceneSys2Wait_exec(cutsceneSys2Wait_exec_arg0 *arg0, CutsceneManager *arg1, s8 arg2);

#endif
