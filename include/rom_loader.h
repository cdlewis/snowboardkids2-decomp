#pragma once

#include "common.h"

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

void initPiManager(void);

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
s32 *queueDirectDmaTransfer(void *romStart, void *romEnd, s32 size, s32 *dramAddr);
s32 getPendingDmaCount(void);

#define USE_ASSET(id)           \
    extern u32 *id##_ROM_START; \
    extern u32 *id##_ROM_END;

#define USE_OVERLAY(id)           \
    extern u32 *id##_ROM_START;   \
    extern u32 *id##_ROM_END;     \
    extern void *id##_VRAM;       \
    extern void *id##_TEXT_START; \
    extern void *id##_TEXT_END;   \
    extern void *id##_DATA_START; \
    extern void *id##_RODATA_END; \
    extern void *id##_BSS_START;  \
    extern void *id##_BSS_END;

#define LOAD_OVERLAY(id)  \
    dmaLoadAndInvalidate( \
        &id##_ROM_START,  \
        &id##_ROM_END,    \
        &id##_VRAM,       \
        &id##_TEXT_START, \
        &id##_TEXT_END,   \
        &id##_DATA_START, \
        &id##_RODATA_END, \
        &id##_BSS_START,  \
        &id##_BSS_END     \
    );
