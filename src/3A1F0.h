#include "common.h"
#include "memory_allocator.h"

void dmaLoadAndInvalidate(
    void *romStart,
    void *romEnd,
    void *ramStart,
    void *icacheStart,
    void *icacheEnd,
    void *dcacheStart,
    void *dcacheEnd,
    void *bssStart,
    void *bssEnd
);

void *dmaQueueRequest(void *arg0, void *arg1, s32 arg2);
void *queueDmaTransfer(void *start, void *end);
void initPiManager(void);

s32 *queueDmaTransferToBuffer(void *romStart, void *romEnd, s32 size, s32 *dramAddr);

s32 func_8003BB5C_3C75C(void);

void func_8003AF6C_3BB6C(void);

void func_8003AC38_3B838(void);

void func_80039B88_3A788(void);

void func_800395F0_3A1F0(void);