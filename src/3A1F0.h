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

void* dmaQueueRequest(void* arg0, void* arg1, s32 arg2);
