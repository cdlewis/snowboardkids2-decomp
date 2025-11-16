#include "cutscene/cutscene_bgm.h"

void func_80057514_58114(s16, u16, u16);
void func_80057564_58164(s16);
void func_80057550_58150(u16, u16);

void cutsceneBgmPlay_init(void) {
}

s32 cutsceneBgmPlay_validate(void) {
    return 0;
}

void cutsceneBgmPlay_exec(cutsceneBgmPlay_exec_arg0 *arg0) {
    func_80057514_58114(arg0->unk0, arg0->unk4, arg0->unk2);
}

void cutsceneBgmFadeOut_init(void) {
}

s32 cutsceneBgmFadeOut_validate(void) {
    return 0;
}

void cutsceneBgmFadeOut_exec(s16 *arg0) {
    if (*arg0 < 4) {
        func_80057564_58164(4);
    } else {
        func_80057564_58164(*arg0);
    }
}

void cutsceneBgmVolume_init(void) {
}

s32 cutsceneBgmVolume_validate(void) {
    return 0;
}

void cutsceneBgmVolume_exec(cutsceneBgmPlay_exec_arg0 *arg0) {
    func_80057550_58150(arg0->unk2, arg0->unk0);
}
