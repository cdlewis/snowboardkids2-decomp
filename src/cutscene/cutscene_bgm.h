#include "common.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
} cutsceneBgmPlay_exec_arg0;

void cutsceneBgmPlay_init(void);
s32 cutsceneBgmPlay_validate(void);
void cutsceneBgmPlay_exec(cutsceneBgmPlay_exec_arg0 *arg0);
void cutsceneBgmFadeOut_init(void);
s32 cutsceneBgmFadeOut_validate(void);
void cutsceneBgmFadeOut_exec(s16 *arg0);
void cutsceneBgmVolume_init(void);
s32 cutsceneBgmVolume_validate(void);
void cutsceneBgmVolume_exec(cutsceneBgmPlay_exec_arg0 *arg0);