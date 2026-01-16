#include "cutscene/cutscene_bgm.h"
#include "graphics.h"

void cutsceneBgmPlay_init(void) {
}

s32 cutsceneBgmPlay_validate(void) {
    return 0;
}

void cutsceneBgmPlay_exec(CutsceneBgmPlayArgs *arg0) {
    playMusicTrackWithFadeIn(arg0->musicTrackId, arg0->fadeDuration, arg0->targetVolume);
}

void cutsceneBgmFadeOut_init(void) {
}

s32 cutsceneBgmFadeOut_validate(void) {
    return 0;
}

void cutsceneBgmFadeOut_exec(s16 *arg0) {
    if (*arg0 < 4) {
        setMusicFadeOut(4);
    } else {
        setMusicFadeOut(*arg0);
    }
}

void cutsceneBgmVolume_init(void) {
}

s32 cutsceneBgmVolume_validate(void) {
    return 0;
}

void cutsceneBgmVolume_exec(CutsceneBgmVolumeArgs *arg0) {
    setMusicVolumeFade(arg0->targetVolume, arg0->musicTrackId); // musicTrackId reused as fadeDuration
}
