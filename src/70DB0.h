#include "common.h"

typedef struct viConfig_s ViConfig;
struct viConfig_s {
    ViConfig **prevConfig;
    ViConfig **nextConfig;
    OSMesgQueue *messageQueue;
    u16 frameCounter;
    u16 maxFrames;
};