#include "common.h"

typedef struct AudioParams_s {
    u32 syn_output_rate;
    s32 sample_rate;
    s32 syn_rsp_cmds;
} AudioParams;

void initAudioManager(
    ALSynConfig *config,
    OSId id,
    AudioParams *audioParams,
    s32 maxChannels,
    s32 maxVoices,
    s32 sampleRate
);