#include "common.h"

void dmaLoadAndInvalidate(
    u32* romStart,
    s32* romEnd,
    void* ramStart,
    void* icacheStart,
    void* icacheEnd,
    void* dcacheStart,
    void* dcacheEnd,
    void* bssStart,
    void* bssEnd);