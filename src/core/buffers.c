#include "buffers.h"

FrameBuffer gFrameBuffer __attribute__((section(".bss")));
u8 gMemoryHeapBase[MEMORY_HEAP_SIZE] __attribute__((section(".bss")));
FrameBuffer gAuxFrameBuffers[1] __attribute__((section(".bss")));