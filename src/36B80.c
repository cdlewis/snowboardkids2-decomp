#include "assets.h"
#include "common.h"
#include "rom_loader.h"
#include "task_scheduler.h"

void *loadTextRenderAsset(s32 variant) {
    if ((u8)variant != 1) {
        return loadCompressedData(&_3F6950_ROM_START, &_3F6950_ROM_END, 0x508);
    } else {
        return loadCompressedData(&_459E00_ROM_START, &_459E00_ROM_END, 0x22E8);
    }
}
