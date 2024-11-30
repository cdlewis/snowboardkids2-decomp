
/*************************************************************

  aud_thread.c : Nintendo 64 Music Tools Programmers Library
  (c) Copyright 1997/1998, Software Creations (Holdings) Ltd.

  Version 3.14

  Music library thread base audio manager.

**************************************************************/

/* include configuartion */
#include "libmus_config.h"

/* include system headers */
#include <ultra64.h>
#ifndef SUPPORT_NAUDIO
#  include <libaudio.h>
#else
#  include <n_libaudio_sc.h>
#  include <n_libaudio_sn_sc.h>
#endif

/* include other header files */
#include "libmus.h"
#include "lib_memory.h"
#include "aud_sched.h"
#include "aud_dma.h"
#include "aud_samples.h"

#ifdef SUPPORT_FXCHANGE
#  include "player_fx.h"
#endif

/* include current header file */
#include "aud_thread.h"

/* internal macros */
#define AUDIO_STACKSIZE    0x2000     /* size of stack for thread (in bytes) */
#define EXTRA_SAMPLES      30         /* ratio[%] of extra samples per frame */
#define EXTRA_SAMPLES_N    15         /* ratio[%] of extra eamples (if n_audio) */
#define NUM_OUTPUT_BUFFERS 3          /* number of output buffers */

#ifndef SUPPORT_NAUDIO
#  define MICROCODE_CODE  aspMainTextStart
#  define MICROCODE_DATA  aspMainDataStart
#else
#  define MICROCODE_CODE  n_aspMainTextStart
#  define MICROCODE_DATA  n_aspMainDataStart
#endif

/* internal data structures */
typedef struct
{
   short *data;
   int frame_samples;
} audio_task_t;

/* internal function prototypes */
static void __MusIntThreadProcess(void *ignored);

/* internal workspace */
static audio_task_t *audio_tasks;
static Acmd *audio_command_list;

/* global workspace */
ALGlobals   __libmus_alglobals;

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [EXTERNAL FUNCTION]
  __MusIntAudManInit(config, vsyncs_per_second, fx_type)

  [Parameters]
  config       address of library configuration structure
  vsyncs_per_second  number of video refreshes per seconds
  fx_type      audio library effect type

  [Explanation]
  Initialise audio thread and all audio managment functionality.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

void   __MusIntAudManInit(musConfig *config, int vsyncs_per_second, int fx_type)
{
   static OSThread thread;
   static u64 *stack_addr;
   u32 i;
   ALSynConfig syn_config;
   u32 extra_rate;
   u32 samples_per_frame;

   /* setup syn configuartion structure */
   syn_config.maxVVoices = syn_config.maxPVoices = config->channels;
   syn_config.maxUpdates = config->syn_updates;
   syn_config.dmaproc = __MusIntDmaInit(config->syn_num_dma_bufs, config->syn_dma_buf_size);
   syn_config.fxType = fx_type;
   syn_config.outputRate = osAiSetFrequency(config->syn_output_rate);
   syn_config.heap       = __MusIntMemGetHeapAddr();

   /* initialise syn driver */
   alInit(&__libmus_alglobals, &syn_config);

   /* initialise sample buffer manager */
#ifndef SUPPORT_NAUDIO
   extra_rate = EXTRA_SAMPLES;
#else
   extra_rate = EXTRA_SAMPLES_N;
#endif

   samples_per_frame = __MusIntSamplesInit((u32)config->syn_retraceCount, (u32)syn_config.outputRate, (u32)vsyncs_per_second, extra_rate);

   /* allocate audio command list */
   audio_command_list = (Acmd*)__MusIntMemMalloc(config->syn_rsp_cmds*sizeof(Acmd));

   /* initialise task control structures */
   audio_tasks = __MusIntMemMalloc(NUM_OUTPUT_BUFFERS*sizeof(audio_task_t));
   for (i=0; i<NUM_OUTPUT_BUFFERS; i++)
      audio_tasks[i].data = __MusIntMemMalloc(4*samples_per_frame);

   /* create and start thread */
   stack_addr = __MusIntMemMalloc(AUDIO_STACKSIZE);
   osCreateThread(&thread, 3, __MusIntThreadProcess, 0,
                   (void *)(stack_addr+AUDIO_STACKSIZE/sizeof(u64)), config->thread_priority);
   osStartThread(&thread);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [INTERNAL FUNCTION]
  __MusIntThreadProcess(ignored)

  [Parameters]
  ignored      ignored

  [Explanation]
  Audio thread. Loop forever process sound data and generating output.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static void __MusIntThreadProcess(void *ignored)
{
   static audio_task_t *last_task=NULL;
   Acmd *cmdp;
   s32 commands;
   u32 task_count;
   u32 samples;
   u32 status;
   audio_task_t *task;
   musTask sched_task;
#ifdef _AUDIODEBUG
   u32 last_samples=0;
#endif

   /* initialise */
   sched_task.ucode = (u64 *) MICROCODE_CODE;
   sched_task.ucode_data = (u64 *) MICROCODE_DATA;
   sched_task.data = (u64 *)audio_command_list;
   task_count = 0;

   /* initialise scheduler functionality */
   __MusIntSched_install();

   /* infinite loop */
   while (1) 
   {
      /* wait for frame sync message */
      __MusIntSched_waitframe();

      /* get AI parameters */
      status = osAiGetStatus();
      samples = osAiGetLength()>>2;

      /* check AI status */
      if(status & AI_STATUS_FIFO_FULL)
         continue;

      /* process dma buffers (find free ones) */
      __MusIntDmaProcess();

      /* set relevant output buffer */
      if (last_task && commands)
         osAiSetNextBuffer(last_task->data, last_task->frame_samples<<2);

      /* call driver to generate audio commands and download samples */
      task = &audio_tasks[task_count];
      task->frame_samples = __MusIntSamplesCurrent(samples);
      cmdp = alAudioFrame(audio_command_list, &commands, (short *)osVirtualToPhysical(task->data), task->frame_samples);
      /* start audio task if required */
      if(commands)
      {
         sched_task.data_size = (cmdp - audio_command_list) * sizeof(Acmd);
         __MusIntSched_dotask(&sched_task);
#ifdef _AUDIODEBUG
         if (last_task && !samples && last_samples)
            osSyncPrintf("AUD_THREAD.C : libaudio out of samples.\n");
	 last_samples = samples;
#endif
         last_task = task;
      }
      task_count = (task_count+1) % NUM_OUTPUT_BUFFERS;
   }
}

/* end of file */
