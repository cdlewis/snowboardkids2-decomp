#include "common.h"
#include "1DFAA0.h"

void debugHandleFanSoundInput(void);

typedef struct {
    s16 soundEffectId; // Used as stoppingSpeed in cutsceneSeStop_exec
    s16 volume;        // Used as channelIndex in cutsceneSeStop_exec
    s16 repeatCount;
    s16 minInterval;
    s16 channelOrInterval; // channel when repeatCount==1, interval when repeatCount>1
    s16 pan;
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
