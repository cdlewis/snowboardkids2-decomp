#ifndef _3A1F0_H_
#define _3A1F0_H_

#include "common.h"
#include "memory_allocator.h"

#ifndef STATE_ENTRY_DECLARED
#define STATE_ENTRY_DECLARED
typedef struct StateEntry StateEntry;
#endif

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

void *queueCompressedDmaTransfer(void *romStart, void *romEnd, s32 decompressedSize);
void *queueUncompressedDmaTransfer(void *start, void *end);
void initPiManager(void);

s32 *queueDirectDmaTransfer(void *romStart, void *romEnd, s32 size, s32 *dramAddr);

s32 func_8003BB5C_3C75C(void);

void *func_8003B510_3C110(void);

void func_8003AF6C_3BB6C(void);

void func_8003AC38_3B838(void);

void startControllerRead(void);

void initControllerSubsystem(void);

void func_80039C34_3A834(void);

void controllerPackReadAsyncStub(void);
int controllerPackReadPollStub(void);
void func_8003A51C_3B11C(void);
int func_8003A524_3B124(void);

#endif // _3A1F0_H_
