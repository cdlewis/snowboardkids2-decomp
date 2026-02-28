#include "assets.h"
#include "common.h"
#include "rom_loader.h"
#include "task_scheduler.h"

void *loadTextRenderAsset(s32 variant) {
    if ((u8)variant != 1) {
        return loadCompressedData(&font_race_timer_ROM_START, &font_race_timer_ROM_END, 0x508);
    } else {
        return loadCompressedData(&font_main_ROM_START, &font_main_ROM_END, 0x22E8);
    }
}
