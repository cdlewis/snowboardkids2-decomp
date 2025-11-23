#include "common.h"
#include "1DFAA0.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
} cutsceneSePlay_exec_arg;

typedef struct {
    u8 padding[0xF0];
    CutsceneSlotData *unkF0;
} cutsceneSe3dPlay_exec_arg1_item;

void cutsceneSePlay_init(void);
s32 cutsceneSePlay_validate(void);
void cutsceneSe3dPlay_init(void);
s32 cutsceneSe3dPlay_validate(void);
void cutsceneSeStop_init(void);
s32 cutsceneSeStop_validate(void);

void cutsceneSePlay_exec(cutsceneSePlay_exec_arg *arg0);

void cutsceneSe3dPlay_exec(cutsceneSePlay_exec_arg *arg0, cutsceneSe3dPlay_exec_arg1_item *arg1, s8 arg2);

void cutsceneSeStop_exec(cutsceneSePlay_exec_arg *arg0);
