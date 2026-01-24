#include "rom_loader.h"

#include "memory_allocator.h"

// Data
s32 gPendingDmaCount = 0;

// Bss
extern OSMesgQueue *D_800A2150_A2D50[];
extern OSThread D_800A1DC0_A29C0;
extern s32 gDmaQueueIndex;
extern s32 gDmaRequestCount;
extern u8 D_800A2168_A2D68[];
extern u8 *gDmaCompressionBuffer;
extern OSMesgQueue D_800A2110_A2D10;
extern OSMesgQueue gDmaMsgQueue;
extern OSMesgQueue gPiDmaMsgQueue;
extern char piManagerThreadStack[0x8]; // this size seems wrong
extern DmaTransferEntry *D_800A2108_A2D08;
extern DmaTransferEntry *gDmaQueue;
extern OSMesg D_800A2128_A2D28[];
extern OSMesg gPiDmaMsgBuf[];

void piDmaHandlerThread(void *);

void initPiManager(void) {
    u8 flag;

    gDmaQueueIndex = 0;
    gDmaRequestCount = 0;
    D_800A2108_A2D08 = allocateMemoryNode(0, 0x168, &flag);
    gDmaQueue = allocateMemoryNode(0, 0x730, &flag);
    gDmaCompressionBuffer = allocateMemoryNode(0, 0x1000, &flag);

    osCreatePiManager(150, (OSMesgQueue *)D_800A2150_A2D50, (OSMesg *)D_800A2168_A2D68, 200);
    osCreateMesgQueue(&D_800A2110_A2D10, D_800A2128_A2D28, 1);
    osCreateMesgQueue(&gPiDmaMsgQueue, gPiDmaMsgBuf, 1);
    osCreateMesgQueue(&gDmaMsgQueue, (OSMesg *)D_800A2108_A2D08, 90);

    osCreateThread(
        &D_800A1DC0_A29C0,
        7,
        piDmaHandlerThread,
        0,
        &piManagerThreadStack + sizeof(piManagerThreadStack),
        1
    );

    osStartThread(&D_800A1DC0_A29C0);
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

                osPiStartDma(&dmaMsg, OS_MESG_PRI_NORMAL, OS_READ, devAddr, dramAddr, copySize, &D_800A2110_A2D10);
                osRecvMesg(&D_800A2110_A2D10, &dummyMesg, OS_MESG_BLOCK);

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
                    &D_800A2110_A2D10
                );
                osRecvMesg(&D_800A2110_A2D10, &dummyMesg, 1);

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
