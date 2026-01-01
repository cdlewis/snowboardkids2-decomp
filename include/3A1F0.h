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

void *pollEepromWriteAllAsync(void);

void resumeMotorStates(void);

void motorUpdate(void);

void startControllerRead(void);

void initControllerSubsystem(void);

void func_80039C34_3A834(void);

void controllerPackReadAsyncStub(void);
int controllerPackReadPollStub(void);
void controllerPackWriteAsyncStub(void);
int controllerPackWritePollStub(void);

#endif // _3A1F0_H_
