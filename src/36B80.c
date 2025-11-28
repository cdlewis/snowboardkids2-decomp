#include "common.h"
#include "task_scheduler.h"

extern u8 D_459E00[];
extern u8 D_45A890[];
extern u8 D_3F6950[];
extern u8 D_3F6BB0[];

void *func_80035F80_36B80(s32 arg0) {
    if ((u8)arg0 != 1) {
        return dmaRequestAndUpdateStateWithSize(D_3F6950, D_3F6BB0, 0x508);
    } else {
        return dmaRequestAndUpdateStateWithSize(D_459E00, D_45A890, 0x22E8);
    }
}
