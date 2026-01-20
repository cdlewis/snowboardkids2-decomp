#include "memory.h"
#include "common.h"

u8 gMemoryHeapBase[0x200000] __attribute__((section(".bss")));
