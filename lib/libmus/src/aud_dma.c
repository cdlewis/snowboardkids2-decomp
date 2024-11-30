
/*************************************************************

  aud_dma.c : Nintendo 64 Music Tools Programmers Library
  (c) Copyright 1997/1998, Software Creations (Holdings) Ltd.

  Version 3.14

  Music library DMA buffer manager.

**************************************************************/

/* include configuartion */
#include "libmus_config.h"

/* include system headers */
#include <ultra64.h>
#ifndef SUPPORT_NAUDIO
  #include <libaudio.h>
#else
  #include <n_libaudio_sc.h>
  #include <n_libaudio_sn_sc.h>
#endif

/* include other header files */
#include "libmus.h"
#include "lib_memory.h"

/* include current header file */
#include "aud_dma.h"

/* internal macros */
#define SAMPLE_ALIGNMENT      2			/* must be power of two */
#define FRAME_LAG				   1			/* number of frames before buffer is purged */
#define DDROM_WAVEDATA_START	0x00140000	/* address of N64DD ROM samples - from <leo.h> */

extern unsigned long __muscontrol_flag;

/* internal data structures */
typedef struct dma_list_s
{
   struct dma_list_s *prev;
   struct dma_list_s *next;
   int               keep_count;
   unsigned long     sample_addr;
   unsigned char     *ram_addr;
} dma_list_t;

/* internal workspace */
static dma_list_t *dma_buffer_head;    /* first used DMA buffer descriptor */
static dma_list_t *dma_buffer_free;    /* first unused DMA buffer descriptor */
static dma_list_t *dma_buffer_list;    /* DMA buffer descriptor list */

/* dma message buffers */
static OSIoMesg   *audio_IO_mess_buf;
static OSMesg     *audio_mess_buf;

static int        audio_dma_size;
static int        audio_dma_count;

/* internal function prototypes */
static ALDMAproc __CallBackDmaNew(void *ignored);
static s32 __CallBackDmaProcess(s32 addr, s32 len, void *ignored);
static dma_list_t *__MusIntDmaSample(unsigned long sample_addr, int sample_length);


/** Queues and storage for use with audio DMA's ****/
static OSMesgQueue     audDMAMessageQ;

/* EPI handles for ROM access */
static OSPiHandle *cartrom_handle;
/*static*/ OSPiHandle *diskrom_handle;


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [EXTERNAL FUNCTION]
  __MusIntDmaInit(dma_buffer_count, dma_buffer_size)

  [Parameters]
  dma_buffer_count	number of DMA buffers
  dma_buffer_size	size of each DMA buffer

  [Explanation]
  Initialise DMA buffer manager.

  [Return value]
  Address of audio library DMA initialisation function
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

ALDMANew __MusIntDmaInit(int dma_buffer_count, int dma_buffer_size)
{
	int i;

	/* get ROM access handles */
	cartrom_handle = osCartRomInit();
// Go to MusInitailize()
//	diskrom_handle = osDriveRomInit();

	/* allocate memory for DMA messages */
	audio_IO_mess_buf = __MusIntMemMalloc(dma_buffer_count*2*sizeof(OSIoMesg));
	audio_mess_buf = __MusIntMemMalloc(dma_buffer_count*2*sizeof(OSMesg));
	/* allocate memory for DMA buffer descriptor list */
	dma_buffer_list = __MusIntMemMalloc(dma_buffer_count*sizeof(dma_list_t));
	__MusIntMemSet(dma_buffer_list, 0, dma_buffer_count*sizeof(dma_list_t));

	/* initialise descriptor list and allocate DMA buffers */
	for (i=0; i<dma_buffer_count-1; i++)
	{
		dma_buffer_list[i].next = dma_buffer_list+i+1;
		dma_buffer_list[i+1].prev = dma_buffer_list+i;
		dma_buffer_list[i].ram_addr = __MusIntMemMalloc(dma_buffer_size);
		dma_buffer_list[i].sample_addr = 0xffffffff;
	}
	/* allocate last DMA buffer */
	dma_buffer_list[i].ram_addr = __MusIntMemMalloc(dma_buffer_size);
	dma_buffer_list[i].sample_addr = 0xffffffff;

	/* set working vars */
	audio_dma_size = dma_buffer_size;
	audio_dma_count = 0;
	dma_buffer_head = NULL;
	dma_buffer_free = dma_buffer_list;

	/* create DMA message queue */
	osCreateMesgQueue(&audDMAMessageQ, audio_mess_buf, dma_buffer_count*2);

	/* return address of audio library DMA new function */
	return (__CallBackDmaNew);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [EXTERNAL FUNCTION]
  __MusIntDmaProcess()

  [Explanation]
  Process the DMA buffers. The DMA message queue is emptied, DMA buffers are aged
  and if necessary freed.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

void __MusIntDmaProcess(void)
{
   OSIoMesg     *iomsg;
	dma_list_t *current_dma_buffer;
    
	/* empty message queue */
   while (audio_dma_count)
    {
#ifndef _AUDIODEBUG
        osRecvMesg(&audDMAMessageQ,(OSMesg *)&iomsg,OS_MESG_NOBLOCK);
#else
        if (osRecvMesg(&audDMAMessageQ,(OSMesg *)&iomsg,OS_MESG_NOBLOCK) == -1)
            osSyncPrintf("AUD_DMA.C: Audio DMA not completed.\n");
#endif
    	audio_dma_count--;
    }
    /* free any DMA buffers not in use */
	current_dma_buffer = dma_buffer_head;
	while(current_dma_buffer)
	{
		current_dma_buffer->keep_count--;
		if (!current_dma_buffer->keep_count)
		{
			dma_list_t *next_buffer;

			next_buffer = current_dma_buffer->next;
			if (current_dma_buffer->next)
				current_dma_buffer->next->prev = current_dma_buffer->prev;
			if (current_dma_buffer->prev)
				current_dma_buffer->prev->next = current_dma_buffer->next;
			else
				dma_buffer_head = current_dma_buffer->next;

			current_dma_buffer->prev = NULL;
			current_dma_buffer->next = dma_buffer_free;
			dma_buffer_free = current_dma_buffer;
			current_dma_buffer=next_buffer;
		}
		else
		{
			current_dma_buffer=current_dma_buffer->next;
		}
	}
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [CALLBACK FUNCTION]
  __CallBackDmaNew(ignored)

  [Parameters]
  ingored		ignored

  [Explanation]
  Audio library DMA processing initialisation function which simply returns the
  address of the normal DMA callback function.

  [Return value]
  Address of audio library DMA processing function
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static ALDMAproc __CallBackDmaNew(void *ignored)
{
	return (__CallBackDmaProcess);
}    

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [CALLBACK FUNCTION]
  __CallBackDmaProcess(addr, len, ignored)

  [Parameters]
  addr			address of sample required
  len			   length of sample required
  ingored		ignored

  [Explanation]
  Audio library DMA processing function. Find the requested sample in RAM and return
  its address.

  [Return value]
  RAM address of sample
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static s32 __CallBackDmaProcess(s32 addr, s32 len, void *ignored)
{
	dma_list_t *buffer;
	unsigned char *ram_addr;;

	buffer = __MusIntDmaSample(addr, len);
	if (!buffer)
		return (osVirtualToPhysical((void *)addr));

	if ((addr&0xff000000)==0xff000000)
	{
		addr &= 0xffffff;
		addr += DDROM_WAVEDATA_START;
	}

	ram_addr = buffer->ram_addr+(u32)addr-buffer->sample_addr;
	return (osVirtualToPhysical(ram_addr));
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [INTERNAL FUNCTION]
  __MusIntDmaSample(sample_addr, sample_length)

  [Parameters]
  sample_addr	address of sample required
  sample_length	length of sample required

  [Explanation]
  Find requested sample in current DMA buffer or download requested sample into a
  new DMA buffer.

  [Return value]
  Address of dma buffer containing sample
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static dma_list_t *__MusIntDmaSample(unsigned long sample_addr, int sample_length)
{
	dma_list_t *current_dma_buffer, *free_buffer, *last_buffer;
	unsigned long sample_addr_end;
	OSPiHandle	*pi_handle;
	OSIoMesg	*io_msg;

	/* test if sample is coming from the N64DD ROM */
	if ((sample_addr&0xff000000)==0xff000000)
	{
		pi_handle = diskrom_handle;
		sample_addr &= 0xffffff;
		sample_addr += DDROM_WAVEDATA_START;
	}
	else
	{
		if (__muscontrol_flag&MUSCONTROL_RAM)
		    return (NULL);
		pi_handle = cartrom_handle;
	}

	/* get address of sample end */
	sample_addr_end = sample_addr+sample_length;
	/* find sample if already downloaded */
	last_buffer = NULL;
	for (current_dma_buffer=dma_buffer_head; current_dma_buffer; current_dma_buffer=current_dma_buffer->next)
	{
		/* have we gone past it? */
		if (sample_addr<current_dma_buffer->sample_addr)
			break;
		/* is it contained in this buffer? */
		if(sample_addr_end<=current_dma_buffer->sample_addr+audio_dma_size)
		{
			/* modification 99.01.12 from Yutaka Murakami v3.13 */
			current_dma_buffer->keep_count = 1+FRAME_LAG;
			return (current_dma_buffer);
		}

		last_buffer = current_dma_buffer;
	}
	/* get next free buffer */
	free_buffer = dma_buffer_free;
	if (!free_buffer)
	{
#ifdef _AUDIODEBUG
		osSyncPrintf("AUD_DMA.C: Cannot find free DMA buffer.\n");
#endif
		return (dma_buffer_head);
	}
	/* free buffer is to be used so move it along */
	dma_buffer_free = free_buffer->next;

	/* insert free buffer into list */
	if (last_buffer)
	{
		/* link free with next and fix next */
		free_buffer->next = last_buffer->next;
		if (free_buffer->next)
			free_buffer->next->prev = free_buffer;
		/* line free with last and fix last */
		free_buffer->prev = last_buffer;
		last_buffer->next = free_buffer;
	}
	else
	{
		free_buffer->next = dma_buffer_head;
		free_buffer->prev = NULL;
		if (dma_buffer_head)
			dma_buffer_head->prev = free_buffer;
		dma_buffer_head = free_buffer;
	}

	/* DMA sample to buffer */
	free_buffer->sample_addr = sample_addr&(~(SAMPLE_ALIGNMENT-1));
	free_buffer->keep_count = 1+FRAME_LAG;

	io_msg = &audio_IO_mess_buf[audio_dma_count++];
	io_msg->hdr.pri = OS_MESG_PRI_NORMAL;
	io_msg->hdr.retQueue = &audDMAMessageQ;
	io_msg->dramAddr = free_buffer->ram_addr;
	io_msg->devAddr = free_buffer->sample_addr;
	io_msg->size = audio_dma_size;
	osEPiStartDma(pi_handle, io_msg, OS_READ);
	/* return dma buffer address */
	return (free_buffer);
}

/* end of file */
