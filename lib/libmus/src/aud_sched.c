
/*************************************************************

  aud_sched.c : Nintendo 64 Music Tools Programmers Library
  (c) Copyright 1997/1998, Software Creations (Holdings) Ltd.

  Version 3.14

  Music library scheduler functions.

**************************************************************/

/* include configuartion */
#include "libmus_config.h"

/* include system headers */
#include <ultra64.h>
#include <sched.h>

/* include other header files */
#include "libmus.h"
#include "lib_memory.h"

/* include current header file */
#include "aud_sched.h"

/* internal macros */
#define QUEUE_SIZE	4

/* internal function prototypes */
static void __OsSchedInstall(void);
static void __OsSchedWaitFrame(void);
static void __OsSchedDoTask(musTask *task);

/* internal data structures */
typedef struct
{
	OSScClient	client;
	OSMesgQueue frame_queue;
	OSMesg      frame_messages[QUEUE_SIZE];
	OSMesgQueue	task_queue;
	OSMesg		task_messages[QUEUE_SIZE];
} ossched_workspace_t;

/* internal workspace */
static OSSched		*audio_sched;
static ossched_workspace_t *sched_mem;
static musSched	default_sched = { __OsSchedInstall, __OsSchedWaitFrame, __OsSchedDoTask };
musSched *__libmus_current_sched=&default_sched;


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [EXTERNAL FUNCTION]
  __MusIntSchedInit(sched)

  [Parameters]
  sched			adress of OSSched structure 

  [Explanation]
  Initialise scheduler support functions.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

void __MusIntSchedInit(void *sched)
{
	audio_sched = (OSSched *)sched;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [CALLBACK FUNCTION]
  __OsSchedInstall()

  [Explanation]
  Default scheduler 'install' function. Called once when audio thread first starts.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static void __OsSchedInstall(void)
{
	sched_mem = __MusIntMemMalloc(sizeof(ossched_workspace_t));
	osCreateMesgQueue(&sched_mem->frame_queue, &sched_mem->frame_messages[0], QUEUE_SIZE);
  	osCreateMesgQueue(&sched_mem->task_queue, &sched_mem->task_messages[0], QUEUE_SIZE);

   osScAddClient(audio_sched, &sched_mem->client, &sched_mem->frame_queue);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [CALLBACK FUNCTION]
  __OsSchedWaitFrame()

  [Explanation]
  Default scheduler 'waitframe' function. Called in audio thread mail loop to wait
  for vsync message.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static void __OsSchedWaitFrame(void)
{
   OSScMsg *message;

	do
	{
		osRecvMesg(&sched_mem->frame_queue, (OSMesg *)&message, OS_MESG_BLOCK);
		osRecvMesg(&sched_mem->frame_queue, NULL, OS_MESG_NOBLOCK);	/* bin any missed syncs! <- only happens if a higher priority thread takes a huge amount of time */
	} while (message->type!=OS_SC_RETRACE_MSG);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [EXTERNAL FUNCTION]
  __OsSchedDoTask(task)

  [Parameters]
  task			address of task descriptor structure

  [Explanation]
  Default scheduler 'dotask' function. Called to process the given task as an RSP
  task and wait for its completion.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static void __OsSchedDoTask(musTask *task)
{
   OSScTask t;
   OSScMsg message;

   t.next      = 0;
   t.msgQ      = &sched_mem->task_queue;
   t.msg       = &message;
   t.flags     = OS_SC_NEEDS_RSP;
    
   t.list.t.data_ptr    = task->data;
   t.list.t.data_size   = task->data_size;
   t.list.t.type  = M_AUDTASK;
   t.list.t.ucode_boot = (u64 *)rspbootTextStart;
   t.list.t.ucode_boot_size = ((int) rspbootTextEnd - (int) rspbootTextStart);
   t.list.t.flags  = 0;
   t.list.t.ucode = (u64 *) task->ucode;
   t.list.t.ucode_data = (u64 *) task->ucode_data;
   t.list.t.ucode_size = 4096;
   t.list.t.ucode_data_size = SP_UCODE_DATA_SIZE;
   t.list.t.dram_stack = (u64 *) NULL;
   t.list.t.dram_stack_size = 0;
   t.list.t.output_buff = (u64 *) NULL;
   t.list.t.output_buff_size = 0;
   t.list.t.yield_data_ptr = NULL;
   t.list.t.yield_data_size = 0;

   osSendMesg(osScGetCmdQ(audio_sched), (OSMesg) &t, OS_MESG_BLOCK);    
	osRecvMesg(&sched_mem->task_queue, NULL, OS_MESG_BLOCK);
}



/* end of file */
