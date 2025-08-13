#include "common.h"

typedef struct viConfig_s ViConfig;
struct viConfig_s {
    ViConfig **prevConfig;
    ViConfig **nextConfig;
    OSMesgQueue *messageQueue;
    u16 frameCounter;
    u16 maxFrames;
};

void initialize_video_and_threads(s32 viMode);

void addViConfig(ViConfig *config, OSMesgQueue *messageQueue, s32 frameCount);

void sendMessageToEventQueue2(OSMesg message);