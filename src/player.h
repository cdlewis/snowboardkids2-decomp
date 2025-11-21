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

void func_80072CC0_738C0(void *pbank, void *wbank);

void func_80072C38_73838(void *, s32);

void func_80072B3C_7373C(void *, f32);

void func_80072ACC_736CC(void *, s32);

void func_80072A6C_7366C(void *, s32);

void func_80072814_73414(s32, s32);
