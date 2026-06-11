#include "system/rom_loader.h"

#include "os_cache.h"
#include "os_libc.h"
#include "os_message.h"
#include "os_pi.h"
#include "os_thread.h"
#include "system/memory_allocator.h"

typedef enum {
    DMA_COMPRESSION_NONE = 0,
    DMA_COMPRESSION_RLE = 1,
    DMA_COMPRESSION_DIRECT_BUFFER = 2
} DmaCompressionType;

typedef struct {
    void *start;
    void *end;
    s32 compressionType;
    s32 size;
    void *dramAddr;
} DmaTransferEntry;

// Data
s32 gPendingDmaCount = 0;

// Bss
OSThread gPiDmaThread BSS = { 0 };
static u8 gPiDmaThreadPadding[0x140] BSS = { 0 };
char piManagerThreadStack[0x8] BSS = { 0 };
static u8 piManagerThreadStackPadding[0x38] BSS = { 0 };
OSMesgQueue gDmaMsgQueue BSS = { 0 };
OSMesg *gDmaMsgBuf BSS = NULL;
static u8 gDmaMsgBufPadding[0x4] BSS = { 0 };
OSMesgQueue gDmaIoMsgQueue BSS = { 0 };
OSMesg gDmaIoMsgBuf[1] BSS = { 0 };
static u8 gDmaIoMsgBufPadding[0x4] BSS = { 0 };
OSMesgQueue gPiDmaMsgQueue BSS = { 0 };
OSMesg gPiDmaMsgBuf[1] BSS = { 0 };
static u8 gPiDmaMsgBufPadding[0x4] BSS = { 0 };
OSMesgQueue gPiManagerMsgQueue BSS = { 0 };
OSMesg gPiManagerMsgBuf[200] BSS = { 0 };
s32 gDmaQueueIndex BSS = 0;
DmaTransferEntry *gDmaQueue BSS = NULL;
s32 gDmaRequestCount BSS = 0;
u8 *gDmaCompressionBuffer BSS = NULL;
static u8 gDmaCompressionBufferPadding[0x8] BSS = { 0 };

void piDmaHandlerThread(void *);

void initPiManager(void) {
    u8 flag;

    gDmaQueueIndex = 0;
    gDmaRequestCount = 0;
    gDmaMsgBuf = allocateMemoryNode(0, 0x168, &flag);
    gDmaQueue = allocateMemoryNode(0, 0x730, &flag);
    gDmaCompressionBuffer = allocateMemoryNode(0, 0x1000, &flag);

    osCreatePiManager(150, &gPiManagerMsgQueue, gPiManagerMsgBuf, 200);
    osCreateMesgQueue(&gDmaIoMsgQueue, gDmaIoMsgBuf, 1);
    osCreateMesgQueue(&gPiDmaMsgQueue, gPiDmaMsgBuf, 1);
    osCreateMesgQueue(&gDmaMsgQueue, gDmaMsgBuf, 90);

    osCreateThread(&gPiDmaThread, 7, piDmaHandlerThread, 0, &piManagerThreadStack + sizeof(piManagerThreadStack), 1);

    osStartThread(&gPiDmaThread);
}

void piDmaHandlerThread(void *arg __attribute__((unused))) {
    OSIoMesg dmaMsg;
    DmaTransferEntry *entry;
    OSMesg dummyMesg;
    s32 dstOffset;
    s32 copySize;
    s32 size;
    s32 srcOffset;
    s32 copyCount;
    u32 devAddr;
    u8 *dramAddr;
    u8 *dst;
    u8 *src;
    u8 *ptr;
    u8 byte1;
    s32 count;
    s16 new_var;

    while (TRUE) {
        osRecvMesg(&gDmaMsgQueue, (OSMesg)&entry, OS_MESG_BLOCK);

        dstOffset = 0;

        if (entry->compressionType == DMA_COMPRESSION_NONE) {
            size = entry->size;
            osInvalDCache(entry->dramAddr, size);
            devAddr = (u32)entry->start;
            dramAddr = entry->dramAddr;

            while (size != 0) {
                copySize = size;
                if ((u32)size >= 0x1001U) {
                    copySize = 0x1000;
                }

                osPiStartDma(&dmaMsg, OS_MESG_PRI_NORMAL, OS_READ, devAddr, dramAddr, copySize, &gDmaIoMsgQueue);
                osRecvMesg(&gDmaIoMsgQueue, &dummyMesg, OS_MESG_BLOCK);

                devAddr += copySize;
                size -= copySize;
                dramAddr += copySize;
            }
        } else {
            dramAddr = entry->dramAddr;
            size = entry->end - entry->start;
            devAddr = (u32)entry->start;

            while (size != 0) {
                volatile u8 padding[0x10];
                osInvalDCache(gDmaCompressionBuffer, 0x1000);
                new_var = 0;
                copySize = size;
                if ((u32)size >= 0x1001U) {
                    copySize = 0x1000;
                }

                osPiStartDma(
                    &dmaMsg,
                    OS_MESG_PRI_NORMAL,
                    OS_READ,
                    devAddr,
                    gDmaCompressionBuffer,
                    copySize,
                    &gDmaIoMsgQueue
                );
                osRecvMesg(&gDmaIoMsgQueue, &dummyMesg, 1);

                srcOffset = 0;
                if (copySize > 0) {
                    dst = (u8 *)((u32)dstOffset + (u32)dramAddr);

                    do {
                        s32 temp_count;
                        s32 temp_offset;

                        src = gDmaCompressionBuffer + srcOffset;
                        byte1 = src[0];
                        count = byte1 & 0xFF;

                        if (count == 0) {
                            *dst = src[1];
                            dstOffset++;
                            dst++;
                        } else {
                            temp_offset = byte1 & 0xF;
                            temp_offset = temp_offset << 8;
                            temp_offset |= src[1];

                            copyCount = 0;
                            temp_count = (u32)count >> 4;

                            if (temp_count != 0) {
                                do {
                                    ptr = (u8 *)(((u32)dstOffset) + ((u32)dramAddr));
                                    do {
                                        dst++;
                                        *ptr = dramAddr[dstOffset - temp_offset];
                                        dstOffset++;
                                        copyCount++;
                                        ptr++;
                                    } while (copyCount < temp_count);
                                } while (0);
                            }
                        }

                        srcOffset += 2;

                        if ((u32)dstOffset >= entry->size) {
                            goto END;
                        }

                    } while (srcOffset < copySize);
                }

                size -= copySize;
                devAddr += copySize;
            }
        }

    END:
        unlockNodeWithInterruptDisable((s32 *)entry->dramAddr);

        if ((u32)entry->compressionType < DMA_COMPRESSION_DIRECT_BUFFER) {
            gPendingDmaCount--;
        }
    }
}

void *queueUncompressedDmaTransfer(void *start, void *end) {
    void *dramAddr;
    u8 nodeAlreadyExists;
    s32 size;
    size = (s32)end - (s32)start;

    gDmaQueue[gDmaQueueIndex].size = size;

    dramAddr = allocateMemoryNode((s32)start, size, &nodeAlreadyExists);

    gDmaQueue[gDmaQueueIndex].dramAddr = dramAddr;

    if (nodeAlreadyExists) {
        return dramAddr;
    }

    markNodeAsLocked(dramAddr);

    gDmaQueue[gDmaQueueIndex].start = start;
    gDmaQueue[gDmaQueueIndex].end = end;
    gDmaQueue[gDmaQueueIndex].compressionType = DMA_COMPRESSION_NONE;

    gPendingDmaCount++;
    osSendMesg(&gDmaMsgQueue, (OSMesg)&gDmaQueue[gDmaQueueIndex], OS_MESG_BLOCK);
    if ((++gDmaQueueIndex) >= 0x5C) {
        gDmaQueueIndex = 0;
    }

    gDmaRequestCount++;

    setNodeSequenceNumber(dramAddr, (void *)gDmaRequestCount);

    return dramAddr;
}

void *queueCompressedDmaTransfer(void *romStart, void *romEnd, s32 size) {
    u8 flag;
    DmaTransferEntry *entry;
    MemoryAllocatorNode *allocatedSpaceStart;
    s32 a1Val;
    s32 a1;

    gDmaQueue[gDmaQueueIndex].size = size;
    allocatedSpaceStart = allocateMemoryNode((s32)romStart, size, &flag);
    gDmaQueue[gDmaQueueIndex].dramAddr = allocatedSpaceStart;

    if (flag == 0) {
        markNodeAsLocked(allocatedSpaceStart);
        gDmaQueue[gDmaQueueIndex].start = romStart;
        entry = &gDmaQueue[gDmaQueueIndex];
        entry->end = romEnd;
        entry->compressionType = DMA_COMPRESSION_RLE;
        gPendingDmaCount++;
        osSendMesg(&gDmaMsgQueue, entry, OS_MESG_BLOCK);
        gDmaQueueIndex++;
        if (gDmaQueueIndex >= 0x5C) {
            gDmaQueueIndex = 0;
        }
        {
            a1 = gDmaRequestCount;
            a1++;
            gDmaRequestCount = a1;
        }
        setNodeSequenceNumber(allocatedSpaceStart, (void *)a1);
    }

    return allocatedSpaceStart;
}

s32 getPendingDmaCount(void) {
    return gPendingDmaCount;
}

s32 *queueDirectDmaTransfer(void *romStart, void *romEnd, s32 size, s32 *dramAddr) {
    (&gDmaQueue[gDmaQueueIndex])->size = size;
    (&gDmaQueue[gDmaQueueIndex])->dramAddr = dramAddr;

    markNodeAsLocked(dramAddr);

    (&gDmaQueue[gDmaQueueIndex])->start = romStart;
    (&gDmaQueue[gDmaQueueIndex])->end = romEnd;
    (&gDmaQueue[gDmaQueueIndex])->compressionType = DMA_COMPRESSION_DIRECT_BUFFER;

    osSendMesg(&gDmaMsgQueue, &gDmaQueue[gDmaQueueIndex], 1);

    if (++gDmaQueueIndex >= 0x5C) {
        gDmaQueueIndex = 0;
    }
    gDmaRequestCount++;

    setNodeSequenceNumber(dramAddr, (void *)gDmaRequestCount);

    return dramAddr;
}

void dmaStub(void) {
}

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
) {
    OSIoMesg dmaMessage;
    void *dummyMessage;
    u32 remainingBytes;
    void *currentRomOffset;
    u32 currentChunkSize;
    void *currentRamDest;

    // Zero out BSS or other region if requested
    if (bssEnd != bssStart) {
        bzero(bssStart, bssEnd - bssStart);
    }

    // Invalidate instruction and data caches for specified ranges
    osInvalICache(icacheStart, icacheEnd - icacheStart);
    osInvalDCache(dcacheStart, dcacheEnd - dcacheStart);

    remainingBytes = romEnd - romStart;
    currentRomOffset = romStart;
    currentRamDest = ramStart;

    while (remainingBytes > 0) {
        currentChunkSize = remainingBytes;

        // Cap the transfer size to 0x1000 bytes (4KB)
        if (remainingBytes >= 0x1001) {
            currentChunkSize = 0x1000;
        }

        osPiStartDma(
            &dmaMessage,
            OS_MESG_PRI_NORMAL,
            OS_READ,
            (u32)currentRomOffset,
            currentRamDest,
            currentChunkSize,
            &gPiDmaMsgQueue
        );
        osRecvMesg(&gPiDmaMsgQueue, &dummyMessage, OS_MESG_BLOCK);

        currentRomOffset += currentChunkSize;
        currentRamDest += currentChunkSize;
        remainingBytes -= currentChunkSize;
    }
}
