#pragma once

#include "common.h"
#include "os_message.h"

typedef struct viConfig_s ViConfig;
struct viConfig_s {
    ViConfig *prevConfig;
    ViConfig *nextConfig;
    OSMesgQueue *messageQueue;
    u16 frameCounter;
    u16 maxFrames;
};

typedef union {
    OSMesg raw;
    struct {
        u16 type;
        u16 frameIndex;
    } retrace;
} ViMessage;

void initThreadManager(s32 viMode);

void addViConfig(ViConfig *config, OSMesgQueue *messageQueue, s32 frameCount);

void submitAudioTask(OSMesg message);

void submitDisplayTask(OSMesg message);
