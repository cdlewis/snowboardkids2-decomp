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

void* dmaQueueRequest(void* arg0, void* arg1, s32 arg2);
void* queueDmaTransfer(void* start, void* end);
void initPiManager(void);