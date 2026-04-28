#include "text/font_render.h"
#include "assets.h"
#include "common.h"
#include "system/task_scheduler.h"

void *loadTextRenderAsset(s32 variant) {
    if ((u8)variant != 1) {
        return loadCompressedData(&digit_sprite_ROM_START, &digit_sprite_ROM_END, 0x508);
    } else {
        return loadCompressedData(&font_main_ROM_START, &font_main_ROM_END, 0x22E8);
    }
}
