#include "common.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_3F6950);
USE_ASSET(_459E00);

void *func_80035F80_36B80(s32 arg0) {
    if ((u8)arg0 != 1) {
        return loadCompressedData(&_3F6950_ROM_START, &_3F6950_ROM_END, 0x508);
    } else {
        return loadCompressedData(&_459E00_ROM_START, &_459E00_ROM_END, 0x22E8);
    }
}
