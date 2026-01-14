#include "common.h"

typedef struct {
    u32 control_flag;
    u32 channels;
    u32 thread_priority;
    u8 *heap;
    s32 heap_length;
    u8 *ptr;
    u8 *wbk;
    void *sched;
    void *default_fxbank;
    s32 fifo_length;
    s32 syn_updates;
    s32 syn_output_rate;
    s32 syn_rsp_cmds;
    s32 syn_retraceCount;
    s32 syn_num_dma_bufs;
    s32 syn_dma_buf_size;
    OSPiHandle *diskrom_handle;
} musConfig;

s32 MusInitialize(musConfig *config);

void initMusicPtrBank(void *pbank, void *wbank);

s32 setFrequencyOffsetByHandle(void *, f32);

s32 setReverbScaleByHandle(void *, s32);

s32 setPanScaleByHandle(void *, s32);

s32 setTempoScaleByHandle(void *, s32);

s32 setVolumeScaleByHandle(void *, s32);

void stopChannelsByFlags(s32 channelFlags, s32 stopSpeed);

s32 stopChannelsByHandle(u32 handle, s32 stopSpeed);

s32 getActiveChannelCount(u32 flags);
