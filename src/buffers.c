#include "buffers.h"

FrameBuffer gFrameBuffer __attribute__((section(".bss")));
u8 gMemoryHeapBase[0x200000] __attribute__((section(".bss")));
