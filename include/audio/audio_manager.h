#pragma once

#include "abi.h"
#include "common.h"
#include "libaudio.h"
#include "os_message.h"
#include "os_thread.h"
#include "sptask.h"

typedef struct AudioParams_s {
    u32 outputRate;
    s32 frameRateScale;
    s32 commandListCapacity;
} AudioParams;

typedef struct AudioInfo AudioInfo;

typedef struct {
    /* 0x00 */ s16 type;
    /* 0x02 */ s16 _pad2;
    /* 0x04 */ AudioInfo *info;
} AudioTaskDoneMessage;

struct AudioInfo {
    /* 0x00 */ s16 *outputBuffer;
    /* 0x04 */ s16 frameSamples;
    /* 0x06 */ s16 _pad6;
    /* 0x08 */ OSTask task;
    /* 0x48 */ OSMesgQueue *messageQueue;
    /* 0x4C */ OSMesg message;
    /* 0x50 */ AudioTaskDoneMessage doneMessage;
    /* 0x58 */ u8 _pad58[0x18];
};

typedef struct {
    /* 0x000 */ Acmd *commandLists[2];
    /* 0x008 */ AudioInfo *audioInfo[4];
    /* 0x018 */ OSThread thread;
    /* 0x1C8 */ OSMesgQueue retraceQueue;
    /* 0x1E0 */ OSMesg retraceMessages[8];
    /* 0x200 */ OSMesgQueue taskDoneQueue;
    /* 0x218 */ OSMesg taskDoneMessages[8];
} AudioManager;

extern AudioManager gAudioManager;

void initAudioManager(
    ALSynConfig *config,
    OSId id,
    AudioParams *audioParams,
    s32 maxChannels,
    s32 maxVoices,
    s32 sampleRate
);
