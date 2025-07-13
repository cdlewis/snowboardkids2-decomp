#include "common.h"
#include "memory_allocator.h"

void dmaLoadAndInvalidate(
    void* romStart,
    void* romEnd,
    void* ramStart,
    void* icacheStart,
    void* icacheEnd,
    void* dcacheStart,
    void* dcacheEnd,
    void* bssStart,
    void* bssEnd);

void *dmaQueueRequest(void *romStart, void *romEnd, s32 size);
void* queueDmaTransfer(void* start, void* end);
void initPiManager(void);