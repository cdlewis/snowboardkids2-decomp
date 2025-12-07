#include "3A1F0.h"
#include "common.h"

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
