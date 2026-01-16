#include "common.h"

typedef struct {
    s16 musicTrackId;
    s16 targetVolume;
    s16 fadeDuration;
} CutsceneBgmPlayArgs;

typedef CutsceneBgmPlayArgs CutsceneBgmVolumeArgs; // Reused with different field meanings

void cutsceneBgmPlay_init(void);
s32 cutsceneBgmPlay_validate(void);
void cutsceneBgmPlay_exec(CutsceneBgmPlayArgs *arg0);
void cutsceneBgmFadeOut_init(void);
s32 cutsceneBgmFadeOut_validate(void);
void cutsceneBgmFadeOut_exec(s16 *arg0);
void cutsceneBgmVolume_init(void);
s32 cutsceneBgmVolume_validate(void);
void cutsceneBgmVolume_exec(CutsceneBgmVolumeArgs *arg0);