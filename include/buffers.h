#include "common.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

typedef union {
    u16 data[SCREEN_HEIGHT * SCREEN_WIDTH];
    u16 array[SCREEN_HEIGHT][SCREEN_WIDTH];
} FrameBuffer; // size = 0x25800

extern FrameBuffer gFrameBuffer;
extern u8 gMemoryHeapBase[];
#define gMemoryHeapEnd (gMemoryHeapBase + 0x200000)