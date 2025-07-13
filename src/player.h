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